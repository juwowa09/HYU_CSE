#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
    int* key;
    int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
    int size;
    int* node;
    int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void Topsort(Graph* G);
int checkCycle(Graph* G, int vertex,int* visited);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);
void checkIndegree(Graph* G, int* idg);
void sort_node(int* arr, int n);

int main(int argc, char* agrv[]) {
    fin = fopen(agrv[1], "r");
    fout = fopen(agrv[2], "w");

    char numbers[100], inputs[150];
    fgets(numbers,100,fin);   // number = 첫째 줄 입력.
    int n = 0;
    countInput(&n, numbers);    // n = 숫자의 개수
    int* nodes = (int*)malloc(sizeof(int)*n);
    parseInput(nodes, numbers, n);  // node = 숫자 입력의 배열

    Graph* G = CreateGraph(nodes, n);

    fgets(inputs,100,fin);
    int len = strlen(inputs), i, j;
    for (i = 0; i < len; i += 4) {
        // a = outdegree node / b = indegree node
        int a = inputs[i] - '0', b = inputs[i + 2] - '0';
        InsertEdge(G, a, b);
    }

    PrintGraph(G);
    Topsort(G);
    DeleteGraph(G);

    fclose(fin);
    fclose(fout);

    return 0;
}

void countInput(int* n, char* str) {
    int len = strlen(str), i;
    for (i = 0; i < len; i++)
        if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
    int len = strlen(str), i;
    int cnt = 0;
    for (i = 0; i < len; i++)
        if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}




Queue* MakeNewQueue(int X)      // make queue
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->key = (int*)malloc(sizeof(int) * X);
    q->max_queue_size = X;
    q->first = 0;
    q->rear = -1;
    q->qsize = 0;
    return q;
}

int IsEmpty(Queue* Q){
    if(Q->qsize == 0) return 1;
    return 0;
}

void Enqueue(Queue* Q, int X){
    Q->rear++;
    if(Q->rear >= Q->max_queue_size) {Q->rear -= Q->max_queue_size;}
    Q->key[Q->rear] = X;
    Q->qsize++;
}

int Dequeue(Queue* Q){
    int dequeue = Q->key[Q->first];
    Q->key[Q->first] = 0;
    Q->first++;
    Q->qsize--;
    if(Q->first >= Q->max_queue_size) {Q->first -= Q->max_queue_size;}
    return dequeue;
}

void DeleteQueue(Queue* Q){
    free(Q->key);
    free(Q);
}

//  입력된 숫자배열과 크기를 통해 Graph를 만드는 함수
Graph* CreateGraph(int* nodes, int n){  // node = 숫자 입력 배열, n = 배열의 크기
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->node = nodes;
    g->size = n;
    sort_node(g->node,g->size); // 입력된 숫자배열 sorting
    g->matrix = (int**)malloc(sizeof(int*) * n);
    for(int i=0;i<n;i++)
    {g->matrix[i] = (int*)malloc(sizeof(int) * n);}
    for(int i=0;i<n;i++){   // Graph 초기화
        for(int j=0;j<n;j++){
            g->matrix[i][j] = 0;
        }
    }
    return g;
}

void InsertEdge(Graph* G, int a, int b){    // 입력된 direct edge 표현
    int row, col;
    for(int i=0;i<G->size;i++)
    {
        if(G->node[i] == a) row = i;    // Source node index 확인
        if(G->node[i] == b) col = i;    // Target node index 확인
    }
    G->matrix[row][col] = 1;    // Source node index 행, Target node index 열에 1 표시 >> dege 존재.
}

void PrintGraph(Graph* G){      // 현재 그래프 상태  출력
    fprintf(fout, "   ");
    for(int i=0; i<G->size;i++)
    {
        fprintf(fout, "%d  ",G->node[i]);
    }
    for(int i=0;i<G->size;i++)
    {
        fprintf(fout, "\n%d  ",G->node[i]);
        for(int j=0;j<G->size;j++)
        {
            fprintf(fout, "%d  ",G->matrix[i][j]);
        }
    }
    fprintf(fout,"\n");
}

void DeleteGraph(Graph* G){     // Graph에 할당된 메모리 해제
    free(G->node);
    for(int i=0;i<G->size;i++)
    {free(G->matrix[i]);}
    free(G->matrix);
    free(G);
}

void Topsort(Graph* G){
    fprintf(fout, "\n");
    int* visited = (int*)malloc(sizeof(int)*G->size);   // cycle 존재 유무를 파악할 visited
    for(int j = 0;j<G->size;j++) visited[j] = 0;
    for(int i=0;i<G->size;i++)
    {
        if (checkCycle(G, i, visited))  // 사이클 존재 시
        {
            fprintf(fout, "sorting error : cycle!\n");
            free(visited);  // visited 메모리 해제
            return;
        }
    }
    int* idg = (int*)malloc(sizeof(int)*G->size);   // 각 노드의 총 indegree 할당할 배열
    Queue* q = MakeNewQueue(G->size);
    checkIndegree(G, idg);   // 각 노드의 총 indegree idg에 할당

    for(int i=0; i<G->size; i++)
    {if(idg[i] == 0) Enqueue(q,G->node[i]);}    // indegree 0 인 경우 Enqueue
    while(!IsEmpty(q))      // queue가 비어있지 않을 때 반복
    {
        int d = Dequeue(q);
        fprintf(fout, "%d ", d);
        for(int i=0;i<G->size;i++)
        {
            if(G->node[i] == d) // indegree가 0인 d 의 인덱스 i
            {
                for(int j=0;j<G->size;j++)
                {
                    if(G->matrix[i][j] == 1)    // d와 인접한 노드의 인덱스 j
                    {
                        G->matrix[i][j] = 0;    // 인접한 관계 끊기
                        idg[j]--;               // indegree 삭제
                        if(idg[j] == 0)         // d 를 통한 indegree를 삭제하여 indegree가 0이 된 경우
                        {Enqueue(q, G->node[j]);}
                    }
                }
                break;
            }
        }
    }fprintf(fout, "\n");
    DeleteQueue(q);
    free(idg);
    free(visited);
}

int checkCycle(Graph* G, int vertex, int* visited){
    visited[vertex] = 1;        // 방문한 노드 기록
    for(int i=0;i<G->size;i++)
    {
        if(G->matrix[vertex][i] == 1)   // 방문한 노드의 인접한 노드가 존재할 경우 실행
        {                               // 사이클 존재하지 않을 시 visited[i] = 0이 되고, 다음 인접한 노드 탐색
            if(visited[i] == 1){    // 인접한 노드 i 를 이미 방문 한 경우 cycle 존재
                return 1;
            }
            else {      // 인접한 노드 i를 처음 방문시(i번째 이전 인접한 노드들의 사이클 존재x) 재귀적으로 방문
                if(checkCycle(G,i,visited)) {
                    return 1;
                }
            }
        }
    }
    visited[vertex] = 0;    // 사이클 존재 하지 않을 시 방문 기록 삭제
    return 0;
}

void checkIndegree(Graph* G, int* idg)
{
    for(int i=0;i<G->size;i++)
    {
        int indegree = 0;
        for(int j=0;j<G->size;j++)
        {indegree += G->matrix[j][i];}  // i 번째 노드의 indegree를 모두 더함
        idg[i] = indegree;
    }
}

void sort_node(int* arr, int n){    // 그래프의 노드들을 오름차순으로 정렬
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-1-i;j++)
        {
            if(arr[j]>arr[j+1]){
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}