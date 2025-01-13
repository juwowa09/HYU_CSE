#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of key */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);

void Insert(BNodePtr *root, int key);
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int pos);
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos);
BNodePtr Createnode(int order);

int Find(BNodePtr root, int key);

void PrintTree(BNodePtr root);

void DeleteTree(BNodePtr root);
void free_node(BNodePtr node);



int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                {
                    Insert(&root, key);
                }
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 0)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

/* 새로운 트리를 생성하는 함수 */
BNodePtr CreateTree(int order){
    BNodePtr node = Createnode(order);
    node->size = 0;         // 첫 트리는 root 노드만 존재, size는 0.
    node->is_leaf = 1;      // 첫 트리는 root 노드만 존재, root노드는 leaf노드.
    return node;
}

/* find node that has key */
int Find(BNodePtr root, int key){
    if(root == NULL){   // 함수를 통해 받아온 노드가 NULL이라면 child, key가 존재하지 않으므로 아무것도 하지않고 return.
        return 0;
    }
    // 받아온 노드의 size만큼 key배열을 확인. key배열 속 key가 존재한다면 함수 종료.
    for(int i = 0; i < root->size ; i++){
        if(root->key[i] == key) return 1;
    }
    int f_key = 0;      // 함수값을 저장할 변수
    for (int i = 0; i<=root->size; i++) // child의 개수는 size의 수보다 1더 크므로 한번더 반복.
    {
        // f_key에 모든 child(child가 NULL이 아닐 때까지 탐색)를 재귀적으로 탐색.
        f_key = Find(root->child[i],key);
        // 해당 child 속에 key 값 존재 시 for문 break.
        if(f_key == 1) break;
    }
    // 못찾은 경우 f_key는 0을 return, 찾은경우 for문을 break하고 1 return.
    return f_key;
}

/* insert key */
void Insert(BNodePtr *root, int key){ // 루트를 함수 내에서 변경시키기 위해 이중 포인터로 선언
    *root = Insert_sub(*root, *root, key, 0);
    // 리턴값이 있는 재귀함수 활용, 처음에 root 노드는 현재 노드이자 parent이므로 같이 parent, node 똑같이 매개변수 전달
}

BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int p_pos){
    // 리프노드인 경우 삽입 수행할 코드
    if(node->is_leaf){
        int i = 0;
        while(i < node->size)
        {
            if(key<node->key[i])    // node의 key배열 i번째 index보다 key가 작을 경우
            {
                for(int k = node->size; k>i; k--)   // i번째 이상 key 배열을 모두 한칸씩 뒤로 보냄
                {node->key[k] = node->key[k-1];}
                break;      // if문 수행하면 무조건 whild문 break.
            }
            i++;            // node의 key 배열 i번째 index보다 key가 클 경우 다음 index 확인
        }
        node->key[i]=key;   // 비어있는 i번째에 key를 삽입
        node->size++;       // size하나 추가
        if(node->size >= node->order) {return split_node(parent, node,p_pos);}  // 노드의 개수가 order랑 똑같아질 시 split 수행
        else {return parent;}       // size가 order보다 작다면 그대로 parent return
    }
    // 리프노드가 아닌 경우 삽입을 수행할 코드
    else {
        int i = node->size-1;       // size는 key배열의 key 개수 이므로 index로 사용 시 size -1.
        while(i>=0)                 // i는 key배열의 마지막 인덱스. i가 0인 경우 size가 1이므로 수행.
        {
            if(key < node->key[i]){ // 마지막 key배열의 값보다 key가 작을시 index줄임.
                i--;
            }
            else break;             // 마지막 key배열의 값보다 크다면 거기서 멈춤.
        }
        i++;        // key는 key[i]보다 크므로 child[i]가 아닌 child[i+1] 자리에 삽입해야함. 그러므로 i++ 수행.
        node = Insert_sub(node,node->child[i],key,i);
        // 해당 노드의 i번째 자식에 key 값 삽입 수행. 스플릿 진행한다면, i자리에 스플릿 진행한 key값이 들어가야 하므로 p_pos에 i전달.
        if(node->size>=node->order){
            // 해당 노드의 자식에서 삽입 수행하였고 split 진행, node의 size가 추가되고 size가 order랑 같아졌다면
            // 해당 노드에서 split 수행. parent에 전달.
            parent = split_node(parent, node,p_pos);
            return parent;
        }
        else return parent; // node->size가 order보다 작다면 문제가 없으므로 그대로 parent return.
    }
}

BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos){
    // split을 수행할 기준점 index m, order의 절반, 짝수인 경우 왼쪽 split node의 크기가 더 작게
    int m = (node->order-1)/2;
    // split을 통해 node의 sibling(다음 child)이 될 new node 생성
    BNodePtr n_node = Createnode(node->order);
    n_node->size = 0;
    // node와 같은 level에 위치하기 때문에 node의 leaf 여부를 동일하게 할당.
    n_node->is_leaf = node->is_leaf;

    for(int i=m+1; i < node->size; i++)
    {
        n_node->key[i-(m+1)] = node->key[i];       // node의 key배열의 m+1번째 index부터 new node key배열로 이동.
        n_node->child[i-(m+1)] = node->child[i];   // node의 child들을 m+1 번째부터 new node의 child에 연결.
        node->key[i] = 0;
        node->child[i] = NULL;                  // new node로 옮긴 node의 child위치를 모두 null 처리.
        n_node->size++;                            // new node의 size 하나씩 올려줌
    }
    n_node->child[n_node->size] = node->child[node->size];   // node->size번째에 있는 마지막 child를 new node에 연결
    node->child[node->size] = NULL;                    // for 문에서 연결 못해줫기 때문.
    node->size = m;                             // node의 size 변경.

    if(parent == node)      // split 하는데 parent가 없을 경우(root 노드를 split하는 경우)
    {
        BNodePtr n_root = Createnode(node->order);  // 새로운 root 노드를 생성
        n_root->size = 0;
        n_root->is_leaf = 0;
        n_root->child[pos] = node;  // root 노드는 pos를 0으로 받아오므로 첫 child에 node(원래 root)를 할당.
        parent = n_root;            // parent를 원래 root(node)에서 새로운 root로 변경.
    }
    // parent의 pos위치에 split한 key와 child를 넣어주어야 하기 때문에 pos위치부터 끝까지 한칸씩 뒤로 밀어내는 for문.
    for(int i = parent->size; i>pos; i--){
        parent->key[i]=parent->key[i-1];
        parent->child[i+1] = parent->child[i];
        //  child는 pos자리에는 node가 이미 들어가있다.
        //  pos + 1자리에 new node가 들어가야 하기 때문에 한칸 건너 뛰어서 한칸씩 밀어줌.
    }
    parent->key[pos] = node->key[m];    // 한칸씩 밀어주었기 때문에 빈 pos자리를 채움
    parent->child[pos+1]=n_node;           // 한칸씩 밀어주었기 때문에 빈 pos+1 자리를 채움.(pos자리는 node)
    parent->size++;
    return parent;
}

/* 노드 하나를 동적할당하는 함수, size, is_leaf는 노드마다 다르므로 초기화하지 않고 공통되는 부분만 초기화 */
BNodePtr Createnode(int order){
    BNodePtr n = (BNodePtr)malloc(sizeof(BNode));
    if(n==NULL){ fprintf(fout, "Out of Space!\n"); exit(0);}
    n->order = order;
    n->child = (BNodePtr*)malloc(sizeof(BNodePtr)*(order+1));
    n->key = (int*)malloc(sizeof(int) * order);
    return n;
}

/* print Tree, inorder traversal */
void PrintTree(BNodePtr root)
{
    BNodePtr tmp = root;
    int cnt = 0;

    if (tmp->is_leaf)    // leaf 노드일 경우 모든 key배열을 출력.
    {
        for (int i = 0; i < tmp->size; i++)
        {fprintf(fout, "%d ", tmp->key[i]);}
    }
    else
    {
        while (cnt < tmp->size)     // key배열의 size만큼 child가 존재하므로 하나씩 출력.
                                    // leaf노드가 아니므로 tmp는 항상 NULL이 아님.
        {
            // root로 받아온 tmp node의 cnt index의 left subtree의 모든 key값을 재귀적으로 출력.
            PrintTree(tmp->child[cnt]);
            //  left subtree 출력 이후 tmp node의 cnt index의 key값 출력
            fprintf(fout, "%d ", tmp->key[cnt]);
            cnt++;  //  다음 key값의 left subtree 출력, key값 출력하기 위해 index한칸 올림.
        }
        PrintTree(tmp->child[cnt]);        // 마지막 child는 index가 tmp->size값이기 때문에
                                                // while문 속에서 재귀 하지 않으므로 따로 마지막에 따로 한번 재귀.
                                                // while문 속에서 재귀로 print시 key배열 사이즈가 넘어가므로,
                                                // child만 따로 한 번 하는 것.
    }
}
/* depth first search -> free memory */
void DeleteTree(BNodePtr root){
    BNodePtr tmp = root;

    if(tmp->is_leaf) free_node(tmp);      // 받아온 tmp node가 leaf 노드 일 경우 free 수행.
    else{                                       // leaf 노드가 아닐 경우
        for (int i = 0; i <= tmp->size; i++)    // 모든 child를 재귀적으로 free 수행.
        { DeleteTree(tmp->child[i]); }
        free_node(tmp);                   // 모든 child free 수행 했다면, 해당 노드 free 수행.
    }
}

/* 노드의 메모리를 해제해주는 helper function */
void free_node(BNodePtr node){
    free(node->child);
    free(node->key);
    free(node);
}
