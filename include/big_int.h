#ifndef BIG_INT_H
#define BIG_INT_H

#define BIG_DIGIT unsigned short
#define BIG_DIGIT_LEN 16
#define BIG_TWO_DIGITS unsigned
#define BIG_SIZE unsigned

#include <malloc.h>
#include <strings.h>

typedef struct {
    BIG_DIGIT *digits;
    BIG_SIZE size;
    BIG_SIZE max_size;
    unsigned char positive;
} bi_t;


#define bi_pt bi_t *

void bi_init(bi_pt res);

void bi_inits(bi_pt res, ...);

void bi_init_set(bi_pt res, const bi_pt a);

void bi_resize(bi_pt res, BIG_SIZE size);

void bi_set(bi_pt res, const bi_pt a);

void bi_fit(bi_pt res);

void bi_clear(bi_pt a);

void bi_clears(bi_pt a, ...);

void bi_print(const BIG_DIGIT *a, BIG_SIZE size);

void bi_print_hex(const bi_pt a);

void bi_set_ui(bi_pt res, unsigned a);

unsigned bi_get_ui(const bi_pt a);

void bi_set_si(bi_pt res, int a);

unsigned bi_get_si(const bi_pt a);

void bi_left_shift(bi_pt res, const bi_pt a, BIG_SIZE num);

void bi_right_shift(bi_pt res, const bi_pt a, BIG_SIZE num);

void bi_add(bi_pt res, const bi_pt a, const bi_pt b);

void bi_sub(bi_pt res, const bi_pt a, const bi_pt b);

void bi_mul(bi_pt res, const bi_pt a, const bi_pt b);

void bi_div(bi_pt res, const bi_pt a, const bi_pt b);

void bi_mod(bi_pt res, const bi_pt a, const bi_pt b);

void bi_add_ui(bi_pt res, const bi_pt a, unsigned b);

void bi_sub_ui(bi_pt res, const bi_pt a, unsigned b);

void bi_mul_ui(bi_pt res, const bi_pt a, unsigned b);

void bi_div_ui(bi_pt res, const bi_pt a, unsigned b);

void bi_mod_ui(bi_pt res, const bi_pt a, unsigned b);

void bi_set_str_16(bi_pt res, const char str[], size_t size);

char *bi_get_str_16(const bi_pt res, size_t *size);

void bi_pow(bi_pt res, const bi_pt a, const bi_pt b);

void bi_pow_mod(bi_pt res, const bi_pt a, const bi_pt b, const bi_pt mod);

void bi_pow_ui(bi_pt res, const bi_pt a, unsigned b);

char bi_cmp(const bi_pt a, const bi_pt b);

void bi_random(bi_pt res, BIG_SIZE size);

void bi_inverse(bi_pt res, const bi_pt a, const bi_pt mod);


#endif //BIG_INT_H
