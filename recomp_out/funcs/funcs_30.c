#include "recomp.h"
#include "funcs.h"

RECOMP_FUNC void func_80141190(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80141190: addu        $a2, $a2, $a0
    ctx->r6 = ADD32(ctx->r6, ctx->r4);
    // 0x80141194: andi        $t0, $a0, 0x3
    ctx->r8 = ctx->r4 & 0X3;
L_80141198:
    // 0x80141198: beq         $t0, $zero, L_801411B0
    if (ctx->r8 == 0) {
        // 0x8014119C: nop
    
            goto L_801411B0;
    }
    // 0x8014119C: nop

    // 0x801411A0: sb          $a1, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r5;
    // 0x801411A4: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x801411A8: bne         $a0, $a2, L_80141198
    if (ctx->r4 != ctx->r6) {
        // 0x801411AC: andi        $t0, $a0, 0x3
        ctx->r8 = ctx->r4 & 0X3;
            goto L_80141198;
    }
    // 0x801411AC: andi        $t0, $a0, 0x3
    ctx->r8 = ctx->r4 & 0X3;
L_801411B0:
    // 0x801411B0: addu        $t1, $a1, $zero
    ctx->r9 = ADD32(ctx->r5, 0);
    // 0x801411B4: sll         $t1, $t1, 8
    ctx->r9 = S32(ctx->r9 << 8);
    // 0x801411B8: or          $t1, $t1, $a1
    ctx->r9 = ctx->r9 | ctx->r5;
    // 0x801411BC: sll         $t1, $t1, 8
    ctx->r9 = S32(ctx->r9 << 8);
    // 0x801411C0: or          $t1, $t1, $a1
    ctx->r9 = ctx->r9 | ctx->r5;
    // 0x801411C4: sll         $t1, $t1, 8
    ctx->r9 = S32(ctx->r9 << 8);
    // 0x801411C8: or          $t1, $t1, $a1
    ctx->r9 = ctx->r9 | ctx->r5;
L_801411CC:
    // 0x801411CC: subu        $t0, $a2, $a0
    ctx->r8 = SUB32(ctx->r6, ctx->r4);
    // 0x801411D0: addi        $t2, $t0, -0x4
    ctx->r10 = ADD32(ctx->r8, -0X4);
    // 0x801411D4: bltz        $t2, L_801411E8
    if (SIGNED(ctx->r10) < 0) {
        // 0x801411D8: nop
    
            goto L_801411E8;
    }
    // 0x801411D8: nop

    // 0x801411DC: sw          $t1, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r9;
    // 0x801411E0: j           L_801411CC
    // 0x801411E4: addi        $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
        goto L_801411CC;
    // 0x801411E4: addi        $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
L_801411E8:
    // 0x801411E8: beq         $t0, $zero, L_80141204
    if (ctx->r8 == 0) {
        // 0x801411EC: nop
    
            goto L_80141204;
    }
    // 0x801411EC: nop

L_801411F0:
    // 0x801411F0: sb          $a1, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r5;
    // 0x801411F4: addi        $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x801411F8: slt         $at, $a0, $a2
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x801411FC: bne         $at, $zero, L_801411F0
    if (ctx->r1 != 0) {
        // 0x80141200: nop
    
            goto L_801411F0;
    }
    // 0x80141200: nop

L_80141204:
    // 0x80141204: jr          $ra
    // 0x80141208: nop

    return;
    // 0x80141208: nop

;}
RECOMP_FUNC void func_80141220(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80141220: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80141224: addiu       $a1, $sp, 0x1C
    ctx->r5 = ADD32(ctx->r29, 0X1C);
    // 0x80141228: lui         $a2, 0x801A
    ctx->r6 = S32(0X801A << 16);
    // 0x8014122C: addiu       $a2, $a2, 0x1DD8
    ctx->r6 = ADD32(ctx->r6, 0X1DD8);
    // 0x80141230: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80141234: jal         0x801412BC
    // 0x80141238: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_801412BC(rdram, ctx);
        goto after_0;
    // 0x80141238: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x8014123C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80141240: lw          $a0, 0x1DE8($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1DE8);
    // 0x80141244: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
    // 0x80141248: andi        $a0, $a0, 0x80
    ctx->r4 = ctx->r4 & 0X80;
    // 0x8014124C: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80141250: movn        $v0, $v1, $a0
    if (ctx->r4 != 0) ctx->r2 = ctx->r3;
    // 0x80141254: jr          $ra
    // 0x80141258: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x80141258: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8014126C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014126C: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80141270: lui         $v1, 0x7FFF
    ctx->r3 = S32(0X7FFF << 16);
    // 0x80141274: ori         $v1, $v1, 0xFFFF
    ctx->r3 = ctx->r3 | 0XFFFF;
    // 0x80141278: addiu       $v0, $zero, 0x110
    ctx->r2 = ADD32(0, 0X110);
    // 0x8014127C: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x80141280: sw          $a0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r4;
    // 0x80141284: addu        $a0, $a1, $zero
    ctx->r4 = ADD32(ctx->r5, 0);
    // 0x80141288: addiu       $a1, $sp, 0x40
    ctx->r5 = ADD32(ctx->r29, 0X40);
    // 0x8014128C: addiu       $a2, $sp, 0x10
    ctx->r6 = ADD32(ctx->r29, 0X10);
    // 0x80141290: sw          $ra, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r31;
    // 0x80141294: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x80141298: jal         0x801412BC
    // 0x8014129C: sw          $v1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r3;
    func_801412BC(rdram, ctx);
        goto after_0;
    // 0x8014129C: sw          $v1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r3;
    after_0:
    // 0x801412A0: lw          $v1, 0x14($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X14);
    // 0x801412A4: lw          $ra, 0x30($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X30);
    // 0x801412A8: sb          $zero, 0x0($v1)
    MEM_B(0X0, ctx->r3) = 0;
    // 0x801412AC: jr          $ra
    // 0x801412B0: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x801412B0: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_801412BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801412BC: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x801412C0: sw          $s0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r16;
    // 0x801412C4: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x801412C8: sw          $s5, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r21;
    // 0x801412CC: addu        $s5, $a1, $zero
    ctx->r21 = ADD32(ctx->r5, 0);
    // 0x801412D0: sw          $s6, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r22;
    // 0x801412D4: addu        $s6, $a2, $zero
    ctx->r22 = ADD32(ctx->r6, 0);
    // 0x801412D8: sw          $ra, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r31;
    // 0x801412DC: sw          $fp, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r30;
    // 0x801412E0: sw          $s7, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r23;
    // 0x801412E4: sw          $s4, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r20;
    // 0x801412E8: sw          $s3, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r19;
    // 0x801412EC: sw          $s2, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r18;
    // 0x801412F0: jal         0x80144448
    // 0x801412F4: sw          $s1, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r17;
    func_80144448(rdram, ctx);
        goto after_0;
    // 0x801412F4: sw          $s1, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r17;
    after_0:
    // 0x801412F8: lw          $v0, 0x4($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X4);
    // 0x801412FC: lw          $v1, 0x10($s6)
    ctx->r3 = MEM_W(ctx->r22, 0X10);
    // 0x80141300: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x80141304: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80141308: sb          $v0, 0x1AC0($at)
    MEM_B(0X1AC0, ctx->r1) = ctx->r2;
    // 0x8014130C: andi        $v0, $v1, 0x200
    ctx->r2 = ctx->r3 & 0X200;
    // 0x80141310: beq         $v0, $zero, L_80141324
    if (ctx->r2 == 0) {
        // 0x80141314: ori         $v0, $v1, 0x10
        ctx->r2 = ctx->r3 | 0X10;
            goto L_80141324;
    }
    // 0x80141314: ori         $v0, $v1, 0x10
    ctx->r2 = ctx->r3 | 0X10;
    // 0x80141318: addiu       $v1, $zero, -0x49
    ctx->r3 = ADD32(0, -0X49);
    // 0x8014131C: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x80141320: sw          $v0, 0x10($s6)
    MEM_W(0X10, ctx->r22) = ctx->r2;
L_80141324:
    // 0x80141324: lw          $v0, 0x10($s6)
    ctx->r2 = MEM_W(ctx->r22, 0X10);
    // 0x80141328: andi        $v0, $v0, 0x10
    ctx->r2 = ctx->r2 & 0X10;
    // 0x8014132C: bne         $v0, $zero, L_8014133C
    if (ctx->r2 != 0) {
        // 0x80141330: addu        $s7, $s0, $zero
        ctx->r23 = ADD32(ctx->r16, 0);
            goto L_8014133C;
    }
    // 0x80141330: addu        $s7, $s0, $zero
    ctx->r23 = ADD32(ctx->r16, 0);
    // 0x80141334: j           L_80141BD0
    // 0x80141338: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_80141BD0;
    // 0x80141338: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_8014133C:
    // 0x8014133C: lui         $t1, 0x8017
    ctx->r9 = S32(0X8017 << 16);
    // 0x80141340: addiu       $t1, $t1, 0x4968
    ctx->r9 = ADD32(ctx->r9, 0X4968);
    // 0x80141344: sw          $t1, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r9;
    // 0x80141348: sw          $zero, 0x28($sp)
    MEM_W(0X28, ctx->r29) = 0;
L_8014134C:
    // 0x8014134C: lbu         $a0, 0x0($s7)
    ctx->r4 = MEM_BU(ctx->r23, 0X0);
    // 0x80141350: lw          $s0, 0x0($s6)
    ctx->r16 = MEM_W(ctx->r22, 0X0);
    // 0x80141354: lw          $s1, 0x4($s6)
    ctx->r17 = MEM_W(ctx->r22, 0X4);
    // 0x80141358: beql        $a0, $zero, L_801413F4
    if (ctx->r4 == 0) {
        // 0x8014135C: sw          $s0, 0x0($s6)
        MEM_W(0X0, ctx->r22) = ctx->r16;
            goto L_801413F4;
    }
    goto skip_0;
    // 0x8014135C: sw          $s0, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r16;
    skip_0:
    // 0x80141360: addiu       $s3, $zero, 0x25
    ctx->r19 = ADD32(0, 0X25);
    // 0x80141364: addiu       $s2, $zero, 0xA
    ctx->r18 = ADD32(0, 0XA);
L_80141368:
    // 0x80141368: beql        $a0, $s3, L_801413F4
    if (ctx->r4 == ctx->r19) {
        // 0x8014136C: sw          $s0, 0x0($s6)
        MEM_W(0X0, ctx->r22) = ctx->r16;
            goto L_801413F4;
    }
    goto skip_1;
    // 0x8014136C: sw          $s0, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r16;
    skip_1:
    // 0x80141370: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80141374: bgez        $s0, L_8014139C
    if (SIGNED(ctx->r16) >= 0) {
        // 0x80141378: nop
    
            goto L_8014139C;
    }
    // 0x80141378: nop

    // 0x8014137C: lw          $v0, 0x10($s6)
    ctx->r2 = MEM_W(ctx->r22, 0X10);
    // 0x80141380: andi        $v0, $v0, 0x4
    ctx->r2 = ctx->r2 & 0X4;
    // 0x80141384: beq         $v0, $zero, L_801413B4
    if (ctx->r2 == 0) {
        // 0x80141388: negu        $v0, $s0
        ctx->r2 = SUB32(0, ctx->r16);
            goto L_801413B4;
    }
    // 0x80141388: negu        $v0, $s0
    ctx->r2 = SUB32(0, ctx->r16);
    // 0x8014138C: lw          $v1, 0xC($s6)
    ctx->r3 = MEM_W(ctx->r22, 0XC);
    // 0x80141390: slt         $v0, $v0, $v1
    ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x80141394: beq         $v0, $zero, L_801413B8
    if (ctx->r2 == 0) {
        // 0x80141398: addu        $a1, $s6, $zero
        ctx->r5 = ADD32(ctx->r22, 0);
            goto L_801413B8;
    }
    // 0x80141398: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
L_8014139C:
    // 0x8014139C: bnel        $a0, $s2, L_801413D4
    if (ctx->r4 != ctx->r18) {
        // 0x801413A0: sb          $a0, 0x0($s1)
        MEM_B(0X0, ctx->r17) = ctx->r4;
            goto L_801413D4;
    }
    goto skip_2;
    // 0x801413A0: sb          $a0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r4;
    skip_2:
    // 0x801413A4: lw          $v0, 0x10($s6)
    ctx->r2 = MEM_W(ctx->r22, 0X10);
    // 0x801413A8: andi        $v0, $v0, 0x4
    ctx->r2 = ctx->r2 & 0X4;
    // 0x801413AC: beql        $v0, $zero, L_801413D4
    if (ctx->r2 == 0) {
        // 0x801413B0: sb          $a0, 0x0($s1)
        MEM_B(0X0, ctx->r17) = ctx->r4;
            goto L_801413D4;
    }
    goto skip_3;
    // 0x801413B0: sb          $a0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r4;
    skip_3:
L_801413B4:
    // 0x801413B4: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
L_801413B8:
    // 0x801413B8: sw          $s0, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r16;
    // 0x801413BC: jal         0x80142A68
    // 0x801413C0: sw          $s1, 0x4($s6)
    MEM_W(0X4, ctx->r22) = ctx->r17;
    func_80142A68(rdram, ctx);
        goto after_1;
    // 0x801413C0: sw          $s1, 0x4($s6)
    MEM_W(0X4, ctx->r22) = ctx->r17;
    after_1:
    // 0x801413C4: lw          $s0, 0x0($s6)
    ctx->r16 = MEM_W(ctx->r22, 0X0);
    // 0x801413C8: lw          $s1, 0x4($s6)
    ctx->r17 = MEM_W(ctx->r22, 0X4);
    // 0x801413CC: j           L_801413DC
    // 0x801413D0: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
        goto L_801413DC;
    // 0x801413D0: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_801413D4:
    // 0x801413D4: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x801413D8: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_801413DC:
    // 0x801413DC: lw          $t1, 0x28($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X28);
    // 0x801413E0: lbu         $a0, 0x0($s7)
    ctx->r4 = MEM_BU(ctx->r23, 0X0);
    // 0x801413E4: addiu       $t1, $t1, 0x1
    ctx->r9 = ADD32(ctx->r9, 0X1);
    // 0x801413E8: bne         $a0, $zero, L_80141368
    if (ctx->r4 != 0) {
        // 0x801413EC: sw          $t1, 0x28($sp)
        MEM_W(0X28, ctx->r29) = ctx->r9;
            goto L_80141368;
    }
    // 0x801413EC: sw          $t1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r9;
    // 0x801413F0: sw          $s0, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r16;
L_801413F4:
    // 0x801413F4: bne         $a0, $zero, L_80141408
    if (ctx->r4 != 0) {
        // 0x801413F8: sw          $s1, 0x4($s6)
        MEM_W(0X4, ctx->r22) = ctx->r17;
            goto L_80141408;
    }
    // 0x801413F8: sw          $s1, 0x4($s6)
    MEM_W(0X4, ctx->r22) = ctx->r17;
L_801413FC:
    // 0x801413FC: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x80141400: j           L_80141BD0
    // 0x80141404: nop

        goto L_80141BD0;
    // 0x80141404: nop

L_80141408:
    // 0x80141408: addu        $s3, $zero, $zero
    ctx->r19 = ADD32(0, 0);
    // 0x8014140C: addiu       $s2, $zero, -0x1
    ctx->r18 = ADD32(0, -0X1);
    // 0x80141410: addu        $t0, $zero, $zero
    ctx->r8 = ADD32(0, 0);
    // 0x80141414: sw          $zero, 0x30($sp)
    MEM_W(0X30, ctx->r29) = 0;
    // 0x80141418: sw          $zero, 0x38($sp)
    MEM_W(0X38, ctx->r29) = 0;
    // 0x8014141C: sw          $zero, 0x40($sp)
    MEM_W(0X40, ctx->r29) = 0;
L_80141420:
    // 0x80141420: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_80141424:
    // 0x80141424: lbu         $v1, 0x0($s7)
    ctx->r3 = MEM_BU(ctx->r23, 0X0);
    // 0x80141428: sltiu       $v0, $v1, 0x79
    ctx->r2 = ctx->r3 < 0X79 ? 1 : 0;
    // 0x8014142C: beq         $v0, $zero, L_80141BB0
    if (ctx->r2 == 0) {
        // 0x80141430: sll         $v0, $v1, 2
        ctx->r2 = S32(ctx->r3 << 2);
            goto L_80141BB0;
    }
    // 0x80141430: sll         $v0, $v1, 2
    ctx->r2 = S32(ctx->r3 << 2);
    // 0x80141434: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80141438: addu        $at, $at, $v0
    gpr jr_addend_80141440 = ctx->r2;
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x8014143C: lw          $v0, 0x4990($at)
    ctx->r2 = ADD32(ctx->r1, 0X4990);
    // 0x80141440: jr          $v0
    // 0x80141444: nop

    switch (jr_addend_80141440 >> 2) {
        case 0: goto L_801413FC; break;
        case 1: goto L_80141BB0; break;
        case 2: goto L_80141BB0; break;
        case 3: goto L_80141BB0; break;
        case 4: goto L_80141BB0; break;
        case 5: goto L_80141BB0; break;
        case 6: goto L_80141BB0; break;
        case 7: goto L_80141BB0; break;
        case 8: goto L_80141BB0; break;
        case 9: goto L_80141BB0; break;
        case 10: goto L_80141BB0; break;
        case 11: goto L_80141BB0; break;
        case 12: goto L_80141BB0; break;
        case 13: goto L_80141BB0; break;
        case 14: goto L_80141BB0; break;
        case 15: goto L_80141BB0; break;
        case 16: goto L_80141BB0; break;
        case 17: goto L_80141BB0; break;
        case 18: goto L_80141BB0; break;
        case 19: goto L_80141BB0; break;
        case 20: goto L_80141BB0; break;
        case 21: goto L_80141BB0; break;
        case 22: goto L_80141BB0; break;
        case 23: goto L_80141BB0; break;
        case 24: goto L_80141BB0; break;
        case 25: goto L_80141BB0; break;
        case 26: goto L_80141BB0; break;
        case 27: goto L_80141BB0; break;
        case 28: goto L_80141BB0; break;
        case 29: goto L_80141BB0; break;
        case 30: goto L_80141BB0; break;
        case 31: goto L_80141BB0; break;
        case 32: goto L_80141448; break;
        case 33: goto L_80141BB0; break;
        case 34: goto L_80141BB0; break;
        case 35: goto L_80141458; break;
        case 36: goto L_80141BB0; break;
        case 37: goto L_80141BB0; break;
        case 38: goto L_80141BB0; break;
        case 39: goto L_80141BB0; break;
        case 40: goto L_80141BB0; break;
        case 41: goto L_80141BB0; break;
        case 42: goto L_80141460; break;
        case 43: goto L_8014148C; break;
        case 44: goto L_80141BB0; break;
        case 45: goto L_80141484; break;
        case 46: goto L_80141494; break;
        case 47: goto L_80141BB0; break;
        case 48: goto L_80141544; break;
        case 49: goto L_8014154C; break;
        case 50: goto L_8014154C; break;
        case 51: goto L_8014154C; break;
        case 52: goto L_8014154C; break;
        case 53: goto L_8014154C; break;
        case 54: goto L_8014154C; break;
        case 55: goto L_8014154C; break;
        case 56: goto L_8014154C; break;
        case 57: goto L_8014154C; break;
        case 58: goto L_80141BB0; break;
        case 59: goto L_80141BB0; break;
        case 60: goto L_80141BB0; break;
        case 61: goto L_80141BB0; break;
        case 62: goto L_80141BB0; break;
        case 63: goto L_80141BB0; break;
        case 64: goto L_80141BB0; break;
        case 65: goto L_80141BB0; break;
        case 66: goto L_80141BB0; break;
        case 67: goto L_80141BB0; break;
        case 68: goto L_80141604; break;
        case 69: goto L_80141658; break;
        case 70: goto L_80141BB0; break;
        case 71: goto L_80141658; break;
        case 72: goto L_80141BB0; break;
        case 73: goto L_80141BB0; break;
        case 74: goto L_80141BB0; break;
        case 75: goto L_80141BB0; break;
        case 76: goto L_801415C0; break;
        case 77: goto L_80141BB0; break;
        case 78: goto L_80141BB0; break;
        case 79: goto L_801417A0; break;
        case 80: goto L_80141BB0; break;
        case 81: goto L_80141BB0; break;
        case 82: goto L_80141BB0; break;
        case 83: goto L_80141BB0; break;
        case 84: goto L_80141BB0; break;
        case 85: goto L_80141868; break;
        case 86: goto L_80141BB0; break;
        case 87: goto L_80141BB0; break;
        case 88: goto L_801418B0; break;
        case 89: goto L_80141BB0; break;
        case 90: goto L_80141BB0; break;
        case 91: goto L_80141BB0; break;
        case 92: goto L_80141BB0; break;
        case 93: goto L_80141BB0; break;
        case 94: goto L_80141BB0; break;
        case 95: goto L_80141BB0; break;
        case 96: goto L_80141BB0; break;
        case 97: goto L_80141BB0; break;
        case 98: goto L_80141BB0; break;
        case 99: goto L_801415D8; break;
        case 100: goto L_80141608; break;
        case 101: goto L_80141658; break;
        case 102: goto L_80141658; break;
        case 103: goto L_80141658; break;
        case 104: goto L_801415C8; break;
        case 105: goto L_80141608; break;
        case 106: goto L_80141BB0; break;
        case 107: goto L_80141BB0; break;
        case 108: goto L_801415D0; break;
        case 109: goto L_80141BB0; break;
        case 110: goto L_80141750; break;
        case 111: goto L_801417A4; break;
        case 112: goto L_801417E8; break;
        case 113: goto L_80141BB0; break;
        case 114: goto L_80141BB0; break;
        case 115: goto L_80141804; break;
        case 116: goto L_80141BB0; break;
        case 117: goto L_8014186C; break;
        case 118: goto L_80141BB0; break;
        case 119: goto L_80141BB0; break;
        case 120: goto L_801418BC; break;
        default: switch_error(__func__, 0x80141440, 0x80174990);
    }
    // 0x80141444: nop

L_80141448:
    // 0x80141448: beql        $t0, $zero, L_80141420
    if (ctx->r8 == 0) {
        // 0x8014144C: addiu       $t0, $zero, 0x20
        ctx->r8 = ADD32(0, 0X20);
            goto L_80141420;
    }
    goto skip_4;
    // 0x8014144C: addiu       $t0, $zero, 0x20
    ctx->r8 = ADD32(0, 0X20);
    skip_4:
    // 0x80141450: j           L_80141424
    // 0x80141454: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
        goto L_80141424;
    // 0x80141454: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_80141458:
    // 0x80141458: j           L_80141420
    // 0x8014145C: ori         $s3, $s3, 0x8
    ctx->r19 = ctx->r19 | 0X8;
        goto L_80141420;
    // 0x8014145C: ori         $s3, $s3, 0x8
    ctx->r19 = ctx->r19 | 0X8;
L_80141460:
    // 0x80141460: addiu       $v1, $s5, 0x3
    ctx->r3 = ADD32(ctx->r21, 0X3);
    // 0x80141464: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x80141468: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x8014146C: lw          $t1, 0x0($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X0);
    // 0x80141470: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x80141474: bgez        $t1, L_80141420
    if (SIGNED(ctx->r9) >= 0) {
        // 0x80141478: sw          $t1, 0x40($sp)
        MEM_W(0X40, ctx->r29) = ctx->r9;
            goto L_80141420;
    }
    // 0x80141478: sw          $t1, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r9;
    // 0x8014147C: negu        $t1, $t1
    ctx->r9 = SUB32(0, ctx->r9);
    // 0x80141480: sw          $t1, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r9;
L_80141484:
    // 0x80141484: j           L_80141420
    // 0x80141488: ori         $s3, $s3, 0x10
    ctx->r19 = ctx->r19 | 0X10;
        goto L_80141420;
    // 0x80141488: ori         $s3, $s3, 0x10
    ctx->r19 = ctx->r19 | 0X10;
L_8014148C:
    // 0x8014148C: j           L_80141420
    // 0x80141490: addiu       $t0, $zero, 0x2B
    ctx->r8 = ADD32(0, 0X2B);
        goto L_80141420;
    // 0x80141490: addiu       $t0, $zero, 0x2B
    ctx->r8 = ADD32(0, 0X2B);
L_80141494:
    // 0x80141494: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
    // 0x80141498: lbu         $v1, 0x0($s7)
    ctx->r3 = MEM_BU(ctx->r23, 0X0);
    // 0x8014149C: addiu       $v0, $zero, 0x2A
    ctx->r2 = ADD32(0, 0X2A);
    // 0x801414A0: bne         $v1, $v0, L_801414C0
    if (ctx->r3 != ctx->r2) {
        // 0x801414A4: andi        $v0, $v1, 0x80
        ctx->r2 = ctx->r3 & 0X80;
            goto L_801414C0;
    }
    // 0x801414A4: andi        $v0, $v1, 0x80
    ctx->r2 = ctx->r3 & 0X80;
    // 0x801414A8: addiu       $v1, $s5, 0x3
    ctx->r3 = ADD32(ctx->r21, 0X3);
    // 0x801414AC: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x801414B0: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x801414B4: lw          $s0, 0x0($v1)
    ctx->r16 = MEM_W(ctx->r3, 0X0);
    // 0x801414B8: j           L_80141530
    // 0x801414BC: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
        goto L_80141530;
    // 0x801414BC: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
L_801414C0:
    // 0x801414C0: bne         $v0, $zero, L_8014152C
    if (ctx->r2 != 0) {
        // 0x801414C4: addu        $s0, $zero, $zero
        ctx->r16 = ADD32(0, 0);
            goto L_8014152C;
    }
    // 0x801414C4: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x801414C8: lui         $a2, 0x801A
    ctx->r6 = S32(0X801A << 16);
    // 0x801414CC: addiu       $a2, $a2, 0x1BB2
    ctx->r6 = ADD32(ctx->r6, 0X1BB2);
    // 0x801414D0: addiu       $a1, $zero, -0x80
    ctx->r5 = ADD32(0, -0X80);
L_801414D4:
    // 0x801414D4: lbu         $v1, 0x0($s7)
    ctx->r3 = MEM_BU(ctx->r23, 0X0);
    // 0x801414D8: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x801414DC: addu        $v0, $v0, $a2
    ctx->r2 = ADD32(ctx->r2, ctx->r6);
    // 0x801414E0: lhu         $v0, 0x0($v0)
    ctx->r2 = MEM_HU(ctx->r2, 0X0);
    // 0x801414E4: andi        $v0, $v0, 0x8
    ctx->r2 = ctx->r2 & 0X8;
    // 0x801414E8: beq         $v0, $zero, L_8014152C
    if (ctx->r2 == 0) {
        // 0x801414EC: sltiu       $v0, $v1, 0x31
        ctx->r2 = ctx->r3 < 0X31 ? 1 : 0;
            goto L_8014152C;
    }
    // 0x801414EC: sltiu       $v0, $v1, 0x31
    ctx->r2 = ctx->r3 < 0X31 ? 1 : 0;
    // 0x801414F0: beq         $v0, $zero, L_80141500
    if (ctx->r2 == 0) {
        // 0x801414F4: addiu       $s7, $s7, 0x1
        ctx->r23 = ADD32(ctx->r23, 0X1);
            goto L_80141500;
    }
    // 0x801414F4: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
    // 0x801414F8: j           L_80141510
    // 0x801414FC: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
        goto L_80141510;
    // 0x801414FC: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
L_80141500:
    // 0x80141500: sltiu       $v0, $v1, 0x39
    ctx->r2 = ctx->r3 < 0X39 ? 1 : 0;
    // 0x80141504: bne         $v0, $zero, L_80141510
    if (ctx->r2 != 0) {
        // 0x80141508: addiu       $a0, $v1, -0x30
        ctx->r4 = ADD32(ctx->r3, -0X30);
            goto L_80141510;
    }
    // 0x80141508: addiu       $a0, $v1, -0x30
    ctx->r4 = ADD32(ctx->r3, -0X30);
    // 0x8014150C: addiu       $a0, $zero, 0x9
    ctx->r4 = ADD32(0, 0X9);
L_80141510:
    // 0x80141510: sll         $v0, $s0, 2
    ctx->r2 = S32(ctx->r16 << 2);
    // 0x80141514: addu        $v0, $v0, $s0
    ctx->r2 = ADD32(ctx->r2, ctx->r16);
    // 0x80141518: lbu         $v1, 0x0($s7)
    ctx->r3 = MEM_BU(ctx->r23, 0X0);
    // 0x8014151C: sll         $v0, $v0, 1
    ctx->r2 = S32(ctx->r2 << 1);
    // 0x80141520: and         $v1, $v1, $a1
    ctx->r3 = ctx->r3 & ctx->r5;
    // 0x80141524: beq         $v1, $zero, L_801414D4
    if (ctx->r3 == 0) {
        // 0x80141528: addu        $s0, $v0, $a0
        ctx->r16 = ADD32(ctx->r2, ctx->r4);
            goto L_801414D4;
    }
    // 0x80141528: addu        $s0, $v0, $a0
    ctx->r16 = ADD32(ctx->r2, ctx->r4);
L_8014152C:
    // 0x8014152C: addiu       $s7, $s7, -0x1
    ctx->r23 = ADD32(ctx->r23, -0X1);
L_80141530:
    // 0x80141530: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
    // 0x80141534: slt         $v0, $v1, $s0
    ctx->r2 = SIGNED(ctx->r3) < SIGNED(ctx->r16) ? 1 : 0;
    // 0x80141538: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x8014153C: j           L_80141420
    // 0x80141540: movn        $s2, $s0, $v0
    if (ctx->r2 != 0) ctx->r18 = ctx->r16;
        goto L_80141420;
    // 0x80141540: movn        $s2, $s0, $v0
    if (ctx->r2 != 0) ctx->r18 = ctx->r16;
L_80141544:
    // 0x80141544: j           L_80141420
    // 0x80141548: ori         $s3, $s3, 0x20
    ctx->r19 = ctx->r19 | 0X20;
        goto L_80141420;
    // 0x80141548: ori         $s3, $s3, 0x20
    ctx->r19 = ctx->r19 | 0X20;
L_8014154C:
    // 0x8014154C: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x80141550: addiu       $a2, $zero, -0x80
    ctx->r6 = ADD32(0, -0X80);
    // 0x80141554: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80141558: addiu       $a1, $a1, 0x1BB2
    ctx->r5 = ADD32(ctx->r5, 0X1BB2);
    // 0x8014155C: lbu         $v1, 0x0($s7)
    ctx->r3 = MEM_BU(ctx->r23, 0X0);
    // 0x80141560: sltiu       $v0, $v1, 0x31
    ctx->r2 = ctx->r3 < 0X31 ? 1 : 0;
L_80141564:
    // 0x80141564: bne         $v0, $zero, L_8014157C
    if (ctx->r2 != 0) {
        // 0x80141568: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_8014157C;
    }
    // 0x80141568: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x8014156C: sltiu       $v0, $v1, 0x39
    ctx->r2 = ctx->r3 < 0X39 ? 1 : 0;
    // 0x80141570: bne         $v0, $zero, L_8014157C
    if (ctx->r2 != 0) {
        // 0x80141574: addiu       $a0, $v1, -0x30
        ctx->r4 = ADD32(ctx->r3, -0X30);
            goto L_8014157C;
    }
    // 0x80141574: addiu       $a0, $v1, -0x30
    ctx->r4 = ADD32(ctx->r3, -0X30);
    // 0x80141578: addiu       $a0, $zero, 0x9
    ctx->r4 = ADD32(0, 0X9);
L_8014157C:
    // 0x8014157C: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
    // 0x80141580: sll         $v0, $s0, 2
    ctx->r2 = S32(ctx->r16 << 2);
    // 0x80141584: addu        $v0, $v0, $s0
    ctx->r2 = ADD32(ctx->r2, ctx->r16);
    // 0x80141588: lbu         $v1, 0x0($s7)
    ctx->r3 = MEM_BU(ctx->r23, 0X0);
    // 0x8014158C: sll         $v0, $v0, 1
    ctx->r2 = S32(ctx->r2 << 1);
    // 0x80141590: addu        $s0, $v0, $a0
    ctx->r16 = ADD32(ctx->r2, ctx->r4);
    // 0x80141594: and         $v0, $v1, $a2
    ctx->r2 = ctx->r3 & ctx->r6;
    // 0x80141598: bne         $v0, $zero, L_801415B4
    if (ctx->r2 != 0) {
        // 0x8014159C: sll         $v0, $v1, 1
        ctx->r2 = S32(ctx->r3 << 1);
            goto L_801415B4;
    }
    // 0x8014159C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x801415A0: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x801415A4: lhu         $v0, 0x0($v0)
    ctx->r2 = MEM_HU(ctx->r2, 0X0);
    // 0x801415A8: andi        $v0, $v0, 0x8
    ctx->r2 = ctx->r2 & 0X8;
    // 0x801415AC: bne         $v0, $zero, L_80141564
    if (ctx->r2 != 0) {
        // 0x801415B0: sltiu       $v0, $v1, 0x31
        ctx->r2 = ctx->r3 < 0X31 ? 1 : 0;
            goto L_80141564;
    }
    // 0x801415B0: sltiu       $v0, $v1, 0x31
    ctx->r2 = ctx->r3 < 0X31 ? 1 : 0;
L_801415B4:
    // 0x801415B4: sw          $s0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r16;
    // 0x801415B8: j           L_80141420
    // 0x801415BC: addiu       $s7, $s7, -0x1
    ctx->r23 = ADD32(ctx->r23, -0X1);
        goto L_80141420;
    // 0x801415BC: addiu       $s7, $s7, -0x1
    ctx->r23 = ADD32(ctx->r23, -0X1);
L_801415C0:
    // 0x801415C0: j           L_80141420
    // 0x801415C4: ori         $s3, $s3, 0x2
    ctx->r19 = ctx->r19 | 0X2;
        goto L_80141420;
    // 0x801415C4: ori         $s3, $s3, 0x2
    ctx->r19 = ctx->r19 | 0X2;
L_801415C8:
    // 0x801415C8: j           L_80141420
    // 0x801415CC: ori         $s3, $s3, 0x4
    ctx->r19 = ctx->r19 | 0X4;
        goto L_80141420;
    // 0x801415CC: ori         $s3, $s3, 0x4
    ctx->r19 = ctx->r19 | 0X4;
L_801415D0:
    // 0x801415D0: j           L_80141420
    // 0x801415D4: ori         $s3, $s3, 0x1
    ctx->r19 = ctx->r19 | 0X1;
        goto L_80141420;
    // 0x801415D4: ori         $s3, $s3, 0x1
    ctx->r19 = ctx->r19 | 0X1;
L_801415D8:
    // 0x801415D8: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x801415DC: addiu       $s1, $s1, 0x3898
    ctx->r17 = ADD32(ctx->r17, 0X3898);
    // 0x801415E0: addiu       $v0, $s5, 0x3
    ctx->r2 = ADD32(ctx->r21, 0X3);
    // 0x801415E4: addiu       $v1, $zero, -0x4
    ctx->r3 = ADD32(0, -0X4);
    // 0x801415E8: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x801415EC: addiu       $s5, $v0, 0x4
    ctx->r21 = ADD32(ctx->r2, 0X4);
    // 0x801415F0: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    // 0x801415F4: addiu       $s4, $zero, 0x1
    ctx->r20 = ADD32(0, 0X1);
    // 0x801415F8: addu        $t0, $zero, $zero
    ctx->r8 = ADD32(0, 0);
    // 0x801415FC: j           L_801419AC
    // 0x80141600: sb          $v0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r2;
        goto L_801419AC;
    // 0x80141600: sb          $v0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r2;
L_80141604:
    // 0x80141604: ori         $s3, $s3, 0x1
    ctx->r19 = ctx->r19 | 0X1;
L_80141608:
    // 0x80141608: andi        $v0, $s3, 0x1
    ctx->r2 = ctx->r19 & 0X1;
    // 0x8014160C: bne         $v0, $zero, L_80141634
    if (ctx->r2 != 0) {
        // 0x80141610: addiu       $v1, $s5, 0x3
        ctx->r3 = ADD32(ctx->r21, 0X3);
            goto L_80141634;
    }
    // 0x80141610: addiu       $v1, $s5, 0x3
    ctx->r3 = ADD32(ctx->r21, 0X3);
    // 0x80141614: andi        $v0, $s3, 0x4
    ctx->r2 = ctx->r19 & 0X4;
    // 0x80141618: beq         $v0, $zero, L_80141634
    if (ctx->r2 == 0) {
        // 0x8014161C: addiu       $v0, $zero, -0x4
        ctx->r2 = ADD32(0, -0X4);
            goto L_80141634;
    }
    // 0x8014161C: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x80141620: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x80141624: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x80141628: lh          $v1, 0x0($v1)
    ctx->r3 = MEM_H(ctx->r3, 0X0);
    // 0x8014162C: j           L_80141644
    // 0x80141630: nop

        goto L_80141644;
    // 0x80141630: nop

L_80141634:
    // 0x80141634: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x80141638: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x8014163C: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x80141640: lw          $v1, 0x0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X0);
L_80141644:
    // 0x80141644: bgez        $v1, L_80141910
    if (SIGNED(ctx->r3) >= 0) {
        // 0x80141648: addiu       $a0, $zero, 0xA
        ctx->r4 = ADD32(0, 0XA);
            goto L_80141910;
    }
    // 0x80141648: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
    // 0x8014164C: negu        $v1, $v1
    ctx->r3 = SUB32(0, ctx->r3);
    // 0x80141650: j           L_80141910
    // 0x80141654: addiu       $t0, $zero, 0x2D
    ctx->r8 = ADD32(0, 0X2D);
        goto L_80141910;
    // 0x80141654: addiu       $t0, $zero, 0x2D
    ctx->r8 = ADD32(0, 0X2D);
L_80141658:
    // 0x80141658: addiu       $v0, $s5, 0x7
    ctx->r2 = ADD32(ctx->r21, 0X7);
    // 0x8014165C: addiu       $v1, $zero, -0x8
    ctx->r3 = ADD32(0, -0X8);
    // 0x80141660: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x80141664: addiu       $s5, $v0, 0x8
    ctx->r21 = ADD32(ctx->r2, 0X8);
    // 0x80141668: ldc1        $f12, 0x0($v0)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r2, 0X0);
    // 0x8014166C: slti        $v0, $s2, 0x28
    ctx->r2 = SIGNED(ctx->r18) < 0X28 ? 1 : 0;
    // 0x80141670: bne         $v0, $zero, L_801416A8
    if (ctx->r2 != 0) {
        // 0x80141674: addiu       $v0, $zero, -0x1
        ctx->r2 = ADD32(0, -0X1);
            goto L_801416A8;
    }
    // 0x80141674: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80141678: lbu         $v1, 0x0($s7)
    ctx->r3 = MEM_BU(ctx->r23, 0X0);
    // 0x8014167C: addiu       $v0, $zero, 0x67
    ctx->r2 = ADD32(0, 0X67);
    // 0x80141680: beq         $v1, $v0, L_801416A0
    if (ctx->r3 == ctx->r2) {
        // 0x80141684: addiu       $v0, $zero, 0x47
        ctx->r2 = ADD32(0, 0X47);
            goto L_801416A0;
    }
    // 0x80141684: addiu       $v0, $zero, 0x47
    ctx->r2 = ADD32(0, 0X47);
    // 0x80141688: bne         $v1, $v0, L_8014169C
    if (ctx->r3 != ctx->r2) {
        // 0x8014168C: addiu       $s2, $s2, -0x27
        ctx->r18 = ADD32(ctx->r18, -0X27);
            goto L_8014169C;
    }
    // 0x8014168C: addiu       $s2, $s2, -0x27
    ctx->r18 = ADD32(ctx->r18, -0X27);
    // 0x80141690: andi        $v0, $s3, 0x8
    ctx->r2 = ctx->r19 & 0X8;
    // 0x80141694: beql        $v0, $zero, L_801416B4
    if (ctx->r2 == 0) {
        // 0x80141698: addiu       $s2, $zero, 0x27
        ctx->r18 = ADD32(0, 0X27);
            goto L_801416B4;
    }
    goto skip_5;
    // 0x80141698: addiu       $s2, $zero, 0x27
    ctx->r18 = ADD32(0, 0X27);
    skip_5:
L_8014169C:
    // 0x8014169C: sw          $s2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r18;
L_801416A0:
    // 0x801416A0: j           L_801416B4
    // 0x801416A4: addiu       $s2, $zero, 0x27
    ctx->r18 = ADD32(0, 0X27);
        goto L_801416B4;
    // 0x801416A4: addiu       $s2, $zero, 0x27
    ctx->r18 = ADD32(0, 0X27);
L_801416A8:
    // 0x801416A8: bne         $s2, $v0, L_801416B4
    if (ctx->r18 != ctx->r2) {
        // 0x801416AC: nop
    
            goto L_801416B4;
    }
    // 0x801416AC: nop

    // 0x801416B0: addiu       $s2, $zero, 0x6
    ctx->r18 = ADD32(0, 0X6);
L_801416B4:
    // 0x801416B4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801416B8: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x801416BC: c.lt.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d < ctx->f0.d;
    // 0x801416C0: bc1f        L_801416D4
    if (!c1cs) {
        // 0x801416C4: addiu       $v0, $zero, 0x2D
        ctx->r2 = ADD32(0, 0X2D);
            goto L_801416D4;
    }
    // 0x801416C4: addiu       $v0, $zero, 0x2D
    ctx->r2 = ADD32(0, 0X2D);
    // 0x801416C8: sb          $v0, 0x20($sp)
    MEM_B(0X20, ctx->r29) = ctx->r2;
    // 0x801416CC: j           L_801416D8
    // 0x801416D0: neg.d       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.d); 
    ctx->f12.d = -ctx->f12.d;
        goto L_801416D8;
    // 0x801416D0: neg.d       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.d); 
    ctx->f12.d = -ctx->f12.d;
L_801416D4:
    // 0x801416D4: sb          $zero, 0x20($sp)
    MEM_B(0X20, ctx->r29) = 0;
L_801416D8:
    // 0x801416D8: lbu         $a0, 0x0($s7)
    ctx->r4 = MEM_BU(ctx->r23, 0X0);
    // 0x801416DC: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    // 0x801416E0: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801416E4: addiu       $v1, $v1, 0x3898
    ctx->r3 = ADD32(ctx->r3, 0X3898);
    // 0x801416E8: addiu       $v0, $sp, 0x20
    ctx->r2 = ADD32(ctx->r29, 0X20);
    // 0x801416EC: sb          $zero, 0x0($v1)
    MEM_B(0X0, ctx->r3) = 0;
    // 0x801416F0: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    // 0x801416F4: addiu       $v1, $v1, 0x1380
    ctx->r3 = ADD32(ctx->r3, 0X1380);
    // 0x801416F8: addu        $a3, $s3, $zero
    ctx->r7 = ADD32(ctx->r19, 0);
    // 0x801416FC: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    // 0x80141700: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x80141704: sw          $t0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r8;
    // 0x80141708: jal         0x80141C04
    // 0x8014170C: sw          $a0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r4;
    func_80141C04(rdram, ctx);
        goto after_2;
    // 0x8014170C: sw          $a0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r4;
    after_2:
    // 0x80141710: lbu         $v1, 0x20($sp)
    ctx->r3 = MEM_BU(ctx->r29, 0X20);
    // 0x80141714: lw          $t0, 0x50($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X50);
    // 0x80141718: beq         $v1, $zero, L_80141730
    if (ctx->r3 == 0) {
        // 0x8014171C: addu        $s4, $v0, $zero
        ctx->r20 = ADD32(ctx->r2, 0);
            goto L_80141730;
    }
    // 0x8014171C: addu        $s4, $v0, $zero
    ctx->r20 = ADD32(ctx->r2, 0);
    // 0x80141720: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80141724: lw          $v0, 0x1AC4($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1AC4);
    // 0x80141728: beql        $v0, $zero, L_80141730
    if (ctx->r2 == 0) {
        // 0x8014172C: addiu       $t0, $zero, 0x2D
        ctx->r8 = ADD32(0, 0X2D);
            goto L_80141730;
    }
    goto skip_6;
    // 0x8014172C: addiu       $t0, $zero, 0x2D
    ctx->r8 = ADD32(0, 0X2D);
    skip_6:
L_80141730:
    // 0x80141730: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80141734: addiu       $v1, $v1, 0x3898
    ctx->r3 = ADD32(ctx->r3, 0X3898);
    // 0x80141738: lbu         $v0, 0x0($v1)
    ctx->r2 = MEM_BU(ctx->r3, 0X0);
    // 0x8014173C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80141740: sw          $zero, 0x1AC4($at)
    MEM_W(0X1AC4, ctx->r1) = 0;
    // 0x80141744: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x80141748: j           L_801419AC
    // 0x8014174C: addu        $s1, $v0, $v1
    ctx->r17 = ADD32(ctx->r2, ctx->r3);
        goto L_801419AC;
    // 0x8014174C: addu        $s1, $v0, $v1
    ctx->r17 = ADD32(ctx->r2, ctx->r3);
L_80141750:
    // 0x80141750: andi        $v0, $s3, 0x1
    ctx->r2 = ctx->r19 & 0X1;
    // 0x80141754: bne         $v0, $zero, L_80141784
    if (ctx->r2 != 0) {
        // 0x80141758: addiu       $v0, $s5, 0x3
        ctx->r2 = ADD32(ctx->r21, 0X3);
            goto L_80141784;
    }
    // 0x80141758: addiu       $v0, $s5, 0x3
    ctx->r2 = ADD32(ctx->r21, 0X3);
    // 0x8014175C: andi        $v0, $s3, 0x4
    ctx->r2 = ctx->r19 & 0X4;
    // 0x80141760: beq         $v0, $zero, L_80141784
    if (ctx->r2 == 0) {
        // 0x80141764: addiu       $v0, $s5, 0x3
        ctx->r2 = ADD32(ctx->r21, 0X3);
            goto L_80141784;
    }
    // 0x80141764: addiu       $v0, $s5, 0x3
    ctx->r2 = ADD32(ctx->r21, 0X3);
    // 0x80141768: addiu       $v1, $zero, -0x4
    ctx->r3 = ADD32(0, -0X4);
    // 0x8014176C: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x80141770: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x80141774: lhu         $t1, 0x28($sp)
    ctx->r9 = MEM_HU(ctx->r29, 0X28);
    // 0x80141778: addiu       $s5, $v0, 0x4
    ctx->r21 = ADD32(ctx->r2, 0X4);
    // 0x8014177C: j           L_80141BC8
    // 0x80141780: sh          $t1, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r9;
        goto L_80141BC8;
    // 0x80141780: sh          $t1, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r9;
L_80141784:
    // 0x80141784: addiu       $v1, $zero, -0x4
    ctx->r3 = ADD32(0, -0X4);
    // 0x80141788: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x8014178C: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x80141790: lw          $t1, 0x28($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X28);
    // 0x80141794: addiu       $s5, $v0, 0x4
    ctx->r21 = ADD32(ctx->r2, 0X4);
    // 0x80141798: j           L_80141BC8
    // 0x8014179C: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
        goto L_80141BC8;
    // 0x8014179C: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
L_801417A0:
    // 0x801417A0: ori         $s3, $s3, 0x1
    ctx->r19 = ctx->r19 | 0X1;
L_801417A4:
    // 0x801417A4: andi        $v0, $s3, 0x1
    ctx->r2 = ctx->r19 & 0X1;
    // 0x801417A8: bne         $v0, $zero, L_801417D0
    if (ctx->r2 != 0) {
        // 0x801417AC: addiu       $v1, $s5, 0x3
        ctx->r3 = ADD32(ctx->r21, 0X3);
            goto L_801417D0;
    }
    // 0x801417AC: addiu       $v1, $s5, 0x3
    ctx->r3 = ADD32(ctx->r21, 0X3);
    // 0x801417B0: andi        $v0, $s3, 0x4
    ctx->r2 = ctx->r19 & 0X4;
    // 0x801417B4: beq         $v0, $zero, L_801417D0
    if (ctx->r2 == 0) {
        // 0x801417B8: addiu       $v0, $zero, -0x4
        ctx->r2 = ADD32(0, -0X4);
            goto L_801417D0;
    }
    // 0x801417B8: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x801417BC: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x801417C0: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x801417C4: lhu         $v1, 0x0($v1)
    ctx->r3 = MEM_HU(ctx->r3, 0X0);
    // 0x801417C8: j           L_8014190C
    // 0x801417CC: addiu       $a0, $zero, 0x8
    ctx->r4 = ADD32(0, 0X8);
        goto L_8014190C;
    // 0x801417CC: addiu       $a0, $zero, 0x8
    ctx->r4 = ADD32(0, 0X8);
L_801417D0:
    // 0x801417D0: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x801417D4: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x801417D8: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x801417DC: lw          $v1, 0x0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X0);
    // 0x801417E0: j           L_8014190C
    // 0x801417E4: addiu       $a0, $zero, 0x8
    ctx->r4 = ADD32(0, 0X8);
        goto L_8014190C;
    // 0x801417E4: addiu       $a0, $zero, 0x8
    ctx->r4 = ADD32(0, 0X8);
L_801417E8:
    // 0x801417E8: addiu       $v1, $s5, 0x3
    ctx->r3 = ADD32(ctx->r21, 0X3);
    // 0x801417EC: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x801417F0: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x801417F4: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x801417F8: lw          $v1, 0x0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X0);
    // 0x801417FC: j           L_8014190C
    // 0x80141800: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
        goto L_8014190C;
    // 0x80141800: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
L_80141804:
    // 0x80141804: addiu       $v1, $s5, 0x3
    ctx->r3 = ADD32(ctx->r21, 0X3);
    // 0x80141808: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x8014180C: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x80141810: lw          $s1, 0x0($v1)
    ctx->r17 = MEM_W(ctx->r3, 0X0);
    // 0x80141814: bne         $s1, $zero, L_80141824
    if (ctx->r17 != 0) {
        // 0x80141818: addiu       $s5, $v1, 0x4
        ctx->r21 = ADD32(ctx->r3, 0X4);
            goto L_80141824;
    }
    // 0x80141818: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x8014181C: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x80141820: addiu       $s1, $s1, 0x1AC8
    ctx->r17 = ADD32(ctx->r17, 0X1AC8);
L_80141824:
    // 0x80141824: bltz        $s2, L_80141854
    if (SIGNED(ctx->r18) < 0) {
        // 0x80141828: addu        $a0, $s1, $zero
        ctx->r4 = ADD32(ctx->r17, 0);
            goto L_80141854;
    }
    // 0x80141828: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    // 0x8014182C: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80141830: jal         0x80142820
    // 0x80141834: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    func_80142820(rdram, ctx);
        goto after_3;
    // 0x80141834: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    after_3:
    // 0x80141838: beq         $v0, $zero, L_8014184C
    if (ctx->r2 == 0) {
        // 0x8014183C: subu        $s4, $v0, $s1
        ctx->r20 = SUB32(ctx->r2, ctx->r17);
            goto L_8014184C;
    }
    // 0x8014183C: subu        $s4, $v0, $s1
    ctx->r20 = SUB32(ctx->r2, ctx->r17);
    // 0x80141840: slt         $v0, $s2, $s4
    ctx->r2 = SIGNED(ctx->r18) < SIGNED(ctx->r20) ? 1 : 0;
    // 0x80141844: j           L_80141860
    // 0x80141848: movn        $s4, $s2, $v0
    if (ctx->r2 != 0) ctx->r20 = ctx->r18;
        goto L_80141860;
    // 0x80141848: movn        $s4, $s2, $v0
    if (ctx->r2 != 0) ctx->r20 = ctx->r18;
L_8014184C:
    // 0x8014184C: j           L_80141860
    // 0x80141850: addu        $s4, $s2, $zero
    ctx->r20 = ADD32(ctx->r18, 0);
        goto L_80141860;
    // 0x80141850: addu        $s4, $s2, $zero
    ctx->r20 = ADD32(ctx->r18, 0);
L_80141854:
    // 0x80141854: jal         0x80141138
    // 0x80141858: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    func_80141138(rdram, ctx);
        goto after_4;
    // 0x80141858: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    after_4:
    // 0x8014185C: addu        $s4, $v0, $zero
    ctx->r20 = ADD32(ctx->r2, 0);
L_80141860:
    // 0x80141860: j           L_801419AC
    // 0x80141864: addu        $t0, $zero, $zero
    ctx->r8 = ADD32(0, 0);
        goto L_801419AC;
    // 0x80141864: addu        $t0, $zero, $zero
    ctx->r8 = ADD32(0, 0);
L_80141868:
    // 0x80141868: ori         $s3, $s3, 0x1
    ctx->r19 = ctx->r19 | 0X1;
L_8014186C:
    // 0x8014186C: andi        $v0, $s3, 0x1
    ctx->r2 = ctx->r19 & 0X1;
    // 0x80141870: bne         $v0, $zero, L_80141898
    if (ctx->r2 != 0) {
        // 0x80141874: addiu       $v1, $s5, 0x3
        ctx->r3 = ADD32(ctx->r21, 0X3);
            goto L_80141898;
    }
    // 0x80141874: addiu       $v1, $s5, 0x3
    ctx->r3 = ADD32(ctx->r21, 0X3);
    // 0x80141878: andi        $v0, $s3, 0x4
    ctx->r2 = ctx->r19 & 0X4;
    // 0x8014187C: beq         $v0, $zero, L_80141898
    if (ctx->r2 == 0) {
        // 0x80141880: addiu       $v0, $zero, -0x4
        ctx->r2 = ADD32(0, -0X4);
            goto L_80141898;
    }
    // 0x80141880: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x80141884: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x80141888: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x8014188C: lhu         $v1, 0x0($v1)
    ctx->r3 = MEM_HU(ctx->r3, 0X0);
    // 0x80141890: j           L_8014190C
    // 0x80141894: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
        goto L_8014190C;
    // 0x80141894: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
L_80141898:
    // 0x80141898: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x8014189C: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x801418A0: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x801418A4: lw          $v1, 0x0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X0);
    // 0x801418A8: j           L_8014190C
    // 0x801418AC: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
        goto L_8014190C;
    // 0x801418AC: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
L_801418B0:
    // 0x801418B0: lui         $t1, 0x8017
    ctx->r9 = S32(0X8017 << 16);
    // 0x801418B4: addiu       $t1, $t1, 0x497C
    ctx->r9 = ADD32(ctx->r9, 0X497C);
    // 0x801418B8: sw          $t1, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r9;
L_801418BC:
    // 0x801418BC: andi        $v0, $s3, 0x1
    ctx->r2 = ctx->r19 & 0X1;
    // 0x801418C0: bne         $v0, $zero, L_801418E8
    if (ctx->r2 != 0) {
        // 0x801418C4: addiu       $v1, $s5, 0x3
        ctx->r3 = ADD32(ctx->r21, 0X3);
            goto L_801418E8;
    }
    // 0x801418C4: addiu       $v1, $s5, 0x3
    ctx->r3 = ADD32(ctx->r21, 0X3);
    // 0x801418C8: andi        $v0, $s3, 0x4
    ctx->r2 = ctx->r19 & 0X4;
    // 0x801418CC: beq         $v0, $zero, L_801418E8
    if (ctx->r2 == 0) {
        // 0x801418D0: addiu       $v0, $zero, -0x4
        ctx->r2 = ADD32(0, -0X4);
            goto L_801418E8;
    }
    // 0x801418D0: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x801418D4: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x801418D8: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x801418DC: lhu         $v1, 0x0($v1)
    ctx->r3 = MEM_HU(ctx->r3, 0X0);
    // 0x801418E0: j           L_801418FC
    // 0x801418E4: andi        $v0, $s3, 0x8
    ctx->r2 = ctx->r19 & 0X8;
        goto L_801418FC;
    // 0x801418E4: andi        $v0, $s3, 0x8
    ctx->r2 = ctx->r19 & 0X8;
L_801418E8:
    // 0x801418E8: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x801418EC: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x801418F0: addiu       $s5, $v1, 0x4
    ctx->r21 = ADD32(ctx->r3, 0X4);
    // 0x801418F4: lw          $v1, 0x0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X0);
    // 0x801418F8: andi        $v0, $s3, 0x8
    ctx->r2 = ctx->r19 & 0X8;
L_801418FC:
    // 0x801418FC: beq         $v0, $zero, L_8014190C
    if (ctx->r2 == 0) {
        // 0x80141900: addiu       $a0, $zero, 0x10
        ctx->r4 = ADD32(0, 0X10);
            goto L_8014190C;
    }
    // 0x80141900: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    // 0x80141904: bnel        $v1, $zero, L_8014190C
    if (ctx->r3 != 0) {
        // 0x80141908: ori         $s3, $s3, 0x40
        ctx->r19 = ctx->r19 | 0X40;
            goto L_8014190C;
    }
    goto skip_7;
    // 0x80141908: ori         $s3, $s3, 0x40
    ctx->r19 = ctx->r19 | 0X40;
    skip_7:
L_8014190C:
    // 0x8014190C: addu        $t0, $zero, $zero
    ctx->r8 = ADD32(0, 0);
L_80141910:
    // 0x80141910: bltz        $s2, L_80141920
    if (SIGNED(ctx->r18) < 0) {
        // 0x80141914: sw          $s2, 0x30($sp)
        MEM_W(0X30, ctx->r29) = ctx->r18;
            goto L_80141920;
    }
    // 0x80141914: sw          $s2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r18;
    // 0x80141918: addiu       $v0, $zero, -0x21
    ctx->r2 = ADD32(0, -0X21);
    // 0x8014191C: and         $s3, $s3, $v0
    ctx->r19 = ctx->r19 & ctx->r2;
L_80141920:
    // 0x80141920: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x80141924: addiu       $s1, $s1, 0x4C18
    ctx->r17 = ADD32(ctx->r17, 0X4C18);
    // 0x80141928: bne         $v1, $zero, L_8014193C
    if (ctx->r3 != 0) {
        // 0x8014192C: nop
    
            goto L_8014193C;
    }
    // 0x8014192C: nop

    // 0x80141930: lw          $t1, 0x30($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X30);
    // 0x80141934: beq         $t1, $zero, L_801419A0
    if (ctx->r9 == 0) {
        // 0x80141938: nop
    
            goto L_801419A0;
    }
    // 0x80141938: nop

L_8014193C:
    // 0x8014193C: divu        $zero, $v1, $a0
    lo = S32(U32(ctx->r3) / U32(ctx->r4)); hi = S32(U32(ctx->r3) % U32(ctx->r4));
    // 0x80141940: bne         $a0, $zero, L_8014194C
    if (ctx->r4 != 0) {
        // 0x80141944: nop
    
            goto L_8014194C;
    }
    // 0x80141944: nop

    // 0x80141948: break       7
    do_break(2148800840);
L_8014194C:
    // 0x8014194C: mflo        $v1
    ctx->r3 = lo;
    // 0x80141950: mfhi        $v0
    ctx->r2 = hi;
    // 0x80141954: lw          $t1, 0x48($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X48);
    // 0x80141958: addu        $v0, $t1, $v0
    ctx->r2 = ADD32(ctx->r9, ctx->r2);
    // 0x8014195C: lbu         $a1, 0x0($v0)
    ctx->r5 = MEM_BU(ctx->r2, 0X0);
    // 0x80141960: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
    // 0x80141964: bne         $v1, $zero, L_8014193C
    if (ctx->r3 != 0) {
        // 0x80141968: sb          $a1, 0x0($s1)
        MEM_B(0X0, ctx->r17) = ctx->r5;
            goto L_8014193C;
    }
    // 0x80141968: sb          $a1, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r5;
    // 0x8014196C: lui         $t1, 0x8017
    ctx->r9 = S32(0X8017 << 16);
    // 0x80141970: addiu       $t1, $t1, 0x4968
    ctx->r9 = ADD32(ctx->r9, 0X4968);
    // 0x80141974: andi        $v0, $s3, 0x8
    ctx->r2 = ctx->r19 & 0X8;
    // 0x80141978: beq         $v0, $zero, L_801419A0
    if (ctx->r2 == 0) {
        // 0x8014197C: sw          $t1, 0x48($sp)
        MEM_W(0X48, ctx->r29) = ctx->r9;
            goto L_801419A0;
    }
    // 0x8014197C: sw          $t1, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r9;
    // 0x80141980: addiu       $v0, $zero, 0x8
    ctx->r2 = ADD32(0, 0X8);
    // 0x80141984: bne         $a0, $v0, L_801419A0
    if (ctx->r4 != ctx->r2) {
        // 0x80141988: andi        $v1, $a1, 0xFF
        ctx->r3 = ctx->r5 & 0XFF;
            goto L_801419A0;
    }
    // 0x80141988: andi        $v1, $a1, 0xFF
    ctx->r3 = ctx->r5 & 0XFF;
    // 0x8014198C: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
    // 0x80141990: beq         $v1, $v0, L_801419A0
    if (ctx->r3 == ctx->r2) {
        // 0x80141994: addiu       $v0, $zero, 0x30
        ctx->r2 = ADD32(0, 0X30);
            goto L_801419A0;
    }
    // 0x80141994: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
    // 0x80141998: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
    // 0x8014199C: sb          $v0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r2;
L_801419A0:
    // 0x801419A0: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801419A4: addiu       $v0, $v0, 0x4C18
    ctx->r2 = ADD32(ctx->r2, 0X4C18);
    // 0x801419A8: subu        $s4, $v0, $s1
    ctx->r20 = SUB32(ctx->r2, ctx->r17);
L_801419AC:
    // 0x801419AC: lw          $t1, 0x38($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X38);
    // 0x801419B0: lw          $s2, 0x30($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X30);
    // 0x801419B4: addu        $fp, $s4, $t1
    ctx->r30 = ADD32(ctx->r20, ctx->r9);
    // 0x801419B8: slt         $v0, $s2, $fp
    ctx->r2 = SIGNED(ctx->r18) < SIGNED(ctx->r30) ? 1 : 0;
    // 0x801419BC: beq         $t0, $zero, L_801419C8
    if (ctx->r8 == 0) {
        // 0x801419C0: movn        $s2, $fp, $v0
        if (ctx->r2 != 0) ctx->r18 = ctx->r30;
            goto L_801419C8;
    }
    // 0x801419C0: movn        $s2, $fp, $v0
    if (ctx->r2 != 0) ctx->r18 = ctx->r30;
    // 0x801419C4: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_801419C8:
    // 0x801419C8: andi        $v0, $s3, 0x40
    ctx->r2 = ctx->r19 & 0X40;
    // 0x801419CC: bnel        $v0, $zero, L_801419D4
    if (ctx->r2 != 0) {
        // 0x801419D0: addiu       $s2, $s2, 0x2
        ctx->r18 = ADD32(ctx->r18, 0X2);
            goto L_801419D4;
    }
    goto skip_8;
    // 0x801419D0: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
    skip_8:
L_801419D4:
    // 0x801419D4: andi        $v0, $s3, 0x30
    ctx->r2 = ctx->r19 & 0X30;
    // 0x801419D8: bne         $v0, $zero, L_80141A20
    if (ctx->r2 != 0) {
        // 0x801419DC: andi        $a0, $t0, 0xFF
        ctx->r4 = ctx->r8 & 0XFF;
            goto L_80141A20;
    }
    // 0x801419DC: andi        $a0, $t0, 0xFF
    ctx->r4 = ctx->r8 & 0XFF;
    // 0x801419E0: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x801419E4: beq         $t1, $zero, L_80141A20
    if (ctx->r9 == 0) {
        // 0x801419E8: slt         $v0, $s2, $t1
        ctx->r2 = SIGNED(ctx->r18) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_80141A20;
    }
    // 0x801419E8: slt         $v0, $s2, $t1
    ctx->r2 = SIGNED(ctx->r18) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x801419EC: beq         $v0, $zero, L_80141A20
    if (ctx->r2 == 0) {
        // 0x801419F0: addu        $s0, $s2, $zero
        ctx->r16 = ADD32(ctx->r18, 0);
            goto L_80141A20;
    }
    // 0x801419F0: addu        $s0, $s2, $zero
    ctx->r16 = ADD32(ctx->r18, 0);
    // 0x801419F4: addiu       $a0, $zero, 0x20
    ctx->r4 = ADD32(0, 0X20);
L_801419F8:
    // 0x801419F8: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    // 0x801419FC: jal         0x80143640
    // 0x80141A00: sw          $t0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r8;
    func_80143640(rdram, ctx);
        goto after_5;
    // 0x80141A00: sw          $t0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r8;
    after_5:
    // 0x80141A04: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x80141A08: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80141A0C: lw          $t0, 0x50($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X50);
    // 0x80141A10: slt         $v0, $s0, $t1
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x80141A14: bne         $v0, $zero, L_801419F8
    if (ctx->r2 != 0) {
        // 0x80141A18: addiu       $a0, $zero, 0x20
        ctx->r4 = ADD32(0, 0X20);
            goto L_801419F8;
    }
    // 0x80141A18: addiu       $a0, $zero, 0x20
    ctx->r4 = ADD32(0, 0X20);
    // 0x80141A1C: andi        $a0, $t0, 0xFF
    ctx->r4 = ctx->r8 & 0XFF;
L_80141A20:
    // 0x80141A20: beq         $a0, $zero, L_80141A34
    if (ctx->r4 == 0) {
        // 0x80141A24: andi        $v0, $s3, 0x40
        ctx->r2 = ctx->r19 & 0X40;
            goto L_80141A34;
    }
    // 0x80141A24: andi        $v0, $s3, 0x40
    ctx->r2 = ctx->r19 & 0X40;
    // 0x80141A28: jal         0x80143640
    // 0x80141A2C: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    func_80143640(rdram, ctx);
        goto after_6;
    // 0x80141A2C: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    after_6:
    // 0x80141A30: andi        $v0, $s3, 0x40
    ctx->r2 = ctx->r19 & 0X40;
L_80141A34:
    // 0x80141A34: beq         $v0, $zero, L_80141A50
    if (ctx->r2 == 0) {
        // 0x80141A38: addiu       $a0, $zero, 0x30
        ctx->r4 = ADD32(0, 0X30);
            goto L_80141A50;
    }
    // 0x80141A38: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
    // 0x80141A3C: jal         0x80143640
    // 0x80141A40: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    func_80143640(rdram, ctx);
        goto after_7;
    // 0x80141A40: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    after_7:
    // 0x80141A44: lbu         $a0, 0x0($s7)
    ctx->r4 = MEM_BU(ctx->r23, 0X0);
    // 0x80141A48: jal         0x80143640
    // 0x80141A4C: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    func_80143640(rdram, ctx);
        goto after_8;
    // 0x80141A4C: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    after_8:
L_80141A50:
    // 0x80141A50: andi        $v1, $s3, 0x30
    ctx->r3 = ctx->r19 & 0X30;
    // 0x80141A54: addiu       $v0, $zero, 0x20
    ctx->r2 = ADD32(0, 0X20);
    // 0x80141A58: bne         $v1, $v0, L_80141A90
    if (ctx->r3 != ctx->r2) {
        // 0x80141A5C: nop
    
            goto L_80141A90;
    }
    // 0x80141A5C: nop

    // 0x80141A60: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x80141A64: slt         $v0, $s2, $t1
    ctx->r2 = SIGNED(ctx->r18) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x80141A68: beq         $v0, $zero, L_80141A90
    if (ctx->r2 == 0) {
        // 0x80141A6C: addu        $s0, $s2, $zero
        ctx->r16 = ADD32(ctx->r18, 0);
            goto L_80141A90;
    }
    // 0x80141A6C: addu        $s0, $s2, $zero
    ctx->r16 = ADD32(ctx->r18, 0);
    // 0x80141A70: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
L_80141A74:
    // 0x80141A74: jal         0x80143640
    // 0x80141A78: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    func_80143640(rdram, ctx);
        goto after_9;
    // 0x80141A78: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    after_9:
    // 0x80141A7C: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x80141A80: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80141A84: slt         $v0, $s0, $t1
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x80141A88: bne         $v0, $zero, L_80141A74
    if (ctx->r2 != 0) {
        // 0x80141A8C: addiu       $a0, $zero, 0x30
        ctx->r4 = ADD32(0, 0X30);
            goto L_80141A74;
    }
    // 0x80141A8C: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
L_80141A90:
    // 0x80141A90: lw          $t1, 0x30($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X30);
    // 0x80141A94: addu        $s0, $fp, $zero
    ctx->r16 = ADD32(ctx->r30, 0);
    // 0x80141A98: slt         $v0, $s0, $t1
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x80141A9C: beq         $v0, $zero, L_80141AC4
    if (ctx->r2 == 0) {
        // 0x80141AA0: nop
    
            goto L_80141AC4;
    }
    // 0x80141AA0: nop

    // 0x80141AA4: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
L_80141AA8:
    // 0x80141AA8: jal         0x80143640
    // 0x80141AAC: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    func_80143640(rdram, ctx);
        goto after_10;
    // 0x80141AAC: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    after_10:
    // 0x80141AB0: lw          $t1, 0x30($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X30);
    // 0x80141AB4: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80141AB8: slt         $v0, $s0, $t1
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x80141ABC: bne         $v0, $zero, L_80141AA8
    if (ctx->r2 != 0) {
        // 0x80141AC0: addiu       $a0, $zero, 0x30
        ctx->r4 = ADD32(0, 0X30);
            goto L_80141AA8;
    }
    // 0x80141AC0: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
L_80141AC4:
    // 0x80141AC4: lw          $v0, 0x0($s6)
    ctx->r2 = MEM_W(ctx->r22, 0X0);
    // 0x80141AC8: addu        $s0, $s4, $zero
    ctx->r16 = ADD32(ctx->r20, 0);
    // 0x80141ACC: subu        $v1, $v0, $s0
    ctx->r3 = SUB32(ctx->r2, ctx->r16);
    // 0x80141AD0: bltzl       $v1, L_80141B0C
    if (SIGNED(ctx->r3) < 0) {
        // 0x80141AD4: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_80141B0C;
    }
    goto skip_9;
    // 0x80141AD4: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    skip_9:
    // 0x80141AD8: lw          $v0, 0x10($s6)
    ctx->r2 = MEM_W(ctx->r22, 0X10);
    // 0x80141ADC: andi        $v0, $v0, 0x4
    ctx->r2 = ctx->r2 & 0X4;
    // 0x80141AE0: bnel        $v0, $zero, L_80141B0C
    if (ctx->r2 != 0) {
        // 0x80141AE4: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_80141B0C;
    }
    goto skip_10;
    // 0x80141AE4: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    skip_10:
    // 0x80141AE8: lw          $a0, 0x4($s6)
    ctx->r4 = MEM_W(ctx->r22, 0X4);
    // 0x80141AEC: addu        $a1, $s1, $zero
    ctx->r5 = ADD32(ctx->r17, 0);
    // 0x80141AF0: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    // 0x80141AF4: jal         0x80141168
    // 0x80141AF8: sw          $v1, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r3;
    func_80141168(rdram, ctx);
        goto after_11;
    // 0x80141AF8: sw          $v1, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r3;
    after_11:
    // 0x80141AFC: lw          $v0, 0x4($s6)
    ctx->r2 = MEM_W(ctx->r22, 0X4);
    // 0x80141B00: addu        $v0, $v0, $s0
    ctx->r2 = ADD32(ctx->r2, ctx->r16);
    // 0x80141B04: j           L_80141B44
    // 0x80141B08: sw          $v0, 0x4($s6)
    MEM_W(0X4, ctx->r22) = ctx->r2;
        goto L_80141B44;
    // 0x80141B08: sw          $v0, 0x4($s6)
    MEM_W(0X4, ctx->r22) = ctx->r2;
L_80141B0C:
    // 0x80141B0C: bltz        $s0, L_80141B44
    if (SIGNED(ctx->r16) < 0) {
        // 0x80141B10: nop
    
            goto L_80141B44;
    }
    // 0x80141B10: nop

L_80141B14:
    // 0x80141B14: lbu         $a0, 0x0($s1)
    ctx->r4 = MEM_BU(ctx->r17, 0X0);
    // 0x80141B18: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80141B1C: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    // 0x80141B20: jal         0x80143640
    // 0x80141B24: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    func_80143640(rdram, ctx);
        goto after_12;
    // 0x80141B24: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    after_12:
    // 0x80141B28: bltz        $s0, L_80141B44
    if (SIGNED(ctx->r16) < 0) {
        // 0x80141B2C: nop
    
            goto L_80141B44;
    }
    // 0x80141B2C: nop

    // 0x80141B30: j           L_80141B14
    // 0x80141B34: nop

        goto L_80141B14;
    // 0x80141B34: nop

L_80141B38:
    // 0x80141B38: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
    // 0x80141B3C: jal         0x80143640
    // 0x80141B40: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    func_80143640(rdram, ctx);
        goto after_13;
    // 0x80141B40: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    after_13:
L_80141B44:
    // 0x80141B44: lw          $t1, 0x38($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X38);
    // 0x80141B48: addiu       $t1, $t1, -0x1
    ctx->r9 = ADD32(ctx->r9, -0X1);
    // 0x80141B4C: bgez        $t1, L_80141B38
    if (SIGNED(ctx->r9) >= 0) {
        // 0x80141B50: sw          $t1, 0x38($sp)
        MEM_W(0X38, ctx->r29) = ctx->r9;
            goto L_80141B38;
    }
    // 0x80141B50: sw          $t1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r9;
    // 0x80141B54: andi        $v0, $s3, 0x10
    ctx->r2 = ctx->r19 & 0X10;
    // 0x80141B58: beq         $v0, $zero, L_80141B90
    if (ctx->r2 == 0) {
        // 0x80141B5C: nop
    
            goto L_80141B90;
    }
    // 0x80141B5C: nop

    // 0x80141B60: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x80141B64: slt         $v0, $s2, $t1
    ctx->r2 = SIGNED(ctx->r18) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x80141B68: beq         $v0, $zero, L_80141B94
    if (ctx->r2 == 0) {
        // 0x80141B6C: addu        $s0, $s2, $zero
        ctx->r16 = ADD32(ctx->r18, 0);
            goto L_80141B94;
    }
    // 0x80141B6C: addu        $s0, $s2, $zero
    ctx->r16 = ADD32(ctx->r18, 0);
    // 0x80141B70: addiu       $a0, $zero, 0x20
    ctx->r4 = ADD32(0, 0X20);
L_80141B74:
    // 0x80141B74: jal         0x80143640
    // 0x80141B78: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    func_80143640(rdram, ctx);
        goto after_14;
    // 0x80141B78: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    after_14:
    // 0x80141B7C: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x80141B80: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80141B84: slt         $v0, $s0, $t1
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x80141B88: bne         $v0, $zero, L_80141B74
    if (ctx->r2 != 0) {
        // 0x80141B8C: addiu       $a0, $zero, 0x20
        ctx->r4 = ADD32(0, 0X20);
            goto L_80141B74;
    }
    // 0x80141B8C: addiu       $a0, $zero, 0x20
    ctx->r4 = ADD32(0, 0X20);
L_80141B90:
    // 0x80141B90: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
L_80141B94:
    // 0x80141B94: slt         $v0, $t1, $s2
    ctx->r2 = SIGNED(ctx->r9) < SIGNED(ctx->r18) ? 1 : 0;
    // 0x80141B98: movn        $t1, $s2, $v0
    if (ctx->r2 != 0) ctx->r9 = ctx->r18;
    // 0x80141B9C: addu        $v0, $t1, $zero
    ctx->r2 = ADD32(ctx->r9, 0);
    // 0x80141BA0: lw          $t1, 0x28($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X28);
    // 0x80141BA4: addu        $t1, $t1, $v0
    ctx->r9 = ADD32(ctx->r9, ctx->r2);
    // 0x80141BA8: j           L_80141BC8
    // 0x80141BAC: sw          $t1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r9;
        goto L_80141BC8;
    // 0x80141BAC: sw          $t1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r9;
L_80141BB0:
    // 0x80141BB0: lw          $t1, 0x28($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X28);
    // 0x80141BB4: lbu         $a0, 0x0($s7)
    ctx->r4 = MEM_BU(ctx->r23, 0X0);
    // 0x80141BB8: addu        $a1, $s6, $zero
    ctx->r5 = ADD32(ctx->r22, 0);
    // 0x80141BBC: addiu       $t1, $t1, 0x1
    ctx->r9 = ADD32(ctx->r9, 0X1);
    // 0x80141BC0: jal         0x80143640
    // 0x80141BC4: sw          $t1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r9;
    func_80143640(rdram, ctx);
        goto after_15;
    // 0x80141BC4: sw          $t1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r9;
    after_15:
L_80141BC8:
    // 0x80141BC8: j           L_8014134C
    // 0x80141BCC: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
        goto L_8014134C;
    // 0x80141BCC: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_80141BD0:
    // 0x80141BD0: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
    // 0x80141BD4: lw          $fp, 0x78($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X78);
    // 0x80141BD8: lw          $s7, 0x74($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X74);
    // 0x80141BDC: lw          $s6, 0x70($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X70);
    // 0x80141BE0: lw          $s5, 0x6C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X6C);
    // 0x80141BE4: lw          $s4, 0x68($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X68);
    // 0x80141BE8: lw          $s3, 0x64($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X64);
    // 0x80141BEC: lw          $s2, 0x60($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X60);
    // 0x80141BF0: lw          $s1, 0x5C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X5C);
    // 0x80141BF4: lw          $s0, 0x58($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X58);
    // 0x80141BF8: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    // 0x80141BFC: jr          $ra
    // 0x80141C00: nop

    return;
    // 0x80141C00: nop

;}
RECOMP_FUNC void func_80141C04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80141C04: addiu       $sp, $sp, -0x90
    ctx->r29 = ADD32(ctx->r29, -0X90);
    // 0x80141C08: sw          $s3, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r19;
    // 0x80141C0C: lw          $s3, 0xA8($sp)
    ctx->r19 = MEM_W(ctx->r29, 0XA8);
    // 0x80141C10: sw          $s6, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r22;
    // 0x80141C14: lbu         $s6, 0xA4($sp)
    ctx->r22 = MEM_BU(ctx->r29, 0XA4);
    // 0x80141C18: sw          $s7, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r23;
    // 0x80141C1C: lw          $s7, 0xA0($sp)
    ctx->r23 = MEM_W(ctx->r29, 0XA0);
    // 0x80141C20: sw          $s4, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r20;
    // 0x80141C24: lw          $s4, 0xAC($sp)
    ctx->r20 = MEM_W(ctx->r29, 0XAC);
    // 0x80141C28: sdc1        $f20, 0x68($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X68, ctx->r29);
    // 0x80141C2C: mov.d       $f20, $f12
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 12);
    ctx->f20.d = ctx->f12.d;
    // 0x80141C30: sw          $s2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r18;
    // 0x80141C34: addu        $s2, $a2, $zero
    ctx->r18 = ADD32(ctx->r6, 0);
    // 0x80141C38: sw          $s5, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r21;
    // 0x80141C3C: addu        $s5, $a3, $zero
    ctx->r21 = ADD32(ctx->r7, 0);
    // 0x80141C40: sw          $ra, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r31;
    // 0x80141C44: sw          $fp, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r30;
    // 0x80141C48: sw          $s1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r17;
    // 0x80141C4C: sw          $s0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r16;
    // 0x80141C50: sdc1        $f28, 0x88($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X88, ctx->r29);
    // 0x80141C54: sdc1        $f26, 0x80($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X80, ctx->r29);
    // 0x80141C58: sdc1        $f24, 0x78($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X78, ctx->r29);
    // 0x80141C5C: sdc1        $f22, 0x70($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X70, ctx->r29);
    // 0x80141C60: jal         0x8014277C
    // 0x80141C64: addu        $a2, $s3, $zero
    ctx->r6 = ADD32(ctx->r19, 0);
    func_8014277C(rdram, ctx);
        goto after_0;
    // 0x80141C64: addu        $a2, $s3, $zero
    ctx->r6 = ADD32(ctx->r19, 0);
    after_0:
    // 0x80141C68: bne         $v0, $zero, L_80142424
    if (ctx->r2 != 0) {
        // 0x80141C6C: sw          $v0, 0x30($sp)
        MEM_W(0X30, ctx->r29) = ctx->r2;
            goto L_80142424;
    }
    // 0x80141C6C: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    // 0x80141C70: addu        $fp, $zero, $zero
    ctx->r30 = ADD32(0, 0);
    // 0x80141C74: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    // 0x80141C78: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80141C7C: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x80141C80: addu        $s0, $s3, $zero
    ctx->r16 = ADD32(ctx->r19, 0);
    // 0x80141C84: c.eq.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d == ctx->f0.d;
    // 0x80141C88: sw          $zero, 0x30($sp)
    MEM_W(0X30, ctx->r29) = 0;
    // 0x80141C8C: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x80141C90: bc1t        L_80141D54
    if (c1cs) {
        // 0x80141C94: addiu       $s1, $s4, -0x1
        ctx->r17 = ADD32(ctx->r20, -0X1);
            goto L_80141D54;
    }
    // 0x80141C94: addiu       $s1, $s4, -0x1
    ctx->r17 = ADD32(ctx->r20, -0X1);
    // 0x80141C98: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80141C9C: ldc1        $f0, 0x1BA0($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X1BA0);
    // 0x80141CA0: addiu       $a2, $zero, 0xC
    ctx->r6 = ADD32(0, 0XC);
    // 0x80141CA4: addiu       $a1, $zero, 0x1000
    ctx->r5 = ADD32(0, 0X1000);
    // 0x80141CA8: c.lt.d      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.d < ctx->f20.d;
    // 0x80141CAC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80141CB0: ldc1        $f4, 0x4B78($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, 0X4B78);
    // 0x80141CB4: bc1f        L_80141D2C
    if (!c1cs) {
        // 0x80141CB8: mov.d       $f6, $f20
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    ctx->f6.d = ctx->f20.d;
            goto L_80141D2C;
    }
    // 0x80141CB8: mov.d       $f6, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    ctx->f6.d = ctx->f20.d;
    // 0x80141CBC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80141CC0: ldc1        $f0, 0x1BA8($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X1BA8);
    // 0x80141CC4: mul.d       $f0, $f20, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f0.d); 
    ctx->f0.d = MUL_D(ctx->f20.d, ctx->f0.d);
    // 0x80141CC8: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80141CCC: addiu       $v1, $v1, 0x1B30
    ctx->r3 = ADD32(ctx->r3, 0X1B30);
    // 0x80141CD0: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80141CD4: addiu       $a0, $a0, 0x1B98
    ctx->r4 = ADD32(ctx->r4, 0X1B98);
    // 0x80141CD8: sdc1        $f0, 0x20($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X20, ctx->r29);
L_80141CDC:
    // 0x80141CDC: ldc1        $f0, 0x0($v1)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r3, 0X0);
    // 0x80141CE0: ldc1        $f2, 0x20($sp)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r29, 0X20);
    // 0x80141CE4: c.le.d      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.d <= ctx->f2.d;
    // 0x80141CE8: bc1fl       L_80141D14
    if (!c1cs) {
        // 0x80141CEC: sra         $a1, $a1, 1
        ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
            goto L_80141D14;
    }
    goto skip_0;
    // 0x80141CEC: sra         $a1, $a1, 1
    ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
    skip_0:
    // 0x80141CF0: ldc1        $f0, 0x0($a0)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r4, 0X0);
    // 0x80141CF4: mul.d       $f0, $f2, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f0.d); 
    ctx->f0.d = MUL_D(ctx->f2.d, ctx->f0.d);
    // 0x80141CF8: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80141CFC: ldc1        $f2, 0x0($v1)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r3, 0X0);
    // 0x80141D00: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80141D04: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    // 0x80141D08: mul.d       $f4, $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f2.d); 
    ctx->f4.d = MUL_D(ctx->f4.d, ctx->f2.d);
    // 0x80141D0C: sdc1        $f0, 0x20($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X20, ctx->r29);
    // 0x80141D10: sra         $a1, $a1, 1
    ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
L_80141D14:
    // 0x80141D14: addiu       $v1, $v1, -0x8
    ctx->r3 = ADD32(ctx->r3, -0X8);
    // 0x80141D18: addiu       $a2, $a2, -0x1
    ctx->r6 = ADD32(ctx->r6, -0X1);
    // 0x80141D1C: bgez        $a2, L_80141CDC
    if (SIGNED(ctx->r6) >= 0) {
        // 0x80141D20: addiu       $a0, $a0, -0x8
        ctx->r4 = ADD32(ctx->r4, -0X8);
            goto L_80141CDC;
    }
    // 0x80141D20: addiu       $a0, $a0, -0x8
    ctx->r4 = ADD32(ctx->r4, -0X8);
    // 0x80141D24: div.d       $f0, $f6, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f4.d); 
    ctx->f0.d = DIV_D(ctx->f6.d, ctx->f4.d);
    // 0x80141D28: sdc1        $f0, 0x20($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X20, ctx->r29);
L_80141D2C:
    // 0x80141D2C: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80141D30: blez        $v0, L_80141D54
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80141D34: addu        $v1, $zero, $zero
        ctx->r3 = ADD32(0, 0);
            goto L_80141D54;
    }
    // 0x80141D34: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
    // 0x80141D38: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
L_80141D3C:
    // 0x80141D3C: sb          $a0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r4;
    // 0x80141D40: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80141D44: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80141D48: slt         $v0, $v1, $v0
    ctx->r2 = SIGNED(ctx->r3) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80141D4C: bne         $v0, $zero, L_80141D3C
    if (ctx->r2 != 0) {
        // 0x80141D50: addiu       $s1, $s1, -0x1
        ctx->r17 = ADD32(ctx->r17, -0X1);
            goto L_80141D3C;
    }
    // 0x80141D50: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
L_80141D54:
    // 0x80141D54: ldc1        $f12, 0x20($sp)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X20);
    // 0x80141D58: jal         0x80144C90
    // 0x80141D5C: addiu       $a2, $sp, 0x20
    ctx->r6 = ADD32(ctx->r29, 0X20);
    func_80144C90(rdram, ctx);
        goto after_1;
    // 0x80141D5C: addiu       $a2, $sp, 0x20
    ctx->r6 = ADD32(ctx->r29, 0X20);
    after_1:
    // 0x80141D60: ldc1        $f12, 0x20($sp)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X20);
    // 0x80141D64: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80141D68: mtc1        $zero, $f3
    ctx->f_odd[(3 - 1) * 2] = 0;
    // 0x80141D6C: c.eq.d      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.d == ctx->f2.d;
    // 0x80141D70: bc1t        L_80141DEC
    if (c1cs) {
        // 0x80141D74: mov.d       $f20, $f0
        CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.d = ctx->f0.d;
            goto L_80141DEC;
    }
    // 0x80141D74: mov.d       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.d = ctx->f0.d;
    // 0x80141D78: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80141D7C: ldc1        $f28, 0x4B80($at)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r1, 0X4B80);
    // 0x80141D80: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80141D84: ldc1        $f26, 0x4B88($at)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r1, 0X4B88);
    // 0x80141D88: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80141D8C: ldc1        $f24, 0x4B90($at)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r1, 0X4B90);
    // 0x80141D90: mov.d       $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    ctx->f22.d = ctx->f2.d;
L_80141D94:
    // 0x80141D94: mul.d       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.d); NAN_CHECK(ctx->f28.d); 
    ctx->f12.d = MUL_D(ctx->f12.d, ctx->f28.d);
    // 0x80141D98: jal         0x80144C90
    // 0x80141D9C: addiu       $a2, $sp, 0x20
    ctx->r6 = ADD32(ctx->r29, 0X20);
    func_80144C90(rdram, ctx);
        goto after_2;
    // 0x80141D9C: addiu       $a2, $sp, 0x20
    ctx->r6 = ADD32(ctx->r29, 0X20);
    after_2:
    // 0x80141DA0: add.d       $f2, $f0, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f26.d); 
    ctx->f2.d = ctx->f0.d + ctx->f26.d;
    // 0x80141DA4: mul.d       $f2, $f2, $f24
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f24.d); 
    ctx->f2.d = MUL_D(ctx->f2.d, ctx->f24.d);
    // 0x80141DA8: sdc1        $f0, 0x28($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X28, ctx->r29);
    // 0x80141DAC: trunc.w.d   $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.u32l = TRUNC_W_D(ctx->f2.d);
    // 0x80141DB0: mfc1        $v1, $f0
    ctx->r3 = (int32_t)ctx->f0.u32l;
    // 0x80141DB4: slti        $v0, $v1, 0x9
    ctx->r2 = SIGNED(ctx->r3) < 0X9 ? 1 : 0;
    // 0x80141DB8: beq         $v0, $zero, L_80141DCC
    if (ctx->r2 == 0) {
        // 0x80141DBC: addiu       $v0, $zero, 0x39
        ctx->r2 = ADD32(0, 0X39);
            goto L_80141DCC;
    }
    // 0x80141DBC: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
    // 0x80141DC0: bgtz        $v1, L_80141DCC
    if (SIGNED(ctx->r3) > 0) {
        // 0x80141DC4: addiu       $v0, $v1, 0x30
        ctx->r2 = ADD32(ctx->r3, 0X30);
            goto L_80141DCC;
    }
    // 0x80141DC4: addiu       $v0, $v1, 0x30
    ctx->r2 = ADD32(ctx->r3, 0X30);
    // 0x80141DC8: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_80141DCC:
    // 0x80141DCC: sb          $v0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r2;
    // 0x80141DD0: ldc1        $f12, 0x20($sp)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X20);
    // 0x80141DD4: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80141DD8: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
    // 0x80141DDC: c.eq.d      $f12, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f12.d == ctx->f22.d;
    // 0x80141DE0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80141DE4: bc1f        L_80141D94
    if (!c1cs) {
        // 0x80141DE8: sw          $v0, 0x30($sp)
        MEM_W(0X30, ctx->r29) = ctx->r2;
            goto L_80141D94;
    }
    // 0x80141DE8: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
L_80141DEC:
    // 0x80141DEC: andi        $v0, $s6, 0xFF
    ctx->r2 = ctx->r22 & 0XFF;
    // 0x80141DF0: addiu       $v1, $v0, -0x45
    ctx->r3 = ADD32(ctx->r2, -0X45);
    // 0x80141DF4: sltiu       $v0, $v1, 0x23
    ctx->r2 = ctx->r3 < 0X23 ? 1 : 0;
    // 0x80141DF8: beq         $v0, $zero, L_80142420
    if (ctx->r2 == 0) {
        // 0x80141DFC: sll         $v0, $v1, 2
        ctx->r2 = S32(ctx->r3 << 2);
            goto L_80142420;
    }
    // 0x80141DFC: sll         $v0, $v1, 2
    ctx->r2 = S32(ctx->r3 << 2);
    // 0x80141E00: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80141E04: addu        $at, $at, $v0
    gpr jr_addend_80141E0C = ctx->r2;
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80141E08: lw          $v0, 0x4B98($at)
    ctx->r2 = ADD32(ctx->r1, 0X4B98);
    // 0x80141E0C: jr          $v0
    // 0x80141E10: nop

    switch (jr_addend_80141E0C >> 2) {
        case 0: goto L_80141F4C; break;
        case 1: goto L_80142420; break;
        case 2: goto L_80142254; break;
        case 3: goto L_80142420; break;
        case 4: goto L_80142420; break;
        case 5: goto L_80142420; break;
        case 6: goto L_80142420; break;
        case 7: goto L_80142420; break;
        case 8: goto L_80142420; break;
        case 9: goto L_80142420; break;
        case 10: goto L_80142420; break;
        case 11: goto L_80142420; break;
        case 12: goto L_80142420; break;
        case 13: goto L_80142420; break;
        case 14: goto L_80142420; break;
        case 15: goto L_80142420; break;
        case 16: goto L_80142420; break;
        case 17: goto L_80142420; break;
        case 18: goto L_80142420; break;
        case 19: goto L_80142420; break;
        case 20: goto L_80142420; break;
        case 21: goto L_80142420; break;
        case 22: goto L_80142420; break;
        case 23: goto L_80142420; break;
        case 24: goto L_80142420; break;
        case 25: goto L_80142420; break;
        case 26: goto L_80142420; break;
        case 27: goto L_80142420; break;
        case 28: goto L_80142420; break;
        case 29: goto L_80142420; break;
        case 30: goto L_80142420; break;
        case 31: goto L_80142420; break;
        case 32: goto L_80141F4C; break;
        case 33: goto L_80141E14; break;
        case 34: goto L_80142254; break;
        default: switch_error(__func__, 0x80141E0C, 0x80174B98);
    }
    // 0x80141E10: nop

L_80141E14:
    // 0x80141E14: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80141E18: beq         $v0, $zero, L_80141E4C
    if (ctx->r2 == 0) {
        // 0x80141E1C: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_80141E4C;
    }
    // 0x80141E1C: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80141E20: sltu        $v0, $s1, $s4
    ctx->r2 = ctx->r17 < ctx->r20 ? 1 : 0;
    // 0x80141E24: beq         $v0, $zero, L_80141E58
    if (ctx->r2 == 0) {
        // 0x80141E28: nop
    
            goto L_80141E58;
    }
    // 0x80141E28: nop

L_80141E2C:
    // 0x80141E2C: lbu         $v0, 0x0($s1)
    ctx->r2 = MEM_BU(ctx->r17, 0X0);
    // 0x80141E30: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80141E34: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80141E38: sltu        $v0, $s1, $s4
    ctx->r2 = ctx->r17 < ctx->r20 ? 1 : 0;
    // 0x80141E3C: bne         $v0, $zero, L_80141E2C
    if (ctx->r2 != 0) {
        // 0x80141E40: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80141E2C;
    }
    // 0x80141E40: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80141E44: j           L_80141E58
    // 0x80141E48: nop

        goto L_80141E58;
    // 0x80141E48: nop

L_80141E4C:
    // 0x80141E4C: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
    // 0x80141E50: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80141E54: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80141E58:
    // 0x80141E58: bne         $s2, $zero, L_80141E68
    if (ctx->r18 != 0) {
        // 0x80141E5C: andi        $v0, $s5, 0x8
        ctx->r2 = ctx->r21 & 0X8;
            goto L_80141E68;
    }
    // 0x80141E5C: andi        $v0, $s5, 0x8
    ctx->r2 = ctx->r21 & 0X8;
    // 0x80141E60: beq         $v0, $zero, L_80141E78
    if (ctx->r2 == 0) {
        // 0x80141E64: nop
    
            goto L_80141E78;
    }
    // 0x80141E64: nop

L_80141E68:
    // 0x80141E68: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80141E6C: lbu         $v0, 0x1AC0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X1AC0);
    // 0x80141E70: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80141E74: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80141E78:
    // 0x80141E78: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80141E7C: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x80141E80: c.eq.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d == ctx->f0.d;
    // 0x80141E84: bc1tl       L_80141F28
    if (c1cs) {
        // 0x80141E88: addiu       $s2, $s2, -0x1
        ctx->r18 = ADD32(ctx->r18, -0X1);
            goto L_80141F28;
    }
    goto skip_1;
    // 0x80141E88: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    skip_1:
    // 0x80141E8C: beq         $s2, $zero, L_80141EFC
    if (ctx->r18 == 0) {
        // 0x80141E90: nop
    
            goto L_80141EFC;
    }
    // 0x80141E90: nop

    // 0x80141E94: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80141E98: ldc1        $f24, 0x4C28($at)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r1, 0X4C28);
    // 0x80141E9C: mov.d       $f22, $f0
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    ctx->f22.d = ctx->f0.d;
    // 0x80141EA0: mul.d       $f12, $f20, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f24.d); 
    ctx->f12.d = MUL_D(ctx->f20.d, ctx->f24.d);
L_80141EA4:
    // 0x80141EA4: jal         0x80144C90
    // 0x80141EA8: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    func_80144C90(rdram, ctx);
        goto after_3;
    // 0x80141EA8: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    after_3:
    // 0x80141EAC: ldc1        $f2, 0x28($sp)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r29, 0X28);
    // 0x80141EB0: mov.d       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.d = ctx->f0.d;
    // 0x80141EB4: trunc.w.d   $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.u32l = TRUNC_W_D(ctx->f2.d);
    // 0x80141EB8: mfc1        $v1, $f0
    ctx->r3 = (int32_t)ctx->f0.u32l;
    // 0x80141EBC: slti        $v0, $v1, 0x9
    ctx->r2 = SIGNED(ctx->r3) < 0X9 ? 1 : 0;
    // 0x80141EC0: beq         $v0, $zero, L_80141ED4
    if (ctx->r2 == 0) {
        // 0x80141EC4: addiu       $v0, $zero, 0x39
        ctx->r2 = ADD32(0, 0X39);
            goto L_80141ED4;
    }
    // 0x80141EC4: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
    // 0x80141EC8: bgtz        $v1, L_80141ED4
    if (SIGNED(ctx->r3) > 0) {
        // 0x80141ECC: addiu       $v0, $v1, 0x30
        ctx->r2 = ADD32(ctx->r3, 0X30);
            goto L_80141ED4;
    }
    // 0x80141ECC: addiu       $v0, $v1, 0x30
    ctx->r2 = ADD32(ctx->r3, 0X30);
    // 0x80141ED0: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_80141ED4:
    // 0x80141ED4: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80141ED8: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x80141EDC: beq         $s2, $zero, L_80141EF0
    if (ctx->r18 == 0) {
        // 0x80141EE0: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80141EF0;
    }
    // 0x80141EE0: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80141EE4: c.eq.d      $f20, $f22
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f20.d == ctx->f22.d;
    // 0x80141EE8: bc1fl       L_80141EA4
    if (!c1cs) {
        // 0x80141EEC: mul.d       $f12, $f20, $f24
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f24.d); 
    ctx->f12.d = MUL_D(ctx->f20.d, ctx->f24.d);
            goto L_80141EA4;
    }
    goto skip_2;
    // 0x80141EEC: mul.d       $f12, $f20, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f24.d); 
    ctx->f12.d = MUL_D(ctx->f20.d, ctx->f24.d);
    skip_2:
L_80141EF0:
    // 0x80141EF0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80141EF4: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x80141EF8: c.eq.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d == ctx->f0.d;
L_80141EFC:
    // 0x80141EFC: bc1t        L_80141F24
    if (c1cs) {
        // 0x80141F00: mov.d       $f12, $f20
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.d = ctx->f20.d;
            goto L_80141F24;
    }
    // 0x80141F00: mov.d       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.d = ctx->f20.d;
    // 0x80141F04: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    // 0x80141F08: addu        $a3, $s3, $zero
    ctx->r7 = ADD32(ctx->r19, 0);
    // 0x80141F0C: addiu       $v0, $s0, -0x1
    ctx->r2 = ADD32(ctx->r16, -0X1);
    // 0x80141F10: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    // 0x80141F14: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    // 0x80141F18: jal         0x8014246C
    // 0x80141F1C: sw          $s7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r23;
    static_0_8014246C(rdram, ctx);
        goto after_4;
    // 0x80141F1C: sw          $s7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r23;
    after_4:
    // 0x80141F20: addu        $s3, $v0, $zero
    ctx->r19 = ADD32(ctx->r2, 0);
L_80141F24:
    // 0x80141F24: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
L_80141F28:
    // 0x80141F28: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80141F2C: beq         $s2, $v0, L_80142420
    if (ctx->r18 == ctx->r2) {
        // 0x80141F30: addiu       $v1, $zero, 0x30
        ctx->r3 = ADD32(0, 0X30);
            goto L_80142420;
    }
    // 0x80141F30: addiu       $v1, $zero, 0x30
    ctx->r3 = ADD32(0, 0X30);
L_80141F34:
    // 0x80141F34: sb          $v1, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r3;
    // 0x80141F38: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x80141F3C: bne         $s2, $v0, L_80141F34
    if (ctx->r18 != ctx->r2) {
        // 0x80141F40: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80141F34;
    }
    // 0x80141F40: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80141F44: j           L_80142424
    // 0x80141F48: subu        $v0, $s0, $s3
    ctx->r2 = SUB32(ctx->r16, ctx->r19);
        goto L_80142424;
    // 0x80141F48: subu        $v0, $s0, $s3
    ctx->r2 = SUB32(ctx->r16, ctx->r19);
L_80141F4C:
    // 0x80141F4C: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80141F50: beq         $v0, $zero, L_80142000
    if (ctx->r2 == 0) {
        // 0x80141F54: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_80142000;
    }
    // 0x80141F54: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80141F58: lbu         $v0, 0x0($s1)
    ctx->r2 = MEM_BU(ctx->r17, 0X0);
    // 0x80141F5C: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80141F60: bne         $s2, $zero, L_80141F74
    if (ctx->r18 != 0) {
        // 0x80141F64: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80141F74;
    }
    // 0x80141F64: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80141F68: andi        $v0, $s5, 0x8
    ctx->r2 = ctx->r21 & 0X8;
    // 0x80141F6C: beq         $v0, $zero, L_80141F84
    if (ctx->r2 == 0) {
        // 0x80141F70: nop
    
            goto L_80141F84;
    }
    // 0x80141F70: nop

L_80141F74:
    // 0x80141F74: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80141F78: lbu         $v0, 0x1AC0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X1AC0);
    // 0x80141F7C: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80141F80: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80141F84:
    // 0x80141F84: beql        $s2, $zero, L_80141FB8
    if (ctx->r18 == 0) {
        // 0x80141F88: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_80141FB8;
    }
    goto skip_3;
    // 0x80141F88: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_3:
L_80141F8C:
    // 0x80141F8C: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80141F90: sltu        $v0, $s1, $s4
    ctx->r2 = ctx->r17 < ctx->r20 ? 1 : 0;
    // 0x80141F94: beq         $v0, $zero, L_80141FB0
    if (ctx->r2 == 0) {
        // 0x80141F98: nop
    
            goto L_80141FB0;
    }
    // 0x80141F98: nop

    // 0x80141F9C: lbu         $v0, 0x0($s1)
    ctx->r2 = MEM_BU(ctx->r17, 0X0);
    // 0x80141FA0: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x80141FA4: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80141FA8: bne         $s2, $zero, L_80141F8C
    if (ctx->r18 != 0) {
        // 0x80141FAC: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80141F8C;
    }
    // 0x80141FAC: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80141FB0:
    // 0x80141FB0: bne         $s2, $zero, L_80141FF0
    if (ctx->r18 != 0) {
        // 0x80141FB4: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_80141FF0;
    }
    // 0x80141FB4: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
L_80141FB8:
    // 0x80141FB8: sltu        $v0, $s1, $s4
    ctx->r2 = ctx->r17 < ctx->r20 ? 1 : 0;
    // 0x80141FBC: beq         $v0, $zero, L_80141FF0
    if (ctx->r2 == 0) {
        // 0x80141FC0: addiu       $v0, $s0, -0x1
        ctx->r2 = ADD32(ctx->r16, -0X1);
            goto L_80141FF0;
    }
    // 0x80141FC0: addiu       $v0, $s0, -0x1
    ctx->r2 = ADD32(ctx->r16, -0X1);
    // 0x80141FC4: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x80141FC8: mtc1        $zero, $f21
    ctx->f_odd[(21 - 1) * 2] = 0;
    // 0x80141FCC: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    // 0x80141FD0: lbu         $v0, 0x0($s1)
    ctx->r2 = MEM_BU(ctx->r17, 0X0);
    // 0x80141FD4: addiu       $a2, $sp, 0x30
    ctx->r6 = ADD32(ctx->r29, 0X30);
    // 0x80141FD8: addu        $a3, $s3, $zero
    ctx->r7 = ADD32(ctx->r19, 0);
    // 0x80141FDC: sw          $s7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r23;
    // 0x80141FE0: mov.d       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.d = ctx->f20.d;
    // 0x80141FE4: jal         0x8014246C
    // 0x80141FE8: sw          $v0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r2;
    static_0_8014246C(rdram, ctx);
        goto after_5;
    // 0x80141FE8: sw          $v0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r2;
    after_5:
    // 0x80141FEC: addu        $s3, $v0, $zero
    ctx->r19 = ADD32(ctx->r2, 0);
L_80141FF0:
    // 0x80141FF0: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80141FF4: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80141FF8: j           L_80142124
    // 0x80141FFC: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
        goto L_80142124;
    // 0x80141FFC: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
L_80142000:
    // 0x80142000: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80142004: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x80142008: c.eq.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d == ctx->f0.d;
    // 0x8014200C: bc1t        L_801420F8
    if (c1cs) {
        // 0x80142010: addiu       $a2, $zero, 0xC
        ctx->r6 = ADD32(0, 0XC);
            goto L_801420F8;
    }
    // 0x80142010: addiu       $a2, $zero, 0xC
    ctx->r6 = ADD32(0, 0XC);
    // 0x80142014: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142018: ldc1        $f0, 0x1BA8($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X1BA8);
    // 0x8014201C: addiu       $a1, $zero, 0x1000
    ctx->r5 = ADD32(0, 0X1000);
    // 0x80142020: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80142024: ldc1        $f2, 0x4C30($at)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r1, 0X4C30);
    // 0x80142028: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x8014202C: c.lt.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d < ctx->f0.d;
    // 0x80142030: mov.d       $f4, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    ctx->f4.d = ctx->f20.d;
    // 0x80142034: bc1f        L_80142098
    if (!c1cs) {
        // 0x80142038: sw          $v0, 0x30($sp)
        MEM_W(0X30, ctx->r29) = ctx->r2;
            goto L_80142098;
    }
    // 0x80142038: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    // 0x8014203C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142040: ldc1        $f0, 0x1BA0($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X1BA0);
    // 0x80142044: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80142048: addiu       $a0, $a0, 0x1B30
    ctx->r4 = ADD32(ctx->r4, 0X1B30);
    // 0x8014204C: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80142050: addiu       $v1, $v1, 0x1B98
    ctx->r3 = ADD32(ctx->r3, 0X1B98);
    // 0x80142054: mul.d       $f20, $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f0.d); 
    ctx->f20.d = MUL_D(ctx->f20.d, ctx->f0.d);
L_80142058:
    // 0x80142058: ldc1        $f0, 0x0($v1)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r3, 0X0);
    // 0x8014205C: c.le.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d <= ctx->f0.d;
    // 0x80142060: bc1fl       L_80142084
    if (!c1cs) {
        // 0x80142064: sra         $a1, $a1, 1
        ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
            goto L_80142084;
    }
    goto skip_4;
    // 0x80142064: sra         $a1, $a1, 1
    ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
    skip_4:
    // 0x80142068: ldc1        $f0, 0x0($a0)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r4, 0X0);
    // 0x8014206C: mul.d       $f20, $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f0.d); 
    ctx->f20.d = MUL_D(ctx->f20.d, ctx->f0.d);
    // 0x80142070: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80142074: subu        $v0, $v0, $a1
    ctx->r2 = SUB32(ctx->r2, ctx->r5);
    // 0x80142078: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    // 0x8014207C: mul.d       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f0.d); 
    ctx->f2.d = MUL_D(ctx->f2.d, ctx->f0.d);
    // 0x80142080: sra         $a1, $a1, 1
    ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
L_80142084:
    // 0x80142084: addiu       $a0, $a0, -0x8
    ctx->r4 = ADD32(ctx->r4, -0X8);
    // 0x80142088: addiu       $a2, $a2, -0x1
    ctx->r6 = ADD32(ctx->r6, -0X1);
    // 0x8014208C: bgez        $a2, L_80142058
    if (SIGNED(ctx->r6) >= 0) {
        // 0x80142090: addiu       $v1, $v1, -0x8
        ctx->r3 = ADD32(ctx->r3, -0X8);
            goto L_80142058;
    }
    // 0x80142090: addiu       $v1, $v1, -0x8
    ctx->r3 = ADD32(ctx->r3, -0X8);
    // 0x80142094: mul.d       $f20, $f4, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f2.d); 
    ctx->f20.d = MUL_D(ctx->f4.d, ctx->f2.d);
L_80142098:
    // 0x80142098: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014209C: ldc1        $f24, 0x4C38($at)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r1, 0X4C38);
    // 0x801420A0: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x801420A4: mtc1        $zero, $f23
    ctx->f_odd[(23 - 1) * 2] = 0;
L_801420A8:
    // 0x801420A8: mul.d       $f12, $f20, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f24.d); 
    ctx->f12.d = MUL_D(ctx->f20.d, ctx->f24.d);
    // 0x801420AC: jal         0x80144C90
    // 0x801420B0: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    func_80144C90(rdram, ctx);
        goto after_6;
    // 0x801420B0: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    after_6:
    // 0x801420B4: ldc1        $f2, 0x28($sp)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r29, 0X28);
    // 0x801420B8: c.eq.d      $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f2.d == ctx->f22.d;
    // 0x801420BC: bc1f        L_801420D4
    if (!c1cs) {
        // 0x801420C0: mov.d       $f20, $f0
        CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.d = ctx->f0.d;
            goto L_801420D4;
    }
    // 0x801420C0: mov.d       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.d = ctx->f0.d;
    // 0x801420C4: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x801420C8: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x801420CC: j           L_801420A8
    // 0x801420D0: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
        goto L_801420A8;
    // 0x801420D0: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
L_801420D4:
    // 0x801420D4: trunc.w.d   $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.u32l = TRUNC_W_D(ctx->f2.d);
    // 0x801420D8: mfc1        $v1, $f0
    ctx->r3 = (int32_t)ctx->f0.u32l;
    // 0x801420DC: slti        $v0, $v1, 0x9
    ctx->r2 = SIGNED(ctx->r3) < 0X9 ? 1 : 0;
    // 0x801420E0: beq         $v0, $zero, L_801420FC
    if (ctx->r2 == 0) {
        // 0x801420E4: addiu       $v0, $zero, 0x39
        ctx->r2 = ADD32(0, 0X39);
            goto L_801420FC;
    }
    // 0x801420E4: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
    // 0x801420E8: bgtz        $v1, L_801420FC
    if (SIGNED(ctx->r3) > 0) {
        // 0x801420EC: addiu       $v0, $v1, 0x30
        ctx->r2 = ADD32(ctx->r3, 0X30);
            goto L_801420FC;
    }
    // 0x801420EC: addiu       $v0, $v1, 0x30
    ctx->r2 = ADD32(ctx->r3, 0X30);
    // 0x801420F0: j           L_801420FC
    // 0x801420F4: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
        goto L_801420FC;
    // 0x801420F4: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_801420F8:
    // 0x801420F8: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_801420FC:
    // 0x801420FC: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80142100: bne         $s2, $zero, L_80142114
    if (ctx->r18 != 0) {
        // 0x80142104: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80142114;
    }
    // 0x80142104: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80142108: andi        $v0, $s5, 0x8
    ctx->r2 = ctx->r21 & 0X8;
    // 0x8014210C: beq         $v0, $zero, L_80142124
    if (ctx->r2 == 0) {
        // 0x80142110: nop
    
            goto L_80142124;
    }
    // 0x80142110: nop

L_80142114:
    // 0x80142114: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142118: lbu         $v0, 0x1AC0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X1AC0);
    // 0x8014211C: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80142120: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80142124:
    // 0x80142124: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80142128: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x8014212C: c.eq.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d == ctx->f0.d;
    // 0x80142130: bc1tl       L_801421D4
    if (c1cs) {
        // 0x80142134: addiu       $s2, $s2, -0x1
        ctx->r18 = ADD32(ctx->r18, -0X1);
            goto L_801421D4;
    }
    goto skip_5;
    // 0x80142134: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    skip_5:
    // 0x80142138: beq         $s2, $zero, L_801421A8
    if (ctx->r18 == 0) {
        // 0x8014213C: nop
    
            goto L_801421A8;
    }
    // 0x8014213C: nop

    // 0x80142140: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80142144: ldc1        $f24, 0x4C40($at)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r1, 0X4C40);
    // 0x80142148: mov.d       $f22, $f0
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    ctx->f22.d = ctx->f0.d;
    // 0x8014214C: mul.d       $f12, $f20, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f24.d); 
    ctx->f12.d = MUL_D(ctx->f20.d, ctx->f24.d);
L_80142150:
    // 0x80142150: jal         0x80144C90
    // 0x80142154: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    func_80144C90(rdram, ctx);
        goto after_7;
    // 0x80142154: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    after_7:
    // 0x80142158: ldc1        $f2, 0x28($sp)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r29, 0X28);
    // 0x8014215C: mov.d       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.d = ctx->f0.d;
    // 0x80142160: trunc.w.d   $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.u32l = TRUNC_W_D(ctx->f2.d);
    // 0x80142164: mfc1        $v1, $f0
    ctx->r3 = (int32_t)ctx->f0.u32l;
    // 0x80142168: slti        $v0, $v1, 0x9
    ctx->r2 = SIGNED(ctx->r3) < 0X9 ? 1 : 0;
    // 0x8014216C: beq         $v0, $zero, L_80142180
    if (ctx->r2 == 0) {
        // 0x80142170: addiu       $v0, $zero, 0x39
        ctx->r2 = ADD32(0, 0X39);
            goto L_80142180;
    }
    // 0x80142170: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
    // 0x80142174: bgtz        $v1, L_80142180
    if (SIGNED(ctx->r3) > 0) {
        // 0x80142178: addiu       $v0, $v1, 0x30
        ctx->r2 = ADD32(ctx->r3, 0X30);
            goto L_80142180;
    }
    // 0x80142178: addiu       $v0, $v1, 0x30
    ctx->r2 = ADD32(ctx->r3, 0X30);
    // 0x8014217C: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_80142180:
    // 0x80142180: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80142184: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x80142188: beq         $s2, $zero, L_8014219C
    if (ctx->r18 == 0) {
        // 0x8014218C: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_8014219C;
    }
    // 0x8014218C: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80142190: c.eq.d      $f20, $f22
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f20.d == ctx->f22.d;
    // 0x80142194: bc1fl       L_80142150
    if (!c1cs) {
        // 0x80142198: mul.d       $f12, $f20, $f24
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f24.d); 
    ctx->f12.d = MUL_D(ctx->f20.d, ctx->f24.d);
            goto L_80142150;
    }
    goto skip_6;
    // 0x80142198: mul.d       $f12, $f20, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f24.d); 
    ctx->f12.d = MUL_D(ctx->f20.d, ctx->f24.d);
    skip_6:
L_8014219C:
    // 0x8014219C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801421A0: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x801421A4: c.eq.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d == ctx->f0.d;
L_801421A8:
    // 0x801421A8: bc1t        L_801421D0
    if (c1cs) {
        // 0x801421AC: mov.d       $f12, $f20
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.d = ctx->f20.d;
            goto L_801421D0;
    }
    // 0x801421AC: mov.d       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.d = ctx->f20.d;
    // 0x801421B0: addiu       $a2, $sp, 0x30
    ctx->r6 = ADD32(ctx->r29, 0X30);
    // 0x801421B4: addu        $a3, $s3, $zero
    ctx->r7 = ADD32(ctx->r19, 0);
    // 0x801421B8: addiu       $v0, $s0, -0x1
    ctx->r2 = ADD32(ctx->r16, -0X1);
    // 0x801421BC: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    // 0x801421C0: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    // 0x801421C4: jal         0x8014246C
    // 0x801421C8: sw          $s7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r23;
    static_0_8014246C(rdram, ctx);
        goto after_8;
    // 0x801421C8: sw          $s7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r23;
    after_8:
    // 0x801421CC: addu        $s3, $v0, $zero
    ctx->r19 = ADD32(ctx->r2, 0);
L_801421D0:
    // 0x801421D0: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
L_801421D4:
    // 0x801421D4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x801421D8: beq         $s2, $v0, L_801421F4
    if (ctx->r18 == ctx->r2) {
        // 0x801421DC: nop
    
            goto L_801421F4;
    }
    // 0x801421DC: nop

    // 0x801421E0: addiu       $v1, $zero, 0x30
    ctx->r3 = ADD32(0, 0X30);
L_801421E4:
    // 0x801421E4: sb          $v1, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r3;
    // 0x801421E8: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x801421EC: bne         $s2, $v0, L_801421E4
    if (ctx->r18 != ctx->r2) {
        // 0x801421F0: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_801421E4;
    }
    // 0x801421F0: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_801421F4:
    // 0x801421F4: beq         $fp, $zero, L_8014223C
    if (ctx->r30 == 0) {
        // 0x801421F8: andi        $v0, $s5, 0x8
        ctx->r2 = ctx->r21 & 0X8;
            goto L_8014223C;
    }
    // 0x801421F8: andi        $v0, $s5, 0x8
    ctx->r2 = ctx->r21 & 0X8;
    // 0x801421FC: bne         $v0, $zero, L_8014223C
    if (ctx->r2 != 0) {
        // 0x80142200: sltu        $v0, $s3, $s0
        ctx->r2 = ctx->r19 < ctx->r16 ? 1 : 0;
            goto L_8014223C;
    }
    // 0x80142200: sltu        $v0, $s3, $s0
    ctx->r2 = ctx->r19 < ctx->r16 ? 1 : 0;
    // 0x80142204: beq         $v0, $zero, L_80142224
    if (ctx->r2 == 0) {
        // 0x80142208: addiu       $v1, $zero, 0x30
        ctx->r3 = ADD32(0, 0X30);
            goto L_80142224;
    }
    // 0x80142208: addiu       $v1, $zero, 0x30
    ctx->r3 = ADD32(0, 0X30);
    // 0x8014220C: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
L_80142210:
    // 0x80142210: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
    // 0x80142214: bne         $v0, $v1, L_80142224
    if (ctx->r2 != ctx->r3) {
        // 0x80142218: sltu        $v0, $s3, $s0
        ctx->r2 = ctx->r19 < ctx->r16 ? 1 : 0;
            goto L_80142224;
    }
    // 0x80142218: sltu        $v0, $s3, $s0
    ctx->r2 = ctx->r19 < ctx->r16 ? 1 : 0;
    // 0x8014221C: bnel        $v0, $zero, L_80142210
    if (ctx->r2 != 0) {
        // 0x80142220: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_80142210;
    }
    goto skip_7;
    // 0x80142220: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    skip_7:
L_80142224:
    // 0x80142224: lbu         $v1, 0x0($s0)
    ctx->r3 = MEM_BU(ctx->r16, 0X0);
    // 0x80142228: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8014222C: lbu         $v0, 0x1AC0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X1AC0);
    // 0x80142230: beql        $v1, $v0, L_80142238
    if (ctx->r3 == ctx->r2) {
        // 0x80142234: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_80142238;
    }
    goto skip_8;
    // 0x80142234: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    skip_8:
L_80142238:
    // 0x80142238: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_8014223C:
    // 0x8014223C: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    // 0x80142240: andi        $a2, $s6, 0xFF
    ctx->r6 = ctx->r22 & 0XFF;
    // 0x80142244: jal         0x80142648
    // 0x80142248: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_80142648(rdram, ctx);
        goto after_9;
    // 0x80142248: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_9:
    // 0x8014224C: j           L_80142420
    // 0x80142250: addu        $s0, $v0, $zero
    ctx->r16 = ADD32(ctx->r2, 0);
        goto L_80142420;
    // 0x80142250: addu        $s0, $v0, $zero
    ctx->r16 = ADD32(ctx->r2, 0);
L_80142254:
    // 0x80142254: lw          $v1, 0x30($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X30);
    // 0x80142258: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8014225C: movz        $s2, $v0, $s2
    if (ctx->r18 == 0) ctx->r18 = ctx->r2;
    // 0x80142260: slt         $v0, $s2, $v1
    ctx->r2 = SIGNED(ctx->r18) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x80142264: bnel        $v0, $zero, L_801422A0
    if (ctx->r2 != 0) {
        // 0x80142268: addiu       $s2, $s2, -0x1
        ctx->r18 = ADD32(ctx->r18, -0X1);
            goto L_801422A0;
    }
    goto skip_9;
    // 0x80142268: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    skip_9:
    // 0x8014226C: bnel        $v1, $zero, L_801422B8
    if (ctx->r3 != 0) {
        // 0x80142270: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_801422B8;
    }
    goto skip_10;
    // 0x80142270: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_10:
    // 0x80142274: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80142278: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x8014227C: c.eq.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d == ctx->f0.d;
    // 0x80142280: bc1t        L_801422AC
    if (c1cs) {
        // 0x80142284: nop
    
            goto L_801422AC;
    }
    // 0x80142284: nop

    // 0x80142288: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014228C: ldc1        $f0, 0x4C48($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X4C48);
    // 0x80142290: c.lt.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d < ctx->f0.d;
    // 0x80142294: bc1f        L_801422AC
    if (!c1cs) {
        // 0x80142298: nop
    
            goto L_801422AC;
    }
    // 0x80142298: nop

    // 0x8014229C: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
L_801422A0:
    // 0x801422A0: addiu       $s6, $s6, -0x2
    ctx->r22 = ADD32(ctx->r22, -0X2);
    // 0x801422A4: j           L_80141F4C
    // 0x801422A8: addiu       $fp, $zero, 0x1
    ctx->r30 = ADD32(0, 0X1);
        goto L_80141F4C;
    // 0x801422A8: addiu       $fp, $zero, 0x1
    ctx->r30 = ADD32(0, 0X1);
L_801422AC:
    // 0x801422AC: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x801422B0: beq         $v0, $zero, L_801422E8
    if (ctx->r2 == 0) {
        // 0x801422B4: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_801422E8;
    }
    // 0x801422B4: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
L_801422B8:
    // 0x801422B8: sltu        $v0, $s1, $s4
    ctx->r2 = ctx->r17 < ctx->r20 ? 1 : 0;
    // 0x801422BC: beq         $v0, $zero, L_801422F4
    if (ctx->r2 == 0) {
        // 0x801422C0: nop
    
            goto L_801422F4;
    }
    // 0x801422C0: nop

L_801422C4:
    // 0x801422C4: lbu         $v0, 0x0($s1)
    ctx->r2 = MEM_BU(ctx->r17, 0X0);
    // 0x801422C8: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x801422CC: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x801422D0: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x801422D4: sltu        $v0, $s1, $s4
    ctx->r2 = ctx->r17 < ctx->r20 ? 1 : 0;
    // 0x801422D8: bne         $v0, $zero, L_801422C4
    if (ctx->r2 != 0) {
        // 0x801422DC: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_801422C4;
    }
    // 0x801422DC: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x801422E0: j           L_801422F4
    // 0x801422E4: nop

        goto L_801422F4;
    // 0x801422E4: nop

L_801422E8:
    // 0x801422E8: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
    // 0x801422EC: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x801422F0: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_801422F4:
    // 0x801422F4: bne         $s2, $zero, L_80142304
    if (ctx->r18 != 0) {
        // 0x801422F8: andi        $v0, $s5, 0x8
        ctx->r2 = ctx->r21 & 0X8;
            goto L_80142304;
    }
    // 0x801422F8: andi        $v0, $s5, 0x8
    ctx->r2 = ctx->r21 & 0X8;
    // 0x801422FC: beq         $v0, $zero, L_80142318
    if (ctx->r2 == 0) {
        // 0x80142300: addu        $s1, $zero, $zero
        ctx->r17 = ADD32(0, 0);
            goto L_80142318;
    }
    // 0x80142300: addu        $s1, $zero, $zero
    ctx->r17 = ADD32(0, 0);
L_80142304:
    // 0x80142304: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142308: lbu         $v0, 0x1AC0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X1AC0);
    // 0x8014230C: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
    // 0x80142310: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80142314: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80142318:
    // 0x80142318: beq         $s2, $zero, L_80142380
    if (ctx->r18 == 0) {
        // 0x8014231C: nop
    
            goto L_80142380;
    }
    // 0x8014231C: nop

    // 0x80142320: mtc1        $zero, $f24
    ctx->f24.u32l = 0;
    // 0x80142324: mtc1        $zero, $f25
    ctx->f_odd[(25 - 1) * 2] = 0;
    // 0x80142328: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014232C: ldc1        $f22, 0x4C50($at)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r1, 0X4C50);
L_80142330:
    // 0x80142330: c.eq.d      $f20, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    c1cs = ctx->f20.d == ctx->f24.d;
    // 0x80142334: bc1t        L_80142380
    if (c1cs) {
        // 0x80142338: nop
    
            goto L_80142380;
    }
    // 0x80142338: nop

    // 0x8014233C: mul.d       $f12, $f20, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f20.d); NAN_CHECK(ctx->f22.d); 
    ctx->f12.d = MUL_D(ctx->f20.d, ctx->f22.d);
    // 0x80142340: jal         0x80144C90
    // 0x80142344: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    func_80144C90(rdram, ctx);
        goto after_10;
    // 0x80142344: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    after_10:
    // 0x80142348: ldc1        $f2, 0x28($sp)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r29, 0X28);
    // 0x8014234C: mov.d       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.d = ctx->f0.d;
    // 0x80142350: trunc.w.d   $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.u32l = TRUNC_W_D(ctx->f2.d);
    // 0x80142354: mfc1        $v1, $f0
    ctx->r3 = (int32_t)ctx->f0.u32l;
    // 0x80142358: slti        $v0, $v1, 0x9
    ctx->r2 = SIGNED(ctx->r3) < 0X9 ? 1 : 0;
    // 0x8014235C: beq         $v0, $zero, L_80142370
    if (ctx->r2 == 0) {
        // 0x80142360: addiu       $v0, $zero, 0x39
        ctx->r2 = ADD32(0, 0X39);
            goto L_80142370;
    }
    // 0x80142360: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
    // 0x80142364: bgtz        $v1, L_80142370
    if (SIGNED(ctx->r3) > 0) {
        // 0x80142368: addiu       $v0, $v1, 0x30
        ctx->r2 = ADD32(ctx->r3, 0X30);
            goto L_80142370;
    }
    // 0x80142368: addiu       $v0, $v1, 0x30
    ctx->r2 = ADD32(ctx->r3, 0X30);
    // 0x8014236C: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_80142370:
    // 0x80142370: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x80142374: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x80142378: bne         $s2, $zero, L_80142330
    if (ctx->r18 != 0) {
        // 0x8014237C: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80142330;
    }
    // 0x8014237C: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80142380:
    // 0x80142380: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80142384: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x80142388: c.eq.d      $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f20.d == ctx->f0.d;
    // 0x8014238C: bc1t        L_801423B4
    if (c1cs) {
        // 0x80142390: mov.d       $f12, $f20
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.d = ctx->f20.d;
            goto L_801423B4;
    }
    // 0x80142390: mov.d       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.d = ctx->f20.d;
    // 0x80142394: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    // 0x80142398: addu        $a3, $s3, $zero
    ctx->r7 = ADD32(ctx->r19, 0);
    // 0x8014239C: addiu       $v0, $s0, -0x1
    ctx->r2 = ADD32(ctx->r16, -0X1);
    // 0x801423A0: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    // 0x801423A4: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    // 0x801423A8: jal         0x8014246C
    // 0x801423AC: sw          $s7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r23;
    static_0_8014246C(rdram, ctx);
        goto after_11;
    // 0x801423AC: sw          $s7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r23;
    after_11:
    // 0x801423B0: addu        $s3, $v0, $zero
    ctx->r19 = ADD32(ctx->r2, 0);
L_801423B4:
    // 0x801423B4: andi        $v0, $s5, 0x8
    ctx->r2 = ctx->r21 & 0X8;
    // 0x801423B8: beq         $v0, $zero, L_801423E4
    if (ctx->r2 == 0) {
        // 0x801423BC: addiu       $s2, $s2, -0x1
        ctx->r18 = ADD32(ctx->r18, -0X1);
            goto L_801423E4;
    }
    // 0x801423BC: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x801423C0: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x801423C4: beq         $s2, $v0, L_80142420
    if (ctx->r18 == ctx->r2) {
        // 0x801423C8: addiu       $v1, $zero, 0x30
        ctx->r3 = ADD32(0, 0X30);
            goto L_80142420;
    }
    // 0x801423C8: addiu       $v1, $zero, 0x30
    ctx->r3 = ADD32(0, 0X30);
L_801423CC:
    // 0x801423CC: sb          $v1, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r3;
    // 0x801423D0: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x801423D4: bne         $s2, $v0, L_801423CC
    if (ctx->r18 != ctx->r2) {
        // 0x801423D8: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_801423CC;
    }
    // 0x801423D8: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x801423DC: j           L_80142424
    // 0x801423E0: subu        $v0, $s0, $s3
    ctx->r2 = SUB32(ctx->r16, ctx->r19);
        goto L_80142424;
    // 0x801423E0: subu        $v0, $s0, $s3
    ctx->r2 = SUB32(ctx->r16, ctx->r19);
L_801423E4:
    // 0x801423E4: beq         $s1, $zero, L_80142420
    if (ctx->r17 == 0) {
        // 0x801423E8: sltu        $v0, $s3, $s0
        ctx->r2 = ctx->r19 < ctx->r16 ? 1 : 0;
            goto L_80142420;
    }
    // 0x801423E8: sltu        $v0, $s3, $s0
    ctx->r2 = ctx->r19 < ctx->r16 ? 1 : 0;
    // 0x801423EC: beq         $v0, $zero, L_8014240C
    if (ctx->r2 == 0) {
        // 0x801423F0: addiu       $v1, $zero, 0x30
        ctx->r3 = ADD32(0, 0X30);
            goto L_8014240C;
    }
    // 0x801423F0: addiu       $v1, $zero, 0x30
    ctx->r3 = ADD32(0, 0X30);
    // 0x801423F4: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
L_801423F8:
    // 0x801423F8: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
    // 0x801423FC: bne         $v0, $v1, L_8014240C
    if (ctx->r2 != ctx->r3) {
        // 0x80142400: sltu        $v0, $s3, $s0
        ctx->r2 = ctx->r19 < ctx->r16 ? 1 : 0;
            goto L_8014240C;
    }
    // 0x80142400: sltu        $v0, $s3, $s0
    ctx->r2 = ctx->r19 < ctx->r16 ? 1 : 0;
    // 0x80142404: bnel        $v0, $zero, L_801423F8
    if (ctx->r2 != 0) {
        // 0x80142408: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_801423F8;
    }
    goto skip_11;
    // 0x80142408: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    skip_11:
L_8014240C:
    // 0x8014240C: lbu         $v1, 0x0($s0)
    ctx->r3 = MEM_BU(ctx->r16, 0X0);
    // 0x80142410: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142414: lbu         $v0, 0x1AC0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X1AC0);
    // 0x80142418: bnel        $v1, $v0, L_80142420
    if (ctx->r3 != ctx->r2) {
        // 0x8014241C: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80142420;
    }
    goto skip_12;
    // 0x8014241C: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_12:
L_80142420:
    // 0x80142420: subu        $v0, $s0, $s3
    ctx->r2 = SUB32(ctx->r16, ctx->r19);
L_80142424:
    // 0x80142424: lw          $ra, 0x64($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X64);
    // 0x80142428: lw          $fp, 0x60($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X60);
    // 0x8014242C: lw          $s7, 0x5C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X5C);
    // 0x80142430: lw          $s6, 0x58($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X58);
    // 0x80142434: lw          $s5, 0x54($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X54);
    // 0x80142438: lw          $s4, 0x50($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X50);
    // 0x8014243C: lw          $s3, 0x4C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X4C);
    // 0x80142440: lw          $s2, 0x48($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X48);
    // 0x80142444: lw          $s1, 0x44($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X44);
    // 0x80142448: lw          $s0, 0x40($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X40);
    // 0x8014244C: ldc1        $f28, 0x88($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X88);
    // 0x80142450: ldc1        $f26, 0x80($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X80);
    // 0x80142454: ldc1        $f24, 0x78($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X78);
    // 0x80142458: ldc1        $f22, 0x70($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X70);
    // 0x8014245C: ldc1        $f20, 0x68($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X68);
    // 0x80142460: addiu       $sp, $sp, 0x90
    ctx->r29 = ADD32(ctx->r29, 0X90);
    // 0x80142464: jr          $ra
    // 0x80142468: nop

    return;
    // 0x80142468: nop

;}
RECOMP_FUNC void func_80142474(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142474: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80142478: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8014247C: lw          $s0, 0x40($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X40);
    // 0x80142480: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80142484: lw          $s3, 0x48($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X48);
    // 0x80142488: lbu         $v0, 0x44($sp)
    ctx->r2 = MEM_BU(ctx->r29, 0X44);
    // 0x8014248C: mov.d       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.d = ctx->f12.d;
    // 0x80142490: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80142494: addu        $s2, $a2, $zero
    ctx->r18 = ADD32(ctx->r6, 0);
    // 0x80142498: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8014249C: c.eq.d      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.d == ctx->f0.d;
    // 0x801424A0: addu        $s1, $a3, $zero
    ctx->r17 = ADD32(ctx->r7, 0);
    // 0x801424A4: bc1t        L_8014252C
    if (c1cs) {
        // 0x801424A8: sw          $ra, 0x28($sp)
        MEM_W(0X28, ctx->r29) = ctx->r31;
            goto L_8014252C;
    }
    // 0x801424A8: sw          $ra, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r31;
    // 0x801424AC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801424B0: ldc1        $f0, 0x4C58($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X4C58);
    // 0x801424B4: c.eq.d      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.d == ctx->f0.d;
    // 0x801424B8: bc1f        L_80142510
    if (!c1cs) {
        // 0x801424BC: addiu       $v0, $zero, 0x2E
        ctx->r2 = ADD32(0, 0X2E);
            goto L_80142510;
    }
    // 0x801424BC: addiu       $v0, $zero, 0x2E
    ctx->r2 = ADD32(0, 0X2E);
    // 0x801424C0: lbu         $v1, 0x0($s0)
    ctx->r3 = MEM_BU(ctx->r16, 0X0);
    // 0x801424C4: bne         $v1, $v0, L_801424D0
    if (ctx->r3 != ctx->r2) {
        // 0x801424C8: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_801424D0;
    }
    // 0x801424C8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x801424CC: addiu       $a0, $s0, -0x1
    ctx->r4 = ADD32(ctx->r16, -0X1);
L_801424D0:
    // 0x801424D0: lbu         $v1, 0x0($a0)
    ctx->r3 = MEM_BU(ctx->r4, 0X0);
    // 0x801424D4: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
    // 0x801424D8: beq         $v1, $v0, L_80142500
    if (ctx->r3 == ctx->r2) {
        // 0x801424DC: addiu       $v0, $zero, 0x32
        ctx->r2 = ADD32(0, 0X32);
            goto L_80142500;
    }
    // 0x801424DC: addiu       $v0, $zero, 0x32
    ctx->r2 = ADD32(0, 0X32);
    // 0x801424E0: beq         $v1, $v0, L_80142500
    if (ctx->r3 == ctx->r2) {
        // 0x801424E4: addiu       $v0, $zero, 0x34
        ctx->r2 = ADD32(0, 0X34);
            goto L_80142500;
    }
    // 0x801424E4: addiu       $v0, $zero, 0x34
    ctx->r2 = ADD32(0, 0X34);
    // 0x801424E8: beq         $v1, $v0, L_80142500
    if (ctx->r3 == ctx->r2) {
        // 0x801424EC: addiu       $v0, $zero, 0x36
        ctx->r2 = ADD32(0, 0X36);
            goto L_80142500;
    }
    // 0x801424EC: addiu       $v0, $zero, 0x36
    ctx->r2 = ADD32(0, 0X36);
    // 0x801424F0: beq         $v1, $v0, L_80142500
    if (ctx->r3 == ctx->r2) {
        // 0x801424F4: addiu       $v0, $zero, 0x38
        ctx->r2 = ADD32(0, 0X38);
            goto L_80142500;
    }
    // 0x801424F4: addiu       $v0, $zero, 0x38
    ctx->r2 = ADD32(0, 0X38);
    // 0x801424F8: bne         $v1, $v0, L_80142510
    if (ctx->r3 != ctx->r2) {
        // 0x801424FC: nop
    
            goto L_80142510;
    }
    // 0x801424FC: nop

L_80142500:
    // 0x80142500: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80142504: ldc1        $f0, 0x4C60($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X4C60);
    // 0x80142508: j           L_8014256C
    // 0x8014250C: nop

        goto L_8014256C;
    // 0x8014250C: nop

L_80142510:
    // 0x80142510: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80142514: ldc1        $f12, 0x4C68($at)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r1, 0X4C68);
    // 0x80142518: mul.d       $f12, $f2, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f12.d); 
    ctx->f12.d = MUL_D(ctx->f2.d, ctx->f12.d);
    // 0x8014251C: jal         0x80144C90
    // 0x80142520: addiu       $a2, $sp, 0x10
    ctx->r6 = ADD32(ctx->r29, 0X10);
    func_80144C90(rdram, ctx);
        goto after_0;
    // 0x80142520: addiu       $a2, $sp, 0x10
    ctx->r6 = ADD32(ctx->r29, 0X10);
    after_0:
    // 0x80142524: j           L_80142570
    // 0x80142528: nop

        goto L_80142570;
    // 0x80142528: nop

L_8014252C:
    // 0x8014252C: andi        $v1, $v0, 0xFF
    ctx->r3 = ctx->r2 & 0XFF;
    // 0x80142530: sltiu       $v0, $v1, 0x31
    ctx->r2 = ctx->r3 < 0X31 ? 1 : 0;
    // 0x80142534: bne         $v0, $zero, L_80142564
    if (ctx->r2 != 0) {
        // 0x80142538: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_80142564;
    }
    // 0x80142538: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x8014253C: sltiu       $v0, $v1, 0x39
    ctx->r2 = ctx->r3 < 0X39 ? 1 : 0;
    // 0x80142540: bne         $v0, $zero, L_80142564
    if (ctx->r2 != 0) {
        // 0x80142544: addiu       $v0, $v1, -0x30
        ctx->r2 = ADD32(ctx->r3, -0X30);
            goto L_80142564;
    }
    // 0x80142544: addiu       $v0, $v1, -0x30
    ctx->r2 = ADD32(ctx->r3, -0X30);
    // 0x80142548: j           L_80142564
    // 0x8014254C: addiu       $v0, $zero, 0x9
    ctx->r2 = ADD32(0, 0X9);
        goto L_80142564;
    // 0x8014254C: addiu       $v0, $zero, 0x9
    ctx->r2 = ADD32(0, 0X9);
L_80142550:
    // 0x80142550: sb          $v0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r2;
    // 0x80142554: lw          $v0, 0x0($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X0);
    // 0x80142558: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x8014255C: j           L_80142624
    // 0x80142560: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
        goto L_80142624;
    // 0x80142560: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
L_80142564:
    // 0x80142564: mtc1        $v0, $f0
    ctx->f0.u32l = ctx->r2;
    // 0x80142568: cvt.d.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.d = CVT_D_W(ctx->f0.u32l);
L_8014256C:
    // 0x8014256C: sdc1        $f0, 0x10($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X10, ctx->r29);
L_80142570:
    // 0x80142570: ldc1        $f2, 0x10($sp)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r29, 0X10);
    // 0x80142574: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80142578: ldc1        $f0, 0x4C70($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X4C70);
    // 0x8014257C: c.lt.d      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.d < ctx->f2.d;
    // 0x80142580: bc1f        L_801425E0
    if (!c1cs) {
        // 0x80142584: addiu       $a0, $zero, 0x30
        ctx->r4 = ADD32(0, 0X30);
            goto L_801425E0;
    }
    // 0x80142584: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
    // 0x80142588: addiu       $a1, $zero, 0x31
    ctx->r5 = ADD32(0, 0X31);
L_8014258C:
    // 0x8014258C: lbu         $v1, 0x0($s0)
    ctx->r3 = MEM_BU(ctx->r16, 0X0);
    // 0x80142590: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142594: lbu         $v0, 0x1AC0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X1AC0);
    // 0x80142598: beql        $v1, $v0, L_801425A0
    if (ctx->r3 == ctx->r2) {
        // 0x8014259C: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_801425A0;
    }
    goto skip_0;
    // 0x8014259C: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    skip_0:
L_801425A0:
    // 0x801425A0: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
    // 0x801425A4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x801425A8: sb          $v0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r2;
    // 0x801425AC: andi        $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 & 0XFF;
    // 0x801425B0: sltiu       $v0, $v0, 0x3A
    ctx->r2 = ctx->r2 < 0X3A ? 1 : 0;
    // 0x801425B4: bnel        $v0, $zero, L_80142628
    if (ctx->r2 != 0) {
        // 0x801425B8: addu        $v0, $s1, $zero
        ctx->r2 = ADD32(ctx->r17, 0);
            goto L_80142628;
    }
    goto skip_1;
    // 0x801425B8: addu        $v0, $s1, $zero
    ctx->r2 = ADD32(ctx->r17, 0);
    skip_1:
    // 0x801425BC: bne         $s0, $s1, L_801425D8
    if (ctx->r16 != ctx->r17) {
        // 0x801425C0: sb          $a0, 0x0($s0)
        MEM_B(0X0, ctx->r16) = ctx->r4;
            goto L_801425D8;
    }
    // 0x801425C0: sb          $a0, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r4;
    // 0x801425C4: bne         $s2, $zero, L_80142550
    if (ctx->r18 != 0) {
        // 0x801425C8: addiu       $v0, $zero, 0x31
        ctx->r2 = ADD32(0, 0X31);
            goto L_80142550;
    }
    // 0x801425C8: addiu       $v0, $zero, 0x31
    ctx->r2 = ADD32(0, 0X31);
    // 0x801425CC: sb          $a1, -0x1($s1)
    MEM_B(-0X1, ctx->r17) = ctx->r5;
    // 0x801425D0: j           L_80142624
    // 0x801425D4: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
        goto L_80142624;
    // 0x801425D4: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
L_801425D8:
    // 0x801425D8: j           L_8014258C
    // 0x801425DC: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
        goto L_8014258C;
    // 0x801425DC: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
L_801425E0:
    // 0x801425E0: lbu         $v1, 0x0($s3)
    ctx->r3 = MEM_BU(ctx->r19, 0X0);
    // 0x801425E4: addiu       $v0, $zero, 0x2D
    ctx->r2 = ADD32(0, 0X2D);
    // 0x801425E8: bne         $v1, $v0, L_80142628
    if (ctx->r3 != ctx->r2) {
        // 0x801425EC: addu        $v0, $s1, $zero
        ctx->r2 = ADD32(ctx->r17, 0);
            goto L_80142628;
    }
    // 0x801425EC: addu        $v0, $s1, $zero
    ctx->r2 = ADD32(ctx->r17, 0);
    // 0x801425F0: addiu       $a0, $zero, 0x30
    ctx->r4 = ADD32(0, 0X30);
L_801425F4:
    // 0x801425F4: lbu         $v1, 0x0($s0)
    ctx->r3 = MEM_BU(ctx->r16, 0X0);
    // 0x801425F8: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801425FC: lbu         $v0, 0x1AC0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X1AC0);
    // 0x80142600: beql        $v1, $v0, L_80142608
    if (ctx->r3 == ctx->r2) {
        // 0x80142604: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_80142608;
    }
    goto skip_2;
    // 0x80142604: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    skip_2:
L_80142608:
    // 0x80142608: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
    // 0x8014260C: bnel        $v0, $a0, L_80142628
    if (ctx->r2 != ctx->r4) {
        // 0x80142610: addu        $v0, $s1, $zero
        ctx->r2 = ADD32(ctx->r17, 0);
            goto L_80142628;
    }
    goto skip_3;
    // 0x80142610: addu        $v0, $s1, $zero
    ctx->r2 = ADD32(ctx->r17, 0);
    skip_3:
    // 0x80142614: beql        $s0, $s1, L_8014261C
    if (ctx->r16 == ctx->r17) {
        // 0x80142618: sb          $zero, 0x0($s3)
        MEM_B(0X0, ctx->r19) = 0;
            goto L_8014261C;
    }
    goto skip_4;
    // 0x80142618: sb          $zero, 0x0($s3)
    MEM_B(0X0, ctx->r19) = 0;
    skip_4:
L_8014261C:
    // 0x8014261C: j           L_801425F4
    // 0x80142620: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
        goto L_801425F4;
    // 0x80142620: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
L_80142624:
    // 0x80142624: addu        $v0, $s1, $zero
    ctx->r2 = ADD32(ctx->r17, 0);
L_80142628:
    // 0x80142628: lw          $ra, 0x28($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X28);
    // 0x8014262C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80142630: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80142634: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80142638: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8014263C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80142640: jr          $ra
    // 0x80142644: nop

    return;
    // 0x80142644: nop

;}
RECOMP_FUNC void func_80142648(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142648: sb          $a2, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r6;
    // 0x8014264C: bgez        $a1, L_80142660
    if (SIGNED(ctx->r5) >= 0) {
        // 0x80142650: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_80142660;
    }
    // 0x80142650: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x80142654: negu        $a1, $a1
    ctx->r5 = SUB32(0, ctx->r5);
    // 0x80142658: j           L_80142664
    // 0x8014265C: addiu       $v0, $zero, 0x2D
    ctx->r2 = ADD32(0, 0X2D);
        goto L_80142664;
    // 0x8014265C: addiu       $v0, $zero, 0x2D
    ctx->r2 = ADD32(0, 0X2D);
L_80142660:
    // 0x80142660: addiu       $v0, $zero, 0x2B
    ctx->r2 = ADD32(0, 0X2B);
L_80142664:
    // 0x80142664: sb          $v0, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r2;
    // 0x80142668: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8014266C: addiu       $a3, $a3, 0x5F70
    ctx->r7 = ADD32(ctx->r7, 0X5F70);
    // 0x80142670: slti        $v0, $a1, 0xA
    ctx->r2 = SIGNED(ctx->r5) < 0XA ? 1 : 0;
    // 0x80142674: bne         $v0, $zero, L_80142744
    if (ctx->r2 != 0) {
        // 0x80142678: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_80142744;
    }
    // 0x80142678: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x8014267C: lui         $t0, 0x6666
    ctx->r8 = S32(0X6666 << 16);
    // 0x80142680: ori         $t0, $t0, 0x6667
    ctx->r8 = ctx->r8 | 0X6667;
    // 0x80142684: mult        $a1, $t0
    result = S64(S32(ctx->r5)) * S64(S32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
L_80142688:
    // 0x80142688: sra         $v0, $a1, 31
    ctx->r2 = S32(SIGNED(ctx->r5) >> 31);
    // 0x8014268C: mfhi        $t1
    ctx->r9 = hi;
    // 0x80142690: sra         $v1, $t1, 2
    ctx->r3 = S32(SIGNED(ctx->r9) >> 2);
    // 0x80142694: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80142698: sll         $v0, $v1, 2
    ctx->r2 = S32(ctx->r3 << 2);
    // 0x8014269C: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x801426A0: sll         $v0, $v0, 1
    ctx->r2 = S32(ctx->r2 << 1);
    // 0x801426A4: subu        $v1, $a1, $v0
    ctx->r3 = SUB32(ctx->r5, ctx->r2);
    // 0x801426A8: slti        $v0, $v1, 0x9
    ctx->r2 = SIGNED(ctx->r3) < 0X9 ? 1 : 0;
    // 0x801426AC: bne         $v0, $zero, L_801426BC
    if (ctx->r2 != 0) {
        // 0x801426B0: addu        $a2, $v1, $zero
        ctx->r6 = ADD32(ctx->r3, 0);
            goto L_801426BC;
    }
    // 0x801426B0: addu        $a2, $v1, $zero
    ctx->r6 = ADD32(ctx->r3, 0);
    // 0x801426B4: j           L_801426C8
    // 0x801426B8: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
        goto L_801426C8;
    // 0x801426B8: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
L_801426BC:
    // 0x801426BC: bgtz        $v1, L_801426C8
    if (SIGNED(ctx->r3) > 0) {
        // 0x801426C0: addiu       $v0, $a2, 0x30
        ctx->r2 = ADD32(ctx->r6, 0X30);
            goto L_801426C8;
    }
    // 0x801426C0: addiu       $v0, $a2, 0x30
    ctx->r2 = ADD32(ctx->r6, 0X30);
    // 0x801426C4: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_801426C8:
    // 0x801426C8: mult        $a1, $t0
    result = S64(S32(ctx->r5)) * S64(S32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x801426CC: addiu       $a3, $a3, -0x1
    ctx->r7 = ADD32(ctx->r7, -0X1);
    // 0x801426D0: sb          $v0, 0x0($a3)
    MEM_B(0X0, ctx->r7) = ctx->r2;
    // 0x801426D4: sra         $v0, $a1, 31
    ctx->r2 = S32(SIGNED(ctx->r5) >> 31);
    // 0x801426D8: mfhi        $t1
    ctx->r9 = hi;
    // 0x801426DC: sra         $v1, $t1, 2
    ctx->r3 = S32(SIGNED(ctx->r9) >> 2);
    // 0x801426E0: subu        $a1, $v1, $v0
    ctx->r5 = SUB32(ctx->r3, ctx->r2);
    // 0x801426E4: slti        $v0, $a1, 0xA
    ctx->r2 = SIGNED(ctx->r5) < 0XA ? 1 : 0;
    // 0x801426E8: beql        $v0, $zero, L_80142688
    if (ctx->r2 == 0) {
        // 0x801426EC: mult        $a1, $t0
        result = S64(S32(ctx->r5)) * S64(S32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
            goto L_80142688;
    }
    goto skip_0;
    // 0x801426EC: mult        $a1, $t0
    result = S64(S32(ctx->r5)) * S64(S32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    skip_0:
    // 0x801426F0: slti        $v0, $a1, 0x9
    ctx->r2 = SIGNED(ctx->r5) < 0X9 ? 1 : 0;
    // 0x801426F4: beq         $v0, $zero, L_80142708
    if (ctx->r2 == 0) {
        // 0x801426F8: addiu       $v0, $zero, 0x39
        ctx->r2 = ADD32(0, 0X39);
            goto L_80142708;
    }
    // 0x801426F8: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
    // 0x801426FC: bgtz        $a1, L_80142708
    if (SIGNED(ctx->r5) > 0) {
        // 0x80142700: addiu       $v0, $a1, 0x30
        ctx->r2 = ADD32(ctx->r5, 0X30);
            goto L_80142708;
    }
    // 0x80142700: addiu       $v0, $a1, 0x30
    ctx->r2 = ADD32(ctx->r5, 0X30);
    // 0x80142704: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_80142708:
    // 0x80142708: addiu       $a3, $a3, -0x1
    ctx->r7 = ADD32(ctx->r7, -0X1);
    // 0x8014270C: sb          $v0, 0x0($a3)
    MEM_B(0X0, ctx->r7) = ctx->r2;
    // 0x80142710: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80142714: addiu       $v1, $v1, 0x5F70
    ctx->r3 = ADD32(ctx->r3, 0X5F70);
    // 0x80142718: sltu        $v0, $a3, $v1
    ctx->r2 = ctx->r7 < ctx->r3 ? 1 : 0;
    // 0x8014271C: beq         $v0, $zero, L_80142774
    if (ctx->r2 == 0) {
        // 0x80142720: nop
    
            goto L_80142774;
    }
    // 0x80142720: nop

L_80142724:
    // 0x80142724: lbu         $v0, 0x0($a3)
    ctx->r2 = MEM_BU(ctx->r7, 0X0);
    // 0x80142728: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
    // 0x8014272C: sb          $v0, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r2;
    // 0x80142730: sltu        $v0, $a3, $v1
    ctx->r2 = ctx->r7 < ctx->r3 ? 1 : 0;
    // 0x80142734: bne         $v0, $zero, L_80142724
    if (ctx->r2 != 0) {
        // 0x80142738: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_80142724;
    }
    // 0x80142738: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x8014273C: j           L_80142774
    // 0x80142740: nop

        goto L_80142774;
    // 0x80142740: nop

L_80142744:
    // 0x80142744: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
    // 0x80142748: sb          $v0, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r2;
    // 0x8014274C: slti        $v0, $a1, 0x9
    ctx->r2 = SIGNED(ctx->r5) < 0X9 ? 1 : 0;
    // 0x80142750: bne         $v0, $zero, L_80142760
    if (ctx->r2 != 0) {
        // 0x80142754: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_80142760;
    }
    // 0x80142754: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x80142758: j           L_8014276C
    // 0x8014275C: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
        goto L_8014276C;
    // 0x8014275C: addiu       $v0, $zero, 0x39
    ctx->r2 = ADD32(0, 0X39);
L_80142760:
    // 0x80142760: bgtzl       $a1, L_8014276C
    if (SIGNED(ctx->r5) > 0) {
        // 0x80142764: addiu       $v0, $a1, 0x30
        ctx->r2 = ADD32(ctx->r5, 0X30);
            goto L_8014276C;
    }
    goto skip_1;
    // 0x80142764: addiu       $v0, $a1, 0x30
    ctx->r2 = ADD32(ctx->r5, 0X30);
    skip_1:
    // 0x80142768: addiu       $v0, $zero, 0x30
    ctx->r2 = ADD32(0, 0X30);
L_8014276C:
    // 0x8014276C: sb          $v0, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r2;
    // 0x80142770: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_80142774:
    // 0x80142774: jr          $ra
    // 0x80142778: addu        $v0, $a0, $zero
    ctx->r2 = ADD32(ctx->r4, 0);
    return;
    // 0x80142778: addu        $v0, $a0, $zero
    ctx->r2 = ADD32(ctx->r4, 0);
;}
RECOMP_FUNC void func_8014277C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014277C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80142780: sdc1        $f12, 0x18($sp)
    CHECK_FR(ctx, 12);
    SD(ctx->f12.u64, 0X18, ctx->r29);
    // 0x80142784: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142788: sw          $zero, 0x1AC4($at)
    MEM_W(0X1AC4, ctx->r1) = 0;
    // 0x8014278C: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x80142790: addu        $a0, $a2, $zero
    ctx->r4 = ADD32(ctx->r6, 0);
    // 0x80142794: lui         $v1, 0x7FF0
    ctx->r3 = S32(0X7FF0 << 16);
    // 0x80142798: and         $v0, $a1, $v1
    ctx->r2 = ctx->r5 & ctx->r3;
    // 0x8014279C: beq         $v0, $v1, L_801427AC
    if (ctx->r2 == ctx->r3) {
        // 0x801427A0: sw          $ra, 0x10($sp)
        MEM_W(0X10, ctx->r29) = ctx->r31;
            goto L_801427AC;
    }
    // 0x801427A0: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x801427A4: j           L_80142810
    // 0x801427A8: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
        goto L_80142810;
    // 0x801427A8: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_801427AC:
    // 0x801427AC: lui         $v0, 0xF
    ctx->r2 = S32(0XF << 16);
    // 0x801427B0: ori         $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 | 0XFFFF;
    // 0x801427B4: lui         $v1, 0x7
    ctx->r3 = S32(0X7 << 16);
    // 0x801427B8: ori         $v1, $v1, 0xFFFF
    ctx->r3 = ctx->r3 | 0XFFFF;
    // 0x801427BC: and         $v0, $a1, $v0
    ctx->r2 = ctx->r5 & ctx->r2;
    // 0x801427C0: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x801427C4: bne         $v0, $zero, L_801427D8
    if (ctx->r2 != 0) {
        // 0x801427C8: nop
    
            goto L_801427D8;
    }
    // 0x801427C8: nop

    // 0x801427CC: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x801427D0: beq         $v0, $zero, L_801427FC
    if (ctx->r2 == 0) {
        // 0x801427D4: nop
    
            goto L_801427FC;
    }
    // 0x801427D4: nop

L_801427D8:
    // 0x801427D8: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x801427DC: addiu       $a1, $a1, 0x4C78
    ctx->r5 = ADD32(ctx->r5, 0X4C78);
    // 0x801427E0: jal         0x80141108
    // 0x801427E4: nop

    func_80141108(rdram, ctx);
        goto after_0;
    // 0x801427E4: nop

    after_0:
    // 0x801427E8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801427EC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801427F0: sw          $v0, 0x1AC4($at)
    MEM_W(0X1AC4, ctx->r1) = ctx->r2;
    // 0x801427F4: j           L_80142810
    // 0x801427F8: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_80142810;
    // 0x801427F8: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_801427FC:
    // 0x801427FC: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80142800: addiu       $a1, $a1, 0x4C7C
    ctx->r5 = ADD32(ctx->r5, 0X4C7C);
    // 0x80142804: jal         0x80141108
    // 0x80142808: nop

    func_80141108(rdram, ctx);
        goto after_1;
    // 0x80142808: nop

    after_1:
    // 0x8014280C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_80142810:
    // 0x80142810: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80142814: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80142818: jr          $ra
    // 0x8014281C: nop

    return;
    // 0x8014281C: nop

;}
RECOMP_FUNC void func_80142820(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142820: beq         $a2, $zero, L_80142848
    if (ctx->r6 == 0) {
        // 0x80142824: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_80142848;
    }
    // 0x80142824: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80142828:
    // 0x80142828: lbu         $v0, 0x0($a0)
    ctx->r2 = MEM_BU(ctx->r4, 0X0);
    // 0x8014282C: bne         $v0, $a1, L_8014283C
    if (ctx->r2 != ctx->r5) {
        // 0x80142830: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_8014283C;
    }
    // 0x80142830: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x80142834: j           L_80142848
    // 0x80142838: addiu       $v0, $a0, -0x1
    ctx->r2 = ADD32(ctx->r4, -0X1);
        goto L_80142848;
    // 0x80142838: addiu       $v0, $a0, -0x1
    ctx->r2 = ADD32(ctx->r4, -0X1);
L_8014283C:
    // 0x8014283C: addiu       $a2, $a2, -0x1
    ctx->r6 = ADD32(ctx->r6, -0X1);
    // 0x80142840: bne         $a2, $zero, L_80142828
    if (ctx->r6 != 0) {
        // 0x80142844: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_80142828;
    }
    // 0x80142844: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80142848:
    // 0x80142848: jr          $ra
    // 0x8014284C: nop

    return;
    // 0x8014284C: nop

;}
RECOMP_FUNC void func_80142850_original(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142850: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80142854: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x80142858: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x8014285C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80142860: addiu       $s0, $zero, -0x1
    ctx->r16 = ADD32(0, -0X1);
    // 0x80142864: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80142868: bne         $s1, $zero, L_80142878
    if (ctx->r17 != 0) {
        // 0x8014286C: sw          $s2, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r18;
            goto L_80142878;
    }
    // 0x8014286C: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x80142870: j           L_8014296C
    // 0x80142874: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_8014296C;
    // 0x80142874: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80142878:
    // 0x80142878: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x8014287C: andi        $v0, $v0, 0x210
    ctx->r2 = ctx->r2 & 0X210;
    // 0x80142880: beq         $v0, $zero, L_801428C8
    if (ctx->r2 == 0) {
        // 0x80142884: addiu       $v0, $zero, 0x2000
        ctx->r2 = ADD32(0, 0X2000);
            goto L_801428C8;
    }
    // 0x80142884: addiu       $v0, $zero, 0x2000
    ctx->r2 = ADD32(0, 0X2000);
    // 0x80142888: lw          $v1, 0x0($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X0);
    // 0x8014288C: subu        $s0, $v0, $v1
    ctx->r16 = SUB32(ctx->r2, ctx->r3);
    // 0x80142890: andi        $s0, $s0, 0x3
    ctx->r16 = ctx->r16 & 0X3;
    // 0x80142894: beq         $s0, $zero, L_801428C8
    if (ctx->r16 == 0) {
        // 0x80142898: addiu       $v0, $zero, 0x4
        ctx->r2 = ADD32(0, 0X4);
            goto L_801428C8;
    }
    // 0x80142898: addiu       $v0, $zero, 0x4
    ctx->r2 = ADD32(0, 0X4);
    // 0x8014289C: subu        $s0, $v0, $s0
    ctx->r16 = SUB32(ctx->r2, ctx->r16);
    // 0x801428A0: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x801428A4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x801428A8: beq         $s0, $v0, L_801428C8
    if (ctx->r16 == ctx->r2) {
        // 0x801428AC: addiu       $a0, $zero, 0x20
        ctx->r4 = ADD32(0, 0X20);
            goto L_801428C8;
    }
    // 0x801428AC: addiu       $a0, $zero, 0x20
    ctx->r4 = ADD32(0, 0X20);
    // 0x801428B0: addiu       $s2, $zero, -0x1
    ctx->r18 = ADD32(0, -0X1);
L_801428B4:
    // 0x801428B4: jal         0x80142EC0
    // 0x801428B8: addu        $a1, $s1, $zero
    ctx->r5 = ADD32(ctx->r17, 0);
    func_80142EC0(rdram, ctx);
        goto after_0;
    // 0x801428B8: addu        $a1, $s1, $zero
    ctx->r5 = ADD32(ctx->r17, 0);
    after_0:
    // 0x801428BC: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x801428C0: bne         $s0, $s2, L_801428B4
    if (ctx->r16 != ctx->r18) {
        // 0x801428C4: addiu       $a0, $zero, 0x20
        ctx->r4 = ADD32(0, 0X20);
            goto L_801428B4;
    }
    // 0x801428C4: addiu       $a0, $zero, 0x20
    ctx->r4 = ADD32(0, 0X20);
L_801428C8:
    // 0x801428C8: lw          $v1, 0x10($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X10);
    // 0x801428CC: andi        $v0, $v1, 0x218
    ctx->r2 = ctx->r3 & 0X218;
    // 0x801428D0: beq         $v0, $zero, L_80142918
    if (ctx->r2 == 0) {
        // 0x801428D4: andi        $v0, $v1, 0x100
        ctx->r2 = ctx->r3 & 0X100;
            goto L_80142918;
    }
    // 0x801428D4: andi        $v0, $v1, 0x100
    ctx->r2 = ctx->r3 & 0X100;
    // 0x801428D8: bne         $v0, $zero, L_80142918
    if (ctx->r2 != 0) {
        // 0x801428DC: nop
    
            goto L_80142918;
    }
    // 0x801428DC: nop

    // 0x801428E0: jal         0x80142988
    // 0x801428E4: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    func_80142988(rdram, ctx);
        goto after_1;
    // 0x801428E4: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    after_1:
    // 0x801428E8: lw          $a0, 0x14($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X14);
    // 0x801428EC: jal         0x80144EB8
    // 0x801428F0: addu        $s0, $v0, $zero
    ctx->r16 = ADD32(ctx->r2, 0);
    func_80144EB8(rdram, ctx);
        goto after_2;
    // 0x801428F0: addu        $s0, $v0, $zero
    ctx->r16 = ADD32(ctx->r2, 0);
    after_2:
    // 0x801428F4: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
    // 0x801428F8: lw          $a0, 0x10($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X10);
    // 0x801428FC: slt         $v0, $v1, $v0
    ctx->r2 = SIGNED(ctx->r3) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80142900: andi        $a0, $a0, 0x20
    ctx->r4 = ctx->r4 & 0X20;
    // 0x80142904: beq         $a0, $zero, L_80142918
    if (ctx->r4 == 0) {
        // 0x80142908: movz        $s0, $v1, $v0
        if (ctx->r2 == 0) ctx->r16 = ctx->r3;
            goto L_80142918;
    }
    // 0x80142908: movz        $s0, $v1, $v0
    if (ctx->r2 == 0) ctx->r16 = ctx->r3;
    // 0x8014290C: lw          $a0, 0x8($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X8);
    // 0x80142910: jal         0x80143E74
    // 0x80142914: nop

    func_80143E74(rdram, ctx);
        goto after_3;
    // 0x80142914: nop

    after_3:
L_80142918:
    // 0x80142918: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x8014291C: andi        $v0, $v0, 0x800
    ctx->r2 = ctx->r2 & 0X800;
    // 0x80142920: beq         $v0, $zero, L_80142950
    if (ctx->r2 == 0) {
        // 0x80142924: addu        $v0, $s0, $zero
        ctx->r2 = ADD32(ctx->r16, 0);
            goto L_80142950;
    }
    // 0x80142924: addu        $v0, $s0, $zero
    ctx->r2 = ADD32(ctx->r16, 0);
    // 0x80142928: lw          $a0, 0x18($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X18);
    // 0x8014292C: beq         $a0, $zero, L_80142954
    if (ctx->r4 == 0) {
        // 0x80142930: addiu       $v1, $zero, -0x1
        ctx->r3 = ADD32(0, -0X1);
            goto L_80142954;
    }
    // 0x80142930: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
    // 0x80142934: jal         0x80143788
    // 0x80142938: nop

    func_80143788(rdram, ctx);
        goto after_4;
    // 0x80142938: nop

    after_4:
    // 0x8014293C: lw          $a0, 0x18($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X18);
    // 0x80142940: jal         0x80143E74
    // 0x80142944: nop

    func_80143E74(rdram, ctx);
        goto after_5;
    // 0x80142944: nop

    after_5:
    // 0x80142948: sw          $zero, 0x18($s1)
    MEM_W(0X18, ctx->r17) = 0;
    // 0x8014294C: addu        $v0, $s0, $zero
    ctx->r2 = ADD32(ctx->r16, 0);
L_80142950:
    // 0x80142950: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
L_80142954:
    // 0x80142954: sw          $zero, 0x0($s1)
    MEM_W(0X0, ctx->r17) = 0;
    // 0x80142958: sw          $zero, 0x8($s1)
    MEM_W(0X8, ctx->r17) = 0;
    // 0x8014295C: sw          $zero, 0x4($s1)
    MEM_W(0X4, ctx->r17) = 0;
    // 0x80142960: sw          $zero, 0xC($s1)
    MEM_W(0XC, ctx->r17) = 0;
    // 0x80142964: sw          $zero, 0x10($s1)
    MEM_W(0X10, ctx->r17) = 0;
    // 0x80142968: sw          $v1, 0x14($s1)
    MEM_W(0X14, ctx->r17) = ctx->r3;
L_8014296C:
    // 0x8014296C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80142970: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x80142974: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80142978: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8014297C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80142980: jr          $ra
    // 0x80142984: nop

    return;
    // 0x80142984: nop

;}
RECOMP_FUNC void func_80142988(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142988: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8014298C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80142990: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x80142994: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80142998: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x8014299C: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x801429A0: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x801429A4: addiu       $v1, $zero, 0x10
    ctx->r3 = ADD32(0, 0X10);
    // 0x801429A8: andi        $v0, $v0, 0x12
    ctx->r2 = ctx->r2 & 0X12;
    // 0x801429AC: bne         $v0, $v1, L_80142A1C
    if (ctx->r2 != ctx->r3) {
        // 0x801429B0: nop
    
            goto L_80142A1C;
    }
    // 0x801429B0: nop

    // 0x801429B4: lw          $s2, 0x8($s0)
    ctx->r18 = MEM_W(ctx->r16, 0X8);
    // 0x801429B8: beq         $s2, $zero, L_80142A1C
    if (ctx->r18 == 0) {
        // 0x801429BC: nop
    
            goto L_80142A1C;
    }
    // 0x801429BC: nop

    // 0x801429C0: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x801429C4: subu        $s1, $v0, $s2
    ctx->r17 = SUB32(ctx->r2, ctx->r18);
    // 0x801429C8: blez        $s1, L_80142A1C
    if (SIGNED(ctx->r17) <= 0) {
        // 0x801429CC: addu        $v1, $zero, $zero
        ctx->r3 = ADD32(0, 0);
            goto L_80142A1C;
    }
    // 0x801429CC: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
    // 0x801429D0: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x801429D4: andi        $v0, $v0, 0x6
    ctx->r2 = ctx->r2 & 0X6;
    // 0x801429D8: bne         $v0, $zero, L_801429E4
    if (ctx->r2 != 0) {
        // 0x801429DC: sw          $s2, 0x4($s0)
        MEM_W(0X4, ctx->r16) = ctx->r18;
            goto L_801429E4;
    }
    // 0x801429DC: sw          $s2, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r18;
    // 0x801429E0: lw          $v1, 0xC($s0)
    ctx->r3 = MEM_W(ctx->r16, 0XC);
L_801429E4:
    // 0x801429E4: sw          $v1, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r3;
L_801429E8:
    // 0x801429E8: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x801429EC: addu        $a1, $s2, $zero
    ctx->r5 = ADD32(ctx->r18, 0);
    // 0x801429F0: jal         0x80145020
    // 0x801429F4: addu        $a2, $s1, $zero
    ctx->r6 = ADD32(ctx->r17, 0);
    func_80145020(rdram, ctx);
        goto after_0;
    // 0x801429F4: addu        $a2, $s1, $zero
    ctx->r6 = ADD32(ctx->r17, 0);
    after_0:
    // 0x801429F8: bgtz        $v0, L_80142A14
    if (SIGNED(ctx->r2) > 0) {
        // 0x801429FC: subu        $s1, $s1, $v0
        ctx->r17 = SUB32(ctx->r17, ctx->r2);
            goto L_80142A14;
    }
    // 0x801429FC: subu        $s1, $s1, $v0
    ctx->r17 = SUB32(ctx->r17, ctx->r2);
    // 0x80142A00: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x80142A04: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80142A08: ori         $v1, $v1, 0x80
    ctx->r3 = ctx->r3 | 0X80;
    // 0x80142A0C: j           L_80142A48
    // 0x80142A10: sw          $v1, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r3;
        goto L_80142A48;
    // 0x80142A10: sw          $v1, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r3;
L_80142A14:
    // 0x80142A14: bgtz        $s1, L_801429E8
    if (SIGNED(ctx->r17) > 0) {
        // 0x80142A18: addu        $s2, $s2, $v0
        ctx->r18 = ADD32(ctx->r18, ctx->r2);
            goto L_801429E8;
    }
    // 0x80142A18: addu        $s2, $s2, $v0
    ctx->r18 = ADD32(ctx->r18, ctx->r2);
L_80142A1C:
    // 0x80142A1C: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x80142A20: andi        $v0, $v0, 0x200
    ctx->r2 = ctx->r2 & 0X200;
    // 0x80142A24: beq         $v0, $zero, L_80142A44
    if (ctx->r2 == 0) {
        // 0x80142A28: addiu       $v1, $zero, -0x19
        ctx->r3 = ADD32(0, -0X19);
            goto L_80142A44;
    }
    // 0x80142A28: addiu       $v1, $zero, -0x19
    ctx->r3 = ADD32(0, -0X19);
    // 0x80142A2C: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x80142A30: lw          $a0, 0x8($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X8);
    // 0x80142A34: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x80142A38: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x80142A3C: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
    // 0x80142A40: sw          $a0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r4;
L_80142A44:
    // 0x80142A44: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80142A48:
    // 0x80142A48: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80142A4C: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x80142A50: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80142A54: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80142A58: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80142A5C: jr          $ra
    // 0x80142A60: nop

    return;
    // 0x80142A60: nop

;}
RECOMP_FUNC void func_80142A68(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142A68: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80142A6C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80142A70: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x80142A74: sw          $ra, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r31;
    // 0x80142A78: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80142A7C: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80142A80: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80142A84: lw          $v1, 0x10($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X10);
    // 0x80142A88: andi        $v0, $v1, 0x200
    ctx->r2 = ctx->r3 & 0X200;
    // 0x80142A8C: beq         $v0, $zero, L_80142AA4
    if (ctx->r2 == 0) {
        // 0x80142A90: addu        $s3, $a0, $zero
        ctx->r19 = ADD32(ctx->r4, 0);
            goto L_80142AA4;
    }
    // 0x80142A90: addu        $s3, $a0, $zero
    ctx->r19 = ADD32(ctx->r4, 0);
    // 0x80142A94: ori         $v0, $v1, 0x10
    ctx->r2 = ctx->r3 | 0X10;
    // 0x80142A98: addiu       $v1, $zero, -0x49
    ctx->r3 = ADD32(0, -0X49);
    // 0x80142A9C: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x80142AA0: sw          $v0, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r2;
L_80142AA4:
    // 0x80142AA4: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x80142AA8: andi        $v0, $v0, 0x10
    ctx->r2 = ctx->r2 & 0X10;
    // 0x80142AAC: beq         $v0, $zero, L_80142C38
    if (ctx->r2 == 0) {
        // 0x80142AB0: addiu       $v0, $zero, -0x1
        ctx->r2 = ADD32(0, -0X1);
            goto L_80142C38;
    }
    // 0x80142AB0: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80142AB4:
    // 0x80142AB4: lw          $v1, 0x10($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X10);
    // 0x80142AB8: andi        $v0, $v1, 0x4
    ctx->r2 = ctx->r3 & 0X4;
    // 0x80142ABC: beq         $v0, $zero, L_80142B10
    if (ctx->r2 == 0) {
        // 0x80142AC0: andi        $v0, $v1, 0x2
        ctx->r2 = ctx->r3 & 0X2;
            goto L_80142B10;
    }
    // 0x80142AC0: andi        $v0, $v1, 0x2
    ctx->r2 = ctx->r3 & 0X2;
    // 0x80142AC4: lw          $v1, 0x4($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X4);
    // 0x80142AC8: lw          $s2, 0x8($s1)
    ctx->r18 = MEM_W(ctx->r17, 0X8);
    // 0x80142ACC: addiu       $v0, $v1, 0x1
    ctx->r2 = ADD32(ctx->r3, 0X1);
    // 0x80142AD0: sw          $v0, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r2;
    // 0x80142AD4: sb          $s3, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r19;
    // 0x80142AD8: lw          $v1, 0x4($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X4);
    // 0x80142ADC: lw          $v0, 0xC($s1)
    ctx->r2 = MEM_W(ctx->r17, 0XC);
    // 0x80142AE0: subu        $s0, $v1, $s2
    ctx->r16 = SUB32(ctx->r3, ctx->r18);
    // 0x80142AE4: slt         $v0, $s0, $v0
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80142AE8: beq         $v0, $zero, L_80142AF8
    if (ctx->r2 == 0) {
        // 0x80142AEC: addiu       $v0, $zero, 0xA
        ctx->r2 = ADD32(0, 0XA);
            goto L_80142AF8;
    }
    // 0x80142AEC: addiu       $v0, $zero, 0xA
    ctx->r2 = ADD32(0, 0XA);
    // 0x80142AF0: bne         $s3, $v0, L_80142B04
    if (ctx->r19 != ctx->r2) {
        // 0x80142AF4: negu        $v0, $s0
        ctx->r2 = SUB32(0, ctx->r16);
            goto L_80142B04;
    }
    // 0x80142AF4: negu        $v0, $s0
    ctx->r2 = SUB32(0, ctx->r16);
L_80142AF8:
    // 0x80142AF8: sw          $s2, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r18;
    // 0x80142AFC: j           L_80142BE0
    // 0x80142B00: sw          $zero, 0x0($s1)
    MEM_W(0X0, ctx->r17) = 0;
        goto L_80142BE0;
    // 0x80142B00: sw          $zero, 0x0($s1)
    MEM_W(0X0, ctx->r17) = 0;
L_80142B04:
    // 0x80142B04: sw          $v0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r2;
    // 0x80142B08: j           L_80142BE0
    // 0x80142B0C: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
        goto L_80142BE0;
    // 0x80142B0C: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
L_80142B10:
    // 0x80142B10: beq         $v0, $zero, L_80142B28
    if (ctx->r2 == 0) {
        // 0x80142B14: addiu       $s0, $zero, 0x1
        ctx->r16 = ADD32(0, 0X1);
            goto L_80142B28;
    }
    // 0x80142B14: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    // 0x80142B18: addiu       $s2, $sp, 0x10
    ctx->r18 = ADD32(ctx->r29, 0X10);
    // 0x80142B1C: sb          $s3, 0x10($sp)
    MEM_B(0X10, ctx->r29) = ctx->r19;
    // 0x80142B20: j           L_80142BE0
    // 0x80142B24: sw          $zero, 0x0($s1)
    MEM_W(0X0, ctx->r17) = 0;
        goto L_80142BE0;
    // 0x80142B24: sw          $zero, 0x0($s1)
    MEM_W(0X0, ctx->r17) = 0;
L_80142B28:
    // 0x80142B28: lw          $s2, 0x8($s1)
    ctx->r18 = MEM_W(ctx->r17, 0X8);
    // 0x80142B2C: bne         $s2, $zero, L_80142BCC
    if (ctx->r18 != 0) {
        // 0x80142B30: nop
    
            goto L_80142BCC;
    }
    // 0x80142B30: nop

    // 0x80142B34: jal         0x80143A40
    // 0x80142B38: addiu       $a0, $zero, 0x2000
    ctx->r4 = ADD32(0, 0X2000);
    func_80143A40(rdram, ctx);
        goto after_0;
    // 0x80142B38: addiu       $a0, $zero, 0x2000
    ctx->r4 = ADD32(0, 0X2000);
    after_0:
    // 0x80142B3C: addu        $s2, $v0, $zero
    ctx->r18 = ADD32(ctx->r2, 0);
    // 0x80142B40: sw          $s2, 0x8($s1)
    MEM_W(0X8, ctx->r17) = ctx->r18;
    // 0x80142B44: bne         $s2, $zero, L_80142B5C
    if (ctx->r18 != 0) {
        // 0x80142B48: addiu       $s0, $zero, 0x2000
        ctx->r16 = ADD32(0, 0X2000);
            goto L_80142B5C;
    }
    // 0x80142B48: addiu       $s0, $zero, 0x2000
    ctx->r16 = ADD32(0, 0X2000);
    // 0x80142B4C: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x80142B50: ori         $v0, $v0, 0x2
    ctx->r2 = ctx->r2 | 0X2;
    // 0x80142B54: j           L_80142AB4
    // 0x80142B58: sw          $v0, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r2;
        goto L_80142AB4;
    // 0x80142B58: sw          $v0, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r2;
L_80142B5C:
    // 0x80142B5C: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x80142B60: sw          $s0, 0xC($s1)
    MEM_W(0XC, ctx->r17) = ctx->r16;
    // 0x80142B64: ori         $v0, $v0, 0x20
    ctx->r2 = ctx->r2 | 0X20;
    // 0x80142B68: sw          $v0, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r2;
    // 0x80142B6C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142B70: addiu       $v0, $v0, 0x1DD8
    ctx->r2 = ADD32(ctx->r2, 0X1DD8);
    // 0x80142B74: bne         $s1, $v0, L_80142BD4
    if (ctx->r17 != ctx->r2) {
        // 0x80142B78: addu        $s0, $zero, $zero
        ctx->r16 = ADD32(0, 0);
            goto L_80142BD4;
    }
    // 0x80142B78: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x80142B7C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80142B80: lw          $a0, 0x1DEC($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1DEC);
    // 0x80142B84: jal         0x800C416C
    // 0x80142B88: nop

    static_0_800C416C(rdram, ctx);
        goto after_1;
    // 0x80142B88: nop

    after_1:
    // 0x80142B8C: beq         $v0, $zero, L_80142BD4
    if (ctx->r2 == 0) {
        // 0x80142B90: nop
    
            goto L_80142BD4;
    }
    // 0x80142B90: nop

    // 0x80142B94: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142B98: lw          $v0, 0x1DE8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1DE8);
    // 0x80142B9C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142BA0: sw          $s2, 0x1DDC($at)
    MEM_W(0X1DDC, ctx->r1) = ctx->r18;
    // 0x80142BA4: ori         $v0, $v0, 0x4
    ctx->r2 = ctx->r2 | 0X4;
    // 0x80142BA8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142BAC: sw          $v0, 0x1DE8($at)
    MEM_W(0X1DE8, ctx->r1) = ctx->r2;
    // 0x80142BB0: j           L_80142AB4
    // 0x80142BB4: nop

        goto L_80142AB4;
    // 0x80142BB4: nop

L_80142BB8:
    // 0x80142BB8: lw          $v1, 0x10($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X10);
    // 0x80142BBC: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80142BC0: ori         $v1, $v1, 0x80
    ctx->r3 = ctx->r3 | 0X80;
    // 0x80142BC4: j           L_80142C38
    // 0x80142BC8: sw          $v1, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r3;
        goto L_80142C38;
    // 0x80142BC8: sw          $v1, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r3;
L_80142BCC:
    // 0x80142BCC: lw          $v0, 0x4($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X4);
    // 0x80142BD0: subu        $s0, $v0, $s2
    ctx->r16 = SUB32(ctx->r2, ctx->r18);
L_80142BD4:
    // 0x80142BD4: lw          $v0, 0xC($s1)
    ctx->r2 = MEM_W(ctx->r17, 0XC);
    // 0x80142BD8: sw          $s2, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r18;
    // 0x80142BDC: sw          $v0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r2;
L_80142BE0:
    // 0x80142BE0: blez        $s0, L_80142C08
    if (SIGNED(ctx->r16) <= 0) {
        // 0x80142BE4: nop
    
            goto L_80142C08;
    }
    // 0x80142BE4: nop

L_80142BE8:
    // 0x80142BE8: lw          $a0, 0x14($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X14);
    // 0x80142BEC: addu        $a1, $s2, $zero
    ctx->r5 = ADD32(ctx->r18, 0);
    // 0x80142BF0: jal         0x80145020
    // 0x80142BF4: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    func_80145020(rdram, ctx);
        goto after_2;
    // 0x80142BF4: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    after_2:
    // 0x80142BF8: blez        $v0, L_80142BB8
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80142BFC: subu        $s0, $s0, $v0
        ctx->r16 = SUB32(ctx->r16, ctx->r2);
            goto L_80142BB8;
    }
    // 0x80142BFC: subu        $s0, $s0, $v0
    ctx->r16 = SUB32(ctx->r16, ctx->r2);
    // 0x80142C00: bgtz        $s0, L_80142BE8
    if (SIGNED(ctx->r16) > 0) {
        // 0x80142C04: addu        $s2, $s2, $v0
        ctx->r18 = ADD32(ctx->r18, ctx->r2);
            goto L_80142BE8;
    }
    // 0x80142C04: addu        $s2, $s2, $v0
    ctx->r18 = ADD32(ctx->r18, ctx->r2);
L_80142C08:
    // 0x80142C08: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x80142C0C: andi        $v0, $v0, 0x6
    ctx->r2 = ctx->r2 & 0X6;
    // 0x80142C10: bne         $v0, $zero, L_80142C38
    if (ctx->r2 != 0) {
        // 0x80142C14: addu        $v0, $s3, $zero
        ctx->r2 = ADD32(ctx->r19, 0);
            goto L_80142C38;
    }
    // 0x80142C14: addu        $v0, $s3, $zero
    ctx->r2 = ADD32(ctx->r19, 0);
    // 0x80142C18: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x80142C1C: lw          $v1, 0x4($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X4);
    // 0x80142C20: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80142C24: sw          $v0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r2;
    // 0x80142C28: addiu       $v0, $v1, 0x1
    ctx->r2 = ADD32(ctx->r3, 0X1);
    // 0x80142C2C: sw          $v0, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r2;
    // 0x80142C30: sb          $s3, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r19;
    // 0x80142C34: addu        $v0, $s3, $zero
    ctx->r2 = ADD32(ctx->r19, 0);
L_80142C38:
    // 0x80142C38: lw          $ra, 0x28($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X28);
    // 0x80142C3C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80142C40: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80142C44: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80142C48: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80142C4C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80142C50: jr          $ra
    // 0x80142C54: nop

    return;
    // 0x80142C54: nop

;}
RECOMP_FUNC void func_80142C58_original(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142C58: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80142C5C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80142C60: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x80142C64: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x80142C68: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x80142C6C: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x80142C70: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x80142C74: beq         $s0, $zero, L_80142D78
    if (ctx->r16 == 0) {
        // 0x80142C78: sw          $s2, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r18;
            goto L_80142D78;
    }
    // 0x80142C78: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x80142C7C: beq         $s1, $zero, L_80142DD4
    if (ctx->r17 == 0) {
        // 0x80142C80: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_80142DD4;
    }
    // 0x80142C80: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x80142C84: jal         0x801437AC
    // 0x80142C88: nop

    func_801437AC(rdram, ctx);
        goto after_0;
    // 0x80142C88: nop

    after_0:
    // 0x80142C8C: addu        $s2, $v0, $zero
    ctx->r18 = ADD32(ctx->r2, 0);
    // 0x80142C90: beq         $s2, $zero, L_80142D78
    if (ctx->r18 == 0) {
        // 0x80142C94: addiu       $v1, $zero, 0x2B
        ctx->r3 = ADD32(0, 0X2B);
            goto L_80142D78;
    }
    // 0x80142C94: addiu       $v1, $zero, 0x2B
    ctx->r3 = ADD32(0, 0X2B);
    // 0x80142C98: lbu         $v0, 0x1($s1)
    ctx->r2 = MEM_BU(ctx->r17, 0X1);
    // 0x80142C9C: beq         $v0, $v1, L_80142CB8
    if (ctx->r2 == ctx->r3) {
        // 0x80142CA0: addu        $s3, $zero, $zero
        ctx->r19 = ADD32(0, 0);
            goto L_80142CB8;
    }
    // 0x80142CA0: addu        $s3, $zero, $zero
    ctx->r19 = ADD32(0, 0);
    // 0x80142CA4: beq         $v0, $zero, L_80142CBC
    if (ctx->r2 == 0) {
        // 0x80142CA8: nop
    
            goto L_80142CBC;
    }
    // 0x80142CA8: nop

    // 0x80142CAC: lbu         $v0, 0x2($s1)
    ctx->r2 = MEM_BU(ctx->r17, 0X2);
    // 0x80142CB0: bne         $v0, $v1, L_80142CBC
    if (ctx->r2 != ctx->r3) {
        // 0x80142CB4: nop
    
            goto L_80142CBC;
    }
    // 0x80142CB4: nop

L_80142CB8:
    // 0x80142CB8: addiu       $s3, $zero, 0x1
    ctx->r19 = ADD32(0, 0X1);
L_80142CBC:
    // 0x80142CBC: lbu         $v1, 0x0($s1)
    ctx->r3 = MEM_BU(ctx->r17, 0X0);
    // 0x80142CC0: addiu       $v0, $zero, 0x72
    ctx->r2 = ADD32(0, 0X72);
    // 0x80142CC4: beq         $v1, $v0, L_80142D00
    if (ctx->r3 == ctx->r2) {
        // 0x80142CC8: slti        $v0, $v1, 0x73
        ctx->r2 = SIGNED(ctx->r3) < 0X73 ? 1 : 0;
            goto L_80142D00;
    }
    // 0x80142CC8: slti        $v0, $v1, 0x73
    ctx->r2 = SIGNED(ctx->r3) < 0X73 ? 1 : 0;
    // 0x80142CCC: beq         $v0, $zero, L_80142CE4
    if (ctx->r2 == 0) {
        // 0x80142CD0: addiu       $v0, $zero, 0x61
        ctx->r2 = ADD32(0, 0X61);
            goto L_80142CE4;
    }
    // 0x80142CD0: addiu       $v0, $zero, 0x61
    ctx->r2 = ADD32(0, 0X61);
    // 0x80142CD4: beq         $v1, $v0, L_80142CF8
    if (ctx->r3 == ctx->r2) {
        // 0x80142CD8: addiu       $a1, $zero, 0x101
        ctx->r5 = ADD32(0, 0X101);
            goto L_80142CF8;
    }
    // 0x80142CD8: addiu       $a1, $zero, 0x101
    ctx->r5 = ADD32(0, 0X101);
    // 0x80142CDC: j           L_80142DD4
    // 0x80142CE0: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
        goto L_80142DD4;
    // 0x80142CE0: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80142CE4:
    // 0x80142CE4: addiu       $v0, $zero, 0x77
    ctx->r2 = ADD32(0, 0X77);
    // 0x80142CE8: beq         $v1, $v0, L_80142D08
    if (ctx->r3 == ctx->r2) {
        // 0x80142CEC: addiu       $a1, $zero, 0x901
        ctx->r5 = ADD32(0, 0X901);
            goto L_80142D08;
    }
    // 0x80142CEC: addiu       $a1, $zero, 0x901
    ctx->r5 = ADD32(0, 0X901);
    // 0x80142CF0: j           L_80142DD4
    // 0x80142CF4: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
        goto L_80142DD4;
    // 0x80142CF4: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80142CF8:
    // 0x80142CF8: j           L_80142D0C
    // 0x80142CFC: addiu       $v0, $zero, 0x102
    ctx->r2 = ADD32(0, 0X102);
        goto L_80142D0C;
    // 0x80142CFC: addiu       $v0, $zero, 0x102
    ctx->r2 = ADD32(0, 0X102);
L_80142D00:
    // 0x80142D00: j           L_80142D10
    // 0x80142D04: sll         $a1, $s3, 1
    ctx->r5 = S32(ctx->r19 << 1);
        goto L_80142D10;
    // 0x80142D04: sll         $a1, $s3, 1
    ctx->r5 = S32(ctx->r19 << 1);
L_80142D08:
    // 0x80142D08: addiu       $v0, $zero, 0x902
    ctx->r2 = ADD32(0, 0X902);
L_80142D0C:
    // 0x80142D0C: movn        $a1, $v0, $s3
    if (ctx->r19 != 0) ctx->r5 = ctx->r2;
L_80142D10:
    // 0x80142D10: lbu         $v0, 0x1($s1)
    ctx->r2 = MEM_BU(ctx->r17, 0X1);
    // 0x80142D14: addu        $a0, $v0, $zero
    ctx->r4 = ADD32(ctx->r2, 0);
    // 0x80142D18: andi        $v1, $a0, 0xFF
    ctx->r3 = ctx->r4 & 0XFF;
    // 0x80142D1C: addiu       $v0, $zero, 0x2B
    ctx->r2 = ADD32(0, 0X2B);
    // 0x80142D20: bne         $v1, $v0, L_80142D30
    if (ctx->r3 != ctx->r2) {
        // 0x80142D24: addiu       $v0, $zero, 0x74
        ctx->r2 = ADD32(0, 0X74);
            goto L_80142D30;
    }
    // 0x80142D24: addiu       $v0, $zero, 0x74
    ctx->r2 = ADD32(0, 0X74);
    // 0x80142D28: lbu         $a0, 0x2($s1)
    ctx->r4 = MEM_BU(ctx->r17, 0X2);
    // 0x80142D2C: andi        $v1, $a0, 0xFF
    ctx->r3 = ctx->r4 & 0XFF;
L_80142D30:
    // 0x80142D30: bne         $v1, $v0, L_80142D40
    if (ctx->r3 != ctx->r2) {
        // 0x80142D34: addiu       $v0, $zero, 0x62
        ctx->r2 = ADD32(0, 0X62);
            goto L_80142D40;
    }
    // 0x80142D34: addiu       $v0, $zero, 0x62
    ctx->r2 = ADD32(0, 0X62);
    // 0x80142D38: j           L_80142D60
    // 0x80142D3C: ori         $a1, $a1, 0x8
    ctx->r5 = ctx->r5 | 0X8;
        goto L_80142D60;
    // 0x80142D3C: ori         $a1, $a1, 0x8
    ctx->r5 = ctx->r5 | 0X8;
L_80142D40:
    // 0x80142D40: bne         $v1, $v0, L_80142D50
    if (ctx->r3 != ctx->r2) {
        // 0x80142D44: nop
    
            goto L_80142D50;
    }
    // 0x80142D44: nop

    // 0x80142D48: j           L_80142D60
    // 0x80142D4C: ori         $a1, $a1, 0x4
    ctx->r5 = ctx->r5 | 0X4;
        goto L_80142D60;
    // 0x80142D4C: ori         $a1, $a1, 0x4
    ctx->r5 = ctx->r5 | 0X4;
L_80142D50:
    // 0x80142D50: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142D54: lw          $v0, 0x1EF0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1EF0);
    // 0x80142D58: andi        $v0, $v0, 0xC
    ctx->r2 = ctx->r2 & 0XC;
    // 0x80142D5C: or          $a1, $a1, $v0
    ctx->r5 = ctx->r5 | ctx->r2;
L_80142D60:
    // 0x80142D60: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x80142D64: jal         0x801451F0
    // 0x80142D68: addiu       $a2, $zero, 0x1B6
    ctx->r6 = ADD32(0, 0X1B6);
    static_0_801451F0(rdram, ctx);
        goto after_1;
    // 0x80142D68: addiu       $a2, $zero, 0x1B6
    ctx->r6 = ADD32(0, 0X1B6);
    after_1:
    // 0x80142D6C: addu        $s0, $v0, $zero
    ctx->r16 = ADD32(ctx->r2, 0);
    // 0x80142D70: bgez        $s0, L_80142D80
    if (SIGNED(ctx->r16) >= 0) {
        // 0x80142D74: addiu       $v0, $zero, 0x61
        ctx->r2 = ADD32(0, 0X61);
            goto L_80142D80;
    }
    // 0x80142D74: addiu       $v0, $zero, 0x61
    ctx->r2 = ADD32(0, 0X61);
L_80142D78:
    // 0x80142D78: j           L_80142DD4
    // 0x80142D7C: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
        goto L_80142DD4;
    // 0x80142D7C: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80142D80:
    // 0x80142D80: lbu         $s1, 0x0($s1)
    ctx->r17 = MEM_BU(ctx->r17, 0X0);
    // 0x80142D84: bnel        $s1, $v0, L_80142DA0
    if (ctx->r17 != ctx->r2) {
        // 0x80142D88: sw          $zero, 0x0($s2)
        MEM_W(0X0, ctx->r18) = 0;
            goto L_80142DA0;
    }
    goto skip_0;
    // 0x80142D88: sw          $zero, 0x0($s2)
    MEM_W(0X0, ctx->r18) = 0;
    skip_0:
    // 0x80142D8C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x80142D90: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80142D94: jal         0x80144ED8
    // 0x80142D98: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    func_80144ED8(rdram, ctx);
        goto after_2;
    // 0x80142D98: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_2:
    // 0x80142D9C: sw          $zero, 0x0($s2)
    MEM_W(0X0, ctx->r18) = 0;
L_80142DA0:
    // 0x80142DA0: sw          $s0, 0x14($s2)
    MEM_W(0X14, ctx->r18) = ctx->r16;
    // 0x80142DA4: beq         $s3, $zero, L_80142DB4
    if (ctx->r19 == 0) {
        // 0x80142DA8: sw          $zero, 0xC($s2)
        MEM_W(0XC, ctx->r18) = 0;
            goto L_80142DB4;
    }
    // 0x80142DA8: sw          $zero, 0xC($s2)
    MEM_W(0XC, ctx->r18) = 0;
    // 0x80142DAC: j           L_80142DC4
    // 0x80142DB0: addiu       $v0, $zero, 0x200
    ctx->r2 = ADD32(0, 0X200);
        goto L_80142DC4;
    // 0x80142DB0: addiu       $v0, $zero, 0x200
    ctx->r2 = ADD32(0, 0X200);
L_80142DB4:
    // 0x80142DB4: addiu       $v0, $zero, 0x72
    ctx->r2 = ADD32(0, 0X72);
    // 0x80142DB8: bne         $s1, $v0, L_80142DC4
    if (ctx->r17 != ctx->r2) {
        // 0x80142DBC: addiu       $v0, $zero, 0x10
        ctx->r2 = ADD32(0, 0X10);
            goto L_80142DC4;
    }
    // 0x80142DBC: addiu       $v0, $zero, 0x10
    ctx->r2 = ADD32(0, 0X10);
    // 0x80142DC0: addiu       $v0, $zero, 0x8
    ctx->r2 = ADD32(0, 0X8);
L_80142DC4:
    // 0x80142DC4: sw          $v0, 0x10($s2)
    MEM_W(0X10, ctx->r18) = ctx->r2;
    // 0x80142DC8: addu        $v0, $s2, $zero
    ctx->r2 = ADD32(ctx->r18, 0);
    // 0x80142DCC: sw          $zero, 0x4($v0)
    MEM_W(0X4, ctx->r2) = 0;
    // 0x80142DD0: sw          $zero, 0x8($v0)
    MEM_W(0X8, ctx->r2) = 0;
L_80142DD4:
    // 0x80142DD4: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x80142DD8: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x80142DDC: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x80142DE0: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80142DE4: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80142DE8: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80142DEC: jr          $ra
    // 0x80142DF0: nop

    return;
    // 0x80142DF0: nop

;}
RECOMP_FUNC void func_80142E04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142E04: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80142E08: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80142E0C: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x80142E10: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x80142E14: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x80142E18: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x80142E1C: addu        $a0, $a1, $zero
    ctx->r4 = ADD32(ctx->r5, 0);
    // 0x80142E20: andi        $v0, $v1, 0x2
    ctx->r2 = ctx->r3 & 0X2;
    // 0x80142E24: beq         $v0, $zero, L_80142E84
    if (ctx->r2 == 0) {
        // 0x80142E28: sw          $a0, 0x24($sp)
        MEM_W(0X24, ctx->r29) = ctx->r4;
            goto L_80142E84;
    }
    // 0x80142E28: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x80142E2C: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    // 0x80142E30: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    // 0x80142E34: addiu       $v0, $zero, -0x3
    ctx->r2 = ADD32(0, -0X3);
    // 0x80142E38: and         $v0, $v1, $v0
    ctx->r2 = ctx->r3 & ctx->r2;
    // 0x80142E3C: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
    // 0x80142E40: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142E44: addiu       $v0, $v0, 0x5F70
    ctx->r2 = ADD32(ctx->r2, 0X5F70);
    // 0x80142E48: sw          $v0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r2;
    // 0x80142E4C: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;
    // 0x80142E50: addiu       $v0, $zero, 0x4000
    ctx->r2 = ADD32(0, 0X4000);
    // 0x80142E54: jal         0x801412BC
    // 0x80142E58: sw          $v0, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r2;
    func_801412BC(rdram, ctx);
        goto after_0;
    // 0x80142E58: sw          $v0, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r2;
    after_0:
    // 0x80142E5C: addu        $s1, $v0, $zero
    ctx->r17 = ADD32(ctx->r2, 0);
    // 0x80142E60: jal         0x80142988
    // 0x80142E64: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_80142988(rdram, ctx);
        goto after_1;
    // 0x80142E64: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_1:
    // 0x80142E68: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x80142E6C: sw          $zero, 0x8($s0)
    MEM_W(0X8, ctx->r16) = 0;
    // 0x80142E70: sw          $zero, 0xC($s0)
    MEM_W(0XC, ctx->r16) = 0;
    // 0x80142E74: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x80142E78: ori         $v0, $v0, 0x2
    ctx->r2 = ctx->r2 | 0X2;
    // 0x80142E7C: j           L_80142E94
    // 0x80142E80: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
        goto L_80142E94;
    // 0x80142E80: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
L_80142E84:
    // 0x80142E84: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    // 0x80142E88: jal         0x801412BC
    // 0x80142E8C: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    func_801412BC(rdram, ctx);
        goto after_2;
    // 0x80142E8C: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    after_2:
    // 0x80142E90: addu        $s1, $v0, $zero
    ctx->r17 = ADD32(ctx->r2, 0);
L_80142E94:
    // 0x80142E94: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x80142E98: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80142E9C: andi        $v1, $v1, 0x80
    ctx->r3 = ctx->r3 & 0X80;
    // 0x80142EA0: movz        $v0, $s1, $v1
    if (ctx->r3 == 0) ctx->r2 = ctx->r17;
    // 0x80142EA4: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x80142EA8: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80142EAC: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80142EB0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80142EB4: jr          $ra
    // 0x80142EB8: nop

    return;
    // 0x80142EB8: nop

;}
RECOMP_FUNC void func_80142EC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142EC0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80142EC4: addu        $a2, $a1, $zero
    ctx->r6 = ADD32(ctx->r5, 0);
    // 0x80142EC8: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80142ECC: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80142ED0: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80142ED4: bgez        $v0, L_80142EEC
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80142ED8: sw          $v0, 0x0($a2)
        MEM_W(0X0, ctx->r6) = ctx->r2;
            goto L_80142EEC;
    }
    // 0x80142ED8: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x80142EDC: jal         0x80142A68
    // 0x80142EE0: nop

    func_80142A68(rdram, ctx);
        goto after_0;
    // 0x80142EE0: nop

    after_0:
    // 0x80142EE4: j           L_80142F00
    // 0x80142EE8: nop

        goto L_80142F00;
    // 0x80142EE8: nop

L_80142EEC:
    // 0x80142EEC: lw          $a1, 0x4($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X4);
    // 0x80142EF0: andi        $v0, $a0, 0xFF
    ctx->r2 = ctx->r4 & 0XFF;
    // 0x80142EF4: addiu       $v1, $a1, 0x1
    ctx->r3 = ADD32(ctx->r5, 0X1);
    // 0x80142EF8: sw          $v1, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r3;
    // 0x80142EFC: sb          $a0, 0x0($a1)
    MEM_B(0X0, ctx->r5) = ctx->r4;
L_80142F00:
    // 0x80142F00: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80142F04: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80142F08: jr          $ra
    // 0x80142F0C: nop

    return;
    // 0x80142F0C: nop

;}
RECOMP_FUNC void func_80142F10_original(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142F10: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80142F14: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x80142F18: addu        $s3, $a1, $zero
    ctx->r19 = ADD32(ctx->r5, 0);
    // 0x80142F1C: sw          $s4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r20;
    // 0x80142F20: addu        $s4, $a2, $zero
    ctx->r20 = ADD32(ctx->r6, 0);
    // 0x80142F24: mult        $s3, $s4
    result = S64(S32(ctx->r19)) * S64(S32(ctx->r20)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80142F28: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x80142F2C: addu        $s1, $a3, $zero
    ctx->r17 = ADD32(ctx->r7, 0);
    // 0x80142F30: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x80142F34: sw          $ra, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r31;
    // 0x80142F38: sw          $s5, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r21;
    // 0x80142F3C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80142F40: mflo        $s0
    ctx->r16 = lo;
    // 0x80142F44: blez        $s0, L_80142FE0
    if (SIGNED(ctx->r16) <= 0) {
        // 0x80142F48: addu        $s2, $a0, $zero
        ctx->r18 = ADD32(ctx->r4, 0);
            goto L_80142FE0;
    }
    // 0x80142F48: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x80142F4C: addiu       $s5, $zero, -0x1
    ctx->r21 = ADD32(0, -0X1);
L_80142F50:
    // 0x80142F50: lw          $a2, 0x0($s1)
    ctx->r6 = MEM_W(ctx->r17, 0X0);
    // 0x80142F54: slt         $v0, $a2, $s0
    ctx->r2 = SIGNED(ctx->r6) < SIGNED(ctx->r16) ? 1 : 0;
    // 0x80142F58: beq         $v0, $zero, L_80142FA8
    if (ctx->r2 == 0) {
        // 0x80142F5C: nop
    
            goto L_80142FA8;
    }
    // 0x80142F5C: nop

    // 0x80142F60: blez        $a2, L_80142F80
    if (SIGNED(ctx->r6) <= 0) {
        // 0x80142F64: nop
    
            goto L_80142F80;
    }
    // 0x80142F64: nop

    // 0x80142F68: lw          $a1, 0x4($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X4);
    // 0x80142F6C: jal         0x80141168
    // 0x80142F70: addu        $a0, $s2, $zero
    ctx->r4 = ADD32(ctx->r18, 0);
    func_80141168(rdram, ctx);
        goto after_0;
    // 0x80142F70: addu        $a0, $s2, $zero
    ctx->r4 = ADD32(ctx->r18, 0);
    after_0:
    // 0x80142F74: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x80142F78: addu        $s2, $s2, $v0
    ctx->r18 = ADD32(ctx->r18, ctx->r2);
    // 0x80142F7C: subu        $s0, $s0, $v0
    ctx->r16 = SUB32(ctx->r16, ctx->r2);
L_80142F80:
    // 0x80142F80: jal         0x8014303C
    // 0x80142F84: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    func_8014303C(rdram, ctx);
        goto after_1;
    // 0x80142F84: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    after_1:
    // 0x80142F88: beq         $v0, $s5, L_80142FE0
    if (ctx->r2 == ctx->r21) {
        // 0x80142F8C: nop
    
            goto L_80142FE0;
    }
    // 0x80142F8C: nop

    // 0x80142F90: sb          $v0, 0x0($s2)
    MEM_B(0X0, ctx->r18) = ctx->r2;
    // 0x80142F94: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x80142F98: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80142F9C: slt         $v0, $v0, $s0
    ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r16) ? 1 : 0;
    // 0x80142FA0: bne         $v0, $zero, L_80142FD8
    if (ctx->r2 != 0) {
        // 0x80142FA4: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80142FD8;
    }
    // 0x80142FA4: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_80142FA8:
    // 0x80142FA8: lw          $a1, 0x4($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X4);
    // 0x80142FAC: addu        $a0, $s2, $zero
    ctx->r4 = ADD32(ctx->r18, 0);
    // 0x80142FB0: jal         0x80141168
    // 0x80142FB4: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    func_80141168(rdram, ctx);
        goto after_2;
    // 0x80142FB4: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    after_2:
    // 0x80142FB8: lw          $v1, 0x4($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X4);
    // 0x80142FBC: lw          $a0, 0x0($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X0);
    // 0x80142FC0: addu        $v0, $s4, $zero
    ctx->r2 = ADD32(ctx->r20, 0);
    // 0x80142FC4: addu        $v1, $v1, $s0
    ctx->r3 = ADD32(ctx->r3, ctx->r16);
    // 0x80142FC8: subu        $a0, $a0, $s0
    ctx->r4 = SUB32(ctx->r4, ctx->r16);
    // 0x80142FCC: sw          $v1, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r3;
    // 0x80142FD0: j           L_8014300C
    // 0x80142FD4: sw          $a0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r4;
        goto L_8014300C;
    // 0x80142FD4: sw          $a0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r4;
L_80142FD8:
    // 0x80142FD8: bgtz        $s0, L_80142F50
    if (SIGNED(ctx->r16) > 0) {
        // 0x80142FDC: nop
    
            goto L_80142F50;
    }
    // 0x80142FDC: nop

L_80142FE0:
    // 0x80142FE0: beq         $s3, $zero, L_80143008
    if (ctx->r19 == 0) {
        // 0x80142FE4: addu        $v0, $s0, $s3
        ctx->r2 = ADD32(ctx->r16, ctx->r19);
            goto L_80143008;
    }
    // 0x80142FE4: addu        $v0, $s0, $s3
    ctx->r2 = ADD32(ctx->r16, ctx->r19);
    // 0x80142FE8: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80142FEC: divu        $zero, $v0, $s3
    lo = S32(U32(ctx->r2) / U32(ctx->r19)); hi = S32(U32(ctx->r2) % U32(ctx->r19));
    // 0x80142FF0: bne         $s3, $zero, L_80142FFC
    if (ctx->r19 != 0) {
        // 0x80142FF4: nop
    
            goto L_80142FFC;
    }
    // 0x80142FF4: nop

    // 0x80142FF8: break       7
    do_break(2148806648);
L_80142FFC:
    // 0x80142FFC: mflo        $v0
    ctx->r2 = lo;
    // 0x80143000: j           L_8014300C
    // 0x80143004: subu        $v0, $s4, $v0
    ctx->r2 = SUB32(ctx->r20, ctx->r2);
        goto L_8014300C;
    // 0x80143004: subu        $v0, $s4, $v0
    ctx->r2 = SUB32(ctx->r20, ctx->r2);
L_80143008:
    // 0x80143008: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_8014300C:
    // 0x8014300C: lw          $ra, 0x28($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X28);
    // 0x80143010: lw          $s5, 0x24($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X24);
    // 0x80143014: lw          $s4, 0x20($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X20);
    // 0x80143018: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x8014301C: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x80143020: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80143024: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80143028: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8014302C: jr          $ra
    // 0x80143030: nop

    return;
    // 0x80143030: nop

;}
RECOMP_FUNC void func_8014303C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014303C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80143040: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80143044: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x80143048: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x8014304C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80143050: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x80143054: andi        $v0, $v1, 0x200
    ctx->r2 = ctx->r3 & 0X200;
    // 0x80143058: beq         $v0, $zero, L_80143068
    if (ctx->r2 == 0) {
        // 0x8014305C: ori         $v0, $v1, 0x8
        ctx->r2 = ctx->r3 | 0X8;
            goto L_80143068;
    }
    // 0x8014305C: ori         $v0, $v1, 0x8
    ctx->r2 = ctx->r3 | 0X8;
    // 0x80143060: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
    // 0x80143064: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
L_80143068:
    // 0x80143068: andi        $v0, $v1, 0x8
    ctx->r2 = ctx->r3 & 0X8;
    // 0x8014306C: beq         $v0, $zero, L_801431BC
    if (ctx->r2 == 0) {
        // 0x80143070: andi        $v0, $v1, 0x140
        ctx->r2 = ctx->r3 & 0X140;
            goto L_801431BC;
    }
    // 0x80143070: andi        $v0, $v1, 0x140
    ctx->r2 = ctx->r3 & 0X140;
    // 0x80143074: bne         $v0, $zero, L_801431D4
    if (ctx->r2 != 0) {
        // 0x80143078: addiu       $v0, $zero, -0x1
        ctx->r2 = ADD32(0, -0X1);
            goto L_801431D4;
    }
    // 0x80143078: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_8014307C:
    // 0x8014307C: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
    // 0x80143080: bne         $v0, $zero, L_801430D4
    if (ctx->r2 != 0) {
        // 0x80143084: nop
    
            goto L_801430D4;
    }
    // 0x80143084: nop

    // 0x80143088: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x8014308C: andi        $v0, $v0, 0x2
    ctx->r2 = ctx->r2 & 0X2;
    // 0x80143090: beq         $v0, $zero, L_801430A0
    if (ctx->r2 == 0) {
        // 0x80143094: addiu       $v0, $sp, 0x10
        ctx->r2 = ADD32(ctx->r29, 0X10);
            goto L_801430A0;
    }
    // 0x80143094: addiu       $v0, $sp, 0x10
    ctx->r2 = ADD32(ctx->r29, 0X10);
    // 0x80143098: j           L_8014307C
    // 0x8014309C: sw          $v0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r2;
        goto L_8014307C;
    // 0x8014309C: sw          $v0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r2;
L_801430A0:
    // 0x801430A0: jal         0x80143A40
    // 0x801430A4: addiu       $a0, $zero, 0x2000
    ctx->r4 = ADD32(0, 0X2000);
    func_80143A40(rdram, ctx);
        goto after_0;
    // 0x801430A4: addiu       $a0, $zero, 0x2000
    ctx->r4 = ADD32(0, 0X2000);
    after_0:
    // 0x801430A8: sw          $v0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r2;
    // 0x801430AC: bne         $v0, $zero, L_801430C4
    if (ctx->r2 != 0) {
        // 0x801430B0: addiu       $s1, $zero, 0x2000
        ctx->r17 = ADD32(0, 0X2000);
            goto L_801430C4;
    }
    // 0x801430B0: addiu       $s1, $zero, 0x2000
    ctx->r17 = ADD32(0, 0X2000);
    // 0x801430B4: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x801430B8: ori         $v0, $v0, 0x2
    ctx->r2 = ctx->r2 | 0X2;
    // 0x801430BC: j           L_8014307C
    // 0x801430C0: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
        goto L_8014307C;
    // 0x801430C0: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
L_801430C4:
    // 0x801430C4: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x801430C8: sw          $s1, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r17;
    // 0x801430CC: ori         $v0, $v0, 0x20
    ctx->r2 = ctx->r2 | 0X20;
    // 0x801430D0: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
L_801430D4:
    // 0x801430D4: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801430D8: addiu       $v0, $v0, 0x1DB8
    ctx->r2 = ADD32(ctx->r2, 0X1DB8);
    // 0x801430DC: bne         $s0, $v0, L_80143124
    if (ctx->r16 != ctx->r2) {
        // 0x801430E0: nop
    
            goto L_80143124;
    }
    // 0x801430E0: nop

    // 0x801430E4: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x801430E8: addiu       $a0, $a0, 0x1DE8
    ctx->r4 = ADD32(ctx->r4, 0X1DE8);
    // 0x801430EC: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x801430F0: andi        $v0, $v0, 0x4
    ctx->r2 = ctx->r2 & 0X4;
    // 0x801430F4: beq         $v0, $zero, L_80143104
    if (ctx->r2 == 0) {
        // 0x801430F8: nop
    
            goto L_80143104;
    }
    // 0x801430F8: nop

    // 0x801430FC: jal         0x80142988
    // 0x80143100: addiu       $a0, $a0, -0x10
    ctx->r4 = ADD32(ctx->r4, -0X10);
    func_80142988(rdram, ctx);
        goto after_1;
    // 0x80143100: addiu       $a0, $a0, -0x10
    ctx->r4 = ADD32(ctx->r4, -0X10);
    after_1:
L_80143104:
    // 0x80143104: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80143108: addiu       $a0, $a0, 0x1E08
    ctx->r4 = ADD32(ctx->r4, 0X1E08);
    // 0x8014310C: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x80143110: andi        $v0, $v0, 0x4
    ctx->r2 = ctx->r2 & 0X4;
    // 0x80143114: beq         $v0, $zero, L_80143124
    if (ctx->r2 == 0) {
        // 0x80143118: nop
    
            goto L_80143124;
    }
    // 0x80143118: nop

    // 0x8014311C: jal         0x80142988
    // 0x80143120: addiu       $a0, $a0, -0x10
    ctx->r4 = ADD32(ctx->r4, -0X10);
    func_80142988(rdram, ctx);
        goto after_2;
    // 0x80143120: addiu       $a0, $a0, -0x10
    ctx->r4 = ADD32(ctx->r4, -0X10);
    after_2:
L_80143124:
    // 0x80143124: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x80143128: andi        $v0, $v0, 0x2
    ctx->r2 = ctx->r2 & 0X2;
    // 0x8014312C: bne         $v0, $zero, L_80143138
    if (ctx->r2 != 0) {
        // 0x80143130: addiu       $a2, $zero, 0x1
        ctx->r6 = ADD32(0, 0X1);
            goto L_80143138;
    }
    // 0x80143130: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x80143134: lw          $a2, 0xC($s0)
    ctx->r6 = MEM_W(ctx->r16, 0XC);
L_80143138:
    // 0x80143138: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8014313C: lw          $a1, 0x8($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X8);
    // 0x80143140: jal         0x80144EF8
    // 0x80143144: nop

    func_80144EF8(rdram, ctx);
        goto after_3;
    // 0x80143144: nop

    after_3:
    // 0x80143148: lw          $a0, 0x8($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X8);
    // 0x8014314C: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x80143150: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
    // 0x80143154: andi        $v1, $v1, 0x2
    ctx->r3 = ctx->r3 & 0X2;
    // 0x80143158: beq         $v1, $zero, L_80143170
    if (ctx->r3 == 0) {
        // 0x8014315C: sw          $a0, 0x4($s0)
        MEM_W(0X4, ctx->r16) = ctx->r4;
            goto L_80143170;
    }
    // 0x8014315C: sw          $a0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r4;
    // 0x80143160: lw          $v1, 0x8($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X8);
    // 0x80143164: addiu       $v0, $sp, 0x10
    ctx->r2 = ADD32(ctx->r29, 0X10);
    // 0x80143168: beql        $v1, $v0, L_80143170
    if (ctx->r3 == ctx->r2) {
        // 0x8014316C: sw          $zero, 0x8($s0)
        MEM_W(0X8, ctx->r16) = 0;
            goto L_80143170;
    }
    goto skip_0;
    // 0x8014316C: sw          $zero, 0x8($s0)
    MEM_W(0X8, ctx->r16) = 0;
    skip_0:
L_80143170:
    // 0x80143170: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x80143174: addiu       $v1, $v0, -0x1
    ctx->r3 = ADD32(ctx->r2, -0X1);
    // 0x80143178: bgez        $v1, L_801431C4
    if (SIGNED(ctx->r3) >= 0) {
        // 0x8014317C: sw          $v1, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r3;
            goto L_801431C4;
    }
    // 0x8014317C: sw          $v1, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r3;
    // 0x80143180: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80143184: bne         $v1, $v0, L_801431AC
    if (ctx->r3 != ctx->r2) {
        // 0x80143188: nop
    
            goto L_801431AC;
    }
    // 0x80143188: nop

    // 0x8014318C: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x80143190: ori         $v1, $v0, 0x40
    ctx->r3 = ctx->r2 | 0X40;
    // 0x80143194: andi        $v0, $v1, 0x200
    ctx->r2 = ctx->r3 & 0X200;
    // 0x80143198: beq         $v0, $zero, L_801431B8
    if (ctx->r2 == 0) {
        // 0x8014319C: sw          $v1, 0x10($s0)
        MEM_W(0X10, ctx->r16) = ctx->r3;
            goto L_801431B8;
    }
    // 0x8014319C: sw          $v1, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r3;
    // 0x801431A0: addiu       $v0, $zero, -0x9
    ctx->r2 = ADD32(0, -0X9);
    // 0x801431A4: j           L_801431B4
    // 0x801431A8: and         $v0, $v1, $v0
    ctx->r2 = ctx->r3 & ctx->r2;
        goto L_801431B4;
    // 0x801431A8: and         $v0, $v1, $v0
    ctx->r2 = ctx->r3 & ctx->r2;
L_801431AC:
    // 0x801431AC: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x801431B0: ori         $v0, $v0, 0x80
    ctx->r2 = ctx->r2 | 0X80;
L_801431B4:
    // 0x801431B4: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
L_801431B8:
    // 0x801431B8: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
L_801431BC:
    // 0x801431BC: j           L_801431D4
    // 0x801431C0: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_801431D4;
    // 0x801431C0: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_801431C4:
    // 0x801431C4: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x801431C8: addiu       $v0, $v1, 0x1
    ctx->r2 = ADD32(ctx->r3, 0X1);
    // 0x801431CC: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;
    // 0x801431D0: lbu         $v0, 0x0($v1)
    ctx->r2 = MEM_BU(ctx->r3, 0X0);
L_801431D4:
    // 0x801431D4: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x801431D8: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x801431DC: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801431E0: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x801431E4: jr          $ra
    // 0x801431E8: nop

    return;
    // 0x801431E8: nop

;}
RECOMP_FUNC void func_801431F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801431F0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801431F4: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x801431F8: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x801431FC: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x80143200: addu        $s3, $a1, $zero
    ctx->r19 = ADD32(ctx->r5, 0);
    // 0x80143204: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x80143208: addu        $s2, $a2, $zero
    ctx->r18 = ADD32(ctx->r6, 0);
    // 0x8014320C: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x80143210: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x80143214: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x80143218: addiu       $v0, $zero, -0x41
    ctx->r2 = ADD32(0, -0X41);
    // 0x8014321C: and         $v0, $v1, $v0
    ctx->r2 = ctx->r3 & ctx->r2;
    // 0x80143220: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
    // 0x80143224: andi        $v0, $v1, 0x8
    ctx->r2 = ctx->r3 & 0X8;
    // 0x80143228: beq         $v0, $zero, L_801432A8
    if (ctx->r2 == 0) {
        // 0x8014322C: addiu       $s1, $zero, -0x1
        ctx->r17 = ADD32(0, -0X1);
            goto L_801432A8;
    }
    // 0x8014322C: addiu       $s1, $zero, -0x1
    ctx->r17 = ADD32(0, -0X1);
    // 0x80143230: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80143234: bne         $s2, $v0, L_8014325C
    if (ctx->r18 != ctx->r2) {
        // 0x80143238: nop
    
            goto L_8014325C;
    }
    // 0x80143238: nop

    // 0x8014323C: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
    // 0x80143240: beq         $v0, $zero, L_8014325C
    if (ctx->r2 == 0) {
        // 0x80143244: andi        $v0, $v1, 0x2
        ctx->r2 = ctx->r3 & 0X2;
            goto L_8014325C;
    }
    // 0x80143244: andi        $v0, $v1, 0x2
    ctx->r2 = ctx->r3 & 0X2;
    // 0x80143248: bne         $v0, $zero, L_8014325C
    if (ctx->r2 != 0) {
        // 0x8014324C: nop
    
            goto L_8014325C;
    }
    // 0x8014324C: nop

    // 0x80143250: jal         0x80143318
    // 0x80143254: addu        $s2, $zero, $zero
    ctx->r18 = ADD32(0, 0);
    func_80143318(rdram, ctx);
        goto after_0;
    // 0x80143254: addu        $s2, $zero, $zero
    ctx->r18 = ADD32(0, 0);
    after_0:
    // 0x80143258: addu        $s3, $s3, $v0
    ctx->r19 = ADD32(ctx->r19, ctx->r2);
L_8014325C:
    // 0x8014325C: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x80143260: andi        $v0, $v0, 0x200
    ctx->r2 = ctx->r2 & 0X200;
    // 0x80143264: beq         $v0, $zero, L_80143284
    if (ctx->r2 == 0) {
        // 0x80143268: nop
    
            goto L_80143284;
    }
    // 0x80143268: nop

    // 0x8014326C: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
    // 0x80143270: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x80143274: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;
    // 0x80143278: addiu       $v0, $zero, -0x9
    ctx->r2 = ADD32(0, -0X9);
    // 0x8014327C: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x80143280: sw          $v1, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r3;
L_80143284:
    // 0x80143284: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x80143288: addu        $a1, $s3, $zero
    ctx->r5 = ADD32(ctx->r19, 0);
    // 0x8014328C: jal         0x80144ED8
    // 0x80143290: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    func_80144ED8(rdram, ctx);
        goto after_1;
    // 0x80143290: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    after_1:
    // 0x80143294: lw          $v1, 0x8($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X8);
    // 0x80143298: addu        $s1, $v0, $zero
    ctx->r17 = ADD32(ctx->r2, 0);
    // 0x8014329C: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x801432A0: j           L_801432EC
    // 0x801432A4: sw          $v1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r3;
        goto L_801432EC;
    // 0x801432A4: sw          $v1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r3;
L_801432A8:
    // 0x801432A8: andi        $v0, $v1, 0x210
    ctx->r2 = ctx->r3 & 0X210;
    // 0x801432AC: beq         $v0, $zero, L_801432F0
    if (ctx->r2 == 0) {
        // 0x801432B0: addiu       $v0, $zero, -0x1
        ctx->r2 = ADD32(0, -0X1);
            goto L_801432F0;
    }
    // 0x801432B0: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x801432B4: jal         0x80142988
    // 0x801432B8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_80142988(rdram, ctx);
        goto after_2;
    // 0x801432B8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_2:
    // 0x801432BC: addu        $s1, $v0, $zero
    ctx->r17 = ADD32(ctx->r2, 0);
    // 0x801432C0: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x801432C4: addu        $a1, $s3, $zero
    ctx->r5 = ADD32(ctx->r19, 0);
    // 0x801432C8: jal         0x80144ED8
    // 0x801432CC: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    func_80144ED8(rdram, ctx);
        goto after_3;
    // 0x801432CC: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    after_3:
    // 0x801432D0: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
    // 0x801432D4: beql        $v0, $v1, L_801432F8
    if (ctx->r2 == ctx->r3) {
        // 0x801432D8: addiu       $v0, $zero, -0x1
        ctx->r2 = ADD32(0, -0X1);
            goto L_801432F8;
    }
    goto skip_0;
    // 0x801432D8: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    skip_0:
    // 0x801432DC: bne         $s1, $v1, L_801432F8
    if (ctx->r17 != ctx->r3) {
        // 0x801432E0: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_801432F8;
    }
    // 0x801432E0: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x801432E4: j           L_801432F8
    // 0x801432E8: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_801432F8;
    // 0x801432E8: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_801432EC:
    // 0x801432EC: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_801432F0:
    // 0x801432F0: nor         $v1, $zero, $s1
    ctx->r3 = ~(0 | ctx->r17);
    // 0x801432F4: movn        $v0, $zero, $v1
    if (ctx->r3 != 0) ctx->r2 = ctx->r0;
L_801432F8:
    // 0x801432F8: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x801432FC: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x80143300: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x80143304: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80143308: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8014330C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80143310: jr          $ra
    // 0x80143314: nop

    return;
    // 0x80143314: nop

;}
RECOMP_FUNC void func_80143318(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143318: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8014331C: addu        $a2, $a0, $zero
    ctx->r6 = ADD32(ctx->r4, 0);
    // 0x80143320: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80143324: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80143328: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x8014332C: bgez        $v0, L_80143338
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80143330: addu        $s0, $zero, $zero
        ctx->r16 = ADD32(0, 0);
            goto L_80143338;
    }
    // 0x80143330: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x80143334: sw          $zero, 0x0($a2)
    MEM_W(0X0, ctx->r6) = 0;
L_80143338:
    // 0x80143338: lw          $v1, 0x10($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X10);
    // 0x8014333C: andi        $v0, $v1, 0x8
    ctx->r2 = ctx->r3 & 0X8;
    // 0x80143340: beq         $v0, $zero, L_801433B0
    if (ctx->r2 == 0) {
        // 0x80143344: andi        $v0, $v1, 0x210
        ctx->r2 = ctx->r3 & 0X210;
            goto L_801433B0;
    }
    // 0x80143344: andi        $v0, $v1, 0x210
    ctx->r2 = ctx->r3 & 0X210;
    // 0x80143348: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8014334C: lw          $v0, 0x1EEC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1EEC);
    // 0x80143350: lw          $v1, 0x14($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X14);
    // 0x80143354: lw          $a1, 0x0($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X0);
    // 0x80143358: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8014335C: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x80143360: andi        $v0, $v0, 0x8
    ctx->r2 = ctx->r2 & 0X8;
    // 0x80143364: beq         $v0, $zero, L_80143434
    if (ctx->r2 == 0) {
        // 0x80143368: negu        $s0, $a1
        ctx->r16 = SUB32(0, ctx->r5);
            goto L_80143434;
    }
    // 0x80143368: negu        $s0, $a1
    ctx->r16 = SUB32(0, ctx->r5);
    // 0x8014336C: beq         $a1, $zero, L_80143434
    if (ctx->r5 == 0) {
        // 0x80143370: nop
    
            goto L_80143434;
    }
    // 0x80143370: nop

    // 0x80143374: lw          $a0, 0x4($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X4);
    // 0x80143378: addu        $v0, $a0, $a1
    ctx->r2 = ADD32(ctx->r4, ctx->r5);
    // 0x8014337C: addiu       $v1, $v0, -0x1
    ctx->r3 = ADD32(ctx->r2, -0X1);
    // 0x80143380: sltu        $v0, $v1, $a0
    ctx->r2 = ctx->r3 < ctx->r4 ? 1 : 0;
    // 0x80143384: bne         $v0, $zero, L_80143434
    if (ctx->r2 != 0) {
        // 0x80143388: addiu       $a1, $zero, 0xA
        ctx->r5 = ADD32(0, 0XA);
            goto L_80143434;
    }
    // 0x80143388: addiu       $a1, $zero, 0xA
    ctx->r5 = ADD32(0, 0XA);
L_8014338C:
    // 0x8014338C: lbu         $v0, 0x0($v1)
    ctx->r2 = MEM_BU(ctx->r3, 0X0);
    // 0x80143390: beql        $v0, $a1, L_80143398
    if (ctx->r2 == ctx->r5) {
        // 0x80143394: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_80143398;
    }
    goto skip_0;
    // 0x80143394: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    skip_0:
L_80143398:
    // 0x80143398: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x8014339C: sltu        $v0, $v1, $a0
    ctx->r2 = ctx->r3 < ctx->r4 ? 1 : 0;
    // 0x801433A0: beq         $v0, $zero, L_8014338C
    if (ctx->r2 == 0) {
        // 0x801433A4: nop
    
            goto L_8014338C;
    }
    // 0x801433A4: nop

    // 0x801433A8: j           L_80143434
    // 0x801433AC: nop

        goto L_80143434;
    // 0x801433AC: nop

L_801433B0:
    // 0x801433B0: beq         $v0, $zero, L_8014342C
    if (ctx->r2 == 0) {
        // 0x801433B4: andi        $v0, $v1, 0x10
        ctx->r2 = ctx->r3 & 0X10;
            goto L_8014342C;
    }
    // 0x801433B4: andi        $v0, $v1, 0x10
    ctx->r2 = ctx->r3 & 0X10;
    // 0x801433B8: beq         $v0, $zero, L_80143434
    if (ctx->r2 == 0) {
        // 0x801433BC: nop
    
            goto L_80143434;
    }
    // 0x801433BC: nop

    // 0x801433C0: lw          $a1, 0x8($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X8);
    // 0x801433C4: beq         $a1, $zero, L_80143434
    if (ctx->r5 == 0) {
        // 0x801433C8: andi        $v0, $v1, 0x2
        ctx->r2 = ctx->r3 & 0X2;
            goto L_80143434;
    }
    // 0x801433C8: andi        $v0, $v1, 0x2
    ctx->r2 = ctx->r3 & 0X2;
    // 0x801433CC: bne         $v0, $zero, L_80143434
    if (ctx->r2 != 0) {
        // 0x801433D0: nop
    
            goto L_80143434;
    }
    // 0x801433D0: nop

    // 0x801433D4: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801433D8: lw          $v0, 0x1EEC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1EEC);
    // 0x801433DC: lw          $v1, 0x14($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X14);
    // 0x801433E0: lw          $a0, 0x4($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X4);
    // 0x801433E4: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x801433E8: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x801433EC: andi        $v0, $v0, 0x8
    ctx->r2 = ctx->r2 & 0X8;
    // 0x801433F0: beq         $v0, $zero, L_80143434
    if (ctx->r2 == 0) {
        // 0x801433F4: subu        $s0, $a0, $a1
        ctx->r16 = SUB32(ctx->r4, ctx->r5);
            goto L_80143434;
    }
    // 0x801433F4: subu        $s0, $a0, $a1
    ctx->r16 = SUB32(ctx->r4, ctx->r5);
    // 0x801433F8: blez        $s0, L_80143434
    if (SIGNED(ctx->r16) <= 0) {
        // 0x801433FC: addiu       $a3, $zero, 0xA
        ctx->r7 = ADD32(0, 0XA);
            goto L_80143434;
    }
    // 0x801433FC: addiu       $a3, $zero, 0xA
    ctx->r7 = ADD32(0, 0XA);
    // 0x80143400: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x80143404: addu        $a0, $s0, $v1
    ctx->r4 = ADD32(ctx->r16, ctx->r3);
L_80143408:
    // 0x80143408: lbu         $v0, 0x0($v1)
    ctx->r2 = MEM_BU(ctx->r3, 0X0);
    // 0x8014340C: beql        $v0, $a3, L_80143414
    if (ctx->r2 == ctx->r7) {
        // 0x80143410: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80143414;
    }
    goto skip_1;
    // 0x80143410: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_1:
L_80143414:
    // 0x80143414: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80143418: slt         $v0, $v1, $a0
    ctx->r2 = SIGNED(ctx->r3) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x8014341C: beq         $v0, $zero, L_80143434
    if (ctx->r2 == 0) {
        // 0x80143420: nop
    
            goto L_80143434;
    }
    // 0x80143420: nop

    // 0x80143424: j           L_80143408
    // 0x80143428: nop

        goto L_80143408;
    // 0x80143428: nop

L_8014342C:
    // 0x8014342C: j           L_8014344C
    // 0x80143430: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_8014344C;
    // 0x80143430: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80143434:
    // 0x80143434: lw          $a0, 0x14($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X14);
    // 0x80143438: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x8014343C: jal         0x80144ED8
    // 0x80143440: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_80144ED8(rdram, ctx);
        goto after_0;
    // 0x80143440: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_0:
    // 0x80143444: bgezl       $v0, L_8014344C
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80143448: addu        $v0, $v0, $s0
        ctx->r2 = ADD32(ctx->r2, ctx->r16);
            goto L_8014344C;
    }
    goto skip_2;
    // 0x80143448: addu        $v0, $v0, $s0
    ctx->r2 = ADD32(ctx->r2, ctx->r16);
    skip_2:
L_8014344C:
    // 0x8014344C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80143450: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80143454: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80143458: jr          $ra
    // 0x8014345C: nop

    return;
    // 0x8014345C: nop

;}
RECOMP_FUNC void func_80143460(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143460: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80143464: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x80143468: addu        $s3, $a1, $zero
    ctx->r19 = ADD32(ctx->r5, 0);
    // 0x8014346C: sw          $s4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r20;
    // 0x80143470: addu        $s4, $a2, $zero
    ctx->r20 = ADD32(ctx->r6, 0);
    // 0x80143474: mult        $s3, $s4
    result = S64(S32(ctx->r19)) * S64(S32(ctx->r20)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80143478: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8014347C: addu        $s0, $a3, $zero
    ctx->r16 = ADD32(ctx->r7, 0);
    // 0x80143480: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80143484: sw          $s6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r22;
    // 0x80143488: sw          $s5, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r21;
    // 0x8014348C: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x80143490: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x80143494: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x80143498: andi        $v0, $v0, 0x4
    ctx->r2 = ctx->r2 & 0X4;
    // 0x8014349C: mflo        $s2
    ctx->r18 = lo;
    // 0x801434A0: beq         $v0, $zero, L_80143520
    if (ctx->r2 == 0) {
        // 0x801434A4: addu        $s1, $a0, $zero
        ctx->r17 = ADD32(ctx->r4, 0);
            goto L_80143520;
    }
    // 0x801434A4: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x801434A8: blez        $s2, L_801435D0
    if (SIGNED(ctx->r18) <= 0) {
        // 0x801434AC: addiu       $s6, $zero, 0xA
        ctx->r22 = ADD32(0, 0XA);
            goto L_801435D0;
    }
    // 0x801434AC: addiu       $s6, $zero, 0xA
    ctx->r22 = ADD32(0, 0XA);
    // 0x801434B0: addiu       $s5, $zero, -0x1
    ctx->r21 = ADD32(0, -0X1);
L_801434B4:
    // 0x801434B4: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x801434B8: lw          $v0, 0xC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC);
    // 0x801434BC: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x801434C0: negu        $v0, $v0
    ctx->r2 = SUB32(0, ctx->r2);
    // 0x801434C4: slt         $v0, $v0, $v1
    ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x801434C8: beq         $v0, $zero, L_801434F4
    if (ctx->r2 == 0) {
        // 0x801434CC: sw          $v1, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r3;
            goto L_801434F4;
    }
    // 0x801434CC: sw          $v1, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r3;
    // 0x801434D0: lbu         $a0, 0x0($s1)
    ctx->r4 = MEM_BU(ctx->r17, 0X0);
    // 0x801434D4: beq         $a0, $s6, L_801434F4
    if (ctx->r4 == ctx->r22) {
        // 0x801434D8: nop
    
            goto L_801434F4;
    }
    // 0x801434D8: nop

    // 0x801434DC: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x801434E0: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x801434E4: addiu       $v0, $v1, 0x1
    ctx->r2 = ADD32(ctx->r3, 0X1);
    // 0x801434E8: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;
    // 0x801434EC: j           L_8014350C
    // 0x801434F0: sb          $a0, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r4;
        goto L_8014350C;
    // 0x801434F0: sb          $a0, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r4;
L_801434F4:
    // 0x801434F4: lbu         $a0, 0x0($s1)
    ctx->r4 = MEM_BU(ctx->r17, 0X0);
    // 0x801434F8: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x801434FC: jal         0x80142A68
    // 0x80143500: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    func_80142A68(rdram, ctx);
        goto after_0;
    // 0x80143500: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_0:
    // 0x80143504: beq         $v0, $s5, L_801435D0
    if (ctx->r2 == ctx->r21) {
        // 0x80143508: nop
    
            goto L_801435D0;
    }
    // 0x80143508: nop

L_8014350C:
    // 0x8014350C: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x80143510: blez        $s2, L_801435D0
    if (SIGNED(ctx->r18) <= 0) {
        // 0x80143514: nop
    
            goto L_801435D0;
    }
    // 0x80143514: nop

    // 0x80143518: j           L_801434B4
    // 0x8014351C: nop

        goto L_801434B4;
    // 0x8014351C: nop

L_80143520:
    // 0x80143520: blez        $s2, L_801435D0
    if (SIGNED(ctx->r18) <= 0) {
        // 0x80143524: nop
    
            goto L_801435D0;
    }
    // 0x80143524: nop

    // 0x80143528: addiu       $s5, $zero, -0x1
    ctx->r21 = ADD32(0, -0X1);
L_8014352C:
    // 0x8014352C: lw          $a2, 0x0($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X0);
    // 0x80143530: slt         $v0, $a2, $s2
    ctx->r2 = SIGNED(ctx->r6) < SIGNED(ctx->r18) ? 1 : 0;
    // 0x80143534: beq         $v0, $zero, L_80143598
    if (ctx->r2 == 0) {
        // 0x80143538: nop
    
            goto L_80143598;
    }
    // 0x80143538: nop

    // 0x8014353C: blez        $a2, L_8014356C
    if (SIGNED(ctx->r6) <= 0) {
        // 0x80143540: nop
    
            goto L_8014356C;
    }
    // 0x80143540: nop

    // 0x80143544: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
    // 0x80143548: jal         0x80141168
    // 0x8014354C: addu        $a1, $s1, $zero
    ctx->r5 = ADD32(ctx->r17, 0);
    func_80141168(rdram, ctx);
        goto after_1;
    // 0x8014354C: addu        $a1, $s1, $zero
    ctx->r5 = ADD32(ctx->r17, 0);
    after_1:
    // 0x80143550: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    // 0x80143554: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x80143558: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x8014355C: addu        $s1, $s1, $a0
    ctx->r17 = ADD32(ctx->r17, ctx->r4);
    // 0x80143560: subu        $s2, $s2, $v1
    ctx->r18 = SUB32(ctx->r18, ctx->r3);
    // 0x80143564: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80143568: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;
L_8014356C:
    // 0x8014356C: lbu         $a0, 0x0($s1)
    ctx->r4 = MEM_BU(ctx->r17, 0X0);
    // 0x80143570: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80143574: jal         0x80142A68
    // 0x80143578: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    func_80142A68(rdram, ctx);
        goto after_2;
    // 0x80143578: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_2:
    // 0x8014357C: beq         $v0, $s5, L_801435D0
    if (ctx->r2 == ctx->r21) {
        // 0x80143580: nop
    
            goto L_801435D0;
    }
    // 0x80143580: nop

    // 0x80143584: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x80143588: addiu       $s2, $s2, -0x1
    ctx->r18 = ADD32(ctx->r18, -0X1);
    // 0x8014358C: slt         $v0, $v0, $s2
    ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r18) ? 1 : 0;
    // 0x80143590: bne         $v0, $zero, L_801435C8
    if (ctx->r2 != 0) {
        // 0x80143594: nop
    
            goto L_801435C8;
    }
    // 0x80143594: nop

L_80143598:
    // 0x80143598: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
    // 0x8014359C: addu        $a1, $s1, $zero
    ctx->r5 = ADD32(ctx->r17, 0);
    // 0x801435A0: jal         0x80141168
    // 0x801435A4: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    func_80141168(rdram, ctx);
        goto after_3;
    // 0x801435A4: addu        $a2, $s2, $zero
    ctx->r6 = ADD32(ctx->r18, 0);
    after_3:
    // 0x801435A8: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x801435AC: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    // 0x801435B0: addu        $v0, $s4, $zero
    ctx->r2 = ADD32(ctx->r20, 0);
    // 0x801435B4: addu        $v1, $v1, $s2
    ctx->r3 = ADD32(ctx->r3, ctx->r18);
    // 0x801435B8: subu        $a0, $a0, $s2
    ctx->r4 = SUB32(ctx->r4, ctx->r18);
    // 0x801435BC: sw          $v1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r3;
    // 0x801435C0: j           L_801435FC
    // 0x801435C4: sw          $a0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r4;
        goto L_801435FC;
    // 0x801435C4: sw          $a0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r4;
L_801435C8:
    // 0x801435C8: bgtz        $s2, L_8014352C
    if (SIGNED(ctx->r18) > 0) {
        // 0x801435CC: nop
    
            goto L_8014352C;
    }
    // 0x801435CC: nop

L_801435D0:
    // 0x801435D0: beq         $s3, $zero, L_801435F8
    if (ctx->r19 == 0) {
        // 0x801435D4: addu        $v0, $s2, $s3
        ctx->r2 = ADD32(ctx->r18, ctx->r19);
            goto L_801435F8;
    }
    // 0x801435D4: addu        $v0, $s2, $s3
    ctx->r2 = ADD32(ctx->r18, ctx->r19);
    // 0x801435D8: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x801435DC: divu        $zero, $v0, $s3
    lo = S32(U32(ctx->r2) / U32(ctx->r19)); hi = S32(U32(ctx->r2) % U32(ctx->r19));
    // 0x801435E0: bne         $s3, $zero, L_801435EC
    if (ctx->r19 != 0) {
        // 0x801435E4: nop
    
            goto L_801435EC;
    }
    // 0x801435E4: nop

    // 0x801435E8: break       7
    do_break(2148808168);
L_801435EC:
    // 0x801435EC: mflo        $v0
    ctx->r2 = lo;
    // 0x801435F0: j           L_801435FC
    // 0x801435F4: subu        $v0, $s4, $v0
    ctx->r2 = SUB32(ctx->r20, ctx->r2);
        goto L_801435FC;
    // 0x801435F4: subu        $v0, $s4, $v0
    ctx->r2 = SUB32(ctx->r20, ctx->r2);
L_801435F8:
    // 0x801435F8: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_801435FC:
    // 0x801435FC: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80143600: lw          $s6, 0x28($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X28);
    // 0x80143604: lw          $s5, 0x24($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X24);
    // 0x80143608: lw          $s4, 0x20($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X20);
    // 0x8014360C: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x80143610: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x80143614: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80143618: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8014361C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80143620: jr          $ra
    // 0x80143624: nop

    return;
    // 0x80143624: nop

;}
RECOMP_FUNC void func_80143640(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143640: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143644: addu        $a2, $a1, $zero
    ctx->r6 = ADD32(ctx->r5, 0);
    // 0x80143648: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x8014364C: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80143650: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80143654: bgez        $v0, L_8014366C
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80143658: sw          $v0, 0x0($a2)
        MEM_W(0X0, ctx->r6) = ctx->r2;
            goto L_8014366C;
    }
    // 0x80143658: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x8014365C: jal         0x80142A68
    // 0x80143660: nop

    func_80142A68(rdram, ctx);
        goto after_0;
    // 0x80143660: nop

    after_0:
    // 0x80143664: j           L_80143680
    // 0x80143668: nop

        goto L_80143680;
    // 0x80143668: nop

L_8014366C:
    // 0x8014366C: lw          $a1, 0x4($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X4);
    // 0x80143670: andi        $v0, $a0, 0xFF
    ctx->r2 = ctx->r4 & 0XFF;
    // 0x80143674: addiu       $v1, $a1, 0x1
    ctx->r3 = ADD32(ctx->r5, 0X1);
    // 0x80143678: sw          $v1, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r3;
    // 0x8014367C: sb          $a0, 0x0($a1)
    MEM_B(0X0, ctx->r5) = ctx->r4;
L_80143680:
    // 0x80143680: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80143684: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80143688: jr          $ra
    // 0x8014368C: nop

    return;
    // 0x8014368C: nop

;}
RECOMP_FUNC void func_80143690(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143690: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143694: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80143698: lui         $a0, 0x8014
    ctx->r4 = S32(0X8014 << 16);
    // 0x8014369C: addiu       $a0, $a0, 0x36B8
    ctx->r4 = ADD32(ctx->r4, 0X36B8);
    // 0x801436A0: jal         0x801436F8
    // 0x801436A4: nop

    func_801436F8(rdram, ctx);
        goto after_0;
    // 0x801436A4: nop

    after_0:
    // 0x801436A8: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x801436AC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801436B0: jr          $ra
    // 0x801436B4: nop

    return;
    // 0x801436B4: nop

;}
RECOMP_FUNC void func_801436B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801436B8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801436BC: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x801436C0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801436C4: jal         0x80142988
    // 0x801436C8: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    func_80142988(rdram, ctx);
        goto after_0;
    // 0x801436C8: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    after_0:
    // 0x801436CC: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x801436D0: slti        $v0, $v0, 0x3
    ctx->r2 = SIGNED(ctx->r2) < 0X3 ? 1 : 0;
    // 0x801436D4: bne         $v0, $zero, L_801436E4
    if (ctx->r2 != 0) {
        // 0x801436D8: nop
    
            goto L_801436E4;
    }
    // 0x801436D8: nop

    // 0x801436DC: jal         0x80142850
    // 0x801436E0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_80142850(rdram, ctx);
        goto after_1;
    // 0x801436E0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_1:
L_801436E4:
    // 0x801436E4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801436E8: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x801436EC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801436F0: jr          $ra
    // 0x801436F4: nop

    return;
    // 0x801436F4: nop

;}
RECOMP_FUNC void func_801436F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801436F8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801436FC: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80143700: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x80143704: lw          $s2, 0x1E78($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X1E78);
    // 0x80143708: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8014370C: addu        $s3, $a0, $zero
    ctx->r19 = ADD32(ctx->r4, 0);
    // 0x80143710: sw          $ra, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r31;
    // 0x80143714: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80143718: beq         $s2, $zero, L_80143768
    if (ctx->r18 == 0) {
        // 0x8014371C: sw          $s0, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r16;
            goto L_80143768;
    }
    // 0x8014371C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
L_80143720:
    // 0x80143720: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
    // 0x80143724: blez        $v0, L_8014375C
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80143728: addu        $s0, $zero, $zero
        ctx->r16 = ADD32(0, 0);
            goto L_8014375C;
    }
    // 0x80143728: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x8014372C: addu        $s1, $s2, $zero
    ctx->r17 = ADD32(ctx->r18, 0);
L_80143730:
    // 0x80143730: lw          $a0, 0x8($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X8);
    // 0x80143734: lw          $v0, 0x10($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X10);
    // 0x80143738: beq         $v0, $zero, L_80143748
    if (ctx->r2 == 0) {
        // 0x8014373C: nop
    
            goto L_80143748;
    }
    // 0x8014373C: nop

    // 0x80143740: jalr        $s3
    // 0x80143744: nop

    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_0;
    // 0x80143744: nop

    after_0:
L_80143748:
    // 0x80143748: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
    // 0x8014374C: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80143750: slt         $v0, $s0, $v0
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80143754: bne         $v0, $zero, L_80143730
    if (ctx->r2 != 0) {
        // 0x80143758: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_80143730;
    }
    // 0x80143758: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
L_8014375C:
    // 0x8014375C: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
    // 0x80143760: bne         $s2, $zero, L_80143720
    if (ctx->r18 != 0) {
        // 0x80143764: nop
    
            goto L_80143720;
    }
    // 0x80143764: nop

L_80143768:
    // 0x80143768: lw          $ra, 0x28($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X28);
    // 0x8014376C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80143770: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80143774: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80143778: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8014377C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80143780: jr          $ra
    // 0x80143784: nop

    return;
    // 0x80143784: nop

;}
RECOMP_FUNC void func_80143788(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143788: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8014378C: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80143790: jal         0x800C421C
    // 0x80143794: nop

    static_0_800C421C(rdram, ctx);
        goto after_0;
    // 0x80143794: nop

    after_0:
    // 0x80143798: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x8014379C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801437A0: jr          $ra
    // 0x801437A4: nop

    return;
    // 0x801437A4: nop

;}
RECOMP_FUNC void func_801437AC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801437AC: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801437B0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801437B4: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x801437B8: lw          $s0, 0x1E78($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X1E78);
    // 0x801437BC: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x801437C0: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x801437C4: addiu       $s1, $s1, 0x1E78
    ctx->r17 = ADD32(ctx->r17, 0X1E78);
    // 0x801437C8: beq         $s0, $zero, L_80143828
    if (ctx->r16 == 0) {
        // 0x801437CC: sw          $ra, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r31;
            goto L_80143828;
    }
    // 0x801437CC: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x801437D0: addiu       $a3, $zero, 0x14
    ctx->r7 = ADD32(0, 0X14);
L_801437D4:
    // 0x801437D4: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x801437D8: addu        $s1, $s0, $zero
    ctx->r17 = ADD32(ctx->r16, 0);
    // 0x801437DC: blez        $v0, L_8014380C
    if (SIGNED(ctx->r2) <= 0) {
        // 0x801437E0: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_8014380C;
    }
    // 0x801437E0: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x801437E4: addu        $a2, $v0, $zero
    ctx->r6 = ADD32(ctx->r2, 0);
    // 0x801437E8: addu        $v1, $s0, $zero
    ctx->r3 = ADD32(ctx->r16, 0);
L_801437EC:
    // 0x801437EC: lw          $a1, 0x8($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X8);
    // 0x801437F0: lw          $v0, 0x10($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X10);
    // 0x801437F4: beq         $v0, $zero, L_80143888
    if (ctx->r2 == 0) {
        // 0x801437F8: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_80143888;
    }
    // 0x801437F8: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x801437FC: slt         $v0, $a0, $a2
    ctx->r2 = SIGNED(ctx->r4) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x80143800: bne         $v0, $zero, L_801437EC
    if (ctx->r2 != 0) {
        // 0x80143804: addiu       $v1, $v1, 0x4
        ctx->r3 = ADD32(ctx->r3, 0X4);
            goto L_801437EC;
    }
    // 0x80143804: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80143808: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
L_8014380C:
    // 0x8014380C: bne         $v0, $a3, L_80143820
    if (ctx->r2 != ctx->r7) {
        // 0x80143810: nop
    
            goto L_80143820;
    }
    // 0x80143810: nop

    // 0x80143814: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x80143818: bne         $s0, $zero, L_801437D4
    if (ctx->r16 != 0) {
        // 0x8014381C: nop
    
            goto L_801437D4;
    }
    // 0x8014381C: nop

L_80143820:
    // 0x80143820: bne         $s0, $zero, L_80143844
    if (ctx->r16 != 0) {
        // 0x80143824: nop
    
            goto L_80143844;
    }
    // 0x80143824: nop

L_80143828:
    // 0x80143828: jal         0x80143A40
    // 0x8014382C: addiu       $a0, $zero, 0x58
    ctx->r4 = ADD32(0, 0X58);
    func_80143A40(rdram, ctx);
        goto after_0;
    // 0x8014382C: addiu       $a0, $zero, 0x58
    ctx->r4 = ADD32(0, 0X58);
    after_0:
    // 0x80143830: addu        $s0, $v0, $zero
    ctx->r16 = ADD32(ctx->r2, 0);
    // 0x80143834: beq         $s0, $zero, L_80143890
    if (ctx->r16 == 0) {
        // 0x80143838: sw          $v0, 0x0($s1)
        MEM_W(0X0, ctx->r17) = ctx->r2;
            goto L_80143890;
    }
    // 0x80143838: sw          $v0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r2;
    // 0x8014383C: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x80143840: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
L_80143844:
    // 0x80143844: jal         0x80143A40
    // 0x80143848: addiu       $a0, $zero, 0x1C
    ctx->r4 = ADD32(0, 0X1C);
    func_80143A40(rdram, ctx);
        goto after_1;
    // 0x80143848: addiu       $a0, $zero, 0x1C
    ctx->r4 = ADD32(0, 0X1C);
    after_1:
    // 0x8014384C: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x80143850: addu        $s1, $v0, $zero
    ctx->r17 = ADD32(ctx->r2, 0);
    // 0x80143854: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80143858: addu        $v1, $v1, $s0
    ctx->r3 = ADD32(ctx->r3, ctx->r16);
    // 0x8014385C: beq         $s1, $zero, L_80143890
    if (ctx->r17 == 0) {
        // 0x80143860: sw          $v0, 0x8($v1)
        MEM_W(0X8, ctx->r3) = ctx->r2;
            goto L_80143890;
    }
    // 0x80143860: sw          $v0, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->r2;
    // 0x80143864: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    // 0x80143868: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x8014386C: jal         0x80141190
    // 0x80143870: addiu       $a2, $zero, 0x1C
    ctx->r6 = ADD32(0, 0X1C);
    func_80141190(rdram, ctx);
        goto after_2;
    // 0x80143870: addiu       $a2, $zero, 0x1C
    ctx->r6 = ADD32(0, 0X1C);
    after_2:
    // 0x80143874: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x80143878: addu        $v0, $s1, $zero
    ctx->r2 = ADD32(ctx->r17, 0);
    // 0x8014387C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80143880: j           L_80143894
    // 0x80143884: sw          $v1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r3;
        goto L_80143894;
    // 0x80143884: sw          $v1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r3;
L_80143888:
    // 0x80143888: j           L_80143894
    // 0x8014388C: addu        $v0, $a1, $zero
    ctx->r2 = ADD32(ctx->r5, 0);
        goto L_80143894;
    // 0x8014388C: addu        $v0, $a1, $zero
    ctx->r2 = ADD32(ctx->r5, 0);
L_80143890:
    // 0x80143890: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80143894:
    // 0x80143894: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x80143898: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8014389C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801438A0: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x801438A4: jr          $ra
    // 0x801438A8: nop

    return;
    // 0x801438A8: nop

;}
RECOMP_FUNC void func_801438B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801438B0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801438B4: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x801438B8: jal         0x800C4244
    // 0x801438BC: nop

    static_0_800C4244(rdram, ctx);
        goto after_0;
    // 0x801438BC: nop

    after_0:
    // 0x801438C0: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x801438C4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801438C8: jr          $ra
    // 0x801438CC: nop

    return;
    // 0x801438CC: nop

;}
RECOMP_FUNC void func_801438D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801438D0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801438D4: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x801438D8: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x801438DC: lw          $s0, 0x1E88($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X1E88);
    // 0x801438E0: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x801438E4: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x801438E8: beq         $s0, $zero, L_80143908
    if (ctx->r16 == 0) {
        // 0x801438EC: sw          $ra, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r31;
            goto L_80143908;
    }
    // 0x801438EC: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
L_801438F0:
    // 0x801438F0: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x801438F4: jalr        $v0
    // 0x801438F8: nop

    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_0;
    // 0x801438F8: nop

    after_0:
    // 0x801438FC: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x80143900: bne         $s0, $zero, L_801438F0
    if (ctx->r16 != 0) {
        // 0x80143904: nop
    
            goto L_801438F0;
    }
    // 0x80143904: nop

L_80143908:
    // 0x80143908: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8014390C: lw          $v0, 0x1E18($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E18);
    // 0x80143910: beql        $v0, $zero, L_80143924
    if (ctx->r2 == 0) {
        // 0x80143914: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_80143924;
    }
    goto skip_0;
    // 0x80143914: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    skip_0:
    // 0x80143918: jalr        $v0
    // 0x8014391C: nop

    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_1;
    // 0x8014391C: nop

    after_1:
    // 0x80143920: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
L_80143924:
    // 0x80143924: jal         0x80144E30
    // 0x80143928: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    func_80144E30(rdram, ctx);
        goto after_2;
    // 0x80143928: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    after_2:
    // 0x8014392C: jal         0x800C4164
    // 0x80143930: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    static_0_800C4164(rdram, ctx);
        goto after_3;
    // 0x80143930: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    after_3:
    // 0x80143934: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x80143938: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8014393C: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80143940: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80143944: jr          $ra
    // 0x80143948: nop

    return;
    // 0x80143948: nop

;}
RECOMP_FUNC void func_80143950(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143950: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80143954: lw          $v1, 0x1ABC($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1ABC);
    // 0x80143958: beq         $v1, $zero, L_801439DC
    if (ctx->r3 == 0) {
        // 0x8014395C: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_801439DC;
    }
    // 0x8014395C: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x80143960: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80143964: beq         $v0, $zero, L_801439DC
    if (ctx->r2 == 0) {
        // 0x80143968: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_801439DC;
    }
    // 0x80143968: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x8014396C: addiu       $t1, $zero, 0x3D
    ctx->r9 = ADD32(0, 0X3D);
    // 0x80143970: addu        $t0, $v1, $zero
    ctx->r8 = ADD32(ctx->r3, 0);
L_80143974:
    // 0x80143974: lw          $a2, 0x0($t0)
    ctx->r6 = MEM_W(ctx->r8, 0X0);
    // 0x80143978: lbu         $a1, 0x0($a2)
    ctx->r5 = MEM_BU(ctx->r6, 0X0);
    // 0x8014397C: j           L_80143998
    // 0x80143980: addu        $a3, $a0, $zero
    ctx->r7 = ADD32(ctx->r4, 0);
        goto L_80143998;
    // 0x80143980: addu        $a3, $a0, $zero
    ctx->r7 = ADD32(ctx->r4, 0);
L_80143984:
    // 0x80143984: beq         $v1, $t1, L_801439B8
    if (ctx->r3 == ctx->r9) {
        // 0x80143988: nop
    
            goto L_801439B8;
    }
    // 0x80143988: nop

    // 0x8014398C: addiu       $a2, $a2, 0x1
    ctx->r6 = ADD32(ctx->r6, 0X1);
    // 0x80143990: lbu         $a1, 0x0($a2)
    ctx->r5 = MEM_BU(ctx->r6, 0X0);
    // 0x80143994: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
L_80143998:
    // 0x80143998: beq         $a1, $zero, L_801439B8
    if (ctx->r5 == 0) {
        // 0x8014399C: nop
    
            goto L_801439B8;
    }
    // 0x8014399C: nop

    // 0x801439A0: lbu         $v1, 0x0($a3)
    ctx->r3 = MEM_BU(ctx->r7, 0X0);
    // 0x801439A4: andi        $v0, $v1, 0xFF
    ctx->r2 = ctx->r3 & 0XFF;
    // 0x801439A8: beq         $v0, $zero, L_801439B8
    if (ctx->r2 == 0) {
        // 0x801439AC: nop
    
            goto L_801439B8;
    }
    // 0x801439AC: nop

    // 0x801439B0: beq         $a1, $v0, L_80143984
    if (ctx->r5 == ctx->r2) {
        // 0x801439B4: nop
    
            goto L_80143984;
    }
    // 0x801439B4: nop

L_801439B8:
    // 0x801439B8: lbu         $v0, 0x0($a2)
    ctx->r2 = MEM_BU(ctx->r6, 0X0);
    // 0x801439BC: bne         $v0, $t1, L_801439D0
    if (ctx->r2 != ctx->r9) {
        // 0x801439C0: addiu       $t0, $t0, 0x4
        ctx->r8 = ADD32(ctx->r8, 0X4);
            goto L_801439D0;
    }
    // 0x801439C0: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x801439C4: lbu         $v0, 0x0($a3)
    ctx->r2 = MEM_BU(ctx->r7, 0X0);
    // 0x801439C8: beq         $v0, $zero, L_801439DC
    if (ctx->r2 == 0) {
        // 0x801439CC: addiu       $v0, $a2, 0x1
        ctx->r2 = ADD32(ctx->r6, 0X1);
            goto L_801439DC;
    }
    // 0x801439CC: addiu       $v0, $a2, 0x1
    ctx->r2 = ADD32(ctx->r6, 0X1);
L_801439D0:
    // 0x801439D0: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x801439D4: bne         $v0, $zero, L_80143974
    if (ctx->r2 != 0) {
        // 0x801439D8: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_80143974;
    }
    // 0x801439D8: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_801439DC:
    // 0x801439DC: jr          $ra
    // 0x801439E0: nop

    return;
    // 0x801439E0: nop

;}
RECOMP_FUNC void func_801439E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801439E8: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801439EC: lw          $v1, 0x1E90($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1E90);
    // 0x801439F0: beq         $v1, $zero, L_80143A08
    if (ctx->r3 == 0) {
        // 0x801439F4: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_80143A08;
    }
    // 0x801439F4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
L_801439F8:
    // 0x801439F8: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x801439FC: lw          $v1, 0x4($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X4);
    // 0x80143A00: bne         $v1, $zero, L_801439F8
    if (ctx->r3 != 0) {
        // 0x80143A04: addu        $a0, $a0, $v0
        ctx->r4 = ADD32(ctx->r4, ctx->r2);
            goto L_801439F8;
    }
    // 0x80143A04: addu        $a0, $a0, $v0
    ctx->r4 = ADD32(ctx->r4, ctx->r2);
L_80143A08:
    // 0x80143A08: jr          $ra
    // 0x80143A0C: addu        $v0, $a0, $zero
    ctx->r2 = ADD32(ctx->r4, 0);
    return;
    // 0x80143A0C: addu        $v0, $a0, $zero
    ctx->r2 = ADD32(ctx->r4, 0);
;}
RECOMP_FUNC void func_80143A10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143A10: lui         $v1, 0x807F
    ctx->r3 = S32(0X807F << 16);
    // 0x80143A14: ori         $v1, $v1, 0xBFF8
    ctx->r3 = ctx->r3 | 0XBFF8;
    // 0x80143A18: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x80143A1C: addiu       $v0, $v0, 0x6850
    ctx->r2 = ADD32(ctx->r2, 0X6850);
    // 0x80143A20: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80143A24: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143A28: sw          $v0, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r2;
    // 0x80143A2C: sw          $v1, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r3;
    // 0x80143A30: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x80143A34: sw          $zero, 0x6854($at)
    MEM_W(0X6854, ctx->r1) = 0;
    // 0x80143A38: jr          $ra
    // 0x80143A3C: nop

    return;
    // 0x80143A3C: nop

;}
RECOMP_FUNC void func_80143A40_real(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143A40: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x80143A44: lw          $a3, 0x1E90($a3)
    ctx->r7 = MEM_W(ctx->r7, 0X1E90);
    // 0x80143A48: beq         $a0, $zero, L_80143A88
    if (ctx->r4 == 0) {
        // 0x80143A4C: addiu       $a0, $a0, 0x7
        ctx->r4 = ADD32(ctx->r4, 0X7);
            goto L_80143A88;
    }
    // 0x80143A4C: addiu       $a0, $a0, 0x7
    ctx->r4 = ADD32(ctx->r4, 0X7);
    // 0x80143A50: srl         $a0, $a0, 3
    ctx->r4 = S32(U32(ctx->r4) >> 3);
    // 0x80143A54: sll         $a0, $a0, 3
    ctx->r4 = S32(ctx->r4 << 3);
    // 0x80143A58: addiu       $v0, $zero, 0x8
    ctx->r2 = ADD32(0, 0X8);
    // 0x80143A5C: beq         $a3, $zero, L_80143A88
    if (ctx->r7 == 0) {
        // 0x80143A60: movz        $a0, $v0, $a0
        if (ctx->r4 == 0) ctx->r4 = ctx->r2;
            goto L_80143A88;
    }
    // 0x80143A60: movz        $a0, $v0, $a0
    if (ctx->r4 == 0) ctx->r4 = ctx->r2;
L_80143A64:
    // 0x80143A64: lw          $v0, 0x0($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X0);
    // 0x80143A68: sltu        $v0, $v0, $a0
    ctx->r2 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x80143A6C: beq         $v0, $zero, L_80143A80
    if (ctx->r2 == 0) {
        // 0x80143A70: nop
    
            goto L_80143A80;
    }
    // 0x80143A70: nop

    // 0x80143A74: lw          $a3, 0x4($a3)
    ctx->r7 = MEM_W(ctx->r7, 0X4);
    // 0x80143A78: bne         $a3, $zero, L_80143A64
    if (ctx->r7 != 0) {
        // 0x80143A7C: nop
    
            goto L_80143A64;
    }
    // 0x80143A7C: nop

L_80143A80:
    // 0x80143A80: bne         $a3, $zero, L_80143A90
    if (ctx->r7 != 0) {
        // 0x80143A84: nop
    
            goto L_80143A90;
    }
    // 0x80143A84: nop

L_80143A88:
    // 0x80143A88: j           L_80143B9C
    // 0x80143A8C: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
        goto L_80143B9C;
    // 0x80143A8C: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80143A90:
    // 0x80143A90: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80143A94: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80143A98: lw          $a1, 0x0($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X0);
    // 0x80143A9C: beq         $v0, $zero, L_80143AC4
    if (ctx->r2 == 0) {
        // 0x80143AA0: addu        $v1, $zero, $zero
        ctx->r3 = ADD32(0, 0);
            goto L_80143AC4;
    }
    // 0x80143AA0: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
L_80143AA4:
    // 0x80143AA4: beq         $v0, $a3, L_80143ABC
    if (ctx->r2 == ctx->r7) {
        // 0x80143AA8: nop
    
            goto L_80143ABC;
    }
    // 0x80143AA8: nop

    // 0x80143AAC: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
    // 0x80143AB0: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    // 0x80143AB4: bne         $v0, $zero, L_80143AA4
    if (ctx->r2 != 0) {
        // 0x80143AB8: nop
    
            goto L_80143AA4;
    }
    // 0x80143AB8: nop

L_80143ABC:
    // 0x80143ABC: bne         $v0, $zero, L_80143AD4
    if (ctx->r2 != 0) {
        // 0x80143AC0: nop
    
            goto L_80143AD4;
    }
    // 0x80143AC0: nop

L_80143AC4:
    // 0x80143AC4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143AC8: sw          $zero, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = 0;
    // 0x80143ACC: j           L_80143AF8
    // 0x80143AD0: subu        $t0, $a1, $a0
    ctx->r8 = SUB32(ctx->r5, ctx->r4);
        goto L_80143AF8;
    // 0x80143AD0: subu        $t0, $a1, $a0
    ctx->r8 = SUB32(ctx->r5, ctx->r4);
L_80143AD4:
    // 0x80143AD4: bne         $v1, $zero, L_80143AF0
    if (ctx->r3 != 0) {
        // 0x80143AD8: subu        $t0, $a1, $a0
        ctx->r8 = SUB32(ctx->r5, ctx->r4);
            goto L_80143AF0;
    }
    // 0x80143AD8: subu        $t0, $a1, $a0
    ctx->r8 = SUB32(ctx->r5, ctx->r4);
    // 0x80143ADC: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x80143AE0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143AE4: sw          $v0, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r2;
    // 0x80143AE8: j           L_80143AF8
    // 0x80143AEC: nop

        goto L_80143AF8;
    // 0x80143AEC: nop

L_80143AF0:
    // 0x80143AF0: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x80143AF4: sw          $v0, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r2;
L_80143AF8:
    // 0x80143AF8: sltiu       $v0, $t0, 0x9
    ctx->r2 = ctx->r8 < 0X9 ? 1 : 0;
    // 0x80143AFC: bne         $v0, $zero, L_80143B94
    if (ctx->r2 != 0) {
        // 0x80143B00: addiu       $v0, $a1, 0x1
        ctx->r2 = ADD32(ctx->r5, 0X1);
            goto L_80143B94;
    }
    // 0x80143B00: addiu       $v0, $a1, 0x1
    ctx->r2 = ADD32(ctx->r5, 0X1);
    // 0x80143B04: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80143B08: lw          $v1, 0x1E90($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1E90);
    // 0x80143B0C: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    // 0x80143B10: addiu       $v0, $a0, 0x8
    ctx->r2 = ADD32(ctx->r4, 0X8);
    // 0x80143B14: addu        $a1, $a3, $v0
    ctx->r5 = ADD32(ctx->r7, ctx->r2);
    // 0x80143B18: addiu       $v0, $t0, -0x8
    ctx->r2 = ADD32(ctx->r8, -0X8);
    // 0x80143B1C: bne         $v1, $zero, L_80143B34
    if (ctx->r3 != 0) {
        // 0x80143B20: sw          $v0, 0x0($a1)
        MEM_W(0X0, ctx->r5) = ctx->r2;
            goto L_80143B34;
    }
    // 0x80143B20: sw          $v0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r2;
    // 0x80143B24: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143B28: sw          $a1, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r5;
    // 0x80143B2C: j           L_80143B90
    // 0x80143B30: sw          $zero, 0x4($a1)
    MEM_W(0X4, ctx->r5) = 0;
        goto L_80143B90;
    // 0x80143B30: sw          $zero, 0x4($a1)
    MEM_W(0X4, ctx->r5) = 0;
L_80143B34:
    // 0x80143B34: addu        $t0, $v0, $zero
    ctx->r8 = ADD32(ctx->r2, 0);
L_80143B38:
    // 0x80143B38: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80143B3C: sltu        $v0, $v0, $t0
    ctx->r2 = ctx->r2 < ctx->r8 ? 1 : 0;
    // 0x80143B40: beq         $v0, $zero, L_80143B58
    if (ctx->r2 == 0) {
        // 0x80143B44: nop
    
            goto L_80143B58;
    }
    // 0x80143B44: nop

    // 0x80143B48: addu        $a2, $v1, $zero
    ctx->r6 = ADD32(ctx->r3, 0);
    // 0x80143B4C: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x80143B50: bne         $v1, $zero, L_80143B38
    if (ctx->r3 != 0) {
        // 0x80143B54: nop
    
            goto L_80143B38;
    }
    // 0x80143B54: nop

L_80143B58:
    // 0x80143B58: bne         $v1, $zero, L_80143B6C
    if (ctx->r3 != 0) {
        // 0x80143B5C: nop
    
            goto L_80143B6C;
    }
    // 0x80143B5C: nop

    // 0x80143B60: sw          $a1, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r5;
    // 0x80143B64: j           L_80143B90
    // 0x80143B68: sw          $zero, 0x4($a1)
    MEM_W(0X4, ctx->r5) = 0;
        goto L_80143B90;
    // 0x80143B68: sw          $zero, 0x4($a1)
    MEM_W(0X4, ctx->r5) = 0;
L_80143B6C:
    // 0x80143B6C: bnel        $a2, $zero, L_80143B8C
    if (ctx->r6 != 0) {
        // 0x80143B70: sw          $v1, 0x4($a1)
        MEM_W(0X4, ctx->r5) = ctx->r3;
            goto L_80143B8C;
    }
    goto skip_0;
    // 0x80143B70: sw          $v1, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r3;
    skip_0:
    // 0x80143B74: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80143B78: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80143B7C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143B80: sw          $a1, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r5;
    // 0x80143B84: j           L_80143B90
    // 0x80143B88: sw          $v0, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r2;
        goto L_80143B90;
    // 0x80143B88: sw          $v0, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r2;
L_80143B8C:
    // 0x80143B8C: sw          $a1, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r5;
L_80143B90:
    // 0x80143B90: addiu       $v0, $a0, 0x1
    ctx->r2 = ADD32(ctx->r4, 0X1);
L_80143B94:
    // 0x80143B94: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80143B98: addiu       $v0, $a3, 0x8
    ctx->r2 = ADD32(ctx->r7, 0X8);
L_80143B9C:
    // 0x80143B9C: jr          $ra
    // 0x80143BA0: nop

    return;
    // 0x80143BA0: nop

;}
RECOMP_FUNC void func_80143BA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143BA8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143BAC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80143BB0: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80143BB4: mflo        $s0
    ctx->r16 = lo;
    // 0x80143BB8: jal         0x80143A40
    // 0x80143BBC: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_80143A40(rdram, ctx);
        goto after_0;
    // 0x80143BBC: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_0:
    // 0x80143BC0: addu        $a1, $v0, $zero
    ctx->r5 = ADD32(ctx->r2, 0);
    // 0x80143BC4: bne         $a1, $zero, L_80143BD4
    if (ctx->r5 != 0) {
        // 0x80143BC8: addu        $a0, $a1, $zero
        ctx->r4 = ADD32(ctx->r5, 0);
            goto L_80143BD4;
    }
    // 0x80143BC8: addu        $a0, $a1, $zero
    ctx->r4 = ADD32(ctx->r5, 0);
    // 0x80143BCC: j           L_80143BFC
    // 0x80143BD0: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
        goto L_80143BFC;
    // 0x80143BD0: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80143BD4:
    // 0x80143BD4: addiu       $v1, $s0, 0x3
    ctx->r3 = ADD32(ctx->r16, 0X3);
    // 0x80143BD8: addiu       $v0, $zero, -0x4
    ctx->r2 = ADD32(0, -0X4);
    // 0x80143BDC: and         $s0, $v1, $v0
    ctx->r16 = ctx->r3 & ctx->r2;
    // 0x80143BE0: beq         $s0, $zero, L_80143BFC
    if (ctx->r16 == 0) {
        // 0x80143BE4: addu        $v0, $a1, $zero
        ctx->r2 = ADD32(ctx->r5, 0);
            goto L_80143BFC;
    }
    // 0x80143BE4: addu        $v0, $a1, $zero
    ctx->r2 = ADD32(ctx->r5, 0);
L_80143BE8:
    // 0x80143BE8: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
    // 0x80143BEC: addiu       $s0, $s0, -0x4
    ctx->r16 = ADD32(ctx->r16, -0X4);
    // 0x80143BF0: bne         $s0, $zero, L_80143BE8
    if (ctx->r16 != 0) {
        // 0x80143BF4: addiu       $a0, $a0, 0x4
        ctx->r4 = ADD32(ctx->r4, 0X4);
            goto L_80143BE8;
    }
    // 0x80143BF4: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80143BF8: addu        $v0, $a1, $zero
    ctx->r2 = ADD32(ctx->r5, 0);
L_80143BFC:
    // 0x80143BFC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80143C00: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80143C04: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80143C08: jr          $ra
    // 0x80143C0C: nop

    return;
    // 0x80143C0C: nop

;}
RECOMP_FUNC void func_80143C10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143C10: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80143C14: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80143C18: addu        $s3, $a0, $zero
    ctx->r19 = ADD32(ctx->r4, 0);
    // 0x80143C1C: addu        $a0, $a1, $zero
    ctx->r4 = ADD32(ctx->r5, 0);
    // 0x80143C20: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80143C24: addiu       $s2, $s3, -0x8
    ctx->r18 = ADD32(ctx->r19, -0X8);
    // 0x80143C28: addiu       $a2, $a0, 0x7
    ctx->r6 = ADD32(ctx->r4, 0X7);
    // 0x80143C2C: sw          $ra, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r31;
    // 0x80143C30: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80143C34: bne         $s3, $zero, L_80143C4C
    if (ctx->r19 != 0) {
        // 0x80143C38: sw          $s0, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r16;
            goto L_80143C4C;
    }
    // 0x80143C38: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80143C3C: jal         0x80143A40
    // 0x80143C40: nop

    func_80143A40(rdram, ctx);
        goto after_0;
    // 0x80143C40: nop

    after_0:
    // 0x80143C44: j           L_80143E54
    // 0x80143C48: nop

        goto L_80143E54;
    // 0x80143C48: nop

L_80143C4C:
    // 0x80143C4C: bne         $a0, $zero, L_80143C64
    if (ctx->r4 != 0) {
        // 0x80143C50: sra         $a2, $a2, 3
        ctx->r6 = S32(SIGNED(ctx->r6) >> 3);
            goto L_80143C64;
    }
    // 0x80143C50: sra         $a2, $a2, 3
    ctx->r6 = S32(SIGNED(ctx->r6) >> 3);
    // 0x80143C54: jal         0x80143E74
    // 0x80143C58: addu        $a0, $s3, $zero
    ctx->r4 = ADD32(ctx->r19, 0);
    func_80143E74(rdram, ctx);
        goto after_1;
    // 0x80143C58: addu        $a0, $s3, $zero
    ctx->r4 = ADD32(ctx->r19, 0);
    after_1:
    // 0x80143C5C: j           L_80143E54
    // 0x80143C60: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
        goto L_80143E54;
    // 0x80143C60: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80143C64:
    // 0x80143C64: sll         $a2, $a2, 3
    ctx->r6 = S32(ctx->r6 << 3);
    // 0x80143C68: lw          $s0, -0x8($s3)
    ctx->r16 = MEM_W(ctx->r19, -0X8);
    // 0x80143C6C: addiu       $v0, $zero, 0x8
    ctx->r2 = ADD32(0, 0X8);
    // 0x80143C70: slt         $v1, $zero, $a2
    ctx->r3 = SIGNED(0) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x80143C74: movz        $a2, $v0, $v1
    if (ctx->r3 == 0) ctx->r6 = ctx->r2;
    // 0x80143C78: andi        $v0, $s0, 0x1
    ctx->r2 = ctx->r16 & 0X1;
    // 0x80143C7C: beq         $v0, $zero, L_80143E50
    if (ctx->r2 == 0) {
        // 0x80143C80: addiu       $s0, $s0, -0x1
        ctx->r16 = ADD32(ctx->r16, -0X1);
            goto L_80143E50;
    }
    // 0x80143C80: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80143C84: beq         $a2, $s0, L_80143CC8
    if (ctx->r6 == ctx->r16) {
        // 0x80143C88: slt         $v0, $a2, $s0
        ctx->r2 = SIGNED(ctx->r6) < SIGNED(ctx->r16) ? 1 : 0;
            goto L_80143CC8;
    }
    // 0x80143C88: slt         $v0, $a2, $s0
    ctx->r2 = SIGNED(ctx->r6) < SIGNED(ctx->r16) ? 1 : 0;
    // 0x80143C8C: beq         $v0, $zero, L_80143CD0
    if (ctx->r2 == 0) {
        // 0x80143C90: subu        $v0, $s0, $a2
        ctx->r2 = SUB32(ctx->r16, ctx->r6);
            goto L_80143CD0;
    }
    // 0x80143C90: subu        $v0, $s0, $a2
    ctx->r2 = SUB32(ctx->r16, ctx->r6);
    // 0x80143C94: slti        $v0, $v0, 0x8
    ctx->r2 = SIGNED(ctx->r2) < 0X8 ? 1 : 0;
    // 0x80143C98: bnel        $v0, $zero, L_80143E54
    if (ctx->r2 != 0) {
        // 0x80143C9C: addu        $v0, $s3, $zero
        ctx->r2 = ADD32(ctx->r19, 0);
            goto L_80143E54;
    }
    goto skip_0;
    // 0x80143C9C: addu        $v0, $s3, $zero
    ctx->r2 = ADD32(ctx->r19, 0);
    skip_0:
    // 0x80143CA0: addiu       $v0, $s0, -0x8
    ctx->r2 = ADD32(ctx->r16, -0X8);
    // 0x80143CA4: subu        $s0, $v0, $a2
    ctx->r16 = SUB32(ctx->r2, ctx->r6);
    // 0x80143CA8: addiu       $v0, $a2, 0x8
    ctx->r2 = ADD32(ctx->r6, 0X8);
    // 0x80143CAC: addu        $s2, $s2, $v0
    ctx->r18 = ADD32(ctx->r18, ctx->r2);
    // 0x80143CB0: addiu       $a0, $s2, 0x8
    ctx->r4 = ADD32(ctx->r18, 0X8);
    // 0x80143CB4: addiu       $v0, $a2, 0x1
    ctx->r2 = ADD32(ctx->r6, 0X1);
    // 0x80143CB8: sw          $v0, -0x8($s3)
    MEM_W(-0X8, ctx->r19) = ctx->r2;
    // 0x80143CBC: addiu       $v0, $s0, 0x1
    ctx->r2 = ADD32(ctx->r16, 0X1);
    // 0x80143CC0: jal         0x80143E74
    // 0x80143CC4: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
    func_80143E74(rdram, ctx);
        goto after_2;
    // 0x80143CC4: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
    after_2:
L_80143CC8:
    // 0x80143CC8: j           L_80143E54
    // 0x80143CCC: addu        $v0, $s3, $zero
    ctx->r2 = ADD32(ctx->r19, 0);
        goto L_80143E54;
    // 0x80143CCC: addu        $v0, $s3, $zero
    ctx->r2 = ADD32(ctx->r19, 0);
L_80143CD0:
    // 0x80143CD0: lui         $v0, 0x807F
    ctx->r2 = S32(0X807F << 16);
    // 0x80143CD4: ori         $v0, $v0, 0xBFF8
    ctx->r2 = ctx->r2 | 0XBFF8;
    // 0x80143CD8: addu        $v1, $s2, $s0
    ctx->r3 = ADD32(ctx->r18, ctx->r16);
    // 0x80143CDC: addiu       $a3, $v1, 0x8
    ctx->r7 = ADD32(ctx->r3, 0X8);
    // 0x80143CE0: sltu        $v0, $a3, $v0
    ctx->r2 = ctx->r7 < ctx->r2 ? 1 : 0;
    // 0x80143CE4: beq         $v0, $zero, L_80143E20
    if (ctx->r2 == 0) {
        // 0x80143CE8: nop
    
            goto L_80143E20;
    }
    // 0x80143CE8: nop

    // 0x80143CEC: lw          $v1, 0x8($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X8);
    // 0x80143CF0: andi        $v0, $v1, 0x1
    ctx->r2 = ctx->r3 & 0X1;
    // 0x80143CF4: bne         $v0, $zero, L_80143E20
    if (ctx->r2 != 0) {
        // 0x80143CF8: addu        $v1, $s0, $v1
        ctx->r3 = ADD32(ctx->r16, ctx->r3);
            goto L_80143E20;
    }
    // 0x80143CF8: addu        $v1, $s0, $v1
    ctx->r3 = ADD32(ctx->r16, ctx->r3);
    // 0x80143CFC: addiu       $v0, $a2, -0x8
    ctx->r2 = ADD32(ctx->r6, -0X8);
    // 0x80143D00: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80143D04: bltz        $v1, L_80143E20
    if (SIGNED(ctx->r3) < 0) {
        // 0x80143D08: nop
    
            goto L_80143E20;
    }
    // 0x80143D08: nop

    // 0x80143D0C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80143D10: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80143D14: beq         $v0, $zero, L_80143D3C
    if (ctx->r2 == 0) {
        // 0x80143D18: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_80143D3C;
    }
    // 0x80143D18: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
L_80143D1C:
    // 0x80143D1C: beq         $v0, $a3, L_80143D34
    if (ctx->r2 == ctx->r7) {
        // 0x80143D20: nop
    
            goto L_80143D34;
    }
    // 0x80143D20: nop

    // 0x80143D24: addu        $a0, $v0, $zero
    ctx->r4 = ADD32(ctx->r2, 0);
    // 0x80143D28: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
    // 0x80143D2C: bne         $v0, $zero, L_80143D1C
    if (ctx->r2 != 0) {
        // 0x80143D30: nop
    
            goto L_80143D1C;
    }
    // 0x80143D30: nop

L_80143D34:
    // 0x80143D34: bne         $v0, $zero, L_80143D4C
    if (ctx->r2 != 0) {
        // 0x80143D38: nop
    
            goto L_80143D4C;
    }
    // 0x80143D38: nop

L_80143D3C:
    // 0x80143D3C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143D40: sw          $zero, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = 0;
    // 0x80143D44: j           L_80143D74
    // 0x80143D48: slti        $v0, $v1, 0x8
    ctx->r2 = SIGNED(ctx->r3) < 0X8 ? 1 : 0;
        goto L_80143D74;
    // 0x80143D48: slti        $v0, $v1, 0x8
    ctx->r2 = SIGNED(ctx->r3) < 0X8 ? 1 : 0;
L_80143D4C:
    // 0x80143D4C: bne         $a0, $zero, L_80143D68
    if (ctx->r4 != 0) {
        // 0x80143D50: nop
    
            goto L_80143D68;
    }
    // 0x80143D50: nop

    // 0x80143D54: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x80143D58: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143D5C: sw          $v0, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r2;
    // 0x80143D60: j           L_80143D74
    // 0x80143D64: slti        $v0, $v1, 0x8
    ctx->r2 = SIGNED(ctx->r3) < 0X8 ? 1 : 0;
        goto L_80143D74;
    // 0x80143D64: slti        $v0, $v1, 0x8
    ctx->r2 = SIGNED(ctx->r3) < 0X8 ? 1 : 0;
L_80143D68:
    // 0x80143D68: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x80143D6C: sw          $v0, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r2;
    // 0x80143D70: slti        $v0, $v1, 0x8
    ctx->r2 = SIGNED(ctx->r3) < 0X8 ? 1 : 0;
L_80143D74:
    // 0x80143D74: beq         $v0, $zero, L_80143D88
    if (ctx->r2 == 0) {
        // 0x80143D78: addu        $v0, $a2, $v1
        ctx->r2 = ADD32(ctx->r6, ctx->r3);
            goto L_80143D88;
    }
    // 0x80143D78: addu        $v0, $a2, $v1
    ctx->r2 = ADD32(ctx->r6, ctx->r3);
    // 0x80143D7C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80143D80: j           L_80143CC8
    // 0x80143D84: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
        goto L_80143CC8;
    // 0x80143D84: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
L_80143D88:
    // 0x80143D88: addiu       $v0, $a2, 0x1
    ctx->r2 = ADD32(ctx->r6, 0X1);
    // 0x80143D8C: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
    // 0x80143D90: addiu       $v0, $a2, 0x8
    ctx->r2 = ADD32(ctx->r6, 0X8);
    // 0x80143D94: addu        $s2, $s2, $v0
    ctx->r18 = ADD32(ctx->r18, ctx->r2);
    // 0x80143D98: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80143D9C: lw          $a0, 0x1E90($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1E90);
    // 0x80143DA0: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80143DA4: addiu       $v0, $v1, -0x8
    ctx->r2 = ADD32(ctx->r3, -0X8);
    // 0x80143DA8: bne         $a0, $zero, L_80143DC0
    if (ctx->r4 != 0) {
        // 0x80143DAC: sw          $v0, 0x0($s2)
        MEM_W(0X0, ctx->r18) = ctx->r2;
            goto L_80143DC0;
    }
    // 0x80143DAC: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
    // 0x80143DB0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143DB4: sw          $s2, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r18;
    // 0x80143DB8: j           L_80143CC8
    // 0x80143DBC: sw          $zero, 0x4($s2)
    MEM_W(0X4, ctx->r18) = 0;
        goto L_80143CC8;
    // 0x80143DBC: sw          $zero, 0x4($s2)
    MEM_W(0X4, ctx->r18) = 0;
L_80143DC0:
    // 0x80143DC0: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
L_80143DC4:
    // 0x80143DC4: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x80143DC8: sltu        $v0, $v0, $v1
    ctx->r2 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x80143DCC: beq         $v0, $zero, L_80143DE4
    if (ctx->r2 == 0) {
        // 0x80143DD0: nop
    
            goto L_80143DE4;
    }
    // 0x80143DD0: nop

    // 0x80143DD4: addu        $a1, $a0, $zero
    ctx->r5 = ADD32(ctx->r4, 0);
    // 0x80143DD8: lw          $a0, 0x4($a1)
    ctx->r4 = MEM_W(ctx->r5, 0X4);
    // 0x80143DDC: bne         $a0, $zero, L_80143DC4
    if (ctx->r4 != 0) {
        // 0x80143DE0: nop
    
            goto L_80143DC4;
    }
    // 0x80143DE0: nop

L_80143DE4:
    // 0x80143DE4: bne         $a0, $zero, L_80143DF8
    if (ctx->r4 != 0) {
        // 0x80143DE8: nop
    
            goto L_80143DF8;
    }
    // 0x80143DE8: nop

    // 0x80143DEC: sw          $s2, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r18;
    // 0x80143DF0: j           L_80143CC8
    // 0x80143DF4: sw          $zero, 0x4($s2)
    MEM_W(0X4, ctx->r18) = 0;
        goto L_80143CC8;
    // 0x80143DF4: sw          $zero, 0x4($s2)
    MEM_W(0X4, ctx->r18) = 0;
L_80143DF8:
    // 0x80143DF8: bnel        $a1, $zero, L_80143E18
    if (ctx->r5 != 0) {
        // 0x80143DFC: sw          $a0, 0x4($s2)
        MEM_W(0X4, ctx->r18) = ctx->r4;
            goto L_80143E18;
    }
    goto skip_1;
    // 0x80143DFC: sw          $a0, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r4;
    skip_1:
    // 0x80143E00: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80143E04: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80143E08: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143E0C: sw          $s2, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r18;
    // 0x80143E10: j           L_80143CC8
    // 0x80143E14: sw          $v0, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r2;
        goto L_80143CC8;
    // 0x80143E14: sw          $v0, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r2;
L_80143E18:
    // 0x80143E18: j           L_80143CC8
    // 0x80143E1C: sw          $s2, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r18;
        goto L_80143CC8;
    // 0x80143E1C: sw          $s2, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r18;
L_80143E20:
    // 0x80143E20: jal         0x80143A40
    // 0x80143E24: nop

    func_80143A40(rdram, ctx);
        goto after_3;
    // 0x80143E24: nop

    after_3:
    // 0x80143E28: addu        $s1, $v0, $zero
    ctx->r17 = ADD32(ctx->r2, 0);
    // 0x80143E2C: beq         $s1, $zero, L_80143E50
    if (ctx->r17 == 0) {
        // 0x80143E30: addu        $a0, $s1, $zero
        ctx->r4 = ADD32(ctx->r17, 0);
            goto L_80143E50;
    }
    // 0x80143E30: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    // 0x80143E34: addiu       $a1, $s2, 0x8
    ctx->r5 = ADD32(ctx->r18, 0X8);
    // 0x80143E38: jal         0x80141168
    // 0x80143E3C: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    func_80141168(rdram, ctx);
        goto after_4;
    // 0x80143E3C: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    after_4:
    // 0x80143E40: jal         0x80143E74
    // 0x80143E44: addu        $a0, $s3, $zero
    ctx->r4 = ADD32(ctx->r19, 0);
    func_80143E74(rdram, ctx);
        goto after_5;
    // 0x80143E44: addu        $a0, $s3, $zero
    ctx->r4 = ADD32(ctx->r19, 0);
    after_5:
    // 0x80143E48: j           L_80143E54
    // 0x80143E4C: addu        $v0, $s1, $zero
    ctx->r2 = ADD32(ctx->r17, 0);
        goto L_80143E54;
    // 0x80143E4C: addu        $v0, $s1, $zero
    ctx->r2 = ADD32(ctx->r17, 0);
L_80143E50:
    // 0x80143E50: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80143E54:
    // 0x80143E54: lw          $ra, 0x28($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X28);
    // 0x80143E58: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80143E5C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80143E60: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80143E64: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80143E68: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80143E6C: jr          $ra
    // 0x80143E70: nop

    return;
    // 0x80143E70: nop

;}
RECOMP_FUNC void func_80143E74_original(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143E74: addiu       $sp, $sp, -0x8
    ctx->r29 = ADD32(ctx->r29, -0X8);
    // 0x80143E78: beq         $a0, $zero, L_801442A8
    if (ctx->r4 == 0) {
        // 0x80143E7C: addiu       $a1, $zero, -0x2
        ctx->r5 = ADD32(0, -0X2);
            goto L_801442A8;
    }
    // 0x80143E7C: addiu       $a1, $zero, -0x2
    ctx->r5 = ADD32(0, -0X2);
    // 0x80143E80: addiu       $a0, $a0, -0x8
    ctx->r4 = ADD32(ctx->r4, -0X8);
    // 0x80143E84: lui         $a2, 0x8023
    ctx->r6 = S32(0X8023 << 16);
    // 0x80143E88: addiu       $a2, $a2, 0x6850
    ctx->r6 = ADD32(ctx->r6, 0X6850);
    // 0x80143E8C: sltu        $v0, $a2, $a0
    ctx->r2 = ctx->r6 < ctx->r4 ? 1 : 0;
    // 0x80143E90: beq         $v0, $zero, L_80143EB8
    if (ctx->r2 == 0) {
        // 0x80143E94: addu        $a3, $zero, $zero
        ctx->r7 = ADD32(0, 0);
            goto L_80143EB8;
    }
    // 0x80143E94: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
L_80143E98:
    // 0x80143E98: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80143E9C: addu        $a3, $a2, $zero
    ctx->r7 = ADD32(ctx->r6, 0);
    // 0x80143EA0: and         $v0, $v0, $a1
    ctx->r2 = ctx->r2 & ctx->r5;
    // 0x80143EA4: addu        $v0, $a3, $v0
    ctx->r2 = ADD32(ctx->r7, ctx->r2);
    // 0x80143EA8: addiu       $a2, $v0, 0x8
    ctx->r6 = ADD32(ctx->r2, 0X8);
    // 0x80143EAC: sltu        $v0, $a2, $a0
    ctx->r2 = ctx->r6 < ctx->r4 ? 1 : 0;
    // 0x80143EB0: bne         $v0, $zero, L_80143E98
    if (ctx->r2 != 0) {
        // 0x80143EB4: nop
    
            goto L_80143E98;
    }
    // 0x80143EB4: nop

L_80143EB8:
    // 0x80143EB8: bne         $a2, $a0, L_801442A8
    if (ctx->r6 != ctx->r4) {
        // 0x80143EBC: nop
    
            goto L_801442A8;
    }
    // 0x80143EBC: nop

    // 0x80143EC0: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x80143EC4: andi        $v0, $v1, 0x1
    ctx->r2 = ctx->r3 & 0X1;
    // 0x80143EC8: beq         $v0, $zero, L_801442A8
    if (ctx->r2 == 0) {
        // 0x80143ECC: and         $v0, $v1, $a1
        ctx->r2 = ctx->r3 & ctx->r5;
            goto L_801442A8;
    }
    // 0x80143ECC: and         $v0, $v1, $a1
    ctx->r2 = ctx->r3 & ctx->r5;
    // 0x80143ED0: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x80143ED4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80143ED8: addu        $a0, $a2, $v0
    ctx->r4 = ADD32(ctx->r6, ctx->r2);
    // 0x80143EDC: lui         $v0, 0x807F
    ctx->r2 = S32(0X807F << 16);
    // 0x80143EE0: ori         $v0, $v0, 0xBFF8
    ctx->r2 = ctx->r2 | 0XBFF8;
    // 0x80143EE4: sltu        $v0, $v0, $a0
    ctx->r2 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x80143EE8: beq         $a3, $zero, L_80144150
    if (ctx->r7 == 0) {
        // 0x80143EEC: movn        $a0, $zero, $v0
        if (ctx->r2 != 0) ctx->r4 = ctx->r0;
            goto L_80144150;
    }
    // 0x80143EEC: movn        $a0, $zero, $v0
    if (ctx->r2 != 0) ctx->r4 = ctx->r0;
    // 0x80143EF0: lw          $v0, 0x0($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X0);
    // 0x80143EF4: andi        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 & 0X1;
    // 0x80143EF8: bne         $v0, $zero, L_80144150
    if (ctx->r2 != 0) {
        // 0x80143EFC: nop
    
            goto L_80144150;
    }
    // 0x80143EFC: nop

    // 0x80143F00: beq         $a0, $zero, L_80144058
    if (ctx->r4 == 0) {
        // 0x80143F04: nop
    
            goto L_80144058;
    }
    // 0x80143F04: nop

    // 0x80143F08: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x80143F0C: andi        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 & 0X1;
    // 0x80143F10: bne         $v0, $zero, L_80144058
    if (ctx->r2 != 0) {
        // 0x80143F14: nop
    
            goto L_80144058;
    }
    // 0x80143F14: nop

    // 0x80143F18: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80143F1C: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80143F20: beq         $v0, $zero, L_80143F48
    if (ctx->r2 == 0) {
        // 0x80143F24: addu        $v1, $zero, $zero
        ctx->r3 = ADD32(0, 0);
            goto L_80143F48;
    }
    // 0x80143F24: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
L_80143F28:
    // 0x80143F28: beq         $v0, $a3, L_80143F40
    if (ctx->r2 == ctx->r7) {
        // 0x80143F2C: nop
    
            goto L_80143F40;
    }
    // 0x80143F2C: nop

    // 0x80143F30: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
    // 0x80143F34: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    // 0x80143F38: bne         $v0, $zero, L_80143F28
    if (ctx->r2 != 0) {
        // 0x80143F3C: nop
    
            goto L_80143F28;
    }
    // 0x80143F3C: nop

L_80143F40:
    // 0x80143F40: bne         $v0, $zero, L_80143F58
    if (ctx->r2 != 0) {
        // 0x80143F44: nop
    
            goto L_80143F58;
    }
    // 0x80143F44: nop

L_80143F48:
    // 0x80143F48: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143F4C: sw          $zero, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = 0;
    // 0x80143F50: j           L_80143F7C
    // 0x80143F54: nop

        goto L_80143F7C;
    // 0x80143F54: nop

L_80143F58:
    // 0x80143F58: bne         $v1, $zero, L_80143F74
    if (ctx->r3 != 0) {
        // 0x80143F5C: nop
    
            goto L_80143F74;
    }
    // 0x80143F5C: nop

    // 0x80143F60: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x80143F64: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143F68: sw          $v0, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r2;
    // 0x80143F6C: j           L_80143F7C
    // 0x80143F70: nop

        goto L_80143F7C;
    // 0x80143F70: nop

L_80143F74:
    // 0x80143F74: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x80143F78: sw          $v0, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r2;
L_80143F7C:
    // 0x80143F7C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80143F80: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80143F84: beq         $v0, $zero, L_80143FAC
    if (ctx->r2 == 0) {
        // 0x80143F88: addu        $v1, $zero, $zero
        ctx->r3 = ADD32(0, 0);
            goto L_80143FAC;
    }
    // 0x80143F88: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
L_80143F8C:
    // 0x80143F8C: beq         $v0, $a0, L_80143FA4
    if (ctx->r2 == ctx->r4) {
        // 0x80143F90: nop
    
            goto L_80143FA4;
    }
    // 0x80143F90: nop

    // 0x80143F94: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
    // 0x80143F98: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    // 0x80143F9C: bne         $v0, $zero, L_80143F8C
    if (ctx->r2 != 0) {
        // 0x80143FA0: nop
    
            goto L_80143F8C;
    }
    // 0x80143FA0: nop

L_80143FA4:
    // 0x80143FA4: bne         $v0, $zero, L_80143FBC
    if (ctx->r2 != 0) {
        // 0x80143FA8: nop
    
            goto L_80143FBC;
    }
    // 0x80143FA8: nop

L_80143FAC:
    // 0x80143FAC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143FB0: sw          $zero, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = 0;
    // 0x80143FB4: j           L_80143FE0
    // 0x80143FB8: nop

        goto L_80143FE0;
    // 0x80143FB8: nop

L_80143FBC:
    // 0x80143FBC: bne         $v1, $zero, L_80143FD8
    if (ctx->r3 != 0) {
        // 0x80143FC0: nop
    
            goto L_80143FD8;
    }
    // 0x80143FC0: nop

    // 0x80143FC4: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
    // 0x80143FC8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143FCC: sw          $v0, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r2;
    // 0x80143FD0: j           L_80143FE0
    // 0x80143FD4: nop

        goto L_80143FE0;
    // 0x80143FD4: nop

L_80143FD8:
    // 0x80143FD8: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
    // 0x80143FDC: sw          $v0, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r2;
L_80143FE0:
    // 0x80143FE0: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80143FE4: lw          $a1, 0x1E90($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X1E90);
    // 0x80143FE8: lw          $v0, 0x0($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X0);
    // 0x80143FEC: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x80143FF0: lw          $a0, 0x0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X0);
    // 0x80143FF4: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    // 0x80143FF8: addiu       $v0, $v0, 0x10
    ctx->r2 = ADD32(ctx->r2, 0X10);
    // 0x80143FFC: addu        $v1, $v1, $a0
    ctx->r3 = ADD32(ctx->r3, ctx->r4);
    // 0x80144000: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x80144004: beq         $a1, $zero, L_801440E0
    if (ctx->r5 == 0) {
        // 0x80144008: sw          $v0, 0x0($a3)
        MEM_W(0X0, ctx->r7) = ctx->r2;
            goto L_801440E0;
    }
    // 0x80144008: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x8014400C: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
L_80144010:
    // 0x80144010: lw          $v0, 0x0($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X0);
    // 0x80144014: sltu        $v0, $v0, $v1
    ctx->r2 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x80144018: beq         $v0, $zero, L_80144030
    if (ctx->r2 == 0) {
        // 0x8014401C: nop
    
            goto L_80144030;
    }
    // 0x8014401C: nop

    // 0x80144020: addu        $a2, $a1, $zero
    ctx->r6 = ADD32(ctx->r5, 0);
    // 0x80144024: lw          $a1, 0x4($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X4);
    // 0x80144028: bne         $a1, $zero, L_80144010
    if (ctx->r5 != 0) {
        // 0x8014402C: nop
    
            goto L_80144010;
    }
    // 0x8014402C: nop

L_80144030:
    // 0x80144030: bne         $a1, $zero, L_80144044
    if (ctx->r5 != 0) {
        // 0x80144034: nop
    
            goto L_80144044;
    }
    // 0x80144034: nop

    // 0x80144038: sw          $a3, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r7;
    // 0x8014403C: j           L_801442A8
    // 0x80144040: sw          $zero, 0x4($a3)
    MEM_W(0X4, ctx->r7) = 0;
        goto L_801442A8;
    // 0x80144040: sw          $zero, 0x4($a3)
    MEM_W(0X4, ctx->r7) = 0;
L_80144044:
    // 0x80144044: beq         $a2, $zero, L_80144130
    if (ctx->r6 == 0) {
        // 0x80144048: nop
    
            goto L_80144130;
    }
    // 0x80144048: nop

    // 0x8014404C: sw          $a1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r5;
    // 0x80144050: j           L_801442A8
    // 0x80144054: sw          $a3, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r7;
        goto L_801442A8;
    // 0x80144054: sw          $a3, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r7;
L_80144058:
    // 0x80144058: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8014405C: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80144060: beq         $v0, $zero, L_80144088
    if (ctx->r2 == 0) {
        // 0x80144064: addu        $v1, $zero, $zero
        ctx->r3 = ADD32(0, 0);
            goto L_80144088;
    }
    // 0x80144064: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
L_80144068:
    // 0x80144068: beq         $v0, $a3, L_80144080
    if (ctx->r2 == ctx->r7) {
        // 0x8014406C: nop
    
            goto L_80144080;
    }
    // 0x8014406C: nop

    // 0x80144070: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
    // 0x80144074: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    // 0x80144078: bne         $v0, $zero, L_80144068
    if (ctx->r2 != 0) {
        // 0x8014407C: nop
    
            goto L_80144068;
    }
    // 0x8014407C: nop

L_80144080:
    // 0x80144080: bne         $v0, $zero, L_80144098
    if (ctx->r2 != 0) {
        // 0x80144084: nop
    
            goto L_80144098;
    }
    // 0x80144084: nop

L_80144088:
    // 0x80144088: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8014408C: sw          $zero, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = 0;
    // 0x80144090: j           L_801440BC
    // 0x80144094: nop

        goto L_801440BC;
    // 0x80144094: nop

L_80144098:
    // 0x80144098: bne         $v1, $zero, L_801440B4
    if (ctx->r3 != 0) {
        // 0x8014409C: nop
    
            goto L_801440B4;
    }
    // 0x8014409C: nop

    // 0x801440A0: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x801440A4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801440A8: sw          $v0, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r2;
    // 0x801440AC: j           L_801440BC
    // 0x801440B0: nop

        goto L_801440BC;
    // 0x801440B0: nop

L_801440B4:
    // 0x801440B4: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x801440B8: sw          $v0, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r2;
L_801440BC:
    // 0x801440BC: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x801440C0: lw          $a0, 0x1E90($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1E90);
    // 0x801440C4: lw          $v0, 0x0($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X0);
    // 0x801440C8: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x801440CC: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x801440D0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801440D4: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x801440D8: bne         $a0, $zero, L_801440F0
    if (ctx->r4 != 0) {
        // 0x801440DC: sw          $v0, 0x0($a3)
        MEM_W(0X0, ctx->r7) = ctx->r2;
            goto L_801440F0;
    }
    // 0x801440DC: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
L_801440E0:
    // 0x801440E0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801440E4: sw          $a3, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r7;
    // 0x801440E8: j           L_801442A8
    // 0x801440EC: sw          $zero, 0x4($a3)
    MEM_W(0X4, ctx->r7) = 0;
        goto L_801442A8;
    // 0x801440EC: sw          $zero, 0x4($a3)
    MEM_W(0X4, ctx->r7) = 0;
L_801440F0:
    // 0x801440F0: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
L_801440F4:
    // 0x801440F4: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x801440F8: sltu        $v0, $v0, $v1
    ctx->r2 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x801440FC: beq         $v0, $zero, L_80144114
    if (ctx->r2 == 0) {
        // 0x80144100: nop
    
            goto L_80144114;
    }
    // 0x80144100: nop

    // 0x80144104: addu        $a1, $a0, $zero
    ctx->r5 = ADD32(ctx->r4, 0);
    // 0x80144108: lw          $a0, 0x4($a1)
    ctx->r4 = MEM_W(ctx->r5, 0X4);
    // 0x8014410C: bne         $a0, $zero, L_801440F4
    if (ctx->r4 != 0) {
        // 0x80144110: nop
    
            goto L_801440F4;
    }
    // 0x80144110: nop

L_80144114:
    // 0x80144114: bne         $a0, $zero, L_80144128
    if (ctx->r4 != 0) {
        // 0x80144118: nop
    
            goto L_80144128;
    }
    // 0x80144118: nop

    // 0x8014411C: sw          $a3, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r7;
    // 0x80144120: j           L_801442A8
    // 0x80144124: sw          $zero, 0x4($a3)
    MEM_W(0X4, ctx->r7) = 0;
        goto L_801442A8;
    // 0x80144124: sw          $zero, 0x4($a3)
    MEM_W(0X4, ctx->r7) = 0;
L_80144128:
    // 0x80144128: bnel        $a1, $zero, L_80144148
    if (ctx->r5 != 0) {
        // 0x8014412C: sw          $a0, 0x4($a3)
        MEM_W(0X4, ctx->r7) = ctx->r4;
            goto L_80144148;
    }
    goto skip_0;
    // 0x8014412C: sw          $a0, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r4;
    skip_0:
L_80144130:
    // 0x80144130: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80144134: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80144138: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8014413C: sw          $a3, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r7;
    // 0x80144140: j           L_801442A8
    // 0x80144144: sw          $v0, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r2;
        goto L_801442A8;
    // 0x80144144: sw          $v0, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r2;
L_80144148:
    // 0x80144148: j           L_801442A8
    // 0x8014414C: sw          $a3, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r7;
        goto L_801442A8;
    // 0x8014414C: sw          $a3, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r7;
L_80144150:
    // 0x80144150: beq         $a0, $zero, L_8014422C
    if (ctx->r4 == 0) {
        // 0x80144154: nop
    
            goto L_8014422C;
    }
    // 0x80144154: nop

    // 0x80144158: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x8014415C: andi        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 & 0X1;
    // 0x80144160: bne         $v0, $zero, L_8014422C
    if (ctx->r2 != 0) {
        // 0x80144164: nop
    
            goto L_8014422C;
    }
    // 0x80144164: nop

    // 0x80144168: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8014416C: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80144170: beq         $v0, $zero, L_80144198
    if (ctx->r2 == 0) {
        // 0x80144174: addu        $v1, $zero, $zero
        ctx->r3 = ADD32(0, 0);
            goto L_80144198;
    }
    // 0x80144174: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
L_80144178:
    // 0x80144178: beq         $v0, $a0, L_80144190
    if (ctx->r2 == ctx->r4) {
        // 0x8014417C: nop
    
            goto L_80144190;
    }
    // 0x8014417C: nop

    // 0x80144180: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
    // 0x80144184: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    // 0x80144188: bne         $v0, $zero, L_80144178
    if (ctx->r2 != 0) {
        // 0x8014418C: nop
    
            goto L_80144178;
    }
    // 0x8014418C: nop

L_80144190:
    // 0x80144190: bne         $v0, $zero, L_801441A8
    if (ctx->r2 != 0) {
        // 0x80144194: nop
    
            goto L_801441A8;
    }
    // 0x80144194: nop

L_80144198:
    // 0x80144198: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8014419C: sw          $zero, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = 0;
    // 0x801441A0: j           L_801441CC
    // 0x801441A4: nop

        goto L_801441CC;
    // 0x801441A4: nop

L_801441A8:
    // 0x801441A8: bne         $v1, $zero, L_801441C4
    if (ctx->r3 != 0) {
        // 0x801441AC: nop
    
            goto L_801441C4;
    }
    // 0x801441AC: nop

    // 0x801441B0: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
    // 0x801441B4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801441B8: sw          $v0, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r2;
    // 0x801441BC: j           L_801441CC
    // 0x801441C0: nop

        goto L_801441CC;
    // 0x801441C0: nop

L_801441C4:
    // 0x801441C4: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
    // 0x801441C8: sw          $v0, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r2;
L_801441CC:
    // 0x801441CC: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x801441D0: lw          $a1, 0x1E90($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X1E90);
    // 0x801441D4: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x801441D8: lw          $v1, 0x0($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X0);
    // 0x801441DC: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x801441E0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801441E4: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x801441E8: beq         $a1, $zero, L_8014423C
    if (ctx->r5 == 0) {
        // 0x801441EC: sw          $v0, 0x0($a2)
        MEM_W(0X0, ctx->r6) = ctx->r2;
            goto L_8014423C;
    }
    // 0x801441EC: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x801441F0: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
L_801441F4:
    // 0x801441F4: lw          $v0, 0x0($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X0);
    // 0x801441F8: sltu        $v0, $v0, $v1
    ctx->r2 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x801441FC: beq         $v0, $zero, L_80144214
    if (ctx->r2 == 0) {
        // 0x80144200: nop
    
            goto L_80144214;
    }
    // 0x80144200: nop

    // 0x80144204: addu        $a0, $a1, $zero
    ctx->r4 = ADD32(ctx->r5, 0);
    // 0x80144208: lw          $a1, 0x4($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X4);
    // 0x8014420C: bne         $a1, $zero, L_801441F4
    if (ctx->r5 != 0) {
        // 0x80144210: nop
    
            goto L_801441F4;
    }
    // 0x80144210: nop

L_80144214:
    // 0x80144214: beql        $a1, $zero, L_8014427C
    if (ctx->r5 == 0) {
        // 0x80144218: sw          $a2, 0x4($a0)
        MEM_W(0X4, ctx->r4) = ctx->r6;
            goto L_8014427C;
    }
    goto skip_1;
    // 0x80144218: sw          $a2, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r6;
    skip_1:
    // 0x8014421C: beq         $a0, $zero, L_8014428C
    if (ctx->r4 == 0) {
        // 0x80144220: nop
    
            goto L_8014428C;
    }
    // 0x80144220: nop

    // 0x80144224: j           L_801442A4
    // 0x80144228: sw          $a1, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r5;
        goto L_801442A4;
    // 0x80144228: sw          $a1, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r5;
L_8014422C:
    // 0x8014422C: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80144230: lw          $v1, 0x1E90($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1E90);
    // 0x80144234: bne         $v1, $zero, L_8014424C
    if (ctx->r3 != 0) {
        // 0x80144238: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_8014424C;
    }
    // 0x80144238: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
L_8014423C:
    // 0x8014423C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80144240: sw          $a2, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r6;
    // 0x80144244: j           L_801442A8
    // 0x80144248: sw          $zero, 0x4($a2)
    MEM_W(0X4, ctx->r6) = 0;
        goto L_801442A8;
    // 0x80144248: sw          $zero, 0x4($a2)
    MEM_W(0X4, ctx->r6) = 0;
L_8014424C:
    // 0x8014424C: lw          $a1, 0x0($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X0);
L_80144250:
    // 0x80144250: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80144254: sltu        $v0, $v0, $a1
    ctx->r2 = ctx->r2 < ctx->r5 ? 1 : 0;
    // 0x80144258: beq         $v0, $zero, L_80144270
    if (ctx->r2 == 0) {
        // 0x8014425C: nop
    
            goto L_80144270;
    }
    // 0x8014425C: nop

    // 0x80144260: addu        $a0, $v1, $zero
    ctx->r4 = ADD32(ctx->r3, 0);
    // 0x80144264: lw          $v1, 0x4($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X4);
    // 0x80144268: bne         $v1, $zero, L_80144250
    if (ctx->r3 != 0) {
        // 0x8014426C: nop
    
            goto L_80144250;
    }
    // 0x8014426C: nop

L_80144270:
    // 0x80144270: bne         $v1, $zero, L_80144284
    if (ctx->r3 != 0) {
        // 0x80144274: nop
    
            goto L_80144284;
    }
    // 0x80144274: nop

    // 0x80144278: sw          $a2, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r6;
L_8014427C:
    // 0x8014427C: j           L_801442A8
    // 0x80144280: sw          $zero, 0x4($a2)
    MEM_W(0X4, ctx->r6) = 0;
        goto L_801442A8;
    // 0x80144280: sw          $zero, 0x4($a2)
    MEM_W(0X4, ctx->r6) = 0;
L_80144284:
    // 0x80144284: bnel        $a0, $zero, L_801442A4
    if (ctx->r4 != 0) {
        // 0x80144288: sw          $v1, 0x4($a2)
        MEM_W(0X4, ctx->r6) = ctx->r3;
            goto L_801442A4;
    }
    goto skip_2;
    // 0x80144288: sw          $v1, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r3;
    skip_2:
L_8014428C:
    // 0x8014428C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80144290: lw          $v0, 0x1E90($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1E90);
    // 0x80144294: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80144298: sw          $a2, 0x1E90($at)
    MEM_W(0X1E90, ctx->r1) = ctx->r6;
    // 0x8014429C: j           L_801442A8
    // 0x801442A0: sw          $v0, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r2;
        goto L_801442A8;
    // 0x801442A0: sw          $v0, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r2;
L_801442A4:
    // 0x801442A4: sw          $a2, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r6;
L_801442A8:
    // 0x801442A8: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
    // 0x801442AC: jr          $ra
    // 0x801442B0: nop

    return;
    // 0x801442B0: nop

;}
RECOMP_FUNC void func_801442B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801442B4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801442B8: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x801442BC: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x801442C0: lw          $s0, 0x1E90($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X1E90);
    // 0x801442C4: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x801442C8: addiu       $v0, $v0, 0x6850
    ctx->r2 = ADD32(ctx->r2, 0X6850);
    // 0x801442CC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801442D0: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x801442D4: sltu        $v0, $s0, $v0
    ctx->r2 = ctx->r16 < ctx->r2 ? 1 : 0;
    // 0x801442D8: beq         $v0, $zero, L_801442F0
    if (ctx->r2 == 0) {
        // 0x801442DC: sw          $s1, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r17;
            goto L_801442F0;
    }
    // 0x801442DC: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x801442E0: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x801442E4: addiu       $a1, $a1, 0x4C80
    ctx->r5 = ADD32(ctx->r5, 0X4C80);
    // 0x801442E8: j           L_8014430C
    // 0x801442EC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
        goto L_8014430C;
    // 0x801442EC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
L_801442F0:
    // 0x801442F0: lui         $v0, 0x807F
    ctx->r2 = S32(0X807F << 16);
    // 0x801442F4: ori         $v0, $v0, 0xC000
    ctx->r2 = ctx->r2 | 0XC000;
    // 0x801442F8: sltu        $v0, $v0, $s0
    ctx->r2 = ctx->r2 < ctx->r16 ? 1 : 0;
    // 0x801442FC: beq         $v0, $zero, L_80144318
    if (ctx->r2 == 0) {
        // 0x80144300: addiu       $a0, $zero, 0x1
        ctx->r4 = ADD32(0, 0X1);
            goto L_80144318;
    }
    // 0x80144300: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80144304: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80144308: addiu       $a1, $a1, 0x4C94
    ctx->r5 = ADD32(ctx->r5, 0X4C94);
L_8014430C:
    // 0x8014430C: jal         0x80145020
    // 0x80144310: addiu       $a2, $zero, 0x11
    ctx->r6 = ADD32(0, 0X11);
    func_80145020(rdram, ctx);
        goto after_0;
    // 0x80144310: addiu       $a2, $zero, 0x11
    ctx->r6 = ADD32(0, 0X11);
    after_0:
    // 0x80144314: break       1, 7
    do_break(2148811540);
L_80144318:
    // 0x80144318: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x8014431C: beq         $v0, $zero, L_8014437C
    if (ctx->r2 == 0) {
        // 0x80144320: lui         $s1, 0x807F
        ctx->r17 = S32(0X807F << 16);
            goto L_8014437C;
    }
    // 0x80144320: lui         $s1, 0x807F
    ctx->r17 = S32(0X807F << 16);
    // 0x80144324: lui         $s2, 0x8023
    ctx->r18 = S32(0X8023 << 16);
    // 0x80144328: addiu       $s2, $s2, 0x6850
    ctx->r18 = ADD32(ctx->r18, 0X6850);
    // 0x8014432C: ori         $s1, $s1, 0xC000
    ctx->r17 = ctx->r17 | 0XC000;
L_80144330:
    // 0x80144330: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x80144334: sltu        $v0, $v1, $s2
    ctx->r2 = ctx->r3 < ctx->r18 ? 1 : 0;
    // 0x80144338: beq         $v0, $zero, L_80144350
    if (ctx->r2 == 0) {
        // 0x8014433C: addiu       $a0, $zero, 0x1
        ctx->r4 = ADD32(0, 0X1);
            goto L_80144350;
    }
    // 0x8014433C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80144340: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80144344: addiu       $a1, $a1, 0x4CA8
    ctx->r5 = ADD32(ctx->r5, 0X4CA8);
    // 0x80144348: j           L_80144364
    // 0x8014434C: nop

        goto L_80144364;
    // 0x8014434C: nop

L_80144350:
    // 0x80144350: sltu        $v0, $s1, $v1
    ctx->r2 = ctx->r17 < ctx->r3 ? 1 : 0;
    // 0x80144354: beql        $v0, $zero, L_80144370
    if (ctx->r2 == 0) {
        // 0x80144358: addu        $s0, $v1, $zero
        ctx->r16 = ADD32(ctx->r3, 0);
            goto L_80144370;
    }
    goto skip_0;
    // 0x80144358: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
    skip_0:
    // 0x8014435C: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80144360: addiu       $a1, $a1, 0x4CC0
    ctx->r5 = ADD32(ctx->r5, 0X4CC0);
L_80144364:
    // 0x80144364: jal         0x80145020
    // 0x80144368: addiu       $a2, $zero, 0x14
    ctx->r6 = ADD32(0, 0X14);
    func_80145020(rdram, ctx);
        goto after_1;
    // 0x80144368: addiu       $a2, $zero, 0x14
    ctx->r6 = ADD32(0, 0X14);
    after_1:
    // 0x8014436C: break       1, 7
    do_break(2148811628);
L_80144370:
    // 0x80144370: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x80144374: bne         $v0, $zero, L_80144330
    if (ctx->r2 != 0) {
        // 0x80144378: nop
    
            goto L_80144330;
    }
    // 0x80144378: nop

L_8014437C:
    // 0x8014437C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80144380: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x80144384: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80144388: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8014438C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80144390: jr          $ra
    // 0x80144394: nop

    return;
    // 0x80144394: nop

;}
RECOMP_FUNC void func_801443A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801443A0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801443A4: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x801443A8: lui         $s0, 0x8023
    ctx->r16 = S32(0X8023 << 16);
    // 0x801443AC: addiu       $s0, $s0, 0x6850
    ctx->r16 = ADD32(ctx->r16, 0X6850);
    // 0x801443B0: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x801443B4: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x801443B8: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x801443BC: beq         $v0, $zero, L_80144428
    if (ctx->r2 == 0) {
        // 0x801443C0: sw          $s1, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r17;
            goto L_80144428;
    }
    // 0x801443C0: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x801443C4: addu        $s3, $s0, $zero
    ctx->r19 = ADD32(ctx->r16, 0);
    // 0x801443C8: lui         $s1, 0x807F
    ctx->r17 = S32(0X807F << 16);
    // 0x801443CC: ori         $s1, $s1, 0xC000
    ctx->r17 = ctx->r17 | 0XC000;
    // 0x801443D0: addiu       $s2, $zero, -0x2
    ctx->r18 = ADD32(0, -0X2);
L_801443D4:
    // 0x801443D4: sltu        $v0, $s0, $s3
    ctx->r2 = ctx->r16 < ctx->r19 ? 1 : 0;
    // 0x801443D8: beq         $v0, $zero, L_801443F0
    if (ctx->r2 == 0) {
        // 0x801443DC: addiu       $a0, $zero, 0x1
        ctx->r4 = ADD32(0, 0X1);
            goto L_801443F0;
    }
    // 0x801443DC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801443E0: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x801443E4: addiu       $a1, $a1, 0x4CD8
    ctx->r5 = ADD32(ctx->r5, 0X4CD8);
    // 0x801443E8: j           L_80144404
    // 0x801443EC: nop

        goto L_80144404;
    // 0x801443EC: nop

L_801443F0:
    // 0x801443F0: sltu        $v0, $s1, $s0
    ctx->r2 = ctx->r17 < ctx->r16 ? 1 : 0;
    // 0x801443F4: beq         $v0, $zero, L_80144410
    if (ctx->r2 == 0) {
        // 0x801443F8: nop
    
            goto L_80144410;
    }
    // 0x801443F8: nop

    // 0x801443FC: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80144400: addiu       $a1, $a1, 0x4CF0
    ctx->r5 = ADD32(ctx->r5, 0X4CF0);
L_80144404:
    // 0x80144404: jal         0x800C414C
    // 0x80144408: addiu       $a2, $zero, 0x14
    ctx->r6 = ADD32(0, 0X14);
    static_0_800C414C(rdram, ctx);
        goto after_0;
    // 0x80144408: addiu       $a2, $zero, 0x14
    ctx->r6 = ADD32(0, 0X14);
    after_0:
    // 0x8014440C: break       1, 7
    do_break(2148811788);
L_80144410:
    // 0x80144410: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x80144414: and         $v0, $v0, $s2
    ctx->r2 = ctx->r2 & ctx->r18;
    // 0x80144418: addu        $v0, $s0, $v0
    ctx->r2 = ADD32(ctx->r16, ctx->r2);
    // 0x8014441C: lw          $v1, 0xC($v0)
    ctx->r3 = MEM_W(ctx->r2, 0XC);
    // 0x80144420: bne         $v1, $zero, L_801443D4
    if (ctx->r3 != 0) {
        // 0x80144424: addiu       $s0, $v0, 0x8
        ctx->r16 = ADD32(ctx->r2, 0X8);
            goto L_801443D4;
    }
    // 0x80144424: addiu       $s0, $v0, 0x8
    ctx->r16 = ADD32(ctx->r2, 0X8);
L_80144428:
    // 0x80144428: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x8014442C: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x80144430: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x80144434: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80144438: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8014443C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80144440: jr          $ra
    // 0x80144444: nop

    return;
    // 0x80144444: nop

;}
RECOMP_FUNC void func_80144448(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144448: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8014444C: addiu       $v0, $v0, 0x1EA0
    ctx->r2 = ADD32(ctx->r2, 0X1EA0);
    // 0x80144450: jr          $ra
    // 0x80144454: nop

    return;
    // 0x80144454: nop

;}
RECOMP_FUNC void func_80144458(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144458: jr          $ra
    // 0x8014445C: sqrt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = sqrtf(ctx->f12.fl);
    return;
    // 0x8014445C: sqrt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = sqrtf(ctx->f12.fl);
;}
RECOMP_FUNC void func_80144460(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144460: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80144464: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x80144468: jal         0x801444F8
    // 0x8014446C: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    func_801444F8(rdram, ctx);
        goto after_0;
    // 0x8014446C: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    after_0:
    // 0x80144470: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80144474: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x80144478: bc1t        L_8014448C
    if (c1cs) {
        // 0x8014447C: nop
    
            goto L_8014448C;
    }
    // 0x8014447C: nop

    // 0x80144480: lwc1        $f0, 0x10($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80144484: j           L_8014449C
    // 0x80144488: nop

        goto L_8014449C;
    // 0x80144488: nop

L_8014448C:
    // 0x8014448C: lwc1        $f2, 0x10($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80144490: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144494: lwc1        $f0, 0x4D08($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4D08);
    // 0x80144498: sub.s       $f0, $f2, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f2.fl - ctx->f0.fl;
L_8014449C:
    // 0x8014449C: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x801444A0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801444A4: jr          $ra
    // 0x801444A8: nop

    return;
    // 0x801444A8: nop

;}
RECOMP_FUNC void func_801444AC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801444AC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801444B0: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    // 0x801444B4: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x801444B8: jal         0x801444F8
    // 0x801444BC: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    func_801444F8(rdram, ctx);
        goto after_0;
    // 0x801444BC: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_0:
    // 0x801444C0: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x801444C4: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x801444C8: bc1f        L_801444E4
    if (!c1cs) {
        // 0x801444CC: nop
    
            goto L_801444E4;
    }
    // 0x801444CC: nop

    // 0x801444D0: lwc1        $f2, 0x10($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X10);
    // 0x801444D4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801444D8: lwc1        $f0, 0x4D0C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4D0C);
    // 0x801444DC: j           L_801444E8
    // 0x801444E0: sub.s       $f0, $f2, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f2.fl - ctx->f0.fl;
        goto L_801444E8;
    // 0x801444E0: sub.s       $f0, $f2, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f2.fl - ctx->f0.fl;
L_801444E4:
    // 0x801444E4: lwc1        $f0, 0x10($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X10);
L_801444E8:
    // 0x801444E8: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x801444EC: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x801444F0: jr          $ra
    // 0x801444F4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x801444F4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_801444F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801444F8: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x801444FC: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x80144500: mtc1        $zero, $f24
    ctx->f24.u32l = 0;
    // 0x80144504: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x80144508: mov.s       $f22, $f12
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 12);
    ctx->f22.fl = ctx->f12.fl;
    // 0x8014450C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80144510: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x80144514: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x80144518: c.lt.s      $f24, $f22
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f24.fl < ctx->f22.fl;
    // 0x8014451C: mov.s       $f20, $f22
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 22);
    ctx->f20.fl = ctx->f22.fl;
    // 0x80144520: bc1t        L_8014452C
    if (c1cs) {
        // 0x80144524: sw          $ra, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r31;
            goto L_8014452C;
    }
    // 0x80144524: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80144528: neg.s       $f20, $f22
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f22.fl); 
    ctx->f20.fl = -ctx->f22.fl;
L_8014452C:
    // 0x8014452C: jal         0x80144C80
    // 0x80144530: nop

    func_80144C80(rdram, ctx);
        goto after_0;
    // 0x80144530: nop

    after_0:
    // 0x80144534: c.le.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl <= ctx->f20.fl;
    // 0x80144538: bc1f        L_80144548
    if (!c1cs) {
        // 0x8014453C: mov.s       $f0, $f24
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    ctx->f0.fl = ctx->f24.fl;
            goto L_80144548;
    }
    // 0x8014453C: mov.s       $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    ctx->f0.fl = ctx->f24.fl;
    // 0x80144540: j           L_801445E4
    // 0x80144544: swc1        $f22, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f22.u32l;
        goto L_801445E4;
    // 0x80144544: swc1        $f22, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f22.u32l;
L_80144548:
    // 0x80144548: jal         0x80144C80
    // 0x8014454C: nop

    func_80144C80(rdram, ctx);
        goto after_1;
    // 0x8014454C: nop

    after_1:
    // 0x80144550: add.s       $f0, $f20, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f20.fl + ctx->f0.fl;
    // 0x80144554: jal         0x80144C80
    // 0x80144558: swc1        $f0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f0.u32l;
    func_80144C80(rdram, ctx);
        goto after_2;
    // 0x80144558: swc1        $f0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f0.u32l;
    after_2:
    // 0x8014455C: lwc1        $f2, 0x10($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80144560: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x80144564: c.lt.s      $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f20.fl < ctx->f2.fl;
    // 0x80144568: bc1f        L_80144580
    if (!c1cs) {
        // 0x8014456C: swc1        $f2, 0x10($sp)
        MEM_W(0X10, ctx->r29) = ctx->f2.u32l;
            goto L_80144580;
    }
    // 0x8014456C: swc1        $f2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f2.u32l;
    // 0x80144570: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144574: lwc1        $f0, 0x4D10($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4D10);
    // 0x80144578: sub.s       $f0, $f2, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x8014457C: swc1        $f0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f0.u32l;
L_80144580:
    // 0x80144580: lwc1        $f2, 0x10($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80144584: c.lt.s      $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f24.fl < ctx->f2.fl;
    // 0x80144588: bc1fl       L_80144590
    if (!c1cs) {
        // 0x8014458C: neg.s       $f2, $f2
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
            goto L_80144590;
    }
    goto skip_0;
    // 0x8014458C: neg.s       $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
    skip_0:
L_80144590:
    // 0x80144590: sub.s       $f0, $f22, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f22.fl - ctx->f2.fl;
    // 0x80144594: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144598: lwc1        $f4, 0x4D14($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4D14);
    // 0x8014459C: c.eq.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl == ctx->f4.fl;
    // 0x801445A0: bc1f        L_801445B0
    if (!c1cs) {
        // 0x801445A4: swc1        $f2, 0x10($sp)
        MEM_W(0X10, ctx->r29) = ctx->f2.u32l;
            goto L_801445B0;
    }
    // 0x801445A4: swc1        $f2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f2.u32l;
    // 0x801445A8: add.s       $f0, $f2, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f2.fl + ctx->f4.fl;
    // 0x801445AC: swc1        $f0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f0.u32l;
L_801445B0:
    // 0x801445B0: c.le.s      $f24, $f22
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f24.fl <= ctx->f22.fl;
    // 0x801445B4: bc1t        L_801445D4
    if (c1cs) {
        // 0x801445B8: nop
    
            goto L_801445D4;
    }
    // 0x801445B8: nop

    // 0x801445BC: lwc1        $f0, 0x10($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X10);
    // 0x801445C0: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x801445C4: swc1        $f0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f0.u32l;
    // 0x801445C8: lwc1        $f0, 0x10($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X10);
    // 0x801445CC: j           L_801445E4
    // 0x801445D0: add.s       $f0, $f22, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f22.fl + ctx->f0.fl;
        goto L_801445E4;
    // 0x801445D0: add.s       $f0, $f22, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f22.fl + ctx->f0.fl;
L_801445D4:
    // 0x801445D4: lwc1        $f0, 0x10($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X10);
    // 0x801445D8: swc1        $f0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f0.u32l;
    // 0x801445DC: lwc1        $f0, 0x10($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X10);
    // 0x801445E0: sub.s       $f0, $f22, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f22.fl - ctx->f0.fl;
L_801445E4:
    // 0x801445E4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801445E8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801445EC: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x801445F0: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x801445F4: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x801445F8: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x801445FC: jr          $ra
    // 0x80144600: nop

    return;
    // 0x80144600: nop

;}
RECOMP_FUNC void func_80144608(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144608: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8014460C: swc1        $f12, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f12.u32l;
    // 0x80144610: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x80144614: lui         $v1, 0x7F80
    ctx->r3 = S32(0X7F80 << 16);
    // 0x80144618: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x8014461C: bne         $v0, $v1, L_80144648
    if (ctx->r2 != ctx->r3) {
        // 0x80144620: sw          $ra, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r31;
            goto L_80144648;
    }
    // 0x80144620: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x80144624: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80144628: addiu       $a0, $a0, 0x4D18
    ctx->r4 = ADD32(ctx->r4, 0X4D18);
    // 0x8014462C: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    // 0x80144630: mfc1        $a1, $f12
    ctx->r5 = (int32_t)ctx->f12.u32l;
    // 0x80144634: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x80144638: jal         0x80144B00
    // 0x8014463C: swc1        $f12, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f12.u32l;
    func_80144B00(rdram, ctx);
        goto after_0;
    // 0x8014463C: swc1        $f12, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f12.u32l;
    after_0:
    // 0x80144640: j           L_8014486C
    // 0x80144644: nop

        goto L_8014486C;
    // 0x80144644: nop

L_80144648:
    // 0x80144648: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8014464C: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x80144650: bc1fl       L_80144684
    if (!c1cs) {
        // 0x80144654: cvt.d.s     $f0, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.d = CVT_D_S(ctx->f12.fl);
            goto L_80144684;
    }
    goto skip_0;
    // 0x80144654: cvt.d.s     $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.d = CVT_D_S(ctx->f12.fl);
    skip_0:
    // 0x80144658: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014465C: lwc1        $f14, 0x4D1C($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4D1C);
    // 0x80144660: jal         0x801448E8
    // 0x80144664: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    static_0_801448E8(rdram, ctx);
        goto after_1;
    // 0x80144664: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_1:
    // 0x80144668: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014466C: ldc1        $f2, 0x4D20($at)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r1, 0X4D20);
    // 0x80144670: cvt.d.s     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.d = CVT_D_S(ctx->f0.fl);
    // 0x80144674: sub.d       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f0.d); 
    ctx->f2.d = ctx->f2.d - ctx->f0.d;
    // 0x80144678: cvt.s.d     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.d); 
    ctx->f2.fl = CVT_S_D(ctx->f2.d);
    // 0x8014467C: j           L_801446AC
    // 0x80144680: swc1        $f2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f2.u32l;
        goto L_801446AC;
    // 0x80144680: swc1        $f2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f2.u32l;
L_80144684:
    // 0x80144684: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144688: ldc1        $f2, 0x4D28($at)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r1, 0X4D28);
    // 0x8014468C: c.le.d      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.d <= ctx->f0.d;
    // 0x80144690: bc1f        L_801446AC
    if (!c1cs) {
        // 0x80144694: nop
    
            goto L_801446AC;
    }
    // 0x80144694: nop

    // 0x80144698: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014469C: lwc1        $f14, 0x4D30($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4D30);
    // 0x801446A0: jal         0x801448E8
    // 0x801446A4: nop

    static_0_801448E8(rdram, ctx);
        goto after_2;
    // 0x801446A4: nop

    after_2:
    // 0x801446A8: swc1        $f0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f0.u32l;
L_801446AC:
    // 0x801446AC: lwc1        $f10, 0x20($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X20);
    // 0x801446B0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801446B4: ldc1        $f0, 0x4D38($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X4D38);
    // 0x801446B8: cvt.d.s     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f2.d = CVT_D_S(ctx->f10.fl);
    // 0x801446BC: c.lt.d      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.d < ctx->f0.d;
    // 0x801446C0: bc1f        L_80144734
    if (!c1cs) {
        // 0x801446C4: nop
    
            goto L_80144734;
    }
    // 0x801446C4: nop

    // 0x801446C8: mul.s       $f0, $f10, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = MUL_S(ctx->f10.fl, ctx->f10.fl);
    // 0x801446CC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801446D0: lwc1        $f2, 0x4D40($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4D40);
    // 0x801446D4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801446D8: lwc1        $f4, 0x4D44($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4D44);
    // 0x801446DC: .word       0x4C8200A0                   # INVALID     $a0, $v0, 0xA0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f0.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x801446E0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801446E4: lwc1        $f4, 0x4D48($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4D48);
    // 0x801446E8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801446EC: lwc1        $f6, 0x4D4C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X4D4C);
    // 0x801446F0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801446F4: lwc1        $f8, 0x4D50($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X4D50);
    // 0x801446F8: .word       0x4C8200A0                   # INVALID     $a0, $v0, 0xA0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f0.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x801446FC: .word       0x4CC200A0                   # INVALID     $a2, $v0, 0xA0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f0.fl * ctx->f2.fl + ctx->f6.fl;
    // 0x80144700: .word       0x4D0200A0                   # INVALID     $t0, $v0, 0xA0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f0.fl * ctx->f2.fl + ctx->f8.fl;
    // 0x80144704: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144708: lwc1        $f4, 0x4D54($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4D54);
    // 0x8014470C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144710: lwc1        $f6, 0x4D58($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X4D58);
    // 0x80144714: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144718: lwc1        $f8, 0x4D5C($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X4D5C);
    // 0x8014471C: .word       0x4C8200A0                   # INVALID     $a0, $v0, 0xA0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f0.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x80144720: .word       0x4CC200A0                   # INVALID     $a2, $v0, 0xA0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f0.fl * ctx->f2.fl + ctx->f6.fl;
    // 0x80144724: .word       0x4D0200A0                   # INVALID     $t0, $v0, 0xA0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f0.fl * ctx->f2.fl + ctx->f8.fl;
    // 0x80144728: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x8014472C: j           L_8014486C
    // 0x80144730: .word       0x4D405020                   # INVALID     $t2, $zero, 0x5020 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f0.fl = ctx->f10.fl * ctx->f0.fl + ctx->f10.fl;
        goto L_8014486C;
    // 0x80144730: .word       0x4D405020                   # INVALID     $t2, $zero, 0x5020 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f0.fl = ctx->f10.fl * ctx->f0.fl + ctx->f10.fl;
L_80144734:
    // 0x80144734: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144738: ldc1        $f4, 0x4D60($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, 0X4D60);
    // 0x8014473C: c.lt.d      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.d < ctx->f4.d;
    // 0x80144740: bc1f        L_801447C0
    if (!c1cs) {
        // 0x80144744: nop
    
            goto L_801447C0;
    }
    // 0x80144744: nop

    // 0x80144748: sub.d       $f0, $f4, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f2.d); 
    ctx->f0.d = ctx->f4.d - ctx->f2.d;
    // 0x8014474C: cvt.s.d     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.d); 
    ctx->f0.fl = CVT_S_D(ctx->f0.d);
    // 0x80144750: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80144754: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144758: lwc1        $f2, 0x4D68($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4D68);
    // 0x8014475C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144760: lwc1        $f4, 0x4D6C($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4D6C);
    // 0x80144764: .word       0x4C8230A0                   # INVALID     $a0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x80144768: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014476C: lwc1        $f4, 0x4D70($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4D70);
    // 0x80144770: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144774: lwc1        $f8, 0x4D74($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X4D74);
    // 0x80144778: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014477C: lwc1        $f10, 0x4D78($at)
    ctx->f10.u32l = MEM_W(ctx->r1, 0X4D78);
    // 0x80144780: .word       0x4C8230A0                   # INVALID     $a0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x80144784: .word       0x4D0230A0                   # INVALID     $t0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f8.fl;
    // 0x80144788: .word       0x4D4230A0                   # INVALID     $t2, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f10.fl;
    // 0x8014478C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144790: lwc1        $f4, 0x4D7C($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4D7C);
    // 0x80144794: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144798: lwc1        $f8, 0x4D80($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X4D80);
    // 0x8014479C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801447A0: lwc1        $f10, 0x4D84($at)
    ctx->f10.u32l = MEM_W(ctx->r1, 0X4D84);
    // 0x801447A4: .word       0x4C8230A0                   # INVALID     $a0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x801447A8: .word       0x4D0230A0                   # INVALID     $t0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f8.fl;
    // 0x801447AC: .word       0x4D4230A0                   # INVALID     $t2, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f10.fl;
    // 0x801447B0: mul.s       $f6, $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x801447B4: swc1        $f0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f0.u32l;
    // 0x801447B8: j           L_8014486C
    // 0x801447BC: .word       0x4C060020                   # INVALID     $zero, $a2, 0x20 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f0.fl = ctx->f0.fl * ctx->f6.fl + ctx->f0.fl;
        goto L_8014486C;
    // 0x801447BC: .word       0x4C060020                   # INVALID     $zero, $a2, 0x20 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f0.fl = ctx->f0.fl * ctx->f6.fl + ctx->f0.fl;
L_801447C0:
    // 0x801447C0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801447C4: ldc1        $f0, 0x4D88($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X4D88);
    // 0x801447C8: c.lt.d      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.d < ctx->f0.d;
    // 0x801447CC: bc1tl       L_801447FC
    if (c1cs) {
        // 0x801447D0: sub.d       $f0, $f2, $f4
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f4.d); 
    ctx->f0.d = ctx->f2.d - ctx->f4.d;
            goto L_801447FC;
    }
    goto skip_1;
    // 0x801447D0: sub.d       $f0, $f2, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f4.d); 
    ctx->f0.d = ctx->f2.d - ctx->f4.d;
    skip_1:
    // 0x801447D4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801447D8: ldc1        $f0, 0x4D90($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X4D90);
    // 0x801447DC: sub.d       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f2.d); 
    ctx->f0.d = ctx->f0.d - ctx->f2.d;
    // 0x801447E0: cvt.s.d     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.d); 
    ctx->f0.fl = CVT_S_D(ctx->f0.d);
    // 0x801447E4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801447E8: lwc1        $f4, 0x4D98($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4D98);
    // 0x801447EC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801447F0: lwc1        $f2, 0x4D9C($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4D9C);
    // 0x801447F4: j           L_80144814
    // 0x801447F8: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
        goto L_80144814;
    // 0x801447F8: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
L_801447FC:
    // 0x801447FC: cvt.s.d     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.d); 
    ctx->f0.fl = CVT_S_D(ctx->f0.d);
    // 0x80144800: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80144804: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144808: lwc1        $f2, 0x4DA0($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4DA0);
    // 0x8014480C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144810: lwc1        $f4, 0x4DA4($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4DA4);
L_80144814:
    // 0x80144814: .word       0x4C8230A0                   # INVALID     $a0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x80144818: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014481C: lwc1        $f4, 0x4DA8($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4DA8);
    // 0x80144820: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144824: lwc1        $f8, 0x4DAC($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X4DAC);
    // 0x80144828: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014482C: lwc1        $f10, 0x4DB0($at)
    ctx->f10.u32l = MEM_W(ctx->r1, 0X4DB0);
    // 0x80144830: .word       0x4C8230A0                   # INVALID     $a0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x80144834: .word       0x4D0230A0                   # INVALID     $t0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f8.fl;
    // 0x80144838: .word       0x4D4230A0                   # INVALID     $t2, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f10.fl;
    // 0x8014483C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144840: lwc1        $f4, 0x4DB4($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4DB4);
    // 0x80144844: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144848: lwc1        $f8, 0x4DB8($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X4DB8);
    // 0x8014484C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144850: lwc1        $f10, 0x4DBC($at)
    ctx->f10.u32l = MEM_W(ctx->r1, 0X4DBC);
    // 0x80144854: .word       0x4C8230A0                   # INVALID     $a0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f4.fl;
    // 0x80144858: .word       0x4D0230A0                   # INVALID     $t0, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f8.fl;
    // 0x8014485C: .word       0x4D4230A0                   # INVALID     $t2, $v0, 0x30A0 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f2.fl = ctx->f6.fl * ctx->f2.fl + ctx->f10.fl;
    // 0x80144860: mul.s       $f6, $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x80144864: swc1        $f0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f0.u32l;
    // 0x80144868: .word       0x4C060030                   # INVALID     $zero, $a2, 0x30 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f0.fl = -(ctx->f0.fl * ctx->f6.fl + ctx->f0.fl);
L_8014486C:
    // 0x8014486C: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x80144870: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80144874: jr          $ra
    // 0x80144878: nop

    return;
    // 0x80144878: nop

;}
RECOMP_FUNC void func_8014487C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014487C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80144880: swc1        $f12, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f12.u32l;
    // 0x80144884: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x80144888: lui         $v1, 0x7F80
    ctx->r3 = S32(0X7F80 << 16);
    // 0x8014488C: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x80144890: beq         $v0, $v1, L_801448B8
    if (ctx->r2 == ctx->r3) {
        // 0x80144894: sw          $ra, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r31;
            goto L_801448B8;
    }
    // 0x80144894: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x80144898: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014489C: ldc1        $f0, 0x4DC8($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X4DC8);
    // 0x801448A0: cvt.d.s     $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.d = CVT_D_S(ctx->f12.fl);
    // 0x801448A4: add.d       $f12, $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.d); NAN_CHECK(ctx->f0.d); 
    ctx->f12.d = ctx->f12.d + ctx->f0.d;
    // 0x801448A8: jal         0x80144608
    // 0x801448AC: cvt.s.d     $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.d); 
    ctx->f12.fl = CVT_S_D(ctx->f12.d);
    func_80144608(rdram, ctx);
        goto after_0;
    // 0x801448AC: cvt.s.d     $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.d); 
    ctx->f12.fl = CVT_S_D(ctx->f12.d);
    after_0:
    // 0x801448B0: j           L_801448D4
    // 0x801448B4: nop

        goto L_801448D4;
    // 0x801448B4: nop

L_801448B8:
    // 0x801448B8: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x801448BC: addiu       $a0, $a0, 0x4DC0
    ctx->r4 = ADD32(ctx->r4, 0X4DC0);
    // 0x801448C0: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    // 0x801448C4: mfc1        $a1, $f12
    ctx->r5 = (int32_t)ctx->f12.u32l;
    // 0x801448C8: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x801448CC: jal         0x80144B00
    // 0x801448D0: swc1        $f12, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f12.u32l;
    func_80144B00(rdram, ctx);
        goto after_1;
    // 0x801448D0: swc1        $f12, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f12.u32l;
    after_1:
L_801448D4:
    // 0x801448D4: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x801448D8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801448DC: jr          $ra
    // 0x801448E0: nop

    return;
    // 0x801448E0: nop

;}
RECOMP_FUNC void func_801448EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801448EC: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801448F0: sw          $ra, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r31;
    // 0x801448F4: sdc1        $f22, 0x38($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X38, ctx->r29);
    // 0x801448F8: sdc1        $f20, 0x30($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X30, ctx->r29);
    // 0x801448FC: c.eq.s      $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl == ctx->f0.fl;
    // 0x80144900: swc1        $f14, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f14.u32l;
    // 0x80144904: bc1f        L_80144930
    if (!c1cs) {
        // 0x80144908: swc1        $f12, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->f12.u32l;
            goto L_80144930;
    }
    // 0x80144908: swc1        $f12, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f12.u32l;
    // 0x8014490C: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80144910: addiu       $a0, $a0, 0x4DD0
    ctx->r4 = ADD32(ctx->r4, 0X4DD0);
    // 0x80144914: mfc1        $a1, $f12
    ctx->r5 = (int32_t)ctx->f12.u32l;
    // 0x80144918: mfc1        $a2, $f14
    ctx->r6 = (int32_t)ctx->f14.u32l;
    // 0x8014491C: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x80144920: jal         0x80144B00
    // 0x80144924: sw          $a1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r5;
    func_80144B00(rdram, ctx);
        goto after_0;
    // 0x80144924: sw          $a1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r5;
    after_0:
    // 0x80144928: j           L_801449F8
    // 0x8014492C: nop

        goto L_801449F8;
    // 0x8014492C: nop

L_80144930:
    // 0x80144930: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80144934: lui         $v1, 0x7F80
    ctx->r3 = S32(0X7F80 << 16);
    // 0x80144938: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x8014493C: beq         $v0, $v1, L_80144954
    if (ctx->r2 == ctx->r3) {
        // 0x80144940: nop
    
            goto L_80144954;
    }
    // 0x80144940: nop

    // 0x80144944: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80144948: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x8014494C: bne         $v0, $v1, L_80144964
    if (ctx->r2 != ctx->r3) {
        // 0x80144950: nop
    
            goto L_80144964;
    }
    // 0x80144950: nop

L_80144954:
    // 0x80144954: jal         0x80144C60
    // 0x80144958: nop

    func_80144C60(rdram, ctx);
        goto after_1;
    // 0x80144958: nop

    after_1:
    // 0x8014495C: j           L_801449F8
    // 0x80144960: nop

        goto L_801449F8;
    // 0x80144960: nop

L_80144964:
    // 0x80144964: jal         0x80144AE0
    // 0x80144968: nop

    func_80144AE0(rdram, ctx);
        goto after_2;
    // 0x80144968: nop

    after_2:
    // 0x8014496C: lwc1        $f12, 0x18($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X18);
    // 0x80144970: jal         0x80144AE0
    // 0x80144974: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
    func_80144AE0(rdram, ctx);
        goto after_3;
    // 0x80144974: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
    after_3:
    // 0x80144978: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
    // 0x8014497C: addiu       $a1, $sp, 0x20
    ctx->r5 = ADD32(ctx->r29, 0X20);
    // 0x80144980: jal         0x80144A10
    // 0x80144984: swc1        $f12, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f12.u32l;
    func_80144A10(rdram, ctx);
        goto after_4;
    // 0x80144984: swc1        $f12, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f12.u32l;
    after_4:
    // 0x80144988: lwc1        $f0, 0x18($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X18);
    // 0x8014498C: c.le.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl <= ctx->f20.fl;
    // 0x80144990: bc1f        L_801449E0
    if (!c1cs) {
        // 0x80144994: mov.s       $f12, $f20
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
            goto L_801449E0;
    }
    // 0x80144994: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    // 0x80144998: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8014499C: lwc1        $f22, 0x4DD8($at)
    ctx->f22.u32l = MEM_W(ctx->r1, 0X4DD8);
L_801449A0:
    // 0x801449A0: jal         0x80144A10
    // 0x801449A4: addiu       $a1, $sp, 0x24
    ctx->r5 = ADD32(ctx->r29, 0X24);
    func_80144A10(rdram, ctx);
        goto after_5;
    // 0x801449A4: addiu       $a1, $sp, 0x24
    ctx->r5 = ADD32(ctx->r29, 0X24);
    after_5:
    // 0x801449A8: lw          $v0, 0x24($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X24);
    // 0x801449AC: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x801449B0: lwc1        $f12, 0x18($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X18);
    // 0x801449B4: jal         0x80144CB0
    // 0x801449B8: subu        $a1, $v0, $a1
    ctx->r5 = SUB32(ctx->r2, ctx->r5);
    func_80144CB0(rdram, ctx);
        goto after_6;
    // 0x801449B8: subu        $a1, $v0, $a1
    ctx->r5 = SUB32(ctx->r2, ctx->r5);
    after_6:
    // 0x801449BC: c.le.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl <= ctx->f20.fl;
    // 0x801449C0: bc1fl       L_801449CC
    if (!c1cs) {
        // 0x801449C4: .word       0x4E960338                   # INVALID     $s4, $s6, 0x338 # 00000000 <InstrIdType: CPU_NORMAL>
        ctx->f12.fl = -(ctx->f0.fl * ctx->f22.fl - ctx->f20.fl);
            goto L_801449CC;
    }
    goto skip_0;
    // 0x801449C4: .word       0x4E960338                   # INVALID     $s4, $s6, 0x338 # 00000000 <InstrIdType: CPU_NORMAL>
    ctx->f12.fl = -(ctx->f0.fl * ctx->f22.fl - ctx->f20.fl);
    skip_0:
    // 0x801449C8: sub.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl - ctx->f0.fl;
L_801449CC:
    // 0x801449CC: lwc1        $f0, 0x18($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X18);
    // 0x801449D0: mov.s       $f20, $f12
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 12);
    ctx->f20.fl = ctx->f12.fl;
    // 0x801449D4: c.le.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl <= ctx->f20.fl;
    // 0x801449D8: bc1t        L_801449A0
    if (c1cs) {
        // 0x801449DC: mov.s       $f12, $f20
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
            goto L_801449A0;
    }
    // 0x801449DC: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
L_801449E0:
    // 0x801449E0: lwc1        $f2, 0x1C($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x801449E4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801449E8: c.le.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl <= ctx->f2.fl;
    // 0x801449EC: bc1t        L_801449F8
    if (c1cs) {
        // 0x801449F0: mov.s       $f0, $f20
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
            goto L_801449F8;
    }
    // 0x801449F0: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x801449F4: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
L_801449F8:
    // 0x801449F8: lw          $ra, 0x28($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X28);
    // 0x801449FC: ldc1        $f22, 0x38($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X38);
    // 0x80144A00: ldc1        $f20, 0x30($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X30);
    // 0x80144A04: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x80144A08: jr          $ra
    // 0x80144A0C: nop

    return;
    // 0x80144A0C: nop

;}
RECOMP_FUNC void func_80144A10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144A10: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80144A14: swc1        $f12, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f12.u32l;
    // 0x80144A18: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80144A1C: lui         $a0, 0x7F80
    ctx->r4 = S32(0X7F80 << 16);
    // 0x80144A20: and         $v0, $v0, $a0
    ctx->r2 = ctx->r2 & ctx->r4;
    // 0x80144A24: bne         $v0, $a0, L_80144A58
    if (ctx->r2 != ctx->r4) {
        // 0x80144A28: sw          $ra, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r31;
            goto L_80144A58;
    }
    // 0x80144A28: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x80144A2C: sw          $zero, 0x0($a1)
    MEM_W(0X0, ctx->r5) = 0;
    // 0x80144A30: lwc1        $f0, 0x18($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X18);
    // 0x80144A34: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80144A38: addiu       $a0, $a0, 0x4DE0
    ctx->r4 = ADD32(ctx->r4, 0X4DE0);
    // 0x80144A3C: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    // 0x80144A40: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x80144A44: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x80144A48: jal         0x80144B00
    // 0x80144A4C: swc1        $f0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f0.u32l;
    func_80144B00(rdram, ctx);
        goto after_0;
    // 0x80144A4C: swc1        $f0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f0.u32l;
    after_0:
    // 0x80144A50: j           L_80144AD0
    // 0x80144A54: nop

        goto L_80144AD0;
    // 0x80144A54: nop

L_80144A58:
    // 0x80144A58: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80144A5C: c.eq.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl == ctx->f0.fl;
    // 0x80144A60: bc1f        L_80144A78
    if (!c1cs) {
        // 0x80144A64: nop
    
            goto L_80144A78;
    }
    // 0x80144A64: nop

    // 0x80144A68: sw          $zero, 0x0($a1)
    MEM_W(0X0, ctx->r5) = 0;
    // 0x80144A6C: lwc1        $f0, 0x18($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X18);
    // 0x80144A70: j           L_80144AD0
    // 0x80144A74: nop

        goto L_80144AD0;
    // 0x80144A74: nop

L_80144A78:
    // 0x80144A78: mfc1        $v1, $f12
    ctx->r3 = (int32_t)ctx->f12.u32l;
    // 0x80144A7C: and         $v0, $v1, $a0
    ctx->r2 = ctx->r3 & ctx->r4;
    // 0x80144A80: beq         $v0, $zero, L_80144A94
    if (ctx->r2 == 0) {
        // 0x80144A84: srl         $v0, $v1, 23
        ctx->r2 = S32(U32(ctx->r3) >> 23);
            goto L_80144A94;
    }
    // 0x80144A84: srl         $v0, $v1, 23
    ctx->r2 = S32(U32(ctx->r3) >> 23);
    // 0x80144A88: andi        $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 & 0XFF;
    // 0x80144A8C: j           L_80144AB4
    // 0x80144A90: addiu       $a0, $v0, -0x7E
    ctx->r4 = ADD32(ctx->r2, -0X7E);
        goto L_80144AB4;
    // 0x80144A90: addiu       $a0, $v0, -0x7E
    ctx->r4 = ADD32(ctx->r2, -0X7E);
L_80144A94:
    // 0x80144A94: addiu       $a0, $zero, -0x7E
    ctx->r4 = ADD32(0, -0X7E);
    // 0x80144A98: lui         $a2, 0x7F80
    ctx->r6 = S32(0X7F80 << 16);
L_80144A9C:
    // 0x80144A9C: mtc1        $v1, $f0
    ctx->f0.u32l = ctx->r3;
    // 0x80144AA0: add.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f0.fl;
    // 0x80144AA4: mfc1        $v1, $f0
    ctx->r3 = (int32_t)ctx->f0.u32l;
    // 0x80144AA8: and         $v0, $v1, $a2
    ctx->r2 = ctx->r3 & ctx->r6;
    // 0x80144AAC: beq         $v0, $zero, L_80144A9C
    if (ctx->r2 == 0) {
        // 0x80144AB0: addiu       $a0, $a0, -0x1
        ctx->r4 = ADD32(ctx->r4, -0X1);
            goto L_80144A9C;
    }
    // 0x80144AB0: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
L_80144AB4:
    // 0x80144AB4: lui         $v0, 0x807F
    ctx->r2 = S32(0X807F << 16);
    // 0x80144AB8: ori         $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 | 0XFFFF;
    // 0x80144ABC: and         $v1, $v1, $v0
    ctx->r3 = ctx->r3 & ctx->r2;
    // 0x80144AC0: sw          $a0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r4;
    // 0x80144AC4: lui         $v0, 0x3F00
    ctx->r2 = S32(0X3F00 << 16);
    // 0x80144AC8: or          $v1, $v1, $v0
    ctx->r3 = ctx->r3 | ctx->r2;
    // 0x80144ACC: mtc1        $v1, $f0
    ctx->f0.u32l = ctx->r3;
L_80144AD0:
    // 0x80144AD0: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x80144AD4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80144AD8: jr          $ra
    // 0x80144ADC: nop

    return;
    // 0x80144ADC: nop

;}
RECOMP_FUNC void func_80144AE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144AE0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80144AE4: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x80144AE8: bc1t        L_80144AF4
    if (c1cs) {
        // 0x80144AEC: mov.s       $f0, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
            goto L_80144AF4;
    }
    // 0x80144AEC: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    // 0x80144AF0: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
L_80144AF4:
    // 0x80144AF4: jr          $ra
    // 0x80144AF8: nop

    return;
    // 0x80144AF8: nop

;}
RECOMP_FUNC void func_80144B00(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144B00: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80144B04: sw          $s0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r16;
    // 0x80144B08: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x80144B0C: sdc1        $f20, 0x38($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X38, ctx->r29);
    // 0x80144B10: mtc1        $a1, $f20
    ctx->f20.u32l = ctx->r5;
    // 0x80144B14: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    // 0x80144B18: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80144B1C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80144B20: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    // 0x80144B24: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    // 0x80144B28: sw          $zero, 0x28($sp)
    MEM_W(0X28, ctx->r29) = 0;
    // 0x80144B2C: jal         0x80144C28
    // 0x80144B30: swc1        $f20, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f20.u32l;
    func_80144C28(rdram, ctx);
        goto after_0;
    // 0x80144B30: swc1        $f20, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f20.u32l;
    after_0:
    // 0x80144B34: beq         $v0, $zero, L_80144B5C
    if (ctx->r2 == 0) {
        // 0x80144B38: nop
    
            goto L_80144B5C;
    }
    // 0x80144B38: nop

    // 0x80144B3C: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x80144B40: beq         $v0, $zero, L_80144B50
    if (ctx->r2 == 0) {
        // 0x80144B44: nop
    
            goto L_80144B50;
    }
    // 0x80144B44: nop

    // 0x80144B48: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80144B4C: sw          $v0, -0x1C8($at)
    MEM_W(-0X1C8, ctx->r1) = ctx->r2;
L_80144B50:
    // 0x80144B50: lwc1        $f0, 0x2C($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80144B54: j           L_80144C0C
    // 0x80144B58: nop

        goto L_80144C0C;
    // 0x80144B58: nop

L_80144B5C:
    // 0x80144B5C: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80144B60: addiu       $v1, $v0, -0x1
    ctx->r3 = ADD32(ctx->r2, -0X1);
    // 0x80144B64: sltiu       $v0, $v1, 0x6
    ctx->r2 = ctx->r3 < 0X6 ? 1 : 0;
    // 0x80144B68: beq         $v0, $zero, L_80144C08
    if (ctx->r2 == 0) {
        // 0x80144B6C: sll         $v0, $v1, 2
        ctx->r2 = S32(ctx->r3 << 2);
            goto L_80144C08;
    }
    // 0x80144B6C: sll         $v0, $v1, 2
    ctx->r2 = S32(ctx->r3 << 2);
    // 0x80144B70: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80144B74: addu        $at, $at, $v0
    gpr jr_addend_80144B7C = ctx->r2;
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80144B78: lw          $v0, 0x4E18($at)
    ctx->r2 = ADD32(ctx->r1, 0X4E18);
    // 0x80144B7C: jr          $v0
    // 0x80144B80: nop

    switch (jr_addend_80144B7C >> 2) {
        case 0: goto L_80144BB8; break;
        case 1: goto L_80144B84; break;
        case 2: goto L_80144BFC; break;
        case 3: goto L_80144BFC; break;
        case 4: goto L_80144C08; break;
        case 5: goto L_80144C08; break;
        default: switch_error(__func__, 0x80144B7C, 0x80174E18);
    }
    // 0x80144B80: nop

L_80144B84:
    // 0x80144B84: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80144B88: addiu       $a0, $a0, 0x1DD8
    ctx->r4 = ADD32(ctx->r4, 0X1DD8);
    // 0x80144B8C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80144B90: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80144B94: sw          $v0, -0x1C8($at)
    MEM_W(-0X1C8, ctx->r1) = ctx->r2;
    // 0x80144B98: jal         0x80142988
    // 0x80144B9C: nop

    func_80142988(rdram, ctx);
        goto after_1;
    // 0x80144B9C: nop

    after_1:
    // 0x80144BA0: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80144BA4: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x80144BA8: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80144BAC: addiu       $a1, $a1, 0x4DE8
    ctx->r5 = ADD32(ctx->r5, 0X4DE8);
    // 0x80144BB0: j           L_80144BE8
    // 0x80144BB4: cvt.d.s     $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f0.d = CVT_D_S(ctx->f20.fl);
        goto L_80144BE8;
    // 0x80144BB4: cvt.d.s     $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f0.d = CVT_D_S(ctx->f20.fl);
L_80144BB8:
    // 0x80144BB8: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80144BBC: addiu       $a0, $a0, 0x1DD8
    ctx->r4 = ADD32(ctx->r4, 0X1DD8);
    // 0x80144BC0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80144BC4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80144BC8: sw          $v0, -0x1C8($at)
    MEM_W(-0X1C8, ctx->r1) = ctx->r2;
    // 0x80144BCC: jal         0x80142988
    // 0x80144BD0: nop

    func_80142988(rdram, ctx);
        goto after_2;
    // 0x80144BD0: nop

    after_2:
    // 0x80144BD4: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80144BD8: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x80144BDC: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80144BE0: addiu       $a1, $a1, 0x4DFC
    ctx->r5 = ADD32(ctx->r5, 0X4DFC);
    // 0x80144BE4: cvt.d.s     $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f0.d = CVT_D_S(ctx->f20.fl);
L_80144BE8:
    // 0x80144BE8: sdc1        $f0, 0x10($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X10, ctx->r29);
    // 0x80144BEC: jal         0x80142DF8
    // 0x80144BF0: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    static_0_80142DF8(rdram, ctx);
        goto after_3;
    // 0x80144BF0: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    after_3:
    // 0x80144BF4: j           L_80144C08
    // 0x80144BF8: nop

        goto L_80144C08;
    // 0x80144BF8: nop

L_80144BFC:
    // 0x80144BFC: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80144C00: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80144C04: sw          $v0, -0x1C8($at)
    MEM_W(-0X1C8, ctx->r1) = ctx->r2;
L_80144C08:
    // 0x80144C08: lwc1        $f0, 0x50($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X50);
L_80144C0C:
    // 0x80144C0C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80144C10: lw          $s0, 0x30($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X30);
    // 0x80144C14: ldc1        $f20, 0x38($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X38);
    // 0x80144C18: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x80144C1C: jr          $ra
    // 0x80144C20: nop

    return;
    // 0x80144C20: nop

;}
RECOMP_FUNC void func_80144C28(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144C28: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80144C2C: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80144C30: lw          $a2, 0x0($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X0);
    // 0x80144C34: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80144C38: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x80144C3C: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80144C40: addiu       $a1, $a1, 0x4E30
    ctx->r5 = ADD32(ctx->r5, 0X4E30);
    // 0x80144C44: jal         0x80142DF8
    // 0x80144C48: nop

    static_0_80142DF8(rdram, ctx);
        goto after_0;
    // 0x80144C48: nop

    after_0:
    // 0x80144C4C: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80144C50: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x80144C54: jr          $ra
    // 0x80144C58: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x80144C58: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_80144C60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144C60: lui         $v0, 0x7F80
    ctx->r2 = S32(0X7F80 << 16);
    // 0x80144C64: mtc1        $v0, $f0
    ctx->f0.u32l = ctx->r2;
    // 0x80144C68: jr          $ra
    // 0x80144C6C: nop

    return;
    // 0x80144C6C: nop

;}
