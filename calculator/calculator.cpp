#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define bool int
#define true 1
#define false 0
#define SIZE 1024
#define STRING_SIZE 100
#define NumberOfOperators 5

typedef struct InputVariable
{
    char Name[STRING_SIZE], Value[STRING_SIZE];
} Variable;

int getOperatorPriority(const char c)
{
    switch (c)
    {
        case '^':
        case '!':
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

bool isLetter(char curChar)
{
    if ((curChar >= 'a' && curChar <= 'z') || (curChar >= 'A' && curChar <= 'Z')) return true;
    else return false;
}

bool isNumber(char curChar)
{
    if (curChar >= '0' && curChar <= '9') return true;
    else return false;
}

int countVariables(char *expression)
{
    int j, cnt = 0, ptv = 0, ptn = 0;
    bool f = false;
    char variables[STRING_SIZE][STRING_SIZE] = {0}, name[STRING_SIZE] = {0};
    for (int i = 0; i < strlen(expression);)
    {
        char cur = expression[i];
        if (isLetter(cur))
        {
            for (j = i; j < strlen(expression); j++)
            {
                if (expression[j] == '(')
                {
                    f = true;
                    break;
                }
                if (!isLetter(expression[j])) break;
                name[ptn] = expression[j];
            }
            if (!f)
            {
                bool flag = false;
                for (int j = 0; j < ptv; j++)
                {
                    if (strcmp(variables[j], name) == 0) flag = true;
                }
                if (!flag)
                {
                    cnt++;
                    strcpy(variables[ptv], name);
                    ptv++;
                }
                ptn = 0;
            }
            i += (j - i);
        }
        else i++;
    }
    return cnt;
}

char *findUnaryMinus(char *inputStr, char *output)
{
    for (int i = 0; i < strlen(inputStr); i++)
    {
        if (inputStr[i] == '-' && (i == 0 || inputStr[i - 1] == '(')) inputStr[i] = '!';
    }
    strcpy(output, inputStr);
    return output;
}

char *makePostfixForm(char *inputStr, char *output)
{
    bool flag = false;
    char *outputStr = (char *) calloc(SIZE, SIZE);
    char *operatorStack = (char *) calloc(SIZE, SIZE);
    int StackPointer = 0, StringPointer = 0;
    for (int i = 0; i < strlen(inputStr); i++)
    {
        char cur = inputStr[i];
        if (isNumber(cur) || (cur == '.' && isNumber(inputStr[i - 1])) || cur == '!')
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
                char temp = operatorStack[StackPointer - 1];
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
            bool f = false;
            while (StackPointer)
            {
                char temp = operatorStack[StackPointer - 1];
                if (temp == '(')
                {
                    f = true;
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
        else
        {
            bool f = false;
            if (isLetter(cur))
            {
                for (int j = i; j < strlen(inputStr); j++)
                {
                    if (inputStr[j] == '(')
                    {
                        f = true;
                        break;
                    }

                }
                if (!f)
                {
                    outputStr[StringPointer] = ' ';
                    StringPointer++;
                    outputStr[StringPointer] = cur;
                    StringPointer++;
                    flag = true;
                }
                //Тут прописать случай если символы являются не названием переменной, а функцией
            }
        }
    }
    while (StackPointer)
    {
        outputStr[StringPointer] = ' ';
        StringPointer++;
        outputStr[StringPointer] = operatorStack[StackPointer - 1];
        StringPointer++;
        StackPointer--;
    }
    strcpy(output, outputStr);
    free(outputStr);
    free(operatorStack);
    return output;
}

double calculatePolish(char inputStr[])
{
    double *stack = (double *) malloc(SIZE);
    int sp = 0;
    for (int i = 0; i < strlen(inputStr); i++)
    {
        char c = inputStr[i];
        double x;
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
                if (stack[sp - 1] == 0)
                {
                    printf("ERROR");
                    exit(0);
                }
                stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
                sp--;
                break;
            case '^':
                stack[sp - 2] = pow(stack[sp - 2], stack[sp - 1]);
                sp--;
                break;
            default:
                for (int j = i; isNumber(inputStr[j]) || (inputStr[j] == '.' && isNumber(inputStr[j - 1])); j++)
                {
                    number[j - i] = inputStr[j];
                }
                i += strlen(number);
                x = atof(number);
                stack[sp] = x;
                sp++;
        }
    }
    double result = stack[sp - 1];
    free(stack);
    return result;
}

char *deleteSpaces(char *inputStr, char *output)
{
    char withoutSpaces[SIZE] = {0};
    int pt = 0;
    for (int i = 0; i < strlen(inputStr); i++)
    {
        if (inputStr[i] != ' ')
        {
            withoutSpaces[pt] = inputStr[i];
            pt++;
        }
    }
    strcpy(output, withoutSpaces);
    return output;
}

double calculateExpression(char *expr)
{
    char result[SIZE] = {0}, temp1[SIZE] = {0}, temp2[SIZE] = {0};
    strcpy(temp1, deleteSpaces(expr, temp1));
    strcpy(temp2, findUnaryMinus(temp1, temp2));
    makePostfixForm(temp2, result);
    printf("%s\n", result);
    return calculatePolish(result);
}

char *replaceWord(const char *s, const char *oldW,
                  const char *newW)
{
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;

            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }

    // Making new string of enough length
    result = (char *) malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s)
    {
        // compare the substring with the result
        if (strstr(s, oldW) == s)
        {
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

void check()
{
    //Неплохо бы написать набор тестов в отдельном текстовом файле и пихнуть в эту функцию проверку
    return;
}

int main()
{
    char Expression[SIZE];
    fgets(Expression, sizeof(Expression), stdin);
    strcpy(Expression, replaceWord(Expression, "PI", "3.1415926"));
    strcpy(Expression, replaceWord(Expression, "E", "2.71828"));
    Variable VariableData[STRING_SIZE] = {0};
    int NumberOfVariables = countVariables(Expression);
    for (int i = 0; i < NumberOfVariables; i++)
    {
        char str[STRING_SIZE] = {0};
        fgets(str, sizeof(str), stdin);
        sscanf(str, "%s = %s", VariableData[i].Name, VariableData[i].Value);
    }
    while (countVariables(Expression))
    {
        int NumberOfVariables = countVariables(Expression);
        for (int i = 0; i < NumberOfVariables; i++)
        {
            strcpy(Expression, replaceWord(Expression, VariableData[i].Name, VariableData[i].Value));
        }
    }
    double Result = calculateExpression(Expression);
    printf("%lf", Result);
    return 0;
}
