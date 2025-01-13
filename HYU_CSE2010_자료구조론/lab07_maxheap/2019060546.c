#include<stdio.h>
#include<stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
    int Capacity;
    int Size;
    int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap *heap);
int IsEmpty(Heap *heap);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    Heap* maxHeap;
    int heapSize, key, max_element;

    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'n':
                fscanf(fin, "%d", &heapSize);
                maxHeap = CreateHeap(heapSize);
                break;
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(maxHeap, key);
                break;
            case 'd':
                max_element = DeleteMax(maxHeap);
                if(max_element != -INF){
                    fprintf(fout, "max element : %d deleted\n", max_element);
                }
                break;
            case 'p':
                PrintHeap(maxHeap);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
                else fprintf(fout, "finding error : %d is not in the heap\n", key);
                break;
        }
    }

    return 0;
}
// Create Heap Capacity with parameter heapSize
// empty Heap Size = -1 becauese 1 elemente Size is 0
Heap* CreateHeap(int heapSize){
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->Capacity = heapSize;
    h->Size = -1;
    h->Elements = (int*)malloc((sizeof(int) * heapSize));
    return h;
}

void Insert(Heap *heap, int value)
{       // not Find, print error message
    if (Find(heap, value)){
        fprintf(fout, "insert error : %d is already in the heap\n", value);
        return;
    }   // if heap be Full state, print error message
    if (IsFull(heap)){
        fprintf(fout, "insert error : heap is full\n");
        return;
    }
    // insert element
    heap->Elements[++heap->Size] = value;
    fprintf(fout, "insert %d\n", value);
    int ci = heap->Size;
    // perform percolate Up
    while (ci != 0){
        // parent index pi
        int pi = (ci - 1) / 2;
        // exchange child and parent if child has higher priority
        if (heap->Elements[ci] > heap->Elements[pi]){
            int temp = heap->Elements[ci];
            heap->Elements[ci] = heap->Elements[pi];
            heap->Elements[pi] = temp;
        }
        ci = pi;
        // repeat until ci == 0
    }
}

int Find(Heap* heap, int value){
    for(int i=0; i <= heap->Size; i++){
        // exist Elements return true
        if(heap->Elements[i] == value) return 1;
    }
    // not exist Element, return false
    return 0;
}

int DeleteMax(Heap* heap)
{   // emtpy state try delete, print error message.
    if (IsEmpty(heap))
    {
        fprintf(fout, "delete error : heap is empty\n");
        return -INF;
    }
    // d is element deleted, root
    int d = heap->Elements[0];
    // last element to root node element
    heap->Elements[0] = heap->Elements[heap->Size];
    heap->Elements[heap->Size] = 0;
    // size decrement
    heap->Size -= 1;
    int pi = 0;
    // repeat when pi index lower than Size
    while (pi<heap->Size){
        int ci = 0;
        int lci = pi * 2 + 1;   // left child index
        int rci = pi * 2 + 2;   // right child index
        int a = 0;
        // left right child noth exist
        if (rci <= heap->Size)
        {   // compare left right child priority
            a = heap->Elements[lci] > heap->Elements[rci] ? heap->Elements[lci] : heap->Elements[rci];
            if (a == heap->Elements[lci]) ci = lci;
            else ci = rci;
            // child has higher priority than parent, change
            // perform percolate down
            if(a > heap->Elements[pi]){
                int temp = heap->Elements[ci];
                heap->Elements[ci] = heap->Elements[pi];
                heap->Elements[pi] = temp;
                pi = ci;
            }
            // child priority less than parent, exit percolate down
            else return d;
        }
        else if (lci == heap->Size) // only left child exist
        {
            a = heap->Elements[lci];
            // left child higher priority than parent, change
            // perform percolate down
            if(a > heap->Elements[pi]){
                int temp = heap->Elements[lci];
                heap->Elements[lci] = heap->Elements[pi];
                heap->Elements[pi] = temp;
                pi = lci;
            }
            // child priority less than parent, exit percolate down
            else return d;
        }
        //  dont have child, exit.
        else{return d;}
    }
    return d;
}

void PrintHeap(Heap* heap){
    // try print Empty Heap, print error message
    if(IsEmpty(heap)){ fprintf(fout, "print error : heap is empty\n"); return; }
    // print order higher priority = level order
    for(int i=0;i<=heap->Size;i++)
    {
        fprintf(fout, "%d ",heap->Elements[i]);
    }
    fprintf(fout, "\n");
}

int IsFull(Heap* heap){
    // heap is Full state, return true
    if (heap->Capacity-1 == heap->Size) return 1;
    // heap is not Full state, return false
    return 0;
}

int IsEmpty(Heap *heap){
    // heap is empty state, Size = -1, return true
    if(heap->Size == -1) return 1;
    // heap is not empty, return false
    return 0;
}