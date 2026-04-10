#include "recomp.h"
#include "funcs.h"

RECOMP_FUNC void func_800D1408(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D1408: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D140C: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D1410: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1414: lwc1        $f6, -0xB60($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB60);
    // 0x800D1418: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D141C: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D1420: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D1424: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D1428: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D142C: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D1430: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D1434: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D1438: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D143C: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D1440: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D1444: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D1448: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D144C: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D1450: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D1454: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D1458: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D145C: bc1f        L_800D14AC
    if (!c1cs) {
        // 0x800D1460: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D14AC;
    }
    // 0x800D1460: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D1464: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D1468: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D146C: nop

    // 0x800D1470: bc1f        L_800D1484
    if (!c1cs) {
        // 0x800D1474: addu        $s1, $a0, $zero
        ctx->r17 = ADD32(ctx->r4, 0);
            goto L_800D1484;
    }
    // 0x800D1474: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D1478: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D147C: j           L_800D14F8
    // 0x800D1480: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D14F8;
    // 0x800D1480: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D1484:
    // 0x800D1484: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D1488: nop

    // 0x800D148C: bc1f        L_800D149C
    if (!c1cs) {
        // 0x800D1490: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D149C;
    }
    // 0x800D1490: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D1494: j           L_800D14F0
    // 0x800D1498: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D14F0;
    // 0x800D1498: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D149C:
    // 0x800D149C: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D14A0: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D14A4: j           L_800D14F8
    // 0x800D14A8: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D14F8;
    // 0x800D14A8: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D14AC:
    // 0x800D14AC: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D14B0: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D14B4: nop

    // 0x800D14B8: bc1f        L_800D14CC
    if (!c1cs) {
        // 0x800D14BC: addu        $s1, $v1, $zero
        ctx->r17 = ADD32(ctx->r3, 0);
            goto L_800D14CC;
    }
    // 0x800D14BC: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
    // 0x800D14C0: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D14C4: j           L_800D14F0
    // 0x800D14C8: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D14F0;
    // 0x800D14C8: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D14CC:
    // 0x800D14CC: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D14D0: nop

    // 0x800D14D4: bc1f        L_800D14E4
    if (!c1cs) {
        // 0x800D14D8: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D14E4;
    }
    // 0x800D14D8: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D14DC: j           L_800D14F8
    // 0x800D14E0: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
        goto L_800D14F8;
    // 0x800D14E0: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D14E4:
    // 0x800D14E4: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D14E8: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D14EC: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D14F0:
    // 0x800D14F0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D14F4: lwc1        $f6, -0xB5C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB5C);
L_800D14F8:
    // 0x800D14F8: lwc1        $f8, 0x0($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D14FC: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D1500: sub.s       $f26, $f8, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x800D1504: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D1508: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D150C: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1510: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D1514: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1518: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D151C: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D1520: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D1524: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D1528: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D152C: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x800D1530: c.eq.s      $f28, $f2
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f28.fl == ctx->f2.fl;
    // 0x800D1534: nop

    // 0x800D1538: bc1t        L_800D16C0
    if (c1cs) {
        // 0x800D153C: nop
    
            goto L_800D16C0;
    }
    // 0x800D153C: nop

    // 0x800D1540: mul.s       $f0, $f28, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f28.fl, ctx->f6.fl);
    // 0x800D1544: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x800D1548: nop

    // 0x800D154C: bc1t        L_800D16C0
    if (c1cs) {
        // 0x800D1550: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D16C0;
    }
    // 0x800D1550: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D1554: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1558: lwc1        $f2, -0xB58($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB58);
    // 0x800D155C: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D1560: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D1564: swc1        $f8, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f8.u32l;
    // 0x800D1568: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D156C: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D1570: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D1574: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D1578: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D157C: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D1580: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D1584: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1588: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D158C: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D1590: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D1594: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D1598: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D159C: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D15A0: jal         0x80167848
    // 0x800D15A4: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D15A4: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D15A8: lwc1        $f0, 0xC($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D15AC: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D15B0: lwc1        $f0, 0xC($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D15B4: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D15B8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D15BC: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D15C0: lwc1        $f4, 0xC($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D15C4: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D15C8: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D15CC: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D15D0: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D15D4: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D15D8: lwc1        $f2, 0xC($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D15DC: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D15E0: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D15E4: lwc1        $f0, 0xC($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D15E8: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D15EC: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D15F0: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D15F4: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D15F8: lwc1        $f0, 0x10($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D15FC: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1600: lwc1        $f0, 0x10($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D1604: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D1608: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D160C: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1610: lwc1        $f4, 0x10($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D1614: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1618: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D161C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1620: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D1624: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D1628: lwc1        $f2, 0x10($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D162C: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D1630: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1634: lwc1        $f0, 0x10($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D1638: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D163C: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D1640: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1644: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D1648: lwc1        $f0, 0x8($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D164C: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D1650: lwc1        $f0, 0x8($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D1654: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D1658: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D165C: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1660: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D1664: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1668: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D166C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1670: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1674: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D1678: lwc1        $f2, 0x8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D167C: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D1680: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1684: lwc1        $f0, 0x8($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D1688: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D168C: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D1690: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D1694: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1698: ori         $a0, $a0, 0xDC
    ctx->r4 = ctx->r4 | 0XDC;
    // 0x800D169C: addiu       $a1, $zero, 0x28
    ctx->r5 = ADD32(0, 0X28);
    // 0x800D16A0: swc1        $f28, 0x34($gp)
    MEM_W(0X34, ctx->r28) = ctx->f28.u32l;
    // 0x800D16A4: jal         0x80167848
    // 0x800D16A8: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D16A8: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D16AC: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D16B0: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D16B4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D16B8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D16BC: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D16C0:
    // 0x800D16C0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D16C4: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D16C8: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D16CC: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D16D0: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D16D4: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D16D8: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D16DC: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D16E0: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D16E4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D16E8: jr          $ra
    // 0x800D16EC: nop

    return;
    // 0x800D16EC: nop

;}
RECOMP_FUNC void func_800D16F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D16F0: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x800D16F4: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D16F8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D16FC: lwc1        $f6, -0xB54($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB54);
    // 0x800D1700: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D1704: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D1708: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D170C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D1710: sdc1        $f30, 0x48($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X48, ctx->r29);
    // 0x800D1714: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D1718: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D171C: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D1720: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D1724: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D1728: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D172C: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D1730: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D1734: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D1738: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D173C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D1740: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D1744: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D1748: bc1f        L_800D1798
    if (!c1cs) {
        // 0x800D174C: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D1798;
    }
    // 0x800D174C: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D1750: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D1754: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D1758: nop

    // 0x800D175C: bc1f        L_800D1770
    if (!c1cs) {
        // 0x800D1760: addu        $s0, $a0, $zero
        ctx->r16 = ADD32(ctx->r4, 0);
            goto L_800D1770;
    }
    // 0x800D1760: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D1764: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D1768: j           L_800D17E4
    // 0x800D176C: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D17E4;
    // 0x800D176C: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D1770:
    // 0x800D1770: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D1774: nop

    // 0x800D1778: bc1f        L_800D1788
    if (!c1cs) {
        // 0x800D177C: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D1788;
    }
    // 0x800D177C: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D1780: j           L_800D17DC
    // 0x800D1784: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D17DC;
    // 0x800D1784: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D1788:
    // 0x800D1788: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D178C: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D1790: j           L_800D17E4
    // 0x800D1794: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D17E4;
    // 0x800D1794: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D1798:
    // 0x800D1798: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D179C: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D17A0: nop

    // 0x800D17A4: bc1f        L_800D17B8
    if (!c1cs) {
        // 0x800D17A8: addu        $s0, $v1, $zero
        ctx->r16 = ADD32(ctx->r3, 0);
            goto L_800D17B8;
    }
    // 0x800D17A8: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
    // 0x800D17AC: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D17B0: j           L_800D17DC
    // 0x800D17B4: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D17DC;
    // 0x800D17B4: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D17B8:
    // 0x800D17B8: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D17BC: nop

    // 0x800D17C0: bc1f        L_800D17D0
    if (!c1cs) {
        // 0x800D17C4: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D17D0;
    }
    // 0x800D17C4: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D17C8: j           L_800D17E4
    // 0x800D17CC: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
        goto L_800D17E4;
    // 0x800D17CC: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D17D0:
    // 0x800D17D0: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D17D4: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D17D8: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D17DC:
    // 0x800D17DC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D17E0: lwc1        $f6, -0xB50($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB50);
L_800D17E4:
    // 0x800D17E4: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D17E8: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D17EC: sub.s       $f26, $f8, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x800D17F0: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D17F4: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D17F8: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D17FC: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D1800: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1804: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D1808: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D180C: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D1810: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D1814: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1818: mtc1        $zero, $f30
    ctx->f30.u32l = 0;
    // 0x800D181C: c.eq.s      $f28, $f30
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 30);
    c1cs = ctx->f28.fl == ctx->f30.fl;
    // 0x800D1820: nop

    // 0x800D1824: bc1t        L_800D1A10
    if (c1cs) {
        // 0x800D1828: nop
    
            goto L_800D1A10;
    }
    // 0x800D1828: nop

    // 0x800D182C: mul.s       $f0, $f28, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f28.fl, ctx->f6.fl);
    // 0x800D1830: c.lt.s      $f0, $f30
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 30);
    c1cs = ctx->f0.fl < ctx->f30.fl;
    // 0x800D1834: nop

    // 0x800D1838: bc1t        L_800D1A10
    if (c1cs) {
        // 0x800D183C: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D1A10;
    }
    // 0x800D183C: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D1840: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1844: lwc1        $f2, -0xB4C($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB4C);
    // 0x800D1848: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D184C: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D1850: swc1        $f8, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f8.u32l;
    // 0x800D1854: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D1858: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D185C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D1860: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D1864: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D1868: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D186C: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D1870: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1874: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D1878: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D187C: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D1880: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D1884: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D1888: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D188C: jal         0x80167848
    // 0x800D1890: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D1890: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D1894: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D1898: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D189C: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D18A0: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D18A4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D18A8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D18AC: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D18B0: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D18B4: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D18B8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D18BC: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D18C0: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D18C4: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D18C8: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D18CC: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D18D0: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D18D4: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D18D8: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D18DC: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D18E0: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D18E4: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D18E8: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D18EC: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D18F0: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D18F4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D18F8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D18FC: lwc1        $f4, 0x10($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D1900: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1904: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1908: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D190C: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D1910: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D1914: lwc1        $f2, 0x10($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D1918: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D191C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1920: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D1924: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D1928: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D192C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1930: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D1934: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D1938: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D193C: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D1940: lwc1        $f2, 0x14($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D1944: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1948: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D194C: lwc1        $f4, 0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D1950: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1954: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1958: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D195C: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1960: lwc1        $f4, 0x14($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D1964: lwc1        $f2, 0x14($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D1968: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D196C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1970: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D1974: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D1978: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D197C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1980: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D1984: ori         $a0, $a0, 0xA0
    ctx->r4 = ctx->r4 | 0XA0;
    // 0x800D1988: addiu       $a1, $zero, 0x24
    ctx->r5 = ADD32(0, 0X24);
    // 0x800D198C: jal         0x80167848
    // 0x800D1990: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D1990: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D1994: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D1998: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D199C: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D19A0: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D19A4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D19A8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D19AC: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D19B0: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D19B4: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D19B8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D19BC: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D19C0: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D19C4: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D19C8: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D19CC: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D19D0: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D19D4: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D19D8: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D19DC: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D19E0: ori         $a0, $a0, 0xF4
    ctx->r4 = ctx->r4 | 0XF4;
    // 0x800D19E4: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D19E8: addiu       $a1, $zero, 0x14
    ctx->r5 = ADD32(0, 0X14);
    // 0x800D19EC: swc1        $f28, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f28.u32l;
    // 0x800D19F0: swc1        $f30, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f30.u32l;
    // 0x800D19F4: jal         0x80167848
    // 0x800D19F8: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_2;
    // 0x800D19F8: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    after_2:
    // 0x800D19FC: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D1A00: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D1A04: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D1A08: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D1A0C: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D1A10:
    // 0x800D1A10: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D1A14: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D1A18: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D1A1C: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D1A20: ldc1        $f30, 0x48($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X48);
    // 0x800D1A24: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D1A28: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D1A2C: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D1A30: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D1A34: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D1A38: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    // 0x800D1A3C: jr          $ra
    // 0x800D1A40: nop

    return;
    // 0x800D1A40: nop

;}
RECOMP_FUNC void func_800D1A44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D1A44: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D1A48: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D1A4C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1A50: lwc1        $f6, -0xB48($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB48);
    // 0x800D1A54: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D1A58: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D1A5C: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D1A60: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D1A64: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D1A68: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D1A6C: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D1A70: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D1A74: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D1A78: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D1A7C: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D1A80: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D1A84: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D1A88: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D1A8C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D1A90: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D1A94: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D1A98: bc1f        L_800D1AE8
    if (!c1cs) {
        // 0x800D1A9C: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D1AE8;
    }
    // 0x800D1A9C: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D1AA0: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D1AA4: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D1AA8: nop

    // 0x800D1AAC: bc1f        L_800D1AC0
    if (!c1cs) {
        // 0x800D1AB0: addu        $s0, $a0, $zero
        ctx->r16 = ADD32(ctx->r4, 0);
            goto L_800D1AC0;
    }
    // 0x800D1AB0: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D1AB4: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D1AB8: j           L_800D1B34
    // 0x800D1ABC: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D1B34;
    // 0x800D1ABC: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D1AC0:
    // 0x800D1AC0: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D1AC4: nop

    // 0x800D1AC8: bc1f        L_800D1AD8
    if (!c1cs) {
        // 0x800D1ACC: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D1AD8;
    }
    // 0x800D1ACC: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D1AD0: j           L_800D1B2C
    // 0x800D1AD4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D1B2C;
    // 0x800D1AD4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D1AD8:
    // 0x800D1AD8: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D1ADC: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D1AE0: j           L_800D1B34
    // 0x800D1AE4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D1B34;
    // 0x800D1AE4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D1AE8:
    // 0x800D1AE8: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D1AEC: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D1AF0: nop

    // 0x800D1AF4: bc1f        L_800D1B08
    if (!c1cs) {
        // 0x800D1AF8: addu        $s0, $v1, $zero
        ctx->r16 = ADD32(ctx->r3, 0);
            goto L_800D1B08;
    }
    // 0x800D1AF8: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
    // 0x800D1AFC: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D1B00: j           L_800D1B2C
    // 0x800D1B04: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D1B2C;
    // 0x800D1B04: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D1B08:
    // 0x800D1B08: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D1B0C: nop

    // 0x800D1B10: bc1f        L_800D1B20
    if (!c1cs) {
        // 0x800D1B14: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D1B20;
    }
    // 0x800D1B14: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D1B18: j           L_800D1B34
    // 0x800D1B1C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
        goto L_800D1B34;
    // 0x800D1B1C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D1B20:
    // 0x800D1B20: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D1B24: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D1B28: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D1B2C:
    // 0x800D1B2C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1B30: lwc1        $f6, -0xB44($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB44);
L_800D1B34:
    // 0x800D1B34: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D1B38: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D1B3C: sub.s       $f26, $f8, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x800D1B40: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D1B44: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D1B48: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1B4C: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D1B50: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1B54: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D1B58: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D1B5C: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D1B60: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D1B64: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1B68: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x800D1B6C: c.eq.s      $f28, $f2
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f28.fl == ctx->f2.fl;
    // 0x800D1B70: nop

    // 0x800D1B74: bc1t        L_800D1DFC
    if (c1cs) {
        // 0x800D1B78: nop
    
            goto L_800D1DFC;
    }
    // 0x800D1B78: nop

    // 0x800D1B7C: mul.s       $f0, $f28, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f28.fl, ctx->f6.fl);
    // 0x800D1B80: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x800D1B84: nop

    // 0x800D1B88: bc1t        L_800D1DFC
    if (c1cs) {
        // 0x800D1B8C: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D1DFC;
    }
    // 0x800D1B8C: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D1B90: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1B94: lwc1        $f2, -0xB40($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB40);
    // 0x800D1B98: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D1B9C: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D1BA0: swc1        $f8, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f8.u32l;
    // 0x800D1BA4: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D1BA8: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D1BAC: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D1BB0: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D1BB4: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D1BB8: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D1BBC: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D1BC0: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1BC4: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D1BC8: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D1BCC: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D1BD0: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D1BD4: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D1BD8: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D1BDC: jal         0x80167848
    // 0x800D1BE0: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D1BE0: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D1BE4: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D1BE8: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D1BEC: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D1BF0: lwc1        $f2, 0x14($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D1BF4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1BF8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1BFC: lwc1        $f4, 0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D1C00: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1C04: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1C08: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1C0C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D1C10: lwc1        $f4, 0x14($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D1C14: lwc1        $f2, 0x14($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D1C18: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D1C1C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1C20: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D1C24: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D1C28: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D1C2C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1C30: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D1C34: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D1C38: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1C3C: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D1C40: lwc1        $f2, 0x18($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X18);
    // 0x800D1C44: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1C48: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1C4C: lwc1        $f4, 0x18($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X18);
    // 0x800D1C50: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1C54: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1C58: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1C5C: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D1C60: lwc1        $f4, 0x18($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X18);
    // 0x800D1C64: lwc1        $f2, 0x18($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X18);
    // 0x800D1C68: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D1C6C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1C70: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D1C74: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D1C78: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D1C7C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1C80: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D1C84: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D1C88: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D1C8C: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D1C90: lwc1        $f2, 0x1C($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X1C);
    // 0x800D1C94: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1C98: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1C9C: lwc1        $f4, 0x1C($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x800D1CA0: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1CA4: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1CA8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1CAC: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1CB0: lwc1        $f4, 0x1C($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X1C);
    // 0x800D1CB4: lwc1        $f2, 0x1C($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x800D1CB8: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D1CBC: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1CC0: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D1CC4: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D1CC8: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D1CCC: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1CD0: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D1CD4: ori         $a0, $a0, 0xA0
    ctx->r4 = ctx->r4 | 0XA0;
    // 0x800D1CD8: addiu       $a1, $zero, 0x24
    ctx->r5 = ADD32(0, 0X24);
    // 0x800D1CDC: jal         0x80167848
    // 0x800D1CE0: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D1CE0: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D1CE4: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D1CE8: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D1CEC: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D1CF0: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D1CF4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1CF8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1CFC: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D1D00: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1D04: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1D08: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1D0C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D1D10: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D1D14: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D1D18: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D1D1C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1D20: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D1D24: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D1D28: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D1D2C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1D30: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D1D34: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D1D38: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1D3C: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D1D40: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D1D44: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1D48: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1D4C: lwc1        $f4, 0x10($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D1D50: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1D54: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1D58: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1D5C: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D1D60: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D1D64: lwc1        $f2, 0x10($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D1D68: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D1D6C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1D70: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D1D74: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D1D78: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D1D7C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1D80: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D1D84: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D1D88: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D1D8C: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D1D90: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D1D94: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1D98: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1D9C: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D1DA0: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1DA4: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1DA8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1DAC: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1DB0: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D1DB4: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D1DB8: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D1DBC: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1DC0: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D1DC4: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D1DC8: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D1DCC: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D1DD0: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1DD4: ori         $a0, $a0, 0xDC
    ctx->r4 = ctx->r4 | 0XDC;
    // 0x800D1DD8: addiu       $a1, $zero, 0x28
    ctx->r5 = ADD32(0, 0X28);
    // 0x800D1DDC: swc1        $f28, 0x34($gp)
    MEM_W(0X34, ctx->r28) = ctx->f28.u32l;
    // 0x800D1DE0: jal         0x80167848
    // 0x800D1DE4: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_2;
    // 0x800D1DE4: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_2:
    // 0x800D1DE8: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D1DEC: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D1DF0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D1DF4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D1DF8: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D1DFC:
    // 0x800D1DFC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D1E00: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D1E04: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D1E08: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D1E0C: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D1E10: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D1E14: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D1E18: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D1E1C: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D1E20: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D1E24: jr          $ra
    // 0x800D1E28: nop

    return;
    // 0x800D1E28: nop

;}
RECOMP_FUNC void func_800D1E2C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D1E2C: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D1E30: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D1E34: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1E38: lwc1        $f6, -0xB3C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB3C);
    // 0x800D1E3C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D1E40: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D1E44: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D1E48: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D1E4C: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D1E50: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D1E54: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D1E58: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D1E5C: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D1E60: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D1E64: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D1E68: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D1E6C: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D1E70: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D1E74: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D1E78: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D1E7C: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D1E80: bc1f        L_800D1ED0
    if (!c1cs) {
        // 0x800D1E84: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D1ED0;
    }
    // 0x800D1E84: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D1E88: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D1E8C: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D1E90: nop

    // 0x800D1E94: bc1f        L_800D1EA8
    if (!c1cs) {
        // 0x800D1E98: addu        $s2, $a0, $zero
        ctx->r18 = ADD32(ctx->r4, 0);
            goto L_800D1EA8;
    }
    // 0x800D1E98: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D1E9C: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
    // 0x800D1EA0: j           L_800D1F1C
    // 0x800D1EA4: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D1F1C;
    // 0x800D1EA4: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D1EA8:
    // 0x800D1EA8: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D1EAC: nop

    // 0x800D1EB0: bc1f        L_800D1EC0
    if (!c1cs) {
        // 0x800D1EB4: addu        $s1, $a1, $zero
        ctx->r17 = ADD32(ctx->r5, 0);
            goto L_800D1EC0;
    }
    // 0x800D1EB4: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D1EB8: j           L_800D1F14
    // 0x800D1EBC: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D1F14;
    // 0x800D1EBC: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D1EC0:
    // 0x800D1EC0: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D1EC4: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D1EC8: j           L_800D1F1C
    // 0x800D1ECC: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D1F1C;
    // 0x800D1ECC: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D1ED0:
    // 0x800D1ED0: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D1ED4: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D1ED8: nop

    // 0x800D1EDC: bc1f        L_800D1EF0
    if (!c1cs) {
        // 0x800D1EE0: addu        $s2, $v1, $zero
        ctx->r18 = ADD32(ctx->r3, 0);
            goto L_800D1EF0;
    }
    // 0x800D1EE0: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D1EE4: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D1EE8: j           L_800D1F14
    // 0x800D1EEC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D1F14;
    // 0x800D1EEC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D1EF0:
    // 0x800D1EF0: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D1EF4: nop

    // 0x800D1EF8: bc1f        L_800D1F08
    if (!c1cs) {
        // 0x800D1EFC: addu        $s1, $a1, $zero
        ctx->r17 = ADD32(ctx->r5, 0);
            goto L_800D1F08;
    }
    // 0x800D1EFC: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D1F00: j           L_800D1F1C
    // 0x800D1F04: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
        goto L_800D1F1C;
    // 0x800D1F04: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D1F08:
    // 0x800D1F08: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D1F0C: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
    // 0x800D1F10: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D1F14:
    // 0x800D1F14: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1F18: lwc1        $f6, -0xB38($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB38);
