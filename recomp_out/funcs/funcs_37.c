#include "recomp.h"
#include "funcs.h"

RECOMP_FUNC void func_8015ED34(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015ED34: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8015ED38: addu        $v1, $a0, $zero
    ctx->r3 = ADD32(ctx->r4, 0);
    // 0x8015ED3C: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    // 0x8015ED40: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8015ED44: lw          $v0, 0x343C($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X343C);
    // 0x8015ED48: beq         $v1, $v0, L_8015ED84
    if (ctx->r3 == ctx->r2) {
        // 0x8015ED4C: sw          $ra, 0x10($sp)
        MEM_W(0X10, ctx->r29) = ctx->r31;
            goto L_8015ED84;
    }
    // 0x8015ED4C: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x8015ED50: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015ED54: sw          $v1, 0x343C($at)
    MEM_W(0X343C, ctx->r1) = ctx->r3;
    // 0x8015ED58: jal         0x8015E200
    // 0x8015ED5C: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    static_0_8015E200(rdram, ctx);
        goto after_0;
    // 0x8015ED5C: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    after_0:
    // 0x8015ED60: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x8015ED64: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015ED68: ori         $a0, $a0, 0x134
    ctx->r4 = ctx->r4 | 0X134;
    // 0x8015ED6C: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015ED70: jal         0x80167848
    // 0x8015ED74: sw          $v0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r2;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x8015ED74: sw          $v0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r2;
    after_1:
    // 0x8015ED78: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x8015ED7C: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x8015ED80: sw          $v0, 0x7BB4($at)
    MEM_W(0X7BB4, ctx->r1) = ctx->r2;
L_8015ED84:
    // 0x8015ED84: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x8015ED88: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8015ED8C: jr          $ra
    // 0x8015ED90: nop

    return;
    // 0x8015ED90: nop

;}
RECOMP_FUNC void func_8015ED94(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015ED94: lui         $t0, 0x8023
    ctx->r8 = S32(0X8023 << 16);
    // 0x8015ED98: lw          $t0, -0x5BBC($t0)
    ctx->r8 = MEM_W(ctx->r8, -0X5BBC);
    // 0x8015ED9C: mtc1        $a0, $f0
    ctx->f0.u32l = ctx->r4;
    // 0x8015EDA0: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x8015EDA4: sll         $v0, $t0, 3
    ctx->r2 = S32(ctx->r8 << 3);
    // 0x8015EDA8: addu        $v0, $v0, $t0
    ctx->r2 = ADD32(ctx->r2, ctx->r8);
    // 0x8015EDAC: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015EDB0: addu        $v0, $v0, $t0
    ctx->r2 = ADD32(ctx->r2, ctx->r8);
    // 0x8015EDB4: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015EDB8: addu        $v0, $v0, $t0
    ctx->r2 = ADD32(ctx->r2, ctx->r8);
    // 0x8015EDBC: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015EDC0: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015EDC4: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015EDC8: subu        $v1, $v1, $t0
    ctx->r3 = SUB32(ctx->r3, ctx->r8);
    // 0x8015EDCC: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015EDD0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015EDD4: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015EDD8: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x8015EDDC: lw          $v0, 0x14($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X14);
    // 0x8015EDE0: sll         $t0, $a0, 16
    ctx->r8 = S32(ctx->r4 << 16);
    // 0x8015EDE4: or          $t0, $t0, $a2
    ctx->r8 = ctx->r8 | ctx->r6;
    // 0x8015EDE8: sw          $t0, 0x118($v0)
    MEM_W(0X118, ctx->r2) = ctx->r8;
    // 0x8015EDEC: lw          $t1, 0x14($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X14);
    // 0x8015EDF0: sll         $v0, $a1, 16
    ctx->r2 = S32(ctx->r5 << 16);
    // 0x8015EDF4: or          $v0, $v0, $a3
    ctx->r2 = ctx->r2 | ctx->r7;
    // 0x8015EDF8: sw          $v0, 0x11C($t1)
    MEM_W(0X11C, ctx->r9) = ctx->r2;
    // 0x8015EDFC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EE00: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x8015EE04: swc1        $f0, 0x1204($at)
    MEM_W(0X1204, ctx->r1) = ctx->f0.u32l;
    // 0x8015EE08: mtc1        $a2, $f0
    ctx->f0.u32l = ctx->r6;
    // 0x8015EE0C: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x8015EE10: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EE14: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x8015EE18: swc1        $f0, 0x120C($at)
    MEM_W(0X120C, ctx->r1) = ctx->f0.u32l;
    // 0x8015EE1C: mtc1        $a1, $f0
    ctx->f0.u32l = ctx->r5;
    // 0x8015EE20: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x8015EE24: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EE28: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x8015EE2C: swc1        $f0, 0x1208($at)
    MEM_W(0X1208, ctx->r1) = ctx->f0.u32l;
    // 0x8015EE30: mtc1        $a3, $f0
    ctx->f0.u32l = ctx->r7;
    // 0x8015EE34: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x8015EE38: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EE3C: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x8015EE40: sw          $t0, 0x1184($at)
    MEM_W(0X1184, ctx->r1) = ctx->r8;
    // 0x8015EE44: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EE48: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x8015EE4C: sw          $v0, 0x1188($at)
    MEM_W(0X1188, ctx->r1) = ctx->r2;
    // 0x8015EE50: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EE54: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x8015EE58: swc1        $f0, 0x1210($at)
    MEM_W(0X1210, ctx->r1) = ctx->f0.u32l;
    // 0x8015EE5C: jr          $ra
    // 0x8015EE60: nop

    return;
    // 0x8015EE60: nop

;}
RECOMP_FUNC void func_8015EE70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015EE70: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8015EE74: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x8015EE78: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015EE7C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015EE80: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015EE84: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015EE88: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015EE8C: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015EE90: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015EE94: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015EE98: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015EE9C: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015EEA0: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015EEA4: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015EEA8: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015EEAC: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015EEB0: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015EEB4: addu        $s1, $v1, $v0
    ctx->r17 = ADD32(ctx->r3, ctx->r2);
    // 0x8015EEB8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EEBC: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015EEC0: lw          $s0, 0x1170($at)
    ctx->r16 = MEM_W(ctx->r1, 0X1170);
    // 0x8015EEC4: lui         $v0, 0xFFFE
    ctx->r2 = S32(0XFFFE << 16);
    // 0x8015EEC8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EECC: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015EED0: lw          $v1, 0x121C($at)
    ctx->r3 = MEM_W(ctx->r1, 0X121C);
    // 0x8015EED4: ori         $v0, $v0, 0xEC
    ctx->r2 = ctx->r2 | 0XEC;
    // 0x8015EED8: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015EEDC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EEE0: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015EEE4: sw          $zero, 0x1224($at)
    MEM_W(0X1224, ctx->r1) = 0;
    // 0x8015EEE8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EEEC: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015EEF0: sw          $zero, 0x1220($at)
    MEM_W(0X1220, ctx->r1) = 0;
    // 0x8015EEF4: lw          $t1, 0x30($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X30);
    // 0x8015EEF8: and         $t2, $s0, $a0
    ctx->r10 = ctx->r16 & ctx->r4;
    // 0x8015EEFC: and         $s0, $s0, $v0
    ctx->r16 = ctx->r16 & ctx->r2;
    // 0x8015EF00: lui         $v0, 0x400
    ctx->r2 = S32(0X400 << 16);
    // 0x8015EF04: bne         $v1, $zero, L_8015EF18
    if (ctx->r3 != 0) {
        // 0x8015EF08: or          $s0, $s0, $v0
        ctx->r16 = ctx->r16 | ctx->r2;
            goto L_8015EF18;
    }
    // 0x8015EF08: or          $s0, $s0, $v0
    ctx->r16 = ctx->r16 | ctx->r2;
    // 0x8015EF0C: lui         $v0, 0xF7FF
    ctx->r2 = S32(0XF7FF << 16);
    // 0x8015EF10: ori         $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 | 0XFFFF;
    // 0x8015EF14: and         $s0, $s0, $v0
    ctx->r16 = ctx->r16 & ctx->r2;
L_8015EF18:
    // 0x8015EF18: andi        $v1, $a1, 0x7
    ctx->r3 = ctx->r5 & 0X7;
    // 0x8015EF1C: sll         $v0, $v1, 10
    ctx->r2 = S32(ctx->r3 << 10);
    // 0x8015EF20: or          $s0, $s0, $v0
    ctx->r16 = ctx->r16 | ctx->r2;
    // 0x8015EF24: andi        $v0, $a1, 0x8
    ctx->r2 = ctx->r5 & 0X8;
    // 0x8015EF28: beql        $v0, $zero, L_8015EF30
    if (ctx->r2 == 0) {
        // 0x8015EF2C: ori         $s0, $s0, 0x2000
        ctx->r16 = ctx->r16 | 0X2000;
            goto L_8015EF30;
    }
    goto skip_0;
    // 0x8015EF2C: ori         $s0, $s0, 0x2000
    ctx->r16 = ctx->r16 | 0X2000;
    skip_0:
L_8015EF30:
    // 0x8015EF30: addiu       $v0, $zero, 0x4
    ctx->r2 = ADD32(0, 0X4);
    // 0x8015EF34: bne         $v1, $v0, L_8015EF54
    if (ctx->r3 != ctx->r2) {
        // 0x8015EF38: sll         $v0, $a2, 4
        ctx->r2 = S32(ctx->r6 << 4);
            goto L_8015EF54;
    }
    // 0x8015EF38: sll         $v0, $a2, 4
    ctx->r2 = S32(ctx->r6 << 4);
    // 0x8015EF3C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8015EF40: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EF44: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015EF48: sw          $v0, 0x1224($at)
    MEM_W(0X1224, ctx->r1) = ctx->r2;
    // 0x8015EF4C: or          $s0, $s0, $a0
    ctx->r16 = ctx->r16 | ctx->r4;
    // 0x8015EF50: sll         $v0, $a2, 4
    ctx->r2 = S32(ctx->r6 << 4);
L_8015EF54:
    // 0x8015EF54: bne         $a2, $zero, L_8015EF6C
    if (ctx->r6 != 0) {
        // 0x8015EF58: or          $s0, $s0, $v0
        ctx->r16 = ctx->r16 | ctx->r2;
            goto L_8015EF6C;
    }
    // 0x8015EF58: or          $s0, $s0, $v0
    ctx->r16 = ctx->r16 | ctx->r2;
    // 0x8015EF5C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8015EF60: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EF64: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015EF68: sw          $v0, 0x1220($at)
    MEM_W(0X1220, ctx->r1) = ctx->r2;
L_8015EF6C:
    // 0x8015EF6C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8015EF70: bne         $a3, $v0, L_8015EF8C
    if (ctx->r7 != ctx->r2) {
        // 0x8015EF74: or          $s0, $s0, $a3
        ctx->r16 = ctx->r16 | ctx->r7;
            goto L_8015EF8C;
    }
    // 0x8015EF74: or          $s0, $s0, $a3
    ctx->r16 = ctx->r16 | ctx->r7;
    // 0x8015EF78: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EF7C: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015EF80: sw          $v0, 0x1224($at)
    MEM_W(0X1224, ctx->r1) = ctx->r2;
    // 0x8015EF84: j           L_8015EFA0
    // 0x8015EF88: or          $s0, $s0, $a0
    ctx->r16 = ctx->r16 | ctx->r4;
        goto L_8015EFA0;
    // 0x8015EF88: or          $s0, $s0, $a0
    ctx->r16 = ctx->r16 | ctx->r4;
L_8015EF8C:
    // 0x8015EF8C: bne         $a3, $zero, L_8015EFA0
    if (ctx->r7 != 0) {
        // 0x8015EF90: nop
    
            goto L_8015EFA0;
    }
    // 0x8015EF90: nop

    // 0x8015EF94: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015EF98: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015EF9C: sw          $v0, 0x1220($at)
    MEM_W(0X1220, ctx->r1) = ctx->r2;
L_8015EFA0:
    // 0x8015EFA0: beq         $t1, $zero, L_8015EFAC
    if (ctx->r9 == 0) {
        // 0x8015EFA4: lui         $v0, 0x1
        ctx->r2 = S32(0X1 << 16);
            goto L_8015EFAC;
    }
    // 0x8015EFA4: lui         $v0, 0x1
    ctx->r2 = S32(0X1 << 16);
    // 0x8015EFA8: or          $s0, $s0, $v0
    ctx->r16 = ctx->r16 | ctx->r2;
L_8015EFAC:
    // 0x8015EFAC: sltiu       $v0, $t0, 0x11
    ctx->r2 = ctx->r8 < 0X11 ? 1 : 0;
    // 0x8015EFB0: beq         $v0, $zero, L_8015F018
    if (ctx->r2 == 0) {
        // 0x8015EFB4: sll         $v0, $t0, 2
        ctx->r2 = S32(ctx->r8 << 2);
            goto L_8015F018;
    }
    // 0x8015EFB4: sll         $v0, $t0, 2
    ctx->r2 = S32(ctx->r8 << 2);
    // 0x8015EFB8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8015EFBC: addu        $at, $at, $v0
    gpr jr_addend_8015EFC4 = ctx->r2;
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x8015EFC0: lw          $v0, 0x5FC8($at)
    ctx->r2 = ADD32(ctx->r1, 0X5FC8);
    // 0x8015EFC4: jr          $v0
    // 0x8015EFC8: nop

    switch (jr_addend_8015EFC4 >> 2) {
        case 0: goto L_8015EFCC; break;
        case 1: goto L_8015EFD4; break;
        case 2: goto L_8015EFDC; break;
        case 3: goto L_8015F018; break;
        case 4: goto L_8015EFE4; break;
        case 5: goto L_8015EFEC; break;
        case 6: goto L_8015EFF4; break;
        case 7: goto L_8015EFFC; break;
        case 8: goto L_8015F004; break;
        case 9: goto L_8015F00C; break;
        case 10: goto L_8015F018; break;
        case 11: goto L_8015F018; break;
        case 12: goto L_8015F018; break;
        case 13: goto L_8015F018; break;
        case 14: goto L_8015F018; break;
        case 15: goto L_8015F018; break;
        case 16: goto L_8015F014; break;
        default: switch_error(__func__, 0x8015EFC4, 0x80175FC8);
    }
    // 0x8015EFC8: nop

L_8015EFCC:
    // 0x8015EFCC: j           L_8015F018
    // 0x8015EFD0: ori         $s0, $s0, 0x100
    ctx->r16 = ctx->r16 | 0X100;
        goto L_8015F018;
    // 0x8015EFD0: ori         $s0, $s0, 0x100
    ctx->r16 = ctx->r16 | 0X100;
L_8015EFD4:
    // 0x8015EFD4: j           L_8015F018
    // 0x8015EFD8: ori         $s0, $s0, 0x4100
    ctx->r16 = ctx->r16 | 0X4100;
        goto L_8015F018;
    // 0x8015EFD8: ori         $s0, $s0, 0x4100
    ctx->r16 = ctx->r16 | 0X4100;
L_8015EFDC:
    // 0x8015EFDC: j           L_8015F018
    // 0x8015EFE0: ori         $s0, $s0, 0x8100
    ctx->r16 = ctx->r16 | 0X8100;
        goto L_8015F018;
    // 0x8015EFE0: ori         $s0, $s0, 0x8100
    ctx->r16 = ctx->r16 | 0X8100;
L_8015EFE4:
    // 0x8015EFE4: j           L_8015F018
    // 0x8015EFE8: ori         $s0, $s0, 0x4000
    ctx->r16 = ctx->r16 | 0X4000;
        goto L_8015F018;
    // 0x8015EFE8: ori         $s0, $s0, 0x4000
    ctx->r16 = ctx->r16 | 0X4000;
L_8015EFEC:
    // 0x8015EFEC: j           L_8015F018
    // 0x8015EFF0: ori         $s0, $s0, 0x8000
    ctx->r16 = ctx->r16 | 0X8000;
        goto L_8015F018;
    // 0x8015EFF0: ori         $s0, $s0, 0x8000
    ctx->r16 = ctx->r16 | 0X8000;
L_8015EFF4:
    // 0x8015EFF4: j           L_8015F018
    // 0x8015EFF8: ori         $s0, $s0, 0x200
    ctx->r16 = ctx->r16 | 0X200;
        goto L_8015F018;
    // 0x8015EFF8: ori         $s0, $s0, 0x200
    ctx->r16 = ctx->r16 | 0X200;
L_8015EFFC:
    // 0x8015EFFC: j           L_8015F018
    // 0x8015F000: ori         $s0, $s0, 0x4200
    ctx->r16 = ctx->r16 | 0X4200;
        goto L_8015F018;
    // 0x8015F000: ori         $s0, $s0, 0x4200
    ctx->r16 = ctx->r16 | 0X4200;
L_8015F004:
    // 0x8015F004: j           L_8015F018
    // 0x8015F008: ori         $s0, $s0, 0x8200
    ctx->r16 = ctx->r16 | 0X8200;
        goto L_8015F018;
    // 0x8015F008: ori         $s0, $s0, 0x8200
    ctx->r16 = ctx->r16 | 0X8200;
L_8015F00C:
    // 0x8015F00C: j           L_8015F018
    // 0x8015F010: ori         $s0, $s0, 0x4300
    ctx->r16 = ctx->r16 | 0X4300;
        goto L_8015F018;
    // 0x8015F010: ori         $s0, $s0, 0x4300
    ctx->r16 = ctx->r16 | 0X4300;
L_8015F014:
    // 0x8015F014: ori         $s0, $s0, 0x8300
    ctx->r16 = ctx->r16 | 0X8300;
L_8015F018:
    // 0x8015F018: lui         $v0, 0x800
    ctx->r2 = S32(0X800 << 16);
    // 0x8015F01C: and         $v0, $s0, $v0
    ctx->r2 = ctx->r16 & ctx->r2;
    // 0x8015F020: beq         $t2, $v0, L_8015F038
    if (ctx->r10 == ctx->r2) {
        // 0x8015F024: lui         $a0, 0x800
        ctx->r4 = S32(0X800 << 16);
            goto L_8015F038;
    }
    // 0x8015F024: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F028: sw          $zero, 0x10($gp)
    MEM_W(0X10, ctx->r28) = 0;
    // 0x8015F02C: ori         $a0, $a0, 0x120
    ctx->r4 = ctx->r4 | 0X120;
    // 0x8015F030: jal         0x80167848
    // 0x8015F034: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015F034: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    after_0:
L_8015F038:
    // 0x8015F038: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F03C: ori         $a0, $a0, 0x104
    ctx->r4 = ctx->r4 | 0X104;
    // 0x8015F040: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F044: jal         0x80167848
    // 0x8015F048: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x8015F048: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_1:
    // 0x8015F04C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F050: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F054: sw          $s0, 0x1170($at)
    MEM_W(0X1170, ctx->r1) = ctx->r16;
    // 0x8015F058: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8015F05C: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015F060: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F064: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8015F068: jr          $ra
    // 0x8015F06C: nop

    return;
    // 0x8015F06C: nop

;}
RECOMP_FUNC void func_8015F070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F070: lui         $a1, 0x8023
    ctx->r5 = S32(0X8023 << 16);
    // 0x8015F074: lw          $a1, -0x5BBC($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X5BBC);
    // 0x8015F078: sll         $v1, $a1, 3
    ctx->r3 = S32(ctx->r5 << 3);
    // 0x8015F07C: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x8015F080: sll         $v1, $v1, 3
    ctx->r3 = S32(ctx->r3 << 3);
    // 0x8015F084: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x8015F088: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F08C: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x8015F090: sll         $v0, $v1, 4
    ctx->r2 = S32(ctx->r3 << 4);
    // 0x8015F094: subu        $v0, $v0, $v1
    ctx->r2 = SUB32(ctx->r2, ctx->r3);
    // 0x8015F098: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F09C: subu        $v0, $v0, $a1
    ctx->r2 = SUB32(ctx->r2, ctx->r5);
    // 0x8015F0A0: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F0A4: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x8015F0A8: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x8015F0AC: sw          $a0, 0x7C4C($at)
    MEM_W(0X7C4C, ctx->r1) = ctx->r4;
    // 0x8015F0B0: jr          $ra
    // 0x8015F0B4: nop

    return;
    // 0x8015F0B4: nop

;}
RECOMP_FUNC void func_8015F0B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F0B8: addu        $a3, $a1, $zero
    ctx->r7 = ADD32(ctx->r5, 0);
    // 0x8015F0BC: lui         $a1, 0x8023
    ctx->r5 = S32(0X8023 << 16);
    // 0x8015F0C0: lw          $a1, -0x5BBC($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X5BBC);
    // 0x8015F0C4: sll         $v0, $a1, 3
    ctx->r2 = S32(ctx->r5 << 3);
    // 0x8015F0C8: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x8015F0CC: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F0D0: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x8015F0D4: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F0D8: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x8015F0DC: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015F0E0: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015F0E4: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F0E8: subu        $v1, $v1, $a1
    ctx->r3 = SUB32(ctx->r3, ctx->r5);
    // 0x8015F0EC: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F0F0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015F0F4: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015F0F8: addu        $a2, $v1, $v0
    ctx->r6 = ADD32(ctx->r3, ctx->r2);
    // 0x8015F0FC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F100: addu        $at, $a2, $at
    ctx->r1 = ADD32(ctx->r6, ctx->r1);
    // 0x8015F104: lw          $a1, 0x117C($at)
    ctx->r5 = MEM_W(ctx->r1, 0X117C);
    // 0x8015F108: beq         $a0, $zero, L_8015F118
    if (ctx->r4 == 0) {
        // 0x8015F10C: addiu       $v0, $zero, -0x201
        ctx->r2 = ADD32(0, -0X201);
            goto L_8015F118;
    }
    // 0x8015F10C: addiu       $v0, $zero, -0x201
    ctx->r2 = ADD32(0, -0X201);
    // 0x8015F110: j           L_8015F11C
    // 0x8015F114: ori         $a1, $a1, 0x200
    ctx->r5 = ctx->r5 | 0X200;
        goto L_8015F11C;
    // 0x8015F114: ori         $a1, $a1, 0x200
    ctx->r5 = ctx->r5 | 0X200;
L_8015F118:
    // 0x8015F118: and         $a1, $a1, $v0
    ctx->r5 = ctx->r5 & ctx->r2;
L_8015F11C:
    // 0x8015F11C: andi        $v0, $a1, 0x10
    ctx->r2 = ctx->r5 & 0X10;
    // 0x8015F120: bne         $v0, $zero, L_8015F15C
    if (ctx->r2 != 0) {
        // 0x8015F124: addiu       $v0, $zero, 0x3
        ctx->r2 = ADD32(0, 0X3);
            goto L_8015F15C;
    }
    // 0x8015F124: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x8015F128: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F12C: addu        $at, $a2, $at
    ctx->r1 = ADD32(ctx->r6, ctx->r1);
    // 0x8015F130: lw          $v1, 0x123C($at)
    ctx->r3 = MEM_W(ctx->r1, 0X123C);
    // 0x8015F134: beq         $v1, $v0, L_8015F15C
    if (ctx->r3 == ctx->r2) {
        // 0x8015F138: nop
    
            goto L_8015F15C;
    }
    // 0x8015F138: nop

    // 0x8015F13C: beq         $a3, $zero, L_8015F150
    if (ctx->r7 == 0) {
        // 0x8015F140: lui         $v0, 0x4
        ctx->r2 = S32(0X4 << 16);
            goto L_8015F150;
    }
    // 0x8015F140: lui         $v0, 0x4
    ctx->r2 = S32(0X4 << 16);
    // 0x8015F144: ori         $v0, $v0, 0x400
    ctx->r2 = ctx->r2 | 0X400;
    // 0x8015F148: j           L_8015F15C
    // 0x8015F14C: or          $a1, $a1, $v0
    ctx->r5 = ctx->r5 | ctx->r2;
        goto L_8015F15C;
    // 0x8015F14C: or          $a1, $a1, $v0
    ctx->r5 = ctx->r5 | ctx->r2;
L_8015F150:
    // 0x8015F150: lui         $v0, 0xFFFB
    ctx->r2 = S32(0XFFFB << 16);
    // 0x8015F154: ori         $v0, $v0, 0xFBFF
    ctx->r2 = ctx->r2 | 0XFBFF;
    // 0x8015F158: and         $a1, $a1, $v0
    ctx->r5 = ctx->r5 & ctx->r2;
L_8015F15C:
    // 0x8015F15C: lw          $v0, 0x14($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X14);
    // 0x8015F160: sw          $a1, 0x110($v0)
    MEM_W(0X110, ctx->r2) = ctx->r5;
    // 0x8015F164: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F168: addu        $at, $a2, $at
    ctx->r1 = ADD32(ctx->r6, ctx->r1);
    // 0x8015F16C: sw          $a1, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r5;
    // 0x8015F170: jr          $ra
    // 0x8015F174: nop

    return;
    // 0x8015F174: nop

;}
RECOMP_FUNC void func_8015F178(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F178: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8015F17C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8015F180: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    // 0x8015F184: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015F188: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8015F18C: jal         0x8015E200
    // 0x8015F190: addu        $s0, $gp, $zero
    ctx->r16 = ADD32(ctx->r28, 0);
    static_0_8015E200(rdram, ctx);
        goto after_0;
    // 0x8015F190: addu        $s0, $gp, $zero
    ctx->r16 = ADD32(ctx->r28, 0);
    after_0:
    // 0x8015F194: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x8015F198: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F19C: ori         $a0, $a0, 0x144
    ctx->r4 = ctx->r4 | 0X144;
    // 0x8015F1A0: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    // 0x8015F1A4: sw          $v0, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r2;
    // 0x8015F1A8: jal         0x80167848
    // 0x8015F1AC: sw          $v0, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->r2;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x8015F1AC: sw          $v0, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->r2;
    after_1:
    // 0x8015F1B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8015F1B4: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F1B8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8015F1BC: jr          $ra
    // 0x8015F1C0: nop

    return;
    // 0x8015F1C0: nop

;}
RECOMP_FUNC void func_8015F23C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F23C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8015F240: lw          $v0, 0x3440($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3440);
    // 0x8015F244: beq         $a0, $v0, L_8015F25C
    if (ctx->r4 == ctx->r2) {
        // 0x8015F248: nop
    
            goto L_8015F25C;
    }
    // 0x8015F248: nop

    // 0x8015F24C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015F250: sw          $a0, 0x3440($at)
    MEM_W(0X3440, ctx->r1) = ctx->r4;
    // 0x8015F254: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x8015F258: sw          $a0, 0x7C60($at)
    MEM_W(0X7C60, ctx->r1) = ctx->r4;
L_8015F25C:
    // 0x8015F25C: jr          $ra
    // 0x8015F260: nop

    return;
    // 0x8015F260: nop

;}
RECOMP_FUNC void func_8015F2F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F2F0: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8015F2F4: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015F2F8: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F2FC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8015F300: jr          $ra
    // 0x8015F304: nop

    return;
    // 0x8015F304: nop

