/*
This program reverses the string given to it by the human
This uses the function fgets, which reads a line from the specified stream (in this case stdin) and stores it into the string str. It stops when it reaches the size of str

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main()
{
        // str is of type char *. Literally a pointer to a char
        char str[100]; /* Declares a string of size 100 */
        int l = 0;
	
        printf("Print individual characters of string in reverse order:\n");

        printf("Fgets the string: ");
        fgets(str, sizeof str, stdin);
        l = strlen(str);

	    printf("The characters of the string in reverse are: ");
        
        // Start at the null character and then minus one until you reach a point where lenght is zero (or length is bigger than minus 1)
        for(str[l] = '\0'; l > -1; l--)
            {
                // printf("l: %d", l);
                // printf("|char = %c\n", str[l]);
                printf("%c", str[l]);
            }
}

/*
Source:
https://www.w3resource.com/c-programming-exercises/string/c-string-exercise-4.php
*/