L_800D1F1C:
    // 0x800D1F1C: lwc1        $f8, 0x0($s2)
    ctx->f8.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D1F20: lwc1        $f4, 0x0($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D1F24: sub.s       $f28, $f8, $f4
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f28.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x800D1F28: lwc1        $f2, 0x4($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D1F2C: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D1F30: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1F34: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D1F38: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1F3C: mul.s       $f2, $f28, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f28.fl, ctx->f20.fl);
    // 0x800D1F40: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D1F44: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D1F48: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D1F4C: sub.s       $f26, $f2, $f0
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f26.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D1F50: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x800D1F54: c.eq.s      $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f26.fl == ctx->f2.fl;
    // 0x800D1F58: nop

    // 0x800D1F5C: bc1t        L_800D2044
    if (c1cs) {
        // 0x800D1F60: nop
    
            goto L_800D2044;
    }
    // 0x800D1F60: nop

    // 0x800D1F64: mul.s       $f0, $f26, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f26.fl, ctx->f6.fl);
    // 0x800D1F68: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x800D1F6C: nop

    // 0x800D1F70: bc1t        L_800D2044
    if (c1cs) {
        // 0x800D1F74: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D2044;
    }
    // 0x800D1F74: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D1F78: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D1F7C: lwc1        $f2, -0xB34($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB34);
    // 0x800D1F80: div.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D1F84: mul.s       $f28, $f28, $f2
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f28.fl = MUL_S(ctx->f28.fl, ctx->f2.fl);
    // 0x800D1F88: swc1        $f8, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f8.u32l;
    // 0x800D1F8C: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D1F90: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D1F94: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D1F98: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D1F9C: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D1FA0: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D1FA4: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D1FA8: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D1FAC: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D1FB0: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D1FB4: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D1FB8: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D1FBC: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D1FC0: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D1FC4: jal         0x80167848
    // 0x800D1FC8: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D1FC8: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D1FCC: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D1FD0: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D1FD4: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D1FD8: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D1FDC: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1FE0: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D1FE4: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D1FE8: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D1FEC: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D1FF0: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D1FF4: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D1FF8: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D1FFC: lwc1        $f2, 0x8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D2000: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D2004: mul.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D2008: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D200C: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2010: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D2014: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2018: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D201C: ori         $a0, $a0, 0xF4
    ctx->r4 = ctx->r4 | 0XF4;
    // 0x800D2020: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x800D2024: swc1        $f26, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f26.u32l;
    // 0x800D2028: jal         0x80167848
    // 0x800D202C: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D202C: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D2030: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D2034: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D2038: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D203C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D2040: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D2044:
    // 0x800D2044: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D2048: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D204C: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D2050: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D2054: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D2058: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D205C: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D2060: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D2064: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D2068: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D206C: jr          $ra
    // 0x800D2070: nop

    return;
    // 0x800D2070: nop

;}
RECOMP_FUNC void func_800D2074(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D2074: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D2078: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D207C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D2080: lwc1        $f6, -0xB30($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB30);
    // 0x800D2084: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D2088: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D208C: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D2090: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D2094: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D2098: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D209C: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D20A0: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D20A4: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D20A8: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D20AC: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D20B0: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D20B4: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D20B8: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D20BC: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D20C0: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D20C4: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D20C8: bc1f        L_800D2118
    if (!c1cs) {
        // 0x800D20CC: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D2118;
    }
    // 0x800D20CC: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D20D0: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D20D4: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D20D8: nop

    // 0x800D20DC: bc1f        L_800D20F0
    if (!c1cs) {
        // 0x800D20E0: addu        $s2, $a0, $zero
        ctx->r18 = ADD32(ctx->r4, 0);
            goto L_800D20F0;
    }
    // 0x800D20E0: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D20E4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
    // 0x800D20E8: j           L_800D2164
    // 0x800D20EC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D2164;
    // 0x800D20EC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D20F0:
    // 0x800D20F0: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D20F4: nop

    // 0x800D20F8: bc1f        L_800D2108
    if (!c1cs) {
        // 0x800D20FC: addu        $s1, $a1, $zero
        ctx->r17 = ADD32(ctx->r5, 0);
            goto L_800D2108;
    }
    // 0x800D20FC: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D2100: j           L_800D215C
    // 0x800D2104: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D215C;
    // 0x800D2104: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D2108:
    // 0x800D2108: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D210C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D2110: j           L_800D2164
    // 0x800D2114: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D2164;
    // 0x800D2114: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D2118:
    // 0x800D2118: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D211C: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D2120: nop

    // 0x800D2124: bc1f        L_800D2138
    if (!c1cs) {
        // 0x800D2128: addu        $s2, $v1, $zero
        ctx->r18 = ADD32(ctx->r3, 0);
            goto L_800D2138;
    }
    // 0x800D2128: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D212C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D2130: j           L_800D215C
    // 0x800D2134: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D215C;
    // 0x800D2134: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D2138:
    // 0x800D2138: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D213C: nop

    // 0x800D2140: bc1f        L_800D2150
    if (!c1cs) {
        // 0x800D2144: addu        $s1, $a1, $zero
        ctx->r17 = ADD32(ctx->r5, 0);
            goto L_800D2150;
    }
    // 0x800D2144: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D2148: j           L_800D2164
    // 0x800D214C: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
        goto L_800D2164;
    // 0x800D214C: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D2150:
    // 0x800D2150: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D2154: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
    // 0x800D2158: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D215C:
    // 0x800D215C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D2160: lwc1        $f6, -0xB2C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB2C);
L_800D2164:
    // 0x800D2164: lwc1        $f8, 0x0($s2)
    ctx->f8.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D2168: lwc1        $f4, 0x0($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D216C: sub.s       $f26, $f8, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x800D2170: lwc1        $f2, 0x4($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D2174: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2178: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D217C: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D2180: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2184: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D2188: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D218C: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D2190: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D2194: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2198: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x800D219C: c.eq.s      $f28, $f2
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f28.fl == ctx->f2.fl;
    // 0x800D21A0: nop

    // 0x800D21A4: bc1t        L_800D22EC
    if (c1cs) {
        // 0x800D21A8: nop
    
            goto L_800D22EC;
    }
    // 0x800D21A8: nop

    // 0x800D21AC: mul.s       $f0, $f28, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f28.fl, ctx->f6.fl);
    // 0x800D21B0: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x800D21B4: nop

    // 0x800D21B8: bc1t        L_800D22EC
    if (c1cs) {
        // 0x800D21BC: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D22EC;
    }
    // 0x800D21BC: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D21C0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D21C4: lwc1        $f2, -0xB28($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB28);
    // 0x800D21C8: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D21CC: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D21D0: swc1        $f8, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f8.u32l;
    // 0x800D21D4: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D21D8: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D21DC: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D21E0: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D21E4: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D21E8: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D21EC: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D21F0: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D21F4: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D21F8: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D21FC: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D2200: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2204: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D2208: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D220C: jal         0x80167848
    // 0x800D2210: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D2210: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D2214: lwc1        $f0, 0xC($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D2218: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D221C: lwc1        $f0, 0xC($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D2220: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D2224: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2228: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D222C: lwc1        $f4, 0xC($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2230: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2234: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2238: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D223C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2240: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D2244: lwc1        $f2, 0xC($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2248: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D224C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2250: lwc1        $f0, 0xC($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D2254: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2258: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D225C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2260: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2264: ori         $a0, $a0, 0xD0
    ctx->r4 = ctx->r4 | 0XD0;
    // 0x800D2268: addiu       $a1, $zero, 0xC
    ctx->r5 = ADD32(0, 0XC);
    // 0x800D226C: jal         0x80167848
    // 0x800D2270: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D2270: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D2274: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2278: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D227C: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2280: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D2284: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2288: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D228C: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D2290: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2294: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2298: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D229C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D22A0: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D22A4: lwc1        $f2, 0x8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D22A8: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D22AC: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D22B0: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D22B4: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D22B8: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D22BC: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D22C0: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D22C4: ori         $a0, $a0, 0xF4
    ctx->r4 = ctx->r4 | 0XF4;
    // 0x800D22C8: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x800D22CC: swc1        $f28, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f28.u32l;
    // 0x800D22D0: jal         0x80167848
    // 0x800D22D4: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_2;
    // 0x800D22D4: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    after_2:
    // 0x800D22D8: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D22DC: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D22E0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D22E4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D22E8: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D22EC:
    // 0x800D22EC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D22F0: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D22F4: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D22F8: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D22FC: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D2300: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D2304: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D2308: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D230C: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D2310: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D2314: jr          $ra
    // 0x800D2318: nop

    return;
    // 0x800D2318: nop

;}
RECOMP_FUNC void func_800D231C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D231C: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D2320: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D2324: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D2328: lwc1        $f6, -0xB24($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB24);
    // 0x800D232C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D2330: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D2334: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D2338: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D233C: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D2340: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D2344: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D2348: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D234C: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D2350: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D2354: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D2358: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D235C: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D2360: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D2364: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D2368: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D236C: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D2370: bc1f        L_800D23C0
    if (!c1cs) {
        // 0x800D2374: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D23C0;
    }
    // 0x800D2374: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D2378: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D237C: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D2380: nop

    // 0x800D2384: bc1f        L_800D2398
    if (!c1cs) {
        // 0x800D2388: addu        $s0, $a0, $zero
        ctx->r16 = ADD32(ctx->r4, 0);
            goto L_800D2398;
    }
    // 0x800D2388: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D238C: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D2390: j           L_800D240C
    // 0x800D2394: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D240C;
    // 0x800D2394: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D2398:
    // 0x800D2398: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D239C: nop

    // 0x800D23A0: bc1f        L_800D23B0
    if (!c1cs) {
        // 0x800D23A4: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D23B0;
    }
    // 0x800D23A4: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D23A8: j           L_800D2404
    // 0x800D23AC: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D2404;
    // 0x800D23AC: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D23B0:
    // 0x800D23B0: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D23B4: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D23B8: j           L_800D240C
    // 0x800D23BC: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D240C;
    // 0x800D23BC: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D23C0:
    // 0x800D23C0: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D23C4: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D23C8: nop

    // 0x800D23CC: bc1f        L_800D23E0
    if (!c1cs) {
        // 0x800D23D0: addu        $s0, $v1, $zero
        ctx->r16 = ADD32(ctx->r3, 0);
            goto L_800D23E0;
    }
    // 0x800D23D0: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
    // 0x800D23D4: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D23D8: j           L_800D2404
    // 0x800D23DC: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D2404;
    // 0x800D23DC: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D23E0:
    // 0x800D23E0: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D23E4: nop

    // 0x800D23E8: bc1f        L_800D23F8
    if (!c1cs) {
        // 0x800D23EC: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D23F8;
    }
    // 0x800D23EC: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D23F0: j           L_800D240C
    // 0x800D23F4: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
        goto L_800D240C;
    // 0x800D23F4: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D23F8:
    // 0x800D23F8: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D23FC: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D2400: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D2404:
    // 0x800D2404: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D2408: lwc1        $f6, -0xB20($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB20);
L_800D240C:
    // 0x800D240C: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D2410: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D2414: sub.s       $f26, $f8, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x800D2418: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D241C: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D2420: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2424: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2428: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D242C: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D2430: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D2434: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D2438: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D243C: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2440: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x800D2444: c.eq.s      $f28, $f2
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f28.fl == ctx->f2.fl;
    // 0x800D2448: nop

    // 0x800D244C: bc1t        L_800D2624
    if (c1cs) {
        // 0x800D2450: nop
    
            goto L_800D2624;
    }
    // 0x800D2450: nop

    // 0x800D2454: mul.s       $f0, $f28, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f28.fl, ctx->f6.fl);
    // 0x800D2458: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x800D245C: nop

    // 0x800D2460: bc1t        L_800D2624
    if (c1cs) {
        // 0x800D2464: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D2624;
    }
    // 0x800D2464: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2468: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D246C: lwc1        $f2, -0xB1C($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB1C);
    // 0x800D2470: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D2474: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D2478: swc1        $f8, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f8.u32l;
    // 0x800D247C: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D2480: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2484: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2488: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D248C: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D2490: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D2494: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D2498: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D249C: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D24A0: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D24A4: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D24A8: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D24AC: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D24B0: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D24B4: jal         0x80167848
    // 0x800D24B8: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D24B8: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D24BC: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D24C0: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D24C4: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D24C8: lwc1        $f2, 0x14($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D24CC: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D24D0: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D24D4: lwc1        $f4, 0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D24D8: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D24DC: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D24E0: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D24E4: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D24E8: lwc1        $f4, 0x14($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D24EC: lwc1        $f2, 0x14($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D24F0: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D24F4: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D24F8: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D24FC: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2500: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D2504: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2508: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D250C: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2510: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D2514: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2518: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D251C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2520: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D2524: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D2528: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D252C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2530: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2534: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D2538: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D253C: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D2540: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D2544: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2548: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D254C: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2550: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D2554: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2558: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D255C: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D2560: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D2564: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D2568: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D256C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2570: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D2574: lwc1        $f4, 0x10($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D2578: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D257C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2580: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2584: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D2588: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D258C: lwc1        $f2, 0x10($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D2590: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D2594: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2598: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D259C: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D25A0: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D25A4: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D25A8: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    // 0x800D25AC: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D25B0: swc1        $f0, 0x34($gp)
    MEM_W(0X34, ctx->r28) = ctx->f0.u32l;
    // 0x800D25B4: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D25B8: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D25BC: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D25C0: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D25C4: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D25C8: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D25CC: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D25D0: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D25D4: swc1        $f0, 0x38($gp)
    MEM_W(0X38, ctx->r28) = ctx->f0.u32l;
    // 0x800D25D8: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D25DC: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D25E0: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D25E4: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D25E8: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D25EC: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D25F0: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D25F4: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D25F8: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D25FC: ori         $a0, $a0, 0xD0
    ctx->r4 = ctx->r4 | 0XD0;
    // 0x800D2600: addiu       $a1, $zero, 0x34
    ctx->r5 = ADD32(0, 0X34);
    // 0x800D2604: swc1        $f28, 0x40($gp)
    MEM_W(0X40, ctx->r28) = ctx->f28.u32l;
    // 0x800D2608: jal         0x80167848
    // 0x800D260C: swc1        $f2, 0x3C($gp)
    MEM_W(0X3C, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D260C: swc1        $f2, 0x3C($gp)
    MEM_W(0X3C, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D2610: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D2614: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D2618: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D261C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D2620: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D2624:
    // 0x800D2624: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D2628: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D262C: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D2630: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D2634: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D2638: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D263C: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D2640: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D2644: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D2648: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D264C: jr          $ra
    // 0x800D2650: nop

    return;
    // 0x800D2650: nop

;}
RECOMP_FUNC void func_800D2654(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D2654: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D2658: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D265C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D2660: lwc1        $f6, -0xB18($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB18);
    // 0x800D2664: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D2668: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D266C: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D2670: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D2674: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D2678: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D267C: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D2680: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D2684: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D2688: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D268C: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D2690: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D2694: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D2698: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D269C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D26A0: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D26A4: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D26A8: bc1f        L_800D26F8
    if (!c1cs) {
        // 0x800D26AC: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D26F8;
    }
    // 0x800D26AC: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D26B0: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D26B4: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D26B8: nop

    // 0x800D26BC: bc1f        L_800D26D0
    if (!c1cs) {
        // 0x800D26C0: addu        $s0, $a0, $zero
        ctx->r16 = ADD32(ctx->r4, 0);
            goto L_800D26D0;
    }
    // 0x800D26C0: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D26C4: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D26C8: j           L_800D2744
    // 0x800D26CC: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D2744;
    // 0x800D26CC: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D26D0:
    // 0x800D26D0: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D26D4: nop

    // 0x800D26D8: bc1f        L_800D26E8
    if (!c1cs) {
        // 0x800D26DC: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D26E8;
    }
    // 0x800D26DC: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D26E0: j           L_800D273C
    // 0x800D26E4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D273C;
    // 0x800D26E4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D26E8:
    // 0x800D26E8: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D26EC: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D26F0: j           L_800D2744
    // 0x800D26F4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D2744;
    // 0x800D26F4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D26F8:
    // 0x800D26F8: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D26FC: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D2700: nop

    // 0x800D2704: bc1f        L_800D2718
    if (!c1cs) {
        // 0x800D2708: addu        $s0, $v1, $zero
        ctx->r16 = ADD32(ctx->r3, 0);
            goto L_800D2718;
    }
    // 0x800D2708: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
    // 0x800D270C: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D2710: j           L_800D273C
    // 0x800D2714: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D273C;
    // 0x800D2714: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D2718:
    // 0x800D2718: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D271C: nop

    // 0x800D2720: bc1f        L_800D2730
    if (!c1cs) {
        // 0x800D2724: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D2730;
    }
    // 0x800D2724: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D2728: j           L_800D2744
    // 0x800D272C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
        goto L_800D2744;
    // 0x800D272C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D2730:
    // 0x800D2730: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D2734: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D2738: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D273C:
    // 0x800D273C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D2740: lwc1        $f6, -0xB14($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0XB14);
L_800D2744:
    // 0x800D2744: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D2748: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D274C: sub.s       $f26, $f8, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x800D2750: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D2754: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D2758: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D275C: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2760: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2764: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D2768: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D276C: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D2770: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D2774: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2778: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x800D277C: c.eq.s      $f28, $f2
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f28.fl == ctx->f2.fl;
    // 0x800D2780: nop

    // 0x800D2784: bc1t        L_800D2A5C
    if (c1cs) {
        // 0x800D2788: nop
    
            goto L_800D2A5C;
    }
    // 0x800D2788: nop

    // 0x800D278C: mul.s       $f0, $f28, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f28.fl, ctx->f6.fl);
    // 0x800D2790: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x800D2794: nop

    // 0x800D2798: bc1t        L_800D2A5C
    if (c1cs) {
        // 0x800D279C: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D2A5C;
    }
    // 0x800D279C: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D27A0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D27A4: lwc1        $f2, -0xB10($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB10);
    // 0x800D27A8: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D27AC: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D27B0: swc1        $f8, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f8.u32l;
    // 0x800D27B4: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D27B8: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D27BC: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D27C0: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D27C4: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D27C8: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D27CC: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D27D0: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D27D4: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D27D8: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D27DC: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D27E0: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D27E4: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D27E8: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D27EC: jal         0x80167848
    // 0x800D27F0: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D27F0: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D27F4: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D27F8: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D27FC: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D2800: lwc1        $f2, 0x18($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X18);
    // 0x800D2804: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2808: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D280C: lwc1        $f4, 0x18($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X18);
    // 0x800D2810: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2814: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2818: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D281C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2820: lwc1        $f4, 0x18($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X18);
    // 0x800D2824: lwc1        $f2, 0x18($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X18);
    // 0x800D2828: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D282C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2830: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D2834: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2838: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D283C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2840: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D2844: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D2848: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D284C: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D2850: lwc1        $f2, 0x1C($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X1C);
    // 0x800D2854: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2858: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D285C: lwc1        $f4, 0x1C($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x800D2860: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2864: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2868: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D286C: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D2870: lwc1        $f4, 0x1C($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X1C);
    // 0x800D2874: lwc1        $f2, 0x1C($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x800D2878: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D287C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2880: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D2884: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2888: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D288C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2890: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D2894: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x800D2898: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D289C: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x800D28A0: lwc1        $f2, 0x20($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X20);
    // 0x800D28A4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D28A8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D28AC: lwc1        $f4, 0x20($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X20);
    // 0x800D28B0: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D28B4: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D28B8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D28BC: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D28C0: lwc1        $f4, 0x20($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X20);
    // 0x800D28C4: lwc1        $f2, 0x20($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X20);
    // 0x800D28C8: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D28CC: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D28D0: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x800D28D4: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D28D8: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D28DC: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D28E0: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D28E4: ori         $a0, $a0, 0xA0
    ctx->r4 = ctx->r4 | 0XA0;
    // 0x800D28E8: addiu       $a1, $zero, 0x24
    ctx->r5 = ADD32(0, 0X24);
    // 0x800D28EC: jal         0x80167848
    // 0x800D28F0: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D28F0: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D28F4: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D28F8: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D28FC: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D2900: lwc1        $f2, 0x14($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D2904: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2908: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D290C: lwc1        $f4, 0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D2910: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2914: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2918: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D291C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2920: lwc1        $f4, 0x14($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D2924: lwc1        $f2, 0x14($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D2928: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D292C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2930: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D2934: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2938: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D293C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2940: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D2944: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2948: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D294C: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2950: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D2954: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2958: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D295C: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D2960: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2964: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2968: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D296C: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D2970: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D2974: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D2978: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D297C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2980: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2984: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2988: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D298C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2990: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D2994: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D2998: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D299C: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D29A0: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D29A4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D29A8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D29AC: lwc1        $f4, 0x10($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D29B0: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D29B4: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D29B8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D29BC: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D29C0: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D29C4: lwc1        $f2, 0x10($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D29C8: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D29CC: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D29D0: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D29D4: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D29D8: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D29DC: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D29E0: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    // 0x800D29E4: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D29E8: swc1        $f0, 0x34($gp)
    MEM_W(0X34, ctx->r28) = ctx->f0.u32l;
    // 0x800D29EC: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D29F0: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D29F4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D29F8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D29FC: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D2A00: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2A04: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2A08: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2A0C: swc1        $f0, 0x38($gp)
    MEM_W(0X38, ctx->r28) = ctx->f0.u32l;
    // 0x800D2A10: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2A14: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D2A18: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D2A1C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2A20: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D2A24: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2A28: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D2A2C: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2A30: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2A34: ori         $a0, $a0, 0xD0
    ctx->r4 = ctx->r4 | 0XD0;
    // 0x800D2A38: addiu       $a1, $zero, 0x34
    ctx->r5 = ADD32(0, 0X34);
    // 0x800D2A3C: swc1        $f28, 0x40($gp)
    MEM_W(0X40, ctx->r28) = ctx->f28.u32l;
    // 0x800D2A40: jal         0x80167848
    // 0x800D2A44: swc1        $f2, 0x3C($gp)
    MEM_W(0X3C, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_2;
    // 0x800D2A44: swc1        $f2, 0x3C($gp)
    MEM_W(0X3C, ctx->r28) = ctx->f2.u32l;
    after_2:
    // 0x800D2A48: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D2A4C: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D2A50: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D2A54: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D2A58: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D2A5C:
    // 0x800D2A5C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D2A60: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D2A64: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D2A68: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D2A6C: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D2A70: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D2A74: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D2A78: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D2A7C: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D2A80: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D2A84: jr          $ra
    // 0x800D2A88: nop

    return;
    // 0x800D2A88: nop

;}
RECOMP_FUNC void func_800D2A8C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D2A8C: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D2A90: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D2A94: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D2A98: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D2A9C: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D2AA0: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D2AA4: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D2AA8: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D2AAC: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D2AB0: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D2AB4: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D2AB8: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D2ABC: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D2AC0: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D2AC4: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D2AC8: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D2ACC: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D2AD0: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D2AD4: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D2AD8: bc1f        L_800D2B28
    if (!c1cs) {
        // 0x800D2ADC: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D2B28;
    }
    // 0x800D2ADC: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D2AE0: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D2AE4: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D2AE8: nop

    // 0x800D2AEC: bc1f        L_800D2B00
    if (!c1cs) {
        // 0x800D2AF0: addu        $s2, $a0, $zero
        ctx->r18 = ADD32(ctx->r4, 0);
            goto L_800D2B00;
    }
    // 0x800D2AF0: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D2AF4: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
    // 0x800D2AF8: j           L_800D2B64
    // 0x800D2AFC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D2B64;
    // 0x800D2AFC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D2B00:
    // 0x800D2B00: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D2B04: nop

    // 0x800D2B08: bc1f        L_800D2B18
    if (!c1cs) {
        // 0x800D2B0C: addu        $s1, $a1, $zero
        ctx->r17 = ADD32(ctx->r5, 0);
            goto L_800D2B18;
    }
    // 0x800D2B0C: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D2B10: j           L_800D2B64
    // 0x800D2B14: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D2B64;
    // 0x800D2B14: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D2B18:
    // 0x800D2B18: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D2B1C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D2B20: j           L_800D2B64
    // 0x800D2B24: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D2B64;
    // 0x800D2B24: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D2B28:
    // 0x800D2B28: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D2B2C: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D2B30: nop

    // 0x800D2B34: bc1f        L_800D2B48
    if (!c1cs) {
        // 0x800D2B38: addu        $s2, $v1, $zero
        ctx->r18 = ADD32(ctx->r3, 0);
            goto L_800D2B48;
    }
    // 0x800D2B38: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D2B3C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D2B40: j           L_800D2B64
    // 0x800D2B44: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D2B64;
    // 0x800D2B44: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D2B48:
    // 0x800D2B48: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D2B4C: nop

    // 0x800D2B50: bc1t        L_800D2B60
    if (c1cs) {
        // 0x800D2B54: addu        $s1, $a1, $zero
        ctx->r17 = ADD32(ctx->r5, 0);
            goto L_800D2B60;
    }
    // 0x800D2B54: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D2B58: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D2B5C: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D2B60:
    // 0x800D2B60: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D2B64:
    // 0x800D2B64: lwc1        $f6, 0x0($s2)
    ctx->f6.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D2B68: lwc1        $f4, 0x0($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D2B6C: sub.s       $f26, $f6, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x800D2B70: lwc1        $f2, 0x4($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D2B74: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2B78: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2B7C: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D2B80: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2B84: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D2B88: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D2B8C: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D2B90: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D2B94: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2B98: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800D2B9C: c.eq.s      $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f28.fl == ctx->f0.fl;
    // 0x800D2BA0: nop

    // 0x800D2BA4: bc1t        L_800D2C78
    if (c1cs) {
        // 0x800D2BA8: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D2C78;
    }
    // 0x800D2BA8: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2BAC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D2BB0: lwc1        $f2, -0xB0C($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB0C);
    // 0x800D2BB4: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D2BB8: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D2BBC: swc1        $f6, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f6.u32l;
    // 0x800D2BC0: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D2BC4: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D2BC8: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2BCC: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D2BD0: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D2BD4: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D2BD8: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D2BDC: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D2BE0: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D2BE4: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D2BE8: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D2BEC: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2BF0: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D2BF4: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D2BF8: jal         0x80167848
    // 0x800D2BFC: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D2BFC: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D2C00: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2C04: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D2C08: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2C0C: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D2C10: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2C14: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D2C18: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D2C1C: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2C20: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2C24: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2C28: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2C2C: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D2C30: lwc1        $f2, 0x8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D2C34: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D2C38: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2C3C: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2C40: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2C44: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D2C48: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2C4C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2C50: ori         $a0, $a0, 0xF4
    ctx->r4 = ctx->r4 | 0XF4;
    // 0x800D2C54: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x800D2C58: swc1        $f28, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f28.u32l;
    // 0x800D2C5C: jal         0x80167848
    // 0x800D2C60: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D2C60: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D2C64: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D2C68: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D2C6C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D2C70: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D2C74: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D2C78:
    // 0x800D2C78: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D2C7C: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D2C80: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D2C84: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D2C88: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D2C8C: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D2C90: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D2C94: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D2C98: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D2C9C: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D2CA0: jr          $ra
    // 0x800D2CA4: nop

    return;
    // 0x800D2CA4: nop

;}
RECOMP_FUNC void func_800D2CA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D2CA8: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D2CAC: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D2CB0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D2CB4: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D2CB8: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D2CBC: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D2CC0: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D2CC4: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D2CC8: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D2CCC: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D2CD0: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D2CD4: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D2CD8: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D2CDC: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D2CE0: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D2CE4: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D2CE8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D2CEC: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D2CF0: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D2CF4: bc1f        L_800D2D44
    if (!c1cs) {
        // 0x800D2CF8: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D2D44;
    }
    // 0x800D2CF8: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D2CFC: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D2D00: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D2D04: nop

    // 0x800D2D08: bc1f        L_800D2D1C
    if (!c1cs) {
        // 0x800D2D0C: addu        $s2, $a0, $zero
        ctx->r18 = ADD32(ctx->r4, 0);
            goto L_800D2D1C;
    }
    // 0x800D2D0C: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D2D10: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
    // 0x800D2D14: j           L_800D2D80
    // 0x800D2D18: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D2D80;
    // 0x800D2D18: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D2D1C:
    // 0x800D2D1C: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D2D20: nop

    // 0x800D2D24: bc1f        L_800D2D34
    if (!c1cs) {
        // 0x800D2D28: addu        $s1, $a1, $zero
        ctx->r17 = ADD32(ctx->r5, 0);
            goto L_800D2D34;
    }
    // 0x800D2D28: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D2D2C: j           L_800D2D80
    // 0x800D2D30: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D2D80;
    // 0x800D2D30: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D2D34:
    // 0x800D2D34: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D2D38: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D2D3C: j           L_800D2D80
    // 0x800D2D40: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D2D80;
    // 0x800D2D40: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D2D44:
    // 0x800D2D44: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D2D48: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D2D4C: nop

    // 0x800D2D50: bc1f        L_800D2D64
    if (!c1cs) {
        // 0x800D2D54: addu        $s2, $v1, $zero
        ctx->r18 = ADD32(ctx->r3, 0);
            goto L_800D2D64;
    }
    // 0x800D2D54: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D2D58: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D2D5C: j           L_800D2D80
    // 0x800D2D60: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D2D80;
    // 0x800D2D60: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D2D64:
    // 0x800D2D64: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D2D68: nop

    // 0x800D2D6C: bc1t        L_800D2D7C
    if (c1cs) {
        // 0x800D2D70: addu        $s1, $a1, $zero
        ctx->r17 = ADD32(ctx->r5, 0);
            goto L_800D2D7C;
    }
    // 0x800D2D70: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D2D74: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D2D78: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D2D7C:
    // 0x800D2D7C: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D2D80:
    // 0x800D2D80: lwc1        $f6, 0x0($s2)
    ctx->f6.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D2D84: lwc1        $f4, 0x0($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D2D88: sub.s       $f26, $f6, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x800D2D8C: lwc1        $f2, 0x4($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D2D90: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2D94: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2D98: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D2D9C: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2DA0: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D2DA4: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D2DA8: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D2DAC: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D2DB0: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2DB4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800D2DB8: c.eq.s      $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f28.fl == ctx->f0.fl;
    // 0x800D2DBC: nop

    // 0x800D2DC0: bc1t        L_800D2EF4
    if (c1cs) {
        // 0x800D2DC4: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D2EF4;
    }
    // 0x800D2DC4: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2DC8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D2DCC: lwc1        $f2, -0xB08($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB08);
    // 0x800D2DD0: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D2DD4: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D2DD8: swc1        $f6, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f6.u32l;
    // 0x800D2DDC: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D2DE0: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D2DE4: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2DE8: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D2DEC: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D2DF0: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D2DF4: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D2DF8: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D2DFC: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D2E00: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D2E04: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D2E08: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D2E0C: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D2E10: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D2E14: jal         0x80167848
    // 0x800D2E18: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D2E18: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D2E1C: lwc1        $f0, 0xC($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D2E20: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D2E24: lwc1        $f0, 0xC($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D2E28: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D2E2C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2E30: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D2E34: lwc1        $f4, 0xC($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2E38: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2E3C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2E40: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2E44: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2E48: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D2E4C: lwc1        $f2, 0xC($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D2E50: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D2E54: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2E58: lwc1        $f0, 0xC($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D2E5C: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2E60: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D2E64: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2E68: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2E6C: ori         $a0, $a0, 0xD0
    ctx->r4 = ctx->r4 | 0XD0;
    // 0x800D2E70: addiu       $a1, $zero, 0xC
    ctx->r5 = ADD32(0, 0XC);
    // 0x800D2E74: jal         0x80167848
    // 0x800D2E78: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D2E78: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D2E7C: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2E80: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D2E84: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2E88: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D2E8C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2E90: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D2E94: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D2E98: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D2E9C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D2EA0: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D2EA4: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D2EA8: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D2EAC: lwc1        $f2, 0x8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D2EB0: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D2EB4: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D2EB8: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D2EBC: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D2EC0: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D2EC4: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D2EC8: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D2ECC: ori         $a0, $a0, 0xF4
    ctx->r4 = ctx->r4 | 0XF4;
    // 0x800D2ED0: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x800D2ED4: swc1        $f28, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f28.u32l;
    // 0x800D2ED8: jal         0x80167848
    // 0x800D2EDC: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_2;
    // 0x800D2EDC: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    after_2:
    // 0x800D2EE0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D2EE4: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D2EE8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D2EEC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D2EF0: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D2EF4:
    // 0x800D2EF4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D2EF8: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D2EFC: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D2F00: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D2F04: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D2F08: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D2F0C: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D2F10: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D2F14: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D2F18: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D2F1C: jr          $ra
    // 0x800D2F20: nop

    return;
    // 0x800D2F20: nop

;}
RECOMP_FUNC void func_800D2F24(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D2F24: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D2F28: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D2F2C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D2F30: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D2F34: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D2F38: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D2F3C: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D2F40: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D2F44: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D2F48: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D2F4C: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D2F50: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D2F54: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D2F58: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D2F5C: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D2F60: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D2F64: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D2F68: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D2F6C: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D2F70: bc1f        L_800D2FC0
    if (!c1cs) {
        // 0x800D2F74: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D2FC0;
    }
    // 0x800D2F74: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D2F78: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D2F7C: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D2F80: nop

    // 0x800D2F84: bc1f        L_800D2F98
    if (!c1cs) {
        // 0x800D2F88: addu        $s1, $a0, $zero
        ctx->r17 = ADD32(ctx->r4, 0);
            goto L_800D2F98;
    }
    // 0x800D2F88: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D2F8C: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D2F90: j           L_800D2FFC
    // 0x800D2F94: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D2FFC;
    // 0x800D2F94: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D2F98:
    // 0x800D2F98: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D2F9C: nop

    // 0x800D2FA0: bc1f        L_800D2FB0
    if (!c1cs) {
        // 0x800D2FA4: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D2FB0;
    }
    // 0x800D2FA4: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D2FA8: j           L_800D2FFC
    // 0x800D2FAC: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D2FFC;
    // 0x800D2FAC: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D2FB0:
    // 0x800D2FB0: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D2FB4: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D2FB8: j           L_800D2FFC
    // 0x800D2FBC: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
        goto L_800D2FFC;
    // 0x800D2FBC: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
L_800D2FC0:
    // 0x800D2FC0: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D2FC4: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D2FC8: nop

    // 0x800D2FCC: bc1f        L_800D2FE0
    if (!c1cs) {
        // 0x800D2FD0: addu        $s1, $v1, $zero
        ctx->r17 = ADD32(ctx->r3, 0);
            goto L_800D2FE0;
    }
    // 0x800D2FD0: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
    // 0x800D2FD4: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D2FD8: j           L_800D2FFC
    // 0x800D2FDC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
        goto L_800D2FFC;
    // 0x800D2FDC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
L_800D2FE0:
    // 0x800D2FE0: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D2FE4: nop

    // 0x800D2FE8: bc1t        L_800D2FF8
    if (c1cs) {
        // 0x800D2FEC: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D2FF8;
    }
    // 0x800D2FEC: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D2FF0: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D2FF4: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
L_800D2FF8:
    // 0x800D2FF8: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
L_800D2FFC:
    // 0x800D2FFC: lwc1        $f6, 0x0($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D3000: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D3004: sub.s       $f26, $f6, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x800D3008: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D300C: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D3010: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3014: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D3018: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D301C: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D3020: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D3024: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D3028: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D302C: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3030: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800D3034: c.eq.s      $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f28.fl == ctx->f0.fl;
    // 0x800D3038: nop

    // 0x800D303C: bc1t        L_800D31B0
    if (c1cs) {
        // 0x800D3040: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D31B0;
    }
    // 0x800D3040: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D3044: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3048: lwc1        $f2, -0xB04($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB04);
    // 0x800D304C: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D3050: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D3054: swc1        $f6, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f6.u32l;
    // 0x800D3058: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D305C: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D3060: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D3064: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D3068: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D306C: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D3070: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D3074: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3078: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D307C: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D3080: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D3084: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D3088: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D308C: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D3090: jal         0x80167848
    // 0x800D3094: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D3094: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D3098: lwc1        $f0, 0xC($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D309C: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D30A0: lwc1        $f0, 0xC($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D30A4: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D30A8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D30AC: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D30B0: lwc1        $f4, 0xC($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D30B4: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D30B8: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D30BC: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D30C0: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D30C4: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D30C8: lwc1        $f2, 0xC($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D30CC: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D30D0: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D30D4: lwc1        $f0, 0xC($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D30D8: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D30DC: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D30E0: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D30E4: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D30E8: lwc1        $f0, 0x10($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D30EC: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D30F0: lwc1        $f0, 0x10($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D30F4: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D30F8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D30FC: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3100: lwc1        $f4, 0x10($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D3104: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3108: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D310C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3110: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D3114: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D3118: lwc1        $f2, 0x10($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D311C: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3120: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3124: lwc1        $f0, 0x10($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D3128: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D312C: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3130: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3134: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D3138: lwc1        $f0, 0x8($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D313C: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D3140: lwc1        $f0, 0x8($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D3144: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D3148: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D314C: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3150: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3154: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3158: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D315C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3160: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3164: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D3168: lwc1        $f2, 0x8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D316C: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3170: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3174: lwc1        $f0, 0x8($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D3178: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D317C: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3180: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D3184: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3188: ori         $a0, $a0, 0xDC
    ctx->r4 = ctx->r4 | 0XDC;
    // 0x800D318C: addiu       $a1, $zero, 0x28
    ctx->r5 = ADD32(0, 0X28);
    // 0x800D3190: swc1        $f28, 0x34($gp)
    MEM_W(0X34, ctx->r28) = ctx->f28.u32l;
    // 0x800D3194: jal         0x80167848
    // 0x800D3198: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D3198: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D319C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D31A0: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D31A4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D31A8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D31AC: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D31B0:
    // 0x800D31B0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D31B4: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D31B8: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D31BC: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D31C0: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D31C4: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D31C8: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D31CC: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D31D0: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D31D4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D31D8: jr          $ra
    // 0x800D31DC: nop

    return;
    // 0x800D31DC: nop

;}
RECOMP_FUNC void func_800D31E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D31E0: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D31E4: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D31E8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D31EC: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D31F0: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D31F4: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D31F8: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D31FC: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D3200: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D3204: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D3208: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D320C: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D3210: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D3214: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D3218: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D321C: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D3220: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D3224: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D3228: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D322C: bc1f        L_800D327C
    if (!c1cs) {
        // 0x800D3230: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D327C;
    }
    // 0x800D3230: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D3234: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D3238: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D323C: nop

    // 0x800D3240: bc1f        L_800D3254
    if (!c1cs) {
        // 0x800D3244: addu        $s0, $a0, $zero
        ctx->r16 = ADD32(ctx->r4, 0);
            goto L_800D3254;
    }
    // 0x800D3244: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D3248: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D324C: j           L_800D32B8
    // 0x800D3250: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D32B8;
    // 0x800D3250: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D3254:
    // 0x800D3254: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D3258: nop

    // 0x800D325C: bc1f        L_800D326C
    if (!c1cs) {
        // 0x800D3260: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D326C;
    }
    // 0x800D3260: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D3264: j           L_800D32B8
    // 0x800D3268: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D32B8;
    // 0x800D3268: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D326C:
    // 0x800D326C: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D3270: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D3274: j           L_800D32B8
    // 0x800D3278: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D32B8;
    // 0x800D3278: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D327C:
    // 0x800D327C: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D3280: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D3284: nop

    // 0x800D3288: bc1f        L_800D329C
    if (!c1cs) {
        // 0x800D328C: addu        $s0, $v1, $zero
        ctx->r16 = ADD32(ctx->r3, 0);
            goto L_800D329C;
    }
    // 0x800D328C: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
    // 0x800D3290: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D3294: j           L_800D32B8
    // 0x800D3298: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D32B8;
    // 0x800D3298: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D329C:
    // 0x800D329C: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D32A0: nop

    // 0x800D32A4: bc1t        L_800D32B4
    if (c1cs) {
        // 0x800D32A8: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D32B4;
    }
    // 0x800D32A8: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D32AC: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D32B0: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
L_800D32B4:
    // 0x800D32B4: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D32B8:
    // 0x800D32B8: lwc1        $f6, 0x0($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D32BC: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D32C0: sub.s       $f26, $f6, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x800D32C4: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D32C8: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D32CC: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D32D0: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D32D4: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D32D8: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D32DC: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D32E0: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D32E4: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D32E8: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D32EC: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800D32F0: c.eq.s      $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f28.fl == ctx->f0.fl;
    // 0x800D32F4: nop

    // 0x800D32F8: bc1t        L_800D34BC
    if (c1cs) {
        // 0x800D32FC: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D34BC;
    }
    // 0x800D32FC: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D3300: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3304: lwc1        $f2, -0xB00($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XB00);
    // 0x800D3308: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D330C: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D3310: swc1        $f6, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f6.u32l;
    // 0x800D3314: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D3318: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D331C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D3320: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D3324: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D3328: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D332C: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D3330: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3334: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D3338: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D333C: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D3340: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D3344: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D3348: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D334C: jal         0x80167848
    // 0x800D3350: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D3350: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D3354: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D3358: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D335C: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D3360: lwc1        $f2, 0x14($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D3364: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3368: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D336C: lwc1        $f4, 0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D3370: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3374: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3378: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D337C: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D3380: lwc1        $f4, 0x14($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D3384: lwc1        $f2, 0x14($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D3388: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D338C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3390: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D3394: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3398: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D339C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D33A0: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D33A4: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D33A8: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D33AC: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D33B0: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D33B4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D33B8: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D33BC: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D33C0: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D33C4: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D33C8: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D33CC: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D33D0: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D33D4: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D33D8: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D33DC: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D33E0: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D33E4: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D33E8: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D33EC: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D33F0: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D33F4: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D33F8: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D33FC: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D3400: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D3404: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3408: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D340C: lwc1        $f4, 0x10($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D3410: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3414: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3418: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D341C: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3420: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D3424: lwc1        $f2, 0x10($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D3428: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D342C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3430: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D3434: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3438: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D343C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3440: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    // 0x800D3444: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3448: swc1        $f0, 0x34($gp)
    MEM_W(0X34, ctx->r28) = ctx->f0.u32l;
    // 0x800D344C: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3450: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D3454: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3458: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D345C: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D3460: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3464: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3468: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D346C: swc1        $f0, 0x38($gp)
    MEM_W(0X38, ctx->r28) = ctx->f0.u32l;
    // 0x800D3470: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D3474: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D3478: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D347C: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3480: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3484: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3488: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D348C: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D3490: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3494: ori         $a0, $a0, 0xD0
    ctx->r4 = ctx->r4 | 0XD0;
    // 0x800D3498: addiu       $a1, $zero, 0x34
    ctx->r5 = ADD32(0, 0X34);
    // 0x800D349C: swc1        $f28, 0x40($gp)
    MEM_W(0X40, ctx->r28) = ctx->f28.u32l;
    // 0x800D34A0: jal         0x80167848
    // 0x800D34A4: swc1        $f2, 0x3C($gp)
    MEM_W(0X3C, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D34A4: swc1        $f2, 0x3C($gp)
    MEM_W(0X3C, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D34A8: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D34AC: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D34B0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D34B4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D34B8: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D34BC:
    // 0x800D34BC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D34C0: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D34C4: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D34C8: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D34CC: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D34D0: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D34D4: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D34D8: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D34DC: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D34E0: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D34E4: jr          $ra
    // 0x800D34E8: nop

    return;
    // 0x800D34E8: nop

;}
RECOMP_FUNC void func_800D34EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D34EC: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D34F0: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D34F4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D34F8: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D34FC: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D3500: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D3504: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D3508: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D350C: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D3510: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D3514: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D3518: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D351C: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D3520: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D3524: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D3528: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D352C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D3530: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D3534: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D3538: bc1f        L_800D3588
    if (!c1cs) {
        // 0x800D353C: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D3588;
    }
    // 0x800D353C: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D3540: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D3544: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D3548: nop

    // 0x800D354C: bc1f        L_800D3560
    if (!c1cs) {
        // 0x800D3550: addu        $s0, $a0, $zero
        ctx->r16 = ADD32(ctx->r4, 0);
            goto L_800D3560;
    }
    // 0x800D3550: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D3554: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D3558: j           L_800D35C4
    // 0x800D355C: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D35C4;
    // 0x800D355C: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D3560:
    // 0x800D3560: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D3564: nop

    // 0x800D3568: bc1f        L_800D3578
    if (!c1cs) {
        // 0x800D356C: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D3578;
    }
    // 0x800D356C: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D3570: j           L_800D35C4
    // 0x800D3574: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D35C4;
    // 0x800D3574: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D3578:
    // 0x800D3578: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D357C: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D3580: j           L_800D35C4
    // 0x800D3584: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D35C4;
    // 0x800D3584: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D3588:
    // 0x800D3588: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D358C: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D3590: nop

    // 0x800D3594: bc1f        L_800D35A8
    if (!c1cs) {
        // 0x800D3598: addu        $s0, $v1, $zero
        ctx->r16 = ADD32(ctx->r3, 0);
            goto L_800D35A8;
    }
    // 0x800D3598: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
    // 0x800D359C: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D35A0: j           L_800D35C4
    // 0x800D35A4: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D35C4;
    // 0x800D35A4: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D35A8:
    // 0x800D35A8: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D35AC: nop

    // 0x800D35B0: bc1t        L_800D35C0
    if (c1cs) {
        // 0x800D35B4: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D35C0;
    }
    // 0x800D35B4: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D35B8: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D35BC: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
L_800D35C0:
    // 0x800D35C0: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D35C4:
    // 0x800D35C4: lwc1        $f6, 0x0($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D35C8: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D35CC: sub.s       $f26, $f6, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x800D35D0: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D35D4: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D35D8: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D35DC: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D35E0: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D35E4: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D35E8: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D35EC: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D35F0: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D35F4: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D35F8: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800D35FC: c.eq.s      $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f28.fl == ctx->f0.fl;
    // 0x800D3600: nop

    // 0x800D3604: bc1t        L_800D3878
    if (c1cs) {
        // 0x800D3608: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D3878;
    }
    // 0x800D3608: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D360C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3610: lwc1        $f2, -0xAFC($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XAFC);
    // 0x800D3614: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D3618: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D361C: swc1        $f6, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f6.u32l;
    // 0x800D3620: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D3624: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D3628: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D362C: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D3630: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D3634: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D3638: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D363C: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3640: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D3644: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D3648: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D364C: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D3650: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D3654: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D3658: jal         0x80167848
    // 0x800D365C: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D365C: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D3660: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D3664: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D3668: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D366C: lwc1        $f2, 0x14($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D3670: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3674: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3678: lwc1        $f4, 0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D367C: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3680: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3684: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3688: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D368C: lwc1        $f4, 0x14($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D3690: lwc1        $f2, 0x14($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D3694: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3698: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D369C: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D36A0: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D36A4: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D36A8: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D36AC: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D36B0: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D36B4: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D36B8: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D36BC: lwc1        $f2, 0x18($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X18);
    // 0x800D36C0: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D36C4: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D36C8: lwc1        $f4, 0x18($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X18);
    // 0x800D36CC: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D36D0: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D36D4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D36D8: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D36DC: lwc1        $f4, 0x18($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X18);
    // 0x800D36E0: lwc1        $f2, 0x18($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X18);
    // 0x800D36E4: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D36E8: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D36EC: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D36F0: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D36F4: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D36F8: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D36FC: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D3700: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D3704: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D3708: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D370C: lwc1        $f2, 0x1C($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X1C);
    // 0x800D3710: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3714: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3718: lwc1        $f4, 0x1C($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x800D371C: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3720: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3724: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3728: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D372C: lwc1        $f4, 0x1C($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X1C);
    // 0x800D3730: lwc1        $f2, 0x1C($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x800D3734: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3738: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D373C: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D3740: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3744: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3748: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D374C: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D3750: ori         $a0, $a0, 0xA0
    ctx->r4 = ctx->r4 | 0XA0;
    // 0x800D3754: addiu       $a1, $zero, 0x24
    ctx->r5 = ADD32(0, 0X24);
    // 0x800D3758: jal         0x80167848
    // 0x800D375C: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D375C: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D3760: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D3764: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D3768: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D376C: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D3770: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3774: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3778: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D377C: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3780: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3784: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3788: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D378C: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D3790: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D3794: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3798: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D379C: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D37A0: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D37A4: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D37A8: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D37AC: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D37B0: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D37B4: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D37B8: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D37BC: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D37C0: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D37C4: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D37C8: lwc1        $f4, 0x10($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D37CC: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D37D0: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D37D4: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D37D8: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D37DC: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D37E0: lwc1        $f2, 0x10($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D37E4: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D37E8: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D37EC: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D37F0: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D37F4: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D37F8: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D37FC: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D3800: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3804: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D3808: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D380C: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D3810: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3814: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3818: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D381C: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3820: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3824: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3828: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D382C: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D3830: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D3834: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3838: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D383C: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3840: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3844: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3848: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D384C: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3850: ori         $a0, $a0, 0xDC
    ctx->r4 = ctx->r4 | 0XDC;
    // 0x800D3854: addiu       $a1, $zero, 0x28
    ctx->r5 = ADD32(0, 0X28);
    // 0x800D3858: swc1        $f28, 0x34($gp)
    MEM_W(0X34, ctx->r28) = ctx->f28.u32l;
    // 0x800D385C: jal         0x80167848
    // 0x800D3860: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_2;
    // 0x800D3860: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_2:
    // 0x800D3864: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D3868: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D386C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D3870: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3874: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D3878:
    // 0x800D3878: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D387C: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D3880: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D3884: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D3888: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D388C: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D3890: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D3894: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D3898: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D389C: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D38A0: jr          $ra
    // 0x800D38A4: nop

    return;
    // 0x800D38A4: nop

;}
RECOMP_FUNC void func_800D38A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D38A8: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D38AC: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x800D38B0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D38B4: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D38B8: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D38BC: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D38C0: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D38C4: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D38C8: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D38CC: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D38D0: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D38D4: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800D38D8: lwc1        $f2, 0x4($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800D38DC: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x800D38E0: lw          $v0, 0x6754($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6754);
    // 0x800D38E4: c.le.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl <= ctx->f2.fl;
    // 0x800D38E8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D38EC: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D38F0: sw          $v0, 0x6754($at)
    MEM_W(0X6754, ctx->r1) = ctx->r2;
    // 0x800D38F4: bc1f        L_800D3944
    if (!c1cs) {
        // 0x800D38F8: addu        $a1, $a2, $zero
        ctx->r5 = ADD32(ctx->r6, 0);
            goto L_800D3944;
    }
    // 0x800D38F8: addu        $a1, $a2, $zero
    ctx->r5 = ADD32(ctx->r6, 0);
    // 0x800D38FC: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D3900: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D3904: nop

    // 0x800D3908: bc1f        L_800D391C
    if (!c1cs) {
        // 0x800D390C: addu        $s0, $a0, $zero
        ctx->r16 = ADD32(ctx->r4, 0);
            goto L_800D391C;
    }
    // 0x800D390C: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D3910: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
    // 0x800D3914: j           L_800D3980
    // 0x800D3918: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D3980;
    // 0x800D3918: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D391C:
    // 0x800D391C: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D3920: nop

    // 0x800D3924: bc1f        L_800D3934
    if (!c1cs) {
        // 0x800D3928: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D3934;
    }
    // 0x800D3928: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D392C: j           L_800D3980
    // 0x800D3930: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D3980;
    // 0x800D3930: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D3934:
    // 0x800D3934: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D3938: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D393C: j           L_800D3980
    // 0x800D3940: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
        goto L_800D3980;
    // 0x800D3940: addu        $s1, $v1, $zero
    ctx->r17 = ADD32(ctx->r3, 0);
L_800D3944:
    // 0x800D3944: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800D3948: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x800D394C: nop

    // 0x800D3950: bc1f        L_800D3964
    if (!c1cs) {
        // 0x800D3954: addu        $s0, $v1, $zero
        ctx->r16 = ADD32(ctx->r3, 0);
            goto L_800D3964;
    }
    // 0x800D3954: addu        $s0, $v1, $zero
    ctx->r16 = ADD32(ctx->r3, 0);
    // 0x800D3958: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x800D395C: j           L_800D3980
    // 0x800D3960: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
        goto L_800D3980;
    // 0x800D3960: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
L_800D3964:
    // 0x800D3964: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x800D3968: nop

    // 0x800D396C: bc1t        L_800D397C
    if (c1cs) {
        // 0x800D3970: addu        $s2, $a1, $zero
        ctx->r18 = ADD32(ctx->r5, 0);
            goto L_800D397C;
    }
    // 0x800D3970: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D3974: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    // 0x800D3978: addu        $s2, $v1, $zero
    ctx->r18 = ADD32(ctx->r3, 0);
L_800D397C:
    // 0x800D397C: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_800D3980:
    // 0x800D3980: lwc1        $f6, 0x0($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800D3984: lwc1        $f4, 0x0($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D3988: sub.s       $f26, $f6, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f26.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x800D398C: lwc1        $f2, 0x4($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D3990: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D3994: sub.s       $f20, $f2, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3998: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D399C: sub.s       $f22, $f0, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D39A0: mul.s       $f2, $f26, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = MUL_S(ctx->f26.fl, ctx->f20.fl);
    // 0x800D39A4: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D39A8: sub.s       $f24, $f4, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x800D39AC: mul.s       $f0, $f24, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = MUL_S(ctx->f24.fl, ctx->f22.fl);
    // 0x800D39B0: sub.s       $f28, $f2, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f28.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D39B4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800D39B8: c.eq.s      $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f28.fl == ctx->f0.fl;
    // 0x800D39BC: nop

    // 0x800D39C0: bc1t        L_800D3C84
    if (c1cs) {
        // 0x800D39C4: lui         $a0, 0x820
        ctx->r4 = S32(0X820 << 16);
            goto L_800D3C84;
    }
    // 0x800D39C4: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D39C8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D39CC: lwc1        $f2, -0xAF8($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XAF8);
    // 0x800D39D0: div.s       $f2, $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f28.fl);
    // 0x800D39D4: mul.s       $f26, $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f26.fl = MUL_S(ctx->f26.fl, ctx->f2.fl);
    // 0x800D39D8: swc1        $f6, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f6.u32l;
    // 0x800D39DC: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x800D39E0: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800D39E4: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D39E8: lwc1        $f0, 0x0($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X0);
    // 0x800D39EC: swc1        $f0, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f0.u32l;
    // 0x800D39F0: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x800D39F4: mul.s       $f22, $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f22.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800D39F8: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D39FC: lwc1        $f0, 0x0($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800D3A00: ori         $a0, $a0, 0x88
    ctx->r4 = ctx->r4 | 0X88;
    // 0x800D3A04: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D3A08: lwc1        $f0, 0x4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800D3A0C: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800D3A10: swc1        $f0, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f0.u32l;
    // 0x800D3A14: jal         0x80167848
    // 0x800D3A18: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x800D3A18: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    after_0:
    // 0x800D3A1C: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D3A20: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D3A24: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D3A28: lwc1        $f2, 0x18($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X18);
    // 0x800D3A2C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3A30: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3A34: lwc1        $f4, 0x18($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X18);
    // 0x800D3A38: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3A3C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3A40: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3A44: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D3A48: lwc1        $f4, 0x18($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X18);
    // 0x800D3A4C: lwc1        $f2, 0x18($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X18);
    // 0x800D3A50: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3A54: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3A58: lwc1        $f0, 0x18($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D3A5C: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3A60: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3A64: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3A68: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D3A6C: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D3A70: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3A74: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D3A78: lwc1        $f2, 0x1C($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X1C);
    // 0x800D3A7C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3A80: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3A84: lwc1        $f4, 0x1C($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x800D3A88: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3A8C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3A90: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3A94: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D3A98: lwc1        $f4, 0x1C($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X1C);
    // 0x800D3A9C: lwc1        $f2, 0x1C($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x800D3AA0: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3AA4: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3AA8: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D3AAC: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3AB0: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3AB4: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3AB8: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D3ABC: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x800D3AC0: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D3AC4: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x800D3AC8: lwc1        $f2, 0x20($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X20);
    // 0x800D3ACC: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3AD0: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3AD4: lwc1        $f4, 0x20($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X20);
    // 0x800D3AD8: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3ADC: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3AE0: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3AE4: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3AE8: lwc1        $f4, 0x20($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X20);
    // 0x800D3AEC: lwc1        $f2, 0x20($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X20);
    // 0x800D3AF0: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3AF4: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3AF8: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x800D3AFC: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3B00: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3B04: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3B08: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D3B0C: ori         $a0, $a0, 0xA0
    ctx->r4 = ctx->r4 | 0XA0;
    // 0x800D3B10: addiu       $a1, $zero, 0x24
    ctx->r5 = ADD32(0, 0X24);
    // 0x800D3B14: jal         0x80167848
    // 0x800D3B18: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x800D3B18: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    after_1:
    // 0x800D3B1C: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D3B20: swc1        $f0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->f0.u32l;
    // 0x800D3B24: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D3B28: lwc1        $f2, 0x14($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D3B2C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3B30: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3B34: lwc1        $f4, 0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D3B38: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3B3C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3B40: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3B44: swc1        $f0, 0x14($gp)
    MEM_W(0X14, ctx->r28) = ctx->f0.u32l;
    // 0x800D3B48: lwc1        $f4, 0x14($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X14);
    // 0x800D3B4C: lwc1        $f2, 0x14($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X14);
    // 0x800D3B50: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3B54: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3B58: lwc1        $f0, 0x14($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X14);
    // 0x800D3B5C: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3B60: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3B64: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3B68: swc1        $f2, 0x18($gp)
    MEM_W(0X18, ctx->r28) = ctx->f2.u32l;
    // 0x800D3B6C: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D3B70: swc1        $f0, 0x1C($gp)
    MEM_W(0X1C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3B74: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D3B78: lwc1        $f2, 0xC($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D3B7C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3B80: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3B84: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D3B88: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3B8C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3B90: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3B94: swc1        $f0, 0x20($gp)
    MEM_W(0X20, ctx->r28) = ctx->f0.u32l;
    // 0x800D3B98: lwc1        $f4, 0xC($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0XC);
    // 0x800D3B9C: lwc1        $f2, 0xC($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0XC);
    // 0x800D3BA0: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3BA4: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3BA8: lwc1        $f0, 0xC($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XC);
    // 0x800D3BAC: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3BB0: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3BB4: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3BB8: swc1        $f2, 0x24($gp)
    MEM_W(0X24, ctx->r28) = ctx->f2.u32l;
    // 0x800D3BBC: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D3BC0: swc1        $f0, 0x28($gp)
    MEM_W(0X28, ctx->r28) = ctx->f0.u32l;
    // 0x800D3BC4: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D3BC8: lwc1        $f2, 0x10($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D3BCC: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3BD0: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3BD4: lwc1        $f4, 0x10($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D3BD8: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3BDC: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3BE0: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3BE4: swc1        $f0, 0x2C($gp)
    MEM_W(0X2C, ctx->r28) = ctx->f0.u32l;
    // 0x800D3BE8: lwc1        $f4, 0x10($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X10);
    // 0x800D3BEC: lwc1        $f2, 0x10($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X10);
    // 0x800D3BF0: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3BF4: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3BF8: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800D3BFC: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3C00: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3C04: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3C08: swc1        $f2, 0x30($gp)
    MEM_W(0X30, ctx->r28) = ctx->f2.u32l;
    // 0x800D3C0C: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3C10: swc1        $f0, 0x34($gp)
    MEM_W(0X34, ctx->r28) = ctx->f0.u32l;
    // 0x800D3C14: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3C18: lwc1        $f2, 0x8($s2)
    ctx->f2.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D3C1C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3C20: mul.s       $f0, $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x800D3C24: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D3C28: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D3C2C: mul.s       $f2, $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f22.fl);
    // 0x800D3C30: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D3C34: swc1        $f0, 0x38($gp)
    MEM_W(0X38, ctx->r28) = ctx->f0.u32l;
    // 0x800D3C38: lwc1        $f4, 0x8($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X8);
    // 0x800D3C3C: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800D3C40: sub.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x800D3C44: mul.s       $f2, $f2, $f26
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f26.fl);
    // 0x800D3C48: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800D3C4C: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D3C50: mul.s       $f0, $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x800D3C54: lui         $a0, 0x820
    ctx->r4 = S32(0X820 << 16);
    // 0x800D3C58: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x800D3C5C: ori         $a0, $a0, 0xD0
    ctx->r4 = ctx->r4 | 0XD0;
    // 0x800D3C60: addiu       $a1, $zero, 0x34
    ctx->r5 = ADD32(0, 0X34);
    // 0x800D3C64: swc1        $f28, 0x40($gp)
    MEM_W(0X40, ctx->r28) = ctx->f28.u32l;
    // 0x800D3C68: jal         0x80167848
    // 0x800D3C6C: swc1        $f2, 0x3C($gp)
    MEM_W(0X3C, ctx->r28) = ctx->f2.u32l;
    func_80167848(rdram, ctx);
        goto after_2;
    // 0x800D3C6C: swc1        $f2, 0x3C($gp)
    MEM_W(0X3C, ctx->r28) = ctx->f2.u32l;
    after_2:
    // 0x800D3C70: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D3C74: lw          $v0, 0x3AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3AC);
    // 0x800D3C78: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D3C7C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3C80: sw          $v0, 0x3AC($at)
    MEM_W(0X3AC, ctx->r1) = ctx->r2;
L_800D3C84:
    // 0x800D3C84: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D3C88: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D3C8C: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D3C90: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D3C94: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D3C98: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D3C9C: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D3CA0: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D3CA4: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D3CA8: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D3CAC: jr          $ra
    // 0x800D3CB0: nop

    return;
    // 0x800D3CB0: nop

;}
RECOMP_FUNC void func_800D3CB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D3CB8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800D3CBC: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x800D3CC0: mov.s       $f20, $f12
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 12);
    ctx->f20.fl = ctx->f12.fl;
    // 0x800D3CC4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3CC8: lwc1        $f12, -0xAF0($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0XAF0);
    // 0x800D3CCC: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x800D3CD0: jal         0x800D0DC0
    // 0x800D3CD4: div.s       $f12, $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f20.fl);
    func_800D0DC0(rdram, ctx);
        goto after_0;
    // 0x800D3CD4: div.s       $f12, $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f20.fl);
    after_0:
    // 0x800D3CD8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3CDC: lwc1        $f12, -0xAEC($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0XAEC);
    // 0x800D3CE0: div.s       $f12, $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f20.fl);
    // 0x800D3CE4: jal         0x800D0DC0
    // 0x800D3CE8: add.s       $f20, $f0, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f0.fl + ctx->f0.fl;
    func_800D0DC0(rdram, ctx);
        goto after_1;
    // 0x800D3CE8: add.s       $f20, $f0, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f0.fl + ctx->f0.fl;
    after_1:
    // 0x800D3CEC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3CF0: lwc1        $f2, -0xAE8($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XAE8);
    // 0x800D3CF4: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x800D3CF8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3CFC: lwc1        $f14, -0xAE4($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0XAE4);
    // 0x800D3D00: div.s       $f12, $f20, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = DIV_S(ctx->f20.fl, ctx->f14.fl);
    // 0x800D3D04: add.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f0.fl;
    // 0x800D3D08: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D3D0C: jal         0x800D3D28
    // 0x800D3D10: div.s       $f14, $f0, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = DIV_S(ctx->f0.fl, ctx->f14.fl);
    static_0_800D3D28(rdram, ctx);
        goto after_2;
    // 0x800D3D10: div.s       $f14, $f0, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = DIV_S(ctx->f0.fl, ctx->f14.fl);
    after_2:
    // 0x800D3D14: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x800D3D18: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x800D3D1C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800D3D20: jr          $ra
    // 0x800D3D24: nop

    return;
    // 0x800D3D24: nop

;}
RECOMP_FUNC void func_800D3D30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D3D30: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D3D34: sdc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X38, ctx->r29);
    // 0x800D3D38: mov.s       $f28, $f12
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 12);
    ctx->f28.fl = ctx->f12.fl;
    // 0x800D3D3C: sdc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X40, ctx->r29);
    // 0x800D3D40: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x800D3D44: sdc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X30, ctx->r29);
    // 0x800D3D48: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x800D3D4C: c.lt.s      $f2, $f28
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 28);
    c1cs = ctx->f2.fl < ctx->f28.fl;
    // 0x800D3D50: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x800D3D54: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x800D3D58: bc1f        L_800D3D64
    if (!c1cs) {
        // 0x800D3D5C: mov.s       $f30, $f14
        CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 14);
    ctx->f30.fl = ctx->f14.fl;
            goto L_800D3D64;
    }
    // 0x800D3D5C: mov.s       $f30, $f14
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 14);
    ctx->f30.fl = ctx->f14.fl;
    // 0x800D3D60: mov.s       $f28, $f2
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    ctx->f28.fl = ctx->f2.fl;
L_800D3D64:
    // 0x800D3D64: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3D68: lwc1        $f0, -0xADC($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0XADC);
    // 0x800D3D6C: c.lt.s      $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f28.fl < ctx->f0.fl;
    // 0x800D3D70: nop

    // 0x800D3D74: bc1tl       L_800D3D7C
    if (c1cs) {
        // 0x800D3D78: mov.s       $f28, $f0
        CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    ctx->f28.fl = ctx->f0.fl;
            goto L_800D3D7C;
    }
    goto skip_0;
    // 0x800D3D78: mov.s       $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    ctx->f28.fl = ctx->f0.fl;
    skip_0:
