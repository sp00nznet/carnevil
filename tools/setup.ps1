<#
CarnEvil Recompiled setup: from a plain download and your own CarnEvil v1.0.1 files to a launcher.

    Setup.cmd                                              double-click; asks before installing anything
    powershell -File tools\setup.ps1 -Yes -Game "D:\mame"  no questions (recomp-launcher uses this)

-Game is a folder holding the boot ROM carnevil1_9.u32 and the DCS2 ROM sound102.u95 (loose, or
inside MAME's carnevil.zip / carnevil1.zip) and the hard disk carnevi1.chd (or an extracted
carnevi1.img). Everything stays on this machine; nothing from the game is downloaded. Steps already
done are skipped, so a rerun after a failure resumes. The full log is logs\setup.log.
#>
param(
    [switch]$Yes,          # install missing prerequisites without asking
    [string]$Game = ""     # folder holding the game files (default: the repo folder)
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
Set-Location $Root
New-Item -ItemType Directory -Force logs, deps | Out-Null
$Log = Join-Path $Root "logs\setup.log"
"CarnEvil Recompiled setup, $(Get-Date)" | Out-File $Log -Encoding utf8

# The recompiler, pinned to the commit this release was tested with.
$MidwayRecomp = @{ Repo = "https://github.com/sp00nznet/MidwayRecomp"; Ref = "023ddb3" }
$Steps = 7

function Step($n, $what) { $line = "[$n/$Steps] $what"; Write-Host $line; $line | Out-File $Log -Append -Encoding utf8 }
function Stop-Setup($why) {
    Write-Host ""
    Write-Host "Setup stopped: $why" -ForegroundColor Red
    Write-Host "Details are in $Log"
    "Setup stopped: $why" | Out-File $Log -Append -Encoding utf8
    if (-not $Yes -and -not $env:RECOMP_LAUNCHER) { Read-Host "Press Enter to close" | Out-Null }
    exit 1
}
# Runs a native command with its output in the log; stops with $why if it fails.
function Run($why, $exe, [string[]]$arguments) {
    "> $exe $($arguments -join ' ')" | Out-File $Log -Append -Encoding utf8
    $old = $ErrorActionPreference
    $ErrorActionPreference = "Continue"   # native stderr isn't an error; only the exit code counts
    $global:LASTEXITCODE = 0
    & $exe @arguments 2>&1 | ForEach-Object { "$_" } | Out-File $Log -Append -Encoding utf8
    $code = $LASTEXITCODE
    $ErrorActionPreference = $old
    if ($code -ne 0) { Stop-Setup $why }
}
function Ask($question) {
    if ($Yes) { return $true }
    return (Read-Host "$question [Y/n]") -notmatch '^[nN]'
}
function Refresh-Path {
    $env:PATH = [Environment]::GetEnvironmentVariable("PATH", "Machine") + ";" + [Environment]::GetEnvironmentVariable("PATH", "User")
}
function Find-VS {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswhere)) { return $null }
    return & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
}
function Find-VSGenerator {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $v = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property catalog_productLineVersion
    if ($v -eq "2019") { return "Visual Studio 16 2019" }
    return "Visual Studio 17 2022"
}

# ---------------------------------------------------------------------------------------------
Step 1 "Checking the tools it needs"
$need = @()
if (-not (Get-Command git -ErrorAction SilentlyContinue))   { $need += @{ Name = "Git"; Id = "Git.Git"; Size = "about 300 MB" } }
if (-not (Get-Command py -ErrorAction SilentlyContinue))    { $need += @{ Name = "Python 3"; Id = "Python.Python.3.12"; Size = "about 100 MB" } }
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) { $need += @{ Name = "CMake"; Id = "Kitware.CMake"; Size = "about 100 MB" } }
if (-not (Find-VS)) { $need += @{ Name = "Visual Studio 2022 Build Tools (C++)"; Id = "Microsoft.VisualStudio.2022.BuildTools"; Size = "about 3 GB";
    Extra = @("--override", "--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --passive --wait") } }
foreach ($t in $need) {
    if (-not (Get-Command winget -ErrorAction SilentlyContinue)) {
        Stop-Setup "$($t.Name) is missing and winget isn't available to install it. Install $($t.Name) yourself, then run Setup again."
    }
    if (-not (Ask "$($t.Name) is needed to build the game and isn't installed. Install it with winget ($($t.Size))?")) {
        Stop-Setup "$($t.Name) is needed. Install it, then run Setup again."
    }
    Run "installing $($t.Name) failed" "winget" (@("install", "--id", $t.Id, "-e", "--accept-source-agreements", "--accept-package-agreements") + $t.Extra)
    Refresh-Path
}
if (-not (Find-VS)) { Stop-Setup "Visual Studio's C++ build tools didn't install. Install the 'Desktop development with C++' workload, then run Setup again." }
$Gen = Find-VSGenerator