;}
RECOMP_FUNC void func_8015F314(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F314: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8015F318: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x8015F31C: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015F320: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015F324: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015F328: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F32C: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F330: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F334: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F338: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F33C: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015F340: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015F344: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F348: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015F34C: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F350: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8015F354: lw          $a0, 0x3444($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X3444);
    // 0x8015F358: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015F35C: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015F360: beq         $a1, $a0, L_8015F3AC
    if (ctx->r5 == ctx->r4) {
        // 0x8015F364: addu        $s1, $v1, $v0
        ctx->r17 = ADD32(ctx->r3, ctx->r2);
            goto L_8015F3AC;
    }
    // 0x8015F364: addu        $s1, $v1, $v0
    ctx->r17 = ADD32(ctx->r3, ctx->r2);
    // 0x8015F368: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F36C: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F370: lw          $s0, 0x117C($at)
    ctx->r16 = MEM_W(ctx->r1, 0X117C);
    // 0x8015F374: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F378: ori         $a0, $a0, 0x110
    ctx->r4 = ctx->r4 | 0X110;
    // 0x8015F37C: addiu       $v0, $zero, -0xE1
    ctx->r2 = ADD32(0, -0XE1);
    // 0x8015F380: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015F384: sw          $a1, 0x3444($at)
    MEM_W(0X3444, ctx->r1) = ctx->r5;
    // 0x8015F388: and         $s0, $s0, $v0
    ctx->r16 = ctx->r16 & ctx->r2;
    // 0x8015F38C: sll         $v0, $a1, 5
    ctx->r2 = S32(ctx->r5 << 5);
    // 0x8015F390: or          $s0, $s0, $v0
    ctx->r16 = ctx->r16 | ctx->r2;
    // 0x8015F394: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F398: jal         0x80167848
    // 0x8015F39C: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015F39C: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_0:
    // 0x8015F3A0: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F3A4: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F3A8: sw          $s0, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r16;
L_8015F3AC:
    // 0x8015F3AC: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8015F3B0: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015F3B4: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F3B8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8015F3BC: jr          $ra
    // 0x8015F3C0: nop

    return;
    // 0x8015F3C0: nop

;}
RECOMP_FUNC void func_8015F3D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F3D0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8015F3D4: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x8015F3D8: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015F3DC: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015F3E0: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015F3E4: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F3E8: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F3EC: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F3F0: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F3F4: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F3F8: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015F3FC: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015F400: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F404: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015F408: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F40C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8015F410: lw          $a0, 0x3448($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X3448);
    // 0x8015F414: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015F418: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015F41C: beq         $a1, $a0, L_8015F480
    if (ctx->r5 == ctx->r4) {
        // 0x8015F420: addu        $s1, $v1, $v0
        ctx->r17 = ADD32(ctx->r3, ctx->r2);
            goto L_8015F480;
    }
    // 0x8015F420: addu        $s1, $v1, $v0
    ctx->r17 = ADD32(ctx->r3, ctx->r2);
    // 0x8015F424: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F428: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F42C: lw          $s0, 0x117C($at)
    ctx->r16 = MEM_W(ctx->r1, 0X117C);
    // 0x8015F430: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015F434: sw          $a1, 0x3448($at)
    MEM_W(0X3448, ctx->r1) = ctx->r5;
    // 0x8015F438: bne         $a1, $zero, L_8015F448
    if (ctx->r5 != 0) {
        // 0x8015F43C: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_8015F448;
    }
    // 0x8015F43C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x8015F440: j           L_8015F45C
    // 0x8015F444: addiu       $v0, $zero, -0x11
    ctx->r2 = ADD32(0, -0X11);
        goto L_8015F45C;
    // 0x8015F444: addiu       $v0, $zero, -0x11
    ctx->r2 = ADD32(0, -0X11);
L_8015F448:
    // 0x8015F448: bne         $a1, $v0, L_8015F458
    if (ctx->r5 != ctx->r2) {
        // 0x8015F44C: ori         $s0, $s0, 0x10
        ctx->r16 = ctx->r16 | 0X10;
            goto L_8015F458;
    }
    // 0x8015F44C: ori         $s0, $s0, 0x10
    ctx->r16 = ctx->r16 | 0X10;
    // 0x8015F450: j           L_8015F460
    // 0x8015F454: ori         $s0, $s0, 0x8
    ctx->r16 = ctx->r16 | 0X8;
        goto L_8015F460;
    // 0x8015F454: ori         $s0, $s0, 0x8
    ctx->r16 = ctx->r16 | 0X8;
L_8015F458:
    // 0x8015F458: addiu       $v0, $zero, -0x9
    ctx->r2 = ADD32(0, -0X9);
L_8015F45C:
    // 0x8015F45C: and         $s0, $s0, $v0
    ctx->r16 = ctx->r16 & ctx->r2;
L_8015F460:
    // 0x8015F460: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F464: ori         $a0, $a0, 0x110
    ctx->r4 = ctx->r4 | 0X110;
    // 0x8015F468: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F46C: jal         0x80167848
    // 0x8015F470: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015F470: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_0:
    // 0x8015F474: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F478: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F47C: sw          $s0, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r16;
L_8015F480:
    // 0x8015F480: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8015F484: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015F488: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F48C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8015F490: jr          $ra
    // 0x8015F494: nop

    return;
    // 0x8015F494: nop

;}
RECOMP_FUNC void func_8015F4A4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F4A4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8015F4A8: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x8015F4AC: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015F4B0: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015F4B4: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015F4B8: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F4BC: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F4C0: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F4C4: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F4C8: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F4CC: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015F4D0: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015F4D4: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F4D8: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015F4DC: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F4E0: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8015F4E4: lw          $a0, 0x344C($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X344C);
    // 0x8015F4E8: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015F4EC: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015F4F0: beq         $a1, $a0, L_8015F540
    if (ctx->r5 == ctx->r4) {
        // 0x8015F4F4: addu        $s1, $v1, $v0
        ctx->r17 = ADD32(ctx->r3, ctx->r2);
            goto L_8015F540;
    }
    // 0x8015F4F4: addu        $s1, $v1, $v0
    ctx->r17 = ADD32(ctx->r3, ctx->r2);
    // 0x8015F4F8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F4FC: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F500: lw          $s0, 0x117C($at)
    ctx->r16 = MEM_W(ctx->r1, 0X117C);
    // 0x8015F504: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015F508: sw          $a1, 0x344C($at)
    MEM_W(0X344C, ctx->r1) = ctx->r5;
    // 0x8015F50C: beq         $a1, $zero, L_8015F51C
    if (ctx->r5 == 0) {
        // 0x8015F510: addiu       $v0, $zero, -0x401
        ctx->r2 = ADD32(0, -0X401);
            goto L_8015F51C;
    }
    // 0x8015F510: addiu       $v0, $zero, -0x401
    ctx->r2 = ADD32(0, -0X401);
    // 0x8015F514: j           L_8015F520
    // 0x8015F518: ori         $s0, $s0, 0x400
    ctx->r16 = ctx->r16 | 0X400;
        goto L_8015F520;
    // 0x8015F518: ori         $s0, $s0, 0x400
    ctx->r16 = ctx->r16 | 0X400;
L_8015F51C:
    // 0x8015F51C: and         $s0, $s0, $v0
    ctx->r16 = ctx->r16 & ctx->r2;
L_8015F520:
    // 0x8015F520: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F524: ori         $a0, $a0, 0x110
    ctx->r4 = ctx->r4 | 0X110;
    // 0x8015F528: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F52C: jal         0x80167848
    // 0x8015F530: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015F530: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_0:
    // 0x8015F534: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F538: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F53C: sw          $s0, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r16;
L_8015F540:
    // 0x8015F540: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8015F544: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015F548: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F54C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8015F550: jr          $ra
    // 0x8015F554: nop

    return;
    // 0x8015F554: nop

;}
RECOMP_FUNC void func_8015F558(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F558: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8015F55C: lui         $a3, 0xFF00
    ctx->r7 = S32(0XFF00 << 16);
    // 0x8015F560: ori         $a3, $a3, 0xFF
    ctx->r7 = ctx->r7 | 0XFF;
    // 0x8015F564: lui         $a2, 0x4
    ctx->r6 = S32(0X4 << 16);
    // 0x8015F568: ori         $a2, $a2, 0x400
    ctx->r6 = ctx->r6 | 0X400;
    // 0x8015F56C: lui         $v1, 0x8023
    ctx->r3 = S32(0X8023 << 16);
    // 0x8015F570: lw          $v1, -0x5BBC($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X5BBC);
    // 0x8015F574: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F578: ori         $a0, $a0, 0x10C
    ctx->r4 = ctx->r4 | 0X10C;
    // 0x8015F57C: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x8015F580: lui         $s2, 0x801E
    ctx->r18 = S32(0X801E << 16);
    // 0x8015F584: addiu       $s2, $s2, 0x6A20
    ctx->r18 = ADD32(ctx->r18, 0X6A20);
    // 0x8015F588: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x8015F58C: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x8015F590: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015F594: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015F598: sll         $v0, $v1, 3
    ctx->r2 = S32(ctx->r3 << 3);
    // 0x8015F59C: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F5A0: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F5A4: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F5A8: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F5AC: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F5B0: sll         $s1, $v0, 4
    ctx->r17 = S32(ctx->r2 << 4);
    // 0x8015F5B4: subu        $s1, $s1, $v0
    ctx->r17 = SUB32(ctx->r17, ctx->r2);
    // 0x8015F5B8: sll         $s1, $s1, 2
    ctx->r17 = S32(ctx->r17 << 2);
    // 0x8015F5BC: subu        $s1, $s1, $v1
    ctx->r17 = SUB32(ctx->r17, ctx->r3);
    // 0x8015F5C0: sll         $s1, $s1, 2
    ctx->r17 = S32(ctx->r17 << 2);
    // 0x8015F5C4: addu        $s1, $s1, $s2
    ctx->r17 = ADD32(ctx->r17, ctx->r18);
    // 0x8015F5C8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F5CC: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F5D0: lw          $s0, 0x1178($at)
    ctx->r16 = MEM_W(ctx->r1, 0X1178);
    // 0x8015F5D4: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F5D8: addiu       $s3, $zero, -0x11
    ctx->r19 = ADD32(0, -0X11);
    // 0x8015F5DC: and         $s0, $s0, $s3
    ctx->r16 = ctx->r16 & ctx->r19;
    // 0x8015F5E0: and         $s0, $s0, $a3
    ctx->r16 = ctx->r16 & ctx->r7;
    // 0x8015F5E4: or          $s0, $s0, $a2
    ctx->r16 = ctx->r16 | ctx->r6;
    // 0x8015F5E8: jal         0x80167848
    // 0x8015F5EC: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015F5EC: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_0:
    // 0x8015F5F0: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F5F4: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F5F8: sw          $s0, 0x1178($at)
    MEM_W(0X1178, ctx->r1) = ctx->r16;
    // 0x8015F5FC: lui         $v1, 0x8023
    ctx->r3 = S32(0X8023 << 16);
    // 0x8015F600: lw          $v1, -0x5BBC($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X5BBC);
    // 0x8015F604: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F608: ori         $a0, $a0, 0x10C
    ctx->r4 = ctx->r4 | 0X10C;
    // 0x8015F60C: sll         $v0, $v1, 3
    ctx->r2 = S32(ctx->r3 << 3);
    // 0x8015F610: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F614: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F618: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F61C: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F620: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F624: sll         $s0, $v0, 4
    ctx->r16 = S32(ctx->r2 << 4);
    // 0x8015F628: subu        $s0, $s0, $v0
    ctx->r16 = SUB32(ctx->r16, ctx->r2);
    // 0x8015F62C: sll         $s0, $s0, 2
    ctx->r16 = S32(ctx->r16 << 2);
    // 0x8015F630: subu        $s0, $s0, $v1
    ctx->r16 = SUB32(ctx->r16, ctx->r3);
    // 0x8015F634: sll         $s0, $s0, 2
    ctx->r16 = S32(ctx->r16 << 2);
    // 0x8015F638: addu        $s0, $s0, $s2
    ctx->r16 = ADD32(ctx->r16, ctx->r18);
    // 0x8015F63C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F640: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015F644: lw          $s1, 0x1178($at)
    ctx->r17 = MEM_W(ctx->r1, 0X1178);
    // 0x8015F648: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F64C: addiu       $v0, $zero, -0x10
    ctx->r2 = ADD32(0, -0X10);
    // 0x8015F650: and         $s1, $s1, $v0
    ctx->r17 = ctx->r17 & ctx->r2;
    // 0x8015F654: jal         0x80167848
    // 0x8015F658: sw          $s1, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r17;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x8015F658: sw          $s1, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r17;
    after_1:
    // 0x8015F65C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F660: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015F664: sw          $s1, 0x1178($at)
    MEM_W(0X1178, ctx->r1) = ctx->r17;
    // 0x8015F668: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x8015F66C: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x8015F670: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015F674: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F678: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F67C: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F680: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F684: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F688: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015F68C: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015F690: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F694: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015F698: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8015F69C: lw          $v0, 0x3438($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3438);
    // 0x8015F6A0: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F6A4: beq         $v0, $zero, L_8015F6E8
    if (ctx->r2 == 0) {
        // 0x8015F6A8: addu        $s1, $v1, $s2
        ctx->r17 = ADD32(ctx->r3, ctx->r18);
            goto L_8015F6E8;
    }
    // 0x8015F6A8: addu        $s1, $v1, $s2
    ctx->r17 = ADD32(ctx->r3, ctx->r18);
    // 0x8015F6AC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F6B0: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F6B4: lw          $s0, 0x117C($at)
    ctx->r16 = MEM_W(ctx->r1, 0X117C);
    // 0x8015F6B8: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F6BC: ori         $a0, $a0, 0x110
    ctx->r4 = ctx->r4 | 0X110;
    // 0x8015F6C0: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F6C4: addiu       $v0, $zero, -0x3
    ctx->r2 = ADD32(0, -0X3);
    // 0x8015F6C8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015F6CC: sw          $zero, 0x3438($at)
    MEM_W(0X3438, ctx->r1) = 0;
    // 0x8015F6D0: and         $s0, $s0, $v0
    ctx->r16 = ctx->r16 & ctx->r2;
    // 0x8015F6D4: jal         0x80167848
    // 0x8015F6D8: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_2;
    // 0x8015F6D8: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_2:
    // 0x8015F6DC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F6E0: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F6E4: sw          $s0, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r16;
L_8015F6E8:
    // 0x8015F6E8: lui         $v1, 0x8023
    ctx->r3 = S32(0X8023 << 16);
    // 0x8015F6EC: lw          $v1, -0x5BBC($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X5BBC);
    // 0x8015F6F0: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F6F4: ori         $a0, $a0, 0x130
    ctx->r4 = ctx->r4 | 0X130;
    // 0x8015F6F8: sll         $v0, $v1, 3
    ctx->r2 = S32(ctx->r3 << 3);
    // 0x8015F6FC: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F700: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F704: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F708: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F70C: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F710: sll         $s0, $v0, 4
    ctx->r16 = S32(ctx->r2 << 4);
    // 0x8015F714: subu        $s0, $s0, $v0
    ctx->r16 = SUB32(ctx->r16, ctx->r2);
    // 0x8015F718: sll         $s0, $s0, 2
    ctx->r16 = S32(ctx->r16 << 2);
    // 0x8015F71C: subu        $s0, $s0, $v1
    ctx->r16 = SUB32(ctx->r16, ctx->r3);
    // 0x8015F720: sll         $s0, $s0, 2
    ctx->r16 = S32(ctx->r16 << 2);
    // 0x8015F724: addu        $s0, $s0, $s2
    ctx->r16 = ADD32(ctx->r16, ctx->r18);
    // 0x8015F728: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F72C: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015F730: lw          $s1, 0x1190($at)
    ctx->r17 = MEM_W(ctx->r1, 0X1190);
    // 0x8015F734: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F738: lui         $v0, 0xFFFF
    ctx->r2 = S32(0XFFFF << 16);
    // 0x8015F73C: and         $s1, $s1, $v0
    ctx->r17 = ctx->r17 & ctx->r2;
    // 0x8015F740: jal         0x80167848
    // 0x8015F744: sw          $s1, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r17;
    func_80167848(rdram, ctx);
        goto after_3;
    // 0x8015F744: sw          $s1, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r17;
    after_3:
    // 0x8015F748: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F74C: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015F750: sw          $s1, 0x1190($at)
    MEM_W(0X1190, ctx->r1) = ctx->r17;
    // 0x8015F754: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x8015F758: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x8015F75C: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015F760: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F764: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F768: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F76C: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F770: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F774: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015F778: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015F77C: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F780: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015F784: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8015F788: lw          $v0, 0x3448($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3448);
    // 0x8015F78C: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F790: beq         $v0, $zero, L_8015F7D0
    if (ctx->r2 == 0) {
        // 0x8015F794: addu        $s1, $v1, $s2
        ctx->r17 = ADD32(ctx->r3, ctx->r18);
            goto L_8015F7D0;
    }
    // 0x8015F794: addu        $s1, $v1, $s2
    ctx->r17 = ADD32(ctx->r3, ctx->r18);
    // 0x8015F798: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F79C: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F7A0: lw          $s0, 0x117C($at)
    ctx->r16 = MEM_W(ctx->r1, 0X117C);
    // 0x8015F7A4: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015F7A8: ori         $a0, $a0, 0x110
    ctx->r4 = ctx->r4 | 0X110;
    // 0x8015F7AC: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F7B0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015F7B4: sw          $zero, 0x3448($at)
    MEM_W(0X3448, ctx->r1) = 0;
    // 0x8015F7B8: and         $s0, $s0, $s3
    ctx->r16 = ctx->r16 & ctx->r19;
    // 0x8015F7BC: jal         0x80167848
    // 0x8015F7C0: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_4;
    // 0x8015F7C0: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_4:
    // 0x8015F7C4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F7C8: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F7CC: sw          $s0, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r16;
L_8015F7D0:
    // 0x8015F7D0: jal         0x8015F8D4
    // 0x8015F7D4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015F8D4(rdram, ctx);
        goto after_5;
    // 0x8015F7D4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_5:
    // 0x8015F7D8: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x8015F7DC: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x8015F7E0: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x8015F7E4: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015F7E8: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F7EC: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8015F7F0: jr          $ra
    // 0x8015F7F4: nop

    return;
    // 0x8015F7F4: nop

;}
RECOMP_FUNC void func_8015F804(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F804: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8015F808: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x8015F80C: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015F810: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015F814: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015F818: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F81C: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F820: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F824: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F828: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F82C: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015F830: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015F834: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F838: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015F83C: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F840: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015F844: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015F848: addu        $s1, $v1, $v0
    ctx->r17 = ADD32(ctx->r3, ctx->r2);
    // 0x8015F84C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F850: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F854: lw          $s0, 0x117C($at)
    ctx->r16 = MEM_W(ctx->r1, 0X117C);
    // 0x8015F858: lui         $v0, 0xFFF7
    ctx->r2 = S32(0XFFF7 << 16);
    // 0x8015F85C: ori         $v0, $v0, 0xF6FF
    ctx->r2 = ctx->r2 | 0XF6FF;
    // 0x8015F860: and         $s0, $s0, $v0
    ctx->r16 = ctx->r16 & ctx->r2;
    // 0x8015F864: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8015F868: beq         $a1, $v0, L_8015F88C
    if (ctx->r5 == ctx->r2) {
        // 0x8015F86C: slti        $v0, $a1, 0x2
        ctx->r2 = SIGNED(ctx->r5) < 0X2 ? 1 : 0;
            goto L_8015F88C;
    }
    // 0x8015F86C: slti        $v0, $a1, 0x2
    ctx->r2 = SIGNED(ctx->r5) < 0X2 ? 1 : 0;
    // 0x8015F870: bnel        $v0, $zero, L_8015F8A0
    if (ctx->r2 != 0) {
        // 0x8015F874: lui         $a0, 0x800
        ctx->r4 = S32(0X800 << 16);
            goto L_8015F8A0;
    }
    goto skip_0;
    // 0x8015F874: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    skip_0:
    // 0x8015F878: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x8015F87C: beq         $a1, $v0, L_8015F894
    if (ctx->r5 == ctx->r2) {
        // 0x8015F880: lui         $v0, 0x8
        ctx->r2 = S32(0X8 << 16);
            goto L_8015F894;
    }
    // 0x8015F880: lui         $v0, 0x8
    ctx->r2 = S32(0X8 << 16);
    // 0x8015F884: j           L_8015F8A0
    // 0x8015F888: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
        goto L_8015F8A0;
    // 0x8015F888: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
L_8015F88C:
    // 0x8015F88C: j           L_8015F89C
    // 0x8015F890: ori         $s0, $s0, 0x900
    ctx->r16 = ctx->r16 | 0X900;
        goto L_8015F89C;
    // 0x8015F890: ori         $s0, $s0, 0x900
    ctx->r16 = ctx->r16 | 0X900;
L_8015F894:
    // 0x8015F894: ori         $v0, $v0, 0x100
    ctx->r2 = ctx->r2 | 0X100;
    // 0x8015F898: or          $s0, $s0, $v0
    ctx->r16 = ctx->r16 | ctx->r2;
L_8015F89C:
    // 0x8015F89C: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
L_8015F8A0:
    // 0x8015F8A0: ori         $a0, $a0, 0x110
    ctx->r4 = ctx->r4 | 0X110;
    // 0x8015F8A4: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F8A8: jal         0x80167848
    // 0x8015F8AC: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015F8AC: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_0:
    // 0x8015F8B0: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F8B4: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F8B8: sw          $s0, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r16;
    // 0x8015F8BC: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8015F8C0: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015F8C4: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F8C8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8015F8CC: jr          $ra
    // 0x8015F8D0: nop

    return;
    // 0x8015F8D0: nop

;}
RECOMP_FUNC void func_8015F8E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F8E0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8015F8E4: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x8015F8E8: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015F8EC: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015F8F0: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015F8F4: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F8F8: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F8FC: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F900: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F904: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015F908: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015F90C: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015F910: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F914: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015F918: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015F91C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015F920: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015F924: addu        $s1, $v1, $v0
    ctx->r17 = ADD32(ctx->r3, ctx->r2);
    // 0x8015F928: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F92C: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F930: lw          $s0, 0x1174($at)
    ctx->r16 = MEM_W(ctx->r1, 0X1174);
    // 0x8015F934: addiu       $v0, $zero, -0x40
    ctx->r2 = ADD32(0, -0X40);
    // 0x8015F938: and         $s0, $s0, $v0
    ctx->r16 = ctx->r16 & ctx->r2;
    // 0x8015F93C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8015F940: beq         $a1, $v0, L_8015F964
    if (ctx->r5 == ctx->r2) {
        // 0x8015F944: slti        $v0, $a1, 0x2
        ctx->r2 = SIGNED(ctx->r5) < 0X2 ? 1 : 0;
            goto L_8015F964;
    }
    // 0x8015F944: slti        $v0, $a1, 0x2
    ctx->r2 = SIGNED(ctx->r5) < 0X2 ? 1 : 0;
    // 0x8015F948: bnel        $v0, $zero, L_8015F96C
    if (ctx->r2 != 0) {
        // 0x8015F94C: lui         $a0, 0x800
        ctx->r4 = S32(0X800 << 16);
            goto L_8015F96C;
    }
    goto skip_0;
    // 0x8015F94C: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    skip_0:
    // 0x8015F950: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x8015F954: beql        $a1, $v0, L_8015F968
    if (ctx->r5 == ctx->r2) {
        // 0x8015F958: ori         $s0, $s0, 0x1
        ctx->r16 = ctx->r16 | 0X1;
            goto L_8015F968;
    }
    goto skip_1;
    // 0x8015F958: ori         $s0, $s0, 0x1
    ctx->r16 = ctx->r16 | 0X1;
    skip_1:
    // 0x8015F95C: j           L_8015F96C
    // 0x8015F960: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
        goto L_8015F96C;
    // 0x8015F960: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
L_8015F964:
    // 0x8015F964: ori         $s0, $s0, 0x9
    ctx->r16 = ctx->r16 | 0X9;
L_8015F968:
    // 0x8015F968: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
L_8015F96C:
    // 0x8015F96C: ori         $a0, $a0, 0x108
    ctx->r4 = ctx->r4 | 0X108;
    // 0x8015F970: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015F974: jal         0x80167848
    // 0x8015F978: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015F978: sw          $s0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r16;
    after_0:
    // 0x8015F97C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015F980: addu        $at, $s1, $at
    ctx->r1 = ADD32(ctx->r17, ctx->r1);
    // 0x8015F984: sw          $s0, 0x1174($at)
    MEM_W(0X1174, ctx->r1) = ctx->r16;
    // 0x8015F988: jal         0x8015E008
    // 0x8015F98C: nop

    func_8015E008(rdram, ctx);
        goto after_1;
    // 0x8015F98C: nop

    after_1:
    // 0x8015F990: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8015F994: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015F998: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015F99C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8015F9A0: jr          $ra
    // 0x8015F9A4: nop

    return;
    // 0x8015F9A4: nop

;}
RECOMP_FUNC void func_8015F9B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015F9B0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8015F9B4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8015F9B8: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    // 0x8015F9BC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8015F9C0: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015F9C4: sll         $v0, $v1, 3
    ctx->r2 = S32(ctx->r3 << 3);
    // 0x8015F9C8: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F9CC: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015F9D0: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F9D4: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015F9D8: addu        $v0, $v0, $v1
    ctx->r2 = ADD32(ctx->r2, ctx->r3);
    // 0x8015F9DC: sll         $s0, $v0, 4
    ctx->r16 = S32(ctx->r2 << 4);
    // 0x8015F9E0: subu        $s0, $s0, $v0
    ctx->r16 = SUB32(ctx->r16, ctx->r2);
    // 0x8015F9E4: sll         $s0, $s0, 2
    ctx->r16 = S32(ctx->r16 << 2);
    // 0x8015F9E8: subu        $s0, $s0, $v1
    ctx->r16 = SUB32(ctx->r16, ctx->r3);
    // 0x8015F9EC: sll         $s0, $s0, 2
    ctx->r16 = S32(ctx->r16 << 2);
    // 0x8015F9F0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015F9F4: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015F9F8: jal         0x8015E200
    // 0x8015F9FC: addu        $s0, $s0, $v0
    ctx->r16 = ADD32(ctx->r16, ctx->r2);
    static_0_8015E200(rdram, ctx);
        goto after_0;
    // 0x8015F9FC: addu        $s0, $s0, $v0
    ctx->r16 = ADD32(ctx->r16, ctx->r2);
    after_0:
    // 0x8015FA00: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x8015FA04: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015FA08: ori         $a0, $a0, 0x12C
    ctx->r4 = ctx->r4 | 0X12C;
    // 0x8015FA0C: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x8015FA10: jal         0x80167848
    // 0x8015FA14: sw          $v0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r2;
    func_80167848(rdram, ctx);
        goto after_1;
    // 0x8015FA14: sw          $v0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r2;
    after_1:
    // 0x8015FA18: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x8015FA1C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015FA20: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015FA24: sw          $v0, 0x118C($at)
    MEM_W(0X118C, ctx->r1) = ctx->r2;
    // 0x8015FA28: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8015FA2C: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015FA30: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8015FA34: jr          $ra
    // 0x8015FA38: nop

    return;
    // 0x8015FA38: nop

