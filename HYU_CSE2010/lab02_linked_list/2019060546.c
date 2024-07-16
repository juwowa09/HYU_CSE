#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct Node* PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

typedef struct Node{
    ElementType element;
    Position next;
};

List MakeEmpty(List L){
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->next = NULL;
    L = n;
    return L;
}

int IsEmpty(List L){
    if(L == NULL)
    {
        return 1;
    }
    else return 0;
}

int IsLast(Position P, List L){
    PtrToNode cnt = L;
    while(cnt->next != NULL)
    {
        cnt=cnt->next;
    }
    // 마지막까지 cnt를 보낸뒤, P가 NULL노드인지 아닌지 검사
    if(cnt->next == P)
        return 1;
    else return 0;
}

void Insert(ElementType X, List L, Position P){
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->element = X;

    // P 위치까지 도달하기 위한 cnt생성
    Position cnt = L;
    // 받아온 P의 position이 header라면 (입력을 -1로 받은 경우)
    if(cnt == P){
        // n 을 header바로 다음에 위치
        PtrToNode temp = cnt->next;
        cnt->next = n;
        n->next = temp;
        return ;
    }
        // 그 외의 모든 경우
    else
    {
        while(cnt->next != P)
        {
            cnt = cnt->next;
        }
        // insert 해주기 위해 잠시 저장할 temp
        PtrToNode temp = cnt->next->next;
        cnt->next->next = n;
        n->next = temp;
    }
}

void PrintList(List L)
{
    PtrToNode cnt = L;
    while(cnt->next !=NULL)
    {
        fprintf(fout,"key:%d ",cnt->next->element);
        cnt = cnt->next;
    }
    fprintf(fout,"\n");
}

Position FindPrevious(ElementType X, List L){
    PtrToNode cnt = L;

    while(cnt->next != NULL)
    {
        // To find previous position
        // Node has element X
        if (cnt->next->element == X)
        {
            return cnt;
        }
        else{cnt = cnt->next;}
    }
    // don`t find, return last NULL Node
    return cnt->next;
}

Position Find(ElementType X, List L){
    PtrToNode cnt = L;
    if(X == -1) {
        // -1 위치에 넣어주기 위해서 반환은 header
        return L;
    }
    else {
        // X가 존재하는 위치까지 갈 cnt
        while (cnt->next != NULL)
        {
            if(cnt->next->element == X)
            {
                // find element, return the Node Position
                return cnt->next;
            }
            // move next node
            cnt=cnt->next;
        }
        // 찾지 못했다면 마지막 NULL Node return
        return cnt->next;
    }
}

void Delete(ElementType X, List L){
    PtrToNode cnt=L;

    while(cnt->next != NULL)
    {
        // List search while문
        if(cnt->element == X){
            // in case cnt data be X

            PtrToNode temp = cnt->next;
            PtrToNode previous = FindPrevious(cnt->element, L);
            previous->next = temp;
            // cnt memory delete
            free(cnt);
            return ;
        }
        cnt = cnt->next;
    }
    // last one Node는 while문에서 검사하지
    // 못했으므로 X를 찾지 못한경우와 함께 if문 처리
    if(cnt->element != X)
    {
        fprintf(fout,"deletion(%d) failed : element %d is not in the list\n",X,X);
    }
    else if(cnt->element == X){
        // To link from front Node to back Node of cnt Node
        // cnt memory delete
        PtrToNode temp = cnt->next;
        PtrToNode previous = FindPrevious(cnt->element, L);
        previous->next = temp;
        free(cnt);
        return ;
    }
}

void DeleteList(List L){
    PtrToNode cnt;
    while(L->next != NULL)
    {
        // all Node memory delete
        cnt = L->next;
        L->next = cnt->next;
        free(cnt);
    }
}

int main(int argc, char **argv){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    char x;

    Position header=NULL, tmp=NULL;
    header = MakeEmpty(header);
    while(fscanf(fin,"%c",&x) != EOF){
        if(x=='i'){
            int a,b;
            fscanf(fin,"%d%d",&a,&b);
            tmp = Find(a, header);
            if(tmp != NULL){
                fprintf(fout,"insertion(%d) failed : the key already exists in the list\n", a);
                continue;
            }
            tmp = Find(b, header);
            if(tmp==NULL){
                fprintf(fout,"insertion(%d) failed : can not find the location to be inserted\n", a);
                continue;
            }
            Insert(a, header, tmp);
        }
        else if(x=='d'){
            int a;
            fscanf(fin,"%d",&a);
            Delete(a, header);
        }
        else if(x=='f'){
            int a;
            fscanf(fin,"%d",&a);
            tmp = FindPrevious(a, header);
            if(IsLast(tmp, header))
                fprintf(fout,"finding(%d) failed : element %d is not in the list\n",a,a);
            else{
                if(tmp->element>0)
                    fprintf(fout,"key of the previous node of %d is %d\n", a, tmp->element);
                else
                    fprintf(fout,"key of the previous node of %d is head\n",a);
            }
        }
        else if(x=='p') PrintList(header);
    }
    DeleteList(header);

    return 0;
}