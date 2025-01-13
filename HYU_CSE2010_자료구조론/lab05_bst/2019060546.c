#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
struct BST{
    int value;
    struct BST* left;
    struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

int main(int argc, char* argv[])
{
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    char cv;
    int key;

    Tree root = NULL;

    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        if(feof(fin))
            break;
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                root = insertNode(root, key);
                break;
            case 'f':
                fscanf(fin, "%d",&key);
                if(findNode(root, key))
                    fprintf(fout, "%d is in the tree\n", key);
                else
                    fprintf(fout, "finding error: %d is not in the tree\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                if(findNode(root, key)){
                    root = deleteNode(root, key);
                    fprintf(fout, "delete %d\n", key);
                }
                else{
                    fprintf(fout, "deletion error: %d is not in the tree\n", key);
                }
                break;
            case 'p':
                fscanf(fin, "%c", &cv);
                if(cv == 'i'){
                    if(root == NULL)
                        fprintf(fout, "tree is empty");
                    else
                        printInorder(root);
                }
                fprintf(fout, "\n");
                break;
        }
    }
    deleteTree(root);
}

Tree insertNode(Tree root, int key)
{
    Tree new_node = NULL;
    new_node = (Tree)malloc(sizeof(BST));
    new_node->value = key;
    new_node->left = NULL;
    new_node->right = NULL;
    if(root == NULL)           // 루트 노드가 비었을 경우 첫 루트 노드 할당.
    {
        root = new_node;
    }
    else
    {
        Tree curr = root;
        Tree prev = NULL;
        while(curr != NULL)                 // 삽입할 위치 비어 있지 않을 때 까지 삽입할 위치 찾기
        {                                   // 이동한 위치를 prev 에 할당.
            prev = curr;
            if (key < prev->value)          // key 값이 prev value 보다 작으면
            {curr = prev->left;}            // 왼쪽 으로 이동.
            else if (key > prev->value)     // key 값이 prev value 보다 크면
            {curr = prev->right;}           // 왼쪽 으로 이동.
            else break;                     // 같은값 존재 시 break.
        }
        if (key < prev->value)      // 삽입 할 값이 prev(부모)보다 작으면
        {prev->left = new_node;}         // 왼쪽에 삽입
        else if (key > prev->value) //삽입 할 값이 prev(부모)보다 크면
        {prev->right = new_node;}        // 오른쪽에 삽입
        else {fprintf(fout, "insertion error: %d is already in the tree\n", key); return root;}
        // key == prev->value 동일 값 이라면 에러 메시지 출력
    }
    fprintf(fout, "insert %d\n",key);
    return root;
}

Tree deleteNode(Tree root, int key)
{
    Tree curr = root;
    Tree prev = root;
    while (curr->value != key)   // 삭제할 노드 찾기
    {
        prev = curr;
        if (key < prev->value)
        { curr = prev->left; }
        else if (key > prev->value)
        { curr = prev->right; }
    }
    // curr == 삭제할 위치.

    while(curr)     // 삭제 할 것이 존재할 시 반복 수행
    {
        if (curr->right)        // 오른쪽 자식 노드 존재 시 오른쪽 자식 노드를 root로 하는 subtree에서 가장 작은 값 올리기.
        {
            Tree tmp = curr;    //  값을 삭제할 node
            curr = curr->right; //  오른쪽 subtree 로 이동
            if(curr->left)      //  오른쪽 subtree의 왼쪽 자식 존재 시,
            {
                while (curr->left)  // 왼쪽 child가 없을 때 까지 이동.
                {
                    prev = curr;
                    curr = curr->left;
                }
                tmp->value = curr->value;   // 더이상 왼쪽 child가 없으면 값 옮김.
            }
            else                // 오른쪽 subtree의 왼쪽 child가 없을 시 prev에 연결.
            {
                if(tmp == root){    // 삭제 점이 root일 시 root를 변경.
                    root = curr;
                    root->left = tmp->left;
                    curr = tmp;
                    free(curr);
                    curr = NULL;
                }
                else
                {
                    if (prev->value > curr->value)
                    { prev->left = curr; }
                    else
                    { prev->right = curr; }
                    curr->left = tmp->left;     // 삭제할 node의 left가 존재할 수 있으므로 연결.
                    curr = tmp;                 // 삭제할 node를 curr에 할당후, free진행.
                    free(curr);
                    curr = NULL;
                }
            }
        }
        else if (curr->left)    // 오른쪽 존재x, 왼쪽 자식 존재 시 왼쪽 subtree 에서 가장 큰 값 올리기.
        {
            Tree tmp = curr;    //  값을 삭제할 노드
            curr = curr->left;  //  왼쪽 subtree 로 이동
            if(curr->right)      //  왼쪽 subtree 의 오른쪽 자식 존재 시,
            {
                while (curr->right)  // 오른쪽 자식이 없을 때 까지 이동.
                {
                    prev = curr;
                    curr = curr->right;
                }
                tmp->value = curr->value;   // 더이상 오른쪽 child가(큰 값) 없으면 값 옮김.
            }
            else                // 왼쪽 subtree 의 오른쪽 child가 없을 시, 왼쪽 subtree를 prev에 연결.
            {
                if(tmp == root){    // 삭제 점이 root일 시 root를 변경.
                    root = curr;
                    curr = tmp;
                    free(curr);
                    curr = NULL;
                }
                else
                {
                    if (prev->value > curr->value)
                    { prev->left = curr; }
                    else
                    { prev->right = curr; }
                    curr->right = tmp->right;       // 삭제할 node의 오른쪽 child가 존재할 수 있으므로, 할당
                    curr = tmp;                     // 삭제할 node를 curr에 할당후, free진행.
                    free(curr);
                    curr = NULL;
                }
            }
        }
        else                    // 자식 존재x, 본인 삭제.
        {
            if (prev->value > curr->value) { prev->left = NULL; }
            else { prev->right = NULL; }
            free(curr);
            curr = NULL;
        }
    }
    return root;
}


int findNode(Tree root, int key)
{
    Tree curr = root;
    Tree prev = NULL;
    while(curr != NULL) //  못 찾았을 경우도 대비 해서 끝까지 탐색.
    {
        prev = curr;    // 이동한 위치 할당
        if (key < prev->value){curr = prev->left;}  // 찾을 값이 작으면 왼쪽이동
        else if (key > prev->value){curr = prev->right;}
            // 찾을 값이 크면 오른쪽으로 이동
        else {return 1;}    // 찾았을 경우 1 return
    }
    return 0;   // curr이 null을 가리킬 시 못찾았으므로 0리턴
}

void printInorder(Tree root)
{
    Tree curr = root;
    if(curr==NULL) return;              // 이동 해 온 자리가 null이면 아무것도 하지 않음.
    printInorder(curr->left);      // 왼쪽 으로 이동,
    fprintf(fout, "%d ",curr->value);          // 더 이상 왼쪽 자식이 없으면 value 출력
    printInorder(curr->right);     // 그 이후 오른쪽으로 이동.
}
void deleteTree(Tree root)
{
    Tree curr = root;
    if(curr==NULL) return;
    deleteTree(curr->left);     // 왼쪽 재귀, 더이상 child 존재 x, free로 이동
    deleteTree(curr->right);    // 오른쪽 재귀, 더이상 child 존재 x, free로 이동
    free(curr);                      // 더이상 자식이 없으면 메모리 해제
    curr->left = NULL;
    curr->right = NULL;
}