L_800D3D7C:
    // 0x800D3D7C: c.lt.s      $f2, $f30
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 30);
    c1cs = ctx->f2.fl < ctx->f30.fl;
    // 0x800D3D80: nop

    // 0x800D3D84: bc1tl       L_800D3D8C
    if (c1cs) {
        // 0x800D3D88: mov.s       $f30, $f2
        CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 2);
    ctx->f30.fl = ctx->f2.fl;
            goto L_800D3D8C;
    }
    goto skip_1;
    // 0x800D3D88: mov.s       $f30, $f2
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 2);
    ctx->f30.fl = ctx->f2.fl;
    skip_1:
L_800D3D8C:
    // 0x800D3D8C: c.lt.s      $f30, $f0
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f30.fl < ctx->f0.fl;
    // 0x800D3D90: nop

    // 0x800D3D94: bc1tl       L_800D3D9C
    if (c1cs) {
        // 0x800D3D98: mov.s       $f30, $f0
        CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 0);
    ctx->f30.fl = ctx->f0.fl;
            goto L_800D3D9C;
    }
    goto skip_2;
    // 0x800D3D98: mov.s       $f30, $f0
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 0);
    ctx->f30.fl = ctx->f0.fl;
    skip_2:
L_800D3D9C:
    // 0x800D3D9C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3DA0: lwc1        $f26, -0xAD8($at)
    ctx->f26.u32l = MEM_W(ctx->r1, -0XAD8);
    // 0x800D3DA4: add.s       $f12, $f28, $f26
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f12.fl = ctx->f28.fl + ctx->f26.fl;
    // 0x800D3DA8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3DAC: lwc1        $f22, -0xAD4($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0XAD4);
    // 0x800D3DB0: div.s       $f12, $f12, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f22.fl);
    // 0x800D3DB4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3DB8: lwc1        $f24, -0xAD0($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0XAD0);
    // 0x800D3DBC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3DC0: lwc1        $f20, -0xACC($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0XACC);
    // 0x800D3DC4: jal         0x800D0FB8
    // 0x800D3DC8: mul.s       $f12, $f12, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f24.fl);
    func_800D0FB8(rdram, ctx);
        goto after_0;
    // 0x800D3DC8: mul.s       $f12, $f12, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f24.fl);
    after_0:
    // 0x800D3DCC: div.s       $f0, $f20, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
    // 0x800D3DD0: add.s       $f12, $f30, $f26
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f12.fl = ctx->f30.fl + ctx->f26.fl;
    // 0x800D3DD4: div.s       $f12, $f12, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f22.fl);
    // 0x800D3DD8: div.s       $f0, $f20, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
    // 0x800D3DDC: div.s       $f28, $f28, $f22
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f28.fl = DIV_S(ctx->f28.fl, ctx->f22.fl);
    // 0x800D3DE0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3DE4: swc1        $f0, 0x525C($at)
    MEM_W(0X525C, ctx->r1) = ctx->f0.u32l;
    // 0x800D3DE8: jal         0x800D0FB8
    // 0x800D3DEC: mul.s       $f12, $f12, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f24.fl);
    func_800D0FB8(rdram, ctx);
        goto after_1;
    // 0x800D3DEC: mul.s       $f12, $f12, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f24.fl);
    after_1:
    // 0x800D3DF0: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D3DF4: lwc1        $f2, -0x2B20($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X2B20);
    // 0x800D3DF8: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D3DFC: div.s       $f0, $f2, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D3E00: div.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D3E04: div.s       $f30, $f30, $f22
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f30.fl = DIV_S(ctx->f30.fl, ctx->f22.fl);
    // 0x800D3E08: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3E0C: swc1        $f2, 0x5260($at)
    MEM_W(0X5260, ctx->r1) = ctx->f2.u32l;
    // 0x800D3E10: jal         0x800D0FB8
    // 0x800D3E14: mul.s       $f12, $f28, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f12.fl = MUL_S(ctx->f28.fl, ctx->f24.fl);
    func_800D0FB8(rdram, ctx);
        goto after_2;
    // 0x800D3E14: mul.s       $f12, $f28, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f12.fl = MUL_S(ctx->f28.fl, ctx->f24.fl);
    after_2:
    // 0x800D3E18: div.s       $f20, $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
    // 0x800D3E1C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3E20: swc1        $f20, 0x5280($at)
    MEM_W(0X5280, ctx->r1) = ctx->f20.u32l;
    // 0x800D3E24: jal         0x800D0FB8
    // 0x800D3E28: mul.s       $f12, $f30, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f12.fl = MUL_S(ctx->f30.fl, ctx->f24.fl);
    func_800D0FB8(rdram, ctx);
        goto after_3;
    // 0x800D3E28: mul.s       $f12, $f30, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f12.fl = MUL_S(ctx->f30.fl, ctx->f24.fl);
    after_3:
    // 0x800D3E2C: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D3E30: lwc1        $f2, -0x2B20($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X2B20);
    // 0x800D3E34: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D3E38: div.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D3E3C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3E40: swc1        $f2, 0x5284($at)
    MEM_W(0X5284, ctx->r1) = ctx->f2.u32l;
    // 0x800D3E44: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x800D3E48: ldc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X40);
    // 0x800D3E4C: ldc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X38);
    // 0x800D3E50: ldc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X30);
    // 0x800D3E54: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x800D3E58: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x800D3E5C: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x800D3E60: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D3E64: jr          $ra
    // 0x800D3E68: nop

    return;
    // 0x800D3E68: nop

;}
RECOMP_FUNC void func_800D3E74(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D3E74: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800D3E78: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800D3E7C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D3E80: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x800D3E84: andi        $v0, $v1, 0x10
    ctx->r2 = ctx->r3 & 0X10;
    // 0x800D3E88: beq         $v0, $zero, L_800D3EAC
    if (ctx->r2 == 0) {
        // 0x800D3E8C: lui         $v0, 0xA
        ctx->r2 = S32(0XA << 16);
            goto L_800D3EAC;
    }
    // 0x800D3E8C: lui         $v0, 0xA
    ctx->r2 = S32(0XA << 16);
    // 0x800D3E90: and         $v0, $v1, $v0
    ctx->r2 = ctx->r3 & ctx->r2;
    // 0x800D3E94: bne         $v0, $zero, L_800D4034
    if (ctx->r2 != 0) {
        // 0x800D3E98: nop
    
            goto L_800D4034;
    }
    // 0x800D3E98: nop

    // 0x800D3E9C: jal         0x800C916C
    // 0x800D3EA0: nop

    static_0_800C916C(rdram, ctx);
        goto after_0;
    // 0x800D3EA0: nop

    after_0:
    // 0x800D3EA4: j           L_800D4034
    // 0x800D3EA8: nop

        goto L_800D4034;
    // 0x800D3EA8: nop

L_800D3EAC:
    // 0x800D3EAC: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D3EB0: lw          $a1, 0x518C($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X518C);
    // 0x800D3EB4: bne         $a1, $zero, L_800D3EF4
    if (ctx->r5 != 0) {
        // 0x800D3EB8: nop
    
            goto L_800D3EF4;
    }
    // 0x800D3EB8: nop

    // 0x800D3EBC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3EC0: sw          $zero, -0x2230($at)
    MEM_W(-0X2230, ctx->r1) = 0;
    // 0x800D3EC4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3EC8: sw          $zero, -0x222C($at)
    MEM_W(-0X222C, ctx->r1) = 0;
    // 0x800D3ECC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3ED0: sw          $zero, -0x2228($at)
    MEM_W(-0X2228, ctx->r1) = 0;
    // 0x800D3ED4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3ED8: sw          $zero, -0x2290($at)
    MEM_W(-0X2290, ctx->r1) = 0;
    // 0x800D3EDC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3EE0: sw          $zero, -0x228C($at)
    MEM_W(-0X228C, ctx->r1) = 0;
    // 0x800D3EE4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3EE8: sw          $zero, -0x2288($at)
    MEM_W(-0X2288, ctx->r1) = 0;
    // 0x800D3EEC: j           L_800D4034
    // 0x800D3EF0: nop

        goto L_800D4034;
    // 0x800D3EF0: nop

L_800D3EF4:
    // 0x800D3EF4: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x800D3EF8: lw          $v1, 0x65C4($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X65C4);
    // 0x800D3EFC: slt         $v0, $a0, $v1
    ctx->r2 = SIGNED(ctx->r4) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x800D3F00: bne         $v0, $zero, L_800D3F7C
    if (ctx->r2 != 0) {
        // 0x800D3F04: sll         $v0, $a0, 3
        ctx->r2 = S32(ctx->r4 << 3);
            goto L_800D3F7C;
    }
    // 0x800D3F04: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x800D3F08: sll         $v0, $v1, 3
    ctx->r2 = S32(ctx->r3 << 3);
    // 0x800D3F0C: subu        $v0, $v0, $v1
    ctx->r2 = SUB32(ctx->r2, ctx->r3);
    // 0x800D3F10: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x800D3F14: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x800D3F18: lwc1        $f0, -0x1C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, -0X1C);
    // 0x800D3F1C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3F20: lwc1        $f2, -0xAC8($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XAC8);
    // 0x800D3F24: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D3F28: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3F2C: swc1        $f0, -0x2230($at)
    MEM_W(-0X2230, ctx->r1) = ctx->f0.u32l;
    // 0x800D3F30: lwc1        $f0, -0x18($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, -0X18);
    // 0x800D3F34: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D3F38: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3F3C: swc1        $f0, -0x222C($at)
    MEM_W(-0X222C, ctx->r1) = ctx->f0.u32l;
    // 0x800D3F40: lwc1        $f0, -0x14($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, -0X14);
    // 0x800D3F44: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D3F48: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3F4C: swc1        $f0, -0x2228($at)
    MEM_W(-0X2228, ctx->r1) = ctx->f0.u32l;
    // 0x800D3F50: lwc1        $f0, -0x10($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, -0X10);
    // 0x800D3F54: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3F58: swc1        $f0, -0x2290($at)
    MEM_W(-0X2290, ctx->r1) = ctx->f0.u32l;
    // 0x800D3F5C: lwc1        $f0, -0xC($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, -0XC);
    // 0x800D3F60: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3F64: swc1        $f0, -0x228C($at)
    MEM_W(-0X228C, ctx->r1) = ctx->f0.u32l;
    // 0x800D3F68: lwc1        $f0, -0x8($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, -0X8);
    // 0x800D3F6C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3F70: swc1        $f0, -0x2288($at)
    MEM_W(-0X2288, ctx->r1) = ctx->f0.u32l;
    // 0x800D3F74: j           L_800D4034
    // 0x800D3F78: nop

        goto L_800D4034;
    // 0x800D3F78: nop

L_800D3F7C:
    // 0x800D3F7C: subu        $v0, $v0, $a0
    ctx->r2 = SUB32(ctx->r2, ctx->r4);
    // 0x800D3F80: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x800D3F84: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x800D3F88: lwc1        $f0, 0x0($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X0);
    // 0x800D3F8C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3F90: lwc1        $f2, -0xAC4($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XAC4);
    // 0x800D3F94: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D3F98: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3F9C: swc1        $f0, -0x2230($at)
    MEM_W(-0X2230, ctx->r1) = ctx->f0.u32l;
    // 0x800D3FA0: lwc1        $f0, 0x4($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X4);
    // 0x800D3FA4: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D3FA8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3FAC: swc1        $f0, -0x222C($at)
    MEM_W(-0X222C, ctx->r1) = ctx->f0.u32l;
    // 0x800D3FB0: lwc1        $f0, 0x8($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X8);
    // 0x800D3FB4: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D3FB8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3FBC: lwc1        $f12, -0xAC0($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0XAC0);
    // 0x800D3FC0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3FC4: swc1        $f0, -0x2228($at)
    MEM_W(-0X2228, ctx->r1) = ctx->f0.u32l;
    // 0x800D3FC8: lwc1        $f0, 0xC($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0XC);
    // 0x800D3FCC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3FD0: swc1        $f0, -0x2290($at)
    MEM_W(-0X2290, ctx->r1) = ctx->f0.u32l;
    // 0x800D3FD4: lwc1        $f0, 0x10($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X10);
    // 0x800D3FD8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3FDC: swc1        $f0, -0x228C($at)
    MEM_W(-0X228C, ctx->r1) = ctx->f0.u32l;
    // 0x800D3FE0: lwc1        $f0, 0x14($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X14);
    // 0x800D3FE4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D3FE8: swc1        $f0, -0x2288($at)
    MEM_W(-0X2288, ctx->r1) = ctx->f0.u32l;
    // 0x800D3FEC: lwc1        $f20, 0x18($v0)
    ctx->f20.u32l = MEM_W(ctx->r2, 0X18);
    // 0x800D3FF0: jal         0x800D0DC0
    // 0x800D3FF4: div.s       $f12, $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f20.fl);
    func_800D0DC0(rdram, ctx);
        goto after_1;
    // 0x800D3FF4: div.s       $f12, $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f20.fl);
    after_1:
    // 0x800D3FF8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D3FFC: lwc1        $f12, -0xABC($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0XABC);
    // 0x800D4000: div.s       $f12, $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f20.fl);
    // 0x800D4004: jal         0x800D0DC0
    // 0x800D4008: add.s       $f20, $f0, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f0.fl + ctx->f0.fl;
    func_800D0DC0(rdram, ctx);
        goto after_2;
    // 0x800D4008: add.s       $f20, $f0, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f0.fl + ctx->f0.fl;
    after_2:
    // 0x800D400C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4010: lwc1        $f2, -0xAB8($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XAB8);
    // 0x800D4014: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x800D4018: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D401C: lwc1        $f14, -0xAB4($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0XAB4);
    // 0x800D4020: div.s       $f12, $f20, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = DIV_S(ctx->f20.fl, ctx->f14.fl);
    // 0x800D4024: add.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f0.fl;
    // 0x800D4028: mul.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D402C: jal         0x800D3D28
    // 0x800D4030: div.s       $f14, $f0, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = DIV_S(ctx->f0.fl, ctx->f14.fl);
    static_0_800D3D28(rdram, ctx);
        goto after_3;
    // 0x800D4030: div.s       $f14, $f0, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = DIV_S(ctx->f0.fl, ctx->f14.fl);
    after_3:
L_800D4034:
    // 0x800D4034: lui         $a0, 0x8020
    ctx->r4 = S32(0X8020 << 16);
    // 0x800D4038: addiu       $a0, $a0, -0x5DF8
    ctx->r4 = ADD32(ctx->r4, -0X5DF8);
    // 0x800D403C: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4040: addiu       $a1, $a1, -0x2290
    ctx->r5 = ADD32(ctx->r5, -0X2290);
    // 0x800D4044: jal         0x800CFE5C
    // 0x800D4048: nop

    func_800CFE5C(rdram, ctx);
        goto after_4;
    // 0x800D4048: nop

    after_4:
    // 0x800D404C: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D4050: addiu       $s0, $s0, -0x2230
    ctx->r16 = ADD32(ctx->r16, -0X2230);
    // 0x800D4054: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4058: addiu       $a1, $a1, 0x5028
    ctx->r5 = ADD32(ctx->r5, 0X5028);
    // 0x800D405C: jal         0x800D0438
    // 0x800D4060: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800D0438(rdram, ctx);
        goto after_5;
    // 0x800D4060: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_5:
    // 0x800D4064: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D4068: lw          $v0, 0x52A4($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X52A4);
    // 0x800D406C: beq         $v0, $zero, L_800D407C
    if (ctx->r2 == 0) {
        // 0x800D4070: nop
    
            goto L_800D407C;
    }
    // 0x800D4070: nop

    // 0x800D4074: jal         0x800FF5D8
    // 0x800D4078: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800FF5D8(rdram, ctx);
        goto after_6;
    // 0x800D4078: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_6:
L_800D407C:
    // 0x800D407C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800D4080: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D4084: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x800D4088: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800D408C: jr          $ra
    // 0x800D4090: nop

    return;
    // 0x800D4090: nop

;}
RECOMP_FUNC void func_800D4094(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D4094: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800D4098: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x800D409C: lui         $a0, 0x8020
    ctx->r4 = S32(0X8020 << 16);
    // 0x800D40A0: addiu       $a0, $a0, -0x5DF8
    ctx->r4 = ADD32(ctx->r4, -0X5DF8);
    // 0x800D40A4: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D40A8: addiu       $a1, $a1, -0x2290
    ctx->r5 = ADD32(ctx->r5, -0X2290);
    // 0x800D40AC: jal         0x800CFE5C
    // 0x800D40B0: nop

    func_800CFE5C(rdram, ctx);
        goto after_0;
    // 0x800D40B0: nop

    after_0:
    // 0x800D40B4: lui         $a0, 0x801E
    ctx->r4 = S32(0X801E << 16);
    // 0x800D40B8: addiu       $a0, $a0, -0x2230
    ctx->r4 = ADD32(ctx->r4, -0X2230);
    // 0x800D40BC: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D40C0: addiu       $a1, $a1, 0x5028
    ctx->r5 = ADD32(ctx->r5, 0X5028);
    // 0x800D40C4: jal         0x800D0438
    // 0x800D40C8: nop

    func_800D0438(rdram, ctx);
        goto after_1;
    // 0x800D40C8: nop

    after_1:
    // 0x800D40CC: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x800D40D0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800D40D4: jr          $ra
    // 0x800D40D8: nop

    return;
    // 0x800D40D8: nop

;}
RECOMP_FUNC void func_800D40DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D40DC: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800D40E0: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x800D40E4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D40E8: lwc1        $f22, -0xAB0($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0XAB0);
    // 0x800D40EC: sdc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X40, ctx->r29);
    // 0x800D40F0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D40F4: lwc1        $f30, -0xAAC($at)
    ctx->f30.u32l = MEM_W(ctx->r1, -0XAAC);
    // 0x800D40F8: add.s       $f12, $f22, $f30
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f30.fl); 
    ctx->f12.fl = ctx->f22.fl + ctx->f30.fl;
    // 0x800D40FC: sdc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X30, ctx->r29);
    // 0x800D4100: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4104: lwc1        $f26, -0xAA8($at)
    ctx->f26.u32l = MEM_W(ctx->r1, -0XAA8);
    // 0x800D4108: div.s       $f12, $f12, $f26
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f26.fl);
    // 0x800D410C: sdc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X38, ctx->r29);
    // 0x800D4110: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4114: lwc1        $f28, -0xAA4($at)
    ctx->f28.u32l = MEM_W(ctx->r1, -0XAA4);
    // 0x800D4118: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D411C: lwc1        $f2, -0xAA0($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0XAA0);
    // 0x800D4120: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4124: lwc1        $f4, -0xA9C($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0XA9C);
    // 0x800D4128: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x800D412C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4130: lwc1        $f20, -0xA98($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0XA98);
    // 0x800D4134: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4138: lwc1        $f0, -0xA94($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0XA94);
    // 0x800D413C: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x800D4140: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4144: lwc1        $f24, -0xA90($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0XA90);
    // 0x800D4148: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x800D414C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4150: sw          $zero, 0x518C($at)
    MEM_W(0X518C, ctx->r1) = 0;
    // 0x800D4154: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4158: sw          $zero, -0x2230($at)
    MEM_W(-0X2230, ctx->r1) = 0;
    // 0x800D415C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4160: sw          $zero, -0x222C($at)
    MEM_W(-0X222C, ctx->r1) = 0;
    // 0x800D4164: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4168: sw          $zero, -0x2228($at)
    MEM_W(-0X2228, ctx->r1) = 0;
    // 0x800D416C: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D4170: swc1        $f2, 0x678C($at)
    MEM_W(0X678C, ctx->r1) = ctx->f2.u32l;
    // 0x800D4174: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D4178: swc1        $f4, 0x6764($at)
    MEM_W(0X6764, ctx->r1) = ctx->f4.u32l;
    // 0x800D417C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4180: swc1        $f0, 0x5298($at)
    MEM_W(0X5298, ctx->r1) = ctx->f0.u32l;
    // 0x800D4184: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4188: swc1        $f22, 0x5268($at)
    MEM_W(0X5268, ctx->r1) = ctx->f22.u32l;
    // 0x800D418C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4190: swc1        $f20, 0x526C($at)
    MEM_W(0X526C, ctx->r1) = ctx->f20.u32l;
    // 0x800D4194: jal         0x800D0FB8
    // 0x800D4198: mul.s       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f28.fl);
    func_800D0FB8(rdram, ctx);
        goto after_0;
    // 0x800D4198: mul.s       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f28.fl);
    after_0:
    // 0x800D419C: div.s       $f0, $f24, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f24.fl, ctx->f0.fl);
    // 0x800D41A0: add.s       $f12, $f20, $f30
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f30.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f30.fl;
    // 0x800D41A4: div.s       $f12, $f12, $f26
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f26.fl);
    // 0x800D41A8: div.s       $f0, $f24, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f24.fl, ctx->f0.fl);
    // 0x800D41AC: div.s       $f22, $f22, $f26
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f22.fl = DIV_S(ctx->f22.fl, ctx->f26.fl);
    // 0x800D41B0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D41B4: swc1        $f0, 0x525C($at)
    MEM_W(0X525C, ctx->r1) = ctx->f0.u32l;
    // 0x800D41B8: jal         0x800D0FB8
    // 0x800D41BC: mul.s       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f28.fl);
    func_800D0FB8(rdram, ctx);
        goto after_1;
    // 0x800D41BC: mul.s       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f28.fl);
    after_1:
    // 0x800D41C0: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D41C4: lwc1        $f2, -0x2B20($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X2B20);
    // 0x800D41C8: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D41CC: div.s       $f0, $f2, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D41D0: div.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D41D4: div.s       $f20, $f20, $f26
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f20.fl = DIV_S(ctx->f20.fl, ctx->f26.fl);
    // 0x800D41D8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D41DC: swc1        $f2, 0x5260($at)
    MEM_W(0X5260, ctx->r1) = ctx->f2.u32l;
    // 0x800D41E0: jal         0x800D0FB8
    // 0x800D41E4: mul.s       $f12, $f22, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f22.fl, ctx->f28.fl);
    func_800D0FB8(rdram, ctx);
        goto after_2;
    // 0x800D41E4: mul.s       $f12, $f22, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f22.fl, ctx->f28.fl);
    after_2:
    // 0x800D41E8: div.s       $f24, $f24, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = DIV_S(ctx->f24.fl, ctx->f0.fl);
    // 0x800D41EC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D41F0: swc1        $f24, 0x5280($at)
    MEM_W(0X5280, ctx->r1) = ctx->f24.u32l;
    // 0x800D41F4: jal         0x800D0FB8
    // 0x800D41F8: mul.s       $f12, $f20, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f20.fl, ctx->f28.fl);
    func_800D0FB8(rdram, ctx);
        goto after_3;
    // 0x800D41F8: mul.s       $f12, $f20, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f20.fl, ctx->f28.fl);
    after_3:
    // 0x800D41FC: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D4200: lwc1        $f2, -0x2B20($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X2B20);
    // 0x800D4204: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D4208: div.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D420C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4210: sw          $zero, -0x2290($at)
    MEM_W(-0X2290, ctx->r1) = 0;
    // 0x800D4214: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4218: sw          $zero, -0x228C($at)
    MEM_W(-0X228C, ctx->r1) = 0;
    // 0x800D421C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4220: sw          $zero, -0x2288($at)
    MEM_W(-0X2288, ctx->r1) = 0;
    // 0x800D4224: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4228: sw          $zero, 0x52A4($at)
    MEM_W(0X52A4, ctx->r1) = 0;
    // 0x800D422C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4230: swc1        $f2, 0x5284($at)
    MEM_W(0X5284, ctx->r1) = ctx->f2.u32l;
    // 0x800D4234: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x800D4238: ldc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X40);
    // 0x800D423C: ldc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X38);
    // 0x800D4240: ldc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X30);
    // 0x800D4244: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x800D4248: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x800D424C: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x800D4250: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x800D4254: jr          $ra
    // 0x800D4258: nop

    return;
    // 0x800D4258: nop

;}
RECOMP_FUNC void func_800D429C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D429C: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x800D42A0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800D42A4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D42A8: sdc1        $f30, 0x48($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X48, ctx->r29);
    // 0x800D42AC: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x800D42B0: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x800D42B4: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x800D42B8: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x800D42BC: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x800D42C0: beq         $v0, $zero, L_800D42D8
    if (ctx->r2 == 0) {
        // 0x800D42C4: addu        $s0, $a0, $zero
        ctx->r16 = ADD32(ctx->r4, 0);
            goto L_800D42D8;
    }
    // 0x800D42C4: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D42C8: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x800D42CC: addiu       $a0, $a0, -0xA8C
    ctx->r4 = ADD32(ctx->r4, -0XA8C);
    // 0x800D42D0: jal         0x80141210
    // 0x800D42D4: nop

    static_0_80141210(rdram, ctx);
        goto after_0;
    // 0x800D42D4: nop

    after_0:
L_800D42D8:
    // 0x800D42D8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D42DC: sw          $zero, 0x52A4($at)
    MEM_W(0X52A4, ctx->r1) = 0;
    // 0x800D42E0: lui         $v0, 0x102
    ctx->r2 = S32(0X102 << 16);
    // 0x800D42E4: beq         $s0, $v0, L_800D4360
    if (ctx->r16 == ctx->r2) {
        // 0x800D42E8: slt         $v0, $v0, $s0
        ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r16) ? 1 : 0;
            goto L_800D4360;
    }
    // 0x800D42E8: slt         $v0, $v0, $s0
    ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r16) ? 1 : 0;
    // 0x800D42EC: bne         $v0, $zero, L_800D432C
    if (ctx->r2 != 0) {
        // 0x800D42F0: lui         $v0, 0x106
        ctx->r2 = S32(0X106 << 16);
            goto L_800D432C;
    }
    // 0x800D42F0: lui         $v0, 0x106
    ctx->r2 = S32(0X106 << 16);
    // 0x800D42F4: lui         $v0, 0xB
    ctx->r2 = S32(0XB << 16);
    // 0x800D42F8: beq         $s0, $v0, L_800D4454
    if (ctx->r16 == ctx->r2) {
        // 0x800D42FC: slt         $v0, $v0, $s0
        ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r16) ? 1 : 0;
            goto L_800D4454;
    }
    // 0x800D42FC: slt         $v0, $v0, $s0
    ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r16) ? 1 : 0;
    // 0x800D4300: bne         $v0, $zero, L_800D431C
    if (ctx->r2 != 0) {
        // 0x800D4304: lui         $v0, 0xC
        ctx->r2 = S32(0XC << 16);
            goto L_800D431C;
    }
    // 0x800D4304: lui         $v0, 0xC
    ctx->r2 = S32(0XC << 16);
    // 0x800D4308: lui         $v0, 0xA
    ctx->r2 = S32(0XA << 16);
    // 0x800D430C: beq         $s0, $v0, L_800D4418
    if (ctx->r16 == ctx->r2) {
        // 0x800D4310: addiu       $v0, $zero, 0x3D
        ctx->r2 = ADD32(0, 0X3D);
            goto L_800D4418;
    }
    // 0x800D4310: addiu       $v0, $zero, 0x3D
    ctx->r2 = ADD32(0, 0X3D);
    // 0x800D4314: j           L_800D4444
    // 0x800D4318: nop

        goto L_800D4444;
    // 0x800D4318: nop

L_800D431C:
    // 0x800D431C: beq         $s0, $v0, L_800D4418
    if (ctx->r16 == ctx->r2) {
        // 0x800D4320: addiu       $v0, $zero, 0x3D
        ctx->r2 = ADD32(0, 0X3D);
            goto L_800D4418;
    }
    // 0x800D4320: addiu       $v0, $zero, 0x3D
    ctx->r2 = ADD32(0, 0X3D);
    // 0x800D4324: j           L_800D4444
    // 0x800D4328: nop

        goto L_800D4444;
    // 0x800D4328: nop

