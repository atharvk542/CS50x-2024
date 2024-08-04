#include <stdio.h>

int main(void)
{
    int n = 50;

    // pointer - a variable that stores an address in memory. 0xsomething
    // data type then * - declares a pointer
    // * then name of pointer - go INTO the address represented by the pointer
    // & - tell me the address, NOT what is in the address
    // can also be written as int* p, makes more sense but less conventional

    int *p = &n;
    // printf("%i\n", *p); - prints out 50

    char *s = "HI!";
    // s is just a pointer to the actual string, no need to use
    // an ampersand since s already has the address stored
    // printf("%p\n", s);

    // need to use an ampersand since s[0] is a char
    // printf("%p\n", &s[0]);
    // printf("%p\n", &s[1]);
    // printf("%p\n", &s[2]);
    // printf("%p\n", &s[3]);

    // prints out what s is storing, the first character of HI!
    printf("%c", *s);
    printf("%c", *(s + 1));
    printf("%c\n", *(s + 2));
}
