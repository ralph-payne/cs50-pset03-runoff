/*
Program below compares two strings
Initial code is from this thread:
https://stackoverflow.com/questions/12136329/how-does-strcmp-work

I have edited it to include fgets so that the two strings are taken from inputs from the user
*/

#include <stdio.h>

// Declare function prototype for own version of string compare
int my_str_cmp (const char *s1, const char *s2);

int main(void) {

    int return_value_from_function;

    char str1[100], str2[100];

    printf("1st word: ");
    fgets(str1, sizeof(str1), stdin);

    printf("2nd word: ");
    fgets(str2, sizeof(str2), stdin);

    return_value_from_function = my_str_cmp(str1, str2);
    printf("s: %i", return_value_from_function);

}

int my_str_cmp (const char *s1, const char *s2) 
{
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    // Each character is compared in turn and a number is returned based on whether ther first of second string is greater
    // Only if the characters are identical do you move to the next character
    while (*p1 != '\0') 
    {
        if (*p2 == '\0') return  1;
        if (*p2 > *p1)   return -1;
        if (*p1 > *p2)   return  1;

        p1++;
        p2++;
    }

    if (*p2 != '\0') return -1;

    // If all the characters are identical, return success / 0
    return 0;
}