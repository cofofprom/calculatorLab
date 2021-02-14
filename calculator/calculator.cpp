#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 1000
#define NumberOfOperators 5

int getOperatorPriority(const char c)
{
    switch (c)
    {
        case '^':
            return 4;
        case '*':
        case '/':
            return 3;
        case '+':
        case '-':
            return 2;
        case '=':
            return 1;
    }
    return 0;
}

bool isOperator(char curChar)
{
    char Operators[NumberOfOperators] = {'+', '-', '*', '/', '^'};
    for (int i = 0; i < NumberOfOperators; i++)
    {
        if (curChar == Operators[i]) return true;
    }
    return false;
}

char *makePostfixForm(char *inputStr, char *output)
{
    bool flag = false;
    char outputStr[SIZE] = {0};
    char operatorStack[SIZE] = {0};
    int StackPointer = 0, StringPointer = 0;
    for (int i = 0; i < strlen(inputStr); i++)
    {
        char cur = inputStr[i];
        if (cur >= '0' && cur <= '9')
        {
            if (flag) outputStr[StringPointer] = cur, StringPointer++;
            else
            {
                outputStr[StringPointer] = ' ';
                StringPointer++;
                outputStr[StringPointer] = cur;
                StringPointer++;
                flag = true;
            }
        }
        else if (isOperator(cur))
        {
            flag = false;
            while (StackPointer)
            {
                char temp = operatorStack[StackPointer-1];
                if (isOperator(temp) && (getOperatorPriority(cur) <= getOperatorPriority(temp)))
                {
                    outputStr[StringPointer] = ' ';
                    StringPointer++;
                    outputStr[StringPointer] = temp;
                    StringPointer++;
                    StackPointer--;
                }
                else break;
            }
            operatorStack[StackPointer] = cur;
            StackPointer++;
        }
        else if (cur == '(')
        {
            flag = false;
            operatorStack[StackPointer] = cur;
            StackPointer++;
        }
        else if (cur == ')')
        {
            flag = false;
            bool flag = false;
            while (StackPointer)
            {
                char temp = operatorStack[StackPointer-1];
                if (temp == '(')
                {
                    flag = true;
                    break;
                }
                else
                {
                    outputStr[StringPointer] = ' ';
                    StringPointer++;
                    outputStr[StringPointer] = temp;
                    StringPointer++;
                    StackPointer--;
                }
            }
            StackPointer--;
        }
    }
    while (StackPointer)
    {
        outputStr[StringPointer] = ' ';
        StringPointer++;
        outputStr[StringPointer] = operatorStack[StackPointer-1];
        StringPointer++;
        StackPointer--;
    }
    strcpy(output, outputStr);
    return output;
}

int calculatePolish(char inputStr[])
{
    int stack[SIZE];
    int sp = 0;
    for (int i = 0; i < strlen(inputStr); i++)
    {
        char c = inputStr[i];
        int x;
        char number[128] = {0};
        switch (c)
        {
            case ' ':
            case '\n':
                break;
            case '+':
                stack[sp - 2] = stack[sp - 2] + stack[sp - 1];
                sp--;
                break;
            case '-':
                stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
                sp--;
                break;
            case '*':
                stack[sp - 2] = stack[sp - 1] * stack[sp - 2];
                sp--;
                break;
            case '/':
                stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
                sp--;
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
    char *Expression = "23-16+45*2";
    char check[SIZE] = {0};
    char *res = makePostfixForm(Expression, check);
    printf("%s\n%d", res, calculatePolish(res));
    return 0;
}