L_800D432C:
    // 0x800D432C: beq         $s0, $v0, L_800D4390
    if (ctx->r16 == ctx->r2) {
        // 0x800D4330: slt         $v0, $v0, $s0
        ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r16) ? 1 : 0;
            goto L_800D4390;
    }
    // 0x800D4330: slt         $v0, $v0, $s0
    ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r16) ? 1 : 0;
    // 0x800D4334: bne         $v0, $zero, L_800D4350
    if (ctx->r2 != 0) {
        // 0x800D4338: lui         $v0, 0x107
        ctx->r2 = S32(0X107 << 16);
            goto L_800D4350;
    }
    // 0x800D4338: lui         $v0, 0x107
    ctx->r2 = S32(0X107 << 16);
    // 0x800D433C: lui         $v0, 0x104
    ctx->r2 = S32(0X104 << 16);
    // 0x800D4340: beq         $s0, $v0, L_800D43C0
    if (ctx->r16 == ctx->r2) {
        // 0x800D4344: addiu       $v0, $zero, 0xF56
        ctx->r2 = ADD32(0, 0XF56);
            goto L_800D43C0;
    }
    // 0x800D4344: addiu       $v0, $zero, 0xF56
    ctx->r2 = ADD32(0, 0XF56);
    // 0x800D4348: j           L_800D4444
    // 0x800D434C: nop

        goto L_800D4444;
    // 0x800D434C: nop

L_800D4350:
    // 0x800D4350: beq         $s0, $v0, L_800D43EC
    if (ctx->r16 == ctx->r2) {
        // 0x800D4354: addiu       $v0, $zero, 0x109B
        ctx->r2 = ADD32(0, 0X109B);
            goto L_800D43EC;
    }
    // 0x800D4354: addiu       $v0, $zero, 0x109B
    ctx->r2 = ADD32(0, 0X109B);
    // 0x800D4358: j           L_800D4444
    // 0x800D435C: nop

        goto L_800D4444;
    // 0x800D435C: nop

L_800D4360:
    // 0x800D4360: addiu       $v0, $zero, 0xCCF
    ctx->r2 = ADD32(0, 0XCCF);
    // 0x800D4364: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4368: sw          $v0, 0x65C4($at)
    MEM_W(0X65C4, ctx->r1) = ctx->r2;
    // 0x800D436C: addiu       $a0, $sp, 0x10
    ctx->r4 = ADD32(ctx->r29, 0X10);
    // 0x800D4370: addiu       $a1, $zero, 0xCCF
    ctx->r5 = ADD32(0, 0XCCF);
    // 0x800D4374: lui         $a2, 0x8017
    ctx->r6 = S32(0X8017 << 16);
    // 0x800D4378: addiu       $a2, $a2, -0xA6C
    ctx->r6 = ADD32(ctx->r6, -0XA6C);
    // 0x800D437C: lui         $a3, 0x2B38
    ctx->r7 = S32(0X2B38 << 16);
    // 0x800D4380: jal         0x800CA278
    // 0x800D4384: ori         $a3, $a3, 0xBCE1
    ctx->r7 = ctx->r7 | 0XBCE1;
    func_800CA278(rdram, ctx);
        goto after_1;
    // 0x800D4384: ori         $a3, $a3, 0xBCE1
    ctx->r7 = ctx->r7 | 0XBCE1;
    after_1:
    // 0x800D4388: j           L_800D4460
    // 0x800D438C: nop

        goto L_800D4460;
    // 0x800D438C: nop

L_800D4390:
    // 0x800D4390: addiu       $v0, $zero, 0xEF7
    ctx->r2 = ADD32(0, 0XEF7);
    // 0x800D4394: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4398: sw          $v0, 0x65C4($at)
    MEM_W(0X65C4, ctx->r1) = ctx->r2;
    // 0x800D439C: addiu       $a0, $sp, 0x10
    ctx->r4 = ADD32(ctx->r29, 0X10);
    // 0x800D43A0: addiu       $a1, $zero, 0xEF7
    ctx->r5 = ADD32(0, 0XEF7);
    // 0x800D43A4: lui         $a2, 0x8017
    ctx->r6 = S32(0X8017 << 16);
    // 0x800D43A8: addiu       $a2, $a2, -0xA60
    ctx->r6 = ADD32(ctx->r6, -0XA60);
    // 0x800D43AC: lui         $a3, 0x82A5
    ctx->r7 = S32(0X82A5 << 16);
    // 0x800D43B0: jal         0x800CA278
    // 0x800D43B4: ori         $a3, $a3, 0x6DA
    ctx->r7 = ctx->r7 | 0X6DA;
    func_800CA278(rdram, ctx);
        goto after_2;
    // 0x800D43B4: ori         $a3, $a3, 0x6DA
    ctx->r7 = ctx->r7 | 0X6DA;
    after_2:
    // 0x800D43B8: j           L_800D4460
    // 0x800D43BC: nop

        goto L_800D4460;
    // 0x800D43BC: nop

L_800D43C0:
    // 0x800D43C0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D43C4: sw          $v0, 0x65C4($at)
    MEM_W(0X65C4, ctx->r1) = ctx->r2;
    // 0x800D43C8: addiu       $a0, $sp, 0x10
    ctx->r4 = ADD32(ctx->r29, 0X10);
    // 0x800D43CC: addiu       $a1, $zero, 0xF56
    ctx->r5 = ADD32(0, 0XF56);
    // 0x800D43D0: lui         $a2, 0x8017
    ctx->r6 = S32(0X8017 << 16);
    // 0x800D43D4: addiu       $a2, $a2, -0xA54
    ctx->r6 = ADD32(ctx->r6, -0XA54);
    // 0x800D43D8: lui         $a3, 0x79E8
    ctx->r7 = S32(0X79E8 << 16);
    // 0x800D43DC: jal         0x800CA278
    // 0x800D43E0: ori         $a3, $a3, 0xF699
    ctx->r7 = ctx->r7 | 0XF699;
    func_800CA278(rdram, ctx);
        goto after_3;
    // 0x800D43E0: ori         $a3, $a3, 0xF699
    ctx->r7 = ctx->r7 | 0XF699;
    after_3:
    // 0x800D43E4: j           L_800D4460
    // 0x800D43E8: nop

        goto L_800D4460;
    // 0x800D43E8: nop

L_800D43EC:
    // 0x800D43EC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D43F0: sw          $v0, 0x65C4($at)
    MEM_W(0X65C4, ctx->r1) = ctx->r2;
    // 0x800D43F4: addiu       $a0, $sp, 0x10
    ctx->r4 = ADD32(ctx->r29, 0X10);
    // 0x800D43F8: addiu       $a1, $zero, 0x109B
    ctx->r5 = ADD32(0, 0X109B);
    // 0x800D43FC: lui         $a2, 0x8017
    ctx->r6 = S32(0X8017 << 16);
    // 0x800D4400: addiu       $a2, $a2, -0xA48
    ctx->r6 = ADD32(ctx->r6, -0XA48);
    // 0x800D4404: lui         $a3, 0xC1F5
    ctx->r7 = S32(0XC1F5 << 16);
    // 0x800D4408: jal         0x800CA278
    // 0x800D440C: ori         $a3, $a3, 0x71D
    ctx->r7 = ctx->r7 | 0X71D;
    func_800CA278(rdram, ctx);
        goto after_4;
    // 0x800D440C: ori         $a3, $a3, 0x71D
    ctx->r7 = ctx->r7 | 0X71D;
    after_4:
    // 0x800D4410: j           L_800D4460
    // 0x800D4414: nop

        goto L_800D4460;
    // 0x800D4414: nop

L_800D4418:
    // 0x800D4418: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D441C: sw          $v0, 0x65C4($at)
    MEM_W(0X65C4, ctx->r1) = ctx->r2;
    // 0x800D4420: addiu       $a0, $sp, 0x10
    ctx->r4 = ADD32(ctx->r29, 0X10);
    // 0x800D4424: addiu       $a1, $zero, 0x3D
    ctx->r5 = ADD32(0, 0X3D);
    // 0x800D4428: lui         $a2, 0x8017
    ctx->r6 = S32(0X8017 << 16);
    // 0x800D442C: addiu       $a2, $a2, -0xA3C
    ctx->r6 = ADD32(ctx->r6, -0XA3C);
    // 0x800D4430: lui         $a3, 0x4BC0
    ctx->r7 = S32(0X4BC0 << 16);
    // 0x800D4434: jal         0x800CA278
    // 0x800D4438: ori         $a3, $a3, 0xDF8F
    ctx->r7 = ctx->r7 | 0XDF8F;
    func_800CA278(rdram, ctx);
        goto after_5;
    // 0x800D4438: ori         $a3, $a3, 0xDF8F
    ctx->r7 = ctx->r7 | 0XDF8F;
    after_5:
    // 0x800D443C: j           L_800D4460
    // 0x800D4440: nop

        goto L_800D4460;
    // 0x800D4440: nop

L_800D4444:
    // 0x800D4444: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x800D4448: addiu       $a0, $a0, -0xA30
    ctx->r4 = ADD32(ctx->r4, -0XA30);
    // 0x800D444C: jal         0x80141210
    // 0x800D4450: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    static_0_80141210(rdram, ctx);
        goto after_6;
    // 0x800D4450: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_6:
L_800D4454:
    // 0x800D4454: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4458: sw          $zero, 0x65C4($at)
    MEM_W(0X65C4, ctx->r1) = 0;
    // 0x800D445C: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
L_800D4460:
    // 0x800D4460: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4464: lwc1        $f22, -0xA00($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0XA00);
    // 0x800D4468: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D446C: lwc1        $f30, -0x9FC($at)
    ctx->f30.u32l = MEM_W(ctx->r1, -0X9FC);
    // 0x800D4470: add.s       $f12, $f22, $f30
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f30.fl); 
    ctx->f12.fl = ctx->f22.fl + ctx->f30.fl;
    // 0x800D4474: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4478: lwc1        $f26, -0x9F8($at)
    ctx->f26.u32l = MEM_W(ctx->r1, -0X9F8);
    // 0x800D447C: div.s       $f12, $f12, $f26
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f26.fl);
    // 0x800D4480: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4484: lwc1        $f28, -0x9F4($at)
    ctx->f28.u32l = MEM_W(ctx->r1, -0X9F4);
    // 0x800D4488: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D448C: lwc1        $f20, -0x9F0($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0X9F0);
    // 0x800D4490: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4494: lwc1        $f24, -0x9EC($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X9EC);
    // 0x800D4498: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D449C: swc1        $f22, 0x5268($at)
    MEM_W(0X5268, ctx->r1) = ctx->f22.u32l;
    // 0x800D44A0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D44A4: swc1        $f20, 0x526C($at)
    MEM_W(0X526C, ctx->r1) = ctx->f20.u32l;
    // 0x800D44A8: jal         0x800D0FB8
    // 0x800D44AC: mul.s       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f28.fl);
    func_800D0FB8(rdram, ctx);
        goto after_7;
    // 0x800D44AC: mul.s       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f28.fl);
    after_7:
    // 0x800D44B0: div.s       $f0, $f24, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f24.fl, ctx->f0.fl);
    // 0x800D44B4: add.s       $f12, $f20, $f30
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f30.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f30.fl;
    // 0x800D44B8: div.s       $f12, $f12, $f26
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f12.fl = DIV_S(ctx->f12.fl, ctx->f26.fl);
    // 0x800D44BC: div.s       $f0, $f24, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f24.fl, ctx->f0.fl);
    // 0x800D44C0: div.s       $f22, $f22, $f26
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f22.fl = DIV_S(ctx->f22.fl, ctx->f26.fl);
    // 0x800D44C4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D44C8: swc1        $f0, 0x525C($at)
    MEM_W(0X525C, ctx->r1) = ctx->f0.u32l;
    // 0x800D44CC: jal         0x800D0FB8
    // 0x800D44D0: mul.s       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f28.fl);
    func_800D0FB8(rdram, ctx);
        goto after_8;
    // 0x800D44D0: mul.s       $f12, $f12, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f28.fl);
    after_8:
    // 0x800D44D4: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D44D8: lwc1        $f2, -0x2B20($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X2B20);
    // 0x800D44DC: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D44E0: div.s       $f0, $f2, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D44E4: div.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D44E8: div.s       $f20, $f20, $f26
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f20.fl = DIV_S(ctx->f20.fl, ctx->f26.fl);
    // 0x800D44EC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D44F0: swc1        $f2, 0x5260($at)
    MEM_W(0X5260, ctx->r1) = ctx->f2.u32l;
    // 0x800D44F4: jal         0x800D0FB8
    // 0x800D44F8: mul.s       $f12, $f22, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f22.fl, ctx->f28.fl);
    func_800D0FB8(rdram, ctx);
        goto after_9;
    // 0x800D44F8: mul.s       $f12, $f22, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f22.fl, ctx->f28.fl);
    after_9:
    // 0x800D44FC: div.s       $f24, $f24, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = DIV_S(ctx->f24.fl, ctx->f0.fl);
    // 0x800D4500: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4504: swc1        $f24, 0x5280($at)
    MEM_W(0X5280, ctx->r1) = ctx->f24.u32l;
    // 0x800D4508: jal         0x800D0FB8
    // 0x800D450C: mul.s       $f12, $f20, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f20.fl, ctx->f28.fl);
    func_800D0FB8(rdram, ctx);
        goto after_10;
    // 0x800D450C: mul.s       $f12, $f20, $f28
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f12.fl = MUL_S(ctx->f20.fl, ctx->f28.fl);
    after_10:
    // 0x800D4510: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D4514: lwc1        $f2, -0x2B20($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X2B20);
    // 0x800D4518: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D451C: div.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f0.fl);
    // 0x800D4520: lw          $v0, 0x10($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X10);
    // 0x800D4524: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4528: sw          $v0, 0x518C($at)
    MEM_W(0X518C, ctx->r1) = ctx->r2;
    // 0x800D452C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4530: swc1        $f2, 0x5284($at)
    MEM_W(0X5284, ctx->r1) = ctx->f2.u32l;
    // 0x800D4534: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D4538: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800D453C: ldc1        $f30, 0x48($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X48);
    // 0x800D4540: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x800D4544: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x800D4548: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x800D454C: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x800D4550: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800D4554: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    // 0x800D4558: jr          $ra
    // 0x800D455C: nop

    return;
    // 0x800D455C: nop

;}
RECOMP_FUNC void func_800D4568(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D4568: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800D456C: beq         $a0, $zero, L_800D457C
    if (ctx->r4 == 0) {
        // 0x800D4570: sw          $ra, 0x10($sp)
        MEM_W(0X10, ctx->r29) = ctx->r31;
            goto L_800D457C;
    }
    // 0x800D4570: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x800D4574: jal         0x80143E74
    // 0x800D4578: nop

    func_80143E74(rdram, ctx);
        goto after_0;
    // 0x800D4578: nop

    after_0:
L_800D457C:
    // 0x800D457C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4580: sw          $zero, 0x52A4($at)
    MEM_W(0X52A4, ctx->r1) = 0;
    // 0x800D4584: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4588: sw          $zero, 0x518C($at)
    MEM_W(0X518C, ctx->r1) = 0;
    // 0x800D458C: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x800D4590: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800D4594: jr          $ra
    // 0x800D4598: nop

    return;
    // 0x800D4598: nop

;}
RECOMP_FUNC void func_800D459C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D459C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D45A0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D45A4: sw          $v0, 0x52A4($at)
    MEM_W(0X52A4, ctx->r1) = ctx->r2;
    // 0x800D45A8: jr          $ra
    // 0x800D45AC: nop

    return;
    // 0x800D45AC: nop

;}
RECOMP_FUNC void func_800D45C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D45C0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x800D45C4: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x800D45C8: lui         $s1, 0xB7F0
    ctx->r17 = S32(0XB7F0 << 16);
    // 0x800D45CC: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x800D45D0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800D45D4: lb          $a1, 0x0($s1)
    ctx->r5 = MEM_B(ctx->r17, 0X0);
    // 0x800D45D8: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x800D45DC: addiu       $a0, $a0, -0x9CC
    ctx->r4 = ADD32(ctx->r4, -0X9CC);
    // 0x800D45E0: jal         0x80141210
    // 0x800D45E4: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    static_0_80141210(rdram, ctx);
        goto after_0;
    // 0x800D45E4: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    after_0:
    // 0x800D45E8: ori         $s0, $s0, 0x10
    ctx->r16 = ctx->r16 | 0X10;
    // 0x800D45EC: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x800D45F0: sb          $s0, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r16;
    // 0x800D45F4: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x800D45F8: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x800D45FC: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800D4600: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x800D4604: jr          $ra
    // 0x800D4608: nop

    return;
    // 0x800D4608: nop

;}
RECOMP_FUNC void func_800D460C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D460C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D4610: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D4614: sw          $v0, -0x6DA4($at)
    MEM_W(-0X6DA4, ctx->r1) = ctx->r2;
    // 0x800D4618: jr          $ra
    // 0x800D461C: nop

    return;
    // 0x800D461C: nop

;}
RECOMP_FUNC void func_800D4620(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D4620: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D4624: sw          $zero, -0x6DA4($at)
    MEM_W(-0X6DA4, ctx->r1) = 0;
    // 0x800D4628: jr          $ra
    // 0x800D462C: nop

    return;
    // 0x800D462C: nop

;}
RECOMP_FUNC void func_800D4630(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D4630: lui         $at, 0x8021
    ctx->r1 = S32(0X8021 << 16);
    // 0x800D4634: sw          $zero, 0x2264($at)
    MEM_W(0X2264, ctx->r1) = 0;
    // 0x800D4638: lui         $at, 0x8021
    ctx->r1 = S32(0X8021 << 16);
    // 0x800D463C: sw          $zero, 0x2268($at)
    MEM_W(0X2268, ctx->r1) = 0;
    // 0x800D4640: jr          $ra
    // 0x800D4644: nop

    return;
    // 0x800D4644: nop

;}
RECOMP_FUNC void func_800D4648(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D4648: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D464C: ori         $v0, $v0, 0x4
    ctx->r2 = ctx->r2 | 0X4;
    // 0x800D4650: lbu         $v1, 0x0($v0)
    ctx->r3 = MEM_BU(ctx->r2, 0X0);
    // 0x800D4654: andi        $v1, $v1, 0x10
    ctx->r3 = ctx->r3 & 0X10;
    // 0x800D4658: beq         $v1, $zero, L_800D4674
    if (ctx->r3 == 0) {
        // 0x800D465C: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_800D4674;
    }
    // 0x800D465C: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x800D4660: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D4664: ori         $v0, $v0, 0x14
    ctx->r2 = ctx->r2 | 0X14;
    // 0x800D4668: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x800D466C: andi        $v0, $v0, 0x10
    ctx->r2 = ctx->r2 & 0X10;
    // 0x800D4670: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_800D4674:
    // 0x800D4674: jr          $ra
    // 0x800D4678: nop

    return;
    // 0x800D4678: nop

;}
RECOMP_FUNC void func_800D467C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D467C: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D4680: ori         $v0, $v0, 0x4
    ctx->r2 = ctx->r2 | 0X4;
    // 0x800D4684: lbu         $v1, 0x0($v0)
    ctx->r3 = MEM_BU(ctx->r2, 0X0);
    // 0x800D4688: andi        $v1, $v1, 0x20
    ctx->r3 = ctx->r3 & 0X20;
    // 0x800D468C: beq         $v1, $zero, L_800D46A8
    if (ctx->r3 == 0) {
        // 0x800D4690: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_800D46A8;
    }
    // 0x800D4690: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x800D4694: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D4698: ori         $v0, $v0, 0x14
    ctx->r2 = ctx->r2 | 0X14;
    // 0x800D469C: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x800D46A0: andi        $v0, $v0, 0x20
    ctx->r2 = ctx->r2 & 0X20;
    // 0x800D46A4: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_800D46A8:
    // 0x800D46A8: jr          $ra
    // 0x800D46AC: nop

    return;
    // 0x800D46AC: nop

;}
RECOMP_FUNC void func_800D46B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D46B0: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D46B4: ori         $v0, $v0, 0x4
    ctx->r2 = ctx->r2 | 0X4;
    // 0x800D46B8: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x800D46BC: andi        $v0, $v0, 0x10
    ctx->r2 = ctx->r2 & 0X10;
    // 0x800D46C0: bne         $v0, $zero, L_800D46E4
    if (ctx->r2 != 0) {
        // 0x800D46C4: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D46E4;
    }
    // 0x800D46C4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D46C8: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D46CC: ori         $v0, $v0, 0x14
    ctx->r2 = ctx->r2 | 0X14;
    // 0x800D46D0: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x800D46D4: andi        $v0, $v0, 0x10
    ctx->r2 = ctx->r2 & 0X10;
    // 0x800D46D8: bne         $v0, $zero, L_800D46E4
    if (ctx->r2 != 0) {
        // 0x800D46DC: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D46E4;
    }
    // 0x800D46DC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D46E0: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_800D46E4:
    // 0x800D46E4: jr          $ra
    // 0x800D46E8: nop

    return;
    // 0x800D46E8: nop

;}
RECOMP_FUNC void func_800D46F4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D46F4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800D46F8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D46FC: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D4700: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D4704: beq         $v0, $zero, L_800D4764
    if (ctx->r2 == 0) {
        // 0x800D4708: sw          $s0, 0x10($sp)
        MEM_W(0X10, ctx->r29) = ctx->r16;
            goto L_800D4764;
    }
    // 0x800D4708: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D470C: lui         $s1, 0xB680
    ctx->r17 = S32(0XB680 << 16);
    // 0x800D4710: ori         $s1, $s1, 0x4
    ctx->r17 = ctx->r17 | 0X4;
    // 0x800D4714: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D4718: lui         $s2, 0xB680
    ctx->r18 = S32(0XB680 << 16);
    // 0x800D471C: lbu         $a1, 0x0($s1)
    ctx->r5 = MEM_BU(ctx->r17, 0X0);
    // 0x800D4720: ori         $s2, $s2, 0x14
    ctx->r18 = ctx->r18 | 0X14;
    // 0x800D4724: jal         0x800D513C
    // 0x800D4728: andi        $a1, $a1, 0x10
    ctx->r5 = ctx->r5 & 0X10;
    func_800D513C(rdram, ctx);
        goto after_0;
    // 0x800D4728: andi        $a1, $a1, 0x10
    ctx->r5 = ctx->r5 & 0X10;
    after_0:
    // 0x800D472C: lbu         $a1, 0x0($s2)
    ctx->r5 = MEM_BU(ctx->r18, 0X0);
    // 0x800D4730: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x800D4734: jal         0x800D513C
    // 0x800D4738: andi        $a1, $a1, 0x10
    ctx->r5 = ctx->r5 & 0X10;
    func_800D513C(rdram, ctx);
        goto after_1;
    // 0x800D4738: andi        $a1, $a1, 0x10
    ctx->r5 = ctx->r5 & 0X10;
    after_1:
    // 0x800D473C: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D4740: addiu       $s0, $s0, 0x440
    ctx->r16 = ADD32(ctx->r16, 0X440);
    // 0x800D4744: lbu         $a1, 0x0($s1)
    ctx->r5 = MEM_BU(ctx->r17, 0X0);
    // 0x800D4748: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D474C: jal         0x800D5D04
    // 0x800D4750: andi        $a1, $a1, 0x20
    ctx->r5 = ctx->r5 & 0X20;
    func_800D5D04(rdram, ctx);
        goto after_2;
    // 0x800D4750: andi        $a1, $a1, 0x20
    ctx->r5 = ctx->r5 & 0X20;
    after_2:
    // 0x800D4754: lbu         $a1, 0x0($s2)
    ctx->r5 = MEM_BU(ctx->r18, 0X0);
    // 0x800D4758: addiu       $a0, $s0, 0x118
    ctx->r4 = ADD32(ctx->r16, 0X118);
    // 0x800D475C: jal         0x800D5D04
    // 0x800D4760: andi        $a1, $a1, 0x20
    ctx->r5 = ctx->r5 & 0X20;
    func_800D5D04(rdram, ctx);
        goto after_3;
    // 0x800D4760: andi        $a1, $a1, 0x20
    ctx->r5 = ctx->r5 & 0X20;
    after_3:
L_800D4764:
    // 0x800D4764: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D4768: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D476C: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D4770: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D4774: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800D4778: jr          $ra
    // 0x800D477C: nop

    return;
    // 0x800D477C: nop

;}
RECOMP_FUNC void func_800D4780(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D4780: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800D4784: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4788: lwc1        $f2, -0x9BC($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X9BC);
    // 0x800D478C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4790: lwc1        $f4, -0x9B8($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X9B8);
    // 0x800D4794: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4798: lwc1        $f6, -0x9B4($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X9B4);
    // 0x800D479C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D47A0: lwc1        $f8, -0x9B0($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X9B0);
    // 0x800D47A4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D47A8: lwc1        $f10, -0x9AC($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X9AC);
    // 0x800D47AC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D47B0: lwc1        $f12, -0x9A8($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X9A8);
    // 0x800D47B4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D47B8: lwc1        $f14, -0x9A4($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X9A4);
    // 0x800D47BC: lui         $a1, 0xB7F0
    ctx->r5 = S32(0XB7F0 << 16);
    // 0x800D47C0: lb          $a1, 0x0($a1)
    ctx->r5 = MEM_B(ctx->r5, 0X0);
    // 0x800D47C4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D47C8: lwc1        $f0, -0x9A0($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X9A0);
    // 0x800D47CC: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x800D47D0: addiu       $a0, $a0, -0x9CC
    ctx->r4 = ADD32(ctx->r4, -0X9CC);
    // 0x800D47D4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D47D8: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800D47DC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D47E0: swc1        $f2, 0x6584($at)
    MEM_W(0X6584, ctx->r1) = ctx->f2.u32l;
    // 0x800D47E4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D47E8: swc1        $f4, 0x6588($at)
    MEM_W(0X6588, ctx->r1) = ctx->f4.u32l;
    // 0x800D47EC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D47F0: swc1        $f6, 0x658C($at)
    MEM_W(0X658C, ctx->r1) = ctx->f6.u32l;
    // 0x800D47F4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D47F8: swc1        $f8, 0x6590($at)
    MEM_W(0X6590, ctx->r1) = ctx->f8.u32l;
    // 0x800D47FC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4800: swc1        $f10, 0x65A8($at)
    MEM_W(0X65A8, ctx->r1) = ctx->f10.u32l;
    // 0x800D4804: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4808: swc1        $f12, 0x65AC($at)
    MEM_W(0X65AC, ctx->r1) = ctx->f12.u32l;
    // 0x800D480C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4810: swc1        $f14, 0x65B0($at)
    MEM_W(0X65B0, ctx->r1) = ctx->f14.u32l;
    // 0x800D4814: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4818: swc1        $f0, 0x65B4($at)
    MEM_W(0X65B4, ctx->r1) = ctx->f0.u32l;
    // 0x800D481C: jal         0x80141210
    // 0x800D4820: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    static_0_80141210(rdram, ctx);
        goto after_0;
    // 0x800D4820: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    after_0:
    // 0x800D4824: ori         $s0, $s0, 0x10
    ctx->r16 = ctx->r16 | 0X10;
    // 0x800D4828: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D482C: lui         $at, 0xB7F0
    ctx->r1 = S32(0XB7F0 << 16);
    // 0x800D4830: sb          $s0, 0x0($at)
    MEM_B(0X0, ctx->r1) = ctx->r16;
    // 0x800D4834: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x800D4838: sw          $zero, 0x7D10($at)
    MEM_W(0X7D10, ctx->r1) = 0;
    // 0x800D483C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D4840: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800D4844: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800D4848: jr          $ra
    // 0x800D484C: nop

    return;
    // 0x800D484C: nop

;}
RECOMP_FUNC void func_800D4850(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D4850: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800D4854: lui         $a1, 0xB7F0
    ctx->r5 = S32(0XB7F0 << 16);
    // 0x800D4858: lb          $a1, 0x0($a1)
    ctx->r5 = MEM_B(ctx->r5, 0X0);
    // 0x800D485C: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x800D4860: addiu       $a0, $a0, -0x9CC
    ctx->r4 = ADD32(ctx->r4, -0X9CC);
    // 0x800D4864: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800D4868: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800D486C: jal         0x80141210
    // 0x800D4870: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    static_0_80141210(rdram, ctx);
        goto after_0;
    // 0x800D4870: addu        $s0, $a1, $zero
    ctx->r16 = ADD32(ctx->r5, 0);
    after_0:
    // 0x800D4874: ori         $s0, $s0, 0x10
    ctx->r16 = ctx->r16 | 0X10;
    // 0x800D4878: lui         $at, 0xB7F0
    ctx->r1 = S32(0XB7F0 << 16);
    // 0x800D487C: sb          $s0, 0x0($at)
    MEM_B(0X0, ctx->r1) = ctx->r16;
    // 0x800D4880: jal         0x800D49B8
    // 0x800D4884: nop

    func_800D49B8(rdram, ctx);
        goto after_1;
    // 0x800D4884: nop

    after_1:
    // 0x800D4888: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D488C: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x800D4890: sw          $zero, 0x7D10($at)
    MEM_W(0X7D10, ctx->r1) = 0;
    // 0x800D4894: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800D4898: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800D489C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800D48A0: jr          $ra
    // 0x800D48A4: nop

    return;
    // 0x800D48A4: nop

;}
RECOMP_FUNC void func_800D48A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D48A8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800D48AC: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x800D48B0: addiu       $a0, $zero, 0x39
    ctx->r4 = ADD32(0, 0X39);
    // 0x800D48B4: jal         0x80146168
    // 0x800D48B8: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    func_80146168(rdram, ctx);
        goto after_0;
    // 0x800D48B8: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    after_0:
    // 0x800D48BC: bne         $v0, $zero, L_800D48D0
    if (ctx->r2 != 0) {
        // 0x800D48C0: addiu       $a0, $zero, 0x39
        ctx->r4 = ADD32(0, 0X39);
            goto L_800D48D0;
    }
    // 0x800D48C0: addiu       $a0, $zero, 0x39
    ctx->r4 = ADD32(0, 0X39);
    // 0x800D48C4: lw          $v0, 0x10($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X10);
    // 0x800D48C8: beq         $v0, $zero, L_800D48DC
    if (ctx->r2 == 0) {
        // 0x800D48CC: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_800D48DC;
    }
    // 0x800D48CC: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_800D48D0:
    // 0x800D48D0: jal         0x801462A0
    // 0x800D48D4: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_801462A0(rdram, ctx);
        goto after_1;
    // 0x800D48D4: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_1:
    // 0x800D48D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_800D48DC:
    // 0x800D48DC: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x800D48E0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800D48E4: jr          $ra
    // 0x800D48E8: nop

    return;
    // 0x800D48E8: nop

;}
RECOMP_FUNC void func_800D48EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D48EC: sll         $v1, $a0, 3
    ctx->r3 = S32(ctx->r4 << 3);
    // 0x800D48F0: addu        $v1, $v1, $a0
    ctx->r3 = ADD32(ctx->r3, ctx->r4);
    // 0x800D48F4: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x800D48F8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D48FC: lwc1        $f0, -0x99C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X99C);
    // 0x800D4900: addiu       $a0, $zero, 0x14A
    ctx->r4 = ADD32(0, 0X14A);
    // 0x800D4904: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4908: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x800D490C: swc1        $f0, 0x6584($at)
    MEM_W(0X6584, ctx->r1) = ctx->f0.u32l;
    // 0x800D4910: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4914: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x800D4918: lwc1        $f8, 0x6584($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X6584);
    // 0x800D491C: mtc1        $a0, $f2
    ctx->f2.u32l = ctx->r4;
    // 0x800D4920: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D4924: div.s       $f2, $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = DIV_S(ctx->f2.fl, ctx->f8.fl);
    // 0x800D4928: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D492C: lwc1        $f0, -0x998($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X998);
    // 0x800D4930: addiu       $v0, $zero, 0x24
    ctx->r2 = ADD32(0, 0X24);
    // 0x800D4934: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4938: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x800D493C: swc1        $f0, 0x6588($at)
    MEM_W(0X6588, ctx->r1) = ctx->f0.u32l;
    // 0x800D4940: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4944: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x800D4948: lwc1        $f6, 0x6588($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X6588);
    // 0x800D494C: mtc1        $v0, $f0
    ctx->f0.u32l = ctx->r2;
    // 0x800D4950: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4954: div.s       $f0, $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = DIV_S(ctx->f0.fl, ctx->f6.fl);
    // 0x800D4958: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D495C: lwc1        $f4, -0x994($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X994);
    // 0x800D4960: sub.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f4.fl;
    // 0x800D4964: mul.s       $f2, $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f8.fl);
    // 0x800D4968: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x800D496C: mul.s       $f0, $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x800D4970: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4974: sw          $a0, 0x2F0($at)
    MEM_W(0X2F0, ctx->r1) = ctx->r4;
    // 0x800D4978: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D497C: sw          $v0, 0x2FC($at)
    MEM_W(0X2FC, ctx->r1) = ctx->r2;
    // 0x800D4980: addiu       $v0, $zero, 0x6B8
    ctx->r2 = ADD32(0, 0X6B8);
    // 0x800D4984: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4988: sw          $v0, 0x2F8($at)
    MEM_W(0X2F8, ctx->r1) = ctx->r2;
    // 0x800D498C: addiu       $v0, $zero, 0xDC
    ctx->r2 = ADD32(0, 0XDC);
    // 0x800D4990: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4994: sw          $v0, 0x300($at)
    MEM_W(0X300, ctx->r1) = ctx->r2;
    // 0x800D4998: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D499C: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x800D49A0: swc1        $f2, 0x658C($at)
    MEM_W(0X658C, ctx->r1) = ctx->f2.u32l;
    // 0x800D49A4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D49A8: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x800D49AC: swc1        $f0, 0x6590($at)
    MEM_W(0X6590, ctx->r1) = ctx->f0.u32l;
    // 0x800D49B0: jr          $ra
    // 0x800D49B4: nop

    return;
    // 0x800D49B4: nop

;}
RECOMP_FUNC void func_800D49B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D49B8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x800D49BC: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x800D49C0: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x800D49C4: addiu       $a0, $zero, 0x50
    ctx->r4 = ADD32(0, 0X50);
    // 0x800D49C8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D49CC: lwc1        $f0, -0x93C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X93C);
    // 0x800D49D0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D49D4: sw          $ra, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r31;
    // 0x800D49D8: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x800D49DC: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D49E0: sw          $v0, -0x6DA8($at)
    MEM_W(-0X6DA8, ctx->r1) = ctx->r2;
    // 0x800D49E4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D49E8: sw          $zero, -0x2280($at)
    MEM_W(-0X2280, ctx->r1) = 0;
    // 0x800D49EC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D49F0: swc1        $f0, 0x6584($at)
    MEM_W(0X6584, ctx->r1) = ctx->f0.u32l;
    // 0x800D49F4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D49F8: swc1        $f0, 0x6588($at)
    MEM_W(0X6588, ctx->r1) = ctx->f0.u32l;
    // 0x800D49FC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4A00: swc1        $f0, 0x65A8($at)
    MEM_W(0X65A8, ctx->r1) = ctx->f0.u32l;
    // 0x800D4A04: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4A08: swc1        $f0, 0x65AC($at)
    MEM_W(0X65AC, ctx->r1) = ctx->f0.u32l;
    // 0x800D4A0C: jal         0x80146168
    // 0x800D4A10: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    func_80146168(rdram, ctx);
        goto after_0;
    // 0x800D4A10: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    after_0:
    // 0x800D4A14: beq         $v0, $zero, L_800D4A30
    if (ctx->r2 == 0) {
        // 0x800D4A18: lui         $v0, 0x23A
        ctx->r2 = S32(0X23A << 16);
            goto L_800D4A30;
    }
    // 0x800D4A18: lui         $v0, 0x23A
    ctx->r2 = S32(0X23A << 16);
    // 0x800D4A1C: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x800D4A20: addiu       $a0, $a0, -0x990
    ctx->r4 = ADD32(ctx->r4, -0X990);
    // 0x800D4A24: jal         0x80141210
    // 0x800D4A28: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    static_0_80141210(rdram, ctx);
        goto after_1;
    // 0x800D4A28: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    after_1:
    // 0x800D4A2C: lui         $v0, 0x23A
    ctx->r2 = S32(0X23A << 16);
L_800D4A30:
    // 0x800D4A30: lw          $a1, 0x10($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X10);
    // 0x800D4A34: ori         $v0, $v0, 0x4729
    ctx->r2 = ctx->r2 | 0X4729;
    // 0x800D4A38: beq         $a1, $v0, L_800D4A58
    if (ctx->r5 == ctx->r2) {
        // 0x800D4A3C: addu        $a2, $a1, $zero
        ctx->r6 = ADD32(ctx->r5, 0);
            goto L_800D4A58;
    }
    // 0x800D4A3C: addu        $a2, $a1, $zero
    ctx->r6 = ADD32(ctx->r5, 0);
    // 0x800D4A40: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x800D4A44: addiu       $a0, $a0, -0x978
    ctx->r4 = ADD32(ctx->r4, -0X978);
    // 0x800D4A48: lui         $a3, 0x23A
    ctx->r7 = S32(0X23A << 16);
    // 0x800D4A4C: jal         0x80141210
    // 0x800D4A50: ori         $a3, $a3, 0x4729
    ctx->r7 = ctx->r7 | 0X4729;
    static_0_80141210(rdram, ctx);
        goto after_2;
    // 0x800D4A50: ori         $a3, $a3, 0x4729
    ctx->r7 = ctx->r7 | 0X4729;
    after_2:
    // 0x800D4A54: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
L_800D4A58:
    // 0x800D4A58: addiu       $a0, $zero, 0x39
    ctx->r4 = ADD32(0, 0X39);
    // 0x800D4A5C: jal         0x80146168
    // 0x800D4A60: addiu       $a1, $sp, 0x18
    ctx->r5 = ADD32(ctx->r29, 0X18);
    func_80146168(rdram, ctx);
        goto after_3;
    // 0x800D4A60: addiu       $a1, $sp, 0x18
    ctx->r5 = ADD32(ctx->r29, 0X18);
    after_3:
    // 0x800D4A64: bne         $v0, $zero, L_800D4A78
    if (ctx->r2 != 0) {
        // 0x800D4A68: addiu       $a0, $zero, 0x39
        ctx->r4 = ADD32(0, 0X39);
            goto L_800D4A78;
    }
    // 0x800D4A68: addiu       $a0, $zero, 0x39
    ctx->r4 = ADD32(0, 0X39);
    // 0x800D4A6C: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x800D4A70: beq         $v0, $zero, L_800D4A84
    if (ctx->r2 == 0) {
        // 0x800D4A74: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_800D4A84;
    }
    // 0x800D4A74: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_800D4A78:
    // 0x800D4A78: jal         0x801462A0
    // 0x800D4A7C: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_801462A0(rdram, ctx);
        goto after_4;
    // 0x800D4A7C: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_4:
    // 0x800D4A80: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_800D4A84:
    // 0x800D4A84: beq         $v0, $zero, L_800D4A9C
    if (ctx->r2 == 0) {
        // 0x800D4A88: nop
    
            goto L_800D4A9C;
    }
    // 0x800D4A88: nop

    // 0x800D4A8C: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x800D4A90: addiu       $a0, $a0, -0x94C
    ctx->r4 = ADD32(ctx->r4, -0X94C);
    // 0x800D4A94: jal         0x80141210
    // 0x800D4A98: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    static_0_80141210(rdram, ctx);
        goto after_5;
    // 0x800D4A98: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    after_5:
