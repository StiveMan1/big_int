#include "big_int.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


void bi_init(bi_pt res) {
    res->size = 0;
    res->max_size = 0;
    res->digits = malloc(0);
    res->positive = 1;
}

void bi_inits(bi_pt arg, ...) {
    va_list args_list;
    va_start(args_list, arg);

    for (bi_pt res = arg; res != NULL; res = va_arg(args_list, bi_pt )) {
        res->size = 0;
        res->max_size = 0;
        res->digits = malloc(0);
        res->positive = 1;
    }
    va_end(args_list);
}

void bi_init_set(bi_pt res, const bi_pt a) {
    res->max_size = res->size = a->size;
    res->positive = a->positive;
    res->digits = malloc(sizeof(BIG_DIGIT) * a->size);
    for (BIG_SIZE i = 0; i < res->size; i++) {
        res->digits[i] = a->digits[i];
    }
}

void bi_set_array(bi_pt res, BIG_DIGIT *array, BIG_SIZE size) {
    free(res->digits);
    res->digits = array;
    res->size = res->max_size = size;
}

void bi_resize(bi_pt res, BIG_SIZE size) {
    res->size = size;
    if (res->max_size < size) {
        res->max_size = size;
        free(res->digits);
        res->digits = malloc(sizeof(BIG_DIGIT) * size);
    }
}

void bi_set(bi_pt res, const bi_pt a) {
    bi_resize(res, a->size);
    res->positive = a->positive;
    for (BIG_SIZE i = 0; i < res->size; i++) {
        res->digits[i] = a->digits[i];
    }
}

void bi_fit(bi_pt res) {
    while (res->size != 0 && res->digits[res->size - 1] == 0) {
        res->size--;
    }
    if(res->size == 0) res->positive = 1;
}

void bi_clear(bi_pt res) {
    free(res->digits);
    free(res);
}

void bi_clears(bi_pt arg, ...) {
    va_list args_list;
    va_start(args_list, arg);

    for (bi_pt res = arg; res != NULL; res = va_arg(args_list, bi_pt )) {
        free(res->digits);
        free(res);
    }
    va_end(args_list);
}

void bi_set_ui(bi_pt res, unsigned a) {
    bi_resize(res, 2);
    res->positive = 1;
    res->digits[0] = (BIG_DIGIT) (a % 65536);
    res->digits[1] = (BIG_DIGIT) (a / 65536);
    bi_fit(res);
}

unsigned bi_get_ui(const bi_pt a) {
    unsigned result = 0;
    if(a->size > 1) {
        result = a->digits[1];
        result <<= BIG_DIGIT_LEN;
    }
    if(a->size>0) result += a->digits[0];
    return result;
}

void bi_set_si(bi_pt res, int a) {
    bi_resize(res, 2);
    unsigned temp;
    if (a < 0) {
        res->positive = 0;
        temp = -a;
    } else {
        res->positive = 1;
        temp = a;
    }
    res->digits[0] = (BIG_DIGIT) (temp % 65536);
    res->digits[1] = (BIG_DIGIT) (temp / 65536);
    bi_fit(res);
}

int bi_get_si(const bi_pt a) {
    unsigned result = 0;
    if(a->size > 1) {
        result = a->digits[1];
        result <<= BIG_DIGIT_LEN;
    }
    if(a->size>0) result += a->digits[0];
    if (!a->positive) {
        result *= -1;
    }
    return result;
}

char get_char_16(unsigned code) {
    if (code >= 0 && code <= 9) {
        return (char) (code + '0');
    }

    return (char) (code - 10 + 'a');
}

BIG_DIGIT set_char_16(char x) {
    if (x >= '0' && x <= '9') {
        return (BIG_DIGIT) (x - '0');
    }
    if (x >= 'a' && x <= 'f') {
        return (BIG_DIGIT) (x - 'a' + 10);
    }
    if (x >= 'A' && x <= 'F') {
        return (BIG_DIGIT) (x - 'A' + 10);
    }
    return (BIG_DIGIT) 0;
}

void bi_print(const BIG_DIGIT *a, BIG_SIZE size) {
    for (BIG_SIZE i = size; i > 0;) {
        i--;
        printf("%c", get_char_16(a[i] / 4096));
        printf("%c", get_char_16((a[i] / 256) % 16));
        printf("%c", get_char_16((a[i] / 16) % 16));
        printf("%c", get_char_16(a[i] % 16));
    }
    printf("\n");
}

void bi_print_hex(const bi_pt res) {
    for (BIG_SIZE i = res->size; i > 0;) {
        i--;
        printf("%c", get_char_16(res->digits[i] / 4096));
        printf("%c", get_char_16((res->digits[i] / 256) % 16));
        printf("%c", get_char_16((res->digits[i] / 16) % 16));
        printf("%c", get_char_16(res->digits[i] % 16));
    }
    printf("\n");
}

