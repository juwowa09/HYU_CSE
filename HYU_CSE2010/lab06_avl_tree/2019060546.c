#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
    ElementType element;
    AVLTree left, right;
    int height;
}AVLNode;

int getHeight(Position T);
void setHeight(Position T);
int getBalance(Position T);
AVLTree MakeNode(ElementType X);
AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
AVLTree UpdateTree(AVLTree T, AVLTree X);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);

int main(int argc, char *argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    AVLTree Tree = NULL;
    char cv;
    int key;

    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d\n", &key);
                Tree = Insert(key, Tree);
                break;
            case 'd':
                fscanf(fin, "%d\n", &key);
                Tree = Delete(key, Tree);
                break;
        }
        PrintInorder(Tree);
        fprintf(fout, "\n");
    }

    DeleteTree(Tree);
    fclose(fin);
    fclose(fout);

    return 0;
}
int getHeight(Position T)
{
    if(T)
        return T->height;
    return -1;
}
void setHeight(Position T)
{
    int right_h = 0, left_h = 0;
    right_h = getHeight(T->right);
    left_h = getHeight(T->left);
    T->height = (right_h > left_h ? right_h : left_h) + 1;
}

int getBalance(Position T)
{
    int diff = 0, right_h = 0, left_h = 0;
    right_h = getHeight(T->right);
    left_h = getHeight(T->left);
    diff = right_h - left_h;
    if (diff < 0) diff *= -1;

    return diff;
}

AVLTree MakeNode(ElementType X){    // make new node that root node or addition node
    AVLTree new_node = NULL;
    new_node = (AVLTree)malloc(sizeof(AVLNode));
    new_node->element = X;
    new_node->height = 0;
    return new_node;
}

AVLTree UpdateTree(AVLTree T, Position p)
{
    int diff=0,right_h=0,left_h=0;
    // curr locate be unbalance, need Rotate

    if (T->height!=0 && T->element < p->element)
    { T->right = UpdateTree(T->right, p); }
    else if (T->height!=0 && T->element > p->element)
    { T->left = UpdateTree(T->left, p); }

    if(T->height != 0)
    {
        diff = getBalance(T);
    }

    // Left subtree depth higher than Right subtree
    if(diff>1)
    {
        if (getHeight(T->left) > getHeight(T->right))
        {
            // Left subtree`s Left subtree depth higer than Right subtree
            // LL - R
            if (getHeight(T->left->left) >= getHeight(T->left->right))
            { T = SingleRotateWithLeft(T); }
                // Left subtree`s Right subtree depth higer than Left subtree
                // LR - R
            else
            { T = DoubleRotateWithLeft(T); }
        }
            // Right subtree depth higher than Left subtree
        else
        {
            // Right subtree`s Right subtree depth higer than Left subtree
            // RR - L
            if (getHeight(T->right->right) >= getHeight(T->right->left))
            {T = SingleRotateWithRight(T);}
                // Right subtree`s Right subtree depth higer than Right subtree
                // RL - L
            else
            {T = DoubleRotateWithRight(T);}
        }
    }
    setHeight(T);
    return T;
}

AVLTree Insert(ElementType X, AVLTree T)
{
    AVLTree new_node = MakeNode(X);

    if (T == NULL)
    {
        T = new_node;
    }
    else
    {
        AVLTree curr = T;
        AVLTree prev = NULL;
        while (curr)
        {
            prev = curr;
            if (X < curr->element)
            { curr = curr->left; }
            else if (X > curr->element)
            { curr = curr->right; }
            else break;
        }
        if (X < prev->element)
        {
            prev->left = new_node;
            // Update Tree because Tree be modified
        }
        else if (X > prev->element)
        {
            prev->right = new_node;
            // Update Tree because Tree be modified
        }
        else
        {
            fprintf(fout, "insertion error : %d is already in the tree!\n", X);
            return T;
        }
        prev->height++;
        T = UpdateTree(T, prev);
    }
    return T;
}