L_800D4A9C:
    // 0x800D4A9C: beq         $s0, $zero, L_800D4B5C
    if (ctx->r16 == 0) {
        // 0x800D4AA0: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D4B5C;
    }
    // 0x800D4AA0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D4AA4: lui         $s0, 0x8021
    ctx->r16 = S32(0X8021 << 16);
    // 0x800D4AA8: addiu       $s0, $s0, 0x22D4
    ctx->r16 = ADD32(ctx->r16, 0X22D4);
    // 0x800D4AAC: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x800D4AB0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x800D4AB4: sw          $v0, 0x25C4($at)
    MEM_W(0X25C4, ctx->r1) = ctx->r2;
    // 0x800D4AB8: jal         0x800D48EC
    // 0x800D4ABC: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    func_800D48EC(rdram, ctx);
        goto after_6;
    // 0x800D4ABC: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_6:
    // 0x800D4AC0: jal         0x800D48EC
    // 0x800D4AC4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    func_800D48EC(rdram, ctx);
        goto after_7;
    // 0x800D4AC4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_7:
    // 0x800D4AC8: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4ACC: lw          $a1, 0x6584($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X6584);
    // 0x800D4AD0: addiu       $a0, $zero, 0x51
    ctx->r4 = ADD32(0, 0X51);
    // 0x800D4AD4: jal         0x800F2130
    // 0x800D4AD8: sw          $s1, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r17;
    func_800F2130(rdram, ctx);
        goto after_8;
    // 0x800D4AD8: sw          $s1, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r17;
    after_8:
    // 0x800D4ADC: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4AE0: lw          $a1, 0x6588($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X6588);
    // 0x800D4AE4: jal         0x800F2130
    // 0x800D4AE8: addiu       $a0, $zero, 0x54
    ctx->r4 = ADD32(0, 0X54);
    func_800F2130(rdram, ctx);
        goto after_9;
    // 0x800D4AE8: addiu       $a0, $zero, 0x54
    ctx->r4 = ADD32(0, 0X54);
    after_9:
    // 0x800D4AEC: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4AF0: lw          $a1, 0x658C($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X658C);
    // 0x800D4AF4: jal         0x800F2130
    // 0x800D4AF8: addiu       $a0, $zero, 0x57
    ctx->r4 = ADD32(0, 0X57);
    func_800F2130(rdram, ctx);
        goto after_10;
    // 0x800D4AF8: addiu       $a0, $zero, 0x57
    ctx->r4 = ADD32(0, 0X57);
    after_10:
    // 0x800D4AFC: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4B00: lw          $a1, 0x6590($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X6590);
    // 0x800D4B04: jal         0x800F2130
    // 0x800D4B08: addiu       $a0, $zero, 0x5A
    ctx->r4 = ADD32(0, 0X5A);
    func_800F2130(rdram, ctx);
        goto after_11;
    // 0x800D4B08: addiu       $a0, $zero, 0x5A
    ctx->r4 = ADD32(0, 0X5A);
    after_11:
    // 0x800D4B0C: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4B10: lw          $a1, 0x65A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X65A8);
    // 0x800D4B14: jal         0x800F2130
    // 0x800D4B18: addiu       $a0, $zero, 0x5D
    ctx->r4 = ADD32(0, 0X5D);
    func_800F2130(rdram, ctx);
        goto after_12;
    // 0x800D4B18: addiu       $a0, $zero, 0x5D
    ctx->r4 = ADD32(0, 0X5D);
    after_12:
    // 0x800D4B1C: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4B20: lw          $a1, 0x65AC($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X65AC);
    // 0x800D4B24: jal         0x800F2130
    // 0x800D4B28: addiu       $a0, $zero, 0x60
    ctx->r4 = ADD32(0, 0X60);
    func_800F2130(rdram, ctx);
        goto after_13;
    // 0x800D4B28: addiu       $a0, $zero, 0x60
    ctx->r4 = ADD32(0, 0X60);
    after_13:
    // 0x800D4B2C: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4B30: lw          $a1, 0x65B0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X65B0);
    // 0x800D4B34: jal         0x800F2130
    // 0x800D4B38: addiu       $a0, $zero, 0x63
    ctx->r4 = ADD32(0, 0X63);
    func_800F2130(rdram, ctx);
        goto after_14;
    // 0x800D4B38: addiu       $a0, $zero, 0x63
    ctx->r4 = ADD32(0, 0X63);
    after_14:
    // 0x800D4B3C: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4B40: lw          $a1, 0x65B4($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X65B4);
    // 0x800D4B44: jal         0x800F2130
    // 0x800D4B48: addiu       $a0, $zero, 0x66
    ctx->r4 = ADD32(0, 0X66);
    func_800F2130(rdram, ctx);
        goto after_15;
    // 0x800D4B48: addiu       $a0, $zero, 0x66
    ctx->r4 = ADD32(0, 0X66);
    after_15:
    // 0x800D4B4C: addiu       $a0, $zero, 0x50
    ctx->r4 = ADD32(0, 0X50);
    // 0x800D4B50: lui         $a1, 0x23A
    ctx->r5 = S32(0X23A << 16);
    // 0x800D4B54: jal         0x801462A0
    // 0x800D4B58: ori         $a1, $a1, 0x4729
    ctx->r5 = ctx->r5 | 0X4729;
    func_801462A0(rdram, ctx);
        goto after_16;
    // 0x800D4B58: ori         $a1, $a1, 0x4729
    ctx->r5 = ctx->r5 | 0X4729;
    after_16:
L_800D4B5C:
    // 0x800D4B5C: addiu       $a0, $zero, 0x51
    ctx->r4 = ADD32(0, 0X51);
    // 0x800D4B60: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D4B64: addiu       $s0, $s0, 0x6584
    ctx->r16 = ADD32(ctx->r16, 0X6584);
    // 0x800D4B68: lui         $a2, 0x4080
    ctx->r6 = S32(0X4080 << 16);
    // 0x800D4B6C: ori         $a2, $a2, 0xC0C2
    ctx->r6 = ctx->r6 | 0XC0C2;
    // 0x800D4B70: jal         0x800F2188
    // 0x800D4B74: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    func_800F2188(rdram, ctx);
        goto after_17;
    // 0x800D4B74: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_17:
    // 0x800D4B78: addiu       $a0, $zero, 0x54
    ctx->r4 = ADD32(0, 0X54);
    // 0x800D4B7C: lui         $a2, 0x3F72
    ctx->r6 = S32(0X3F72 << 16);
    // 0x800D4B80: ori         $a2, $a2, 0x86B6
    ctx->r6 = ctx->r6 | 0X86B6;
    // 0x800D4B84: jal         0x800F2188
    // 0x800D4B88: addiu       $a1, $s0, 0x4
    ctx->r5 = ADD32(ctx->r16, 0X4);
    func_800F2188(rdram, ctx);
        goto after_18;
    // 0x800D4B88: addiu       $a1, $s0, 0x4
    ctx->r5 = ADD32(ctx->r16, 0X4);
    after_18:
    // 0x800D4B8C: addiu       $a0, $zero, 0x57
    ctx->r4 = ADD32(0, 0X57);
    // 0x800D4B90: lui         $a2, 0x438E
    ctx->r6 = S32(0X438E << 16);
    // 0x800D4B94: ori         $a2, $a2, 0xC3C4
    ctx->r6 = ctx->r6 | 0XC3C4;
    // 0x800D4B98: jal         0x800F2188
    // 0x800D4B9C: addiu       $a1, $s0, 0x8
    ctx->r5 = ADD32(ctx->r16, 0X8);
    func_800F2188(rdram, ctx);
        goto after_19;
    // 0x800D4B9C: addiu       $a1, $s0, 0x8
    ctx->r5 = ADD32(ctx->r16, 0X8);
    after_19:
    // 0x800D4BA0: addiu       $a0, $zero, 0x5A
    ctx->r4 = ADD32(0, 0X5A);
    // 0x800D4BA4: lui         $a2, 0x41C8
    ctx->r6 = S32(0X41C8 << 16);
    // 0x800D4BA8: ori         $a2, $a2, 0x6BCA
    ctx->r6 = ctx->r6 | 0X6BCA;
    // 0x800D4BAC: jal         0x800F2188
    // 0x800D4BB0: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    func_800F2188(rdram, ctx);
        goto after_20;
    // 0x800D4BB0: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    after_20:
    // 0x800D4BB4: addiu       $a0, $zero, 0x5D
    ctx->r4 = ADD32(0, 0X5D);
    // 0x800D4BB8: lui         $a2, 0x4080
    ctx->r6 = S32(0X4080 << 16);
    // 0x800D4BBC: ori         $a2, $a2, 0x6061
    ctx->r6 = ctx->r6 | 0X6061;
    // 0x800D4BC0: jal         0x800F2188
    // 0x800D4BC4: addiu       $a1, $s0, 0x24
    ctx->r5 = ADD32(ctx->r16, 0X24);
    func_800F2188(rdram, ctx);
        goto after_21;
    // 0x800D4BC4: addiu       $a1, $s0, 0x24
    ctx->r5 = ADD32(ctx->r16, 0X24);
    after_21:
    // 0x800D4BC8: addiu       $a0, $zero, 0x60
    ctx->r4 = ADD32(0, 0X60);
    // 0x800D4BCC: lui         $a2, 0x3F65
    ctx->r6 = S32(0X3F65 << 16);
    // 0x800D4BD0: ori         $a2, $a2, 0xD7C
    ctx->r6 = ctx->r6 | 0XD7C;
    // 0x800D4BD4: jal         0x800F2188
    // 0x800D4BD8: addiu       $a1, $s0, 0x28
    ctx->r5 = ADD32(ctx->r16, 0X28);
    func_800F2188(rdram, ctx);
        goto after_22;
    // 0x800D4BD8: addiu       $a1, $s0, 0x28
    ctx->r5 = ADD32(ctx->r16, 0X28);
    after_22:
    // 0x800D4BDC: addiu       $a0, $zero, 0x63
    ctx->r4 = ADD32(0, 0X63);
    // 0x800D4BE0: lui         $a2, 0x4387
    ctx->r6 = S32(0X4387 << 16);
    // 0x800D4BE4: ori         $a2, $a2, 0xE1E2
    ctx->r6 = ctx->r6 | 0XE1E2;
    // 0x800D4BE8: jal         0x800F2188
    // 0x800D4BEC: addiu       $a1, $s0, 0x2C
    ctx->r5 = ADD32(ctx->r16, 0X2C);
    func_800F2188(rdram, ctx);
        goto after_23;
    // 0x800D4BEC: addiu       $a1, $s0, 0x2C
    ctx->r5 = ADD32(ctx->r16, 0X2C);
    after_23:
    // 0x800D4BF0: addiu       $a0, $zero, 0x66
    ctx->r4 = ADD32(0, 0X66);
    // 0x800D4BF4: lui         $a2, 0x421C
    ctx->r6 = S32(0X421C << 16);
    // 0x800D4BF8: ori         $a2, $a2, 0x6BCA
    ctx->r6 = ctx->r6 | 0X6BCA;
    // 0x800D4BFC: jal         0x800F2188
    // 0x800D4C00: addiu       $a1, $s0, 0x30
    ctx->r5 = ADD32(ctx->r16, 0X30);
    func_800F2188(rdram, ctx);
        goto after_24;
    // 0x800D4C00: addiu       $a1, $s0, 0x30
    ctx->r5 = ADD32(ctx->r16, 0X30);
    after_24:
    // 0x800D4C04: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4C08: sw          $zero, -0x2280($at)
    MEM_W(-0X2280, ctx->r1) = 0;
    // 0x800D4C0C: lw          $ra, 0x28($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X28);
    // 0x800D4C10: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x800D4C14: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x800D4C18: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x800D4C1C: jr          $ra
    // 0x800D4C20: nop

    return;
    // 0x800D4C20: nop

;}
RECOMP_FUNC void func_800D4C24_original(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D4C24: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x800D4C28: lui         $a2, 0xB680
    ctx->r6 = S32(0XB680 << 16);
    // 0x800D4C2C: ori         $a2, $a2, 0x4
    ctx->r6 = ctx->r6 | 0X4;
    // 0x800D4C30: lui         $v1, 0xB680
    ctx->r3 = S32(0XB680 << 16);
    // 0x800D4C34: ori         $v1, $v1, 0x8
    ctx->r3 = ctx->r3 | 0X8;
    // 0x800D4C38: lui         $a3, 0xB680
    ctx->r7 = S32(0XB680 << 16);
    // 0x800D4C3C: ori         $a3, $a3, 0xC
    ctx->r7 = ctx->r7 | 0XC;
    // 0x800D4C40: lui         $t1, 0xB680
    ctx->r9 = S32(0XB680 << 16);
    // 0x800D4C44: ori         $t1, $t1, 0x10
    ctx->r9 = ctx->r9 | 0X10;
    // 0x800D4C48: lui         $t0, 0xB680
    ctx->r8 = S32(0XB680 << 16);
    // 0x800D4C4C: ori         $t0, $t0, 0x14
    ctx->r8 = ctx->r8 | 0X14;
    // 0x800D4C50: lui         $t3, 0xB680
    ctx->r11 = S32(0XB680 << 16);
    // 0x800D4C54: ori         $t3, $t3, 0x18
    ctx->r11 = ctx->r11 | 0X18;
    // 0x800D4C58: lui         $t2, 0xB680
    ctx->r10 = S32(0XB680 << 16);
    // 0x800D4C5C: ori         $t2, $t2, 0x1C
    ctx->r10 = ctx->r10 | 0X1C;
    // 0x800D4C60: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x800D4C64: addiu       $a1, $a1, 0x6578
    ctx->r5 = ADD32(ctx->r5, 0X6578);
    // 0x800D4C68: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4C6C: lwc1        $f12, 0x65A8($at)
    ctx->f12.u32l = MEM_W(ctx->r1, 0X65A8);
    // 0x800D4C70: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4C74: lwc1        $f0, -0x938($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X938);
    // 0x800D4C78: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D4C7C: sw          $ra, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r31;
    // 0x800D4C80: sw          $s5, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r21;
    // 0x800D4C84: sw          $s4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r20;
    // 0x800D4C88: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x800D4C8C: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D4C90: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D4C94: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D4C98: lbu         $a0, 0x0($v0)
    ctx->r4 = MEM_BU(ctx->r2, 0X0);
    // 0x800D4C9C: lbu         $v0, 0x0($a2)
    ctx->r2 = MEM_BU(ctx->r6, 0X0);
    // 0x800D4CA0: andi        $v0, $v0, 0xF
    ctx->r2 = ctx->r2 & 0XF;
    // 0x800D4CA4: sll         $v0, $v0, 8
    ctx->r2 = S32(ctx->r2 << 8);
    // 0x800D4CA8: sw          $a0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r4;
    // 0x800D4CAC: or          $a0, $a0, $v0
    ctx->r4 = ctx->r4 | ctx->r2;
    // 0x800D4CB0: sw          $a0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r4;
    // 0x800D4CB4: lbu         $v1, 0x0($v1)
    ctx->r3 = MEM_BU(ctx->r3, 0X0);
    // 0x800D4CB8: lbu         $v0, 0x0($a3)
    ctx->r2 = MEM_BU(ctx->r7, 0X0);
    // 0x800D4CBC: andi        $a3, $a0, 0xFFF
    ctx->r7 = ctx->r4 & 0XFFF;
    // 0x800D4CC0: andi        $v0, $v0, 0x3
    ctx->r2 = ctx->r2 & 0X3;
    // 0x800D4CC4: sll         $v0, $v0, 8
    ctx->r2 = S32(ctx->r2 << 8);
    // 0x800D4CC8: or          $t4, $v1, $v0
    ctx->r12 = ctx->r3 | ctx->r2;
    // 0x800D4CCC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4CD0: sw          $v1, 0x657C($at)
    MEM_W(0X657C, ctx->r1) = ctx->r3;
    // 0x800D4CD4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4CD8: sw          $t4, 0x657C($at)
    MEM_W(0X657C, ctx->r1) = ctx->r12;
    // 0x800D4CDC: lbu         $v0, 0x0($a2)
    ctx->r2 = MEM_BU(ctx->r6, 0X0);
    // 0x800D4CE0: c.eq.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl == ctx->f0.fl;
    // 0x800D4CE4: sw          $a3, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r7;
    // 0x800D4CE8: andi        $v0, $v0, 0x40
    ctx->r2 = ctx->r2 & 0X40;
    // 0x800D4CEC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4CF0: sw          $v0, 0x6580($at)
    MEM_W(0X6580, ctx->r1) = ctx->r2;
    // 0x800D4CF4: lbu         $a0, 0x0($t1)
    ctx->r4 = MEM_BU(ctx->r9, 0X0);
    // 0x800D4CF8: lbu         $v0, 0x0($t0)
    ctx->r2 = MEM_BU(ctx->r8, 0X0);
    // 0x800D4CFC: andi        $v0, $v0, 0xF
    ctx->r2 = ctx->r2 & 0XF;
    // 0x800D4D00: sll         $v0, $v0, 8
    ctx->r2 = S32(ctx->r2 << 8);
    // 0x800D4D04: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4D08: sw          $a0, 0x659C($at)
    MEM_W(0X659C, ctx->r1) = ctx->r4;
    // 0x800D4D0C: or          $a0, $a0, $v0
    ctx->r4 = ctx->r4 | ctx->r2;
    // 0x800D4D10: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4D14: sw          $a0, 0x659C($at)
    MEM_W(0X659C, ctx->r1) = ctx->r4;
    // 0x800D4D18: lbu         $v1, 0x0($t3)
    ctx->r3 = MEM_BU(ctx->r11, 0X0);
    // 0x800D4D1C: lbu         $v0, 0x0($t2)
    ctx->r2 = MEM_BU(ctx->r10, 0X0);
    // 0x800D4D20: andi        $v0, $v0, 0x3
    ctx->r2 = ctx->r2 & 0X3;
    // 0x800D4D24: sll         $v0, $v0, 8
    ctx->r2 = S32(ctx->r2 << 8);
    // 0x800D4D28: or          $a1, $v1, $v0
    ctx->r5 = ctx->r3 | ctx->r2;
    // 0x800D4D2C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4D30: sw          $v1, 0x65A0($at)
    MEM_W(0X65A0, ctx->r1) = ctx->r3;
    // 0x800D4D34: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4D38: sw          $a1, 0x65A0($at)
    MEM_W(0X65A0, ctx->r1) = ctx->r5;
    // 0x800D4D3C: lbu         $v0, 0x0($t0)
    ctx->r2 = MEM_BU(ctx->r8, 0X0);
    // 0x800D4D40: andi        $a0, $a0, 0xFFF
    ctx->r4 = ctx->r4 & 0XFFF;
    // 0x800D4D44: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4D48: sw          $a0, 0x659C($at)
    MEM_W(0X659C, ctx->r1) = ctx->r4;
    // 0x800D4D4C: andi        $v0, $v0, 0x40
    ctx->r2 = ctx->r2 & 0X40;
    // 0x800D4D50: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4D54: sw          $v0, 0x65A4($at)
    MEM_W(0X65A4, ctx->r1) = ctx->r2;
    // 0x800D4D58: lbu         $v1, 0x0($a2)
    ctx->r3 = MEM_BU(ctx->r6, 0X0);
    // 0x800D4D5C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x800D4D60: lw          $v0, 0x24CC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X24CC);
    // 0x800D4D64: andi        $v1, $v1, 0x7F
    ctx->r3 = ctx->r3 & 0X7F;
    // 0x800D4D68: andi        $v0, $v0, 0x4
    ctx->r2 = ctx->r2 & 0X4;
    // 0x800D4D6C: sll         $v0, $v0, 5
    ctx->r2 = S32(ctx->r2 << 5);
    // 0x800D4D70: or          $v1, $v1, $v0
    ctx->r3 = ctx->r3 | ctx->r2;
    // 0x800D4D74: bc1t        L_800D4F44
    if (c1cs) {
        // 0x800D4D78: sb          $v1, 0x0($a2)
        MEM_B(0X0, ctx->r6) = ctx->r3;
            goto L_800D4F44;
    }
    // 0x800D4D78: sb          $v1, 0x0($a2)
    MEM_B(0X0, ctx->r6) = ctx->r3;
    // 0x800D4D7C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4D80: lwc1        $f2, 0x658C($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X658C);
    // 0x800D4D84: mtc1        $a3, $f0
    ctx->f0.u32l = ctx->r7;
    // 0x800D4D88: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4D8C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D4D90: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4D94: lwc1        $f2, 0x6584($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6584);
    // 0x800D4D98: div.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D4D9C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4DA0: lwc1        $f4, -0x934($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X934);
    // 0x800D4DA4: trunc.w.s   $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.u32l = TRUNC_W_S(ctx->f0.fl);
    // 0x800D4DA8: mfc1        $v0, $f2
    ctx->r2 = (int32_t)ctx->f2.u32l;
    // 0x800D4DAC: mtc1        $v0, $f0
    ctx->f0.u32l = ctx->r2;
    // 0x800D4DB0: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4DB4: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x800D4DB8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4DBC: sw          $v0, 0x6594($at)
    MEM_W(0X6594, ctx->r1) = ctx->r2;
    // 0x800D4DC0: bc1f        L_800D4DD0
    if (!c1cs) {
        // 0x800D4DC4: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D4DD0;
    }
    // 0x800D4DC4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D4DC8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4DCC: sw          $v0, 0x6594($at)
    MEM_W(0X6594, ctx->r1) = ctx->r2;
L_800D4DD0:
    // 0x800D4DD0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4DD4: lwc1        $f10, -0x930($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X930);
    // 0x800D4DD8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4DDC: lwc1        $f0, 0x6594($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X6594);
    // 0x800D4DE0: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4DE4: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x800D4DE8: nop

    // 0x800D4DEC: bc1f        L_800D4DFC
    if (!c1cs) {
        // 0x800D4DF0: addiu       $v0, $zero, 0x18F
        ctx->r2 = ADD32(0, 0X18F);
            goto L_800D4DFC;
    }
    // 0x800D4DF0: addiu       $v0, $zero, 0x18F
    ctx->r2 = ADD32(0, 0X18F);
    // 0x800D4DF4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4DF8: sw          $v0, 0x6594($at)
    MEM_W(0X6594, ctx->r1) = ctx->r2;
L_800D4DFC:
    // 0x800D4DFC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4E00: lwc1        $f2, 0x6590($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6590);
    // 0x800D4E04: mtc1        $t4, $f0
    ctx->f0.u32l = ctx->r12;
    // 0x800D4E08: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4E0C: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D4E10: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4E14: lwc1        $f2, 0x6588($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6588);
    // 0x800D4E18: div.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D4E1C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4E20: lwc1        $f8, -0x92C($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X92C);
    // 0x800D4E24: sub.s       $f0, $f8, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f8.fl - ctx->f0.fl;
    // 0x800D4E28: trunc.w.s   $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.u32l = TRUNC_W_S(ctx->f0.fl);
    // 0x800D4E2C: mfc1        $v0, $f2
    ctx->r2 = (int32_t)ctx->f2.u32l;
    // 0x800D4E30: mtc1        $v0, $f0
    ctx->f0.u32l = ctx->r2;
    // 0x800D4E34: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4E38: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x800D4E3C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4E40: sw          $v0, 0x6598($at)
    MEM_W(0X6598, ctx->r1) = ctx->r2;
    // 0x800D4E44: bc1f        L_800D4E54
    if (!c1cs) {
        // 0x800D4E48: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D4E54;
    }
    // 0x800D4E48: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D4E4C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4E50: sw          $v0, 0x6594($at)
    MEM_W(0X6594, ctx->r1) = ctx->r2;
L_800D4E54:
    // 0x800D4E54: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D4E58: lwc1        $f6, -0x928($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X928);
    // 0x800D4E5C: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x800D4E60: nop

    // 0x800D4E64: bc1f        L_800D4E74
    if (!c1cs) {
        // 0x800D4E68: addiu       $v0, $zero, 0xFF
        ctx->r2 = ADD32(0, 0XFF);
            goto L_800D4E74;
    }
    // 0x800D4E68: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
    // 0x800D4E6C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4E70: sw          $v0, 0x6594($at)
    MEM_W(0X6594, ctx->r1) = ctx->r2;
L_800D4E74:
    // 0x800D4E74: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4E78: lwc1        $f2, 0x65B0($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X65B0);
    // 0x800D4E7C: mtc1        $a0, $f0
    ctx->f0.u32l = ctx->r4;
    // 0x800D4E80: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4E84: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D4E88: div.s       $f0, $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = DIV_S(ctx->f0.fl, ctx->f12.fl);
    // 0x800D4E8C: trunc.w.s   $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.u32l = TRUNC_W_S(ctx->f0.fl);
    // 0x800D4E90: mfc1        $v0, $f2
    ctx->r2 = (int32_t)ctx->f2.u32l;
    // 0x800D4E94: mtc1        $v0, $f0
    ctx->f0.u32l = ctx->r2;
    // 0x800D4E98: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4E9C: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x800D4EA0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4EA4: sw          $v0, 0x65B8($at)
    MEM_W(0X65B8, ctx->r1) = ctx->r2;
    // 0x800D4EA8: bc1f        L_800D4EB8
    if (!c1cs) {
        // 0x800D4EAC: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D4EB8;
    }
    // 0x800D4EAC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D4EB0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4EB4: sw          $v0, 0x65B8($at)
    MEM_W(0X65B8, ctx->r1) = ctx->r2;
L_800D4EB8:
    // 0x800D4EB8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4EBC: lwc1        $f0, 0x65B8($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X65B8);
    // 0x800D4EC0: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4EC4: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x800D4EC8: nop

    // 0x800D4ECC: bc1f        L_800D4EDC
    if (!c1cs) {
        // 0x800D4ED0: addiu       $v0, $zero, 0x18F
        ctx->r2 = ADD32(0, 0X18F);
            goto L_800D4EDC;
    }
    // 0x800D4ED0: addiu       $v0, $zero, 0x18F
    ctx->r2 = ADD32(0, 0X18F);
    // 0x800D4ED4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4ED8: sw          $v0, 0x65B8($at)
    MEM_W(0X65B8, ctx->r1) = ctx->r2;
L_800D4EDC:
    // 0x800D4EDC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4EE0: lwc1        $f2, 0x65B4($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X65B4);
    // 0x800D4EE4: mtc1        $a1, $f0
    ctx->f0.u32l = ctx->r5;
    // 0x800D4EE8: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4EEC: sub.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x800D4EF0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4EF4: lwc1        $f2, 0x65AC($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X65AC);
    // 0x800D4EF8: div.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D4EFC: sub.s       $f0, $f8, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = ctx->f8.fl - ctx->f0.fl;
    // 0x800D4F00: trunc.w.s   $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.u32l = TRUNC_W_S(ctx->f0.fl);
    // 0x800D4F04: mfc1        $v0, $f2
    ctx->r2 = (int32_t)ctx->f2.u32l;
    // 0x800D4F08: mtc1        $v0, $f0
    ctx->f0.u32l = ctx->r2;
    // 0x800D4F0C: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D4F10: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x800D4F14: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4F18: sw          $v0, 0x65BC($at)
    MEM_W(0X65BC, ctx->r1) = ctx->r2;
    // 0x800D4F1C: bc1f        L_800D4F2C
    if (!c1cs) {
        // 0x800D4F20: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D4F2C;
    }
    // 0x800D4F20: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D4F24: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4F28: sw          $v0, 0x65B8($at)
    MEM_W(0X65B8, ctx->r1) = ctx->r2;
L_800D4F2C:
    // 0x800D4F2C: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x800D4F30: nop

    // 0x800D4F34: bc1f        L_800D4F44
    if (!c1cs) {
        // 0x800D4F38: addiu       $v0, $zero, 0xFF
        ctx->r2 = ADD32(0, 0XFF);
            goto L_800D4F44;
    }
    // 0x800D4F38: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
    // 0x800D4F3C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4F40: sw          $v0, 0x65B8($at)
    MEM_W(0X65B8, ctx->r1) = ctx->r2;
L_800D4F44:
    // 0x800D4F44: addu        $s2, $zero, $zero
    ctx->r18 = ADD32(0, 0);
    // 0x800D4F48: addiu       $s5, $zero, 0x1
    ctx->r21 = ADD32(0, 0X1);
    // 0x800D4F4C: addiu       $s4, $zero, 0x2
    ctx->r20 = ADD32(0, 0X2);
    // 0x800D4F50: lui         $s3, 0x801E
    ctx->r19 = S32(0X801E << 16);
    // 0x800D4F54: addiu       $s3, $s3, 0x440
    ctx->r19 = ADD32(ctx->r19, 0X440);
    // 0x800D4F58: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x800D4F5C: addu        $s1, $zero, $zero
    ctx->r17 = ADD32(0, 0);
L_800D4F60:
    // 0x800D4F60: lui         $v1, 0x8021
    ctx->r3 = S32(0X8021 << 16);
    // 0x800D4F64: lw          $v1, 0x22D4($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X22D4);
    // 0x800D4F68: addiu       $v0, $zero, 0x40
    ctx->r2 = ADD32(0, 0X40);
    // 0x800D4F6C: bne         $v1, $v0, L_800D504C
    if (ctx->r3 != ctx->r2) {
        // 0x800D4F70: nop
    
            goto L_800D504C;
    }
    // 0x800D4F70: nop

    // 0x800D4F74: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D4F78: lw          $v0, -0x2280($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X2280);
    // 0x800D4F7C: bne         $v0, $s5, L_800D5010
    if (ctx->r2 != ctx->r21) {
        // 0x800D4F80: nop
    
            goto L_800D5010;
    }
    // 0x800D4F80: nop

    // 0x800D4F84: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4F88: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x800D4F8C: lw          $v0, 0x6580($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6580);
    // 0x800D4F90: beq         $v0, $zero, L_800D5008
    if (ctx->r2 == 0) {
        // 0x800D4F94: addiu       $v0, $zero, -0x1
        ctx->r2 = ADD32(0, -0X1);
            goto L_800D5008;
    }
    // 0x800D4F94: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x800D4F98: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x800D4F9C: lw          $v1, 0x2F0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X2F0);
    // 0x800D4FA0: bne         $v1, $v0, L_800D4FD8
    if (ctx->r3 != ctx->r2) {
        // 0x800D4FA4: nop
    
            goto L_800D4FD8;
    }
    // 0x800D4FA4: nop

    // 0x800D4FA8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4FAC: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x800D4FB0: lw          $v0, 0x6578($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6578);
    // 0x800D4FB4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4FB8: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x800D4FBC: lw          $v1, 0x657C($at)
    ctx->r3 = MEM_W(ctx->r1, 0X657C);
    // 0x800D4FC0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4FC4: sw          $v0, 0x2F0($at)
    MEM_W(0X2F0, ctx->r1) = ctx->r2;
    // 0x800D4FC8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4FCC: sw          $v1, 0x2FC($at)
    MEM_W(0X2FC, ctx->r1) = ctx->r3;
    // 0x800D4FD0: j           L_800D5000
    // 0x800D4FD4: nop

        goto L_800D5000;
    // 0x800D4FD4: nop

L_800D4FD8:
    // 0x800D4FD8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4FDC: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x800D4FE0: lw          $v0, 0x6578($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6578);
    // 0x800D4FE4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4FE8: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x800D4FEC: lw          $v1, 0x657C($at)
    ctx->r3 = MEM_W(ctx->r1, 0X657C);
    // 0x800D4FF0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4FF4: sw          $v0, 0x2F8($at)
    MEM_W(0X2F8, ctx->r1) = ctx->r2;
    // 0x800D4FF8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D4FFC: sw          $v1, 0x300($at)
    MEM_W(0X300, ctx->r1) = ctx->r3;
L_800D5000:
    // 0x800D5000: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5004: sw          $zero, -0x2280($at)
    /* PATCHED: Original writes 0 to *(0x1DDD80) then reads back expecting 2
     * (set by VBLANK interrupt on real hardware). Write 2 directly to simulate
     * the interrupt having fired. */
    MEM_W(-0X2280, ctx->r1) = 2;
L_800D5008:
    // 0x800D5008: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D500C: lw          $v0, -0x2280($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X2280);
L_800D5010:
    // 0x800D5010: bne         $v0, $s4, L_800D50F8
    if (ctx->r2 != ctx->r20) {
        // 0x800D5014: addiu       $v0, $zero, 0x14A
        ctx->r2 = ADD32(0, 0X14A);
            goto L_800D50F8;
    }
    // 0x800D5014: addiu       $v0, $zero, 0x14A
    ctx->r2 = ADD32(0, 0X14A);
    // 0x800D5018: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D501C: sw          $v0, 0x2F0($at)
    MEM_W(0X2F0, ctx->r1) = ctx->r2;
    // 0x800D5020: addiu       $v0, $zero, 0x24
    ctx->r2 = ADD32(0, 0X24);
    // 0x800D5024: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5028: sw          $v0, 0x2FC($at)
    MEM_W(0X2FC, ctx->r1) = ctx->r2;
    // 0x800D502C: addiu       $v0, $zero, 0x6B8
    ctx->r2 = ADD32(0, 0X6B8);
    // 0x800D5030: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5034: sw          $v0, 0x2F8($at)
    MEM_W(0X2F8, ctx->r1) = ctx->r2;
    // 0x800D5038: addiu       $v0, $zero, 0xDC
    ctx->r2 = ADD32(0, 0XDC);
    // 0x800D503C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5040: sw          $v0, 0x300($at)
    MEM_W(0X300, ctx->r1) = ctx->r2;
    // 0x800D5044: j           L_800D50F8
    // 0x800D5048: nop

        goto L_800D50F8;
    // 0x800D5048: nop

L_800D504C:
    // 0x800D504C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5050: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x800D5054: lw          $v0, 0x490($at)
    ctx->r2 = MEM_W(ctx->r1, 0X490);
    // 0x800D5058: bne         $v0, $s5, L_800D5084
    if (ctx->r2 != ctx->r21) {
        // 0x800D505C: nop
    
            goto L_800D5084;
    }
    // 0x800D505C: nop

    // 0x800D5060: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5064: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x800D5068: lw          $v0, 0x6580($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6580);
    // 0x800D506C: beq         $v0, $zero, L_800D50C0
    if (ctx->r2 == 0) {
        // 0x800D5070: nop
    
            goto L_800D50C0;
    }
    // 0x800D5070: nop

    // 0x800D5074: jal         0x800D5B10
    // 0x800D5078: addu        $a0, $s0, $s3
    ctx->r4 = ADD32(ctx->r16, ctx->r19);
    func_800D5B10(rdram, ctx);
        goto after_0;
    // 0x800D5078: addu        $a0, $s0, $s3
    ctx->r4 = ADD32(ctx->r16, ctx->r19);
    after_0:
    // 0x800D507C: j           L_800D50F8
    // 0x800D5080: nop

        goto L_800D50F8;
    // 0x800D5080: nop

L_800D5084:
    // 0x800D5084: bne         $v0, $s4, L_800D50F8
    if (ctx->r2 != ctx->r20) {
        // 0x800D5088: nop
    
            goto L_800D50F8;
    }
    // 0x800D5088: nop

    // 0x800D508C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5090: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x800D5094: lw          $v0, 0x6580($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6580);
    // 0x800D5098: beq         $v0, $zero, L_800D50C0
    if (ctx->r2 == 0) {
        // 0x800D509C: nop
    
            goto L_800D50C0;
    }
    // 0x800D509C: nop

    // 0x800D50A0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D50A4: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x800D50A8: sw          $zero, 0x490($at)
    MEM_W(0X490, ctx->r1) = 0;
    // 0x800D50AC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D50B0: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x800D50B4: sw          $zero, 0x494($at)
    MEM_W(0X494, ctx->r1) = 0;
    // 0x800D50B8: j           L_800D50F8
    // 0x800D50BC: nop

        goto L_800D50F8;
    // 0x800D50BC: nop

L_800D50C0:
    // 0x800D50C0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D50C4: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x800D50C8: lw          $v0, 0x494($at)
    ctx->r2 = MEM_W(ctx->r1, 0X494);
    // 0x800D50CC: blez        $v0, L_800D50F8
    if (SIGNED(ctx->r2) <= 0) {
        // 0x800D50D0: addu        $a0, $s0, $s3
        ctx->r4 = ADD32(ctx->r16, ctx->r19);
            goto L_800D50F8;
    }
    // 0x800D50D0: addu        $a0, $s0, $s3
    ctx->r4 = ADD32(ctx->r16, ctx->r19);
    // 0x800D50D4: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x800D50D8: sw          $v0, 0x54($a0)
    MEM_W(0X54, ctx->r4) = ctx->r2;
    // 0x800D50DC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D50E0: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x800D50E4: lw          $v0, 0x494($at)
    ctx->r2 = MEM_W(ctx->r1, 0X494);
    // 0x800D50E8: bgtz        $v0, L_800D50F8
    if (SIGNED(ctx->r2) > 0) {
        // 0x800D50EC: nop
    
            goto L_800D50F8;
    }
    // 0x800D50EC: nop

    // 0x800D50F0: jal         0x800D56E0
    // 0x800D50F4: nop

    func_800D56E0(rdram, ctx);
        goto after_1;
    // 0x800D50F4: nop

    after_1:
L_800D50F8:
    // 0x800D50F8: lui         $at, 0xB680
    ctx->r1 = S32(0XB680 << 16);
    // 0x800D50FC: sw          $zero, 0x0($at)
    MEM_W(0X0, ctx->r1) = 0;
    // 0x800D5100: addiu       $s0, $s0, 0x118
    ctx->r16 = ADD32(ctx->r16, 0X118);
    // 0x800D5104: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    // 0x800D5108: slti        $v0, $s2, 0x2
    ctx->r2 = SIGNED(ctx->r18) < 0X2 ? 1 : 0;
    // 0x800D510C: bne         $v0, $zero, L_800D4F60
    if (ctx->r2 != 0) {
        // 0x800D5110: addiu       $s1, $s1, 0x24
        ctx->r17 = ADD32(ctx->r17, 0X24);
            goto L_800D4F60;
    }
    // 0x800D5110: addiu       $s1, $s1, 0x24
    ctx->r17 = ADD32(ctx->r17, 0X24);
    // 0x800D5114: lw          $ra, 0x28($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X28);
    // 0x800D5118: lw          $s5, 0x24($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X24);
    // 0x800D511C: lw          $s4, 0x20($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X20);
    // 0x800D5120: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x800D5124: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D5128: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D512C: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D5130: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x800D5134: jr          $ra
    // 0x800D5138: nop

    return;
    // 0x800D5138: nop

;}
RECOMP_FUNC void func_800D513C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D513C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x800D5140: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D5144: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x800D5148: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x800D514C: addu        $s2, $a1, $zero
    ctx->r18 = ADD32(ctx->r5, 0);
    // 0x800D5150: sll         $v0, $s1, 3
    ctx->r2 = S32(ctx->r17 << 3);
    // 0x800D5154: addu        $v0, $v0, $s1
    ctx->r2 = ADD32(ctx->r2, ctx->r17);
    // 0x800D5158: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x800D515C: subu        $v0, $v0, $s1
    ctx->r2 = SUB32(ctx->r2, ctx->r17);
    // 0x800D5160: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x800D5164: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x800D5168: addiu       $v1, $v1, 0x440
    ctx->r3 = ADD32(ctx->r3, 0X440);
    // 0x800D516C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D5170: addu        $s0, $v0, $v1
    ctx->r16 = ADD32(ctx->r2, ctx->r3);
    // 0x800D5174: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x800D5178: addiu       $s3, $s0, 0x90
    ctx->r19 = ADD32(ctx->r16, 0X90);
    // 0x800D517C: beq         $s2, $zero, L_800D51A8
    if (ctx->r18 == 0) {
        // 0x800D5180: sw          $ra, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r31;
            goto L_800D51A8;
    }
    // 0x800D5180: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x800D5184: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D5188: bne         $v0, $zero, L_800D51A8
    if (ctx->r2 != 0) {
        // 0x800D518C: nop
    
            goto L_800D51A8;
    }
    // 0x800D518C: nop

    // 0x800D5190: lw          $v0, 0xC8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC8);
    // 0x800D5194: bne         $v0, $zero, L_800D51A0
    if (ctx->r2 != 0) {
        // 0x800D5198: addiu       $a0, $zero, 0x6D
        ctx->r4 = ADD32(0, 0X6D);
            goto L_800D51A0;
    }
    // 0x800D5198: addiu       $a0, $zero, 0x6D
    ctx->r4 = ADD32(0, 0X6D);
    // 0x800D519C: addiu       $a0, $zero, 0x6C
    ctx->r4 = ADD32(0, 0X6C);