void bi_left_shift(bi_pt res, const bi_pt a, BIG_SIZE num) {
    BIG_SIZE newchars = num / BIG_DIGIT_LEN + (BIG_SIZE) (num % BIG_DIGIT_LEN != 0) + a->size;
    BIG_SIZE shift_m = num / BIG_DIGIT_LEN + (BIG_SIZE) (num % BIG_DIGIT_LEN != 0);
    BIG_DIGIT *array = malloc(sizeof(BIG_DIGIT) * (newchars));
    for (BIG_SIZE i = 0; i < shift_m; i++) {
        array[i] = 0;
    }
    for (BIG_SIZE i = 0; i < a->size; i++) {
        array[i + shift_m] = a->digits[i];
    }
    if (num % BIG_DIGIT_LEN != 0) {
        BIG_DIGIT mask_m = (1 << (BIG_DIGIT_LEN - num % BIG_DIGIT_LEN));
        BIG_DIGIT mask_l = mask_m - 1;
        for (BIG_SIZE i = 0; i < newchars; i++) {
            array[i] >>= (BIG_DIGIT_LEN - num % BIG_DIGIT_LEN);
            if (i + 1 < newchars) {
                array[i] |= ((array[i + 1] & mask_l) << num % BIG_DIGIT_LEN);
            }
        }
    }
    bi_set_array(res, array, newchars);
    res->positive = a->positive;
    bi_fit(res);
}

void bi_right_shift(bi_pt res, const bi_pt a, BIG_SIZE num) {
    BIG_SIZE newchars = a->size - num / BIG_DIGIT_LEN;
    if (num / BIG_DIGIT_LEN > a->size) newchars = 0;
    BIG_SIZE shift_m = num / BIG_DIGIT_LEN;
    BIG_DIGIT *array = malloc(sizeof(BIG_DIGIT) * (newchars));

    for (BIG_SIZE i = 0; i < newchars; i++) {
        array[i] = a->digits[i + shift_m];
    }
    if (num % BIG_DIGIT_LEN != 0) {
        BIG_DIGIT mask_m = (1 << (num % BIG_DIGIT_LEN));
        BIG_DIGIT mask_l = mask_m - 1;
        for (BIG_SIZE i = 0; i < newchars; i++) {
            array[i] >>= (num % BIG_DIGIT_LEN);
            if (i + 1 < newchars) {
                array[i] |= ((array[i + 1] & mask_l) << (BIG_DIGIT_LEN - num % BIG_DIGIT_LEN));
            }
        }
    }
    bi_set_array(res, array, newchars);
    res->positive = a->positive;
    bi_fit(res);
}

// O(n)
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

// O(n)
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

// O(n)
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

// O(n^1.58)
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
        bi_left_shift(L, L, mid * BIG_DIGIT_LEN);
        bi_add(res, L, R);
        return bi_clears(L, R, NULL);
    } else if (bl < mid) {
        bi_pt R = malloc(sizeof(bi_t));
        bi_pt L = malloc(sizeof(bi_t));
        bi_inits(L, R, NULL);
        bi_mul_pos(R, a, b, mid, bl);
        bi_mul_pos(L, a + mid, b, al - mid, bl);
        bi_left_shift(L, L, mid * BIG_DIGIT_LEN);
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
    bi_left_shift(L, L, mid * BIG_DIGIT_LEN * 2);
    bi_left_shift(S, S, mid * BIG_DIGIT_LEN);
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

//O(n^2)
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
        bi_left_shift(x, x, 1);
        bi_left_shift(r, r, 1);
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

//O(n^2)
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
        bi_left_shift(x, x, 1);
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

void bi_add_ui(bi_pt res, const bi_pt a, unsigned b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, b);
    bi_add(res, a, b1);
}

void bi_sub_ui(bi_pt res, const bi_pt a, unsigned b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, b);
    bi_sub(res, a, b1);
}

void bi_mul_ui(bi_pt res, const bi_pt a, unsigned b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, b);
    bi_mul(res, a, b1);
}

void bi_div_ui(bi_pt res, const bi_pt a, unsigned b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, b);
    bi_div(res, a, b1);
}

void bi_mod_ui(bi_pt res, const bi_pt a, unsigned b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, b);
    bi_mod(res, a, b1);
}


void bi_set_str_16(bi_pt res, const char str[], const size_t size) {
    BIG_SIZE _size = size / 4 + (BIG_SIZE) (size % 4 != 0);
    bi_resize(res, _size);
    for (BIG_SIZE i = 0; i < _size; i++) res->digits[i] = 0;
    BIG_SIZE _x;
    for (BIG_SIZE i = size, j = 0; i > 0; j++) {
        i--;
        _x = i / 4;
        res->digits[_x] <<= 4;
        res->digits[_x] |= set_char_16(str[j]);
    }
    bi_fit(res);
}

char *bi_get_str_16(const bi_pt res, size_t *size) {
    *size = (res->size * 4);
    char *result = malloc(sizeof(char) * *size);
    for (BIG_SIZE i = res->size, j = 0; i > 0;) {
        i--;
        result[j++] = get_char_16(res->digits[i] / 4096);
        result[j++] = get_char_16((res->digits[i] / 256) % 16);
        result[j++] = get_char_16((res->digits[i] / 16) % 16);
        result[j++] = get_char_16(res->digits[i] % 16);
    }
    return result;
}

//O(n^2.58)
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

//O(n^3)
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

void bi_pow_ui(bi_pt res, const bi_pt a, unsigned b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, b);
    bi_pow(res, a, b1);
    bi_clear(b1);
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

void bi_random(bi_pt res, const BIG_SIZE size) {
    bi_resize(res, size);
    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < size; i++) {
        res->digits[i] = (BIG_DIGIT) (rand() % 256);
    }
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

