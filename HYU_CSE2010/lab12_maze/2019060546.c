#include<stdio.h>
#include<stdlib.h>
#include<time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
int Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);

int main(int argc, char* agrv[]) {
    srand((unsigned int)time(NULL));

    int num;
    fin = fopen(agrv[1], "r");
    fout = fopen(agrv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *sets, DisjointSets *maze_print, int num){
	int i;

	sets->size_maze = num * num;
	sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze + 1));
	for (i = 1; i <= sets->size_maze; i++) sets->ptr_arr[i] = i;

	maze_print->size_maze = num * num * 2;
	maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze + 1));
	for (i = 1; i <= maze_print->size_maze; i++) maze_print->ptr_arr[i] = 1;
	//maze_print->ptr_arr[(x-1)*2+1 ~ x*2] : two directions(right, down) walls of the number x

	//start and end position have not to have the wall
    maze_print->ptr_arr[0] = 0;
	maze_print->ptr_arr[num*num] = 0;
}

int Union(DisjointSets *sets, int i, int j) // 두개가 disjoint 라면 union
{
    int root1 = find(sets, i);
    int root2 = find(sets,j);

    if(root1 != root2)
    {
        if(sets->ptr_arr[root1] > 0 && sets->ptr_arr[root2] > 0){   // depth가 0인 경우
            sets->ptr_arr[root1] = root2;
            sets->ptr_arr[root2] = -1;
        }else if(sets->ptr_arr[root1] < sets->ptr_arr[root2]){  // root1의 depth가 더 깊은경우
            sets->ptr_arr[root2] = root1;
        }else if(sets->ptr_arr[root1] > sets->ptr_arr[root2]){  // root2의 depth가 더 깊은경우
            sets->ptr_arr[root1] = root2;
        }else{
            sets->ptr_arr[root1] = root2;       // depth가 동일한 경우
            sets->ptr_arr[root2]--;
        }
        return 1;
    }
    return 0;
}

int find(DisjointSets *sets, int i) // set의  ptr arr = root인가?  // random 함수
{
    int root = sets->ptr_arr[i];
    if(root == i || root < 0) return i;  // 본인이 root인 경우 본인 return

    while(sets->ptr_arr[root] > 0)
    {root = sets->ptr_arr[root];}
    return root;
}

void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num)
{
    srand(time(NULL));
    while(find(sets, 1) != find(sets, num*num))
    {
        int i = rand() % (num*num) + 1; // random cell select

        while(1){
            int j = rand() % 4 + 1;     // random direction select
            if (j == 1){    // right direction
                if(i % num == 0) continue;
                else{
                    if(Union(sets,i,i+1))   // 합치기 성공한경우
                    {maze_print->ptr_arr[i]=0;} // 벽 허물기
                }
            }
            else if (j == 2){   // down direction
                if(i + num > num*num) continue;
                else{
                    if(Union(sets,i,i+num))
                    {maze_print->ptr_arr[i+num*num]=0;}
                }
            }
            else if (j == 3){   // left direction
                if(i % num == 1) continue;
                else{
                    if(Union(sets,i,i-1))
                    {maze_print->ptr_arr[i-1]=0;}
                }
            }
            else if (j == 4){   // up direction
                if(i - num <= 0)continue;
                else{
                    if(Union(sets, i,i-num))
                    {maze_print->ptr_arr[i-num+num*num]=0;}
                }
            }break;
        }
    }
}

void printMaze(DisjointSets *sets, int num)
{
    for(int i=0;i<num;i++)
    {
        fprintf(fout, "+---");  // 첫줄 print
    }fprintf(fout, "+\n");

    for(int i=0;i<num;i++)
    {
        if(i==0) fprintf(fout, " ");    // 입구 뚫기
        else fprintf(fout, "|");
        for(int j = 1; j<=num; j++){
            fprintf(fout, "   ");   //  길 생성
            if(sets->ptr_arr[i*num+j]==1) fprintf(fout, "|");   // 오른쪽 벽있으면 벽 출력
            else fprintf(fout, " ");    // 없으면 벽 뚫기
        }fprintf(fout, "\n+");
        for(int j=1; j<=num; j++){  // 위아래 벽
            if(sets->ptr_arr[num*num+i*num+j] == 1)
            {fprintf(fout, "---+");}    // 벽 존재
            else fprintf(fout, "   +"); // 없으면 뚫기
        }fprintf(fout, "\n");
    }
}

void freeMaze(DisjointSets *sets, DisjointSets *maze_print)
{
    free(sets->ptr_arr);
    free(maze_print->ptr_arr);
    free(sets);
    free(maze_print);
}