;}
RECOMP_FUNC void func_8015FA3C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015FA3C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8015FA40: sw          $s4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r20;
    // 0x8015FA44: addiu       $s4, $zero, 0x1F
    ctx->r20 = ADD32(0, 0X1F);
    // 0x8015FA48: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x8015FA4C: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x8015FA50: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015FA54: addu        $s1, $zero, $zero
    ctx->r17 = ADD32(0, 0);
    // 0x8015FA58: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x8015FA5C: lui         $s3, 0x800
    ctx->r19 = S32(0X800 << 16);
    // 0x8015FA60: ori         $s3, $s3, 0x160
    ctx->r19 = ctx->r19 | 0X160;
    // 0x8015FA64: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015FA68: addiu       $s0, $s2, 0x2
    ctx->r16 = ADD32(ctx->r18, 0X2);
    // 0x8015FA6C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
L_8015FA70:
    // 0x8015FA70: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
    // 0x8015FA74: lbu         $v1, -0x1($s0)
    ctx->r3 = MEM_BU(ctx->r16, -0X1);
    // 0x8015FA78: subu        $v0, $v1, $a0
    ctx->r2 = SUB32(ctx->r3, ctx->r4);
    // 0x8015FA7C: sll         $a1, $v0, 2
    ctx->r5 = S32(ctx->r2 << 2);
    // 0x8015FA80: beq         $s1, $s4, L_8015FA8C
    if (ctx->r17 == ctx->r20) {
        // 0x8015FA84: addu        $v0, $v1, $zero
        ctx->r2 = ADD32(ctx->r3, 0);
            goto L_8015FA8C;
    }
    // 0x8015FA84: addu        $v0, $v1, $zero
    ctx->r2 = ADD32(ctx->r3, 0);
    // 0x8015FA88: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
L_8015FA8C:
    // 0x8015FA8C: subu        $v0, $v0, $v1
    ctx->r2 = SUB32(ctx->r2, ctx->r3);
    // 0x8015FA90: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015FA94: sll         $v1, $v1, 24
    ctx->r3 = S32(ctx->r3 << 24);
    // 0x8015FA98: andi        $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 & 0XFF;
    // 0x8015FA9C: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x8015FAA0: or          $v1, $v1, $v0
    ctx->r3 = ctx->r3 | ctx->r2;
    // 0x8015FAA4: sll         $v0, $a0, 8
    ctx->r2 = S32(ctx->r4 << 8);
    // 0x8015FAA8: or          $v1, $v1, $v0
    ctx->r3 = ctx->r3 | ctx->r2;
    // 0x8015FAAC: andi        $v0, $a1, 0xFF
    ctx->r2 = ctx->r5 & 0XFF;
    // 0x8015FAB0: or          $v1, $v1, $v0
    ctx->r3 = ctx->r3 | ctx->r2;
    // 0x8015FAB4: sw          $v1, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r3;
    // 0x8015FAB8: addu        $a0, $s3, $zero
    ctx->r4 = ADD32(ctx->r19, 0);
    // 0x8015FABC: jal         0x80167848
    // 0x8015FAC0: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015FAC0: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    after_0:
    // 0x8015FAC4: addiu       $s0, $s0, 0x2
    ctx->r16 = ADD32(ctx->r16, 0X2);
    // 0x8015FAC8: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
    // 0x8015FACC: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x8015FAD0: slt         $v0, $s4, $s1
    ctx->r2 = SIGNED(ctx->r20) < SIGNED(ctx->r17) ? 1 : 0;
    // 0x8015FAD4: beq         $v0, $zero, L_8015FA70
    if (ctx->r2 == 0) {
        // 0x8015FAD8: addiu       $s3, $s3, 0x4
        ctx->r19 = ADD32(ctx->r19, 0X4);
            goto L_8015FA70;
    }
    // 0x8015FAD8: addiu       $s3, $s3, 0x4
    ctx->r19 = ADD32(ctx->r19, 0X4);
    // 0x8015FADC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x8015FAE0: lw          $s4, 0x20($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X20);
    // 0x8015FAE4: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x8015FAE8: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x8015FAEC: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015FAF0: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015FAF4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8015FAF8: jr          $ra
    // 0x8015FAFC: nop

    return;
    // 0x8015FAFC: nop

;}
RECOMP_FUNC void func_8015FB80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015FB80: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8015FB84: sll         $a0, $a0, 14
    ctx->r4 = S32(ctx->r4 << 14);
    // 0x8015FB88: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x8015FB8C: sw          $a0, 0x10($gp)
    MEM_W(0X10, ctx->r28) = ctx->r4;
    // 0x8015FB90: lui         $a0, 0x800
    ctx->r4 = S32(0X800 << 16);
    // 0x8015FB94: ori         $a0, $a0, 0x110
    ctx->r4 = ctx->r4 | 0X110;
    // 0x8015FB98: jal         0x80167848
    // 0x8015FB9C: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    func_80167848(rdram, ctx);
        goto after_0;
    // 0x8015FB9C: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    after_0:
    // 0x8015FBA0: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x8015FBA4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8015FBA8: jr          $ra
    // 0x8015FBAC: nop

    return;
    // 0x8015FBAC: nop

;}
RECOMP_FUNC void func_8015FBB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015FBB0: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8015FBB4: sw          $s0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r16;
    // 0x8015FBB8: addu        $s0, $zero, $zero
    ctx->r16 = ADD32(0, 0);
    // 0x8015FBBC: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x8015FBC0: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x8015FBC4: addiu       $v1, $v1, -0x1C4
    ctx->r3 = ADD32(ctx->r3, -0X1C4);
    // 0x8015FBC8: lui         $v0, 0x8016
    ctx->r2 = S32(0X8016 << 16);
    // 0x8015FBCC: addiu       $v0, $v0, 0x7A8
    ctx->r2 = ADD32(ctx->r2, 0X7A8);
    // 0x8015FBD0: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x8015FBD4: sw          $v0, 0x6784($at)
    MEM_W(0X6784, ctx->r1) = ctx->r2;
    // 0x8015FBD8: addiu       $v0, $zero, 0x40
    ctx->r2 = ADD32(0, 0X40);
    // 0x8015FBDC: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x8015FBE0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015FBE4: sb          $v0, 0x3418($at)
    MEM_B(0X3418, ctx->r1) = ctx->r2;
L_8015FBE8:
    // 0x8015FBE8: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x8015FBEC: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8015FBF0: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x8015FBF4: blez        $a0, L_8015FBE8
    if (SIGNED(ctx->r4) <= 0) {
        // 0x8015FBF8: addu        $s0, $s0, $v0
        ctx->r16 = ADD32(ctx->r16, ctx->r2);
            goto L_8015FBE8;
    }
    // 0x8015FBF8: addu        $s0, $s0, $v0
    ctx->r16 = ADD32(ctx->r16, ctx->r2);
    // 0x8015FBFC: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x8015FC00: addiu       $a0, $a0, 0x600C
    ctx->r4 = ADD32(ctx->r4, 0X600C);
    // 0x8015FC04: jal         0x80143950
    // 0x8015FC08: nop

    func_80143950(rdram, ctx);
        goto after_0;
    // 0x8015FC08: nop

    after_0:
    // 0x8015FC0C: bne         $v0, $zero, L_8015FC3C
    if (ctx->r2 != 0) {
        // 0x8015FC10: nop
    
            goto L_8015FC3C;
    }
    // 0x8015FC10: nop

    // 0x8015FC14: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8015FC18: lw          $v0, 0x3450($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3450);
    // 0x8015FC1C: bne         $v0, $zero, L_8015FC3C
    if (ctx->r2 != 0) {
        // 0x8015FC20: nop
    
            goto L_8015FC3C;
    }
    // 0x8015FC20: nop

    // 0x8015FC24: bne         $s0, $zero, L_8015FC3C
    if (ctx->r16 != 0) {
        // 0x8015FC28: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_8015FC3C;
    }
    // 0x8015FC28: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8015FC2C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8015FC30: sw          $v0, 0x3450($at)
    MEM_W(0X3450, ctx->r1) = ctx->r2;
    // 0x8015FC34: jal         0x80161B10
    // 0x8015FC38: addiu       $a0, $sp, 0x10
    ctx->r4 = ADD32(ctx->r29, 0X10);
    static_0_80161B10(rdram, ctx);
        goto after_1;
    // 0x8015FC38: addiu       $a0, $sp, 0x10
    ctx->r4 = ADD32(ctx->r29, 0X10);
    after_1:
L_8015FC3C:
    // 0x8015FC3C: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x8015FC40: lw          $s0, 0x38($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X38);
    // 0x8015FC44: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8015FC48: jr          $ra
    // 0x8015FC4C: nop

    return;
    // 0x8015FC4C: nop

;}
RECOMP_FUNC void func_8015FC58(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015FC58: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8015FC5C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8015FC60: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015FC64: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015FC68: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FC6C: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015FC70: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FC74: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015FC78: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FC7C: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015FC80: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015FC84: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015FC88: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015FC8C: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015FC90: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015FC94: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015FC98: addu        $s0, $v1, $v0
    ctx->r16 = ADD32(ctx->r3, ctx->r2);
    // 0x8015FC9C: beq         $s0, $zero, L_8015FCBC
    if (ctx->r16 == 0) {
        // 0x8015FCA0: addiu       $v0, $zero, 0xFF
        ctx->r2 = ADD32(0, 0XFF);
            goto L_8015FCBC;
    }
    // 0x8015FCA0: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
    // 0x8015FCA4: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8015FCA8: beql        $a0, $zero, L_8015FCC0
    if (ctx->r4 == 0) {
        // 0x8015FCAC: sw          $v0, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r2;
            goto L_8015FCC0;
    }
    goto skip_0;
    // 0x8015FCAC: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
    skip_0:
    // 0x8015FCB0: jal         0x80157860
    // 0x8015FCB4: nop

    func_80157860(rdram, ctx);
        goto after_0;
    // 0x8015FCB4: nop

    after_0:
    // 0x8015FCB8: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
L_8015FCBC:
    // 0x8015FCBC: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
L_8015FCC0:
    // 0x8015FCC0: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;
    // 0x8015FCC4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8015FCC8: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015FCCC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8015FCD0: jr          $ra
    // 0x8015FCD4: nop

    return;
    // 0x8015FCD4: nop

;}
RECOMP_FUNC void func_8015FD70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015FD70: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8015FD74: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x8015FD78: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x8015FD7C: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x8015FD80: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x8015FD84: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015FD88: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FD8C: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015FD90: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FD94: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015FD98: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FD9C: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015FDA0: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015FDA4: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015FDA8: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015FDAC: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015FDB0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015FDB4: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015FDB8: beq         $a2, $zero, L_8015FDD4
    if (ctx->r6 == 0) {
        // 0x8015FDBC: addu        $s0, $v1, $v0
        ctx->r16 = ADD32(ctx->r3, ctx->r2);
            goto L_8015FDD4;
    }
    // 0x8015FDBC: addu        $s0, $v1, $v0
    ctx->r16 = ADD32(ctx->r3, ctx->r2);
    // 0x8015FDC0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8015FDC4: beq         $a2, $v0, L_8015FDF0
    if (ctx->r6 == ctx->r2) {
        // 0x8015FDC8: nop
    
            goto L_8015FDF0;
    }
    // 0x8015FDC8: nop

    // 0x8015FDCC: j           L_8015FE58
    // 0x8015FDD0: nop

        goto L_8015FE58;
    // 0x8015FDD0: nop

L_8015FDD4:
    // 0x8015FDD4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015FDD8: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015FDDC: sw          $s1, 0x1264($at)
    MEM_W(0X1264, ctx->r1) = ctx->r17;
    // 0x8015FDE0: jal         0x8015FE70
    // 0x8015FDE4: nop

    func_8015FE70(rdram, ctx);
        goto after_0;
    // 0x8015FDE4: nop

    after_0:
    // 0x8015FDE8: j           L_8015FE58
    // 0x8015FDEC: nop

        goto L_8015FE58;
    // 0x8015FDEC: nop

L_8015FDF0:
    // 0x8015FDF0: beq         $s1, $zero, L_8015FE4C
    if (ctx->r17 == 0) {
        // 0x8015FDF4: nop
    
            goto L_8015FE4C;
    }
    // 0x8015FDF4: nop

    // 0x8015FDF8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015FDFC: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015FE00: sw          $a2, 0x1268($at)
    MEM_W(0X1268, ctx->r1) = ctx->r6;
    // 0x8015FE04: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x8015FE08: addiu       $a0, $a0, 0x6020
    ctx->r4 = ADD32(ctx->r4, 0X6020);
    // 0x8015FE0C: jal         0x80143950
    // 0x8015FE10: nop

    func_80143950(rdram, ctx);
        goto after_1;
    // 0x8015FE10: nop

    after_1:
    // 0x8015FE14: beql        $v0, $zero, L_8015FE38
    if (ctx->r2 == 0) {
        // 0x8015FE18: andi        $v0, $s1, 0xFFFF
        ctx->r2 = ctx->r17 & 0XFFFF;
            goto L_8015FE38;
    }
    goto skip_0;
    // 0x8015FE18: andi        $v0, $s1, 0xFFFF
    ctx->r2 = ctx->r17 & 0XFFFF;
    skip_0:
    // 0x8015FE1C: jal         0x8014DE00
    // 0x8015FE20: addu        $a0, $v0, $zero
    ctx->r4 = ADD32(ctx->r2, 0);
    func_8014DE00(rdram, ctx);
        goto after_2;
    // 0x8015FE20: addu        $a0, $v0, $zero
    ctx->r4 = ADD32(ctx->r2, 0);
    after_2:
    // 0x8015FE24: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015FE28: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015FE2C: sw          $v0, 0x126C($at)
    MEM_W(0X126C, ctx->r1) = ctx->r2;
    // 0x8015FE30: j           L_8015FE58
    // 0x8015FE34: nop

        goto L_8015FE58;
    // 0x8015FE34: nop

L_8015FE38:
    // 0x8015FE38: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015FE3C: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015FE40: sw          $v0, 0x126C($at)
    MEM_W(0X126C, ctx->r1) = ctx->r2;
    // 0x8015FE44: j           L_8015FE58
    // 0x8015FE48: nop

        goto L_8015FE58;
    // 0x8015FE48: nop

L_8015FE4C:
    // 0x8015FE4C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8015FE50: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8015FE54: sw          $zero, 0x1268($at)
    MEM_W(0X1268, ctx->r1) = 0;
L_8015FE58:
    // 0x8015FE58: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8015FE5C: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x8015FE60: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x8015FE64: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8015FE68: jr          $ra
    // 0x8015FE6C: nop

    return;
    // 0x8015FE6C: nop

;}
RECOMP_FUNC void func_8015FE70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015FE70: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x8015FE74: lw          $v0, -0x5BBC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X5BBC);
    // 0x8015FE78: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015FE7C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x8015FE80: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x8015FE84: lw          $v0, 0x348($at)
    ctx->r2 = MEM_W(ctx->r1, 0X348);
    // 0x8015FE88: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x8015FE8C: sw          $v0, 0x5098($at)
    MEM_W(0X5098, ctx->r1) = ctx->r2;
    // 0x8015FE90: jr          $ra
    // 0x8015FE94: nop

    return;
    // 0x8015FE94: nop

;}
RECOMP_FUNC void func_8015FE98(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8015FE98: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8015FE9C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8015FEA0: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
    // 0x8015FEA4: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x8015FEA8: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x8015FEAC: sw          $ra, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r31;
    // 0x8015FEB0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8015FEB4: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8015FEB8: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FEBC: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8015FEC0: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FEC4: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8015FEC8: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8015FECC: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8015FED0: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x8015FED4: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015FED8: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x8015FEDC: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8015FEE0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015FEE4: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x8015FEE8: addu        $s0, $v1, $v0
    ctx->r16 = ADD32(ctx->r3, ctx->r2);
    // 0x8015FEEC: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FEF0: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x8015FEF4: sw          $v0, 0x104($v1)
    MEM_W(0X104, ctx->r3) = ctx->r2;
    // 0x8015FEF8: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FEFC: lw          $v0, 0x4($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X4);
    // 0x8015FF00: sw          $v0, 0x108($v1)
    MEM_W(0X108, ctx->r3) = ctx->r2;
    // 0x8015FF04: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF08: lw          $v0, 0x8($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X8);
    // 0x8015FF0C: sw          $v0, 0x10C($v1)
    MEM_W(0X10C, ctx->r3) = ctx->r2;
    // 0x8015FF10: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF14: lw          $v0, 0xC($s1)
    ctx->r2 = MEM_W(ctx->r17, 0XC);
    // 0x8015FF18: sw          $v0, 0x110($v1)
    MEM_W(0X110, ctx->r3) = ctx->r2;
    // 0x8015FF1C: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF20: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x8015FF24: sw          $v0, 0x114($v1)
    MEM_W(0X114, ctx->r3) = ctx->r2;
    // 0x8015FF28: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF2C: lw          $v0, 0x14($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X14);
    // 0x8015FF30: sw          $v0, 0x118($v1)
    MEM_W(0X118, ctx->r3) = ctx->r2;
    // 0x8015FF34: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF38: lw          $v0, 0x18($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X18);
    // 0x8015FF3C: sw          $v0, 0x11C($v1)
    MEM_W(0X11C, ctx->r3) = ctx->r2;
    // 0x8015FF40: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF44: lw          $v0, 0x1C($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X1C);
    // 0x8015FF48: sw          $v0, 0x12C($v1)
    MEM_W(0X12C, ctx->r3) = ctx->r2;
    // 0x8015FF4C: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF50: lw          $v0, 0x20($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X20);
    // 0x8015FF54: sw          $v0, 0x130($v1)
    MEM_W(0X130, ctx->r3) = ctx->r2;
    // 0x8015FF58: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF5C: lw          $v0, 0x24($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X24);
    // 0x8015FF60: sw          $v0, 0x134($v1)
    MEM_W(0X134, ctx->r3) = ctx->r2;
    // 0x8015FF64: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF68: lw          $v0, 0x28($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X28);
    // 0x8015FF6C: sw          $v0, 0x140($v1)
    MEM_W(0X140, ctx->r3) = ctx->r2;
    // 0x8015FF70: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF74: lw          $v0, 0x2C($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X2C);
    // 0x8015FF78: sw          $v0, 0x144($v1)
    MEM_W(0X144, ctx->r3) = ctx->r2;
    // 0x8015FF7C: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8015FF80: lw          $v0, 0x30($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X30);
    // 0x8015FF84: sw          $v0, 0x148($v1)
    MEM_W(0X148, ctx->r3) = ctx->r2;
    // 0x8015FF88: lw          $v0, 0xC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC);
    // 0x8015FF8C: blez        $v0, L_801600D0
    if (SIGNED(ctx->r2) <= 0) {
        // 0x8015FF90: addu        $a3, $zero, $zero
        ctx->r7 = ADD32(0, 0);
            goto L_801600D0;
    }
    // 0x8015FF90: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    // 0x8015FF94: addiu       $t0, $zero, 0x4
    ctx->r8 = ADD32(0, 0X4);
    // 0x8015FF98: addu        $a2, $s1, $zero
    ctx->r6 = ADD32(ctx->r17, 0);
L_8015FF9C:
    // 0x8015FF9C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015FFA0: lw          $v0, 0x5098($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5098);
    // 0x8015FFA4: lw          $a0, 0x34($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X34);
    // 0x8015FFA8: addiu       $a1, $a3, 0x9
    ctx->r5 = ADD32(ctx->r7, 0X9);
    // 0x8015FFAC: sw          $v0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r2;
    // 0x8015FFB0: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x8015FFB4: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x8015FFB8: lw          $v1, 0x5098($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X5098);
    // 0x8015FFBC: sllv        $a1, $t0, $a1
    ctx->r5 = S32(ctx->r8 << (ctx->r5 & 31));
    // 0x8015FFC0: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x8015FFC4: sw          $a0, 0x300($v0)
    MEM_W(0X300, ctx->r2) = ctx->r4;
    // 0x8015FFC8: sw          $v1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r3;
    // 0x8015FFCC: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015FFD0: lw          $v0, 0x5098($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5098);
    // 0x8015FFD4: lw          $a0, 0x38($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X38);
    // 0x8015FFD8: sw          $v0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r2;
    // 0x8015FFDC: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x8015FFE0: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x8015FFE4: lw          $v1, 0x5098($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X5098);
    // 0x8015FFE8: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x8015FFEC: sw          $a0, 0x304($v0)
    MEM_W(0X304, ctx->r2) = ctx->r4;
    // 0x8015FFF0: sw          $v1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r3;
    // 0x8015FFF4: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x8015FFF8: lw          $v0, 0x5098($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5098);
    // 0x8015FFFC: lw          $a0, 0x3C($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X3C);
    // 0x80160000: sw          $v0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r2;
    // 0x80160004: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80160008: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x8016000C: lw          $v1, 0x5098($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X5098);
    // 0x80160010: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80160014: sw          $a0, 0x308($v0)
    MEM_W(0X308, ctx->r2) = ctx->r4;
    // 0x80160018: sw          $v1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r3;
    // 0x8016001C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160020: lw          $v0, 0x5098($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5098);
    // 0x80160024: lw          $a0, 0x40($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X40);
    // 0x80160028: sw          $v0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r2;
    // 0x8016002C: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80160030: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x80160034: lw          $v1, 0x5098($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X5098);
    // 0x80160038: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x8016003C: sw          $a0, 0x30C($v0)
    MEM_W(0X30C, ctx->r2) = ctx->r4;
    // 0x80160040: sw          $v1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r3;
    // 0x80160044: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160048: lw          $v0, 0x5098($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5098);
    // 0x8016004C: lw          $a0, 0x44($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X44);
    // 0x80160050: sw          $v0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r2;
    // 0x80160054: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80160058: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x8016005C: lw          $v1, 0x5098($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X5098);
    // 0x80160060: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80160064: sw          $a0, 0x310($v0)
    MEM_W(0X310, ctx->r2) = ctx->r4;
    // 0x80160068: sw          $v1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r3;
    // 0x8016006C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160070: lw          $v0, 0x5098($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5098);
    // 0x80160074: lw          $a0, 0x48($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X48);
    // 0x80160078: sw          $v0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r2;
    // 0x8016007C: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80160080: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x80160084: lw          $v1, 0x5098($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X5098);
    // 0x80160088: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x8016008C: sw          $a0, 0x314($v0)
    MEM_W(0X314, ctx->r2) = ctx->r4;
    // 0x80160090: sw          $v1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r3;
    // 0x80160094: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160098: lw          $v0, 0x5098($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X5098);
    // 0x8016009C: lw          $a0, 0x4C($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X4C);
    // 0x801600A0: sw          $v0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r2;
    // 0x801600A4: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x801600A8: lui         $v1, 0x801E
    ctx->r3 = S32(0X801E << 16);
    // 0x801600AC: lw          $v1, 0x5098($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X5098);
    // 0x801600B0: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x801600B4: sw          $a0, 0x318($v0)
    MEM_W(0X318, ctx->r2) = ctx->r4;
    // 0x801600B8: sw          $v1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r3;
    // 0x801600BC: lw          $v0, 0xC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC);
    // 0x801600C0: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
    // 0x801600C4: slt         $v0, $a3, $v0
    ctx->r2 = SIGNED(ctx->r7) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x801600C8: bne         $v0, $zero, L_8015FF9C
    if (ctx->r2 != 0) {
        // 0x801600CC: addiu       $a2, $a2, 0x30
        ctx->r6 = ADD32(ctx->r6, 0X30);
            goto L_8015FF9C;
    }
    // 0x801600CC: addiu       $a2, $a2, 0x30
    ctx->r6 = ADD32(ctx->r6, 0X30);
L_801600D0:
    // 0x801600D0: jal         0x8015E008
    // 0x801600D4: nop

    func_8015E008(rdram, ctx);
        goto after_0;
    // 0x801600D4: nop

    after_0:
    // 0x801600D8: lui         $v0, 0x1
    ctx->r2 = S32(0X1 << 16);
    // 0x801600DC: ori         $v0, $v0, 0x1170
    ctx->r2 = ctx->r2 | 0X1170;
    // 0x801600E0: addu        $a3, $s0, $v0
    ctx->r7 = ADD32(ctx->r16, ctx->r2);
    // 0x801600E4: addu        $a2, $s1, $zero
    ctx->r6 = ADD32(ctx->r17, 0);
    // 0x801600E8: addiu       $t0, $a2, 0x100
    ctx->r8 = ADD32(ctx->r6, 0X100);
L_801600EC:
    // 0x801600EC: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x801600F0: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x801600F4: lw          $a0, 0x8($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X8);
    // 0x801600F8: lw          $a1, 0xC($a2)
    ctx->r5 = MEM_W(ctx->r6, 0XC);
    // 0x801600FC: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80160100: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
    // 0x80160104: sw          $a0, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r4;
    // 0x80160108: sw          $a1, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->r5;
    // 0x8016010C: addiu       $a2, $a2, 0x10
    ctx->r6 = ADD32(ctx->r6, 0X10);
    // 0x80160110: bne         $a2, $t0, L_801600EC
    if (ctx->r6 != ctx->r8) {
        // 0x80160114: addiu       $a3, $a3, 0x10
        ctx->r7 = ADD32(ctx->r7, 0X10);
            goto L_801600EC;
    }
    // 0x80160114: addiu       $a3, $a3, 0x10
    ctx->r7 = ADD32(ctx->r7, 0X10);
    // 0x80160118: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x8016011C: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80160120: lw          $ra, 0x20($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X20);
    // 0x80160124: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80160128: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016012C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80160130: jr          $ra
    // 0x80160134: nop

    return;
    // 0x80160134: nop

;}
RECOMP_FUNC void func_80160180(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160180: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80160184: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80160188: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016018C: addiu       $s0, $s0, 0x1DF8
    ctx->r16 = ADD32(ctx->r16, 0X1DF8);
    // 0x80160190: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80160194: addiu       $a1, $a1, 0x6030
    ctx->r5 = ADD32(ctx->r5, 0X6030);
    // 0x80160198: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8016019C: jal         0x80142DF8
    // 0x801601A0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80142DF8(rdram, ctx);
        goto after_0;
    // 0x801601A0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_0:
    // 0x801601A4: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x801601A8: addiu       $a1, $a1, 0x606C
    ctx->r5 = ADD32(ctx->r5, 0X606C);
    // 0x801601AC: jal         0x80142DF8
    // 0x801601B0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    static_0_80142DF8(rdram, ctx);
        goto after_1;
    // 0x801601B0: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_1:
    // 0x801601B4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801601B8: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x801601BC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801601C0: jr          $ra
    // 0x801601C4: nop

    return;
    // 0x801601C4: nop

;}
RECOMP_FUNC void func_801601C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801601C8: addiu       $sp, $sp, -0xD8
    ctx->r29 = ADD32(ctx->r29, -0XD8);
    // 0x801601CC: sw          $ra, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->r31;
    // 0x801601D0: addu        $t0, $a0, $zero
    ctx->r8 = ADD32(ctx->r4, 0);
    // 0x801601D4: addiu       $a3, $sp, 0x10
    ctx->r7 = ADD32(ctx->r29, 0X10);
    // 0x801601D8: addu        $a2, $t0, $zero
    ctx->r6 = ADD32(ctx->r8, 0);
    // 0x801601DC: addiu       $t1, $t0, 0x40
    ctx->r9 = ADD32(ctx->r8, 0X40);
