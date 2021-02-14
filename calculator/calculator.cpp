#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

int calculatePolish(char* inputStr)
{
    int stack[1000];
    int sp = 0;
    while (!feof(stdin)) {
        int c = getchar();
        int x;
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
            // получить число
        }
    }
    return stack[sp - 1];
}


int main()
{
    
}