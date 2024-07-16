#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);
	return 0;
}

Array* CreateArray(int size)    // 총 size만큼의 메모리 할당
{
    Array* arr = (Array*)malloc(sizeof(Array));
    arr->values = (int*)malloc(sizeof(int)*size);
    arr->size = size;
    return arr;
}

void QuickSort(Array* array, int left, int right, int depth)    // Quick sort를 재귀적으로 수행
{
    if(left >= right) return;       // sort 할 것이 안꼬여있는 경우만 수행

    int p = Partition(array, left, right, depth);   // array를 sort수행 후 기준으로 잡고 앞뒤로 나눠 재귀적으로 sort할 index p 할당
    QuickSort(array, left, p-1,depth+1);    // 처음부터 p-1까지를 sort 수행
    QuickSort(array, p+1, right, depth+1);      // p+1부터 끝까지 sort 수행
}

int Partition(Array* array, int left, int right, int depth)
{
    int i = left;
    int j = right-1;

    fprintf(fout, "left : %d, right : %d, pivot : %d\n",left, right, array->values[right]);
    PrintArray(array, left, right);

    while(i <= j){
        if(array->values[i] > array->values[right]){}   // 처음부터 pivot보다 큰 index 찾을 때 까지 탐색
        else {i++;}

        if(array->values[j] < array->values[right]){}   // 끝부터 pivot보다 작은 index 찾을 때 까지 탐색
        else {j--;}

        if(i>j) break;      // 교차 됐을 경우 반복문 탈출

        if(array->values[i] > array->values[right] && array->values[j] < array->values[right]){ // pivot보다 작고 큰거 찾으면 교체
            swap(&array->values[i], &array->values[j]);
            PrintArray(array, left, right);
        }
    }

    if(array->values[i]>array->values[right])
    {
        swap(&array->values[i], &array->values[right]);  // i j 교차 시 pivot 과 i index 교체
        PrintArray(array, left, right);
    }
    return i;
}

void PrintArray(Array* array, int left, int right)
{
    for(int i=0; i<array->size; i++){
        if(i>=left && i<=right){
            fprintf(fout, "%-3d", array->values[i]);
        }
        else fprintf(fout, "%-3s","-");
    }fprintf(fout, "\n");
}

void DeleteArray(Array* array)
{
    free(array->values);
    free(array);
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
