#include "big_int.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void bi_init_set_si(bi_pt res, int a){
    res->max_size = 0;
    res->positive = 1;
    res->size = 2;
    res->digits = malloc(sizeof(BIG_DIGIT) * 2);
    if (a < 0) {
        res->positive = 0;
        a = -a;
    }
    res->digits[0] = (BIG_DIGIT) (a % 65536);
    res->digits[1] = (BIG_DIGIT) (a / 65536);
    bi_fit(res);
}
void bi_set_si(bi_pt res, int a) {
    bi_resize(res, 2);
    res->positive = 1;
    if (a < 0) {
        res->positive = 0;
        a = -a;
    }
    res->digits[0] = (BIG_DIGIT) (a % 65536);
    res->digits[1] = (BIG_DIGIT) (a / 65536);
    bi_fit(res);
}
int bi_get_si(const bi_pt a) {
    int result = 0;
    if (a->size > 1) {
        result = a->digits[1];
        result <<= BIG_DIGIT_LEN;
    }
    if (a->size > 0) result += a->digits[0];
    if (!a->positive) {
        result *= -1;
    }
    return result;
}


void bi_add_si(bi_pt res, const bi_pt a, int b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_si(b1, b);
    bi_add(res, a, b1);
}
void bi_sub_si(bi_pt res, const bi_pt a, int b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_si(b1, b);
    bi_sub(res, a, b1);
}
void bi_mul_si(bi_pt res, const bi_pt a, int b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_si(b1, b);
    bi_mul(res, a, b1);
}
void bi_div_si(bi_pt res, const bi_pt a, int b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_si(b1, b);
    bi_div(res, a, b1);
}
void bi_mod_si(bi_pt res, const bi_pt a, int b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_si(b1, b);
    bi_mod(res, a, b1);
}
void bi_pow_si(bi_pt res, const bi_pt a, int b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_si(b1, b);
    bi_pow(res, a, b1);
    bi_clear(b1);
}
void bi_pow_mod_si(bi_pt res, const bi_pt a, int b, int mod) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_pt mod1 = malloc(sizeof(bi_t));
    bi_inits(b1, mod1, NULL);
    bi_set_si(b1, b);
    bi_set_si(mod1, mod);
    bi_pow_mod(res, a, b1, mod1);
    bi_clears(b1, mod, NULL);
}
char bi_cmp_si(const bi_pt a, int b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_si(b1, b);
    char result = bi_cmp(a, b1);
    bi_clear(b1);
    return result;
}
void bi_inverse_si(bi_pt res, const bi_pt a, int mod) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_si(b1, mod);
    bi_inverse(res, a, b1);
    bi_clear(b1);
}