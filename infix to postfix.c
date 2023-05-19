#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#define MAXSIZE 100
/*
* Item: An item that is being pushed to or popped from the stack
* It may be float (to be used while evaluating the postfix)
* It may be char (to be used while converting infix to postfix)
*/
typedef union
{
    float fData;
    char  cData;
} Item;
typedef struct
{
    Item items[MAXSIZE];
    int top;

} Stack;
Stack * initialize()
{

    Stack *s=malloc(sizeof(Stack));
    s->top=0;
    return s;

}
int isEmpty(Stack *s)
{
    if(s->top==0)
        return 1;
    else
        return 0;
}
Item top(Stack *s)
{
    return s->items[s->top-1];
}
Item pop(Stack *s)
{

    (s->top)--;
    return s->items[s->top];

}
void push(Stack *s, Item val)
{

    s->items[s->top]=val;
    (s->top)++;

}
/*
* infixToPostfix: converts an expression in infix notation to
* to a postfix notation (Reverse-Polish Notation)
* ASSUME ^*+-/ operators
* e.g., 22 + 31 --> 22 31 +
* e.g., ( 2 + 3 ) * 4 --> 2 3 + 4 *
*/
int priority(char c)
{
    switch(c)
    {
    case '*':
    case '/':
    case '%':
        return 1;
    case '+':
    case '-':
        return 0;
    case '^':
        return 2;
    case '(' :
        return 3;
    }
    return 0;
}
void removeSpaces(char *str)
{
    int count = 0;
    for (int i=0; i<strlen(str); i++)
    {
        if (str[i]!=' ')
            str[count++]=str[i];
        if(str[i]=='-'&&str[i+1]==' ')
            str[count++]=str[i+1];
    }

    str[count] = '\0';
}
void infixToPostfix(char* infix, char* postfix)
{
    int i;
    int count=0;
    removeSpaces(infix);
    Stack* s= initialize();
    for(i=0; i<strlen(infix); i++)
    {
        if(isdigit(infix[i]))
        {
            postfix[count++]=infix[i];
        }
        else if(infix[i]=='-'&&isdigit(infix[i+1]))
        {
            postfix[count++]=infix[i];
        }
        else if(infix[i]=='.')
        {
            postfix[count++]=infix[i];
        }
        else if(infix[i]==')')
        {
            while(!isEmpty(s)&&top(s).cData!='(')
            {
                postfix[count++]=' ';
                postfix[count++]=pop(s).cData;
            }
            pop(s).cData; //for the '(' character to be taken out of the stack
        }
        else if(isEmpty(s))
        {
            if(infix[i]!='(')
            {
                postfix[count++]=' ';
            }
            Item Operation1;
            Operation1.cData=infix[i];
            push(s,Operation1);
        }
        else
        {
            if(infix[i]!=' ')
            {
                while(!isEmpty(s)&&priority(top(s).cData)>=priority(infix[i])&&top(s).cData!='(')
                {
                    postfix[count++]=' ';
                    postfix[count++]=pop(s).cData;
                }
                if(infix[i]!='(')
                {
                    postfix[count++]=' ';
                }

            Item Operation2;
            Operation2.cData=infix[i];
            push(s,Operation2);
            }
        }
    }
    while(!isEmpty(s))
    {
        postfix[count++]=' ';
        postfix[count++]=pop(s).cData;
    }

    postfix[count]='\0';
}
/*
* evaluatePostfix: Evaluates an expression in postfix notation
* (Reverse-Polish Notation)
*/
int positive_number(char* op)
{
    int count=0;
    int i;
    for(i=0; i<strlen(op); i++)
    {
        if(isdigit(op[i]))
        {
            count++;
        }

    }
    if(count==strlen(op))
        return 1;
    else
        return 0;
}
int negative_number(char* op)
{
    int countn=0;
    int count=0;
    int i;
    for(i=0; i<strlen(op); i++)
    {
        if(op[i]=='-')
        {
            countn++;
            if(isdigit(op[i+1]))
                break;
            else
                return 0;
        }
    }
    if(countn!=1)
        return 0;
    for(i=0; i<strlen(op); i++)
    {
        if(isdigit(op[i]))
        {
            count++;
        }

    }
    if((count+countn)==strlen(op))
        return 1;
    else
        return 0;
}
int positive_decimal(char* op)
{
    int countd=0;
    int count=0;
    int i;
    for(i=0; i<strlen(op); i++)
    {
        if(op[i]=='.')
        {
            countd++;
        }
    }
    if(countd!=1)
        return 0;
    for(i=0; i<strlen(op); i++)
    {
        if(isdigit(op[i]))
        {
            count++;
        }

    }
    if((count+countd)==strlen(op))
        return 1;
    else
        return 0;
}
int negative_decimal(char* op)
{
    int countd=0;
    int countn=0;
    int count=0;
    int i;
    for(i=0; i<strlen(op); i++)
    {
        if(op[i]=='.')
        {
            countd++;
        }
    }
    for(i=0; i<strlen(op); i++)
    {
        if(op[i]=='-')
        {
            countn++;
        }
    }
    if(countd!=1)
        return 0;
    if(countn!=1)
        return 0;
    for(i=0; i<strlen(op); i++)
    {
        if(isdigit(op[i]))
        {
            count++;
        }

    }
    if((count+countd+countn)==strlen(op))
        return 1;
    else
        return 0;
}
float evaluate(char x,float op1,float op2)
{
if(x=='+')
return(op1+op2);
if(x=='^')
return pow(op1,op2);
if(x=='-')
return(op1-op2);
if(x=='*')
return(op1*op2);
if(x=='/')
return(op1/op2);
return 0;
}
float evaluatePostfix(char* postfix) //32 12 +
{
    Stack *s= initialize();
    float Pushop;
    float op1,op2;
    char*token;
    token=strtok(postfix," ");
    while(token!=NULL)
    {
        if(positive_number(token)||negative_number(token)||positive_decimal(token)||negative_decimal(token))
        {
            Pushop=atof(token);
            Item Operator1;
            Operator1.fData=Pushop;
            push(s,Operator1);
        }
        else if(token[0]=='+')
        {
            op2=pop(s).fData;
            op1=pop(s).fData;
            Item OperatorADD;
            OperatorADD.fData=evaluate(token[0],op1,op2);
            push(s,OperatorADD);
        }
        else if(token[0]=='-')
        {
            op2=pop(s).fData;
            op1=pop(s).fData;
            Item OperatorSUB;
            OperatorSUB.fData=evaluate(token[0],op1,op2);
            push(s,OperatorSUB);
        }
        else if(token[0]=='*')
        {
            op2=pop(s).fData;
            op1=pop(s).fData;
            Item OperatorM;
            OperatorM.fData=evaluate(token[0],op1,op2);
            push(s,OperatorM);
        }
        else if(token[0]=='/')
        {
            op2=pop(s).fData;
            op1=pop(s).fData;
            Item OperatorD;
            OperatorD.fData=evaluate(token[0],op1,op2);
            push(s,OperatorD);
        }
        else if(token[0]=='^')
        {
            op2=pop(s).fData;
            op1=pop(s).fData;
            Item OperatorI;
            OperatorI.fData=evaluate(token[0],op1,op2);
            push(s,OperatorI);
        }
        token=strtok(NULL," ");
    }
    return pop(s).fData;
}
/*
*
*/
void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1=' ';
}
/*
*
*/

//Mazen Ahmed Ramzy_6999
//Mazen Ahmed Mohamed_6896
int main()
{

    printf("infix to postfix converter by:\nMazen Ahmed Ramzy_6999\nMazen Ahmed Mohamed Ghanem_6896\n\n");
    char infixExpr[256] = "";
    char postfixExpr[256] = "";
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(infixExpr);
        infixToPostfix(infixExpr, postfixExpr);
        printf("Postfix : %s\n", postfixExpr);
        float result = evaluatePostfix(postfixExpr);
        printf("Result: %f\n\n", result);
        printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    }
    return 0;
}