L_800D51A0:
    // 0x800D51A0: jal         0x80145F98
    // 0x800D51A4: nop

    func_80145F98(rdram, ctx);
        goto after_0;
    // 0x800D51A4: nop

    after_0:
L_800D51A8:
    // 0x800D51A8: lui         $a0, 0x8021
    ctx->r4 = S32(0X8021 << 16);
    // 0x800D51AC: addiu       $a0, $a0, 0x22D4
    ctx->r4 = ADD32(ctx->r4, 0X22D4);
    // 0x800D51B0: lw          $v1, 0x0($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X0);
    // 0x800D51B4: addiu       $v0, $zero, 0x1000
    ctx->r2 = ADD32(0, 0X1000);
    // 0x800D51B8: bne         $v1, $v0, L_800D51F8
    if (ctx->r3 != ctx->r2) {
        // 0x800D51BC: addiu       $v0, $zero, 0x40
        ctx->r2 = ADD32(0, 0X40);
            goto L_800D51F8;
    }
    // 0x800D51BC: addiu       $v0, $zero, 0x40
    ctx->r2 = ADD32(0, 0X40);
    // 0x800D51C0: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D51C4: ori         $v0, $v0, 0x4
    ctx->r2 = ctx->r2 | 0X4;
    // 0x800D51C8: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x800D51CC: andi        $v0, $v0, 0x10
    ctx->r2 = ctx->r2 & 0X10;
    // 0x800D51D0: bne         $v0, $zero, L_800D51F0
    if (ctx->r2 != 0) {
        // 0x800D51D4: addiu       $v0, $zero, 0x2000
        ctx->r2 = ADD32(0, 0X2000);
            goto L_800D51F0;
    }
    // 0x800D51D4: addiu       $v0, $zero, 0x2000
    ctx->r2 = ADD32(0, 0X2000);
    // 0x800D51D8: lui         $v0, 0xB680
    ctx->r2 = S32(0XB680 << 16);
    // 0x800D51DC: ori         $v0, $v0, 0x14
    ctx->r2 = ctx->r2 | 0X14;
    // 0x800D51E0: lbu         $v0, 0x0($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X0);
    // 0x800D51E4: andi        $v0, $v0, 0x10
    ctx->r2 = ctx->r2 & 0X10;
    // 0x800D51E8: beq         $v0, $zero, L_800D5458
    if (ctx->r2 == 0) {
        // 0x800D51EC: addiu       $v0, $zero, 0x2000
        ctx->r2 = ADD32(0, 0X2000);
            goto L_800D5458;
    }
    // 0x800D51EC: addiu       $v0, $zero, 0x2000
    ctx->r2 = ADD32(0, 0X2000);
L_800D51F0:
    // 0x800D51F0: j           L_800D5458
    // 0x800D51F4: sw          $v0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r2;
        goto L_800D5458;
    // 0x800D51F4: sw          $v0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r2;
L_800D51F8:
    // 0x800D51F8: bnel        $v1, $v0, L_800D5254
    if (ctx->r3 != ctx->r2) {
        // 0x800D51FC: addiu       $v0, $zero, 0x20
        ctx->r2 = ADD32(0, 0X20);
            goto L_800D5254;
    }
    goto skip_0;
    // 0x800D51FC: addiu       $v0, $zero, 0x20
    ctx->r2 = ADD32(0, 0X20);
    skip_0:
    // 0x800D5200: beq         $s2, $zero, L_800D522C
    if (ctx->r18 == 0) {
        // 0x800D5204: nop
    
            goto L_800D522C;
    }
    // 0x800D5204: nop

    // 0x800D5208: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D520C: bne         $v0, $zero, L_800D522C
    if (ctx->r2 != 0) {
        // 0x800D5210: nop
    
            goto L_800D522C;
    }
    // 0x800D5210: nop

    // 0x800D5214: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D5218: lw          $v0, 0x51B0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X51B0);
    // 0x800D521C: bne         $s1, $v0, L_800D522C
    if (ctx->r17 != ctx->r2) {
        // 0x800D5220: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D522C;
    }
    // 0x800D5220: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D5224: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5228: sw          $v0, -0x2280($at)
    MEM_W(-0X2280, ctx->r1) = ctx->r2;
L_800D522C:
    // 0x800D522C: jal         0x8014726C
    // 0x800D5230: nop

    func_8014726C(rdram, ctx);
        goto after_1;
    // 0x800D5230: nop

    after_1:
    // 0x800D5234: nor         $v0, $zero, $v0
    ctx->r2 = ~(0 | ctx->r2);
    // 0x800D5238: andi        $v0, $v0, 0x24
    ctx->r2 = ctx->r2 & 0X24;
    // 0x800D523C: beq         $v0, $zero, L_800D5458
    if (ctx->r2 == 0) {
        // 0x800D5240: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_800D5458;
    }
    // 0x800D5240: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D5244: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5248: sw          $v0, -0x2280($at)
    MEM_W(-0X2280, ctx->r1) = ctx->r2;
    // 0x800D524C: j           L_800D545C
    // 0x800D5250: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
        goto L_800D545C;
    // 0x800D5250: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
