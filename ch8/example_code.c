#include <stdio.h>

int main()
{
    printf("hi\n");
     char c;
    puts("Enter text. Include a dot ('.') in a sentence to exit:");
    do {
         c = getchar();
         putchar(c);
    } while (c != '.');

    return 0;
}