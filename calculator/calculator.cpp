#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int calculatePolish(char inputStr[])
{
    int stack[1000];
    int sp = 0;
    for (int i = 0; i < strlen(inputStr); i++)
    {
        char c = inputStr[i];
        int x;
        char number[128] = { 0 };
        switch (c) {
        case  ' ':
        case '\n':
            break;
        case '+':
            stack[sp - 2] = stack[sp - 2] + stack[sp - 1];  sp--;
            break;
        case '-':
            stack[sp - 2] = stack[sp - 2] - stack[sp - 1];  sp--;
            break;
        case '*':
            stack[sp - 2] = stack[sp - 1] * stack[sp - 2];  sp--;
            break;
        case '/':
            stack[sp - 2] = stack[sp - 2] / stack[sp - 1];   sp--;
            break;
        default:
            for (int j = i; inputStr[j] >= '0' && inputStr[j] <= '9'; j++)
            {
                number[j - i] = inputStr[j];
            }
            i += strlen(number);
            x = atoi(number);
            stack[sp] = x;
            sp++;
        }
    }
    return stack[sp - 1];
}


int main()
{
    printf("%d", calculatePolish("5 10 *"));
}