L_801601E0:
    // 0x801601E0: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x801601E4: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x801601E8: lw          $a0, 0x8($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X8);
    // 0x801601EC: lw          $a1, 0xC($a2)
    ctx->r5 = MEM_W(ctx->r6, 0XC);
    // 0x801601F0: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x801601F4: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
    // 0x801601F8: sw          $a0, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r4;
    // 0x801601FC: sw          $a1, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->r5;
    // 0x80160200: addiu       $a2, $a2, 0x10
    ctx->r6 = ADD32(ctx->r6, 0X10);
    // 0x80160204: bne         $a2, $t1, L_801601E0
    if (ctx->r6 != ctx->r9) {
        // 0x80160208: addiu       $a3, $a3, 0x10
        ctx->r7 = ADD32(ctx->r7, 0X10);
            goto L_801601E0;
    }
    // 0x80160208: addiu       $a3, $a3, 0x10
    ctx->r7 = ADD32(ctx->r7, 0X10);
    // 0x8016020C: addiu       $a3, $sp, 0x50
    ctx->r7 = ADD32(ctx->r29, 0X50);
    // 0x80160210: addu        $a2, $t0, $zero
    ctx->r6 = ADD32(ctx->r8, 0);
    // 0x80160214: addiu       $t1, $t0, 0x40
    ctx->r9 = ADD32(ctx->r8, 0X40);
L_80160218:
    // 0x80160218: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x8016021C: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x80160220: lw          $a0, 0x8($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X8);
    // 0x80160224: lw          $a1, 0xC($a2)
    ctx->r5 = MEM_W(ctx->r6, 0XC);
    // 0x80160228: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x8016022C: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
    // 0x80160230: sw          $a0, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r4;
    // 0x80160234: sw          $a1, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->r5;
    // 0x80160238: addiu       $a2, $a2, 0x10
    ctx->r6 = ADD32(ctx->r6, 0X10);
    // 0x8016023C: bne         $a2, $t1, L_80160218
    if (ctx->r6 != ctx->r9) {
        // 0x80160240: addiu       $a3, $a3, 0x10
        ctx->r7 = ADD32(ctx->r7, 0X10);
            goto L_80160218;
    }
    // 0x80160240: addiu       $a3, $a3, 0x10
    ctx->r7 = ADD32(ctx->r7, 0X10);
    // 0x80160244: addiu       $a3, $sp, 0x90
    ctx->r7 = ADD32(ctx->r29, 0X90);
    // 0x80160248: addu        $a2, $t0, $zero
    ctx->r6 = ADD32(ctx->r8, 0);
    // 0x8016024C: addiu       $t1, $t0, 0x40
    ctx->r9 = ADD32(ctx->r8, 0X40);
L_80160250:
    // 0x80160250: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80160254: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x80160258: lw          $a0, 0x8($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X8);
    // 0x8016025C: lw          $a1, 0xC($a2)
    ctx->r5 = MEM_W(ctx->r6, 0XC);
    // 0x80160260: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80160264: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
    // 0x80160268: sw          $a0, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r4;
    // 0x8016026C: sw          $a1, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->r5;
    // 0x80160270: addiu       $a2, $a2, 0x10
    ctx->r6 = ADD32(ctx->r6, 0X10);
    // 0x80160274: bne         $a2, $t1, L_80160250
    if (ctx->r6 != ctx->r9) {
        // 0x80160278: addiu       $a3, $a3, 0x10
        ctx->r7 = ADD32(ctx->r7, 0X10);
            goto L_80160250;
    }
    // 0x80160278: addiu       $a3, $a3, 0x10
    ctx->r7 = ADD32(ctx->r7, 0X10);
    // 0x8016027C: lwc1        $f0, 0x0($t0)
    ctx->f0.u32l = MEM_W(ctx->r8, 0X0);
    // 0x80160280: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80160284: lwc1        $f4, 0x608C($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X608C);
    // 0x80160288: trunc.w.s   $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.u32l = TRUNC_W_S(ctx->f0.fl);
    // 0x8016028C: mfc1        $v0, $f2
    ctx->r2 = (int32_t)ctx->f2.u32l;
    // 0x80160290: lwc1        $f2, 0x4($t0)
    ctx->f2.u32l = MEM_W(ctx->r8, 0X4);
    // 0x80160294: mtc1        $v0, $f0
    ctx->f0.u32l = ctx->r2;
    // 0x80160298: cvt.s.w     $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    ctx->f0.fl = CVT_S_W(ctx->f0.u32l);
    // 0x8016029C: add.s       $f8, $f0, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f0.fl + ctx->f4.fl;
    // 0x801602A0: trunc.w.s   $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    ctx->f6.u32l = TRUNC_W_S(ctx->f2.fl);
    // 0x801602A4: mfc1        $v0, $f6
    ctx->r2 = (int32_t)ctx->f6.u32l;
    // 0x801602A8: mtc1        $v0, $f2
    ctx->f2.u32l = ctx->r2;
    // 0x801602AC: cvt.s.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.fl = CVT_S_W(ctx->f2.u32l);
    // 0x801602B0: add.s       $f4, $f2, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f4.fl = ctx->f2.fl + ctx->f4.fl;
    // 0x801602B4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801602B8: lwc1        $f6, 0x6090($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X6090);
    // 0x801602BC: add.s       $f0, $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f6.fl;
    // 0x801602C0: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x801602C4: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x801602C8: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x801602CC: addiu       $a1, $a1, 0x6030
    ctx->r5 = ADD32(ctx->r5, 0X6030);
    // 0x801602D0: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    // 0x801602D4: add.s       $f2, $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f2.fl + ctx->f6.fl;
    // 0x801602D8: swc1        $f4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f4.u32l;
    // 0x801602DC: swc1        $f4, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f4.u32l;
    // 0x801602E0: swc1        $f0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f0.u32l;
    // 0x801602E4: swc1        $f0, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->f0.u32l;
    // 0x801602E8: jal         0x80142DF8
    // 0x801602EC: swc1        $f2, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->f2.u32l;
    static_0_80142DF8(rdram, ctx);
        goto after_0;
    // 0x801602EC: swc1        $f2, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->f2.u32l;
    after_0:
    // 0x801602F0: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x801602F4: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x801602F8: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x801602FC: addiu       $a1, $a1, 0x606C
    ctx->r5 = ADD32(ctx->r5, 0X606C);
    // 0x80160300: jal         0x80142DF8
    // 0x80160304: nop

    static_0_80142DF8(rdram, ctx);
        goto after_1;
    // 0x80160304: nop

    after_1:
    // 0x80160308: lw          $ra, 0xD0($sp)
    ctx->r31 = MEM_W(ctx->r29, 0XD0);
    // 0x8016030C: addiu       $sp, $sp, 0xD8
    ctx->r29 = ADD32(ctx->r29, 0XD8);
    // 0x80160310: jr          $ra
    // 0x80160314: nop

    return;
    // 0x80160314: nop

;}
RECOMP_FUNC void func_80160318(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160318: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016031C: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80160320: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    // 0x80160324: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
    // 0x80160328: slt         $v0, $s0, $a0
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x8016032C: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x80160330: beq         $v0, $zero, L_80160378
    if (ctx->r2 == 0) {
        // 0x80160334: sw          $s1, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r17;
            goto L_80160378;
    }
    // 0x80160334: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x80160338: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_8016033C:
    // 0x8016033C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80160340: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x80160344: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80160348: addiu       $a1, $a1, 0x6030
    ctx->r5 = ADD32(ctx->r5, 0X6030);
    // 0x8016034C: jal         0x80142DF8
    // 0x80160350: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    static_0_80142DF8(rdram, ctx);
        goto after_0;
    // 0x80160350: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    after_0:
    // 0x80160354: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80160358: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x8016035C: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80160360: addiu       $a1, $a1, 0x606C
    ctx->r5 = ADD32(ctx->r5, 0X606C);
    // 0x80160364: jal         0x80142DF8
    // 0x80160368: nop

    static_0_80142DF8(rdram, ctx);
        goto after_1;
    // 0x80160368: nop

    after_1:
    // 0x8016036C: slt         $v0, $s0, $s1
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r17) ? 1 : 0;
    // 0x80160370: bne         $v0, $zero, L_8016033C
    if (ctx->r2 != 0) {
        // 0x80160374: nop
    
            goto L_8016033C;
    }
    // 0x80160374: nop

L_80160378:
    // 0x80160378: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x8016037C: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80160380: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80160384: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80160388: jr          $ra
    // 0x8016038C: nop

    return;
    // 0x8016038C: nop

;}
RECOMP_FUNC void func_80160390(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160390: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80160394: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80160398: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    // 0x8016039C: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
    // 0x801603A0: slt         $v0, $s0, $a0
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x801603A4: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x801603A8: beq         $v0, $zero, L_801603F0
    if (ctx->r2 == 0) {
        // 0x801603AC: sw          $s1, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r17;
            goto L_801603F0;
    }
    // 0x801603AC: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x801603B0: addu        $s1, $a0, $zero
    ctx->r17 = ADD32(ctx->r4, 0);
L_801603B4:
    // 0x801603B4: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x801603B8: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x801603BC: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x801603C0: addiu       $a1, $a1, 0x6030
    ctx->r5 = ADD32(ctx->r5, 0X6030);
    // 0x801603C4: jal         0x80142DF8
    // 0x801603C8: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    static_0_80142DF8(rdram, ctx);
        goto after_0;
    // 0x801603C8: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    after_0:
    // 0x801603CC: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x801603D0: addiu       $a0, $a0, 0x1DF8
    ctx->r4 = ADD32(ctx->r4, 0X1DF8);
    // 0x801603D4: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x801603D8: addiu       $a1, $a1, 0x606C
    ctx->r5 = ADD32(ctx->r5, 0X606C);
    // 0x801603DC: jal         0x80142DF8
    // 0x801603E0: nop

    static_0_80142DF8(rdram, ctx);
        goto after_1;
    // 0x801603E0: nop

    after_1:
    // 0x801603E4: slt         $v0, $s0, $s1
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r17) ? 1 : 0;
    // 0x801603E8: bne         $v0, $zero, L_801603B4
    if (ctx->r2 != 0) {
        // 0x801603EC: nop
    
            goto L_801603B4;
    }
    // 0x801603EC: nop

L_801603F0:
    // 0x801603F0: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x801603F4: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x801603F8: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x801603FC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80160400: jr          $ra
    // 0x80160404: nop

    return;
    // 0x80160404: nop

;}
RECOMP_FUNC void func_80160408(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160408: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8016040C: sw          $s3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r19;
    // 0x80160410: addu        $s3, $a1, $zero
    ctx->r19 = ADD32(ctx->r5, 0);
    // 0x80160414: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80160418: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    // 0x8016041C: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
    // 0x80160420: slt         $v0, $s0, $a0
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x80160424: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80160428: sw          $s4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r20;
    // 0x8016042C: sw          $s2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r18;
    // 0x80160430: beq         $v0, $zero, L_8016046C
    if (ctx->r2 == 0) {
        // 0x80160434: sw          $s1, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r17;
            goto L_8016046C;
    }
    // 0x80160434: sw          $s1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r17;
    // 0x80160438: addu        $s4, $a0, $zero
    ctx->r20 = ADD32(ctx->r4, 0);
    // 0x8016043C: addiu       $s2, $zero, 0x30
    ctx->r18 = ADD32(0, 0X30);
    // 0x80160440: addiu       $s1, $s3, 0x18
    ctx->r17 = ADD32(ctx->r19, 0X18);
L_80160444:
    // 0x80160444: addu        $a0, $s3, $zero
    ctx->r4 = ADD32(ctx->r19, 0);
    // 0x80160448: addu        $a1, $s1, $zero
    ctx->r5 = ADD32(ctx->r17, 0);
    // 0x8016044C: addu        $a2, $s3, $s2
    ctx->r6 = ADD32(ctx->r19, ctx->r18);
    // 0x80160450: jal         0x801555F0
    // 0x80160454: addiu       $a3, $zero, 0x4
    ctx->r7 = ADD32(0, 0X4);
    func_801555F0(rdram, ctx);
        goto after_0;
    // 0x80160454: addiu       $a3, $zero, 0x4
    ctx->r7 = ADD32(0, 0X4);
    after_0:
    // 0x80160458: addiu       $s2, $s2, 0x18
    ctx->r18 = ADD32(ctx->r18, 0X18);
    // 0x8016045C: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80160460: slt         $v0, $s0, $s4
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r20) ? 1 : 0;
    // 0x80160464: bne         $v0, $zero, L_80160444
    if (ctx->r2 != 0) {
        // 0x80160468: addiu       $s1, $s1, 0x18
        ctx->r17 = ADD32(ctx->r17, 0X18);
            goto L_80160444;
    }
    // 0x80160468: addiu       $s1, $s1, 0x18
    ctx->r17 = ADD32(ctx->r17, 0X18);
L_8016046C:
    // 0x8016046C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80160470: lw          $s4, 0x20($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X20);
    // 0x80160474: lw          $s3, 0x1C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X1C);
    // 0x80160478: lw          $s2, 0x18($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X18);
    // 0x8016047C: lw          $s1, 0x14($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X14);
    // 0x80160480: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80160484: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80160488: jr          $ra
    // 0x8016048C: nop

    return;
    // 0x8016048C: nop

;}
RECOMP_FUNC void func_80160490(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160490: addiu       $sp, $sp, -0x88
    ctx->r29 = ADD32(ctx->r29, -0X88);
    // 0x80160494: sw          $s1, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r17;
    // 0x80160498: addu        $s1, $a1, $zero
    ctx->r17 = ADD32(ctx->r5, 0);
    // 0x8016049C: lui         $a1, 0x8023
    ctx->r5 = S32(0X8023 << 16);
    // 0x801604A0: lw          $a1, -0x5BBC($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X5BBC);
    // 0x801604A4: sw          $s0, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r16;
    // 0x801604A8: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x801604AC: sw          $ra, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r31;
    // 0x801604B0: sw          $s3, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r19;
    // 0x801604B4: sw          $s2, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r18;
    // 0x801604B8: sll         $v1, $a1, 3
    ctx->r3 = S32(ctx->r5 << 3);
    // 0x801604BC: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x801604C0: sll         $v1, $v1, 3
    ctx->r3 = S32(ctx->r3 << 3);
    // 0x801604C4: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x801604C8: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x801604CC: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x801604D0: sll         $v0, $v1, 4
    ctx->r2 = S32(ctx->r3 << 4);
    // 0x801604D4: subu        $v0, $v0, $v1
    ctx->r2 = SUB32(ctx->r2, ctx->r3);
    // 0x801604D8: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x801604DC: subu        $v0, $v0, $a1
    ctx->r2 = SUB32(ctx->r2, ctx->r5);
    // 0x801604E0: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x801604E4: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x801604E8: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x801604EC: lw          $s3, 0x7C60($at)
    ctx->r19 = MEM_W(ctx->r1, 0X7C60);
    // 0x801604F0: jal         0x8015F23C
    // 0x801604F4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    func_8015F23C(rdram, ctx);
        goto after_0;
    // 0x801604F4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_0:
    // 0x801604F8: lwc1        $f2, 0x4($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X4);
    // 0x801604FC: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80160500: c.lt.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
    // 0x80160504: bc1f        L_80160514
    if (!c1cs) {
        // 0x80160508: addu        $v0, $s0, $zero
        ctx->r2 = ADD32(ctx->r16, 0);
            goto L_80160514;
    }
    // 0x80160508: addu        $v0, $s0, $zero
    ctx->r2 = ADD32(ctx->r16, 0);
    // 0x8016050C: addu        $s0, $s1, $zero
    ctx->r16 = ADD32(ctx->r17, 0);
    // 0x80160510: addu        $s1, $v0, $zero
    ctx->r17 = ADD32(ctx->r2, 0);
L_80160514:
    // 0x80160514: lwc1        $f2, 0x0($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X0);
    // 0x80160518: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x8016051C: sub.s       $f4, $f2, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x80160520: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80160524: lwc1        $f2, 0x4($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80160528: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x8016052C: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x80160530: bc1f        L_8016053C
    if (!c1cs) {
        // 0x80160534: sub.s       $f2, $f2, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
            goto L_8016053C;
    }
    // 0x80160534: sub.s       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x80160538: neg.s       $f4, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f4.fl = -ctx->f4.fl;
L_8016053C:
    // 0x8016053C: c.lt.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl < ctx->f6.fl;
    // 0x80160540: bc1tl       L_80160548
    if (c1cs) {
        // 0x80160544: neg.s       $f2, $f2
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
            goto L_80160548;
    }
    goto skip_0;
    // 0x80160544: neg.s       $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
    skip_0:
L_80160548:
    // 0x80160548: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x8016054C: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x80160550: lw          $a0, 0x8($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X8);
    // 0x80160554: lw          $a1, 0xC($s0)
    ctx->r5 = MEM_W(ctx->r16, 0XC);
    // 0x80160558: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    // 0x8016055C: sw          $v1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r3;
    // 0x80160560: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80160564: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80160568: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x8016056C: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x80160570: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x80160574: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    // 0x80160578: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x8016057C: lw          $v1, 0x4($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X4);
    // 0x80160580: lw          $a0, 0x8($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X8);
    // 0x80160584: lw          $a1, 0xC($s0)
    ctx->r5 = MEM_W(ctx->r16, 0XC);
    // 0x80160588: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    // 0x8016058C: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    // 0x80160590: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80160594: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x80160598: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x8016059C: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x801605A0: sw          $v0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r2;
    // 0x801605A4: sw          $v1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r3;
    // 0x801605A8: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x801605AC: lw          $v1, 0x4($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X4);
    // 0x801605B0: lw          $a0, 0x8($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X8);
    // 0x801605B4: lw          $a1, 0xC($s1)
    ctx->r5 = MEM_W(ctx->r17, 0XC);
    // 0x801605B8: sw          $v0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r2;
    // 0x801605BC: sw          $v1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r3;
    // 0x801605C0: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x801605C4: sw          $a1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r5;
    // 0x801605C8: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x801605CC: lw          $v1, 0x14($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X14);
    // 0x801605D0: sw          $v0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r2;
    // 0x801605D4: sw          $v1, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r3;
    // 0x801605D8: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x801605DC: lw          $v1, 0x4($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X4);
    // 0x801605E0: lw          $a0, 0x8($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X8);
    // 0x801605E4: lw          $a1, 0xC($s1)
    ctx->r5 = MEM_W(ctx->r17, 0XC);
    // 0x801605E8: sw          $v0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r2;
    // 0x801605EC: sw          $v1, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r3;
    // 0x801605F0: sw          $a0, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r4;
    // 0x801605F4: sw          $a1, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r5;
    // 0x801605F8: lw          $v0, 0x10($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X10);
    // 0x801605FC: lw          $v1, 0x14($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X14);
    // 0x80160600: sw          $v0, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r2;
    // 0x80160604: sw          $v1, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r3;
    // 0x80160608: addiu       $s1, $sp, 0x10
    ctx->r17 = ADD32(ctx->r29, 0X10);
    // 0x8016060C: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    // 0x80160610: c.le.s      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.fl <= ctx->f4.fl;
    // 0x80160614: addiu       $s2, $sp, 0x40
    ctx->r18 = ADD32(ctx->r29, 0X40);
    // 0x80160618: bc1f        L_80160668
    if (!c1cs) {
        // 0x8016061C: addiu       $s0, $sp, 0x58
        ctx->r16 = ADD32(ctx->r29, 0X58);
            goto L_80160668;
    }
    // 0x8016061C: addiu       $s0, $sp, 0x58
    ctx->r16 = ADD32(ctx->r29, 0X58);
    // 0x80160620: lwc1        $f8, 0x14($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X14);
    // 0x80160624: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80160628: lwc1        $f2, 0x6094($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6094);
    // 0x8016062C: sub.s       $f8, $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x80160630: lwc1        $f6, 0x2C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80160634: add.s       $f6, $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f2.fl;
    // 0x80160638: lwc1        $f4, 0x44($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016063C: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    // 0x80160640: sub.s       $f4, $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x80160644: lwc1        $f0, 0x5C($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80160648: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    // 0x8016064C: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    // 0x80160650: add.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x80160654: swc1        $f8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f8.u32l;
    // 0x80160658: swc1        $f6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f6.u32l;
    // 0x8016065C: swc1        $f4, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f4.u32l;
    // 0x80160660: j           L_801606AC
    // 0x80160664: swc1        $f0, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f0.u32l;
        goto L_801606AC;
    // 0x80160664: swc1        $f0, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f0.u32l;
L_80160668:
    // 0x80160668: lwc1        $f8, 0x10($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X10);
    // 0x8016066C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80160670: lwc1        $f2, 0x6098($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6098);
    // 0x80160674: sub.s       $f8, $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x80160678: lwc1        $f6, 0x28($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016067C: add.s       $f6, $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f2.fl;
    // 0x80160680: lwc1        $f4, 0x40($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80160684: addu        $a0, $s1, $zero
    ctx->r4 = ADD32(ctx->r17, 0);
    // 0x80160688: sub.s       $f4, $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x8016068C: lwc1        $f0, 0x58($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80160690: addu        $a2, $s0, $zero
    ctx->r6 = ADD32(ctx->r16, 0);
    // 0x80160694: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    // 0x80160698: add.s       $f0, $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x8016069C: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    // 0x801606A0: swc1        $f6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f6.u32l;
    // 0x801606A4: swc1        $f4, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f4.u32l;
    // 0x801606A8: swc1        $f0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f0.u32l;
L_801606AC:
    // 0x801606AC: jal         0x801555F0
    // 0x801606B0: nop

    func_801555F0(rdram, ctx);
        goto after_1;
    // 0x801606B0: nop

    after_1:
    // 0x801606B4: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    // 0x801606B8: addu        $a1, $s2, $zero
    ctx->r5 = ADD32(ctx->r18, 0);
    // 0x801606BC: addu        $a2, $s1, $zero
    ctx->r6 = ADD32(ctx->r17, 0);
    // 0x801606C0: jal         0x801555F0
    // 0x801606C4: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    func_801555F0(rdram, ctx);
        goto after_2;
    // 0x801606C4: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    after_2:
    // 0x801606C8: jal         0x8015F23C
    // 0x801606CC: addu        $a0, $s3, $zero
    ctx->r4 = ADD32(ctx->r19, 0);
    func_8015F23C(rdram, ctx);
        goto after_3;
    // 0x801606CC: addu        $a0, $s3, $zero
    ctx->r4 = ADD32(ctx->r19, 0);
    after_3:
    // 0x801606D0: lw          $ra, 0x80($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X80);
    // 0x801606D4: lw          $s3, 0x7C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X7C);
    // 0x801606D8: lw          $s2, 0x78($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X78);
    // 0x801606DC: lw          $s1, 0x74($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X74);
    // 0x801606E0: lw          $s0, 0x70($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X70);
    // 0x801606E4: addiu       $sp, $sp, 0x88
    ctx->r29 = ADD32(ctx->r29, 0X88);
    // 0x801606E8: jr          $ra
    // 0x801606EC: nop

    return;
    // 0x801606EC: nop

;}
RECOMP_FUNC void func_801606F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801606F0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801606F4: sw          $ra, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r31;
    // 0x801606F8: jal         0x80167778
    // 0x801606FC: nop

    func_80167778(rdram, ctx);
        goto after_0;
    // 0x801606FC: nop

    after_0:
    // 0x80160700: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x80160704: addiu       $a1, $a1, 0x6540
    ctx->r5 = ADD32(ctx->r5, 0X6540);
    // 0x80160708: lw          $v1, 0x0($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X0);
    // 0x8016070C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160710: sw          $v0, 0x420($at)
    MEM_W(0X420, ctx->r1) = ctx->r2;
    // 0x80160714: blez        $v1, L_80160784
    if (SIGNED(ctx->r3) <= 0) {
        // 0x80160718: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_80160784;
    }
    // 0x80160718: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x8016071C: lui         $t1, 0x60
    ctx->r9 = S32(0X60 << 16);
    // 0x80160720: lui         $t0, 0x2
    ctx->r8 = S32(0X2 << 16);
    // 0x80160724: addu        $v1, $a1, $zero
    ctx->r3 = ADD32(ctx->r5, 0);
    // 0x80160728: lui         $a3, 0x801E
    ctx->r7 = S32(0X801E << 16);
    // 0x8016072C: addiu       $a3, $a3, 0x348
    ctx->r7 = ADD32(ctx->r7, 0X348);
    // 0x80160730: lui         $a2, 0x801E
    ctx->r6 = S32(0X801E << 16);
    // 0x80160734: addiu       $a2, $a2, 0x6A20
    ctx->r6 = ADD32(ctx->r6, 0X6A20);
    // 0x80160738: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
L_8016073C:
    // 0x8016073C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160740: addu        $at, $at, $a1
    ctx->r1 = ADD32(ctx->r1, ctx->r5);
    // 0x80160744: lw          $v0, 0x6544($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6544);
    // 0x80160748: bnel        $v0, $zero, L_80160764
    if (ctx->r2 != 0) {
        // 0x8016074C: addiu       $a3, $a3, 0x4
        ctx->r7 = ADD32(ctx->r7, 0X4);
            goto L_80160764;
    }
    goto skip_0;
    // 0x8016074C: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    skip_0:
    // 0x80160750: lw          $v0, 0x18($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X18);
    // 0x80160754: addu        $v0, $v0, $t1
    ctx->r2 = ADD32(ctx->r2, ctx->r9);
    // 0x80160758: addu        $v0, $v0, $t0
    ctx->r2 = ADD32(ctx->r2, ctx->r8);
    // 0x8016075C: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80160760: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
L_80160764:
    // 0x80160764: lui         $v0, 0x1
    ctx->r2 = S32(0X1 << 16);
    // 0x80160768: ori         $v0, $v0, 0x12AC
    ctx->r2 = ctx->r2 | 0X12AC;
    // 0x8016076C: addu        $a2, $a2, $v0
    ctx->r6 = ADD32(ctx->r6, ctx->r2);
    // 0x80160770: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80160774: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x80160778: slt         $v0, $a0, $v0
    ctx->r2 = SIGNED(ctx->r4) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x8016077C: bne         $v0, $zero, L_8016073C
    if (ctx->r2 != 0) {
        // 0x80160780: addiu       $a1, $a1, 0x24
        ctx->r5 = ADD32(ctx->r5, 0X24);
            goto L_8016073C;
    }
    // 0x80160780: addiu       $a1, $a1, 0x24
    ctx->r5 = ADD32(ctx->r5, 0X24);
