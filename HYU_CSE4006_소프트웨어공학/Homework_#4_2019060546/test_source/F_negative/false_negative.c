#include <stdlib.h>
#include <stdio.h>

void forget_to_free() {
    int *data = NULL;
    // free(data); // 메모리 해제 누락
    int j = 10;
    while(j > 0){
    	data = (int*)malloc(sizeof(int) *5);
    	j--;
    }
    free(data);
}

int main() {
	int i;
	scanf("%d",&i);
	if(i > 0)
	{
		forget_to_free();
	}
    return 0;
}
