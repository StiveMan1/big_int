#include "big_int.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


char bi_cmp_pos(const BIG_DIGIT *a, const BIG_DIGIT *b, unsigned al, unsigned bl) {
    if (al != bl) {
        if (al > bl) return 1;
        return -1;
    }
    for (BIG_SIZE i = al; i > 0;) {
        i--;
        if (a[i] != b[i]) {
            if (a[i] > b[i]) return 1;
            return -1;
        }
    }
    return 0;
}
void bi_add_pos(const BIG_DIGIT *a, const BIG_DIGIT *b, unsigned al, unsigned bl, bi_pt res) {
    if (res->digits == b || res->digits == a) {
        bi_pt temp = malloc(sizeof(bi_t));
        bi_init(temp);
        bi_add_pos(a, b, al, bl, temp);
        bi_set(res, temp);
        return bi_clear(temp);
    }
    BIG_DIGIT temp = 0;
    BIG_DIGIT temp2;
    bi_resize(res, al + 1);
    for (BIG_SIZE i = 0; i < al; i++) {
        temp2 = a[i] + temp;
        if (i < bl) temp2 += b[i];
        temp = (temp2 < a[i] || (temp2 == a[i] && (temp != 0 || (bl > i && b[i] != 0))));
        res->digits[i] = temp2;
    }
    res->digits[al] = temp;
    bi_fit(res);
}
void bi_sub_pos(const BIG_DIGIT *a, const BIG_DIGIT *b, unsigned al, unsigned bl, bi_pt res) {
    if (res->digits == b) {
        bi_pt temp = malloc(sizeof(bi_t));
        bi_init(temp);
        bi_sub_pos(a, b, al, bl, temp);
        bi_set(res, temp);
        return bi_clear(temp);
    }
    BIG_DIGIT temp = 0;
    BIG_DIGIT temp2;
    bi_resize(res, al);
    for (BIG_SIZE i = 0; i < al; i++) {
        temp2 = a[i] - temp;
        if (i < bl) temp2 -= b[i];
        temp = (temp2 > a[i] || (temp2 == a[i] && (temp != 0 || (bl > i && b[i] != 0))));
        res->digits[i] = temp2;
    }
    bi_fit(res);
}

void
bi_mul_pos(bi_pt res, const BIG_DIGIT *a, const BIG_DIGIT *b, const unsigned al, const unsigned bl) {
    BIG_SIZE n = al;
    if (bl > n) n = bl;
    if (al == 0 || bl == 0 || n == 0) return bi_resize(res, 0);
    if (n == 1) {
        BIG_TWO_DIGITS res1 = ((BIG_TWO_DIGITS)a[0] * (BIG_TWO_DIGITS)b[0]);
        bi_resize(res, 2);
        res->digits[0] = (BIG_DIGIT)(res1 % (1 << 16));
        res->digits[1] = (BIG_DIGIT)(res1 / (1 << 16));
        bi_fit(res);
        return;
    }
    BIG_SIZE mid = n / 2;
    if (al < mid) {
        bi_pt R = malloc(sizeof(bi_t));
        bi_pt L = malloc(sizeof(bi_t));
        bi_inits(L, R, NULL);
        bi_mul_pos(R, a, b, al, mid);
        bi_mul_pos(L, a, b + mid, al, bl - mid);
        bi_bool_ls(L, L, mid * BIG_DIGIT_LEN);
        bi_add(res, L, R);
        return bi_clears(L, R, NULL);
    } else if (bl < mid) {
        bi_pt R = malloc(sizeof(bi_t));
        bi_pt L = malloc(sizeof(bi_t));
        bi_inits(L, R, NULL);
        bi_mul_pos(R, a, b, mid, bl);
        bi_mul_pos(L, a + mid, b, al - mid, bl);
        bi_bool_ls(L, L, mid * BIG_DIGIT_LEN);
        bi_add(res, L, R);
        return bi_clears(L, R, NULL);
    }
    bi_pt S = malloc(sizeof(bi_t));
    bi_init(S);
    {
        bi_pt A = malloc(sizeof(bi_t));
        bi_pt B = malloc(sizeof(bi_t));
        bi_inits(A, B, NULL);
        if (al - mid >= mid) bi_add_pos(a + mid, a, al - mid, mid, A);
        else bi_add_pos(a, a + mid, mid, al - mid, A);
        if (bl - mid >= mid) bi_add_pos(b + mid, b, bl - mid, mid, B);
        else bi_add_pos(b, b + mid, mid, bl - mid, B);
        bi_mul_pos(S, A->digits, B->digits, A->size, B->size);
        bi_clears(A, B, NULL);
    }
    bi_pt R = malloc(sizeof(bi_t));
    bi_pt L = malloc(sizeof(bi_t));
    bi_inits(L, R, NULL);
    bi_mul_pos(R, a, b, mid, mid);
    bi_mul_pos(L, a + mid, b + mid, al - mid, bl - mid);
    bi_sub_pos(S->digits, R->digits, S->size, R->size, S);
    bi_sub_pos(S->digits, L->digits, S->size, L->size, S);
    bi_bool_ls(L, L, mid * BIG_DIGIT_LEN * 2);
    bi_bool_ls(S, S, mid * BIG_DIGIT_LEN);
    bi_add(res, L, S);
    bi_add(res, res, R);
    bi_clears(L, R, S, NULL);
}