L_80160784:
    // 0x80160784: lw          $ra, 0x18($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X18);
    // 0x80160788: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8016078C: jr          $ra
    // 0x80160790: nop

    return;
    // 0x80160790: nop

;}
RECOMP_FUNC void func_801607A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801607A8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801607AC: beq         $a1, $zero, L_801607CC
    if (ctx->r5 == 0) {
        // 0x801607B0: sw          $ra, 0x10($sp)
        MEM_W(0X10, ctx->r29) = ctx->r31;
            goto L_801607CC;
    }
    // 0x801607B0: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x801607B4: jal         0x801607E8
    // 0x801607B8: nop

    func_801607E8(rdram, ctx);
        goto after_0;
    // 0x801607B8: nop

    after_0:
    // 0x801607BC: jal         0x8015FC50
    // 0x801607C0: nop

    static_0_8015FC50(rdram, ctx);
        goto after_1;
    // 0x801607C0: nop

    after_1:
    // 0x801607C4: jal         0x801438D0
    // 0x801607C8: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    func_801438D0(rdram, ctx);
        goto after_2;
    // 0x801607C8: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_2:
L_801607CC:
    // 0x801607CC: jal         0x801607E8
    // 0x801607D0: nop

    func_801607E8(rdram, ctx);
        goto after_3;
    // 0x801607D0: nop

    after_3:
    // 0x801607D4: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x801607D8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801607DC: jr          $ra
    // 0x801607E0: nop

    return;
    // 0x801607E0: nop

;}
RECOMP_FUNC void func_801607E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801607E8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801607EC: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x801607F0: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x801607F4: lbu         $a0, 0x0($s0)
    ctx->r4 = MEM_BU(ctx->r16, 0X0);
    // 0x801607F8: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x801607FC: beq         $a0, $zero, L_80160818
    if (ctx->r4 == 0) {
        // 0x80160800: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_80160818;
    }
    // 0x80160800: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
L_80160804:
    // 0x80160804: jal         0x80160838
    // 0x80160808: nop

    func_80160838(rdram, ctx);
        goto after_0;
    // 0x80160808: nop

    after_0:
    // 0x8016080C: lbu         $a0, 0x0($s0)
    ctx->r4 = MEM_BU(ctx->r16, 0X0);
    // 0x80160810: bne         $a0, $zero, L_80160804
    if (ctx->r4 != 0) {
        // 0x80160814: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80160804;
    }
    // 0x80160814: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80160818:
    // 0x80160818: jal         0x80160838
    // 0x8016081C: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
    func_80160838(rdram, ctx);
        goto after_1;
    // 0x8016081C: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
    after_1:
    // 0x80160820: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80160824: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80160828: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8016082C: jr          $ra
    // 0x80160830: nop

    return;
    // 0x80160830: nop

;}
RECOMP_FUNC void func_80160838(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160838: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8016083C: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80160840: addiu       $a1, $a1, 0x1DD8
    ctx->r5 = ADD32(ctx->r5, 0X1DD8);
    // 0x80160844: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80160848: lw          $v0, 0x0($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X0);
    // 0x8016084C: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80160850: bgez        $v0, L_80160868
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80160854: sw          $v0, 0x0($a1)
        MEM_W(0X0, ctx->r5) = ctx->r2;
            goto L_80160868;
    }
    // 0x80160854: sw          $v0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r2;
    // 0x80160858: jal         0x80142A68
    // 0x8016085C: nop

    func_80142A68(rdram, ctx);
        goto after_0;
    // 0x8016085C: nop

    after_0:
    // 0x80160860: j           L_80160884
    // 0x80160864: nop

        goto L_80160884;
    // 0x80160864: nop

L_80160868:
    // 0x80160868: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016086C: lw          $a1, 0x1DDC($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X1DDC);
    // 0x80160870: andi        $v0, $a0, 0xFF
    ctx->r2 = ctx->r4 & 0XFF;
    // 0x80160874: addiu       $v1, $a1, 0x1
    ctx->r3 = ADD32(ctx->r5, 0X1);
    // 0x80160878: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016087C: sw          $v1, 0x1DDC($at)
    MEM_W(0X1DDC, ctx->r1) = ctx->r3;
    // 0x80160880: sb          $a0, 0x0($a1)
    MEM_B(0X0, ctx->r5) = ctx->r4;
L_80160884:
    // 0x80160884: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80160888: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8016088C: jr          $ra
    // 0x80160890: nop

    return;
    // 0x80160890: nop

;}
RECOMP_FUNC void func_80160898(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160898: lui         $a1, 0x8023
    ctx->r5 = S32(0X8023 << 16);
    // 0x8016089C: lw          $a1, -0x5BBC($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X5BBC);
    // 0x801608A0: sll         $v0, $a1, 3
    ctx->r2 = S32(ctx->r5 << 3);
    // 0x801608A4: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x801608A8: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x801608AC: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x801608B0: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x801608B4: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x801608B8: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x801608BC: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x801608C0: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x801608C4: subu        $v1, $v1, $a1
    ctx->r3 = SUB32(ctx->r3, ctx->r5);
    // 0x801608C8: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x801608CC: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x801608D0: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x801608D4: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x801608D8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801608DC: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x801608E0: lw          $a1, 0x1180($at)
    ctx->r5 = MEM_W(ctx->r1, 0X1180);
    // 0x801608E4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801608E8: bnel        $a0, $v0, L_801608F8
    if (ctx->r4 != ctx->r2) {
        // 0x801608EC: ori         $a1, $a1, 0x100
        ctx->r5 = ctx->r5 | 0X100;
            goto L_801608F8;
    }
    goto skip_0;
    // 0x801608EC: ori         $a1, $a1, 0x100
    ctx->r5 = ctx->r5 | 0X100;
    skip_0:
    // 0x801608F0: addiu       $v0, $zero, -0x101
    ctx->r2 = ADD32(0, -0X101);
    // 0x801608F4: and         $a1, $a1, $v0
    ctx->r5 = ctx->r5 & ctx->r2;
L_801608F8:
    // 0x801608F8: lw          $v0, 0x14($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X14);
    // 0x801608FC: sw          $a1, 0x114($v0)
    MEM_W(0X114, ctx->r2) = ctx->r5;
    // 0x80160900: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160904: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160908: sw          $a1, 0x1180($at)
    MEM_W(0X1180, ctx->r1) = ctx->r5;
    // 0x8016090C: jr          $ra
    // 0x80160910: nop

    return;
    // 0x80160910: nop

;}
RECOMP_FUNC void func_80160914(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160914: addu        $a1, $a0, $zero
    ctx->r5 = ADD32(ctx->r4, 0);
    // 0x80160918: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x8016091C: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x80160920: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x80160924: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160928: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x8016092C: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160930: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160934: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160938: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x8016093C: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80160940: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160944: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x80160948: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8016094C: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160950: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x80160954: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x80160958: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016095C: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160960: lw          $a0, 0x1180($at)
    ctx->r4 = MEM_W(ctx->r1, 0X1180);
    // 0x80160964: addiu       $v0, $zero, -0xC1
    ctx->r2 = ADD32(0, -0XC1);
    // 0x80160968: beq         $a1, $zero, L_8016098C
    if (ctx->r5 == 0) {
        // 0x8016096C: and         $a0, $a0, $v0
        ctx->r4 = ctx->r4 & ctx->r2;
            goto L_8016098C;
    }
    // 0x8016096C: and         $a0, $a0, $v0
    ctx->r4 = ctx->r4 & ctx->r2;
    // 0x80160970: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80160974: bne         $a1, $v0, L_80160984
    if (ctx->r5 != ctx->r2) {
        // 0x80160978: andi        $v0, $a1, 0x2
        ctx->r2 = ctx->r5 & 0X2;
            goto L_80160984;
    }
    // 0x80160978: andi        $v0, $a1, 0x2
    ctx->r2 = ctx->r5 & 0X2;
    // 0x8016097C: j           L_8016098C
    // 0x80160980: ori         $a0, $a0, 0x40
    ctx->r4 = ctx->r4 | 0X40;
        goto L_8016098C;
    // 0x80160980: ori         $a0, $a0, 0x40
    ctx->r4 = ctx->r4 | 0X40;
L_80160984:
    // 0x80160984: bnel        $v0, $zero, L_8016098C
    if (ctx->r2 != 0) {
        // 0x80160988: ori         $a0, $a0, 0x80
        ctx->r4 = ctx->r4 | 0X80;
            goto L_8016098C;
    }
    goto skip_0;
    // 0x80160988: ori         $a0, $a0, 0x80
    ctx->r4 = ctx->r4 | 0X80;
    skip_0:
L_8016098C:
    // 0x8016098C: lw          $v0, 0x14($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X14);
    // 0x80160990: sw          $a0, 0x114($v0)
    MEM_W(0X114, ctx->r2) = ctx->r4;
    // 0x80160994: lw          $v0, 0x1C($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X1C);
    // 0x80160998: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016099C: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x801609A0: sw          $a0, 0x1180($at)
    MEM_W(0X1180, ctx->r1) = ctx->r4;
    // 0x801609A4: jr          $ra
    // 0x801609A8: nop

    return;
    // 0x801609A8: nop

;}
RECOMP_FUNC void func_801609AC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801609AC: addu        $a1, $a0, $zero
    ctx->r5 = ADD32(ctx->r4, 0);
    // 0x801609B0: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x801609B4: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x801609B8: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x801609BC: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x801609C0: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x801609C4: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x801609C8: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x801609CC: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x801609D0: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x801609D4: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x801609D8: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x801609DC: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x801609E0: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x801609E4: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x801609E8: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x801609EC: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x801609F0: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801609F4: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x801609F8: lw          $a0, 0x1180($at)
    ctx->r4 = MEM_W(ctx->r1, 0X1180);
    // 0x801609FC: addiu       $v0, $zero, -0x31
    ctx->r2 = ADD32(0, -0X31);
    // 0x80160A00: beq         $a1, $zero, L_80160A24
    if (ctx->r5 == 0) {
        // 0x80160A04: and         $a0, $a0, $v0
        ctx->r4 = ctx->r4 & ctx->r2;
            goto L_80160A24;
    }
    // 0x80160A04: and         $a0, $a0, $v0
    ctx->r4 = ctx->r4 & ctx->r2;
    // 0x80160A08: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80160A0C: bne         $a1, $v0, L_80160A1C
    if (ctx->r5 != ctx->r2) {
        // 0x80160A10: andi        $v0, $a1, 0x2
        ctx->r2 = ctx->r5 & 0X2;
            goto L_80160A1C;
    }
    // 0x80160A10: andi        $v0, $a1, 0x2
    ctx->r2 = ctx->r5 & 0X2;
    // 0x80160A14: j           L_80160A24
    // 0x80160A18: ori         $a0, $a0, 0x10
    ctx->r4 = ctx->r4 | 0X10;
        goto L_80160A24;
    // 0x80160A18: ori         $a0, $a0, 0x10
    ctx->r4 = ctx->r4 | 0X10;
L_80160A1C:
    // 0x80160A1C: bnel        $v0, $zero, L_80160A24
    if (ctx->r2 != 0) {
        // 0x80160A20: ori         $a0, $a0, 0x20
        ctx->r4 = ctx->r4 | 0X20;
            goto L_80160A24;
    }
    goto skip_0;
    // 0x80160A20: ori         $a0, $a0, 0x20
    ctx->r4 = ctx->r4 | 0X20;
    skip_0:
L_80160A24:
    // 0x80160A24: lw          $v0, 0x14($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X14);
    // 0x80160A28: sw          $a0, 0x114($v0)
    MEM_W(0X114, ctx->r2) = ctx->r4;
    // 0x80160A2C: lw          $v0, 0x1C($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X1C);
    // 0x80160A30: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160A34: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160A38: sw          $a0, 0x1180($at)
    MEM_W(0X1180, ctx->r1) = ctx->r4;
    // 0x80160A3C: jr          $ra
    // 0x80160A40: nop

    return;
    // 0x80160A40: nop

;}
RECOMP_FUNC void func_80160A44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160A44: lui         $a1, 0x8023
    ctx->r5 = S32(0X8023 << 16);
    // 0x80160A48: lw          $a1, -0x5BBC($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X5BBC);
    // 0x80160A4C: sll         $v0, $a1, 3
    ctx->r2 = S32(ctx->r5 << 3);
    // 0x80160A50: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80160A54: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x80160A58: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80160A5C: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160A60: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80160A64: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x80160A68: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80160A6C: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160A70: subu        $v1, $v1, $a1
    ctx->r3 = SUB32(ctx->r3, ctx->r5);
    // 0x80160A74: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160A78: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160A7C: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x80160A80: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x80160A84: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160A88: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160A8C: lw          $a1, 0x1180($at)
    ctx->r5 = MEM_W(ctx->r1, 0X1180);
    // 0x80160A90: bnel        $a0, $zero, L_80160AA0
    if (ctx->r4 != 0) {
        // 0x80160A94: ori         $a1, $a1, 0x2000
        ctx->r5 = ctx->r5 | 0X2000;
            goto L_80160AA0;
    }
    goto skip_0;
    // 0x80160A94: ori         $a1, $a1, 0x2000
    ctx->r5 = ctx->r5 | 0X2000;
    skip_0:
    // 0x80160A98: addiu       $v0, $zero, -0x2001
    ctx->r2 = ADD32(0, -0X2001);
    // 0x80160A9C: and         $a1, $a1, $v0
    ctx->r5 = ctx->r5 & ctx->r2;
L_80160AA0:
    // 0x80160AA0: lw          $v0, 0x14($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X14);
    // 0x80160AA4: sw          $a1, 0x114($v0)
    MEM_W(0X114, ctx->r2) = ctx->r5;
    // 0x80160AA8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160AAC: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160AB0: sw          $a1, 0x1180($at)
    MEM_W(0X1180, ctx->r1) = ctx->r5;
    // 0x80160AB4: jr          $ra
    // 0x80160AB8: nop

    return;
    // 0x80160AB8: nop

;}
RECOMP_FUNC void func_80160ABC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160ABC: lui         $a1, 0x8023
    ctx->r5 = S32(0X8023 << 16);
    // 0x80160AC0: lw          $a1, -0x5BBC($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X5BBC);
    // 0x80160AC4: sll         $v0, $a1, 3
    ctx->r2 = S32(ctx->r5 << 3);
    // 0x80160AC8: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80160ACC: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x80160AD0: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80160AD4: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160AD8: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80160ADC: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x80160AE0: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80160AE4: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160AE8: subu        $v1, $v1, $a1
    ctx->r3 = SUB32(ctx->r3, ctx->r5);
    // 0x80160AEC: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160AF0: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160AF4: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x80160AF8: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x80160AFC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160B00: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160B04: lw          $v0, 0x1180($at)
    ctx->r2 = MEM_W(ctx->r1, 0X1180);
    // 0x80160B08: addiu       $a1, $zero, -0x10
    ctx->r5 = ADD32(0, -0X10);
    // 0x80160B0C: lw          $a2, 0x14($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X14);
    // 0x80160B10: and         $v0, $v0, $a1
    ctx->r2 = ctx->r2 & ctx->r5;
    // 0x80160B14: or          $v0, $v0, $a0
    ctx->r2 = ctx->r2 | ctx->r4;
    // 0x80160B18: sw          $v0, 0x114($a2)
    MEM_W(0X114, ctx->r6) = ctx->r2;
    // 0x80160B1C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160B20: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160B24: sw          $v0, 0x1180($at)
    MEM_W(0X1180, ctx->r1) = ctx->r2;
    // 0x80160B28: jr          $ra
    // 0x80160B2C: nop

    return;
    // 0x80160B2C: nop

;}
RECOMP_FUNC void func_80160B30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160B30: lui         $a1, 0x8023
    ctx->r5 = S32(0X8023 << 16);
    // 0x80160B34: lw          $a1, -0x5BBC($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X5BBC);
    // 0x80160B38: sll         $v1, $a1, 3
    ctx->r3 = S32(ctx->r5 << 3);
    // 0x80160B3C: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x80160B40: sll         $v1, $v1, 3
    ctx->r3 = S32(ctx->r3 << 3);
    // 0x80160B44: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x80160B48: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160B4C: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x80160B50: sll         $v0, $v1, 4
    ctx->r2 = S32(ctx->r3 << 4);
    // 0x80160B54: subu        $v0, $v0, $v1
    ctx->r2 = SUB32(ctx->r2, ctx->r3);
    // 0x80160B58: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160B5C: subu        $v0, $v0, $a1
    ctx->r2 = SUB32(ctx->r2, ctx->r5);
    // 0x80160B60: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160B64: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x80160B68: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80160B6C: sb          $a0, 0x7C36($at)
    MEM_B(0X7C36, ctx->r1) = ctx->r4;
    // 0x80160B70: jr          $ra
    // 0x80160B74: nop

    return;
    // 0x80160B74: nop

;}
RECOMP_FUNC void func_80160B78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160B78: lui         $a1, 0x8023
    ctx->r5 = S32(0X8023 << 16);
    // 0x80160B7C: lw          $a1, -0x5BBC($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X5BBC);
    // 0x80160B80: sll         $v1, $a1, 3
    ctx->r3 = S32(ctx->r5 << 3);
    // 0x80160B84: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x80160B88: sll         $v1, $v1, 3
    ctx->r3 = S32(ctx->r3 << 3);
    // 0x80160B8C: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x80160B90: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160B94: addu        $v1, $v1, $a1
    ctx->r3 = ADD32(ctx->r3, ctx->r5);
    // 0x80160B98: sll         $v0, $v1, 4
    ctx->r2 = S32(ctx->r3 << 4);
    // 0x80160B9C: subu        $v0, $v0, $v1
    ctx->r2 = SUB32(ctx->r2, ctx->r3);
    // 0x80160BA0: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160BA4: subu        $v0, $v0, $a1
    ctx->r2 = SUB32(ctx->r2, ctx->r5);
    // 0x80160BA8: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160BAC: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x80160BB0: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80160BB4: sh          $a0, 0x7C34($at)
    MEM_H(0X7C34, ctx->r1) = ctx->r4;
    // 0x80160BB8: jr          $ra
    // 0x80160BBC: nop

    return;
    // 0x80160BBC: nop

;}
RECOMP_FUNC void func_80160BC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160BC0: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x80160BC4: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x80160BC8: lui         $a3, 0xFFFE
    ctx->r7 = S32(0XFFFE << 16);
    // 0x80160BCC: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x80160BD0: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160BD4: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x80160BD8: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160BDC: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160BE0: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160BE4: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x80160BE8: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80160BEC: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160BF0: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x80160BF4: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160BF8: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160BFC: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x80160C00: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x80160C04: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160C08: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160C0C: lw          $v0, 0x1190($at)
    ctx->r2 = MEM_W(ctx->r1, 0X1190);
    // 0x80160C10: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160C14: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160C18: lw          $a2, 0x117C($at)
    ctx->r6 = MEM_W(ctx->r1, 0X117C);
    // 0x80160C1C: lui         $a0, 0xFFFF
    ctx->r4 = S32(0XFFFF << 16);
    // 0x80160C20: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160C24: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160C28: lhu         $a1, 0x1214($at)
    ctx->r5 = MEM_HU(ctx->r1, 0X1214);
    // 0x80160C2C: and         $v0, $v0, $a0
    ctx->r2 = ctx->r2 & ctx->r4;
    // 0x80160C30: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160C34: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160C38: lbu         $a0, 0x1216($at)
    ctx->r4 = MEM_BU(ctx->r1, 0X1216);
    // 0x80160C3C: or          $v0, $v0, $a1
    ctx->r2 = ctx->r2 | ctx->r5;
    // 0x80160C40: lw          $a1, 0x14($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X14);
    // 0x80160C44: sll         $a0, $a0, 24
    ctx->r4 = S32(ctx->r4 << 24);
    // 0x80160C48: or          $v0, $v0, $a0
    ctx->r2 = ctx->r2 | ctx->r4;
    // 0x80160C4C: sw          $v0, 0x130($a1)
    MEM_W(0X130, ctx->r5) = ctx->r2;
    // 0x80160C50: lw          $v0, 0x14($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X14);
    // 0x80160C54: ori         $a3, $a3, 0xFFFF
    ctx->r7 = ctx->r7 | 0XFFFF;
    // 0x80160C58: and         $a2, $a2, $a3
    ctx->r6 = ctx->r6 & ctx->r7;
    // 0x80160C5C: jr          $ra
    // 0x80160C60: sw          $a2, 0x110($v0)
    MEM_W(0X110, ctx->r2) = ctx->r6;
    return;
    // 0x80160C60: sw          $a2, 0x110($v0)
    MEM_W(0X110, ctx->r2) = ctx->r6;
;}
RECOMP_FUNC void func_80160C64(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160C64: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x80160C68: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x80160C6C: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x80160C70: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160C74: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x80160C78: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160C7C: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80160C80: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80160C84: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x80160C88: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80160C8C: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160C90: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x80160C94: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80160C98: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160C9C: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x80160CA0: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x80160CA4: lw          $a0, 0x14($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X14);
    // 0x80160CA8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160CAC: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160CB0: lw          $v0, 0x1190($at)
    ctx->r2 = MEM_W(ctx->r1, 0X1190);
    // 0x80160CB4: sw          $v0, 0x130($a0)
    MEM_W(0X130, ctx->r4) = ctx->r2;
    // 0x80160CB8: lw          $a0, 0x14($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X14);
    // 0x80160CBC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80160CC0: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80160CC4: lw          $v0, 0x117C($at)
    ctx->r2 = MEM_W(ctx->r1, 0X117C);
    // 0x80160CC8: jr          $ra
    // 0x80160CCC: sw          $v0, 0x110($a0)
    MEM_W(0X110, ctx->r4) = ctx->r2;
    return;
    // 0x80160CCC: sw          $v0, 0x110($a0)
    MEM_W(0X110, ctx->r4) = ctx->r2;
;}
RECOMP_FUNC void func_80160DB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80160DB0: addiu       $sp, $sp, -0xF8
    ctx->r29 = ADD32(ctx->r29, -0XF8);
    // 0x80160DB4: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160DB8: addiu       $v0, $v0, 0x6540
    ctx->r2 = ADD32(ctx->r2, 0X6540);
    // 0x80160DBC: sw          $s7, 0xEC($sp)
    MEM_W(0XEC, ctx->r29) = ctx->r23;
    // 0x80160DC0: addu        $s7, $zero, $zero
    ctx->r23 = ADD32(0, 0);
    // 0x80160DC4: addiu       $a3, $sp, 0x10
    ctx->r7 = ADD32(ctx->r29, 0X10);
    // 0x80160DC8: sw          $s1, 0xD4($sp)
    MEM_W(0XD4, ctx->r29) = ctx->r17;
    // 0x80160DCC: addu        $s1, $zero, $zero
    ctx->r17 = ADD32(0, 0);
    // 0x80160DD0: sw          $ra, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = ctx->r31;
    // 0x80160DD4: sw          $fp, 0xF0($sp)
    MEM_W(0XF0, ctx->r29) = ctx->r30;
    // 0x80160DD8: sw          $s6, 0xE8($sp)
    MEM_W(0XE8, ctx->r29) = ctx->r22;
    // 0x80160DDC: sw          $s5, 0xE4($sp)
    MEM_W(0XE4, ctx->r29) = ctx->r21;
    // 0x80160DE0: sw          $s4, 0xE0($sp)
    MEM_W(0XE0, ctx->r29) = ctx->r20;
    // 0x80160DE4: sw          $s3, 0xDC($sp)
    MEM_W(0XDC, ctx->r29) = ctx->r19;
    // 0x80160DE8: sw          $s2, 0xD8($sp)
    MEM_W(0XD8, ctx->r29) = ctx->r18;
    // 0x80160DEC: sw          $s0, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->r16;
    // 0x80160DF0: sw          $zero, 0x0($v0)
    MEM_W(0X0, ctx->r2) = 0;
    // 0x80160DF4: sw          $a3, 0xB0($sp)
    MEM_W(0XB0, ctx->r29) = ctx->r7;
    // 0x80160DF8: lui         $a3, 0x801F
    ctx->r7 = S32(0X801F << 16);
    // 0x80160DFC: addiu       $a3, $a3, 0x7C94
    ctx->r7 = ADD32(ctx->r7, 0X7C94);
    // 0x80160E00: addu        $s6, $zero, $zero
    ctx->r22 = ADD32(0, 0);
    // 0x80160E04: sw          $a3, 0xC0($sp)
    MEM_W(0XC0, ctx->r29) = ctx->r7;
    // 0x80160E08: lui         $a3, 0xFFFE
    ctx->r7 = S32(0XFFFE << 16);
    // 0x80160E0C: ori         $a3, $a3, 0xED54
    ctx->r7 = ctx->r7 | 0XED54;
    // 0x80160E10: sw          $zero, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = 0;
    // 0x80160E14: sw          $a3, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->r7;
