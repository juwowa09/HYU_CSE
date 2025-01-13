#include <stdlib.h>
#include <stdio.h>

void test_leak() {
    int *ptr = (int *)malloc(10 * sizeof(int));
    ptr[0] = 42;
    // free(ptr); // 의도적으로 free 누락
}

int main() {
    test_leak();
    return 0;
}
