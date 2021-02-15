﻿#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define bool int
#define true 1
#define false 0
#define SIZE 1024
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
    char* outputStr = (char*)calloc(SIZE, SIZE);
    char* operatorStack = (char*)calloc(SIZE, SIZE);
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
    free(outputStr);
    free(operatorStack);
    return output;
}

int calculatePolish(char inputStr[])
{
    int *stack = (int*)malloc(SIZE);
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
    int result = stack[sp - 1];
    free(stack);
    return result;
}

int calculateExpression(char* expr)
{
    char result[SIZE] = { 0 };
    makePostfixForm(expr, result);
    return calculatePolish(result);
}

char* replaceWord(const char* s, const char* oldW,
    const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word 
    // occur in the string 
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;

            // Jumping to index after the old word. 
            i += oldWlen - 1;
        }
    }

    // Making new string of enough length 
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        // compare the substring with the result 
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}

int main()
{
    char expression[SIZE];
    gets_s(expression, SIZE);
    //expression[strlen(expression) - 1] = 0;
    strcpy(expression, replaceWord(expression, "a", "2"));
    strcpy(expression, replaceWord(expression, "b", "4"));
    int result = calculateExpression(expression);
    printf("%s = %d", expression, result);

    return 0;
}