# ---------------------------------------------------------------------------------------------
Step 2 "Finding your CarnEvil files"
$dirs = @($Root) + $(if ($Game) { @($Game) } else { @() })
# A file loose in a game folder, or else inside a zip there (MAME's set); copied to the repo root.
function Get-GameFile($name, $dest) {
    if (Test-Path $dest) { return $true }
    foreach ($d in $dirs) {
        $f = Get-ChildItem -Path $d -File -Recurse -Depth 2 -Filter $name -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($f) { Copy-Item $f.FullName $dest -Force; return $true }
    }
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    foreach ($d in $dirs) {
        foreach ($z in Get-ChildItem -Path $d -File -Recurse -Depth 2 -Filter *.zip -ErrorAction SilentlyContinue) {
            try { $zip = [IO.Compression.ZipFile]::OpenRead($z.FullName) } catch { continue }
            try {
                $e = $zip.Entries | Where-Object { $_.Name -eq $name } | Select-Object -First 1
                if ($e) { [IO.Compression.ZipFileExtensions]::ExtractToFile($e, $dest, $true); return $true }
            } finally { $zip.Dispose() }
        }
    }
    return $false
}
if (-not (Get-GameFile "carnevil1_9.u32" (Join-Path $Root "carnevil1_9.u32"))) {
    Stop-Setup "The boot ROM carnevil1_9.u32 wasn't found (loose or in carnevil.zip). Pick the folder that holds your CarnEvil files, then run Setup again."
}
if (-not (Get-GameFile "sound102.u95" (Join-Path $Root "sound102.u95"))) {
    Stop-Setup "The sound ROM sound102.u95 wasn't found (loose or in carnevil.zip). Pick the folder that holds your CarnEvil files, then run Setup again."
}
New-Item -ItemType Directory -Force carnevil | Out-Null
$img = Join-Path $Root "carnevil\carnevi1.img"
$chd = $null
if (-not (Test-Path $img) -and -not (Get-GameFile "carnevi1.img" $img)) {
    foreach ($d in $dirs) {
        $chd = Get-ChildItem -Path $d -File -Recurse -Depth 2 -Filter carnevi1.chd -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($chd) { break }
    }
    if (-not $chd) { Stop-Setup "The hard disk carnevi1.chd (CarnEvil v1.0.1) wasn't found. Pick the folder that holds your CarnEvil files, then run Setup again." }
    Write-Host "  disk: $($chd.FullName)"
}

# ---------------------------------------------------------------------------------------------
Step 3 "Unpacking the hard disk and checking it is v1.0.1"
if ($chd) {
    Run "unpacking carnevi1.chd failed (is it MAME's CHD v5 of CarnEvil?)" "py" @("-3", "tools\chd_extract.py", $chd.FullName, $img)
}
Run "these files aren't CarnEvil v1.0.1 (MAME's carnevil1 with disk carnevi1); see the log for which one" "py" @("-3", "tools\prepare_inputs.py", "carnevil1_9.u32", $img)

# ---------------------------------------------------------------------------------------------
Step 4 "Getting the recompiler"
$mr = "deps\MidwayRecomp"
if (-not (Test-Path "$mr\.git")) {
    Run "downloading MidwayRecomp failed (no internet, or see the log)" "git" @("clone", "-q", $MidwayRecomp.Repo, $mr)
}
Run "updating MidwayRecomp failed (no internet, or see the log)" "git" @("-C", $mr, "fetch", "-q", "origin")
Run "updating MidwayRecomp failed" "git" @("-C", $mr, "checkout", "-q", $MidwayRecomp.Ref)
Run "updating MidwayRecomp failed" "git" @("-C", $mr, "-c", "core.longpaths=true", "submodule", "update", "-q", "--init", "--recursive")

# ---------------------------------------------------------------------------------------------
Step 5 "Building the recompiler"
$recomp = Join-Path $Root "$mr\build\Release\MidwayRecomp.exe"
Run "configuring MidwayRecomp failed" "cmake" @("-S", $mr, "-B", "$mr\build", "-G", $Gen, "-A", "x64")
Run "building MidwayRecomp failed" "cmake" @("--build", "$mr\build", "--config", "Release", "--target", "MidwayRecompCLI", "--parallel")

# ---------------------------------------------------------------------------------------------
Step 6 "Recompiling the game and the BIOS, then building (10 to 20 minutes)"
New-Item -ItemType Directory -Force recomp_out\funcs, recomp_out\rtos_funcs | Out-Null
foreach ($c in "carnevil", "rtos") { Run "recompiling $c failed" $recomp @("${c}_recomp.toml") }
Run "configuring CarnEvil failed" "cmake" @("-S", ".", "-B", "build", "-G", $Gen, "-A", "x64")
Run "building CarnEvil failed" "cmake" @("--build", "build", "--config", "Release", "--target", "carnevil", "--parallel")
if (-not (Test-Path "build\Release\carnevil.exe")) { Stop-Setup "the build finished but build\Release\carnevil.exe is missing" }

# ---------------------------------------------------------------------------------------------
Step 7 "Making the launcher"
"@echo off`r`ncd /d `"%~dp0`"`r`nstart `"`" `"build\Release\carnevil.exe`" %*`r`n" | Out-File (Join-Path $Root "CarnEvil (recomp).cmd") -Encoding ascii
Write-Host "  CarnEvil (recomp).cmd"
Write-Host ""
Write-Host "Done. Double-click 'CarnEvil (recomp).cmd' to play."
if (-not $Yes -and -not $env:RECOMP_LAUNCHER) { Read-Host "Press Enter to close" | Out-Null }
