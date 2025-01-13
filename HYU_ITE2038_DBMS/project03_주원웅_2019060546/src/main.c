#include "bpt.h"

int main()
{
    int64_t input;
    char instruction;
    char buf[120];
    char *result;
    open_table("DB[2019060546].db");
    while (scanf("%c", &instruction) != EOF)
    {
        switch (instruction)
        {
        case 'i':
            scanf("%ld %s", &input, buf);
            db_insert(input, buf);
            break;
        case 'f':
            scanf("%ld", &input);
            result = db_find(input);
            if (result)
            {
                printf("Key: %ld, Value: %s\n", input, result);
                free(result);
            }
            else
                printf("Not Exists\n");

            fflush(stdout);
            break;
        case 'd':
            scanf("%ld", &input);
            db_delete(input);
            break;
        case 'q':
            while (getchar() != (int)'\n')
                ;
            return EXIT_SUCCESS;
            break;
        }
        while (getchar() != (int)'\n')
            ;
    }
    printf("\n");
    return 0;
}
