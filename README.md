# big_int Library for C

This Library writen for C programming language in standard processor without using GPU resources.


##Installing
First install this library to your project then write same code as this.
In this example library located in "lib" directory.
```CMake
#Add library to compilation 
add_subdirectory(lib/big_int)
link_directories(lib/big_int)


add_executable(${PROJECT_NAME} ...)

#Link library to your project
target_link_libraries(${PROJECT_NAME} PUBLIC big_int)
```
Then to use this library in the code just write 
```C
#include <big_int.h>
// or
#include "big_int.h"
```

# Usage

##Initialize a variable
Here can be problem with double Initializing variables. Does not return the error.
```C
bi_pt num = malloc(sizeof(bi_t));
bi_init(num); // Using to inti only one variable

bi_pt num1 = malloc(sizeof(bi_t));
bi_pt num2 = malloc(sizeof(bi_t));
bi_inits(num1, num2, NULL); // must end with null pointer
```

##Deallocate a variable
Here can be problem with double Clearing variables. Does not return the error but may be broke while working.
```C
bi_clear(num); // Using to deallocate only one variable
bi_clears(num1, num2, NULL); // must end with null pointer
```


##Standard functions
In this functions in the first place located pointer for big_int where you get result and does not return anything
```C
void bi_add(bi_pt res, const bi_pt a, const bi_pt b); // res = a + b
void bi_sub(bi_pt res, const bi_pt a, const bi_pt b); // res = a - b
void bi_mul(bi_pt res, const bi_pt a, const bi_pt b); // res = a * b
void bi_div(bi_pt res, const bi_pt a, const bi_pt b); // res = a / b
void bi_mod(bi_pt res, const bi_pt a, const bi_pt b); // res = a % b
void bi_pow(bi_pt res, const bi_pt a, const bi_pt b); // res = a ^ b
void bi_pow_mod(bi_pt res, const bi_pt a, const bi_pt b, const bi_pt mod); // res = (a ^ b) % mod
void bi_inverse(bi_pt res, const bi_pt a, const bi_pt mod); // math inverse by module
void bi_left_shift(bi_pt res, const bi_pt a, BIG_SIZE num); // res = a << num
void bi_right_shift(bi_pt res, const bi_pt a, BIG_SIZE num); // res = b >> num
```
This Standard functions can use unsigned int as second incoming variable
```C
void bi_add_ui(bi_pt res, const bi_pt a, unsigned b); // res = a + b
void bi_sub_ui(bi_pt res, const bi_pt a, unsigned b); // res = a - b
void bi_mul_ui(bi_pt res, const bi_pt a, unsigned b); // res = a * b
void bi_div_ui(bi_pt res, const bi_pt a, unsigned b); // res = a / b
void bi_mod_ui(bi_pt res, const bi_pt a, unsigned b); // res = a % b
void bi_pow_ui(bi_pt res, const bi_pt a, unsigned b); // res = a ^ b
```
##Compare function of two big ints
Returns 1 if a > b, -1 is a < b and 0 is a = b.
```C
char bi_cmp(const bi_pt a, const bi_pt b);
```
## Get and set value

Set value of variable. In future will add other types and bases.
```C
void bi_set_ui(bi_pt res, unsigned a);
void bi_set_si(bi_pt res, int a);
// In this function you need to give string of hexadecimal number and this string size
void bi_set_str_16(bi_pt res, const char str[], const size_t size);
``` 
Get value of variable. In future will add other types and bases.
```C
unsigned bi_get_ui(const bi_pt a);
int bi_get_si(const bi_pt a);
// This function return string and change the data of pointer size.
// The resulting string in hexadecimal type.
char *bi_get_str_16(const bi_pt res, size_t *size);
```