L_80160E18:
    // 0x80160E18: jal         0x80156B70
    // 0x80160E1C: addu        $a0, $s7, $zero
    ctx->r4 = ADD32(ctx->r23, 0);
    static_0_80156B70(rdram, ctx);
        goto after_0;
    // 0x80160E1C: addu        $a0, $s7, $zero
    ctx->r4 = ADD32(ctx->r23, 0);
    after_0:
    // 0x80160E20: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E24: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160E28: sw          $v0, 0x6A34($at)
    MEM_W(0X6A34, ctx->r1) = ctx->r2;
    // 0x80160E2C: beq         $v0, $zero, L_80161098
    if (ctx->r2 == 0) {
        // 0x80160E30: addiu       $v0, $zero, 0xFF
        ctx->r2 = ADD32(0, 0XFF);
            goto L_80161098;
    }
    // 0x80160E30: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
    // 0x80160E34: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E38: addu        $at, $at, $s6
    ctx->r1 = ADD32(ctx->r1, ctx->r22);
    // 0x80160E3C: sw          $zero, 0x6544($at)
    MEM_W(0X6544, ctx->r1) = 0;
    // 0x80160E40: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E44: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160E48: lw          $a0, 0x6A34($at)
    ctx->r4 = MEM_W(ctx->r1, 0X6A34);
    // 0x80160E4C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E50: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160E54: lw          $v1, 0x6A34($at)
    ctx->r3 = MEM_W(ctx->r1, 0X6A34);
    // 0x80160E58: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E5C: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160E60: lw          $a1, 0x6A34($at)
    ctx->r5 = MEM_W(ctx->r1, 0X6A34);
    // 0x80160E64: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E68: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160E6C: sw          $v0, 0x6A20($at)
    MEM_W(0X6A20, ctx->r1) = ctx->r2;
    // 0x80160E70: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E74: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160E78: sw          $v0, 0x6A24($at)
    MEM_W(0X6A24, ctx->r1) = ctx->r2;
    // 0x80160E7C: lui         $v0, 0x40
    ctx->r2 = S32(0X40 << 16);
    // 0x80160E80: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E84: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160E88: sw          $zero, 0x6A40($at)
    MEM_W(0X6A40, ctx->r1) = 0;
    // 0x80160E8C: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x80160E90: lui         $v0, 0x80
    ctx->r2 = S32(0X80 << 16);
    // 0x80160E94: addu        $a1, $a1, $v0
    ctx->r5 = ADD32(ctx->r5, ctx->r2);
    // 0x80160E98: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160E9C: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160EA0: sw          $v1, 0x6A3C($at)
    MEM_W(0X6A3C, ctx->r1) = ctx->r3;
    // 0x80160EA4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160EA8: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160EAC: sw          $a1, 0x6A38($at)
    MEM_W(0X6A38, ctx->r1) = ctx->r5;
    // 0x80160EB0: jal         0x80156FAC
    // 0x80160EB4: nop

    func_80156FAC(rdram, ctx);
        goto after_1;
    // 0x80160EB4: nop

    after_1:
    // 0x80160EB8: beq         $v0, $zero, L_8016110C
    if (ctx->r2 == 0) {
        // 0x80160EBC: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_8016110C;
    }
    // 0x80160EBC: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x80160EC0: lw          $a3, 0xA8($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XA8);
    // 0x80160EC4: andi        $v0, $a3, 0x1
    ctx->r2 = ctx->r7 & 0X1;
    // 0x80160EC8: beq         $v0, $zero, L_80160F08
    if (ctx->r2 == 0) {
        // 0x80160ECC: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_80160F08;
    }
    // 0x80160ECC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80160ED0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80160ED4: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160ED8: lw          $v1, 0x5784($at)
    ctx->r3 = MEM_W(ctx->r1, 0X5784);
    // 0x80160EDC: bne         $v1, $v0, L_80160F08
    if (ctx->r3 != ctx->r2) {
        // 0x80160EE0: nop
    
            goto L_80160F08;
    }
    // 0x80160EE0: nop

    // 0x80160EE4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160EE8: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160EEC: lw          $v0, 0x6A34($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6A34);
    // 0x80160EF0: lw          $a3, 0xC8($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC8);
    // 0x80160EF4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160EF8: addu        $at, $at, $a3
    ctx->r1 = ADD32(ctx->r1, ctx->r7);
    // 0x80160EFC: sw          $v0, 0x6A40($at)
    MEM_W(0X6A40, ctx->r1) = ctx->r2;
    // 0x80160F00: j           L_80161090
    // 0x80160F04: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
        goto L_80161090;
    // 0x80160F04: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_80160F08:
    // 0x80160F08: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80160F0C: lw          $v0, 0x6540($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6540);
    // 0x80160F10: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80160F14: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F18: sw          $v0, 0x6540($at)
    MEM_W(0X6540, ctx->r1) = ctx->r2;
    // 0x80160F1C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F20: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160F24: lw          $a0, 0x6A34($at)
    ctx->r4 = MEM_W(ctx->r1, 0X6A34);
    // 0x80160F28: jal         0x8015BF2C
    // 0x80160F2C: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    func_8015BF2C(rdram, ctx);
        goto after_2;
    // 0x80160F2C: addiu       $a1, $sp, 0x10
    ctx->r5 = ADD32(ctx->r29, 0X10);
    after_2:
    // 0x80160F30: beq         $v0, $zero, L_8016110C
    if (ctx->r2 == 0) {
        // 0x80160F34: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_8016110C;
    }
    // 0x80160F34: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x80160F38: lw          $v0, 0x44($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X44);
    // 0x80160F3C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F40: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160F44: sw          $v0, 0x6A2C($at)
    MEM_W(0X6A2C, ctx->r1) = ctx->r2;
    // 0x80160F48: lw          $v0, 0x44($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X44);
    // 0x80160F4C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F50: addu        $at, $at, $s6
    ctx->r1 = ADD32(ctx->r1, ctx->r22);
    // 0x80160F54: sw          $v0, 0x6550($at)
    MEM_W(0X6550, ctx->r1) = ctx->r2;
    // 0x80160F58: lw          $v0, 0x24($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X24);
    // 0x80160F5C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F60: addu        $at, $at, $s6
    ctx->r1 = ADD32(ctx->r1, ctx->r22);
    // 0x80160F64: sw          $v0, 0x654C($at)
    MEM_W(0X654C, ctx->r1) = ctx->r2;
    // 0x80160F68: lw          $v0, 0x4C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X4C);
    // 0x80160F6C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F70: addu        $at, $at, $s6
    ctx->r1 = ADD32(ctx->r1, ctx->r22);
    // 0x80160F74: sw          $v0, 0x6548($at)
    MEM_W(0X6548, ctx->r1) = ctx->r2;
    // 0x80160F78: lw          $v0, 0x3C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X3C);
    // 0x80160F7C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F80: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160F84: sw          $v0, 0x6A30($at)
    MEM_W(0X6A30, ctx->r1) = ctx->r2;
    // 0x80160F88: lw          $v0, 0x3C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X3C);
    // 0x80160F8C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F90: addu        $at, $at, $s6
    ctx->r1 = ADD32(ctx->r1, ctx->r22);
    // 0x80160F94: sw          $v0, 0x6554($at)
    MEM_W(0X6554, ctx->r1) = ctx->r2;
    // 0x80160F98: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160F9C: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80160FA0: lw          $v0, 0x6A2C($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6A2C);
    // 0x80160FA4: blez        $v0, L_80161048
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80160FA8: addu        $s4, $zero, $zero
        ctx->r20 = ADD32(0, 0);
            goto L_80161048;
    }
    // 0x80160FA8: addu        $s4, $zero, $zero
    ctx->r20 = ADD32(0, 0);
    // 0x80160FAC: addu        $fp, $s1, $zero
    ctx->r30 = ADD32(ctx->r17, 0);
    // 0x80160FB0: addu        $s0, $s1, $zero
    ctx->r16 = ADD32(ctx->r17, 0);
    // 0x80160FB4: lw          $s5, 0xC0($sp)
    ctx->r21 = MEM_W(ctx->r29, 0XC0);
    // 0x80160FB8: lw          $s3, 0xB0($sp)
    ctx->r19 = MEM_W(ctx->r29, 0XB0);
    // 0x80160FBC: addu        $s2, $s6, $zero
    ctx->r18 = ADD32(ctx->r22, 0);
L_80160FC0:
    // 0x80160FC0: lw          $v0, 0x40($s3)
    ctx->r2 = MEM_W(ctx->r19, 0X40);
    // 0x80160FC4: addu        $a0, $s5, $zero
    ctx->r4 = ADD32(ctx->r21, 0);
    // 0x80160FC8: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80160FCC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160FD0: addu        $at, $at, $s2
    ctx->r1 = ADD32(ctx->r1, ctx->r18);
    // 0x80160FD4: sw          $v0, 0x655C($at)
    MEM_W(0X655C, ctx->r1) = ctx->r2;
    // 0x80160FD8: lw          $v0, 0x40($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X40);
    // 0x80160FDC: addiu       $s5, $s5, 0x1C
    ctx->r21 = ADD32(ctx->r21, 0X1C);
    // 0x80160FE0: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80160FE4: addu        $at, $at, $s2
    ctx->r1 = ADD32(ctx->r1, ctx->r18);
    // 0x80160FE8: sw          $v0, 0x6558($at)
    MEM_W(0X6558, ctx->r1) = ctx->r2;
    // 0x80160FEC: jal         0x80141190
    // 0x80160FF0: addiu       $a2, $zero, 0x1C
    ctx->r6 = ADD32(0, 0X1C);
    func_80141190(rdram, ctx);
        goto after_3;
    // 0x80160FF0: addiu       $a2, $zero, 0x1C
    ctx->r6 = ADD32(0, 0X1C);
    after_3:
    // 0x80160FF4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80160FF8: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x80160FFC: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x80161000: sw          $v0, 0x7CA0($at)
    MEM_W(0X7CA0, ctx->r1) = ctx->r2;
    // 0x80161004: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x80161008: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x8016100C: sw          $v0, 0x7CA4($at)
    MEM_W(0X7CA4, ctx->r1) = ctx->r2;
    // 0x80161010: lw          $v0, 0x40($s3)
    ctx->r2 = MEM_W(ctx->r19, 0X40);
    // 0x80161014: addiu       $s4, $s4, 0x1
    ctx->r20 = ADD32(ctx->r20, 0X1);
    // 0x80161018: addiu       $s3, $s3, 0x4
    ctx->r19 = ADD32(ctx->r19, 0X4);
    // 0x8016101C: sll         $v0, $v0, 20
    ctx->r2 = S32(ctx->r2 << 20);
    // 0x80161020: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x80161024: addu        $at, $at, $s0
    ctx->r1 = ADD32(ctx->r1, ctx->r16);
    // 0x80161028: sw          $v0, 0x7C98($at)
    MEM_W(0X7C98, ctx->r1) = ctx->r2;
    // 0x8016102C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80161030: addu        $at, $at, $fp
    ctx->r1 = ADD32(ctx->r1, ctx->r30);
    // 0x80161034: lw          $v0, 0x6A2C($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6A2C);
    // 0x80161038: addiu       $s2, $s2, 0x8
    ctx->r18 = ADD32(ctx->r18, 0X8);
    // 0x8016103C: slt         $v0, $s4, $v0
    ctx->r2 = SIGNED(ctx->r20) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80161040: bne         $v0, $zero, L_80160FC0
    if (ctx->r2 != 0) {
        // 0x80161044: addiu       $s0, $s0, 0x1C
        ctx->r16 = ADD32(ctx->r16, 0X1C);
            goto L_80160FC0;
    }
    // 0x80161044: addiu       $s0, $s0, 0x1C
    ctx->r16 = ADD32(ctx->r16, 0X1C);
L_80161048:
    // 0x80161048: lw          $a3, 0xC0($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC0);
    // 0x8016104C: lui         $v0, 0x1
    ctx->r2 = S32(0X1 << 16);
    // 0x80161050: ori         $v0, $v0, 0x12AC
    ctx->r2 = ctx->r2 | 0X12AC;
    // 0x80161054: addu        $a3, $a3, $v0
    ctx->r7 = ADD32(ctx->r7, ctx->r2);
    // 0x80161058: sw          $a3, 0xC0($sp)
    MEM_W(0XC0, ctx->r29) = ctx->r7;
    // 0x8016105C: lw          $a3, 0xC8($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XC8);
    // 0x80161060: addu        $a3, $a3, $v0
    ctx->r7 = ADD32(ctx->r7, ctx->r2);
    // 0x80161064: sw          $a3, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->r7;
    // 0x80161068: lw          $a3, 0xA8($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XA8);
    // 0x8016106C: lw          $v1, 0x4C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X4C);
    // 0x80161070: addiu       $s6, $s6, 0x24
    ctx->r22 = ADD32(ctx->r22, 0X24);
    // 0x80161074: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
    // 0x80161078: sw          $a3, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = ctx->r7;
    // 0x8016107C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80161080: addu        $at, $at, $s1
    ctx->r1 = ADD32(ctx->r1, ctx->r17);
    // 0x80161084: sw          $v1, 0x6A28($at)
    MEM_W(0X6A28, ctx->r1) = ctx->r3;
    // 0x80161088: addu        $s1, $s1, $v0
    ctx->r17 = ADD32(ctx->r17, ctx->r2);
    // 0x8016108C: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_80161090:
    // 0x80161090: blez        $s7, L_80160E18
    if (SIGNED(ctx->r23) <= 0) {
        // 0x80161094: nop
    
            goto L_80160E18;
    }
    // 0x80161094: nop

L_80161098:
    // 0x80161098: bgtz        $s7, L_80161100
    if (SIGNED(ctx->r23) > 0) {
        // 0x8016109C: nop
    
            goto L_80161100;
    }
    // 0x8016109C: nop

    // 0x801610A0: addu        $v1, $zero, $zero
    ctx->r3 = ADD32(0, 0);
    // 0x801610A4: lui         $a1, 0x801E
    ctx->r5 = S32(0X801E << 16);
    // 0x801610A8: addiu       $a1, $a1, 0x6A20
    ctx->r5 = ADD32(ctx->r5, 0X6A20);
    // 0x801610AC: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
L_801610B0:
    // 0x801610B0: lw          $v0, 0x3C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X3C);
    // 0x801610B4: xori        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 ^ 0X1;
    // 0x801610B8: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x801610BC: beq         $v0, $zero, L_801610F4
    if (ctx->r2 == 0) {
        // 0x801610C0: sw          $v0, 0x10($a1)
        MEM_W(0X10, ctx->r5) = ctx->r2;
            goto L_801610F4;
    }
    // 0x801610C0: sw          $v0, 0x10($a1)
    MEM_W(0X10, ctx->r5) = ctx->r2;
    // 0x801610C4: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x801610C8: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x801610CC: lw          $v0, 0x6A40($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6A40);
    // 0x801610D0: bnel        $v0, $zero, L_801610F8
    if (ctx->r2 != 0) {
        // 0x801610D4: addiu       $s7, $s7, 0x1
        ctx->r23 = ADD32(ctx->r23, 0X1);
            goto L_801610F8;
    }
    goto skip_0;
    // 0x801610D4: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
    skip_0:
    // 0x801610D8: sw          $zero, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = 0;
    // 0x801610DC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x801610E0: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x801610E4: sw          $zero, 0x6A30($at)
    MEM_W(0X6A30, ctx->r1) = 0;
    // 0x801610E8: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x801610EC: addu        $at, $at, $a0
    ctx->r1 = ADD32(ctx->r1, ctx->r4);
    // 0x801610F0: sw          $zero, 0x6554($at)
    MEM_W(0X6554, ctx->r1) = 0;
L_801610F4:
    // 0x801610F4: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_801610F8:
    // 0x801610F8: blez        $s7, L_801610B0
    if (SIGNED(ctx->r23) <= 0) {
        // 0x801610FC: nop
    
            goto L_801610B0;
    }
    // 0x801610FC: nop

L_80161100:
    // 0x80161100: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80161104: lw          $v0, 0x6540($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6540);
    // 0x80161108: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_8016110C:
    // 0x8016110C: lw          $ra, 0xF4($sp)
    ctx->r31 = MEM_W(ctx->r29, 0XF4);
    // 0x80161110: lw          $fp, 0xF0($sp)
    ctx->r30 = MEM_W(ctx->r29, 0XF0);
    // 0x80161114: lw          $s7, 0xEC($sp)
    ctx->r23 = MEM_W(ctx->r29, 0XEC);
    // 0x80161118: lw          $s6, 0xE8($sp)
    ctx->r22 = MEM_W(ctx->r29, 0XE8);
    // 0x8016111C: lw          $s5, 0xE4($sp)
    ctx->r21 = MEM_W(ctx->r29, 0XE4);
    // 0x80161120: lw          $s4, 0xE0($sp)
    ctx->r20 = MEM_W(ctx->r29, 0XE0);
    // 0x80161124: lw          $s3, 0xDC($sp)
    ctx->r19 = MEM_W(ctx->r29, 0XDC);
    // 0x80161128: lw          $s2, 0xD8($sp)
    ctx->r18 = MEM_W(ctx->r29, 0XD8);
    // 0x8016112C: lw          $s1, 0xD4($sp)
    ctx->r17 = MEM_W(ctx->r29, 0XD4);
    // 0x80161130: lw          $s0, 0xD0($sp)
    ctx->r16 = MEM_W(ctx->r29, 0XD0);
    // 0x80161134: addiu       $sp, $sp, 0xF8
    ctx->r29 = ADD32(ctx->r29, 0XF8);
    // 0x80161138: jr          $ra
    // 0x8016113C: nop

    return;
    // 0x8016113C: nop

;}
RECOMP_FUNC void func_80161140_real(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80161140: addiu       $sp, $sp, -0xE8
    ctx->r29 = ADD32(ctx->r29, -0XE8);
    // 0x80161144: sw          $s2, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->r18;
    // 0x80161148: addu        $s2, $a0, $zero
    ctx->r18 = ADD32(ctx->r4, 0);
    // 0x8016114C: sw          $s3, 0xCC($sp)
    MEM_W(0XCC, ctx->r29) = ctx->r19;
    // 0x80161150: addu        $s3, $a1, $zero
    ctx->r19 = ADD32(ctx->r5, 0);
    // 0x80161154: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x80161158: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x8016115C: lw          $a1, 0xFC($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XFC);
    // 0x80161160: sw          $ra, 0xD8($sp)
    MEM_W(0XD8, ctx->r29) = ctx->r31;
    // 0x80161164: sw          $s5, 0xD4($sp)
    MEM_W(0XD4, ctx->r29) = ctx->r21;
    // 0x80161168: sw          $s4, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->r20;
    // 0x8016116C: sw          $s1, 0xC4($sp)
    MEM_W(0XC4, ctx->r29) = ctx->r17;
    // 0x80161170: sw          $s0, 0xC0($sp)
    MEM_W(0XC0, ctx->r29) = ctx->r16;
    // 0x80161174: sdc1        $f20, 0xE0($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0XE0, ctx->r29);
    // 0x80161178: sll         $v0, $a0, 3
    ctx->r2 = S32(ctx->r4 << 3);
    // 0x8016117C: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80161180: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x80161184: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80161188: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8016118C: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x80161190: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x80161194: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80161198: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8016119C: subu        $v1, $v1, $a0
    ctx->r3 = SUB32(ctx->r3, ctx->r4);
    // 0x801611A0: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x801611A4: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x801611A8: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x801611AC: addu        $s0, $v1, $v0
    ctx->r16 = ADD32(ctx->r3, ctx->r2);
    // 0x801611B0: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
    // 0x801611B4: addu        $s4, $a2, $zero
    ctx->r20 = ADD32(ctx->r6, 0);
    // 0x801611B8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801611BC: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801611C0: sw          $s4, 0x1238($at)
    MEM_W(0X1238, ctx->r1) = ctx->r20;
    // 0x801611C4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801611C8: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801611CC: sw          $a1, 0x123C($at)
    MEM_W(0X123C, ctx->r1) = ctx->r5;
    // 0x801611D0: slti        $v0, $v0, 0x3
    ctx->r2 = SIGNED(ctx->r2) < 0X3 ? 1 : 0;
    // 0x801611D4: beq         $v0, $zero, L_8016122C
    if (ctx->r2 == 0) {
        // 0x801611D8: addu        $s5, $a3, $zero
        ctx->r21 = ADD32(ctx->r7, 0);
            goto L_8016122C;
    }
    // 0x801611D8: addu        $s5, $a3, $zero
    ctx->r21 = ADD32(ctx->r7, 0);
    // 0x801611DC: addiu       $v0, $zero, 0x8
    ctx->r2 = ADD32(0, 0X8);
    // 0x801611E0: beq         $s2, $v0, L_801611F0
    if (ctx->r18 == ctx->r2) {
        // 0x801611E4: addiu       $v0, $zero, 0xA
        ctx->r2 = ADD32(0, 0XA);
            goto L_801611F0;
    }
    // 0x801611E4: addiu       $v0, $zero, 0xA
    ctx->r2 = ADD32(0, 0XA);
    // 0x801611E8: bne         $s2, $v0, L_801611FC
    if (ctx->r18 != ctx->r2) {
        // 0x801611EC: nop
    
            goto L_801611FC;
    }
    // 0x801611EC: nop

L_801611F0:
    // 0x801611F0: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x801611F4: beq         $a1, $v0, L_80161214
    if (ctx->r5 == ctx->r2) {
        // 0x801611F8: nop
    
            goto L_80161214;
    }
    // 0x801611F8: nop

L_801611FC:
    // 0x801611FC: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
    // 0x80161200: slti        $v0, $v0, 0x3
    ctx->r2 = SIGNED(ctx->r2) < 0X3 ? 1 : 0;
    // 0x80161204: beq         $v0, $zero, L_8016122C
    if (ctx->r2 == 0) {
        // 0x80161208: addiu       $v0, $zero, 0x9
        ctx->r2 = ADD32(0, 0X9);
            goto L_8016122C;
    }
    // 0x80161208: addiu       $v0, $zero, 0x9
    ctx->r2 = ADD32(0, 0X9);
    // 0x8016120C: bne         $s2, $v0, L_8016122C
    if (ctx->r18 != ctx->r2) {
        // 0x80161210: nop
    
            goto L_8016122C;
    }
    // 0x80161210: nop

L_80161214:
    // 0x80161214: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x80161218: lw          $v0, 0x6784($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6784);
    // 0x8016121C: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80161220: addiu       $a0, $a0, 0x60A0
    ctx->r4 = ADD32(ctx->r4, 0X60A0);
    // 0x80161224: jalr        $v0
    // 0x80161228: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_0;
    // 0x80161228: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_0:
L_8016122C:
    // 0x8016122C: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80161230: addiu       $a0, $a0, 0x60D8
    ctx->r4 = ADD32(ctx->r4, 0X60D8);
    // 0x80161234: jal         0x80143950
    // 0x80161238: nop

    func_80143950(rdram, ctx);
        goto after_1;
    // 0x80161238: nop

    after_1:
    // 0x8016123C: beq         $v0, $zero, L_80161254
    if (ctx->r2 == 0) {
        // 0x80161240: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_80161254;
    }
    // 0x80161240: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80161244: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x80161248: sw          $v0, 0x267C($at)
    MEM_W(0X267C, ctx->r1) = ctx->r2;
    // 0x8016124C: j           L_80161260
    // 0x80161250: sltiu       $v0, $s2, 0xD
    ctx->r2 = ctx->r18 < 0XD ? 1 : 0;
        goto L_80161260;
    // 0x80161250: sltiu       $v0, $s2, 0xD
    ctx->r2 = ctx->r18 < 0XD ? 1 : 0;
L_80161254:
    // 0x80161254: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x80161258: sw          $zero, 0x267C($at)
    MEM_W(0X267C, ctx->r1) = 0;
    // 0x8016125C: sltiu       $v0, $s2, 0xD
    ctx->r2 = ctx->r18 < 0XD ? 1 : 0;
L_80161260:
    // 0x80161260: beq         $v0, $zero, L_8016139C
    if (ctx->r2 == 0) {
        // 0x80161264: sll         $v0, $s2, 2
        ctx->r2 = S32(ctx->r18 << 2);
            goto L_8016139C;
    }
    // 0x80161264: sll         $v0, $s2, 2
    ctx->r2 = S32(ctx->r18 << 2);
    // 0x80161268: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8016126C: addu        $at, $at, $v0
    gpr jr_addend_80161274 = ctx->r2;
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80161270: lw          $v0, 0x6210($at)
    ctx->r2 = ADD32(ctx->r1, 0X6210);
    // 0x80161274: jr          $v0
    // 0x80161278: nop

    switch (jr_addend_80161274 >> 2) {
        case 0: goto L_8016127C; break;
        case 1: goto L_80161294; break;
        case 2: goto L_801612AC; break;
        case 3: goto L_801612DC; break;
        case 4: goto L_8016139C; break;
        case 5: goto L_8016139C; break;
        case 6: goto L_8016130C; break;
        case 7: goto L_80161324; break;
        case 8: goto L_8016133C; break;
        case 9: goto L_8016136C; break;
        case 10: goto L_80161354; break;
        case 11: goto L_801612C4; break;
        case 12: goto L_801612F4; break;
        default: switch_error(__func__, 0x80161274, 0x80176210);
    }
    // 0x80161278: nop

L_8016127C:
    // 0x8016127C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161280: lwc1        $f0, 0x6244($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X6244);
    // 0x80161284: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161288: lwc1        $f2, 0x6248($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6248);
    // 0x8016128C: j           L_8016137C
    // 0x80161290: nop

        goto L_8016137C;
    // 0x80161290: nop

L_80161294:
    // 0x80161294: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161298: lwc1        $f0, 0x624C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X624C);
    // 0x8016129C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801612A0: lwc1        $f2, 0x6250($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6250);
    // 0x801612A4: j           L_8016137C
    // 0x801612A8: nop

        goto L_8016137C;
    // 0x801612A8: nop

L_801612AC:
    // 0x801612AC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801612B0: lwc1        $f0, 0x6254($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X6254);
    // 0x801612B4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801612B8: lwc1        $f2, 0x6258($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6258);
    // 0x801612BC: j           L_8016137C
    // 0x801612C0: nop

        goto L_8016137C;
    // 0x801612C0: nop

L_801612C4:
    // 0x801612C4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801612C8: lwc1        $f0, 0x625C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X625C);
    // 0x801612CC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801612D0: lwc1        $f2, 0x6260($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6260);
    // 0x801612D4: j           L_8016137C
    // 0x801612D8: nop

        goto L_8016137C;
    // 0x801612D8: nop

L_801612DC:
    // 0x801612DC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801612E0: lwc1        $f0, 0x6264($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X6264);
    // 0x801612E4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801612E8: lwc1        $f2, 0x6268($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6268);
    // 0x801612EC: j           L_8016137C
    // 0x801612F0: nop

        goto L_8016137C;
    // 0x801612F0: nop

L_801612F4:
    // 0x801612F4: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801612F8: lwc1        $f0, 0x626C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X626C);
    // 0x801612FC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161300: lwc1        $f2, 0x6270($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6270);
    // 0x80161304: j           L_8016137C
    // 0x80161308: nop

        goto L_8016137C;
    // 0x80161308: nop

L_8016130C:
    // 0x8016130C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161310: lwc1        $f0, 0x6274($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X6274);
    // 0x80161314: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161318: lwc1        $f2, 0x6278($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6278);
    // 0x8016131C: j           L_8016137C
    // 0x80161320: nop

        goto L_8016137C;
    // 0x80161320: nop

L_80161324:
    // 0x80161324: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161328: lwc1        $f0, 0x627C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X627C);
    // 0x8016132C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161330: lwc1        $f2, 0x6280($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6280);
    // 0x80161334: j           L_8016137C
    // 0x80161338: nop

        goto L_8016137C;
    // 0x80161338: nop

L_8016133C:
    // 0x8016133C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161340: lwc1        $f0, 0x6284($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X6284);
    // 0x80161344: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161348: lwc1        $f2, 0x6288($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6288);
    // 0x8016134C: j           L_8016137C
    // 0x80161350: nop

        goto L_8016137C;
    // 0x80161350: nop

L_80161354:
    // 0x80161354: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161358: lwc1        $f0, 0x628C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X628C);
    // 0x8016135C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161360: lwc1        $f2, 0x6290($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6290);
    // 0x80161364: j           L_8016137C
    // 0x80161368: nop

        goto L_8016137C;
    // 0x80161368: nop

L_8016136C:
    // 0x8016136C: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161370: lwc1        $f0, 0x6294($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X6294);
    // 0x80161374: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x80161378: lwc1        $f2, 0x6298($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X6298);
L_8016137C:
    // 0x8016137C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161380: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161384: swc1        $f0, 0x124C($at)
    MEM_W(0X124C, ctx->r1) = ctx->f0.u32l;
    // 0x80161388: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016138C: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161390: swc1        $f2, 0x1250($at)
    MEM_W(0X1250, ctx->r1) = ctx->f2.u32l;
    // 0x80161394: j           L_801613B4
    // 0x80161398: nop

        goto L_801613B4;
    // 0x80161398: nop

L_8016139C:
    // 0x8016139C: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x801613A0: lw          $v0, 0x6784($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6784);
    // 0x801613A4: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x801613A8: addiu       $a0, $a0, 0x60EC
    ctx->r4 = ADD32(ctx->r4, 0X60EC);
    // 0x801613AC: jalr        $v0
    // 0x801613B0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_2;
    // 0x801613B0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_2:
L_801613B4:
    // 0x801613B4: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x801613B8: bne         $s2, $v1, L_801613CC
    if (ctx->r18 != ctx->r3) {
        // 0x801613BC: addiu       $v0, $zero, 0x7
        ctx->r2 = ADD32(0, 0X7);
            goto L_801613CC;
    }
    // 0x801613BC: addiu       $v0, $zero, 0x7
    ctx->r2 = ADD32(0, 0X7);
    // 0x801613C0: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x801613C4: beq         $s3, $v0, L_801615F0
    if (ctx->r19 == ctx->r2) {
        // 0x801613C8: addiu       $v0, $zero, 0x7
        ctx->r2 = ADD32(0, 0X7);
            goto L_801615F0;
    }
    // 0x801613C8: addiu       $v0, $zero, 0x7
    ctx->r2 = ADD32(0, 0X7);
L_801613CC:
    // 0x801613CC: bnel        $v1, $v0, L_80161414
    if (ctx->r3 != ctx->r2) {
        // 0x801613D0: sw          $s2, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r18;
            goto L_80161414;
    }
    goto skip_0;
    // 0x801613D0: sw          $s2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r18;
    skip_0:
    // 0x801613D4: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x801613D8: bnel        $v0, $zero, L_80161414
    if (ctx->r2 != 0) {
        // 0x801613DC: sw          $s2, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r18;
            goto L_80161414;
    }
    goto skip_1;
    // 0x801613DC: sw          $s2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r18;
    skip_1:
    // 0x801613E0: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x801613E4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801613E8: bnel        $v1, $v0, L_80161414
    if (ctx->r3 != ctx->r2) {
        // 0x801613EC: sw          $s2, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r18;
            goto L_80161414;
    }
    goto skip_2;
    // 0x801613EC: sw          $s2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r18;
    skip_2:
    // 0x801613F0: lw          $v0, 0x20($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X20);
    // 0x801613F4: beql        $v0, $zero, L_80161414
    if (ctx->r2 == 0) {
        // 0x801613F8: sw          $s2, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r18;
            goto L_80161414;
    }
    goto skip_3;
    // 0x801613F8: sw          $s2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r18;
    skip_3:
    // 0x801613FC: jal         0x8016217C
    // 0x80161400: nop

    static_0_8016217C(rdram, ctx);
        goto after_3;
    // 0x80161400: nop

    after_3:
    // 0x80161404: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x80161408: jal         0x8015C9F0
    // 0x8016140C: nop

    static_0_8015C9F0(rdram, ctx);
        goto after_4;
    // 0x8016140C: nop

    after_4:
    // 0x80161410: sw          $s2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r18;
L_80161414:
    // 0x80161414: sw          $s3, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r19;
    // 0x80161418: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x8016141C: lw          $v0, -0x5BBC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X5BBC);
    // 0x80161420: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x80161424: addu        $a1, $s2, $zero
    ctx->r5 = ADD32(ctx->r18, 0);
    // 0x80161428: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x8016142C: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80161430: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80161434: lw          $a3, -0x1C4($at)
    ctx->r7 = MEM_W(ctx->r1, -0X1C4);
    // 0x80161438: lui         $s1, 0x801E
    ctx->r17 = S32(0X801E << 16);
    // 0x8016143C: addiu       $s1, $s1, -0x1C4
    ctx->r17 = ADD32(ctx->r17, -0X1C4);
    // 0x80161440: jal         0x80155888
    // 0x80161444: addu        $a2, $s3, $zero
    ctx->r6 = ADD32(ctx->r19, 0);
    func_80155888(rdram, ctx);
        goto after_5;
    // 0x80161444: addu        $a2, $s3, $zero
    ctx->r6 = ADD32(ctx->r19, 0);
    after_5:
    // 0x80161448: bne         $v0, $zero, L_80161468
    if (ctx->r2 != 0) {
        // 0x8016144C: nop
    
            goto L_80161468;
    }
    // 0x8016144C: nop

    // 0x80161450: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x80161454: lw          $v0, 0x6784($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6784);
    // 0x80161458: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x8016145C: addiu       $a0, $a0, 0x6118
    ctx->r4 = ADD32(ctx->r4, 0X6118);
    // 0x80161460: jalr        $v0
    // 0x80161464: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_6;
    // 0x80161464: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_6:
L_80161468:
    // 0x80161468: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x8016146C: lw          $v0, -0x5BBC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X5BBC);
    // 0x80161470: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80161474: addu        $v0, $v0, $s1
    ctx->r2 = ADD32(ctx->r2, ctx->r17);
    // 0x80161478: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    // 0x8016147C: beq         $v0, $zero, L_801614F8
    if (ctx->r2 == 0) {
        // 0x80161480: nop
    
            goto L_801614F8;
    }
    // 0x80161480: nop

    // 0x80161484: lw          $v0, 0x18($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X18);
    // 0x80161488: mtc1        $v0, $f2
    ctx->f2.u32l = ctx->r2;
    // 0x8016148C: cvt.d.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.d = CVT_D_W(ctx->f2.u32l);
    // 0x80161490: bgezl       $v0, L_801614A8
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80161494: cvt.s.d     $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.d); 
    ctx->f0.fl = CVT_S_D(ctx->f2.d);
            goto L_801614A8;
    }
    goto skip_4;
    // 0x80161494: cvt.s.d     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.d); 
    ctx->f0.fl = CVT_S_D(ctx->f2.d);
    skip_4:
    // 0x80161498: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x8016149C: ldc1        $f0, 0x62A0($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X62A0);
    // 0x801614A0: add.d       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f0.d); 
    ctx->f2.d = ctx->f2.d + ctx->f0.d;
    // 0x801614A4: cvt.s.d     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.d); 
    ctx->f0.fl = CVT_S_D(ctx->f2.d);
