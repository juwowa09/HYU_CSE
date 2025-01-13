#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;
	int dist;	//distance
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** vertices;
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);
int findPath(Graph* g, Node n);
int findNode(Heap* h, int vertex);

int main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d", &size);
	g = createGraph(size + 1);

    char tmp = 0;
    while (tmp != '\n' && tmp != EOF) {
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g->vertices[node1][node2] = weight;
        tmp = fgetc(fin);
    }

	printShortestPath(g);

	deleteGraph(g);

    fclose(fin);
    fclose(fout);
}

Graph* createGraph(int size)    // Create Graph that vertices size = size x size, Node size = size.
{
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->size = size-1;   // parameter 가 size + 1 로 들어왔으므로 -1
    g->nodes = (Node*)malloc(sizeof(Node) * size-1);

    g->vertices = (int**)malloc(sizeof(int*) * size);
    for(int i=0;i<size-1;i++)
    {
        g->vertices[i] = (int*)malloc(sizeof(int)*size);
        g->nodes[i].vertex = i+1;
        g->nodes[i].dist = INF;     // grpah의 초기 상태의 모든 노드의 dist는 무한대 (INF)
        g->nodes[i].prev = 0;       // 이전 노드(방향성)도 없으므로 0 으로 둠.
    }
    g->vertices[size-1] = (int*)malloc(sizeof(int)*size); // edge를 모자라게 동적할당 했으므로 하나 더 동적할당.

    for(int i=0; i<size; i++)   //  모든 edge 초기화
    {
        for(int j=0; j<size; j++)
        {g->vertices[i][j] = 0;}
    }
    return g;
}

void deleteGraph(Graph* g)  // graph에 동적할당한 메모리 모두 해제
{
    free(g->nodes);
    for(int i=0;i< g->size+1; i++)
    {free(g->vertices[i]);}
    free(g->vertices);
    free(g);
}

int findPath(Graph* g, Node n){     // 최단거리 path에 해당하는 노드들을 찾아줄 재귀 함수
    if(n.vertex == 1){      // prev를 따라와서 1을 만난 경우 path가 존재
        fprintf(fout, "1");
        return 1;
    }
    if(n.prev == 0){    // prev가 존재 x -> 1에서 부터 path가 끊겨있는 경우
        return 0;       // return 0
    }

    if(findPath(g, g->nodes[n.prev-1]) == 0){ // prev(이전노드)를 따라서 재귀 실행. 0값이면 path 존재 x
        return 0;
    }
    else {  // 0이 아닌 값이 나올 시 prev에 연결되어 있음. return 값은 해당 노드의 distance
        fprintf(fout, "->%d",n.vertex);
        return n.dist;
    }
}

void printShortestPath(Graph* g)
{
    Heap* h = createMinHeap(g->size);
    g->nodes[0].dist = 0;   // 1번 노드의 dist = INF -> 0 설정
    insertToMinHeap(h, 1, 0);   // 맨 첫 노드 heap에 삽입
    int vCnt = 0;
    while(vCnt < g->size) //
    {
        Node n = deleteMin(h);  // 하나 추출
        vCnt++;
        for(int i=1; i<=g->size; i++)
        {
            // 추출한 노드에서 연결된 노드를 찾고, 연결된 노드의 weight edge + 현재 노드dist 가 그 노드가 가지고있는 dist보다 작으면 교체.
            if(g->vertices[n.vertex][i] != 0 && (g->nodes[n.vertex-1].dist + g->vertices[n.vertex][i] < g->nodes[i-1].dist))
            {
                insertToMinHeap(h, i, g->nodes[n.vertex-1].dist + g->vertices[n.vertex][i]);
                g->nodes[i-1].dist = g->vertices[n.vertex][i] + g->nodes[n.vertex-1].dist;  // dist 갱신
                g->nodes[i-1].prev = n.vertex; // 이전 노드 연결
            }
        }
    }
    for(int i = 1; i<g->size; i++){ // 각 노드에 대한 path 찾기
        int d = findPath(g,g->nodes[i]);
        if(d == 0){
            fprintf(fout, "can not reach to node %d", g->nodes[i].vertex);  // path 없으면 이거 출력
        }
        else{ fprintf(fout, " (cost : %d)",d);}  // 있으면 길이 출력
        fprintf(fout, "\n");
    }
    deleteMinHeap(h);
}

