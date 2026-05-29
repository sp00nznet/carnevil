? func_80141210(? *, u16, s32);                     /* extern */
s32 func_800CC47C(s16 arg0);                        /* static */
extern ? D_8016F274;
extern ? D_8016F2A4;
extern ? D_801E3880;
extern s16 D_801E3882;
extern ? D_801E3884;
extern ? D_801E3886;
extern s16 D_801E6360;
extern s16 D_80212250;
extern s16 D_80212252;

s16 func_800CC218(void *arg0) {
    ? *temp_s0;
    ? *var_a1_2;
    ? *var_a3;
    s16 var_v1;
    s32 (*temp_v0_2)(? *, ? *, ?, ? *);
    s32 temp_t0;
    s32 var_a2_2;
    u16 *temp_a0_2;
    u16 temp_a0;
    u16 temp_v0;
    u16 temp_v1;
    u16 var_a1;
    void *var_a2;

    if (D_80212250 < 0x80) {
        var_a1 = (u16) D_801E6360;
        if (*(&D_801E3880 + (D_801E6360 * 0x28)) != 0xFFFF) {
            do {
                temp_v0 = (var_a1 + 1) & 0x7F;
                var_a1 = temp_v0;
            } while (*(&D_801E3880 + (temp_v0 * 0x28)) != 0xFFFF);
        }
        D_801E6360 = (var_a1 + 1) & 0x7F;
        temp_s0 = ((s16) var_a1 * 0x28) + &D_801E3880;
        var_a3 = temp_s0;
        var_a2 = arg0;
        temp_t0 = var_a2 + 0x20;
        do {
            var_a3->unk0 = (s32) var_a2->unk0;
            var_a3->unk4 = (s32) var_a2->unk4;
            var_a3->unk8 = (s32) var_a2->unk8;
            var_a3->unkC = (s32) var_a2->unkC;
            var_a2 += 0x10;
            var_a3 += 0x10;
        } while (var_a2 != temp_t0);
        var_a3->unk0 = (s32) var_a2->unk0;
        var_a3->unk4 = (s32) var_a2->unk4;
        var_a1_2 = &D_801E3880;
        temp_s0->unk2 = (s16) var_a1;
        temp_s0->unk18 = 0;
        temp_s0->unk1C = 0;
        temp_s0->unk20 = 0;
        temp_s0->unk8 = NULL;
        if (D_801E3882 != -2) {
            var_a3 = &D_801E3880;
loop_7:
            temp_v1 = var_a1_2->unk0;
            if ((temp_v1 == 0xFFFE) || (temp_v1 >= (u16) temp_s0->unk0)) {
                var_v1 = var_a1_2->unk4;
                temp_a0 = (u16) var_a1_2->unk4;
                if (var_v1 != -2) {
                    var_a1_2 = (var_v1 * 0x28) + &D_801E3880;
                    if ((s16) temp_a0 == -2) {
                        goto block_11;
                    }
                    goto loop_7;
                }
            } else {
                goto block_11;
            }
        } else {
block_11:
            var_v1 = var_a1_2->unk4;
        }
        if (var_v1 == -2) {
            temp_s0->unk4 = var_v1;
            temp_s0->unk6 = (u16) var_a1_2->unk2;
            var_a1_2->unk4 = (s16) var_a1;
        } else {
            temp_s0->unk6 = (u16) var_a1_2->unk6;
            temp_s0->unk4 = (s16) var_a1_2->unk2;
            *(&D_801E3884 + ((s16) var_a1_2->unk6 * 0x28)) = (s16) var_a1;
            var_a1_2->unk6 = (u16) (s16) var_a1;
        }
        temp_v0_2 = temp_s0->unkC;
        var_a2_2 = 0;
        if (temp_v0_2 != NULL) {
            var_a2_2 = temp_v0_2(temp_s0, var_a1_2, 0, var_a3);
        }
        temp_a0_2 = temp_s0->unk8;
        if ((temp_a0_2 != NULL) && (temp_s0->unk0 != 0x1000)) {
            *temp_a0_2 = (u16) temp_s0->unk2;
        }
        D_80212250 = (u16) D_80212250 + 1;
        if (var_a2_2 == 0) {
            return temp_s0->unk2;
        }
        func_80141210(&D_8016F274, temp_s0->unk0, var_a2_2);
        func_800CC47C(temp_s0->unk2);
        goto block_23;
    }
block_23:
    return 0;
}

s32 func_800CC47C(s16 arg0) {
    ? (*temp_v0)(void *, s16);
    s16 temp_v1;
    s32 var_v0;
    void *temp_s0;

    temp_s0 = (arg0 * 0x28) + &D_801E3880;
    if (D_80212250 >= 2) {
        if (arg0 == 0) {
            func_80141210(&D_8016F2A4, (u16) arg0);
            goto block_3;
        }
        var_v0 = 0;
        if (temp_s0->unk0 != 0xFFFF) {
            temp_v0 = temp_s0->unk20;
            if (temp_v0 != NULL) {
                temp_v0(temp_s0, arg0);
            }
            if (temp_s0->unk2 == D_80212252) {
                D_80212252 = 0;
            }
            temp_v1 = temp_s0->unk4;
            if (temp_v1 == -2) {
                *(&D_801E3884 + (temp_s0->unk6 * 0x28)) = -2;
                var_v0 = 1;
            } else {
                *(&D_801E3884 + (temp_s0->unk6 * 0x28)) = temp_v1;
                *(&D_801E3886 + (temp_s0->unk4 * 0x28)) = (u16) temp_s0->unk6;
                var_v0 = 1;
            }
            *(&D_801E3880 + (arg0 * 0x28)) = 0xFFFF;
            D_80212250 = (u16) D_80212250 - 1;
        }
        return var_v0;
    }
block_3:
    return 0;
}