L_801614A8:
    // 0x801614A8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801614AC: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801614B0: swc1        $f0, 0x124C($at)
    MEM_W(0X124C, ctx->r1) = ctx->f0.u32l;
    // 0x801614B4: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x801614B8: lw          $v0, -0x5BBC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X5BBC);
    // 0x801614BC: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x801614C0: addu        $v0, $v0, $s1
    ctx->r2 = ADD32(ctx->r2, ctx->r17);
    // 0x801614C4: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    // 0x801614C8: lw          $v0, 0x1C($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1C);
    // 0x801614CC: mtc1        $v0, $f2
    ctx->f2.u32l = ctx->r2;
    // 0x801614D0: cvt.d.w     $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    ctx->f2.d = CVT_D_W(ctx->f2.u32l);
    // 0x801614D4: bgezl       $v0, L_801614EC
    if (SIGNED(ctx->r2) >= 0) {
        // 0x801614D8: cvt.s.d     $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.d); 
    ctx->f0.fl = CVT_S_D(ctx->f2.d);
            goto L_801614EC;
    }
    goto skip_5;
    // 0x801614D8: cvt.s.d     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.d); 
    ctx->f0.fl = CVT_S_D(ctx->f2.d);
    skip_5:
    // 0x801614DC: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801614E0: ldc1        $f0, 0x62A8($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X62A8);
    // 0x801614E4: add.d       $f2, $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f0.d); 
    ctx->f2.d = ctx->f2.d + ctx->f0.d;
    // 0x801614E8: cvt.s.d     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.d); 
    ctx->f0.fl = CVT_S_D(ctx->f2.d);
L_801614EC:
    // 0x801614EC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801614F0: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801614F4: swc1        $f0, 0x1250($at)
    MEM_W(0X1250, ctx->r1) = ctx->f0.u32l;
L_801614F8:
    // 0x801614F8: lui         $at, 0x8017
    ctx->r1 = S32(0X8017 << 16);
    // 0x801614FC: lwc1        $f20, 0x62B0($at)
    ctx->f20.u32l = MEM_W(ctx->r1, 0X62B0);
    // 0x80161500: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x80161504: mfc1        $a1, $f20
    ctx->r5 = (int32_t)ctx->f20.u32l;
    // 0x80161508: jal         0x80162280
    // 0x8016150C: nop

    func_80162280(rdram, ctx);
        goto after_7;
    // 0x8016150C: nop

    after_7:
    // 0x80161510: bne         $v0, $zero, L_80161530
    if (ctx->r2 != 0) {
        // 0x80161514: nop
    
            goto L_80161530;
    }
    // 0x80161514: nop

    // 0x80161518: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x8016151C: lw          $v0, 0x6784($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6784);
    // 0x80161520: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80161524: addiu       $a0, $a0, 0x614C
    ctx->r4 = ADD32(ctx->r4, 0X614C);
    // 0x80161528: jalr        $v0
    // 0x8016152C: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_8;
    // 0x8016152C: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_8:
L_80161530:
    // 0x80161530: lw          $v1, 0x10($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X10);
    // 0x80161534: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80161538: bne         $v1, $v0, L_801615F0
    if (ctx->r3 != ctx->r2) {
        // 0x8016153C: nop
    
            goto L_801615F0;
    }
    // 0x8016153C: nop

    // 0x80161540: lw          $a0, 0x20($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X20);
    // 0x80161544: beql        $a0, $zero, L_801615F0
    if (ctx->r4 == 0) {
        // 0x80161548: sw          $zero, 0x10($s0)
        MEM_W(0X10, ctx->r16) = 0;
            goto L_801615F0;
    }
    goto skip_6;
    // 0x80161548: sw          $zero, 0x10($s0)
    MEM_W(0X10, ctx->r16) = 0;
    skip_6:
    // 0x8016154C: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x80161550: lw          $v0, -0x5BBC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X5BBC);
    // 0x80161554: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80161558: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x8016155C: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80161560: lw          $a3, -0x1C4($at)
    ctx->r7 = MEM_W(ctx->r1, -0X1C4);
    // 0x80161564: addu        $a1, $s2, $zero
    ctx->r5 = ADD32(ctx->r18, 0);
    // 0x80161568: jal         0x80155888
    // 0x8016156C: addu        $a2, $s3, $zero
    ctx->r6 = ADD32(ctx->r19, 0);
    func_80155888(rdram, ctx);
        goto after_9;
    // 0x8016156C: addu        $a2, $s3, $zero
    ctx->r6 = ADD32(ctx->r19, 0);
    after_9:
    // 0x80161570: bne         $v0, $zero, L_80161590
    if (ctx->r2 != 0) {
        // 0x80161574: nop
    
            goto L_80161590;
    }
    // 0x80161574: nop

    // 0x80161578: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x8016157C: lw          $v0, 0x6784($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6784);
    // 0x80161580: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80161584: addiu       $a0, $a0, 0x6178
    ctx->r4 = ADD32(ctx->r4, 0X6178);
    // 0x80161588: jalr        $v0
    // 0x8016158C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_10;
    // 0x8016158C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_10:
L_80161590:
    // 0x80161590: lw          $a0, 0x20($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X20);
    // 0x80161594: mfc1        $a1, $f20
    ctx->r5 = (int32_t)ctx->f20.u32l;
    // 0x80161598: jal         0x80162280
    // 0x8016159C: nop

    func_80162280(rdram, ctx);
        goto after_11;
    // 0x8016159C: nop

    after_11:
    // 0x801615A0: bne         $v0, $zero, L_801615C0
    if (ctx->r2 != 0) {
        // 0x801615A4: nop
    
            goto L_801615C0;
    }
    // 0x801615A4: nop

    // 0x801615A8: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x801615AC: lw          $v0, 0x6784($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6784);
    // 0x801615B0: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x801615B4: addiu       $a0, $a0, 0x61B4
    ctx->r4 = ADD32(ctx->r4, 0X61B4);
    // 0x801615B8: jalr        $v0
    // 0x801615BC: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_12;
    // 0x801615BC: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_12:
L_801615C0:
    // 0x801615C0: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x801615C4: lw          $a1, 0x20($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X20);
    // 0x801615C8: jal         0x8015C050
    // 0x801615CC: nop

    func_8015C050(rdram, ctx);
        goto after_13;
    // 0x801615CC: nop

    after_13:
    // 0x801615D0: bne         $v0, $zero, L_801615F0
    if (ctx->r2 != 0) {
        // 0x801615D4: nop
    
            goto L_801615F0;
    }
    // 0x801615D4: nop

    // 0x801615D8: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x801615DC: lw          $v0, 0x6784($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6784);
    // 0x801615E0: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x801615E4: addiu       $a0, $a0, 0x61E8
    ctx->r4 = ADD32(ctx->r4, 0X61E8);
    // 0x801615E8: jalr        $v0
    // 0x801615EC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_14;
    // 0x801615EC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_14:
L_801615F0:
    // 0x801615F0: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x801615F4: jal         0x8015BF2C
    // 0x801615F8: addiu       $a1, $sp, 0x18
    ctx->r5 = ADD32(ctx->r29, 0X18);
    func_8015BF2C(rdram, ctx);
        goto after_15;
    // 0x801615F8: addiu       $a1, $sp, 0x18
    ctx->r5 = ADD32(ctx->r29, 0X18);
    after_15:
    // 0x801615FC: lw          $v0, 0x8C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X8C);
    // 0x80161600: addiu       $v0, $v0, 0x1000
    ctx->r2 = ADD32(ctx->r2, 0X1000);
    // 0x80161604: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161608: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8016160C: sw          $v0, 0x126C($at)
    MEM_W(0X126C, ctx->r1) = ctx->r2;
    // 0x80161610: jal         0x8016202C
    // 0x80161614: nop

    func_8016202C(rdram, ctx);
        goto after_16;
    // 0x80161614: nop

    after_16:
    // 0x80161618: srl         $v0, $v0, 12
    ctx->r2 = S32(U32(ctx->r2) >> 12);
    // 0x8016161C: andi        $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 & 0XFFFF;
    // 0x80161620: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161624: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161628: sw          $v0, 0x1270($at)
    MEM_W(0X1270, ctx->r1) = ctx->r2;
    // 0x8016162C: lw          $a1, 0x8C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X8C);
    // 0x80161630: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80161634: jal         0x8015FD64
    // 0x80161638: addiu       $a1, $a1, 0x1000
    ctx->r5 = ADD32(ctx->r5, 0X1000);
    static_0_8015FD64(rdram, ctx);
        goto after_17;
    // 0x80161638: addiu       $a1, $a1, 0x1000
    ctx->r5 = ADD32(ctx->r5, 0X1000);
    after_17:
    // 0x8016163C: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80161640: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x80161644: lw          $v0, 0x104($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X104);
    // 0x80161648: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016164C: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161650: sw          $v0, 0x1170($at)
    MEM_W(0X1170, ctx->r1) = ctx->r2;
    // 0x80161654: lw          $v0, 0x108($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X108);
    // 0x80161658: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8016165C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161660: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161664: sw          $v0, 0x1174($at)
    MEM_W(0X1174, ctx->r1) = ctx->r2;
    // 0x80161668: lw          $v0, 0x10C($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X10C);
    // 0x8016166C: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x80161670: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161674: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161678: sw          $v0, 0x1178($at)
    MEM_W(0X1178, ctx->r1) = ctx->r2;
    // 0x8016167C: lw          $v0, 0x110($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X110);
    // 0x80161680: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x80161684: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161688: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8016168C: sw          $v0, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r2;
    // 0x80161690: lw          $v0, 0x114($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X114);
    // 0x80161694: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x80161698: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016169C: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801616A0: sw          $v0, 0x1180($at)
    MEM_W(0X1180, ctx->r1) = ctx->r2;
    // 0x801616A4: lw          $v0, 0x118($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X118);
    // 0x801616A8: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x801616AC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801616B0: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801616B4: sw          $v0, 0x1184($at)
    MEM_W(0X1184, ctx->r1) = ctx->r2;
    // 0x801616B8: lw          $v0, 0x11C($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X11C);
    // 0x801616BC: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x801616C0: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801616C4: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801616C8: sw          $zero, 0x118C($at)
    MEM_W(0X118C, ctx->r1) = 0;
    // 0x801616CC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801616D0: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801616D4: sw          $zero, 0x1190($at)
    MEM_W(0X1190, ctx->r1) = 0;
    // 0x801616D8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801616DC: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801616E0: sw          $zero, 0x1194($at)
    MEM_W(0X1194, ctx->r1) = 0;
    // 0x801616E4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801616E8: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x801616EC: sw          $v0, 0x1188($at)
    MEM_W(0X1188, ctx->r1) = ctx->r2;
    // 0x801616F0: lw          $v0, 0x140($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X140);
    // 0x801616F4: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x801616F8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801616FC: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161700: sw          $v0, 0x1198($at)
    MEM_W(0X1198, ctx->r1) = ctx->r2;
    // 0x80161704: lw          $v0, 0x144($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X144);
    // 0x80161708: lw          $v1, 0x14($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X14);
    // 0x8016170C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161710: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161714: sw          $v0, 0x119C($at)
    MEM_W(0X119C, ctx->r1) = ctx->r2;
    // 0x80161718: lw          $v0, 0x148($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X148);
    // 0x8016171C: lw          $v1, 0xC($s0)
    ctx->r3 = MEM_W(ctx->r16, 0XC);
    // 0x80161720: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161724: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161728: sw          $v0, 0x11A0($at)
    MEM_W(0X11A0, ctx->r1) = ctx->r2;
    // 0x8016172C: blez        $v1, L_801617E8
    if (SIGNED(ctx->r3) <= 0) {
        // 0x80161730: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_801617E8;
    }
    // 0x80161730: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x80161734: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x80161738: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    // 0x8016173C: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    // 0x80161740: addu        $v1, $s0, $zero
    ctx->r3 = ADD32(ctx->r16, 0);
L_80161744:
    // 0x80161744: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161748: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x8016174C: sw          $zero, 0x11A4($at)
    MEM_W(0X11A4, ctx->r1) = 0;
    // 0x80161750: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161754: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80161758: sw          $zero, 0x11A8($at)
    MEM_W(0X11A8, ctx->r1) = 0;
    // 0x8016175C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161760: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80161764: sw          $zero, 0x11AC($at)
    MEM_W(0X11AC, ctx->r1) = 0;
    // 0x80161768: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016176C: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80161770: sw          $zero, 0x11B0($at)
    MEM_W(0X11B0, ctx->r1) = 0;
    // 0x80161774: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161778: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x8016177C: sw          $zero, 0x11B4($at)
    MEM_W(0X11B4, ctx->r1) = 0;
    // 0x80161780: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161784: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80161788: sw          $zero, 0x11B8($at)
    MEM_W(0X11B8, ctx->r1) = 0;
    // 0x8016178C: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161790: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x80161794: sw          $zero, 0x11BC($at)
    MEM_W(0X11BC, ctx->r1) = 0;
    // 0x80161798: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016179C: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x801617A0: sw          $a3, 0x11C0($at)
    MEM_W(0X11C0, ctx->r1) = ctx->r7;
    // 0x801617A4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801617A8: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x801617AC: sw          $a1, 0x11C4($at)
    MEM_W(0X11C4, ctx->r1) = ctx->r5;
    // 0x801617B0: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801617B4: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x801617B8: sw          $a1, 0x11C8($at)
    MEM_W(0X11C8, ctx->r1) = ctx->r5;
    // 0x801617BC: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801617C0: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x801617C4: sw          $a2, 0x11CC($at)
    MEM_W(0X11CC, ctx->r1) = ctx->r6;
    // 0x801617C8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801617CC: addu        $at, $v1, $at
    ctx->r1 = ADD32(ctx->r3, ctx->r1);
    // 0x801617D0: sw          $zero, 0x11D0($at)
    MEM_W(0X11D0, ctx->r1) = 0;
    // 0x801617D4: lw          $v0, 0xC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC);
    // 0x801617D8: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x801617DC: slt         $v0, $a0, $v0
    ctx->r2 = SIGNED(ctx->r4) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x801617E0: bne         $v0, $zero, L_80161744
    if (ctx->r2 != 0) {
        // 0x801617E4: addiu       $v1, $v1, 0x30
        ctx->r3 = ADD32(ctx->r3, 0X30);
            goto L_80161744;
    }
    // 0x801617E4: addiu       $v1, $v1, 0x30
    ctx->r3 = ADD32(ctx->r3, 0X30);
L_801617E8:
    // 0x801617E8: lui         $v1, 0x1
    ctx->r3 = S32(0X1 << 16);
    // 0x801617EC: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x801617F0: ori         $v1, $v1, 0x4001
    ctx->r3 = ctx->r3 | 0X4001;
    // 0x801617F4: sw          $v1, 0x110($v0)
    MEM_W(0X110, ctx->r2) = ctx->r3;
    // 0x801617F8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x801617FC: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161800: sw          $v1, 0x117C($at)
    MEM_W(0X117C, ctx->r1) = ctx->r3;
    // 0x80161804: jal         0x801621DC
    // 0x80161808: addu        $a0, $s5, $zero
    ctx->r4 = ADD32(ctx->r21, 0);
    func_801621DC(rdram, ctx);
        goto after_18;
    // 0x80161808: addu        $a0, $s5, $zero
    ctx->r4 = ADD32(ctx->r21, 0);
    after_18:
    // 0x8016180C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    // 0x80161810: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80161814: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80161818: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    // 0x8016181C: sw          $zero, 0x10C($v0)
    MEM_W(0X10C, ctx->r2) = 0;
    // 0x80161820: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161824: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161828: sw          $zero, 0x1178($at)
    MEM_W(0X1178, ctx->r1) = 0;
    // 0x8016182C: jal         0x8015E2F4
    // 0x80161830: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    static_0_8015E2F4(rdram, ctx);
        goto after_19;
    // 0x80161830: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    after_19:
    // 0x80161834: jal         0x8015E658
    // 0x80161838: addiu       $a0, $zero, 0x7
    ctx->r4 = ADD32(0, 0X7);
    static_0_8015E658(rdram, ctx);
        goto after_20;
    // 0x80161838: addiu       $a0, $zero, 0x7
    ctx->r4 = ADD32(0, 0X7);
    after_20:
    // 0x8016183C: jal         0x8015E70C
    // 0x80161840: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    func_8015E70C(rdram, ctx);
        goto after_21;
    // 0x80161840: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_21:
    // 0x80161844: jal         0x8015EC70
    // 0x80161848: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015EC70(rdram, ctx);
        goto after_22;
    // 0x80161848: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_22:
    // 0x8016184C: jal         0x8015F178
    // 0x80161850: addiu       $a0, $zero, -0x1
    ctx->r4 = ADD32(0, -0X1);
    func_8015F178(rdram, ctx);
        goto after_23;
    // 0x80161850: addiu       $a0, $zero, -0x1
    ctx->r4 = ADD32(0, -0X1);
    after_23:
    // 0x80161854: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x80161858: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x8016185C: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161860: lwc1        $f0, 0x124C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X124C);
    // 0x80161864: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161868: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x8016186C: lwc1        $f2, 0x1250($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X1250);
    // 0x80161870: trunc.w.s   $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    ctx->f4.u32l = TRUNC_W_S(ctx->f0.fl);
    // 0x80161874: mfc1        $a2, $f4
    ctx->r6 = (int32_t)ctx->f4.u32l;
    // 0x80161878: trunc.w.s   $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.u32l = TRUNC_W_S(ctx->f2.fl);
    // 0x8016187C: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x80161880: jal         0x8015ED94
    // 0x80161884: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_8015ED94(rdram, ctx);
        goto after_24;
    // 0x80161884: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_24:
    // 0x80161888: jal         0x8015F070
    // 0x8016188C: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    func_8015F070(rdram, ctx);
        goto after_25;
    // 0x8016188C: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_25:
    // 0x80161890: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    // 0x80161894: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    // 0x80161898: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    // 0x8016189C: addu        $a3, $zero, $zero
    ctx->r7 = ADD32(0, 0);
    // 0x801618A0: jal         0x8015EE64
    // 0x801618A4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    static_0_8015EE64(rdram, ctx);
        goto after_26;
    // 0x801618A4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_26:
    // 0x801618A8: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    // 0x801618AC: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    // 0x801618B0: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x801618B4: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
    // 0x801618B8: jal         0x8015E3E4
    // 0x801618BC: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    static_0_8015E3E4(rdram, ctx);
        goto after_27;
    // 0x801618BC: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_27:
    // 0x801618C0: jal         0x8015F23C
    // 0x801618C4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    func_8015F23C(rdram, ctx);
        goto after_28;
    // 0x801618C4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_28:
    // 0x801618C8: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801618CC: jal         0x8015F0B8
    // 0x801618D0: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_8015F0B8(rdram, ctx);
        goto after_29;
    // 0x801618D0: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_29:
    // 0x801618D4: jal         0x8015F264
    // 0x801618D8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015F264(rdram, ctx);
        goto after_30;
    // 0x801618D8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_30:
    // 0x801618DC: jal         0x8015F498
    // 0x801618E0: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015F498(rdram, ctx);
        goto after_31;
    // 0x801618E0: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_31:
    // 0x801618E4: jal         0x8015F3C4
    // 0x801618E8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015F3C4(rdram, ctx);
        goto after_32;
    // 0x801618E8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_32:
    // 0x801618EC: jal         0x8015F308
    // 0x801618F0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    static_0_8015F308(rdram, ctx);
        goto after_33;
    // 0x801618F0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_33:
    // 0x801618F4: jal         0x8015F264
    // 0x801618F8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015F264(rdram, ctx);
        goto after_34;
    // 0x801618F8: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_34:
    // 0x801618FC: jal         0x8015F7F8
    // 0x80161900: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    static_0_8015F7F8(rdram, ctx);
        goto after_35;
    // 0x80161900: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    after_35:
    // 0x80161904: jal         0x8015F8D4
    // 0x80161908: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015F8D4(rdram, ctx);
        goto after_36;
    // 0x80161908: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_36:
    // 0x8016190C: jal         0x8015F9A8
    // 0x80161910: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    static_0_8015F9A8(rdram, ctx);
        goto after_37;
    // 0x80161910: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_37:
    // 0x80161914: jal         0x80163948
    // 0x80161918: nop

    func_80163948(rdram, ctx);
        goto after_38;
    // 0x80161918: nop

    after_38:
    // 0x8016191C: lw          $v1, 0xC($s0)
    ctx->r3 = MEM_W(ctx->r16, 0XC);
    // 0x80161920: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80161924: beq         $v1, $v0, L_801619AC
    if (ctx->r3 == ctx->r2) {
        // 0x80161928: slti        $v0, $v1, 0x3
        ctx->r2 = SIGNED(ctx->r3) < 0X3 ? 1 : 0;
            goto L_801619AC;
    }
    // 0x80161928: slti        $v0, $v1, 0x3
    ctx->r2 = SIGNED(ctx->r3) < 0X3 ? 1 : 0;
    // 0x8016192C: beq         $v0, $zero, L_80161944
    if (ctx->r2 == 0) {
        // 0x80161930: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_80161944;
    }
    // 0x80161930: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80161934: beq         $v1, $v0, L_80161A0C
    if (ctx->r3 == ctx->r2) {
        // 0x80161938: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_80161A0C;
    }
    // 0x80161938: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x8016193C: j           L_80161A68
    // 0x80161940: addiu       $v0, $zero, 0x2000
    ctx->r2 = ADD32(0, 0X2000);
        goto L_80161A68;
    // 0x80161940: addiu       $v0, $zero, 0x2000
    ctx->r2 = ADD32(0, 0X2000);
