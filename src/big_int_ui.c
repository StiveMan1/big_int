#include "big_int.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void bi_init_set_ui(bi_pt res, unsigned a){
    res->max_size = 0;
    res->positive = 1;
    res->size = 2;
    res->digits = malloc(sizeof(BIG_DIGIT) * 2);
    res->digits[0] = (BIG_DIGIT) (a % 65536);
    res->digits[1] = (BIG_DIGIT) (a / 65536);
    bi_fit(res);
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
void bi_pow_ui(bi_pt res, const bi_pt a, unsigned b) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, b);
    bi_pow(res, a, b1);
    bi_clear(b1);
}
void bi_pow_mod_ui(bi_pt res, const bi_pt a, unsigned b, unsigned mod) {
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_pt mod1 = malloc(sizeof(bi_t));
    bi_inits(b1, mod1, NULL);
    bi_set_ui(b1, b);
    bi_set_ui(mod1, mod);
    bi_pow_mod(res, a, b1, mod1);
    bi_clears(b1, mod, NULL);
}
char bi_cmp_ui(const bi_pt a, unsigned b){
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, b);
    char result = bi_cmp(a, b1);
    bi_clear(b1);
    return result;
}
void bi_inverse_ui(bi_pt res, const bi_pt a, unsigned mod){
    bi_pt b1 = malloc(sizeof(bi_t));
    bi_init(b1);
    bi_set_ui(b1, mod);
    bi_inverse(res, a, b1);
    bi_clear(b1);
}