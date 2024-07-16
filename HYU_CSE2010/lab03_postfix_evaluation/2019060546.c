#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// stack이 비어있을 때 Pop 함수실행시 반환값 전달하기 위해 추가한
// headerfile
#include<limits.h>


#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack{
    int *key;
    int top;
    int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

int main(int argc, char *argv[]){
    fin=fopen(argv[1], "r");
    fout=fopen(argv[2], "w");

    Stack* stack;
    char input_str[101];
    int max=20;
    int i=0, a, b, result, error_flag=0;
    fgets(input_str,101,fin);
    stack = CreateStack(max);
    fprintf(fout,"top numbers : ");

    while(input_str[i] != '#'){
        int first,second;
        // 입력이 숫자인 경우, Push 진행
        if(input_str[i] > 48 && input_str[i] < 58 )
        {
            if(IsFull(stack))
            {
                // stack속 배열이 꽉차있다면, error 메시지
                // 출력해주기 위해 세팅.
                error_flag = ISFULL;
                break;
            }
            Push(stack,input_str[i]-48);
        }
        // 입력이 숫자 외의 연산자 인 경우 연산 진행.
        else {
            first = Pop(stack);
            second = Pop(stack);
            // Pop 실행시 비어있다면 각 first,second가
            // INT_MIN을 받아왔으므로 둘중 하나라도 Pop 실패시
            // error메시지 출력 해주기 위한 세팅.
            if(first ==INT_MIN || second == INT_MIN)
            {
                error_flag = ISEMPTY;
                break;
            }
            // 각 연산마다 if statement 분기 설정.
            else if(input_str[i] == '+')
            {
                Push(stack,second+first);
            }
            else if(input_str[i] == '-')
            {
                Push(stack,second-first);
            }
            else if(input_str[i] == '*')
            {
                Push(stack,second * first);
            }
            else if(input_str[i] == '/')
            {
                if(first == 0){
                    // 0으로 나눈 경우, error메시지
                    // 출력 해주기 위한 세팅.
                    error_flag = DIVIDEZERO;
                    break;
                }
                Push(stack,second/first);
            }
            else if(input_str[i] == '%')
            {
                // 모듈러 연산을 0으로 한 경우 에러메시지 출력
                // 해주기 위해 세팅.
                if(first == 0){
                    error_flag = DIVIDEZERO;
                    break;
                }
                Push(stack,second % first);
            }
        }

        if(error_flag) break;

        int t = Top(stack);
        fprintf(fout, "%d ",t);
        result = t;
        i++;
    }

    if(error_flag == ISFULL){
        fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
    }
    else if(error_flag == ISEMPTY){
        fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
    }
    else if(error_flag == DIVIDEZERO){
        fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
    }
    else{
        if(stack->top+1 > 1){
            fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
        }
        else{
            fprintf(fout, "\nevaluation result : %d\n",result);
        }
    }
    fclose(fin);
    fclose(fout);
    DeleteStack(stack);
}
Stack* CreateStack(int max){
    // Stack에 메모리 할당, 인자로 받아온 max만큼 stack속
    // 배열 메모리 할당, stack에 아무것도 없으므로 top index -1 할당.
    Stack* S = (Stack*)malloc(sizeof(Stack));
    if(S==NULL)
    {
        fprintf(fout,"Out of space !!\n");
        exit(0);
    }
    S->max_stack_size = max;
    S->key = (int*)malloc(max * sizeof(int));
    if(S->key==NULL)
    {
        fprintf(fout,"Out of space !!\n");
        exit(0);
    }
    S->top = -1;
    return S;
}

void Push(Stack* S, int X)
{
    // 꽉차있는 경우, 아무것도 하지 않음.
    if(IsFull(S) == 1)
        return;
    S->top +=1;
    S->key[S->top] = X;
}

int Pop(Stack* S)
{
    if(IsEmpty(S)){
        // 비어있는 경우 Pop의 return값을 가장 작은 정수값으로 설정
        return INT_MIN;
    }
    else {
        // 비어있지 않은 경우, top index값을 빼내고 0으로 만든후,
        // top index를 하나 줄이기.
        int pop = S->key[S->top];
        S->key[S->top] = 0;
        S->top -= 1;
        return pop;
    }
}

int Top(Stack* S)
{
    if(IsEmpty(S))
    {
        return -1;
    }
    else
        return S->key[S->top];
}

void DeleteStack(Stack* S){
    if(IsEmpty(S))
    {   // stack이 비어있다면 아무것도 하지않음
        return ;
    }
    // key와 stack에 할당된 메모리 해제
    free(S->key);
    free(S);
}

int IsEmpty(Stack* S){
    if(S->top == -1)
    {   // 비어있는 경우 top의 index가 -1, bool타입 대신 1
        return 1;
    }
    else
        return 0;
}

int IsFull(Stack* S){
    // 비어있지 않고, top의 index와 max 사이즈가 동일 할 시 FULL상태.
    if(!IsEmpty(S) && S->top == S->max_stack_size-1)
        return 1;
    else return 0;
}