Heap* createMinHeap(int heapSize)   // heap 생성
{
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->Capacity = heapSize;
    h->Size = -1;
    h->Element = (Node*)malloc(sizeof(Node) * (heapSize));
    return h;
}

void insertToMinHeap(Heap* minHeap, int vertex, int distance)
{
    int pi;
    int ci;
    int vi = findNode(minHeap, vertex);     // heap 속에 이미 vertex가 존재하는 경우 ( 해당 노드에 연결된 edge 변경 )

    if(vi){             // size를 추가하지 않고 dist갱신, minheap을 변경
        ci = vi-1;      // 반환된 값이 +1 되어있으므로 -1 처리
        pi = (ci-1)/2;
        minHeap->Element[vi-1].dist = distance;
    }
    else        // heap 속에 없는 경우는 새로 insert.
    {
        minHeap->Size++;
        minHeap->Element[minHeap->Size].vertex = vertex;    // 마지막 index에 insert
        minHeap->Element[minHeap->Size].dist = distance;

        pi = (minHeap->Size - 1) / 2;       // parent node의 index
        ci = minHeap->Size;             // 현재 index
    }

    while(minHeap->Element[ci].dist < minHeap->Element[pi].dist && ci != 0) // ci = 0 일 경우, 맨위까지 올라왔으므로 종료.
    {                                                                       // ci node의 dist pi보다 작으면 교체
        Node tmp = minHeap->Element[ci];
        minHeap->Element[ci] = minHeap->Element[pi];
        minHeap->Element[pi] = tmp;
        ci = pi;
        pi = (ci-1)/2;
    }
}
int findNode(Heap* h, int vertex){
    for(int i=0;i<=h->Size;i++)
    {
        if(h->Element[i].vertex == vertex)  // 이미 heap속에 해당 vertex가 존재하는 경우
        {return i+1;}                         // 그 vertex의 index를 반환 ( 0 이 반환될 수도 있으므로 +1 )
    }
    return 0;
}

Node deleteMin(Heap* minHeap)
{
    if(minHeap->Size == -1){ exit(0); }
    int pi = 0;
    int ci;
    int rci;
    int lci;
    Node n = minHeap->Element[pi];  // 삭제할 노드
    minHeap->Element[pi] = minHeap->Element[minHeap->Size]; // 맨아래 노드를 맨위로 끌고 올라옴
    minHeap->Size--;    // 사이즈 하나 줄이기
    while(pi < minHeap->Size)
    {
        ci = 0;
        rci = pi * 2 + 1;
        lci = pi * 2 + 2;
        if(rci<=minHeap->Size)      // complete binary 트리의 오른쪽 자식까지 있는 경우
        {
            ci = minHeap->Element[rci].dist < minHeap->Element[lci].dist ? rci : lci;   // dist가 더 작은 ci의 index 저장
            if(minHeap->Element[ci].dist < minHeap->Element[pi].dist)   //  ci dist가 pi 보다 작으면 교체.
            {
                Node tmp = minHeap->Element[ci];
                minHeap->Element[ci] = minHeap->Element[pi];
                minHeap->Element[pi] = tmp;
            }
            else return n;
        }
        else if(lci == minHeap->Size)   // complete binay트리 왼쪽 자식까지만 있는경우 ( 오른쪽 자식 존재 x )
        {
            ci = lci;
            if(minHeap->Element[ci].dist < minHeap->Element[pi].dist)   // lci dist가 더 작으면 교체
            {
                Node tmp = minHeap->Element[ci];
                minHeap->Element[ci] = minHeap->Element[pi];
                minHeap->Element[pi] = tmp;
            }
            else return n;
        }
        else return n;  // 자식이 없는 경우 ( 다 내려온경우 )
        pi = ci;
    }
    return n;
}

void deleteMinHeap(Heap* minHeap)   // heap에 동적할당 메모리를 해제
{
    free(minHeap->Element);
    free(minHeap);
}