AVLTree Delete(ElementType X, AVLTree T)
{
    AVLTree curr = T;
    AVLTree prev = T;
    if(!curr) {
        fprintf(fout, "deletion error : %d is not in the tree!\n",X);
        return T;
    }
    while (curr)   // find node be delete
    {
        if(X == curr->element) break;
        prev = curr;
        if (X < prev->element)
        { curr = prev->left; }
        else if (X > prev->element)
        { curr = prev->right; }
    }

    if (!curr)
    {        //  don`t exist node deleted, print error message.
        fprintf(fout,"deletion error : %d is not in the tree!\n", X);
        return T;
    }
    // curr == 삭제할 위치.

    AVLTree tmp = curr; //  deleted node location
    if (curr->right)            //  오른쪽 자식 노드 존재 시 오른쪽 자식 노드를 root로 하는 subtree에서 가장 작은 값 올리기.
    {
        curr = curr->right;     //  move right subtree
        if(curr->left)          //  if exist left child of right subtree,
        {
            while (curr->left)  //  move left child until don`t exist left child.
            {
                prev = curr;
                curr = curr->left;
            }
            tmp->element = curr->element;   // 더이상 왼쪽 child가 없으면 값 옮김.
            prev->left = curr->right;
            T = UpdateTree(T, prev);
            free(curr);
            curr = NULL;
        }
        else                    // 오른쪽 subtree의 왼쪽 child가 없을 시 prev에 연결.
        {
            if(tmp == T){       // 삭제 점이 root일 시 root를 변경.
                T = curr;       // root를 오른쪽 subtree로 변경
                T->left = tmp->left;
                curr = tmp;
                T = UpdateTree(T,T);
                free(curr);
                curr = NULL;
            }
            else
            {
                if (prev->element > curr->element)
                { prev->left = curr; }
                else
                { prev->right = curr; }
                curr->left = tmp->left;     // 삭제할 node의 left가 존재할 수 있으므로 연결.
                T = UpdateTree(T,curr);
                curr = tmp;                 // 삭제할 node를 curr에 할당후, free진행.
                free(curr);
                curr = NULL;
            }
        }
    }
    else if (curr->left)        // 오른쪽 존재x, 왼쪽 자식 존재 시 왼쪽 subtree 에서 가장 큰 값 올리기.
    {
        curr = curr->left;      //  왼쪽 subtree 로 이동
        if(curr->right)         //  왼쪽 subtree 의 오른쪽 자식 존재 시,
        {
            while (curr->right) // 오른쪽 자식이 없을 때 까지 이동.
            {
                prev = curr;
                curr = curr->right;
            }
            tmp->element = curr->element;   // 더이상 오른쪽 child가(큰 값) 없으면 값 옮김.
            prev->right = curr->left;
            T = UpdateTree(T, prev);
            free(curr);
            curr = NULL;
        }
        else                    // 왼쪽 subtree 의 오른쪽 child가 없을 시, 왼쪽 subtree를 prev에 연결.
        {
            if(tmp == T){    // 삭제 점이 root일 시 root를 변경.
                T = curr;
                curr = tmp;
                T = UpdateTree(T,T);
                free(curr);
                curr = NULL;
            }
            else
            {
                if (prev->element > curr->element)
                { prev->left = curr; }
                else
                { prev->right = curr; }
                curr->right = tmp->right;       // 삭제할 node의 오른쪽 child가 존재할 수 있으므로, 할당
                T = UpdateTree(T,curr);
                curr = tmp;                     // 삭제할 node를 curr에 할당후, free진행.
                free(curr);
                curr = NULL;
            }
        }
    }
    else                    // 자식 존재x, 본인 삭제.
    {
        if(curr == T)
        {
            free(curr);
            curr=NULL;
            T=NULL;
        }
        else
        {
            if (prev->element > curr->element)
            { prev->left = NULL; }
            else
            { prev->right = NULL; }
            T = UpdateTree(T, prev);
            free(curr);
            curr = NULL;
        }
    }

    return T;
}

Position SingleRotateWithLeft(Position node){
    AVLTree tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    node->height = (getHeight(node->right) > getHeight(node->left) ? getHeight(node->right) : getHeight(node->left)) + 1;
    tmp->height = (getHeight(tmp->right) > getHeight(tmp->left) ? getHeight(tmp->right) : getHeight(tmp->left)) + 1;
    return tmp;
}
Position SingleRotateWithRight(Position node){
    AVLTree tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    node->height = (getHeight(node->right) > getHeight(node->left) ? getHeight(node->right) : getHeight(node->left)) + 1;
    tmp->height = (getHeight(tmp->right) > getHeight(tmp->left) ? getHeight(tmp->right) : getHeight(tmp->left)) + 1;
    return tmp;
}
Position DoubleRotateWithLeft(Position node){
    node->left = SingleRotateWithRight(node->left);
    return SingleRotateWithLeft(node);
}
Position DoubleRotateWithRight(Position node){
    node->right = SingleRotateWithLeft(node->right);
    return SingleRotateWithRight(node);
}

void PrintInorder(AVLTree T){
    AVLTree curr = T;
    if(!curr)
    {return ;}
    PrintInorder(curr->left);      // 왼쪽 으로 이동,
    fprintf(fout, "%d(%d) ",curr->element, curr->height);          // 더 이상 왼쪽 자식이 없으면 value 출력
    PrintInorder(curr->right);     // 그 이후 오른쪽으로 이동.
}

void DeleteTree(AVLTree T){
    AVLTree curr = T;
    if(!curr) return;
    DeleteTree(curr->left);     // 왼쪽 재귀, 더이상 child 존재 x, free로 이동
    DeleteTree(curr->right);    // 오른쪽 재귀, 더이상 child 존재 x, free로 이동
    free(curr);                      // 더이상 자식이 없으면 메모리 해제
    curr->left = NULL;
    curr->right = NULL;
}