L_80161944:
    // 0x80161944: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x80161948: bne         $v1, $v0, L_80161A68
    if (ctx->r3 != ctx->r2) {
        // 0x8016194C: addiu       $v0, $zero, 0x2000
        ctx->r2 = ADD32(0, 0X2000);
            goto L_80161A68;
    }
    // 0x8016194C: addiu       $v0, $zero, 0x2000
    ctx->r2 = ADD32(0, 0X2000);
    // 0x80161950: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x80161954: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x80161958: jal         0x80162DA8
    // 0x8016195C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_80162DA8(rdram, ctx);
        goto after_39;
    // 0x8016195C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_39:
    // 0x80161960: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x80161964: lui         $a3, 0x3F80
    ctx->r7 = S32(0X3F80 << 16);
    // 0x80161968: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x8016196C: jal         0x801633BC
    // 0x80161970: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_801633BC(rdram, ctx);
        goto after_40;
    // 0x80161970: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_40:
    // 0x80161974: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x80161978: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x8016197C: jal         0x801635E8
    // 0x80161980: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    func_801635E8(rdram, ctx);
        goto after_41;
    // 0x80161980: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    after_41:
    // 0x80161984: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x80161988: jal         0x80163874
    // 0x8016198C: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_80163874(rdram, ctx);
        goto after_42;
    // 0x8016198C: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_42:
    // 0x80161990: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x80161994: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80161998: jal         0x80163ACC
    // 0x8016199C: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    func_80163ACC(rdram, ctx);
        goto after_43;
    // 0x8016199C: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    after_43:
    // 0x801619A0: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x801619A4: jal         0x801631DC
    // 0x801619A8: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_801631DC(rdram, ctx);
        goto after_44;
    // 0x801619A8: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_44:
L_801619AC:
    // 0x801619AC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801619B0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801619B4: jal         0x80162DA8
    // 0x801619B8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_80162DA8(rdram, ctx);
        goto after_45;
    // 0x801619B8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_45:
    // 0x801619BC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801619C0: lui         $a3, 0x3F80
    ctx->r7 = S32(0X3F80 << 16);
    // 0x801619C4: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x801619C8: jal         0x801633BC
    // 0x801619CC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_801633BC(rdram, ctx);
        goto after_46;
    // 0x801619CC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_46:
    // 0x801619D0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801619D4: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x801619D8: jal         0x801635E8
    // 0x801619DC: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    func_801635E8(rdram, ctx);
        goto after_47;
    // 0x801619DC: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    after_47:
    // 0x801619E0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801619E4: jal         0x80163874
    // 0x801619E8: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_80163874(rdram, ctx);
        goto after_48;
    // 0x801619E8: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_48:
    // 0x801619EC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801619F0: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x801619F4: jal         0x80163ACC
    // 0x801619F8: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    func_80163ACC(rdram, ctx);
        goto after_49;
    // 0x801619F8: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    after_49:
    // 0x801619FC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80161A00: jal         0x801631DC
    // 0x80161A04: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_801631DC(rdram, ctx);
        goto after_50;
    // 0x80161A04: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_50:
    // 0x80161A08: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
L_80161A0C:
    // 0x80161A0C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x80161A10: jal         0x80162DA8
    // 0x80161A14: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_80162DA8(rdram, ctx);
        goto after_51;
    // 0x80161A14: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_51:
    // 0x80161A18: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x80161A1C: lui         $a3, 0x3F80
    ctx->r7 = S32(0X3F80 << 16);
    // 0x80161A20: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80161A24: jal         0x801633BC
    // 0x80161A28: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_801633BC(rdram, ctx);
        goto after_52;
    // 0x80161A28: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_52:
    // 0x80161A2C: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x80161A30: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80161A34: jal         0x801635E8
    // 0x80161A38: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    func_801635E8(rdram, ctx);
        goto after_53;
    // 0x80161A38: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    after_53:
    // 0x80161A3C: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x80161A40: jal         0x80163874
    // 0x80161A44: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_80163874(rdram, ctx);
        goto after_54;
    // 0x80161A44: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_54:
    // 0x80161A48: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x80161A4C: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    // 0x80161A50: jal         0x80163ACC
    // 0x80161A54: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    func_80163ACC(rdram, ctx);
        goto after_55;
    // 0x80161A54: addu        $a2, $zero, $zero
    ctx->r6 = ADD32(0, 0);
    after_55:
    // 0x80161A58: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x80161A5C: jal         0x801631DC
    // 0x80161A60: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    func_801631DC(rdram, ctx);
        goto after_56;
    // 0x80161A60: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
    after_56:
    // 0x80161A64: addiu       $v0, $zero, 0x2000
    ctx->r2 = ADD32(0, 0X2000);
L_80161A68:
    // 0x80161A68: xori        $v1, $s5, 0x1
    ctx->r3 = ctx->r21 ^ 0X1;
    // 0x80161A6C: movn        $v0, $zero, $v1
    if (ctx->r3 != 0) ctx->r2 = ctx->r0;
    // 0x80161A70: addu        $v1, $v0, $zero
    ctx->r3 = ADD32(ctx->r2, 0);
    // 0x80161A74: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80161A78: beq         $s4, $v0, L_80161AA0
    if (ctx->r20 == ctx->r2) {
        // 0x80161A7C: slti        $v0, $s4, 0x2
        ctx->r2 = SIGNED(ctx->r20) < 0X2 ? 1 : 0;
            goto L_80161AA0;
    }
    // 0x80161A7C: slti        $v0, $s4, 0x2
    ctx->r2 = SIGNED(ctx->r20) < 0X2 ? 1 : 0;
    // 0x80161A80: bne         $v0, $zero, L_80161AAC
    if (ctx->r2 != 0) {
        // 0x80161A84: addiu       $v0, $zero, 0x2
        ctx->r2 = ADD32(0, 0X2);
            goto L_80161AAC;
    }
    // 0x80161A84: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80161A88: beq         $s4, $v0, L_80161AA8
    if (ctx->r20 == ctx->r2) {
        // 0x80161A8C: addiu       $v0, $zero, 0x3
        ctx->r2 = ADD32(0, 0X3);
            goto L_80161AA8;
    }
    // 0x80161A8C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x80161A90: beql        $s4, $v0, L_80161AAC
    if (ctx->r20 == ctx->r2) {
        // 0x80161A94: ori         $v1, $v1, 0x600
        ctx->r3 = ctx->r3 | 0X600;
            goto L_80161AAC;
    }
    goto skip_7;
    // 0x80161A94: ori         $v1, $v1, 0x600
    ctx->r3 = ctx->r3 | 0X600;
    skip_7:
    // 0x80161A98: j           L_80161AAC
    // 0x80161A9C: nop

        goto L_80161AAC;
    // 0x80161A9C: nop

L_80161AA0:
    // 0x80161AA0: j           L_80161AAC
    // 0x80161AA4: ori         $v1, $v1, 0x200
    ctx->r3 = ctx->r3 | 0X200;
        goto L_80161AAC;
    // 0x80161AA4: ori         $v1, $v1, 0x200
    ctx->r3 = ctx->r3 | 0X200;
L_80161AA8:
    // 0x80161AA8: ori         $v1, $v1, 0x400
    ctx->r3 = ctx->r3 | 0X400;
L_80161AAC:
    // 0x80161AAC: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80161AB0: sw          $v1, 0x114($v0)
    MEM_W(0X114, ctx->r2) = ctx->r3;
    // 0x80161AB4: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x80161AB8: addu        $at, $s0, $at
    ctx->r1 = ADD32(ctx->r16, ctx->r1);
    // 0x80161ABC: sw          $v1, 0x1180($at)
    MEM_W(0X1180, ctx->r1) = ctx->r3;
    // 0x80161AC0: jal         0x80160898
    // 0x80161AC4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    func_80160898(rdram, ctx);
        goto after_57;
    // 0x80161AC4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_57:
    // 0x80161AC8: jal         0x80160B30
    // 0x80161ACC: addiu       $a0, $zero, 0xFF
    ctx->r4 = ADD32(0, 0XFF);
    func_80160B30(rdram, ctx);
        goto after_58;
    // 0x80161ACC: addiu       $a0, $zero, 0xFF
    ctx->r4 = ADD32(0, 0XFF);
    after_58:
    // 0x80161AD0: jal         0x80160B78
    // 0x80161AD4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    func_80160B78(rdram, ctx);
        goto after_59;
    // 0x80161AD4: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    after_59:
    // 0x80161AD8: jal         0x80160ABC
    // 0x80161ADC: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    func_80160ABC(rdram, ctx);
        goto after_60;
    // 0x80161ADC: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    after_60:
    // 0x80161AE0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80161AE4: lw          $ra, 0xD8($sp)
    ctx->r31 = MEM_W(ctx->r29, 0XD8);
    // 0x80161AE8: lw          $s5, 0xD4($sp)
    ctx->r21 = MEM_W(ctx->r29, 0XD4);
    // 0x80161AEC: lw          $s4, 0xD0($sp)
    ctx->r20 = MEM_W(ctx->r29, 0XD0);
    // 0x80161AF0: lw          $s3, 0xCC($sp)
    ctx->r19 = MEM_W(ctx->r29, 0XCC);
    // 0x80161AF4: lw          $s2, 0xC8($sp)
    ctx->r18 = MEM_W(ctx->r29, 0XC8);
    // 0x80161AF8: lw          $s1, 0xC4($sp)
    ctx->r17 = MEM_W(ctx->r29, 0XC4);
    // 0x80161AFC: lw          $s0, 0xC0($sp)
    ctx->r16 = MEM_W(ctx->r29, 0XC0);
    // 0x80161B00: ldc1        $f20, 0xE0($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0XE0);
    // 0x80161B04: addiu       $sp, $sp, 0xE8
    ctx->r29 = ADD32(ctx->r29, 0XE8);
    // 0x80161B08: jr          $ra
    // 0x80161B0C: nop

    return;
    // 0x80161B0C: nop

;}
RECOMP_FUNC void func_80161B18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80161B18: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80161B1C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80161B20: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x80161B24: bne         $v0, $zero, L_80161BDC
    if (ctx->r2 != 0) {
        // 0x80161B28: sw          $ra, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r31;
            goto L_80161BDC;
    }
    // 0x80161B28: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80161B2C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80161B30: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80161B34: sw          $v0, 0x3498($at)
    MEM_W(0X3498, ctx->r1) = ctx->r2;
    // 0x80161B38: jal         0x80160DB0
    // 0x80161B3C: nop

    func_80160DB0(rdram, ctx);
        goto after_0;
    // 0x80161B3C: nop

    after_0:
    // 0x80161B40: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x80161B44: sw          $v0, 0x2678($at)
    MEM_W(0X2678, ctx->r1) = ctx->r2;
    // 0x80161B48: beq         $v0, $zero, L_80161C34
    if (ctx->r2 == 0) {
        // 0x80161B4C: addu        $v0, $zero, $zero
        ctx->r2 = ADD32(0, 0);
            goto L_80161C34;
    }
    // 0x80161B4C: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
    // 0x80161B50: lui         $a2, 0x801E
    ctx->r6 = S32(0X801E << 16);
    // 0x80161B54: addiu       $a2, $a2, 0x6540
    ctx->r6 = ADD32(ctx->r6, 0X6540);
    // 0x80161B58: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80161B5C: blez        $v0, L_80161B90
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80161B60: addu        $a0, $zero, $zero
        ctx->r4 = ADD32(0, 0);
            goto L_80161B90;
    }
    // 0x80161B60: addu        $a0, $zero, $zero
    ctx->r4 = ADD32(0, 0);
    // 0x80161B64: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
L_80161B68:
    // 0x80161B68: lui         $v1, 0x1
    ctx->r3 = S32(0X1 << 16);
    // 0x80161B6C: ori         $v1, $v1, 0x12AC
    ctx->r3 = ctx->r3 | 0X12AC;
    // 0x80161B70: lui         $at, 0x801F
    ctx->r1 = S32(0X801F << 16);
    // 0x80161B74: addu        $at, $at, $a1
    ctx->r1 = ADD32(ctx->r1, ctx->r5);
    // 0x80161B78: sw          $zero, 0x7B8C($at)
    MEM_W(0X7B8C, ctx->r1) = 0;
    // 0x80161B7C: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80161B80: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x80161B84: slt         $v0, $a0, $v0
    ctx->r2 = SIGNED(ctx->r4) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80161B88: bne         $v0, $zero, L_80161B68
    if (ctx->r2 != 0) {
        // 0x80161B8C: addu        $a1, $a1, $v1
        ctx->r5 = ADD32(ctx->r5, ctx->r3);
            goto L_80161B68;
    }
    // 0x80161B8C: addu        $a1, $a1, $v1
    ctx->r5 = ADD32(ctx->r5, ctx->r3);
L_80161B90:
    // 0x80161B90: addu        $a3, $s0, $zero
    ctx->r7 = ADD32(ctx->r16, 0);
    // 0x80161B94: lui         $a2, 0x801E
    ctx->r6 = S32(0X801E << 16);
    // 0x80161B98: addiu       $a2, $a2, 0x6540
    ctx->r6 = ADD32(ctx->r6, 0X6540);
    // 0x80161B9C: addiu       $t0, $a2, 0x20
    ctx->r8 = ADD32(ctx->r6, 0X20);
L_80161BA0:
    // 0x80161BA0: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80161BA4: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x80161BA8: lw          $a0, 0x8($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X8);
    // 0x80161BAC: lw          $a1, 0xC($a2)
    ctx->r5 = MEM_W(ctx->r6, 0XC);
    // 0x80161BB0: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80161BB4: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
    // 0x80161BB8: sw          $a0, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r4;
    // 0x80161BBC: sw          $a1, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->r5;
    // 0x80161BC0: addiu       $a2, $a2, 0x10
    ctx->r6 = ADD32(ctx->r6, 0X10);
    // 0x80161BC4: bne         $a2, $t0, L_80161BA0
    if (ctx->r6 != ctx->r8) {
        // 0x80161BC8: addiu       $a3, $a3, 0x10
        ctx->r7 = ADD32(ctx->r7, 0X10);
            goto L_80161BA0;
    }
    // 0x80161BC8: addiu       $a3, $a3, 0x10
    ctx->r7 = ADD32(ctx->r7, 0X10);
    // 0x80161BCC: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80161BD0: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x80161BD4: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80161BD8: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
L_80161BDC:
    // 0x80161BDC: addu        $a3, $s0, $zero
    ctx->r7 = ADD32(ctx->r16, 0);
    // 0x80161BE0: lui         $a2, 0x801E
    ctx->r6 = S32(0X801E << 16);
    // 0x80161BE4: addiu       $a2, $a2, 0x6540
    ctx->r6 = ADD32(ctx->r6, 0X6540);
    // 0x80161BE8: addiu       $t0, $a2, 0x20
    ctx->r8 = ADD32(ctx->r6, 0X20);
L_80161BEC:
    // 0x80161BEC: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80161BF0: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x80161BF4: lw          $a0, 0x8($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X8);
    // 0x80161BF8: lw          $a1, 0xC($a2)
    ctx->r5 = MEM_W(ctx->r6, 0XC);
    // 0x80161BFC: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80161C00: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
    // 0x80161C04: sw          $a0, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r4;
    // 0x80161C08: sw          $a1, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->r5;
    // 0x80161C0C: addiu       $a2, $a2, 0x10
    ctx->r6 = ADD32(ctx->r6, 0X10);
    // 0x80161C10: bne         $a2, $t0, L_80161BEC
    if (ctx->r6 != ctx->r8) {
        // 0x80161C14: addiu       $a3, $a3, 0x10
        ctx->r7 = ADD32(ctx->r7, 0X10);
            goto L_80161BEC;
    }
    // 0x80161C14: addiu       $a3, $a3, 0x10
    ctx->r7 = ADD32(ctx->r7, 0X10);
    // 0x80161C18: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80161C1C: lw          $v1, 0x4($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4);
    // 0x80161C20: sw          $v0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r2;
    // 0x80161C24: jal         0x801606F0
    // 0x80161C28: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
    func_801606F0(rdram, ctx);
        goto after_1;
    // 0x80161C28: sw          $v1, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r3;
    after_1:
    // 0x80161C2C: lui         $v0, 0x801B
    ctx->r2 = S32(0X801B << 16);
    // 0x80161C30: lw          $v0, 0x2678($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X2678);
L_80161C34:
    // 0x80161C34: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80161C38: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80161C3C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80161C40: jr          $ra
    // 0x80161C44: nop

    return;
    // 0x80161C44: nop

;}
RECOMP_FUNC void func_80161C50(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80161C50: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80161C54: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    // 0x80161C58: addu        $s0, $a0, $zero
    ctx->r16 = ADD32(ctx->r4, 0);
    // 0x80161C5C: slt         $v0, $s0, $v0
    ctx->r2 = SIGNED(ctx->r16) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80161C60: bne         $v0, $zero, L_80161C80
    if (ctx->r2 != 0) {
        // 0x80161C64: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_80161C80;
    }
    // 0x80161C64: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80161C68: lui         $v0, 0x8023
    ctx->r2 = S32(0X8023 << 16);
    // 0x80161C6C: lw          $v0, 0x6784($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X6784);
    // 0x80161C70: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80161C74: addiu       $a0, $a0, 0x62B4
    ctx->r4 = ADD32(ctx->r4, 0X62B4);
    // 0x80161C78: jalr        $v0
    // 0x80161C7C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_0;
    // 0x80161C7C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_0:
L_80161C80:
    // 0x80161C80: lui         $at, 0x8023
    ctx->r1 = S32(0X8023 << 16);
    // 0x80161C84: sw          $s0, -0x5BBC($at)
    MEM_W(-0X5BBC, ctx->r1) = ctx->r16;
    // 0x80161C88: jal         0x8015FE70
    // 0x80161C8C: nop

    func_8015FE70(rdram, ctx);
        goto after_1;
    // 0x80161C8C: nop

    after_1:
    // 0x80161C90: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80161C94: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80161C98: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80161C9C: jr          $ra
    // 0x80161CA0: nop

    return;
    // 0x80161CA0: nop

;}
RECOMP_FUNC void func_80161D44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80161D44: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80161D48: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80161D4C: addiu       $a0, $a0, 0x62D4
    ctx->r4 = ADD32(ctx->r4, 0X62D4);
    // 0x80161D50: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80161D54: jal         0x80143950
    // 0x80161D58: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    func_80143950(rdram, ctx);
        goto after_0;
    // 0x80161D58: sw          $s0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r16;
    after_0:
    // 0x80161D5C: addu        $s0, $v0, $zero
    ctx->r16 = ADD32(ctx->r2, 0);
    // 0x80161D60: lui         $a1, 0x8017
    ctx->r5 = S32(0X8017 << 16);
    // 0x80161D64: addiu       $a1, $a1, 0x62E8
    ctx->r5 = ADD32(ctx->r5, 0X62E8);
    // 0x80161D68: jal         0x80142C58
    // 0x80161D6C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    func_80142C58(rdram, ctx);
        goto after_1;
    // 0x80161D6C: addu        $a0, $s0, $zero
    ctx->r4 = ADD32(ctx->r16, 0);
    after_1:
    // 0x80161D70: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x80161D74: sw          $v0, 0x2680($at)
    MEM_W(0X2680, ctx->r1) = ctx->r2;
    // 0x80161D78: bne         $v0, $zero, L_80161D94
    if (ctx->r2 != 0) {
        // 0x80161D7C: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_80161D94;
    }
    // 0x80161D7C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80161D80: lui         $a0, 0x8017
    ctx->r4 = S32(0X8017 << 16);
    // 0x80161D84: addiu       $a0, $a0, 0x62EC
    ctx->r4 = ADD32(ctx->r4, 0X62EC);
    // 0x80161D88: jal         0x80141210
    // 0x80161D8C: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    static_0_80141210(rdram, ctx);
        goto after_2;
    // 0x80161D8C: addu        $a1, $s0, $zero
    ctx->r5 = ADD32(ctx->r16, 0);
    after_2:
    // 0x80161D90: addu        $v0, $zero, $zero
    ctx->r2 = ADD32(0, 0);
L_80161D94:
    // 0x80161D94: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80161D98: lw          $s0, 0x10($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X10);
    // 0x80161D9C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80161DA0: jr          $ra
    // 0x80161DA4: nop

    return;
    // 0x80161DA4: nop

;}
RECOMP_FUNC void func_80161DB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80161DB8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80161DBC: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80161DC0: sll         $v1, $a0, 3
    ctx->r3 = S32(ctx->r4 << 3);
    // 0x80161DC4: addu        $v1, $v1, $a0
    ctx->r3 = ADD32(ctx->r3, ctx->r4);
    // 0x80161DC8: sll         $v1, $v1, 3
    ctx->r3 = S32(ctx->r3 << 3);
    // 0x80161DCC: addu        $v1, $v1, $a0
    ctx->r3 = ADD32(ctx->r3, ctx->r4);
    // 0x80161DD0: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80161DD4: addu        $v1, $v1, $a0
    ctx->r3 = ADD32(ctx->r3, ctx->r4);
    // 0x80161DD8: sll         $v0, $v1, 4
    ctx->r2 = S32(ctx->r3 << 4);
    // 0x80161DDC: subu        $v0, $v0, $v1
    ctx->r2 = SUB32(ctx->r2, ctx->r3);
    // 0x80161DE0: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80161DE4: subu        $v0, $v0, $a0
    ctx->r2 = SUB32(ctx->r2, ctx->r4);
    // 0x80161DE8: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80161DEC: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80161DF0: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80161DF4: lw          $a0, 0x6A34($at)
    ctx->r4 = MEM_W(ctx->r1, 0X6A34);
    // 0x80161DF8: addiu       $a1, $zero, 0xD6
    ctx->r5 = ADD32(0, 0XD6);
    // 0x80161DFC: jal         0x80143460
    // 0x80161E00: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    func_80143460(rdram, ctx);
        goto after_0;
    // 0x80161E00: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_0:
    // 0x80161E04: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80161E08: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80161E0C: jr          $ra
    // 0x80161E10: nop

    return;
    // 0x80161E10: nop

;}
RECOMP_FUNC void func_80161E1C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80161E1C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80161E20: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80161E24: jal         0x80142850
    // 0x80161E28: nop

    func_80142850(rdram, ctx);
        goto after_0;
    // 0x80161E28: nop

    after_0:
    // 0x80161E2C: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80161E30: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80161E34: jr          $ra
    // 0x80161E38: nop

    return;
    // 0x80161E38: nop

;}
RECOMP_FUNC void func_80161F10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80161F10: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80161F14: sw          $ra, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r31;
    // 0x80161F18: sll         $v0, $a1, 3
    ctx->r2 = S32(ctx->r5 << 3);
    // 0x80161F1C: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80161F20: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x80161F24: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80161F28: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80161F2C: addu        $v0, $v0, $a1
    ctx->r2 = ADD32(ctx->r2, ctx->r5);
    // 0x80161F30: sll         $v1, $v0, 4
    ctx->r3 = S32(ctx->r2 << 4);
    // 0x80161F34: subu        $v1, $v1, $v0
    ctx->r3 = SUB32(ctx->r3, ctx->r2);
    // 0x80161F38: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80161F3C: subu        $v1, $v1, $a1
    ctx->r3 = SUB32(ctx->r3, ctx->r5);
    // 0x80161F40: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80161F44: lui         $v0, 0x801E
    ctx->r2 = S32(0X801E << 16);
    // 0x80161F48: addiu       $v0, $v0, 0x6A20
    ctx->r2 = ADD32(ctx->r2, 0X6A20);
    // 0x80161F4C: bne         $a0, $zero, L_80161F60
    if (ctx->r4 != 0) {
        // 0x80161F50: addu        $v1, $v1, $v0
        ctx->r3 = ADD32(ctx->r3, ctx->r2);
            goto L_80161F60;
    }
    // 0x80161F50: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x80161F54: lw          $a0, 0x14($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X14);
    // 0x80161F58: j           L_80161F68
    // 0x80161F5C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
        goto L_80161F68;
    // 0x80161F5C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
L_80161F60:
    // 0x80161F60: lw          $a0, 0x14($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X14);
    // 0x80161F64: addu        $a1, $zero, $zero
    ctx->r5 = ADD32(0, 0);
L_80161F68:
    // 0x80161F68: jal         0x8015D248
    // 0x80161F6C: nop

    func_8015D248(rdram, ctx);
        goto after_0;
    // 0x80161F6C: nop

    after_0:
    // 0x80161F70: lw          $ra, 0x10($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X10);
    // 0x80161F74: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80161F78: jr          $ra
    // 0x80161F7C: nop

    return;
    // 0x80161F7C: nop

;}
RECOMP_FUNC void func_8016202C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016202C: lui         $a0, 0x8023
    ctx->r4 = S32(0X8023 << 16);
    // 0x80162030: lw          $a0, -0x5BBC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5BBC);
    // 0x80162034: sll         $v1, $a0, 3
    ctx->r3 = S32(ctx->r4 << 3);
    // 0x80162038: addu        $v1, $v1, $a0
    ctx->r3 = ADD32(ctx->r3, ctx->r4);
    // 0x8016203C: sll         $v1, $v1, 3
    ctx->r3 = S32(ctx->r3 << 3);
    // 0x80162040: addu        $v1, $v1, $a0
    ctx->r3 = ADD32(ctx->r3, ctx->r4);
    // 0x80162044: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80162048: addu        $v1, $v1, $a0
    ctx->r3 = ADD32(ctx->r3, ctx->r4);
    // 0x8016204C: sll         $v0, $v1, 4
    ctx->r2 = S32(ctx->r3 << 4);
    // 0x80162050: subu        $v0, $v0, $v1
    ctx->r2 = SUB32(ctx->r2, ctx->r3);
    // 0x80162054: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80162058: subu        $v0, $v0, $a0
    ctx->r2 = SUB32(ctx->r2, ctx->r4);
    // 0x8016205C: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80162060: lui         $at, 0x801E
    ctx->r1 = S32(0X801E << 16);
    // 0x80162064: addu        $at, $at, $v0
    ctx->r1 = ADD32(ctx->r1, ctx->r2);
    // 0x80162068: lw          $v0, 0x6A34($at)
    ctx->r2 = MEM_W(ctx->r1, 0X6A34);
    // 0x8016206C: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    // 0x80162070: jr          $ra
    // 0x80162074: nop

    return;
    // 0x80162074: nop

;}
