#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
    int left_thread; // flag if ptr is thread
    ThreadedPtr left_child;
    ElementType data;
    ThreadedPtr right_child;
    int right_thread; // flag if ptr is thread
}ThreadedTree;

ThreadedPtr CreateTree(){
    ThreadedPtr tree = NULL;
    tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    if(tree==NULL){
        fprintf(fout, "Out of Space!\n");
        return NULL;
    }
    tree->data = -1;
    tree->left_thread = 1;
    tree->left_child = tree;
    tree->right_thread = 1;
    tree->right_child = tree;
    return tree;
}

ThreadedPtr CreateNode(ElementType data)
// TreeNode 메모리할당, data값으로 초기화 해주는 함수
{
    ThreadedPtr Node = (ThreadedPtr) malloc(sizeof(ThreadedTree));
    if(Node == NULL){
        fprintf(fout, "Out of Space!\n");
        return NULL;
    }
    Node->data = data;
    Node->left_thread = 1;
    Node->left_child = NULL;
    Node->right_thread = 1;
    Node->right_child = NULL;
    return Node;
}

int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx)
{
    ThreadedPtr curr = root->right_child;
    ThreadedPtr prev = root;
    int level = 0;
    int curr_idx = root_idx;

    for(int i = idx; i >= 2; i = i/2,level++); // i = level의 깊이

    for(int i=0; i<level; i++)    // 위에서 부터 부모의 위치를 찾아가는 과정
    {
        int cnt_idx = idx;  //  부모 위치의 idx를 세어줄 cnt_idx

        for(int j=0;j < level-i-1; j++)
        {
            cnt_idx = cnt_idx/2;
            //  첫번째 분기, 두번째 분기 등 계속해서 분기의 부모 인덱스를 찾음
        }
        if(cnt_idx % 2 == 0)    // 부모 idx가 짝수일 경우 왼쪽으로 이동
        {
            prev = curr;
            curr_idx = cnt_idx;
            curr = prev->left_child;
        }
        else {                  //부모 idx가 홀수일 경우 오른쪽으로 이동
            prev = curr;
            curr_idx = cnt_idx;
            curr = prev->right_child;
        }
    }

    ThreadedPtr new_Node = CreateNode(data);    // 추가하기 위한 Node생성
    if(new_Node == NULL){
        exit(0);
    }
    if(idx % 2 == 0)    // 추가하려는 Node의 idx가 짝수일 경우 부모 왼쪽에 삽입
    {
        new_Node->left_child = prev->left_child;    // child 왼쪽 thread할당
        new_Node->right_child = prev;
        prev->left_child = new_Node;
        prev->left_thread = 0;  // 부모의 왼쪽 child가 할당 되었으므로 thread 삭제.
    }
    else {              // 추가하려는 Node의 idx가 홀수일 경우 부모 오른에 삽입
        new_Node->right_child = prev->right_child;  // child 오른쪽 thread할당
        new_Node->left_child = prev;
        prev->right_child = new_Node;
        prev->right_thread = 0; // 부모의 오른쪽 child가 할당 되었으므로 thread 삭제.
    }return 1;
}

ThreadedPtr Successor(ThreadedPtr node)
{
    // p는 후임자를 찾는 노드의 오른쪽 child 노드부터 시작.
    ThreadedPtr p = node->right_child;
    // node의 오른쪽 child 가 없으면, thread 가 있으므로 그 thread 가 후임자.
    if (node->right_thread == 1)
    {
        return p;
    }
    else
    {
        // node의 오른쪽 child p 가 존재 하고, p 의 왼쪽 child가 존재 하면
        // p는 후임자가 없으므로 left child 로 이동.
        // p의 왼쪽 child가 존재하지 않을경우 그 p가 node의 후임자가 된다.
        while (p->left_thread == 0)
        {
            p = p->left_child;
        }
        return p;
    }
}


void printInorder(ThreadedPtr root){
    // 처음 root로 시작
    ThreadedPtr p = root;
    fprintf(fout, "inorder traversal : ");
    while(p->right_child != root)
    {   // p의 다음 노드가 root가(한 바퀴 돌지 않았을 때)
        // 아닐 때. p는 자신의 후임자로 이동.
        p = Successor(p);
        fprintf(fout, "%d ",p->data);
        // 이동 후 출력을 해야 마지막 노드(right thread 로 root로 가지는)까지 출력함.
    }//마지막 p의 위치는 right thread로 root를 가지는 node.
    fprintf(fout,"\n");
}

void DeleteTree(ThreadedPtr root){
    // root 부터 시작
    ThreadedPtr p = root;
    while(p->right_child != root)   // root 의 오른쪽 자식(첫번째 노드) 가 본인이 아닐 때
    {
        // temp는 p의 후임자 노드
        ThreadedPtr temp = Successor(p);
        free(p);    // p의 메모리 해제
        p = temp;   // p에 후임자 할당. 후임자를 돌면서 모두 메모리 해제.
    }
    free(p);    // 마지막으로 남은 node의 메모리 해제.
}

int main(int argc, char *argv[]){
    fin=fopen(argv[1], "r");
    fout=fopen(argv[2], "w");

    ThreadedPtr root = CreateTree();
    if(root == NULL)
    {exit(0);}

    // input Nodes number that be inserted
    int NUM_NODES;
    fscanf(fin,"%d", &NUM_NODES);


    int root_idx=0, idx=0;

    while(++idx <= NUM_NODES){
        ElementType data;
        fscanf(fin, "%d", &data);

        if(Insert(root, root_idx, data, idx) == 0){
            return 0;
        }
    }

    printInorder(root);
    DeleteTree(root);


    fclose(fin);
    fclose(fout);

    return 0;
}