L_800D5254:
    // 0x800D5254: bne         $v1, $v0, L_800D5298
    if (ctx->r3 != ctx->r2) {
        // 0x800D5258: addiu       $v0, $zero, 0x4000
        ctx->r2 = ADD32(0, 0X4000);
            goto L_800D5298;
    }
    // 0x800D5258: addiu       $v0, $zero, 0x4000
    ctx->r2 = ADD32(0, 0X4000);
    // 0x800D525C: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x800D5260: addiu       $v0, $zero, 0x4
    ctx->r2 = ADD32(0, 0X4);
    // 0x800D5264: bnel        $v1, $v0, L_800D545C
    if (ctx->r3 != ctx->r2) {
        // 0x800D5268: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_1;
    // 0x800D5268: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_1:
    // 0x800D526C: beql        $s2, $zero, L_800D545C
    if (ctx->r18 == 0) {
        // 0x800D5270: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_2;
    // 0x800D5270: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_2:
    // 0x800D5274: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D5278: bnel        $v0, $zero, L_800D545C
    if (ctx->r2 != 0) {
        // 0x800D527C: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_3;
    // 0x800D527C: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_3:
L_800D5280:
    // 0x800D5280: lw          $v0, 0x3C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X3C);
    // 0x800D5284: lw          $v0, 0x8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X8);
    // 0x800D5288: bne         $v0, $zero, L_800D5444
    if (ctx->r2 != 0) {
        // 0x800D528C: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D5444;
    }
    // 0x800D528C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D5290: j           L_800D5454
    // 0x800D5294: nop

        goto L_800D5454;
    // 0x800D5294: nop

L_800D5298:
    // 0x800D5298: bnel        $v1, $v0, L_800D52E4
    if (ctx->r3 != ctx->r2) {
        // 0x800D529C: addiu       $v0, $zero, 0x80
        ctx->r2 = ADD32(0, 0X80);
            goto L_800D52E4;
    }
    goto skip_4;
    // 0x800D529C: addiu       $v0, $zero, 0x80
    ctx->r2 = ADD32(0, 0X80);
    skip_4:
    // 0x800D52A0: beql        $s2, $zero, L_800D545C
    if (ctx->r18 == 0) {
        // 0x800D52A4: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_5;
    // 0x800D52A4: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_5:
    // 0x800D52A8: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D52AC: bnel        $v0, $zero, L_800D545C
    if (ctx->r2 != 0) {
        // 0x800D52B0: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_6;
    // 0x800D52B0: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_6:
    // 0x800D52B4: lw          $v0, 0x3C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X3C);
    // 0x800D52B8: lw          $v0, 0x8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X8);
    // 0x800D52BC: beq         $v0, $zero, L_800D52D4
    if (ctx->r2 == 0) {
        // 0x800D52C0: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_800D52D4;
    }
    // 0x800D52C0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D52C4: jal         0x800E6DA0
    // 0x800D52C8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800E6DA0(rdram, ctx);
        goto after_2;
    // 0x800D52C8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_2:
    // 0x800D52CC: j           L_800D545C
    // 0x800D52D0: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
        goto L_800D545C;
    // 0x800D52D0: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
L_800D52D4:
    // 0x800D52D4: jal         0x800EBCF0
    // 0x800D52D8: addiu       $a1, $zero, 0x21
    ctx->r5 = ADD32(0, 0X21);
    func_800EBCF0(rdram, ctx);
        goto after_3;
    // 0x800D52D8: addiu       $a1, $zero, 0x21
    ctx->r5 = ADD32(0, 0X21);
    after_3:
    // 0x800D52DC: j           L_800D545C
    // 0x800D52E0: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
        goto L_800D545C;
    // 0x800D52E0: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
L_800D52E4:
    // 0x800D52E4: bnel        $v1, $v0, L_800D532C
    if (ctx->r3 != ctx->r2) {
        // 0x800D52E8: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_800D532C;
    }
    goto skip_7;
    // 0x800D52E8: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    skip_7:
    // 0x800D52EC: beql        $s2, $zero, L_800D545C
    if (ctx->r18 == 0) {
        // 0x800D52F0: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_8;
    // 0x800D52F0: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_8:
    // 0x800D52F4: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D52F8: bnel        $v0, $zero, L_800D545C
    if (ctx->r2 != 0) {
        // 0x800D52FC: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_9;
    // 0x800D52FC: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_9:
    // 0x800D5300: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5304: jal         0x800EBCF0
    // 0x800D5308: addiu       $a1, $zero, 0x21
    ctx->r5 = ADD32(0, 0X21);
    func_800EBCF0(rdram, ctx);
        goto after_4;
    // 0x800D5308: addiu       $a1, $zero, 0x21
    ctx->r5 = ADD32(0, 0X21);
    after_4:
    // 0x800D530C: addiu       $v0, $zero, 0xF
    ctx->r2 = ADD32(0, 0XF);
    // 0x800D5310: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5314: sw          $v0, 0x651C($at)
    MEM_W(0X651C, ctx->r1) = ctx->r2;
    // 0x800D5318: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D531C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5320: sw          $v0, 0x69D0($at)
    MEM_W(0X69D0, ctx->r1) = ctx->r2;
    // 0x800D5324: j           L_800D545C
    // 0x800D5328: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
        goto L_800D545C;
    // 0x800D5328: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
L_800D532C:
    // 0x800D532C: bnel        $v1, $v0, L_800D545C
    if (ctx->r3 != ctx->r2) {
        // 0x800D5330: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_10;
    // 0x800D5330: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_10:
    // 0x800D5334: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x800D5338: addiu       $v0, $zero, 0x4
    ctx->r2 = ADD32(0, 0X4);
    // 0x800D533C: bnel        $v1, $v0, L_800D545C
    if (ctx->r3 != ctx->r2) {
        // 0x800D5340: sw          $s2, 0x58($s0)
        MEM_W(0X58, ctx->r16) = ctx->r18;
            goto L_800D545C;
    }
    goto skip_11;
    // 0x800D5340: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
    skip_11:
    // 0x800D5344: lui         $v1, 0x100
    ctx->r3 = S32(0X100 << 16);
    // 0x800D5348: lw          $v0, 0x0($s3)
    ctx->r2 = MEM_W(ctx->r19, 0X0);
    // 0x800D534C: ori         $v1, $v1, 0x4
    ctx->r3 = ctx->r3 | 0X4;
    // 0x800D5350: bnel        $v0, $v1, L_800D5398
    if (ctx->r2 != ctx->r3) {
        // 0x800D5354: lui         $v1, 0x100
        ctx->r3 = S32(0X100 << 16);
            goto L_800D5398;
    }
    goto skip_12;
    // 0x800D5354: lui         $v1, 0x100
    ctx->r3 = S32(0X100 << 16);
    skip_12:
    // 0x800D5358: beq         $s2, $zero, L_800D5378
    if (ctx->r18 == 0) {
        // 0x800D535C: nop
    
            goto L_800D5378;
    }
    // 0x800D535C: nop

    // 0x800D5360: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D5364: bne         $v0, $zero, L_800D5394
    if (ctx->r2 != 0) {
        // 0x800D5368: lui         $v1, 0x100
        ctx->r3 = S32(0X100 << 16);
            goto L_800D5394;
    }
    // 0x800D5368: lui         $v1, 0x100
    ctx->r3 = S32(0X100 << 16);
    // 0x800D536C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5370: j           L_800D5388
    // 0x800D5374: addiu       $a1, $zero, 0x9C
    ctx->r5 = ADD32(0, 0X9C);
        goto L_800D5388;
    // 0x800D5374: addiu       $a1, $zero, 0x9C
    ctx->r5 = ADD32(0, 0X9C);
L_800D5378:
    // 0x800D5378: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D537C: beq         $v0, $zero, L_800D5390
    if (ctx->r2 == 0) {
        // 0x800D5380: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_800D5390;
    }
    // 0x800D5380: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5384: addiu       $a1, $zero, 0x9D
    ctx->r5 = ADD32(0, 0X9D);
L_800D5388:
    // 0x800D5388: jal         0x800EBCF0
    // 0x800D538C: nop

    func_800EBCF0(rdram, ctx);
        goto after_5;
    // 0x800D538C: nop

    after_5:
L_800D5390:
    // 0x800D5390: lui         $v1, 0x100
    ctx->r3 = S32(0X100 << 16);
L_800D5394:
    // 0x800D5394: lw          $v0, 0x0($s3)
    ctx->r2 = MEM_W(ctx->r19, 0X0);
L_800D5398:
    // 0x800D5398: ori         $v1, $v1, 0x5
    ctx->r3 = ctx->r3 | 0X5;
    // 0x800D539C: bne         $v0, $v1, L_800D53E0
    if (ctx->r2 != ctx->r3) {
        // 0x800D53A0: nop
    
            goto L_800D53E0;
    }
    // 0x800D53A0: nop

    // 0x800D53A4: beq         $s2, $zero, L_800D53C0
    if (ctx->r18 == 0) {
        // 0x800D53A8: nop
    
            goto L_800D53C0;
    }
    // 0x800D53A8: nop

    // 0x800D53AC: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D53B0: bne         $v0, $zero, L_800D53E0
    if (ctx->r2 != 0) {
        // 0x800D53B4: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_800D53E0;
    }
    // 0x800D53B4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D53B8: j           L_800D53D0
    // 0x800D53BC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
        goto L_800D53D0;
    // 0x800D53BC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
L_800D53C0:
    // 0x800D53C0: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D53C4: beq         $v0, $zero, L_800D53E0
    if (ctx->r2 == 0) {
        // 0x800D53C8: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_800D53E0;
    }
    // 0x800D53C8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D53CC: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
L_800D53D0:
    // 0x800D53D0: lui         $a2, 0x8017
    ctx->r6 = S32(0X8017 << 16);
    // 0x800D53D4: addiu       $a2, $a2, -0x924
    ctx->r6 = ADD32(ctx->r6, -0X924);
    // 0x800D53D8: jal         0x800EBD70
    // 0x800D53DC: nop

    func_800EBD70(rdram, ctx);
        goto after_6;
    // 0x800D53DC: nop

    after_6:
L_800D53E0:
    // 0x800D53E0: beql        $s2, $zero, L_800D5458
    if (ctx->r18 == 0) {
        // 0x800D53E4: sw          $zero, 0x60($s0)
        MEM_W(0X60, ctx->r16) = 0;
            goto L_800D5458;
    }
    goto skip_13;
    // 0x800D53E4: sw          $zero, 0x60($s0)
    MEM_W(0X60, ctx->r16) = 0;
    skip_13:
    // 0x800D53E8: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D53EC: beq         $v0, $zero, L_800D5280
    if (ctx->r2 == 0) {
        // 0x800D53F0: lui         $v1, 0x100
        ctx->r3 = S32(0X100 << 16);
            goto L_800D5280;
    }
    // 0x800D53F0: lui         $v1, 0x100
    ctx->r3 = S32(0X100 << 16);
    // 0x800D53F4: lw          $v0, 0x0($s3)
    ctx->r2 = MEM_W(ctx->r19, 0X0);
    // 0x800D53F8: and         $v0, $v0, $v1
    ctx->r2 = ctx->r2 & ctx->r3;
    // 0x800D53FC: beq         $v0, $zero, L_800D5458
    if (ctx->r2 == 0) {
        // 0x800D5400: lui         $v0, 0x2AAA
        ctx->r2 = S32(0X2AAA << 16);
            goto L_800D5458;
    }
    // 0x800D5400: lui         $v0, 0x2AAA
    ctx->r2 = S32(0X2AAA << 16);
    // 0x800D5404: lw          $a0, 0x60($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X60);
    // 0x800D5408: ori         $v0, $v0, 0xAAAB
    ctx->r2 = ctx->r2 | 0XAAAB;
    // 0x800D540C: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x800D5410: mult        $a0, $v0
    result = S64(S32(ctx->r4)) * S64(S32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x800D5414: sra         $v1, $a0, 31
    ctx->r3 = S32(SIGNED(ctx->r4) >> 31);
    // 0x800D5418: mfhi        $a3
    ctx->r7 = hi;
    // 0x800D541C: subu        $v1, $a3, $v1
    ctx->r3 = SUB32(ctx->r7, ctx->r3);
    // 0x800D5420: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x800D5424: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x800D5428: sll         $v0, $v0, 1
    ctx->r2 = S32(ctx->r2 << 1);
    // 0x800D542C: bne         $a0, $v0, L_800D5458
    if (ctx->r4 != ctx->r2) {
        // 0x800D5430: sw          $a0, 0x60($s0)
        MEM_W(0X60, ctx->r16) = ctx->r4;
            goto L_800D5458;
    }
    // 0x800D5430: sw          $a0, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r4;
    // 0x800D5434: lw          $v0, 0x3C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X3C);
    // 0x800D5438: lw          $v0, 0x8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X8);
    // 0x800D543C: beq         $v0, $zero, L_800D5454
    if (ctx->r2 == 0) {
        // 0x800D5440: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D5454;
    }
    // 0x800D5440: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_800D5444:
    // 0x800D5444: jal         0x800D5B10
    // 0x800D5448: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800D5B10(rdram, ctx);
        goto after_7;
    // 0x800D5448: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_7:
    // 0x800D544C: j           L_800D545C
    // 0x800D5450: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
        goto L_800D545C;
    // 0x800D5450: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
L_800D5454:
    // 0x800D5454: sw          $v0, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->r2;
L_800D5458:
    // 0x800D5458: sw          $s2, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r18;
L_800D545C:
    // 0x800D545C: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x800D5460: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x800D5464: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x800D5468: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D546C: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D5470: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x800D5474: jr          $ra
    // 0x800D5478: nop

    return;
    // 0x800D5478: nop

;}
RECOMP_FUNC void func_800D5484(uint8_t* rdram, recomp_context* ctx) {
    { static int cc = 0; cc++;
      if (cc <= 10 || cc % 5000 == 0)
        fprintf(stderr, "[render_submit] func_800D5484 #%d v0=0x%08X flag@16925C=%u\n",
                cc, (uint32_t)ctx->r2, *(uint32_t*)(rdram + 0x0016925C));
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D5484: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800D5488: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x800D548C: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D5490: beq         $v0, $zero, L_800D54F0
    if (ctx->r2 == 0) {
        // 0x800D5494: sw          $s0, 0x10($sp)
        MEM_W(0X10, ctx->r29) = ctx->r16;
            goto L_800D54F0;
    }
    // 0x800D5494: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D5498: lui         $s0, 0xB680
    ctx->r16 = S32(0XB680 << 16);
    // 0x800D549C: ori         $s0, $s0, 0x4
    ctx->r16 = ctx->r16 | 0X4;
    // 0x800D54A0: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D54A4: lui         $s1, 0xB680
    ctx->r17 = S32(0XB680 << 16);
    // 0x800D54A8: lbu         $a1, 0x0($s0)
    ctx->r5 = MEM_BU(ctx->r16, 0X0);
    // 0x800D54AC: ori         $s1, $s1, 0x14
    ctx->r17 = ctx->r17 | 0X14;
    // 0x800D54B0: jal         0x800D513C
    // 0x800D54B4: andi        $a1, $a1, 0x10
    ctx->r5 = ctx->r5 & 0X10;
    func_800D513C(rdram, ctx);
        goto after_0;
    // 0x800D54B4: andi        $a1, $a1, 0x10
    ctx->r5 = ctx->r5 & 0X10;
    after_0:
    // 0x800D54B8: lbu         $a1, 0x0($s1)
    ctx->r5 = MEM_BU(ctx->r17, 0X0);
    // 0x800D54BC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x800D54C0: jal         0x800D513C
    // 0x800D54C4: andi        $a1, $a1, 0x10
    ctx->r5 = ctx->r5 & 0X10;
    func_800D513C(rdram, ctx);
        goto after_1;
    // 0x800D54C4: andi        $a1, $a1, 0x10
    ctx->r5 = ctx->r5 & 0X10;
    after_1:
    // 0x800D54C8: lbu         $a1, 0x0($s0)
    ctx->r5 = MEM_BU(ctx->r16, 0X0);
    // 0x800D54CC: lui         $a0, 0x801E
    ctx->r4 = S32(0X801E << 16);
    // 0x800D54D0: addiu       $a0, $a0, 0x440
    ctx->r4 = ADD32(ctx->r4, 0X440);
    // 0x800D54D4: jal         0x800D5D04
    // 0x800D54D8: andi        $a1, $a1, 0x20
    ctx->r5 = ctx->r5 & 0X20;
    func_800D5D04(rdram, ctx);
        goto after_2;
    // 0x800D54D8: andi        $a1, $a1, 0x20
    ctx->r5 = ctx->r5 & 0X20;
    after_2:
    // 0x800D54DC: lbu         $a1, 0x0($s1)
    ctx->r5 = MEM_BU(ctx->r17, 0X0);
    // 0x800D54E0: lui         $a0, 0x801E
    ctx->r4 = S32(0X801E << 16);
    // 0x800D54E4: addiu       $a0, $a0, 0x558
    ctx->r4 = ADD32(ctx->r4, 0X558);
    // 0x800D54E8: jal         0x800D5D04
    // 0x800D54EC: andi        $a1, $a1, 0x20
    ctx->r5 = ctx->r5 & 0X20;
    func_800D5D04(rdram, ctx);
        goto after_3;
    // 0x800D54EC: andi        $a1, $a1, 0x20
    ctx->r5 = ctx->r5 & 0X20;
    after_3:
L_800D54F0:
    // 0x800D54F0: lui         $v1, 0x8021
    ctx->r3 = S32(0X8021 << 16);
    // 0x800D54F4: lw          $v1, 0x22D4($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X22D4);
    // 0x800D54F8: addiu       $v0, $zero, 0x100
    ctx->r2 = ADD32(0, 0X100);
    // 0x800D54FC: bne         $v1, $v0, L_800D559C
    if (ctx->r3 != ctx->r2) {
        // 0x800D5500: addiu       $v0, $zero, 0x40
        ctx->r2 = ADD32(0, 0X40);
            goto L_800D559C;
    }
    // 0x800D5500: addiu       $v0, $zero, 0x40
    ctx->r2 = ADD32(0, 0X40);
    // 0x800D5504: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D5508: lw          $s0, 0x4D38($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X4D38);
    // 0x800D550C: lw          $v0, 0x58($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X58);
    // 0x800D5510: beq         $v0, $zero, L_800D5588
    if (ctx->r2 == 0) {
        // 0x800D5514: nop
    
            goto L_800D5588;
    }
    // 0x800D5514: nop

    // 0x800D5518: lui         $at, 0x8021
    ctx->r1 = S32(0X8021 << 16);
    // 0x800D551C: lwc1        $f12, 0x22B4($at)
    ctx->f12.u32l = MEM_W(ctx->r1, 0X22B4);
    // 0x800D5520: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5524: lwc1        $f0, -0x91C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X91C);
    // 0x800D5528: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x800D552C: nop

    // 0x800D5530: bc1f        L_800D5548
    if (!c1cs) {
        // 0x800D5534: nop
    
            goto L_800D5548;
    }
    // 0x800D5534: nop

    // 0x800D5538: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D553C: lwc1        $f14, -0x918($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X918);
    // 0x800D5540: j           L_800D5550
    // 0x800D5544: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
        goto L_800D5550;
    // 0x800D5544: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
L_800D5548:
    // 0x800D5548: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D554C: lwc1        $f14, -0x914($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X914);
L_800D5550:
    // 0x800D5550: jal         0x8014F8D4
    // 0x800D5554: nop

    func_8014F8D4(rdram, ctx);
        goto after_4;
    // 0x800D5554: nop

    after_4:
    // 0x800D5558: lw          $v1, 0x3C($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X3C);
    // 0x800D555C: lw          $v0, 0x8($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X8);
    // 0x800D5560: beq         $v0, $zero, L_800D56C8
    if (ctx->r2 == 0) {
        // 0x800D5564: nop
    
            goto L_800D56C8;
    }
    // 0x800D5564: nop

    // 0x800D5568: lwc1        $f12, 0x1C($v1)
    ctx->f12.u32l = MEM_W(ctx->r3, 0X1C);
    // 0x800D556C: cvt.s.w     $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    ctx->f12.fl = CVT_S_W(ctx->f12.u32l);
    // 0x800D5570: lwc1        $f14, 0x20($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X20);
    // 0x800D5574: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x800D5578: jal         0x800E5628
    // 0x800D557C: nop

    func_800E5628(rdram, ctx);
        goto after_5;
    // 0x800D557C: nop

    after_5:
    // 0x800D5580: j           L_800D56C8
    // 0x800D5584: nop

        goto L_800D56C8;
    // 0x800D5584: nop

L_800D5588:
    // 0x800D5588: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x800D558C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5590: lwc1        $f14, -0x910($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X910);
    // 0x800D5594: j           L_800D56C0
    // 0x800D5598: nop

        goto L_800D56C0;
    // 0x800D5598: nop

L_800D559C:
    // 0x800D559C: bne         $v1, $v0, L_800D55E0
    if (ctx->r3 != ctx->r2) {
        // 0x800D55A0: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D55E0;
    }
    // 0x800D55A0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D55A4: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x800D55A8: lw          $v1, -0x2280($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X2280);
    // 0x800D55AC: bne         $v1, $v0, L_800D55CC
    if (ctx->r3 != ctx->r2) {
        // 0x800D55B0: nop
    
            goto L_800D55CC;
    }
    // 0x800D55B0: nop

    // 0x800D55B4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D55B8: lwc1        $f12, -0x90C($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X90C);
    // 0x800D55BC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D55C0: lwc1        $f14, -0x908($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X908);
    // 0x800D55C4: j           L_800D56C0
    // 0x800D55C8: nop

        goto L_800D56C0;
    // 0x800D55C8: nop

L_800D55CC:
    // 0x800D55CC: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x800D55D0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D55D4: lwc1        $f14, -0x904($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X904);
    // 0x800D55D8: j           L_800D56C0
    // 0x800D55DC: nop

        goto L_800D56C0;
    // 0x800D55DC: nop

L_800D55E0:
    // 0x800D55E0: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D55E4: addiu       $s0, $s0, 0x490
    ctx->r16 = ADD32(ctx->r16, 0X490);
    // 0x800D55E8: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x800D55EC: bne         $v0, $zero, L_800D5604
    if (ctx->r2 != 0) {
        // 0x800D55F0: nop
    
            goto L_800D5604;
    }
    // 0x800D55F0: nop

    // 0x800D55F4: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D55F8: lw          $v0, 0x5A8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5A8);
    // 0x800D55FC: beq         $v0, $zero, L_800D568C
    if (ctx->r2 == 0) {
        // 0x800D5600: nop
    
            goto L_800D568C;
    }
    // 0x800D5600: nop

L_800D5604:
    // 0x800D5604: jal         0x801620C8
    // 0x800D5608: nop

    func_801620C8(rdram, ctx);
        goto after_6;
    // 0x800D5608: nop

    after_6:
    // 0x800D560C: bne         $v0, $zero, L_800D56C8
    if (ctx->r2 != 0) {
        // 0x800D5610: nop
    
            goto L_800D56C8;
    }
    // 0x800D5610: nop

    // 0x800D5614: lui         $at, 0x8021
    ctx->r1 = S32(0X8021 << 16);
    // 0x800D5618: lwc1        $f12, 0x22B4($at)
    ctx->f12.u32l = MEM_W(ctx->r1, 0X22B4);
    // 0x800D561C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5620: lwc1        $f14, -0x900($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X900);
    // 0x800D5624: jal         0x8014F8D4
    // 0x800D5628: nop

    func_8014F8D4(rdram, ctx);
        goto after_7;
    // 0x800D5628: nop

    after_7:
    // 0x800D562C: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x800D5630: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D5634: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D5638: sw          $v0, -0x6D98($at)
    MEM_W(-0X6D98, ctx->r1) = ctx->r2;
    // 0x800D563C: beq         $v1, $zero, L_800D565C
    if (ctx->r3 == 0) {
        // 0x800D5640: nop
    
            goto L_800D565C;
    }
    // 0x800D5640: nop

    // 0x800D5644: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D5648: lw          $v0, 0x494($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X494);
    // 0x800D564C: bne         $v0, $zero, L_800D565C
    if (ctx->r2 != 0) {
        // 0x800D5650: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_800D565C;
    }
    // 0x800D5650: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D5654: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5658: sw          $v0, 0x494($at)
    MEM_W(0X494, ctx->r1) = ctx->r2;
L_800D565C:
    // 0x800D565C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D5660: lw          $v0, 0x5A8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5A8);
    // 0x800D5664: beq         $v0, $zero, L_800D56C8
    if (ctx->r2 == 0) {
        // 0x800D5668: nop
    
            goto L_800D56C8;
    }
    // 0x800D5668: nop

    // 0x800D566C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D5670: lw          $v0, 0x5AC($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5AC);
    // 0x800D5674: bne         $v0, $zero, L_800D56C8
    if (ctx->r2 != 0) {
        // 0x800D5678: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_800D56C8;
    }
    // 0x800D5678: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D567C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x800D5680: sw          $v0, 0x5AC($at)
    MEM_W(0X5AC, ctx->r1) = ctx->r2;
    // 0x800D5684: j           L_800D56C8
    // 0x800D5688: nop

        goto L_800D56C8;
    // 0x800D5688: nop

L_800D568C:
    // 0x800D568C: jal         0x801620C8
    // 0x800D5690: nop

    func_801620C8(rdram, ctx);
        goto after_8;
    // 0x800D5690: nop

    after_8:
    // 0x800D5694: bne         $v0, $zero, L_800D56C8
    if (ctx->r2 != 0) {
        // 0x800D5698: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800D56C8;
    }
    // 0x800D5698: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D569C: lui         $v1, 0x8018
    ctx->r3 = S32(0X8018 << 16);
    // 0x800D56A0: lw          $v1, -0x6D98($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X6D98);
    // 0x800D56A4: bne         $v1, $v0, L_800D56C8
    if (ctx->r3 != ctx->r2) {
        // 0x800D56A8: nop
    
            goto L_800D56C8;
    }
    // 0x800D56A8: nop

    // 0x800D56AC: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x800D56B0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D56B4: lwc1        $f14, -0x8FC($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X8FC);
    // 0x800D56B8: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D56BC: sw          $zero, -0x6D98($at)
    MEM_W(-0X6D98, ctx->r1) = 0;
L_800D56C0:
    // 0x800D56C0: jal         0x8014F8D4
    // 0x800D56C4: nop

    func_8014F8D4(rdram, ctx);
        goto after_9;
    // 0x800D56C4: nop

    after_9:
L_800D56C8:
    // 0x800D56C8: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x800D56CC: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D56D0: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D56D4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800D56D8: jr          $ra
    // 0x800D56DC: nop

    return;
    // 0x800D56DC: nop

;}
RECOMP_FUNC void func_800D56E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D56E0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800D56E4: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D56E8: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D56EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800D56F0: lw          $v0, 0x9C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X9C);
    // 0x800D56F4: beql        $v0, $zero, L_800D5714
    if (ctx->r2 == 0) {
        // 0x800D56F8: sw          $zero, 0x50($s0)
        MEM_W(0X50, ctx->r16) = 0;
            goto L_800D5714;
    }
    goto skip_0;
    // 0x800D56F8: sw          $zero, 0x50($s0)
    MEM_W(0X50, ctx->r16) = 0;
    skip_0:
    // 0x800D56FC: jal         0x800EBCF0
    // 0x800D5700: addiu       $a1, $zero, 0x20
    ctx->r5 = ADD32(0, 0X20);
    func_800EBCF0(rdram, ctx);
        goto after_0;
    // 0x800D5700: addiu       $a1, $zero, 0x20
    ctx->r5 = ADD32(0, 0X20);
    after_0:
    // 0x800D5704: lw          $v0, 0x98($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X98);
    // 0x800D5708: sw          $zero, 0x100($s0)
    MEM_W(0X100, ctx->r16) = 0;
    // 0x800D570C: sw          $v0, 0x94($s0)
    MEM_W(0X94, ctx->r16) = ctx->r2;
    // 0x800D5710: sw          $zero, 0x50($s0)
    MEM_W(0X50, ctx->r16) = 0;
L_800D5714:
    // 0x800D5714: sw          $zero, 0x54($s0)
    MEM_W(0X54, ctx->r16) = 0;
    // 0x800D5718: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800D571C: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D5720: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800D5724: jr          $ra
    // 0x800D5728: nop

    return;
    // 0x800D5728: nop

;}
RECOMP_FUNC void func_800D5750(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D5750: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800D5754: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D5758: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D575C: lui         $a0, 0x100
    ctx->r4 = S32(0X100 << 16);
    // 0x800D5760: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800D5764: lw          $v0, 0x90($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X90);
    // 0x800D5768: lwc1        $f0, 0xA0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0XA0);
    // 0x800D576C: lw          $v1, 0x90($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X90);
    // 0x800D5770: ori         $a0, $a0, 0x3
    ctx->r4 = ctx->r4 | 0X3;
    // 0x800D5774: sw          $v0, 0xB4($s0)
    MEM_W(0XB4, ctx->r16) = ctx->r2;
    // 0x800D5778: beq         $v1, $a0, L_800D5958
    if (ctx->r3 == ctx->r4) {
        // 0x800D577C: swc1        $f0, 0xB8($s0)
        MEM_W(0XB8, ctx->r16) = ctx->f0.u32l;
            goto L_800D5958;
    }
    // 0x800D577C: swc1        $f0, 0xB8($s0)
    MEM_W(0XB8, ctx->r16) = ctx->f0.u32l;
    // 0x800D5780: slt         $v0, $a0, $v1
    ctx->r2 = SIGNED(ctx->r4) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x800D5784: bne         $v0, $zero, L_800D57A8
    if (ctx->r2 != 0) {
        // 0x800D5788: lui         $v0, 0x100
        ctx->r2 = S32(0X100 << 16);
            goto L_800D57A8;
    }
    // 0x800D5788: lui         $v0, 0x100
    ctx->r2 = S32(0X100 << 16);
    // 0x800D578C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D5790: beq         $v1, $v0, L_800D57C8
    if (ctx->r3 == ctx->r2) {
        // 0x800D5794: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_800D57C8;
    }
    // 0x800D5794: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D5798: beq         $v1, $v0, L_800D58CC
    if (ctx->r3 == ctx->r2) {
        // 0x800D579C: nop
    
            goto L_800D58CC;
    }
    // 0x800D579C: nop

    // 0x800D57A0: j           L_800D5AFC
    // 0x800D57A4: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
        goto L_800D5AFC;
    // 0x800D57A4: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
L_800D57A8:
    // 0x800D57A8: ori         $v0, $v0, 0x4
    ctx->r2 = ctx->r2 | 0X4;
    // 0x800D57AC: beq         $v1, $v0, L_800D5A1C
    if (ctx->r3 == ctx->r2) {
        // 0x800D57B0: lui         $v0, 0x100
        ctx->r2 = S32(0X100 << 16);
            goto L_800D5A1C;
    }
    // 0x800D57B0: lui         $v0, 0x100
    ctx->r2 = S32(0X100 << 16);
    // 0x800D57B4: ori         $v0, $v0, 0x5
    ctx->r2 = ctx->r2 | 0X5;
    // 0x800D57B8: beq         $v1, $v0, L_800D5A70
    if (ctx->r3 == ctx->r2) {
        // 0x800D57BC: nop
    
            goto L_800D5A70;
    }
    // 0x800D57BC: nop

    // 0x800D57C0: j           L_800D5AFC
    // 0x800D57C4: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
        goto L_800D5AFC;
    // 0x800D57C4: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
L_800D57C8:
    // 0x800D57C8: jal         0x800D5B44
    // 0x800D57CC: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800D5B44(rdram, ctx);
        goto after_0;
    // 0x800D57CC: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_0:
    // 0x800D57D0: bne         $v0, $zero, L_800D58A8
    if (ctx->r2 != 0) {
        // 0x800D57D4: nop
    
            goto L_800D58A8;
    }
    // 0x800D57D4: nop

    // 0x800D57D8: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D57DC: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D57E0: beq         $v0, $zero, L_800D57F4
    if (ctx->r2 == 0) {
        // 0x800D57E4: nop
    
            goto L_800D57F4;
    }
    // 0x800D57E4: nop

    // 0x800D57E8: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
    // 0x800D57EC: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D57F0: sw          $v0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r2;
L_800D57F4:
    // 0x800D57F4: jal         0x800CC888
    // 0x800D57F8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_800CC888(rdram, ctx);
        goto after_1;
    // 0x800D57F8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_1:
    // 0x800D57FC: lw          $v0, 0xBC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XBC);
    // 0x800D5800: beq         $v0, $zero, L_800D5828
    if (ctx->r2 == 0) {
        // 0x800D5804: nop
    
            goto L_800D5828;
    }
    // 0x800D5804: nop

    // 0x800D5808: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D580C: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5810: beq         $v0, $zero, L_800D5878
    if (ctx->r2 == 0) {
        // 0x800D5814: nop
    
            goto L_800D5878;
    }
    // 0x800D5814: nop

    // 0x800D5818: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    // 0x800D581C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5820: j           L_800D5850
    // 0x800D5824: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;
        goto L_800D5850;
    // 0x800D5824: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;
L_800D5828:
    // 0x800D5828: lwc1        $f2, 0xF8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0XF8);
    // 0x800D582C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5830: lwc1        $f0, -0x8F8($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X8F8);
    // 0x800D5834: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x800D5838: nop

    // 0x800D583C: bc1f        L_800D5850
    if (!c1cs) {
        // 0x800D5840: addiu       $a0, $s0, 0xDC
        ctx->r4 = ADD32(ctx->r16, 0XDC);
            goto L_800D5850;
    }
    // 0x800D5840: addiu       $a0, $s0, 0xDC
    ctx->r4 = ADD32(ctx->r16, 0XDC);
    // 0x800D5844: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x800D5848: jal         0x800DB4E0
    // 0x800D584C: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    func_800DB4E0(rdram, ctx);
        goto after_2;
    // 0x800D584C: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    after_2:
L_800D5850:
    // 0x800D5850: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D5854: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5858: beq         $v0, $zero, L_800D5878
    if (ctx->r2 == 0) {
        // 0x800D585C: nop
    
            goto L_800D5878;
    }
    // 0x800D585C: nop

    // 0x800D5860: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D5864: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D5868: lwc1        $f2, 0x18($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D586C: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D5870: div.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D5874: swc1        $f0, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f0.u32l;
L_800D5878:
    // 0x800D5878: lwc1        $f12, 0xF0($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0XF0);
    // 0x800D587C: lwc1        $f14, 0xF4($s0)
    ctx->f14.u32l = MEM_W(ctx->r16, 0XF4);
    // 0x800D5880: lw          $a2, 0xC8($s0)
    ctx->r6 = MEM_W(ctx->r16, 0XC8);
    // 0x800D5884: jal         0x800DAE28
    // 0x800D5888: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    func_800DAE28(rdram, ctx);
        goto after_3;
    // 0x800D5888: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    after_3:
    // 0x800D588C: jal         0x801474C0
    // 0x800D5890: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    func_801474C0(rdram, ctx);
        goto after_4;
    // 0x800D5890: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_4:
    // 0x800D5894: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5898: jal         0x800EBCF0
    // 0x800D589C: addiu       $a1, $v0, 0x3D
    ctx->r5 = ADD32(ctx->r2, 0X3D);
    func_800EBCF0(rdram, ctx);
        goto after_5;
    // 0x800D589C: addiu       $a1, $v0, 0x3D
    ctx->r5 = ADD32(ctx->r2, 0X3D);
    after_5:
    // 0x800D58A0: j           L_800D5AFC
    // 0x800D58A4: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
        goto L_800D5AFC;
    // 0x800D58A4: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
L_800D58A8:
    // 0x800D58A8: lw          $v0, 0x100($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X100);
    // 0x800D58AC: addiu       $v1, $zero, 0x3
    ctx->r3 = ADD32(0, 0X3);
    // 0x800D58B0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D58B4: sw          $v0, 0x100($s0)
    MEM_W(0X100, ctx->r16) = ctx->r2;
    // 0x800D58B8: andi        $v0, $v0, 0x3
    ctx->r2 = ctx->r2 & 0X3;
    // 0x800D58BC: beq         $v0, $v1, L_800D59FC
    if (ctx->r2 == ctx->r3) {
        // 0x800D58C0: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_800D59FC;
    }
    // 0x800D58C0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D58C4: j           L_800D5A0C
    // 0x800D58C8: nop

        goto L_800D5A0C;
    // 0x800D58C8: nop

L_800D58CC:
    // 0x800D58CC: jal         0x800D5B44
    // 0x800D58D0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800D5B44(rdram, ctx);
        goto after_6;
    // 0x800D58D0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_6:
    // 0x800D58D4: bne         $v0, $zero, L_800D58A8
    if (ctx->r2 != 0) {
        // 0x800D58D8: nop
    
            goto L_800D58A8;
    }
    // 0x800D58D8: nop

    // 0x800D58DC: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D58E0: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D58E4: beq         $v0, $zero, L_800D58F8
    if (ctx->r2 == 0) {
        // 0x800D58E8: nop
    
            goto L_800D58F8;
    }
    // 0x800D58E8: nop

    // 0x800D58EC: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
    // 0x800D58F0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D58F4: sw          $v0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r2;
L_800D58F8:
    // 0x800D58F8: jal         0x800CC888
    // 0x800D58FC: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_800CC888(rdram, ctx);
        goto after_7;
    // 0x800D58FC: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_7:
    // 0x800D5900: lw          $v0, 0xBC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XBC);
    // 0x800D5904: beq         $v0, $zero, L_800D5928
    if (ctx->r2 == 0) {
        // 0x800D5908: addu        $a3, $zero, $zero
        ctx->r7 = ADD32(0, 0);
            goto L_800D5928;
    }
    // 0x800D5908: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    // 0x800D590C: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D5910: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5914: beq         $v0, $zero, L_800D5928
    if (ctx->r2 == 0) {
        // 0x800D5918: nop
    
            goto L_800D5928;
    }
    // 0x800D5918: nop

    // 0x800D591C: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    // 0x800D5920: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5924: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;
L_800D5928:
    // 0x800D5928: lwc1        $f12, 0xF0($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0XF0);
    // 0x800D592C: lw          $a2, 0xC8($s0)
    ctx->r6 = MEM_W(ctx->r16, 0XC8);
    // 0x800D5930: lwc1        $f14, 0xF4($s0)
    ctx->f14.u32l = MEM_W(ctx->r16, 0XF4);
    // 0x800D5934: jal         0x800DAE28
    // 0x800D5938: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    func_800DAE28(rdram, ctx);
        goto after_8;
    // 0x800D5938: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    after_8:
    // 0x800D593C: jal         0x801474C0
    // 0x800D5940: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    func_801474C0(rdram, ctx);
        goto after_9;
    // 0x800D5940: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    after_9:
    // 0x800D5944: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5948: jal         0x800EBCF0
    // 0x800D594C: addiu       $a1, $v0, 0x38
    ctx->r5 = ADD32(ctx->r2, 0X38);
    func_800EBCF0(rdram, ctx);
        goto after_10;
    // 0x800D594C: addiu       $a1, $v0, 0x38
    ctx->r5 = ADD32(ctx->r2, 0X38);
    after_10:
    // 0x800D5950: j           L_800D5AD0
    // 0x800D5954: nop

        goto L_800D5AD0;
    // 0x800D5954: nop

L_800D5958:
    // 0x800D5958: jal         0x800D5B44
    // 0x800D595C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800D5B44(rdram, ctx);
        goto after_11;
    // 0x800D595C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_11:
    // 0x800D5960: bne         $v0, $zero, L_800D59E4
    if (ctx->r2 != 0) {
        // 0x800D5964: addiu       $v1, $zero, 0x3
        ctx->r3 = ADD32(0, 0X3);
            goto L_800D59E4;
    }
    // 0x800D5964: addiu       $v1, $zero, 0x3
    ctx->r3 = ADD32(0, 0X3);
    // 0x800D5968: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D596C: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5970: beq         $v0, $zero, L_800D5984
    if (ctx->r2 == 0) {
        // 0x800D5974: nop
    
            goto L_800D5984;
    }
    // 0x800D5974: nop

    // 0x800D5978: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
    // 0x800D597C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5980: sw          $v0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r2;
L_800D5984:
    // 0x800D5984: jal         0x800CC888
    // 0x800D5988: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_800CC888(rdram, ctx);
        goto after_12;
    // 0x800D5988: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_12:
    // 0x800D598C: lw          $v0, 0xBC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XBC);
    // 0x800D5990: beq         $v0, $zero, L_800D59B4
    if (ctx->r2 == 0) {
        // 0x800D5994: nop
    
            goto L_800D59B4;
    }
    // 0x800D5994: nop

    // 0x800D5998: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D599C: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D59A0: beq         $v0, $zero, L_800D59B4
    if (ctx->r2 == 0) {
        // 0x800D59A4: nop
    
            goto L_800D59B4;
    }
    // 0x800D59A4: nop

    // 0x800D59A8: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    // 0x800D59AC: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D59B0: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;
L_800D59B4:
    // 0x800D59B4: lwc1        $f12, 0xF0($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0XF0);
    // 0x800D59B8: lwc1        $f14, 0xF4($s0)
    ctx->f14.u32l = MEM_W(ctx->r16, 0XF4);
    // 0x800D59BC: lw          $a2, 0xC8($s0)
    ctx->r6 = MEM_W(ctx->r16, 0XC8);
    // 0x800D59C0: jal         0x800DAE28
    // 0x800D59C4: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    func_800DAE28(rdram, ctx);
        goto after_13;
    // 0x800D59C4: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    after_13:
    // 0x800D59C8: jal         0x801474C0
    // 0x800D59CC: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    func_801474C0(rdram, ctx);
        goto after_14;
    // 0x800D59CC: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    after_14:
    // 0x800D59D0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D59D4: jal         0x800EBCF0
    // 0x800D59D8: addiu       $a1, $v0, 0x38
    ctx->r5 = ADD32(ctx->r2, 0X38);
    func_800EBCF0(rdram, ctx);
        goto after_15;
    // 0x800D59D8: addiu       $a1, $v0, 0x38
    ctx->r5 = ADD32(ctx->r2, 0X38);
    after_15:
    // 0x800D59DC: j           L_800D5AD0
    // 0x800D59E0: nop

        goto L_800D5AD0;
    // 0x800D59E0: nop

L_800D59E4:
    // 0x800D59E4: lw          $v0, 0x100($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X100);
    // 0x800D59E8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D59EC: sw          $v0, 0x100($s0)
    MEM_W(0X100, ctx->r16) = ctx->r2;
    // 0x800D59F0: andi        $v0, $v0, 0x3
    ctx->r2 = ctx->r2 & 0X3;
    // 0x800D59F4: bne         $v0, $v1, L_800D5A0C
    if (ctx->r2 != ctx->r3) {
        // 0x800D59F8: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_800D5A0C;
    }
    // 0x800D59F8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
L_800D59FC:
    // 0x800D59FC: jal         0x800EBCF0
    // 0x800D5A00: addiu       $a1, $zero, 0x59
    ctx->r5 = ADD32(0, 0X59);
    func_800EBCF0(rdram, ctx);
        goto after_16;
    // 0x800D5A00: addiu       $a1, $zero, 0x59
    ctx->r5 = ADD32(0, 0X59);
    after_16:
    // 0x800D5A04: j           L_800D5AFC
    // 0x800D5A08: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
        goto L_800D5AFC;
    // 0x800D5A08: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
L_800D5A0C:
    // 0x800D5A0C: jal         0x800EBCF0
    // 0x800D5A10: addiu       $a1, $zero, 0x21
    ctx->r5 = ADD32(0, 0X21);
    func_800EBCF0(rdram, ctx);
        goto after_17;
    // 0x800D5A10: addiu       $a1, $zero, 0x21
    ctx->r5 = ADD32(0, 0X21);
    after_17:
    // 0x800D5A14: j           L_800D5AFC
    // 0x800D5A18: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
        goto L_800D5AFC;
    // 0x800D5A18: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
L_800D5A1C:
    // 0x800D5A1C: jal         0x800D5B44
    // 0x800D5A20: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800D5B44(rdram, ctx);
        goto after_18;
    // 0x800D5A20: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_18:
    // 0x800D5A24: bnel        $v0, $zero, L_800D5AFC
    if (ctx->r2 != 0) {
        // 0x800D5A28: sw          $zero, 0xB0($s0)
        MEM_W(0XB0, ctx->r16) = 0;
            goto L_800D5AFC;
    }
    goto skip_0;
    // 0x800D5A28: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
    skip_0:
    // 0x800D5A2C: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D5A30: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5A34: beq         $v0, $zero, L_800D5A48
    if (ctx->r2 == 0) {
        // 0x800D5A38: nop
    
            goto L_800D5A48;
    }
    // 0x800D5A38: nop

    // 0x800D5A3C: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
    // 0x800D5A40: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5A44: sw          $v0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r2;
L_800D5A48:
    // 0x800D5A48: jal         0x800CC888
    // 0x800D5A4C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_800CC888(rdram, ctx);
        goto after_19;
    // 0x800D5A4C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_19:
    // 0x800D5A50: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D5A54: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5A58: beq         $v0, $zero, L_800D5AC0
    if (ctx->r2 == 0) {
        // 0x800D5A5C: addiu       $a2, $zero, 0x5
        ctx->r6 = ADD32(0, 0X5);
            goto L_800D5AC0;
    }
    // 0x800D5A5C: addiu       $a2, $zero, 0x5
    ctx->r6 = ADD32(0, 0X5);
    // 0x800D5A60: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    // 0x800D5A64: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5A68: j           L_800D5AC0
    // 0x800D5A6C: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;
        goto L_800D5AC0;
    // 0x800D5A6C: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;
L_800D5A70:
    // 0x800D5A70: jal         0x800D5B44
    // 0x800D5A74: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_800D5B44(rdram, ctx);
        goto after_20;
    // 0x800D5A74: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_20:
    // 0x800D5A78: bnel        $v0, $zero, L_800D5AFC
    if (ctx->r2 != 0) {
        // 0x800D5A7C: sw          $zero, 0xB0($s0)
        MEM_W(0XB0, ctx->r16) = 0;
            goto L_800D5AFC;
    }
    goto skip_1;
    // 0x800D5A7C: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
    skip_1:
    // 0x800D5A80: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D5A84: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5A88: beq         $v0, $zero, L_800D5A9C
    if (ctx->r2 == 0) {
        // 0x800D5A8C: nop
    
            goto L_800D5A9C;
    }
    // 0x800D5A8C: nop

    // 0x800D5A90: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
    // 0x800D5A94: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5A98: sw          $v0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r2;
L_800D5A9C:
    // 0x800D5A9C: jal         0x800CC888
    // 0x800D5AA0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_800CC888(rdram, ctx);
        goto after_21;
    // 0x800D5AA0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_21:
    // 0x800D5AA4: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D5AA8: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5AAC: beq         $v0, $zero, L_800D5AC0
    if (ctx->r2 == 0) {
        // 0x800D5AB0: addiu       $a2, $zero, 0x4
        ctx->r6 = ADD32(0, 0X4);
            goto L_800D5AC0;
    }
    // 0x800D5AB0: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    // 0x800D5AB4: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    // 0x800D5AB8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5ABC: sw          $v0, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r2;
L_800D5AC0:
    // 0x800D5AC0: lwc1        $f12, 0xF0($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0XF0);
    // 0x800D5AC4: lwc1        $f14, 0xF4($s0)
    ctx->f14.u32l = MEM_W(ctx->r16, 0XF4);
    // 0x800D5AC8: jal         0x800DAE28
    // 0x800D5ACC: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    func_800DAE28(rdram, ctx);
        goto after_22;
    // 0x800D5ACC: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    after_22:
L_800D5AD0:
    // 0x800D5AD0: lui         $v0, 0x801F
    ctx->r2 = S32(0X801F << 16);
    // 0x800D5AD4: lw          $v0, 0x7D10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X7D10);
    // 0x800D5AD8: beql        $v0, $zero, L_800D5AFC
    if (ctx->r2 == 0) {
        // 0x800D5ADC: sw          $zero, 0xB0($s0)
        MEM_W(0XB0, ctx->r16) = 0;
            goto L_800D5AFC;
    }
    goto skip_2;
    // 0x800D5ADC: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
    skip_2:
    // 0x800D5AE0: lwc1        $f0, 0x1C($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x800D5AE4: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D5AE8: lwc1        $f2, 0x18($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X18);
    // 0x800D5AEC: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x800D5AF0: div.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f0.fl, ctx->f2.fl);
    // 0x800D5AF4: swc1        $f0, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f0.u32l;
    // 0x800D5AF8: sw          $zero, 0xB0($s0)
    MEM_W(0XB0, ctx->r16) = 0;
L_800D5AFC:
    // 0x800D5AFC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800D5B00: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D5B04: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800D5B08: jr          $ra
    // 0x800D5B0C: nop

    return;
    // 0x800D5B0C: nop

;}
RECOMP_FUNC void func_800D5B10(uint8_t* rdram, recomp_context* ctx) {
    { static int cc = 0; cc++;
      if (cc <= 20 || cc % 5000 == 0)
        fprintf(stderr, "[scene_draw] func_800D5B10 called #%d a0=0x%08X\n", cc, (uint32_t)ctx->r4);
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D5B10: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D5B14: sw          $v0, 0xB0($a0)
    MEM_W(0XB0, ctx->r4) = ctx->r2;
    // 0x800D5B18: lw          $v0, 0x3C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X3C);
    // 0x800D5B1C: lw          $v1, 0x3C($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X3C);
    // 0x800D5B20: sw          $zero, 0x50($a0)
    MEM_W(0X50, ctx->r4) = 0;
    // 0x800D5B24: sw          $zero, 0x54($a0)
    MEM_W(0X54, ctx->r4) = 0;
    // 0x800D5B28: lwc1        $f0, 0x1C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X1C);
    // 0x800D5B2C: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D5B30: swc1        $f0, 0xF0($a0)
    MEM_W(0XF0, ctx->r4) = ctx->f0.u32l;
    // 0x800D5B34: lwc1        $f0, 0x20($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X20);
    // 0x800D5B38: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x800D5B3C: jr          $ra
    // 0x800D5B40: swc1        $f0, 0xF4($a0)
    MEM_W(0XF4, ctx->r4) = ctx->f0.u32l;
    return;
    // 0x800D5B40: swc1        $f0, 0xF4($a0)
    MEM_W(0XF4, ctx->r4) = ctx->f0.u32l;
;}
RECOMP_FUNC void func_800D5B44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D5B44: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800D5B48: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D5B4C: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D5B50: lui         $v1, 0x8021
    ctx->r3 = S32(0X8021 << 16);
    // 0x800D5B54: lw          $v1, 0x22F0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X22F0);
    // 0x800D5B58: lui         $v0, 0xC
    ctx->r2 = S32(0XC << 16);
    // 0x800D5B5C: beq         $v1, $v0, L_800D5BE0
    if (ctx->r3 == ctx->r2) {
        // 0x800D5B60: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_800D5BE0;
    }
    // 0x800D5B60: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800D5B64: lui         $v0, 0xA
    ctx->r2 = S32(0XA << 16);
    // 0x800D5B68: beq         $v1, $v0, L_800D5BEC
    if (ctx->r3 == ctx->r2) {
        // 0x800D5B6C: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_800D5BEC;
    }
    // 0x800D5B6C: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x800D5B70: lw          $v0, 0x94($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X94);
    // 0x800D5B74: blez        $v0, L_800D5BE8
    if (SIGNED(ctx->r2) <= 0) {
        // 0x800D5B78: addiu       $v0, $v0, -0x1
        ctx->r2 = ADD32(ctx->r2, -0X1);
            goto L_800D5BE8;
    }
    // 0x800D5B78: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x800D5B7C: bgtz        $v0, L_800D5BE0
    if (SIGNED(ctx->r2) > 0) {
        // 0x800D5B80: sw          $v0, 0x94($s0)
        MEM_W(0X94, ctx->r16) = ctx->r2;
            goto L_800D5BE0;
    }
    // 0x800D5B80: sw          $v0, 0x94($s0)
    MEM_W(0X94, ctx->r16) = ctx->r2;
    // 0x800D5B84: lui         $v1, 0x100
    ctx->r3 = S32(0X100 << 16);
    // 0x800D5B88: lw          $v0, 0x90($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X90);
    // 0x800D5B8C: ori         $v1, $v1, 0x4
    ctx->r3 = ctx->r3 | 0X4;
    // 0x800D5B90: bne         $v0, $v1, L_800D5BAC
    if (ctx->r2 != ctx->r3) {
        // 0x800D5B94: lui         $v1, 0x100
        ctx->r3 = S32(0X100 << 16);
            goto L_800D5BAC;
    }
    // 0x800D5B94: lui         $v1, 0x100
    ctx->r3 = S32(0X100 << 16);
    // 0x800D5B98: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5B9C: jal         0x800EBCF0
    // 0x800D5BA0: addiu       $a1, $zero, 0x9D
    ctx->r5 = ADD32(0, 0X9D);
    func_800EBCF0(rdram, ctx);
        goto after_0;
    // 0x800D5BA0: addiu       $a1, $zero, 0x9D
    ctx->r5 = ADD32(0, 0X9D);
    after_0:
    // 0x800D5BA4: lui         $v1, 0x100
    ctx->r3 = S32(0X100 << 16);
    // 0x800D5BA8: lw          $v0, 0x90($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X90);
L_800D5BAC:
    // 0x800D5BAC: ori         $v1, $v1, 0x5
    ctx->r3 = ctx->r3 | 0X5;
    // 0x800D5BB0: bne         $v0, $v1, L_800D5BC8
    if (ctx->r2 != ctx->r3) {
        // 0x800D5BB4: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_800D5BC8;
    }
    // 0x800D5BB4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5BB8: lui         $a2, 0x8017
    ctx->r6 = S32(0X8017 << 16);
    // 0x800D5BBC: addiu       $a2, $a2, -0x924
    ctx->r6 = ADD32(ctx->r6, -0X924);
    // 0x800D5BC0: jal         0x800EBD70
    // 0x800D5BC4: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    func_800EBD70(rdram, ctx);
        goto after_1;
    // 0x800D5BC4: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    after_1:
L_800D5BC8:
    // 0x800D5BC8: lw          $v0, 0x9C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X9C);
    // 0x800D5BCC: bne         $v0, $zero, L_800D5BEC
    if (ctx->r2 != 0) {
        // 0x800D5BD0: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_800D5BEC;
    }
    // 0x800D5BD0: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x800D5BD4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5BD8: jal         0x800D5C00
    // 0x800D5BDC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_800D5C00(rdram, ctx);
        goto after_2;
    // 0x800D5BDC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_2:
L_800D5BE0:
    // 0x800D5BE0: j           L_800D5BEC
    // 0x800D5BE4: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
        goto L_800D5BEC;
    // 0x800D5BE4: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_800D5BE8:
    // 0x800D5BE8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_800D5BEC:
    // 0x800D5BEC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800D5BF0: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D5BF4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800D5BF8: jr          $ra
    // 0x800D5BFC: nop

    return;
    // 0x800D5BFC: nop

;}
RECOMP_FUNC void func_800D5C00(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D5C00: lui         $v0, 0x100
    ctx->r2 = S32(0X100 << 16);
    // 0x800D5C04: ori         $v0, $v0, 0x3
    ctx->r2 = ctx->r2 | 0X3;
    // 0x800D5C08: beq         $a1, $v0, L_800D5C98
    if (ctx->r5 == ctx->r2) {
        // 0x800D5C0C: slt         $v0, $v0, $a1
        ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r5) ? 1 : 0;
            goto L_800D5C98;
    }
    // 0x800D5C0C: slt         $v0, $v0, $a1
    ctx->r2 = SIGNED(ctx->r2) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x800D5C10: bne         $v0, $zero, L_800D5C34
    if (ctx->r2 != 0) {
        // 0x800D5C14: lui         $v0, 0x100
        ctx->r2 = S32(0X100 << 16);
            goto L_800D5C34;
    }
    // 0x800D5C14: lui         $v0, 0x100
    ctx->r2 = S32(0X100 << 16);
    // 0x800D5C18: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D5C1C: beq         $a1, $v0, L_800D5C54
    if (ctx->r5 == ctx->r2) {
        // 0x800D5C20: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_800D5C54;
    }
    // 0x800D5C20: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D5C24: beq         $a1, $v0, L_800D5C78
    if (ctx->r5 == ctx->r2) {
        // 0x800D5C28: nop
    
            goto L_800D5C78;
    }
    // 0x800D5C28: nop

    // 0x800D5C2C: j           L_800D5CC8
    // 0x800D5C30: nop

        goto L_800D5CC8;
    // 0x800D5C30: nop

L_800D5C34:
    // 0x800D5C34: ori         $v0, $v0, 0x4
    ctx->r2 = ctx->r2 | 0X4;
    // 0x800D5C38: beq         $a1, $v0, L_800D5CA8
    if (ctx->r5 == ctx->r2) {
        // 0x800D5C3C: lui         $v0, 0x100
        ctx->r2 = S32(0X100 << 16);
            goto L_800D5CA8;
    }
    // 0x800D5C3C: lui         $v0, 0x100
    ctx->r2 = S32(0X100 << 16);
    // 0x800D5C40: ori         $v0, $v0, 0x5
    ctx->r2 = ctx->r2 | 0X5;
    // 0x800D5C44: beq         $a1, $v0, L_800D5C88
    if (ctx->r5 == ctx->r2) {
        // 0x800D5C48: nop
    
            goto L_800D5C88;
    }
    // 0x800D5C48: nop

    // 0x800D5C4C: j           L_800D5CC8
    // 0x800D5C50: nop

        goto L_800D5CC8;
    // 0x800D5C50: nop

L_800D5C54:
    // 0x800D5C54: lw          $v0, 0xFC($a0)
    ctx->r2 = MEM_W(ctx->r4, 0XFC);
    // 0x800D5C58: lw          $v1, 0xFC($a0)
    ctx->r3 = MEM_W(ctx->r4, 0XFC);
    // 0x800D5C5C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5C60: lwc1        $f0, -0x8F4($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X8F4);
    // 0x800D5C64: sw          $a1, 0x90($a0)
    MEM_W(0X90, ctx->r4) = ctx->r5;
    // 0x800D5C68: sw          $a1, 0x9C($a0)
    MEM_W(0X9C, ctx->r4) = ctx->r5;
    // 0x800D5C6C: sw          $v0, 0x94($a0)
    MEM_W(0X94, ctx->r4) = ctx->r2;
    // 0x800D5C70: j           L_800D5CC4
    // 0x800D5C74: sw          $v1, 0x98($a0)
    MEM_W(0X98, ctx->r4) = ctx->r3;
        goto L_800D5CC4;
    // 0x800D5C74: sw          $v1, 0x98($a0)
    MEM_W(0X98, ctx->r4) = ctx->r3;
L_800D5C78:
    // 0x800D5C78: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5C7C: lwc1        $f0, -0x8F0($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X8F0);
    // 0x800D5C80: j           L_800D5CB4
    // 0x800D5C84: addiu       $v0, $zero, 0xA
    ctx->r2 = ADD32(0, 0XA);
        goto L_800D5CB4;
    // 0x800D5C84: addiu       $v0, $zero, 0xA
    ctx->r2 = ADD32(0, 0XA);
L_800D5C88:
    // 0x800D5C88: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5C8C: lwc1        $f0, -0x8EC($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X8EC);
    // 0x800D5C90: j           L_800D5CB4
    // 0x800D5C94: addiu       $v0, $zero, 0x12
    ctx->r2 = ADD32(0, 0X12);
        goto L_800D5CB4;
    // 0x800D5C94: addiu       $v0, $zero, 0x12
    ctx->r2 = ADD32(0, 0X12);
L_800D5C98:
    // 0x800D5C98: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5C9C: lwc1        $f0, -0x8E8($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X8E8);
    // 0x800D5CA0: j           L_800D5CB4
    // 0x800D5CA4: addiu       $v0, $zero, 0x32
    ctx->r2 = ADD32(0, 0X32);
        goto L_800D5CB4;
    // 0x800D5CA4: addiu       $v0, $zero, 0x32
    ctx->r2 = ADD32(0, 0X32);
L_800D5CA8:
    // 0x800D5CA8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5CAC: lwc1        $f0, -0x8E4($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X8E4);
    // 0x800D5CB0: addiu       $v0, $zero, 0x1E
    ctx->r2 = ADD32(0, 0X1E);
L_800D5CB4:
    // 0x800D5CB4: sw          $a1, 0x90($a0)
    MEM_W(0X90, ctx->r4) = ctx->r5;
    // 0x800D5CB8: sw          $v0, 0x94($a0)
    MEM_W(0X94, ctx->r4) = ctx->r2;
    // 0x800D5CBC: sw          $v0, 0x98($a0)
    MEM_W(0X98, ctx->r4) = ctx->r2;
    // 0x800D5CC0: sw          $zero, 0x9C($a0)
    MEM_W(0X9C, ctx->r4) = 0;
L_800D5CC4:
    // 0x800D5CC4: swc1        $f0, 0xA0($a0)
    MEM_W(0XA0, ctx->r4) = ctx->f0.u32l;
L_800D5CC8:
    // 0x800D5CC8: jr          $ra
    // 0x800D5CCC: nop

    return;
    // 0x800D5CCC: nop

;}
RECOMP_FUNC void func_800D5CD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D5CD0: addiu       $v0, $zero, 0xA
    ctx->r2 = ADD32(0, 0XA);
    // 0x800D5CD4: sw          $v0, 0xFC($a0)
    MEM_W(0XFC, ctx->r4) = ctx->r2;
    // 0x800D5CD8: lw          $v1, 0xFC($a0)
    ctx->r3 = MEM_W(ctx->r4, 0XFC);
    // 0x800D5CDC: lw          $a1, 0xFC($a0)
    ctx->r5 = MEM_W(ctx->r4, 0XFC);
    // 0x800D5CE0: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5CE4: lwc1        $f0, -0x8E0($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X8E0);
    // 0x800D5CE8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D5CEC: sw          $v0, 0x90($a0)
    MEM_W(0X90, ctx->r4) = ctx->r2;
    // 0x800D5CF0: sw          $v0, 0x9C($a0)
    MEM_W(0X9C, ctx->r4) = ctx->r2;
    // 0x800D5CF4: sw          $v1, 0x94($a0)
    MEM_W(0X94, ctx->r4) = ctx->r3;
    // 0x800D5CF8: sw          $a1, 0x98($a0)
    MEM_W(0X98, ctx->r4) = ctx->r5;
    // 0x800D5CFC: jr          $ra
    // 0x800D5D00: swc1        $f0, 0xA0($a0)
    MEM_W(0XA0, ctx->r4) = ctx->f0.u32l;
    return;
    // 0x800D5D00: swc1        $f0, 0xA0($a0)
    MEM_W(0XA0, ctx->r4) = ctx->f0.u32l;
;}
RECOMP_FUNC void func_800D5D04(uint8_t* rdram, recomp_context* ctx) {
    { static int cc = 0; cc++;
      if (cc <= 10 || cc % 5000 == 0) {
        uint32_t s0p = (uint32_t)ctx->r4 & 0x1FFFFFFF;
        uint32_t mode = *(uint32_t*)(rdram + 0x002122D4);
        uint32_t entry4 = (s0p+4 < 0x800000) ? *(uint32_t*)(rdram + s0p + 4) : 0xDEAD;
        uint32_t entry5c = (s0p+0x5C < 0x800000) ? *(uint32_t*)(rdram + s0p + 0x5C) : 0xDEAD;
        uint32_t ptr50e0 = *(uint32_t*)(rdram + 0x001E50E0);
        uint32_t entry94 = (s0p+0x94 < 0x800000) ? *(uint32_t*)(rdram + s0p + 0x94) : 0xDEAD;
        uint32_t entry98 = (s0p+0x98 < 0x800000) ? *(uint32_t*)(rdram + s0p + 0x98) : 0xDEAD;
        fprintf(stderr, "[d5d04] #%d a0=0x%08X a1=0x%08X mode=%u e4=%u e5c=0x%X p50e0=0x%X e94=0x%X e98=0x%X\n",
                cc, (uint32_t)ctx->r4, (uint32_t)ctx->r5, mode, entry4, entry5c, ptr50e0, entry94, entry98);
      }
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D5D04: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800D5D08: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x800D5D0C: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x800D5D10: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x800D5D14: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x800D5D18: beq         $s1, $zero, L_800D5D44
    if (ctx->r17 == 0) {
        // 0x800D5D1C: sw          $ra, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r31;
            goto L_800D5D44;
    }
    // 0x800D5D1C: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x800D5D20: lw          $v0, 0x5C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X5C);
    // 0x800D5D24: bne         $v0, $zero, L_800D5D44
    if (ctx->r2 != 0) {
        // 0x800D5D28: nop
    
            goto L_800D5D44;
    }
    // 0x800D5D28: nop

    // 0x800D5D2C: lw          $v0, 0xC8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC8);
    // 0x800D5D30: bne         $v0, $zero, L_800D5D3C
    if (ctx->r2 != 0) {
        // 0x800D5D34: addiu       $a0, $zero, 0x6F
        ctx->r4 = ADD32(0, 0X6F);
            goto L_800D5D3C;
    }
    // 0x800D5D34: addiu       $a0, $zero, 0x6F
    ctx->r4 = ADD32(0, 0X6F);
    // 0x800D5D38: addiu       $a0, $zero, 0x6E
    ctx->r4 = ADD32(0, 0X6E);
L_800D5D3C:
    // 0x800D5D3C: jal         0x80145F98
    // 0x800D5D40: nop

    func_80145F98(rdram, ctx);
        goto after_0;
    // 0x800D5D40: nop

    after_0:
L_800D5D44:
    // 0x800D5D44: lui         $v1, 0x8021
    ctx->r3 = S32(0X8021 << 16);
    // 0x800D5D48: lw          $v1, 0x22D4($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X22D4);
    // 0x800D5D4C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D5D50: bne         $v1, $v0, L_800D5E18
    if (ctx->r3 != ctx->r2) {
        // 0x800D5D54: addiu       $v0, $zero, 0x80
        ctx->r2 = ADD32(0, 0X80);
            goto L_800D5E18;
    }
    // 0x800D5D54: addiu       $v0, $zero, 0x80
    ctx->r2 = ADD32(0, 0X80);
    // 0x800D5D58: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x800D5D5C: addiu       $v0, $zero, 0x4
    ctx->r2 = ADD32(0, 0X4);
    // 0x800D5D60: bnel        $v1, $v0, L_800D5E74
    if (ctx->r3 != ctx->r2) {
        // 0x800D5D64: sw          $s1, 0x5C($s0)
        MEM_W(0X5C, ctx->r16) = ctx->r17;
            goto L_800D5E74;
    }
    goto skip_0;
    // 0x800D5D64: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
    skip_0:
    // 0x800D5D68: beql        $s1, $zero, L_800D5E74
    if (ctx->r17 == 0) {
        // 0x800D5D6C: sw          $s1, 0x5C($s0)
        MEM_W(0X5C, ctx->r16) = ctx->r17;
            goto L_800D5E74;
    }
    goto skip_1;
    // 0x800D5D6C: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
    skip_1:
    // 0x800D5D70: lw          $v0, 0x5C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X5C);
    // 0x800D5D74: bnel        $v0, $zero, L_800D5E74
    if (ctx->r2 != 0) {
        // 0x800D5D78: sw          $s1, 0x5C($s0)
        MEM_W(0X5C, ctx->r16) = ctx->r17;
            goto L_800D5E74;
    }
    goto skip_2;
    // 0x800D5D78: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
    skip_2:
    // 0x800D5D7C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D5D80: lw          $v0, 0x50E0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X50E0);
    // 0x800D5D84: beq         $v0, $zero, L_800D5DDC
    if (ctx->r2 == 0) {
        // 0x800D5D88: nop
    
            goto L_800D5DDC;
    }
    // 0x800D5D88: nop

    // 0x800D5D8C: lw          $v0, 0x9C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X9C);
    // 0x800D5D90: beql        $v0, $zero, L_800D5DA8
    if (ctx->r2 == 0) {
        // 0x800D5D94: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_800D5DA8;
    }
    goto skip_3;
    // 0x800D5D94: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    skip_3:
    // 0x800D5D98: lw          $v0, 0x94($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X94);
    // 0x800D5D9C: lw          $v1, 0x98($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X98);
    // 0x800D5DA0: xor         $v0, $v0, $v1
    ctx->r2 = ctx->r2 ^ ctx->r3;
    // 0x800D5DA4: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_800D5DA8:
    // 0x800D5DA8: beql        $v0, $zero, L_800D5E74
    if (ctx->r2 == 0) {
        // 0x800D5DAC: sw          $s1, 0x5C($s0)
        MEM_W(0X5C, ctx->r16) = ctx->r17;
            goto L_800D5E74;
    }
    goto skip_4;
    // 0x800D5DAC: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
    skip_4:
    // 0x800D5DB0: lw          $v0, 0x9C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X9C);
    // 0x800D5DB4: beq         $v0, $zero, L_800D5DD0
    if (ctx->r2 == 0) {
        // 0x800D5DB8: addu        $a0, $s0, $zero
        ctx->r4 = ADD32(ctx->r16, 0);
            goto L_800D5DD0;
    }
    // 0x800D5DB8: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x800D5DBC: jal         0x800EBCF0
    // 0x800D5DC0: addiu       $a1, $zero, 0x20
    ctx->r5 = ADD32(0, 0X20);
    func_800EBCF0(rdram, ctx);
        goto after_1;
    // 0x800D5DC0: addiu       $a1, $zero, 0x20
    ctx->r5 = ADD32(0, 0X20);
    after_1:
    // 0x800D5DC4: lw          $v0, 0x98($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X98);
    // 0x800D5DC8: sw          $zero, 0x100($s0)
    MEM_W(0X100, ctx->r16) = 0;
    // 0x800D5DCC: sw          $v0, 0x94($s0)
    MEM_W(0X94, ctx->r16) = ctx->r2;
L_800D5DD0:
    // 0x800D5DD0: sw          $zero, 0x50($s0)
    MEM_W(0X50, ctx->r16) = 0;
    // 0x800D5DD4: j           L_800D5E70
    // 0x800D5DD8: sw          $zero, 0x54($s0)
    MEM_W(0X54, ctx->r16) = 0;
        goto L_800D5E70;
    // 0x800D5DD8: sw          $zero, 0x54($s0)
    MEM_W(0X54, ctx->r16) = 0;
L_800D5DDC:
    // 0x800D5DDC: lw          $v0, 0x3C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X3C);
    // 0x800D5DE0: lw          $v0, 0x8($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X8);
    // 0x800D5DE4: bnel        $v0, $zero, L_800D5E74
    if (ctx->r2 != 0) {
        // 0x800D5DE8: sw          $s1, 0x5C($s0)
        MEM_W(0X5C, ctx->r16) = ctx->r17;
            goto L_800D5E74;
    }
    goto skip_5;
    // 0x800D5DE8: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
    skip_5:
    // 0x800D5DEC: lw          $v0, 0x9C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X9C);
    // 0x800D5DF0: beql        $v0, $zero, L_800D5E08
    if (ctx->r2 == 0) {
        // 0x800D5DF4: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_800D5E08;
    }
    goto skip_6;
    // 0x800D5DF4: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    skip_6:
    // 0x800D5DF8: lw          $v0, 0x94($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X94);
    // 0x800D5DFC: lw          $v1, 0x98($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X98);
    // 0x800D5E00: xor         $v0, $v0, $v1
    ctx->r2 = ctx->r2 ^ ctx->r3;
    // 0x800D5E04: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_800D5E08:
    // 0x800D5E08: beq         $v0, $zero, L_800D5E70
    if (ctx->r2 == 0) {
        // 0x800D5E0C: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_800D5E70;
    }
    // 0x800D5E0C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x800D5E10: j           L_800D5E70
    // 0x800D5E14: sw          $v0, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->r2;
        goto L_800D5E70;
    // 0x800D5E14: sw          $v0, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->r2;
L_800D5E18:
    // 0x800D5E18: bnel        $v1, $v0, L_800D5E74
    if (ctx->r3 != ctx->r2) {
        // 0x800D5E1C: sw          $s1, 0x5C($s0)
        MEM_W(0X5C, ctx->r16) = ctx->r17;
            goto L_800D5E74;
    }
    goto skip_7;
    // 0x800D5E1C: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
    skip_7:
    // 0x800D5E20: beql        $s1, $zero, L_800D5E74
    if (ctx->r17 == 0) {
        // 0x800D5E24: sw          $s1, 0x5C($s0)
        MEM_W(0X5C, ctx->r16) = ctx->r17;
            goto L_800D5E74;
    }
    goto skip_8;
    // 0x800D5E24: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
    skip_8:
    // 0x800D5E28: lw          $v0, 0x5C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X5C);
    // 0x800D5E2C: bnel        $v0, $zero, L_800D5E74
    if (ctx->r2 != 0) {
        // 0x800D5E30: sw          $s1, 0x5C($s0)
        MEM_W(0X5C, ctx->r16) = ctx->r17;
            goto L_800D5E74;
    }
    goto skip_9;
    // 0x800D5E30: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
    skip_9:
    // 0x800D5E34: lw          $v0, 0xC8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC8);
    // 0x800D5E38: bne         $v0, $zero, L_800D5E5C
    if (ctx->r2 != 0) {
        // 0x800D5E3C: nop
    
            goto L_800D5E5C;
    }
    // 0x800D5E3C: nop

    // 0x800D5E40: lui         $v0, 0x8021
    ctx->r2 = S32(0X8021 << 16);
    // 0x800D5E44: lw          $v0, 0x2264($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X2264);
    // 0x800D5E48: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5E4C: lui         $at, 0x8021
    ctx->r1 = S32(0X8021 << 16);
    // 0x800D5E50: sw          $v0, 0x2264($at)
    MEM_W(0X2264, ctx->r1) = ctx->r2;
    // 0x800D5E54: j           L_800D5E74
    // 0x800D5E58: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
        goto L_800D5E74;
    // 0x800D5E58: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
L_800D5E5C:
    // 0x800D5E5C: lui         $v0, 0x8021
    ctx->r2 = S32(0X8021 << 16);
    // 0x800D5E60: lw          $v0, 0x2268($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X2268);
    // 0x800D5E64: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D5E68: lui         $at, 0x8021
    ctx->r1 = S32(0X8021 << 16);
    // 0x800D5E6C: sw          $v0, 0x2268($at)
    MEM_W(0X2268, ctx->r1) = ctx->r2;
L_800D5E70:
    // 0x800D5E70: sw          $s1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r17;
L_800D5E74:
    // 0x800D5E74: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x800D5E78: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x800D5E7C: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x800D5E80: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800D5E84: jr          $ra
    // 0x800D5E88: nop

    return;
    // 0x800D5E88: nop

;}
RECOMP_FUNC void func_800D5E8C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D5E8C: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x800D5E90: sw          $ra, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r31;
    // 0x800D5E94: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x800D5E98: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x800D5E9C: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x800D5EA0: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x800D5EA4: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x800D5EA8: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x800D5EAC: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x800D5EB0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800D5EB4: sdc1        $f24, 0x50($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X50, ctx->r29);
    // 0x800D5EB8: sdc1        $f22, 0x48($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X48, ctx->r29);
    // 0x800D5EBC: sdc1        $f20, 0x40($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X40, ctx->r29);
    // 0x800D5EC0: jal         0x80145538
    // 0x800D5EC4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    func_80145538(rdram, ctx);
        goto after_0;
    // 0x800D5EC4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_0:
    // 0x800D5EC8: lui         $v1, 0x8021
    ctx->r3 = S32(0X8021 << 16);
    // 0x800D5ECC: lw          $v1, 0x22D4($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X22D4);
    // 0x800D5ED0: addiu       $v0, $zero, 0x1000
    ctx->r2 = ADD32(0, 0X1000);
    // 0x800D5ED4: bne         $v1, $v0, L_800D6140
    if (ctx->r3 != ctx->r2) {
        // 0x800D5ED8: addiu       $s1, $zero, -0x1
        ctx->r17 = ADD32(0, -0X1);
            goto L_800D6140;
    }
    // 0x800D5ED8: addiu       $s1, $zero, -0x1
    ctx->r17 = ADD32(0, -0X1);
    // 0x800D5EDC: jal         0x8015EC70
    // 0x800D5EE0: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015EC70(rdram, ctx);
        goto after_1;
    // 0x800D5EE0: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_1:
    // 0x800D5EE4: jal         0x80166448
    // 0x800D5EE8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_80166448(rdram, ctx);
        goto after_2;
    // 0x800D5EE8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_2:
    // 0x800D5EEC: jal         0x80166504
    // 0x800D5EF0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    static_0_80166504(rdram, ctx);
        goto after_3;
    // 0x800D5EF0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_3:
    // 0x800D5EF4: addu        $s3, $zero, $zero
    ctx->r19 = ADD32(0, 0);
    // 0x800D5EF8: lui         $s4, 0xFF00
    ctx->r20 = S32(0XFF00 << 16);
    // 0x800D5EFC: addu        $s7, $zero, $zero
    ctx->r23 = ADD32(0, 0);
    // 0x800D5F00: addu        $s6, $zero, $zero
    ctx->r22 = ADD32(0, 0);
    // 0x800D5F04: addu        $s5, $zero, $zero
    ctx->r21 = ADD32(0, 0);
    // 0x800D5F08: addu        $s2, $zero, $zero
    ctx->r18 = ADD32(0, 0);
    // 0x800D5F0C: addu        $s1, $zero, $zero
    ctx->r17 = ADD32(0, 0);
    // 0x800D5F10: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x800D5F14: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5F18: lwc1        $f24, -0x850($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X850);
    // 0x800D5F1C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5F20: lwc1        $f22, -0x84C($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0X84C);
L_800D5F24:
    // 0x800D5F24: jal         0x8015F178
    // 0x800D5F28: or          $a0, $s1, $s4
    ctx->r4 = ctx->r17 | ctx->r20;
    func_8015F178(rdram, ctx);
        goto after_4;
    // 0x800D5F28: or          $a0, $s1, $s4
    ctx->r4 = ctx->r17 | ctx->r20;
    after_4:
    // 0x800D5F2C: mtc1        $s3, $f20
    ctx->f20.u32l = ctx->r19;
    // 0x800D5F30: cvt.s.w     $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    ctx->f20.fl = CVT_S_W(ctx->f20.u32l);
    // 0x800D5F34: mul.s       $f20, $f20, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f24.fl);
    // 0x800D5F38: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    // 0x800D5F3C: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x800D5F40: mfc1        $a2, $f22
    ctx->r6 = (int32_t)ctx->f22.u32l;
    // 0x800D5F44: mfc1        $a3, $f24
    ctx->r7 = (int32_t)ctx->f24.u32l;
    // 0x800D5F48: jal         0x800D1348
    // 0x800D5F4C: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    static_0_800D1348(rdram, ctx);
        goto after_5;
    // 0x800D5F4C: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    after_5:
    // 0x800D5F50: jal         0x8015F178
    // 0x800D5F54: or          $a0, $s2, $s4
    ctx->r4 = ctx->r18 | ctx->r20;
    func_8015F178(rdram, ctx);
        goto after_6;
    // 0x800D5F54: or          $a0, $s2, $s4
    ctx->r4 = ctx->r18 | ctx->r20;
    after_6:
    // 0x800D5F58: mov.s       $f12, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    ctx->f12.fl = ctx->f22.fl;
    // 0x800D5F5C: mfc1        $a2, $f22
    ctx->r6 = (int32_t)ctx->f22.u32l;
    // 0x800D5F60: mfc1        $a3, $f24
    ctx->r7 = (int32_t)ctx->f24.u32l;
    // 0x800D5F64: jal         0x800D1348
    // 0x800D5F68: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    static_0_800D1348(rdram, ctx);
        goto after_7;
    // 0x800D5F68: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    after_7:
    // 0x800D5F6C: jal         0x8015F178
    // 0x800D5F70: or          $a0, $s0, $s4
    ctx->r4 = ctx->r16 | ctx->r20;
    func_8015F178(rdram, ctx);
        goto after_8;
    // 0x800D5F70: or          $a0, $s0, $s4
    ctx->r4 = ctx->r16 | ctx->r20;
    after_8:
    // 0x800D5F74: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5F78: lwc1        $f12, -0x848($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X848);
    // 0x800D5F7C: mfc1        $a2, $f22
    ctx->r6 = (int32_t)ctx->f22.u32l;
    // 0x800D5F80: mfc1        $a3, $f24
    ctx->r7 = (int32_t)ctx->f24.u32l;
    // 0x800D5F84: jal         0x800D1348
    // 0x800D5F88: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    static_0_800D1348(rdram, ctx);
        goto after_9;
    // 0x800D5F88: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    after_9:
    // 0x800D5F8C: addu        $a0, $s5, $s6
    ctx->r4 = ADD32(ctx->r21, ctx->r22);
    // 0x800D5F90: jal         0x8015F178
    // 0x800D5F94: or          $a0, $a0, $s4
    ctx->r4 = ctx->r4 | ctx->r20;
    func_8015F178(rdram, ctx);
        goto after_10;
    // 0x800D5F94: or          $a0, $a0, $s4
    ctx->r4 = ctx->r4 | ctx->r20;
    after_10:
    // 0x800D5F98: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    // 0x800D5F9C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5FA0: lwc1        $f12, -0x844($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X844);
    // 0x800D5FA4: mfc1        $a2, $f22
    ctx->r6 = (int32_t)ctx->f22.u32l;
    // 0x800D5FA8: mfc1        $a3, $f24
    ctx->r7 = (int32_t)ctx->f24.u32l;
    // 0x800D5FAC: jal         0x800D1348
    // 0x800D5FB0: addiu       $s5, $s5, 0x111
    ctx->r21 = ADD32(ctx->r21, 0X111);
    static_0_800D1348(rdram, ctx);
        goto after_11;
    // 0x800D5FB0: addiu       $s5, $s5, 0x111
    ctx->r21 = ADD32(ctx->r21, 0X111);
    after_11:
    // 0x800D5FB4: addu        $a0, $s0, $s2
    ctx->r4 = ADD32(ctx->r16, ctx->r18);
    // 0x800D5FB8: jal         0x8015F178
    // 0x800D5FBC: or          $a0, $a0, $s4
    ctx->r4 = ctx->r4 | ctx->r20;
    func_8015F178(rdram, ctx);
        goto after_12;
    // 0x800D5FBC: or          $a0, $a0, $s4
    ctx->r4 = ctx->r4 | ctx->r20;
    after_12:
    // 0x800D5FC0: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    // 0x800D5FC4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5FC8: lwc1        $f12, -0x840($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X840);
    // 0x800D5FCC: mfc1        $a2, $f22
    ctx->r6 = (int32_t)ctx->f22.u32l;
    // 0x800D5FD0: mfc1        $a3, $f24
    ctx->r7 = (int32_t)ctx->f24.u32l;
    // 0x800D5FD4: jal         0x800D1348
    // 0x800D5FD8: addiu       $s2, $s2, 0x1100
    ctx->r18 = ADD32(ctx->r18, 0X1100);
    static_0_800D1348(rdram, ctx);
        goto after_13;
    // 0x800D5FD8: addiu       $s2, $s2, 0x1100
    ctx->r18 = ADD32(ctx->r18, 0X1100);
    after_13:
    // 0x800D5FDC: jal         0x8015F178
    // 0x800D5FE0: or          $a0, $s7, $s4
    ctx->r4 = ctx->r23 | ctx->r20;
    func_8015F178(rdram, ctx);
        goto after_14;
    // 0x800D5FE0: or          $a0, $s7, $s4
    ctx->r4 = ctx->r23 | ctx->r20;
    after_14:
    // 0x800D5FE4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D5FE8: lwc1        $f12, -0x83C($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X83C);
    // 0x800D5FEC: mfc1        $a2, $f22
    ctx->r6 = (int32_t)ctx->f22.u32l;
    // 0x800D5FF0: mfc1        $a3, $f24
    ctx->r7 = (int32_t)ctx->f24.u32l;
    // 0x800D5FF4: jal         0x800D1348
    // 0x800D5FF8: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    static_0_800D1348(rdram, ctx);
        goto after_15;
    // 0x800D5FF8: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    after_15:
    // 0x800D5FFC: addu        $a0, $s0, $s1
    ctx->r4 = ADD32(ctx->r16, ctx->r17);
    // 0x800D6000: jal         0x8015F178
    // 0x800D6004: or          $a0, $a0, $s4
    ctx->r4 = ctx->r4 | ctx->r20;
    func_8015F178(rdram, ctx);
        goto after_16;
    // 0x800D6004: or          $a0, $a0, $s4
    ctx->r4 = ctx->r4 | ctx->r20;
    after_16:
    // 0x800D6008: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    // 0x800D600C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D6010: lwc1        $f12, -0x838($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X838);
    // 0x800D6014: mfc1        $a2, $f22
    ctx->r6 = (int32_t)ctx->f22.u32l;
    // 0x800D6018: mfc1        $a3, $f24
    ctx->r7 = (int32_t)ctx->f24.u32l;
    // 0x800D601C: jal         0x800D1348
    // 0x800D6020: addiu       $s0, $s0, 0x11
    ctx->r16 = ADD32(ctx->r16, 0X11);
    static_0_800D1348(rdram, ctx);
        goto after_17;
    // 0x800D6020: addiu       $s0, $s0, 0x11
    ctx->r16 = ADD32(ctx->r16, 0X11);
    after_17:
    // 0x800D6024: lui         $v0, 0x11
    ctx->r2 = S32(0X11 << 16);
    // 0x800D6028: ori         $v0, $v0, 0x1100
    ctx->r2 = ctx->r2 | 0X1100;
    // 0x800D602C: addu        $s7, $s7, $v0
    ctx->r23 = ADD32(ctx->r23, ctx->r2);
    // 0x800D6030: lui         $v0, 0x11
    ctx->r2 = S32(0X11 << 16);
    // 0x800D6034: ori         $v0, $v0, 0x1000
    ctx->r2 = ctx->r2 | 0X1000;
    // 0x800D6038: addu        $s6, $s6, $v0
    ctx->r22 = ADD32(ctx->r22, ctx->r2);
    // 0x800D603C: lui         $v0, 0x11
    ctx->r2 = S32(0X11 << 16);
    // 0x800D6040: addu        $s1, $s1, $v0
    ctx->r17 = ADD32(ctx->r17, ctx->r2);
    // 0x800D6044: slti        $v0, $s3, 0x10
    ctx->r2 = SIGNED(ctx->r19) < 0X10 ? 1 : 0;
    // 0x800D6048: bne         $v0, $zero, L_800D5F24
    if (ctx->r2 != 0) {
        // 0x800D604C: nop
    
            goto L_800D5F24;
    }
    // 0x800D604C: nop

    // 0x800D6050: jal         0x8015F178
    // 0x800D6054: addiu       $a0, $zero, -0x1
    ctx->r4 = ADD32(0, -0X1);
    func_8015F178(rdram, ctx);
        goto after_18;
    // 0x800D6054: addiu       $a0, $zero, -0x1
    ctx->r4 = ADD32(0, -0X1);
    after_18:
    // 0x800D6058: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x800D605C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D6060: lwc1        $f24, -0x834($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X834);
    // 0x800D6064: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    // 0x800D6068: mfc1        $a2, $f24
    ctx->r6 = (int32_t)ctx->f24.u32l;
    // 0x800D606C: mfc1        $a3, $f20
    ctx->r7 = (int32_t)ctx->f20.u32l;
    // 0x800D6070: jal         0x800D122C
    // 0x800D6074: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    static_0_800D122C(rdram, ctx);
        goto after_19;
    // 0x800D6074: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    after_19:
    // 0x800D6078: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    // 0x800D607C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D6080: lwc1        $f22, -0x830($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0X830);
    // 0x800D6084: mfc1        $a2, $f24
    ctx->r6 = (int32_t)ctx->f24.u32l;
    // 0x800D6088: mfc1        $a3, $f22
    ctx->r7 = (int32_t)ctx->f22.u32l;
    // 0x800D608C: jal         0x800D122C
    // 0x800D6090: mov.s       $f14, $f22
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 22);
    ctx->f14.fl = ctx->f22.fl;
    static_0_800D122C(rdram, ctx);
        goto after_20;
    // 0x800D6090: mov.s       $f14, $f22
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 22);
    ctx->f14.fl = ctx->f22.fl;
    after_20:
    // 0x800D6094: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    // 0x800D6098: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x800D609C: mfc1        $a3, $f22
    ctx->r7 = (int32_t)ctx->f22.u32l;
    // 0x800D60A0: jal         0x800D122C
    // 0x800D60A4: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    static_0_800D122C(rdram, ctx);
        goto after_21;
    // 0x800D60A4: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    after_21:
    // 0x800D60A8: mov.s       $f12, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    ctx->f12.fl = ctx->f24.fl;
    // 0x800D60AC: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x800D60B0: mfc1        $a3, $f22
    ctx->r7 = (int32_t)ctx->f22.u32l;
    // 0x800D60B4: jal         0x800D122C
    // 0x800D60B8: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    static_0_800D122C(rdram, ctx);
        goto after_22;
    // 0x800D60B8: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    after_22:
    // 0x800D60BC: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D60C0: addiu       $s0, $s0, 0x65C8
    ctx->r16 = ADD32(ctx->r16, 0X65C8);
    // 0x800D60C4: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D60C8: addiu       $a1, $a1, -0x8DC
    ctx->r5 = ADD32(ctx->r5, -0X8DC);
    // 0x800D60CC: jal         0x80141260
    // 0x800D60D0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_23;
    // 0x800D60D0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_23:
    // 0x800D60D4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D60D8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D60DC: lwc1        $f20, -0x82C($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0X82C);
    // 0x800D60E0: lui         $a3, 0x4316
    ctx->r7 = S32(0X4316 << 16);
    // 0x800D60E4: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x800D60E8: jal         0x800DBC6C
    // 0x800D60EC: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    func_800DBC6C(rdram, ctx);
        goto after_24;
    // 0x800D60EC: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_24:
    // 0x800D60F0: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D60F4: addiu       $a1, $a1, -0x8CC
    ctx->r5 = ADD32(ctx->r5, -0X8CC);
    // 0x800D60F8: jal         0x80141260
    // 0x800D60FC: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_25;
    // 0x800D60FC: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_25:
    // 0x800D6100: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D6104: lui         $a2, 0x42F0
    ctx->r6 = S32(0X42F0 << 16);
    // 0x800D6108: mfc1        $a3, $f20
    ctx->r7 = (int32_t)ctx->f20.u32l;
    // 0x800D610C: jal         0x800DBC6C
    // 0x800D6110: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    func_800DBC6C(rdram, ctx);
        goto after_26;
    // 0x800D6110: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_26:
    // 0x800D6114: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D6118: addiu       $a1, $a1, -0x8B8
    ctx->r5 = ADD32(ctx->r5, -0X8B8);
    // 0x800D611C: jal         0x80141260
    // 0x800D6120: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_27;
    // 0x800D6120: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_27:
    // 0x800D6124: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D6128: lui         $a2, 0x42AA
    ctx->r6 = S32(0X42AA << 16);
    // 0x800D612C: lui         $a3, 0x42DC
    ctx->r7 = S32(0X42DC << 16);
    // 0x800D6130: jal         0x800DBC6C
    // 0x800D6134: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    func_800DBC6C(rdram, ctx);
        goto after_28;
    // 0x800D6134: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_28:
    // 0x800D6138: j           L_800D63A8
    // 0x800D613C: nop

        goto L_800D63A8;
    // 0x800D613C: nop

L_800D6140:
    // 0x800D6140: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D6144: lw          $v0, 0x2F0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X2F0);
    // 0x800D6148: bne         $v0, $s1, L_800D6270
    if (ctx->r2 != ctx->r17) {
        // 0x800D614C: nop
    
            goto L_800D6270;
    }
    // 0x800D614C: nop

    // 0x800D6150: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D6154: addiu       $s0, $s0, 0x65C8
    ctx->r16 = ADD32(ctx->r16, 0X65C8);
    // 0x800D6158: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D615C: addiu       $a1, $a1, -0x89C
    ctx->r5 = ADD32(ctx->r5, -0X89C);
    // 0x800D6160: jal         0x80141260
    // 0x800D6164: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_29;
    // 0x800D6164: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_29:
    // 0x800D6168: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D616C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D6170: lwc1        $f20, -0x828($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0X828);
    // 0x800D6174: lui         $a3, 0x4316
    ctx->r7 = S32(0X4316 << 16);
    // 0x800D6178: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x800D617C: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    // 0x800D6180: jal         0x800DDC44
    // 0x800D6184: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    func_800DDC44(rdram, ctx);
        goto after_30;
    // 0x800D6184: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    after_30:
    // 0x800D6188: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D618C: addiu       $a1, $a1, -0x890
    ctx->r5 = ADD32(ctx->r5, -0X890);
    // 0x800D6190: jal         0x80141260
    // 0x800D6194: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_31;
    // 0x800D6194: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_31:
    // 0x800D6198: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D619C: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x800D61A0: lui         $a3, 0x42F0
    ctx->r7 = S32(0X42F0 << 16);
    // 0x800D61A4: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    // 0x800D61A8: jal         0x800DDC44
    // 0x800D61AC: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    func_800DDC44(rdram, ctx);
        goto after_32;
    // 0x800D61AC: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    after_32:
    // 0x800D61B0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D61B4: lw          $v0, 0x51B0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X51B0);
    // 0x800D61B8: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x800D61BC: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D61C0: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x800D61C4: lw          $a2, -0x6DA0($at)
    ctx->r6 = MEM_W(ctx->r1, -0X6DA0);
    // 0x800D61C8: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D61CC: addiu       $a1, $a1, -0x880
    ctx->r5 = ADD32(ctx->r5, -0X880);
    // 0x800D61D0: jal         0x80141260
    // 0x800D61D4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_33;
    // 0x800D61D4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_33:
    // 0x800D61D8: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D61DC: lw          $v0, 0x51B0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X51B0);
    // 0x800D61E0: bnel        $v0, $zero, L_800D61F4
    if (ctx->r2 != 0) {
        // 0x800D61E4: lui         $v0, 0xFFFF
        ctx->r2 = S32(0XFFFF << 16);
            goto L_800D61F4;
    }
    goto skip_0;
    // 0x800D61E4: lui         $v0, 0xFFFF
    ctx->r2 = S32(0XFFFF << 16);
    skip_0:
    // 0x800D61E8: lui         $v0, 0xFF00
    ctx->r2 = S32(0XFF00 << 16);
    // 0x800D61EC: j           L_800D61F8
    // 0x800D61F0: ori         $v0, $v0, 0xFF00
    ctx->r2 = ctx->r2 | 0XFF00;
        goto L_800D61F8;
    // 0x800D61F0: ori         $v0, $v0, 0xFF00
    ctx->r2 = ctx->r2 | 0XFF00;
L_800D61F4:
    // 0x800D61F4: ori         $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 | 0XFF;
L_800D61F8:
    // 0x800D61F8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D61FC: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x800D6200: lui         $a3, 0x42B4
    ctx->r7 = S32(0X42B4 << 16);
    // 0x800D6204: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    // 0x800D6208: jal         0x800DDC44
    // 0x800D620C: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    func_800DDC44(rdram, ctx);
        goto after_34;
    // 0x800D620C: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    after_34:
    // 0x800D6210: lui         $v0, 0xFF80
    ctx->r2 = S32(0XFF80 << 16);
    // 0x800D6214: ori         $v0, $v0, 0x8080
    ctx->r2 = ctx->r2 | 0X8080;
    // 0x800D6218: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D621C: lui         $a2, 0x4348
    ctx->r6 = S32(0X4348 << 16);
    // 0x800D6220: lui         $a3, 0x4270
    ctx->r7 = S32(0X4270 << 16);
    // 0x800D6224: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D6228: addiu       $a1, $a1, -0x878
    ctx->r5 = ADD32(ctx->r5, -0X878);
    // 0x800D622C: jal         0x800DDC44
    // 0x800D6230: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    func_800DDC44(rdram, ctx);
        goto after_35;
    // 0x800D6230: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    after_35:
    // 0x800D6234: lui         $v0, 0x8018
    ctx->r2 = S32(0X8018 << 16);
    // 0x800D6238: lw          $v0, -0x6D94($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X6D94);
    // 0x800D623C: andi        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 & 0X1;
    // 0x800D6240: beq         $v0, $zero, L_800D6394
    if (ctx->r2 == 0) {
        // 0x800D6244: nop
    
            goto L_800D6394;
    }
    // 0x800D6244: nop

    // 0x800D6248: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D624C: addiu       $s0, $s0, 0x65C8
    ctx->r16 = ADD32(ctx->r16, 0X65C8);
    // 0x800D6250: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D6254: addiu       $a1, $a1, -0x854
    ctx->r5 = ADD32(ctx->r5, -0X854);
    // 0x800D6258: jal         0x80141260
    // 0x800D625C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_36;
    // 0x800D625C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_36:
    // 0x800D6260: lui         $a2, 0x41A0
    ctx->r6 = S32(0X41A0 << 16);
    // 0x800D6264: lui         $a3, 0x436C
    ctx->r7 = S32(0X436C << 16);
    // 0x800D6268: j           L_800D638C
    // 0x800D626C: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
        goto L_800D638C;
    // 0x800D626C: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
L_800D6270:
    // 0x800D6270: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D6274: addiu       $s0, $s0, 0x65C8
    ctx->r16 = ADD32(ctx->r16, 0X65C8);
    // 0x800D6278: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D627C: addiu       $a1, $a1, -0x89C
    ctx->r5 = ADD32(ctx->r5, -0X89C);
    // 0x800D6280: jal         0x80141260
    // 0x800D6284: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_37;
    // 0x800D6284: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_37:
    // 0x800D6288: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D628C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x800D6290: lwc1        $f20, -0x824($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0X824);
    // 0x800D6294: lui         $a3, 0x4316
    ctx->r7 = S32(0X4316 << 16);
    // 0x800D6298: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x800D629C: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    // 0x800D62A0: jal         0x800DDC44
    // 0x800D62A4: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    func_800DDC44(rdram, ctx);
        goto after_38;
    // 0x800D62A4: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    after_38:
    // 0x800D62A8: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D62AC: addiu       $a1, $a1, -0x890
    ctx->r5 = ADD32(ctx->r5, -0X890);
    // 0x800D62B0: jal         0x80141260
    // 0x800D62B4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_39;
    // 0x800D62B4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_39:
    // 0x800D62B8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D62BC: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x800D62C0: lui         $a3, 0x42F0
    ctx->r7 = S32(0X42F0 << 16);
    // 0x800D62C4: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    // 0x800D62C8: jal         0x800DDC44
    // 0x800D62CC: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    func_800DDC44(rdram, ctx);
        goto after_40;
    // 0x800D62CC: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    after_40:
    // 0x800D62D0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D62D4: lw          $v0, 0x51B0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X51B0);
    // 0x800D62D8: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x800D62DC: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D62E0: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x800D62E4: lw          $a2, -0x6DA0($at)
    ctx->r6 = MEM_W(ctx->r1, -0X6DA0);
    // 0x800D62E8: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D62EC: addiu       $a1, $a1, -0x880
    ctx->r5 = ADD32(ctx->r5, -0X880);
    // 0x800D62F0: jal         0x80141260
    // 0x800D62F4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_41;
    // 0x800D62F4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_41:
    // 0x800D62F8: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D62FC: lw          $v0, 0x51B0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X51B0);
    // 0x800D6300: bnel        $v0, $zero, L_800D6314
    if (ctx->r2 != 0) {
        // 0x800D6304: lui         $v0, 0xFFFF
        ctx->r2 = S32(0XFFFF << 16);
            goto L_800D6314;
    }
    goto skip_1;
    // 0x800D6304: lui         $v0, 0xFFFF
    ctx->r2 = S32(0XFFFF << 16);
    skip_1:
    // 0x800D6308: lui         $v0, 0xFF00
    ctx->r2 = S32(0XFF00 << 16);
    // 0x800D630C: j           L_800D6318
    // 0x800D6310: ori         $v0, $v0, 0xFF00
    ctx->r2 = ctx->r2 | 0XFF00;
        goto L_800D6318;
    // 0x800D6310: ori         $v0, $v0, 0xFF00
    ctx->r2 = ctx->r2 | 0XFF00;
L_800D6314:
    // 0x800D6314: ori         $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 | 0XFF;
L_800D6318:
    // 0x800D6318: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D631C: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x800D6320: lui         $a3, 0x42B4
    ctx->r7 = S32(0X42B4 << 16);
    // 0x800D6324: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    // 0x800D6328: jal         0x800DDC44
    // 0x800D632C: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    func_800DDC44(rdram, ctx);
        goto after_42;
    // 0x800D632C: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    after_42:
    // 0x800D6330: lui         $v0, 0xFF80
    ctx->r2 = S32(0XFF80 << 16);
    // 0x800D6334: ori         $v0, $v0, 0x8080
    ctx->r2 = ctx->r2 | 0X8080;
    // 0x800D6338: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D633C: lui         $a2, 0x4348
    ctx->r6 = S32(0X4348 << 16);
    // 0x800D6340: lui         $a3, 0x4270
    ctx->r7 = S32(0X4270 << 16);
    // 0x800D6344: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D6348: addiu       $a1, $a1, -0x878
    ctx->r5 = ADD32(ctx->r5, -0X878);
    // 0x800D634C: jal         0x800DDC44
    // 0x800D6350: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    func_800DDC44(rdram, ctx);
        goto after_43;
    // 0x800D6350: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    after_43:
    // 0x800D6354: lui         $v0, 0x8018
    ctx->r2 = S32(0X8018 << 16);
    // 0x800D6358: lw          $v0, -0x6D94($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X6D94);
    // 0x800D635C: andi        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 & 0X1;
    // 0x800D6360: beq         $v0, $zero, L_800D6394
    if (ctx->r2 == 0) {
        // 0x800D6364: nop
    
            goto L_800D6394;
    }
    // 0x800D6364: nop

    // 0x800D6368: lui         $s0, 0x801E
    ctx->r16 = S32(0X801E << 16);
    // 0x800D636C: addiu       $s0, $s0, 0x65C8
    ctx->r16 = ADD32(ctx->r16, 0X65C8);
    // 0x800D6370: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x800D6374: addiu       $a1, $a1, -0x854
    ctx->r5 = ADD32(ctx->r5, -0X854);
    // 0x800D6378: jal         0x80141260
    // 0x800D637C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80141260(rdram, ctx);
        goto after_44;
    // 0x800D637C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_44:
    // 0x800D6380: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x800D6384: lui         $a2, 0x43BE
    ctx->r6 = S32(0X43BE << 16);
    // 0x800D6388: lui         $a3, 0x41A0
    ctx->r7 = S32(0X41A0 << 16);
L_800D638C:
    // 0x800D638C: jal         0x800DBC6C
    // 0x800D6390: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    func_800DBC6C(rdram, ctx);
        goto after_45;
    // 0x800D6390: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_45:
L_800D6394:
    // 0x800D6394: lui         $v0, 0x8018
    ctx->r2 = S32(0X8018 << 16);
    // 0x800D6398: lw          $v0, -0x6D94($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X6D94);
    // 0x800D639C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800D63A0: lui         $at, 0x8018
    ctx->r1 = S32(0X8018 << 16);
    // 0x800D63A4: sw          $v0, -0x6D94($at)
    MEM_W(-0X6D94, ctx->r1) = ctx->r2;
L_800D63A8:
    // 0x800D63A8: lw          $ra, 0x38($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X38);
    // 0x800D63AC: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x800D63B0: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x800D63B4: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x800D63B8: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x800D63BC: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x800D63C0: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x800D63C4: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x800D63C8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800D63CC: ldc1        $f24, 0x50($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X50);
    // 0x800D63D0: ldc1        $f22, 0x48($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X48);
    // 0x800D63D4: ldc1        $f20, 0x40($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X40);
    // 0x800D63D8: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    // 0x800D63DC: jr          $ra
    // 0x800D63E0: nop

    return;
    // 0x800D63E0: nop

;}
RECOMP_FUNC void func_800D63E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D63E4: lw          $v0, 0xC8($a0)
    ctx->r2 = MEM_W(ctx->r4, 0XC8);
    // 0x800D63E8: bne         $v0, $zero, L_800D6400
    if (ctx->r2 != 0) {
        // 0x800D63EC: nop
    
            goto L_800D6400;
    }
    // 0x800D63EC: nop

    // 0x800D63F0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D63F4: addiu       $v0, $v0, 0x6578
    ctx->r2 = ADD32(ctx->r2, 0X6578);
    // 0x800D63F8: j           L_800D6408
    // 0x800D63FC: nop

        goto L_800D6408;
    // 0x800D63FC: nop

L_800D6400:
    // 0x800D6400: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x800D6404: addiu       $v0, $v0, 0x659C
    ctx->r2 = ADD32(ctx->r2, 0X659C);
L_800D6408:
    // 0x800D6408: jr          $ra
    // 0x800D640C: sw          $v0, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r2;
    return;
    // 0x800D640C: sw          $v0, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r2;
;}
RECOMP_FUNC void func_800D6410(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D6410: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800D6414: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x800D6418: sw          $v0, 0x7D10($at)
    MEM_W(0X7D10, ctx->r1) = ctx->r2;
    // 0x800D641C: jr          $ra
    // 0x800D6420: nop

    return;
    // 0x800D6420: nop

;}
RECOMP_FUNC void func_800D6424(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D6424: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x800D6428: sw          $zero, 0x7D10($at)
    MEM_W(0X7D10, ctx->r1) = 0;
    // 0x800D642C: jr          $ra
    // 0x800D6430: nop

    return;
    // 0x800D6430: nop

;}
RECOMP_FUNC void func_800D6438(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D6438: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
    // 0x800D643C: addiu       $v0, $zero, 0x23FC
    ctx->r2 = ADD32(0, 0X23FC);
L_800D6440:
    // 0x800D6440: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x800D6444: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x800D6448: sw          $v1, 0x7D18($at)
    MEM_W(0X7D18, ctx->r1) = ctx->r3;
    // 0x800D644C: addiu       $v0, $v0, -0x1C
    ctx->r2 = ADD32(ctx->r2, -0X1C);
    // 0x800D6450: bgez        $v0, L_800D6440
    if (SIGNED(ctx->r2) >= 0) {
        // 0x800D6454: nop
    
            goto L_800D6440;
    }
    // 0x800D6454: nop

    // 0x800D6458: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x800D645C: sw          $zero, 0x67EC($at)
    MEM_W(0X67EC, ctx->r1) = 0;
    // 0x800D6460: jr          $ra
    // 0x800D6464: nop

    return;
    // 0x800D6464: nop

;}
RECOMP_FUNC void func_800D64E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800D64E4: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x800D64E8: lui         $a2, 0x801F
    ctx->r6 = S32(0X801F << 16);
    // 0x800D64EC: addiu       $a2, $a2, 0x7D18
    ctx->r6 = ADD32(ctx->r6, 0X7D18);
    // 0x800D64F0: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
L_800D64F4:
    // 0x800D64F4: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x800D64F8: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x800D64FC: lw          $v0, 0x7D18($at)
    ctx->r2 = MEM_W(ctx->r1, 0X7D18);
    // 0x800D6500: bnel        $v0, $a0, L_800D6510
    if (ctx->r2 != ctx->r4) {
        // 0x800D6504: addiu       $a2, $a2, 0x1C
        ctx->r6 = ADD32(ctx->r6, 0X1C);
            goto L_800D6510;
    }
    goto skip_0;
    // 0x800D6504: addiu       $a2, $a2, 0x1C
    ctx->r6 = ADD32(ctx->r6, 0X1C);
    skip_0:
    // 0x800D6508: j           L_800D6524
    // 0x800D650C: addu        $v0, $a2, $zero
    ctx->r2 = ADD32(ctx->r6, 0);
        goto L_800D6524;
    // 0x800D650C: addu        $v0, $a2, $zero
    ctx->r2 = ADD32(ctx->r6, 0);
L_800D6510:
    // 0x800D6510: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x800D6514: slti        $v0, $a1, 0x14A
    ctx->r2 = SIGNED(ctx->r5) < 0X14A ? 1 : 0;
    // 0x800D6518: bne         $v0, $zero, L_800D64F4
    if (ctx->r2 != 0) {
        // 0x800D651C: addiu       $v1, $v1, 0x1C
        ctx->r3 = ADD32(ctx->r3, 0X1C);
            goto L_800D64F4;
    }
    // 0x800D651C: addiu       $v1, $v1, 0x1C
    ctx->r3 = ADD32(ctx->r3, 0X1C);
    // 0x800D6520: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_800D6524:
    // 0x800D6524: jr          $ra
    // 0x800D6528: nop

    return;
    // 0x800D6528: nop

;}