void bi_add(bi_pt res, const bi_pt a, const bi_pt b) {
    BIG_DIGIT _positive = a->positive;
    if (a->positive ^ b->positive) {
        if (bi_cmp_pos(a->digits, b->digits, a->size, b->size) < 0) {
            bi_sub_pos(b->digits, a->digits, b->size, a->size, res);
            _positive = b->positive;
        } else {
            bi_sub_pos(a->digits, b->digits, a->size, b->size, res);
        }
    } else {
        if (bi_cmp_pos(a->digits, b->digits, a->size, b->size) < 0) {
            bi_add_pos(b->digits, a->digits, b->size, a->size, res);
        } else {
            bi_add_pos(a->digits, b->digits, a->size, b->size, res);
        }
    }
    res->positive = _positive;
}
void bi_sub(bi_pt res, const bi_pt a, const bi_pt b) {
    BIG_DIGIT _positive = a->positive;
    if (a->positive ^ b->positive) {
        if (bi_cmp_pos(a->digits, b->digits, a->size, b->size) < 0) {
            bi_add_pos(b->digits, a->digits, b->size, a->size, res);
        } else {
            bi_add_pos(a->digits, b->digits, a->size, b->size, res);
        }
    } else {
        if (bi_cmp_pos(a->digits, b->digits, a->size, b->size) < 0) {
            bi_sub_pos(b->digits, a->digits, b->size, a->size, res);
            _positive = !b->positive;
        } else {
            bi_sub_pos(a->digits, b->digits, a->size, b->size, res);
        }
    }
    res->positive = _positive;
}
void bi_mul(bi_pt res, const bi_pt a, const bi_pt b) {
    BIG_DIGIT positive = !(a->positive ^ b->positive);
    bi_mul_pos(res, a->digits, b->digits, a->size, b->size);
    res->positive = positive;
}
void bi_div(bi_pt res, const bi_pt a, const bi_pt b) {
    if (b->size > a->size || bi_cmp_pos(a->digits, b->digits, a->size, b->size) < 0) {
        bi_resize(res, 0);
        return;
    }
    bi_pt x = malloc(sizeof(bi_t));
    bi_pt r = malloc(sizeof(bi_t));
    bi_inits(x, r, NULL);
    BIG_SIZE i = a->size * BIG_DIGIT_LEN;
    BIG_SIZE mod;
    while (i > 0) {
        bi_bool_ls(x, x, 1);
        bi_bool_ls(r, r, 1);
        i--;
        mod = (i) % BIG_DIGIT_LEN;
        if (x->size == 0) bi_resize(x, 1);
        x->digits[0] |= ((a->digits[i / BIG_DIGIT_LEN] & (1 << mod)) >> mod);
        if (bi_cmp_pos(x->digits, b->digits, x->size, b->size) >= 0) {
            bi_sub_pos(x->digits, b->digits, x->size, b->size, x);
            if (r->size == 0) bi_resize(r, 1);
            r->digits[0] |= 1;
        }
    }
    r->positive = !(a->positive ^ b->positive);
    bi_set(res, r);
    bi_clears(x, r, NULL);
}
void bi_mod(bi_pt res, const bi_pt a, const bi_pt b) {
    if (b->size > a->size || bi_cmp_pos(a->digits, b->digits, a->size, b->size) < 0) {
        bi_set(res, a);
        while (!res->positive) bi_add(res, res, b);
        return;
    }
    bi_pt x = malloc(sizeof(bi_t));
    bi_init(x);
    BIG_SIZE i = a->size * BIG_DIGIT_LEN;
    BIG_SIZE mod;
    while (i > 0) {
        bi_bool_ls(x, x, 1);
        i--;
        mod = (i) % BIG_DIGIT_LEN;
        if (x->size == 0) bi_resize(x, 1);
        x->digits[0] |= ((a->digits[i / BIG_DIGIT_LEN] & (1 << mod)) >> mod);
        bi_fit(x);
        if (bi_cmp_pos(x->digits, b->digits, x->size, b->size) >= 0) {
            bi_sub_pos(x->digits, b->digits, x->size, b->size, x);
        }
    }
    x->positive = a->positive;
    while (!x->positive) bi_add(x, x, b);
    bi_set(res, x);
    bi_clear(x);
}
void bi_pow(bi_pt res, const bi_pt a, const bi_pt b) {
    bi_pt a1 = malloc(sizeof(bi_t));
    bi_init_set(a1, a);
    bi_set_ui(res, 1);
    BIG_SIZE size = b->size * BIG_DIGIT_LEN;
    BIG_SIZE _size = 0;
    for (BIG_SIZE i = 0; i < size; i++)
        if (b->digits[i / BIG_DIGIT_LEN] & (1 << (i % BIG_DIGIT_LEN))) _size = i;
    for (BIG_SIZE i = 0; i <= _size; i++) {
        if (b->digits[i / BIG_DIGIT_LEN] & (1 << (i % BIG_DIGIT_LEN))) {
            bi_mul(res, res, a1);
        }
        bi_mul(a1, a1, a1);
    }
}
void bi_pow_mod(bi_pt res, const bi_pt a, const bi_pt b, const bi_pt mod) {
    bi_pt a1 = malloc(sizeof(bi_t));
    bi_init_set(a1, a);
    bi_set_ui(res, 1);
    BIG_SIZE size = b->size * BIG_DIGIT_LEN;
    BIG_SIZE _size = 0;
    for (BIG_SIZE i = 0; i < size; i++)
        if (b->digits[i / BIG_DIGIT_LEN] & (1 << (i % BIG_DIGIT_LEN))) _size = i;
    for (BIG_SIZE i = 0; i <= _size; i++) {
        if (b->digits[i / BIG_DIGIT_LEN] & (1 << (i % BIG_DIGIT_LEN))) {
            bi_mul(res, res, a1);
            bi_mod(res, res, mod);
        }
        bi_mul(a1, a1, a1);
        bi_mod(a1, a1, mod);
    }
    bi_clear(a1);
}
char bi_cmp(const bi_pt a, const bi_pt b) {
    if (a->positive ^ b->positive) {
        if (a->positive) {
            return 1;
        }
        return -1;
    }
    char res = bi_cmp_pos(a->digits, b->digits, a->size, b->size);
    if (!a->positive) {
        res *= -1;
    }
    return res;
}
void bi_inverse(bi_pt res, const bi_pt a, const bi_pt mod) {
    bi_pt r_p = malloc(sizeof(bi_t));
    bi_pt r_n = malloc(sizeof(bi_t));
    bi_pt s_n = malloc(sizeof(bi_t));
    bi_pt q = malloc(sizeof(bi_t));
    bi_pt temp = malloc(sizeof(bi_t));
    bi_inits(r_p, r_n, s_n, q, temp, NULL);
    bi_set(r_p, a);
    bi_set(r_n, mod);
    bi_set_ui(res, 1);
    bi_set_ui(s_n, 0);
    while (r_n->size > 0) {
        bi_div(q, r_p, r_n);

        bi_set(temp, r_n);
        bi_mul(r_n, r_n, q);
        bi_sub(r_n, r_p, r_n);
        bi_set(r_p, temp);

        bi_set(temp, s_n);
        bi_mul(s_n, s_n, q);
        bi_sub(s_n, res, s_n);
        bi_set(res, temp);
    }
    while (!res->positive) bi_add(res, res, mod);
    bi_clears(r_p, r_n, s_n, q, temp, NULL);
}