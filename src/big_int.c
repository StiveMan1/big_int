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
void bi_set(bi_pt res, const bi_pt a) {
    bi_resize(res, a->size);
    res->positive = a->positive;
    for (BIG_SIZE i = 0; i < res->size; i++) {
        res->digits[i] = a->digits[i];
    }
}
void bi_resize(bi_pt res, BIG_SIZE size) {
    res->size = size;
    if (res->max_size < size) {
        res->max_size = size;
        free(res->digits);
        res->digits = malloc(sizeof(BIG_DIGIT) * size);
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
void bi_random(bi_pt res, const BIG_SIZE size) {
    bi_resize(res, size);
    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < size; i++) {
        res->digits[i] = (BIG_DIGIT) (rand() % 256);
    }
}


void bi_set_array(bi_pt res, BIG_DIGIT *array, BIG_SIZE size) {
    free(res->digits);
    res->digits = array;
    res->size = res->max_size = size;
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


void bi_bool_ls(bi_pt res, const bi_pt a, BIG_SIZE num) {
    BIG_SIZE nl = num / BIG_DIGIT_LEN + (BIG_SIZE) (num % BIG_DIGIT_LEN != 0) + a->size;
    BIG_SIZE shift_m = num / BIG_DIGIT_LEN + (BIG_SIZE) (num % BIG_DIGIT_LEN != 0);
    BIG_DIGIT *array = malloc(sizeof(BIG_DIGIT) * (nl));
    for (BIG_SIZE i = 0; i < shift_m; i++) {
        array[i] = 0;
    }
    for (BIG_SIZE i = 0; i < a->size; i++) {
        array[i + shift_m] = a->digits[i];
    }
    if (num % BIG_DIGIT_LEN != 0) {
        BIG_DIGIT mask_m = (1 << (BIG_DIGIT_LEN - num % BIG_DIGIT_LEN));
        BIG_DIGIT mask_l = mask_m - 1;
        for (BIG_SIZE i = 0; i < nl; i++) {
            array[i] >>= (BIG_DIGIT_LEN - num % BIG_DIGIT_LEN);
            if (i + 1 < nl) {
                array[i] |= ((array[i + 1] & mask_l) << num % BIG_DIGIT_LEN);
            }
        }
    }
    bi_set_array(res, array, nl);
    res->positive = a->positive;
    bi_fit(res);
}
void bi_bool_rs(bi_pt res, const bi_pt a, BIG_SIZE num) {
    BIG_SIZE nl = a->size - num / BIG_DIGIT_LEN;
    if (num / BIG_DIGIT_LEN > a->size) nl = 0;
    BIG_SIZE shift_m = num / BIG_DIGIT_LEN;
    BIG_DIGIT *array = malloc(sizeof(BIG_DIGIT) * (nl));

    for (BIG_SIZE i = 0; i < nl; i++) {
        array[i] = a->digits[i + shift_m];
    }
    if (num % BIG_DIGIT_LEN != 0) {
        BIG_DIGIT mask_m = (1 << (num % BIG_DIGIT_LEN));
        BIG_DIGIT mask_l = mask_m - 1;
        for (BIG_SIZE i = 0; i < nl; i++) {
            array[i] >>= (num % BIG_DIGIT_LEN);
            if (i + 1 < nl) {
                array[i] |= ((array[i + 1] & mask_l) << (BIG_DIGIT_LEN - num % BIG_DIGIT_LEN));
            }
        }
    }
    bi_set_array(res, array, nl);
    res->positive = a->positive;
    bi_fit(res);
}
void bi_bool_and(bi_pt res, const bi_pt a, const bi_pt b){
    bi_pt temp = malloc(sizeof(bi_t));
    bi_init(temp);
    BIG_SIZE l = a->size;
    if(b->size > l) l = b->size;
    bi_resize(temp, l);
    for (BIG_SIZE i = 0; i < l; i++) {
        if (i < b->size && i < a->size) {
            temp->digits[i] = (a->digits[i] & b->digits[i]);
        }else{
            temp->digits[i] = 0;
        }
    }
    bi_fit(temp);
    bi_set(res, temp);
    bi_clear(temp);
}
void bi_bool_or(bi_pt res, const bi_pt a, const bi_pt b){
    bi_pt temp = malloc(sizeof(bi_t));
    bi_init(temp);
    BIG_SIZE l = a->size;
    if(b->size > l) l = b->size;
    bi_resize(temp, l);
    for (BIG_SIZE i = 0; i < l; i++) {
        if (i < b->size && i < a->size) {
            temp->digits[i] = (a->digits[i] | b->digits[i]);
        }else{
            temp->digits[i] = 0;
        }
    }
    bi_fit(temp);
    bi_set(res, temp);
    bi_clear(temp);
}
void bi_bool_xor(bi_pt res, const bi_pt a, const bi_pt b){
    bi_pt temp = malloc(sizeof(bi_t));
    bi_init(temp);
    BIG_SIZE l = a->size;
    if(b->size > l) l = b->size;
    bi_resize(temp, l);
    for (BIG_SIZE i = 0; i < l; i++) {
        if (i < b->size && i < a->size) {
            temp->digits[i] = (a->digits[i] ^ b->digits[i]);
        }else{
            temp->digits[i] = 0;
        }
    }
    bi_fit(temp);
    bi_set(res, temp);
    bi_clear(temp);
}