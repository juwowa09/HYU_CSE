#include "bpt.h"

H_P *hp;

page *rt = NULL; // root is declared as global

int fd = -1; // fd is declared as global

H_P *load_header(off_t off)
{
    H_P *newhp = (H_P *)calloc(1, sizeof(H_P));
    if (sizeof(H_P) > pread(fd, newhp, sizeof(H_P), 0))
    {

        return NULL;
    }
    return newhp;
}

page *load_page(off_t off)
{
    page *load = (page *)calloc(1, sizeof(page));
    // if (off % sizeof(page) != 0) printf("load fail : page offset error\n");
    if (sizeof(page) > pread(fd, load, sizeof(page), off))
    {

        return NULL;
    }
    return load;
}

int open_table(char *pathname)
{
    fd = open(pathname, O_RDWR | O_CREAT | O_EXCL | O_SYNC, 0775);
    hp = (H_P *)calloc(1, sizeof(H_P));
    if (fd > 0)
    {
        // printf("New File created\n");
        hp->fpo = 0;
        hp->num_of_pages = 1;
        hp->rpo = 0;
        pwrite(fd, hp, sizeof(H_P), 0);
        free(hp);
        hp = load_header(0);
        return 0;
    }
    fd = open(pathname, O_RDWR | O_SYNC);
    if (fd > 0)
    {
        // printf("Read Existed File\n");
        if (sizeof(H_P) > pread(fd, hp, sizeof(H_P), 0))
        {
            return -1;
        }
        off_t r_o = hp->rpo;
        rt = load_page(r_o); // rt != not null
        // printf("%p\n", rt);
        // if (rt == NULL)
        //     printf("null\n");
        // else{
        //     printf("not null\n");
        // }
        // printf("%d\n", rt->num_of_keys);
        return 0;
    }
    else
        return -1;
}

void reset(off_t off)
{
    page *reset;
    reset = (page *)calloc(1, sizeof(page));
    reset->parent_page_offset = 0;
    reset->is_leaf = 0;
    reset->num_of_keys = 0;
    reset->next_offset = 0;
    pwrite(fd, reset, sizeof(page), off);
    free(reset);
    return;
}

void freetouse(off_t fpo)
{
    page *reset;
    reset = load_page(fpo);
    reset->parent_page_offset = 0;
    reset->is_leaf = 0;
    reset->num_of_keys = 0;
    reset->next_offset = 0;
    pwrite(fd, reset, sizeof(page), fpo);
    free(reset);
    return;
}

void usetofree(off_t wbf)
{
    page *utf = load_page(wbf);
    utf->parent_page_offset = hp->fpo;
    utf->is_leaf = 0;
    utf->num_of_keys = 0;
    utf->next_offset = 0;
    pwrite(fd, utf, sizeof(page), wbf);
    free(utf);
    hp->fpo = wbf;
    pwrite(fd, hp, sizeof(hp), 0);
    free(hp);
    hp = load_header(0);
    return;
}

off_t new_page()
{
    off_t newp;
    page *np;
    off_t prev;
    if (hp->fpo != 0)
    {
        newp = hp->fpo;
        np = load_page(newp);
        hp->fpo = np->parent_page_offset;
        pwrite(fd, hp, sizeof(hp), 0);
        free(hp);
        hp = load_header(0);
        free(np);
        freetouse(newp);
        return newp;
    }
    // change previous offset to 0 is needed
    newp = lseek(fd, 0, SEEK_END);
    // if (newp % sizeof(page) != 0) printf("new page made error : file size error\n");
    reset(newp);
    hp->num_of_pages++;
    pwrite(fd, hp, sizeof(H_P), 0);
    free(hp);
    hp = load_header(0);
    return newp;
}

int cut(int length)
{
    if (length % 2 == 0)
        return length / 2;
    else
        return length / 2 + 1;
}

void start_new_file(record rec)
{

    page *root;
    off_t ro;
    ro = new_page();
    rt = load_page(ro);
    hp->rpo = ro;
    pwrite(fd, hp, sizeof(H_P), 0);
    free(hp);
    hp = load_header(0);
    rt->num_of_keys = 1;
    rt->is_leaf = 1;
    rt->records[0] = rec;
    pwrite(fd, rt, sizeof(page), hp->rpo);
    free(rt);
    rt = load_page(hp->rpo);
    // printf("new file is made\n");
}

record *create_rec(int64_t key, char *value)
{
    record *nrec = (record *)malloc(sizeof(record));
    if (nrec == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    nrec->key = key;
    strcpy(nrec->value, value);

    return nrec;
}

// void printTree()
// {
//     page *cur = load_page(hp->rpo);
//     off_t cur_off = hp->rpo; // 없어도 됨

//     while (!cur->is_leaf)
//     {
//         printf("num: %d ,inner key:", cur->num_of_keys);
//         for (int i = 0; i < cur->num_of_keys; i++)
//             printf(" %ld", cur->b_f[i]);
//         printf("\n");
//         cur = load_page(cur->next_offset);
//     }

//     while (cur->next_offset != 0)
//     {
//         printf("key:");
//         for (int i = 0; i < cur->num_of_keys; i++)
//         {
//             printf(" %ld", cur->records[i].key);
//         }
//         printf("\n");
//         cur = load_page(cur->next_offset);
//     }
//     printf("key:");
//     for (int i = 0; i < cur->num_of_keys; i++)
//     {
//         printf(" %ld ", cur->records[i].key);
//     }
//     printf("\n");
//     free(cur);
// }

// void printTree2(off_t cur_off)
// {
//     page *cur = load_page(cur_off);
//     printf("p: %lld c: %lld ", cur->parent_page_offset, cur_off);
//     if (cur->is_leaf)
//     {
//         printf("n: %d r key: ", cur->num_of_keys);
//         for (int i = 0; i < cur->num_of_keys; i++)
//             printf("%lld ", cur->records[i].key);
//         printf("\n");
//         return;
//     }
//     if (!cur->is_leaf)
//     {
//         printf("n: %d i key: ", cur->num_of_keys);
//         for (int i = 0; i < cur->num_of_keys; i++)
//             printf("%lld ", cur->b_f[i].key);
//         printf("\n");

//         printTree2(cur->next_offset);
//         for (int i = 0; i < cur->num_of_keys; i++)
//             printTree2(cur->b_f[i].p_offset);
//     }
// }

char *db_find(int64_t key)
{

    if (rt == NULL)
        return 0;

    page *cur = load_page(hp->rpo);
    off_t cur_off = hp->rpo; // 없어도 됨
    off_t prev_off = 0;

    char *result = (char *)malloc(sizeof(char) * 120);
    while (!cur->is_leaf)
    {
        if (cur->parent_page_offset != prev_off)
        {
            cur->parent_page_offset = prev_off;
            pwrite(fd, cur, sizeof(page), cur_off);
        }
        if (key < cur->b_f[0].key)
        {
            prev_off = cur_off;
            cur_off = cur->next_offset;
            cur = load_page(cur->next_offset);
        }
        else
        {
            int flag = 0;
            for (int i = 1; i < cur->num_of_keys; i++)
            {
                if (key < cur->b_f[i].key)
                {
                    prev_off = cur_off;
                    cur_off = cur->b_f[i - 1].p_offset;
                    cur = load_page(cur->b_f[i - 1].p_offset);
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                prev_off = cur_off;
                cur_off = cur->b_f[cur->num_of_keys - 1].p_offset;
                cur = load_page(cur->b_f[cur->num_of_keys - 1].p_offset);
            }
        }
    }
    cur->parent_page_offset = prev_off;
    pwrite(fd, cur, sizeof(page), cur_off);

    for (int i = 0; i < cur->num_of_keys; i++)
    {
        if (cur->records[i].key == key)
        {
            // printf("%p\n", rt->parent_page_offset);
            // printf("entire page: %d, rt offset: %p, cur offset: %p, num_of_keys: %d, key: %d, value:%s\n", hp->num_of_pages, hp->rpo, cur_off, cur->num_of_keys, cur->records[i].key, cur->records[i].value); // 확인용 코드
            // printTree2(hp->rpo);
            // printTree();
            strcpy(result, cur->records[i].value);
            free(cur);
            return result;
        }
    }
    return 0;
}

void split_insert(page *cur, page *new_p, int key, off_t new_p_off)
{
    if (cur->parent_page_offset == 0) // cur 가 root 인 경우
    {
        // printf("%d %d\n", hp->rpo, rt->parent_page_offset);
        off_t new_rt_off = new_page();
        page *new_rt = load_page(new_rt_off);

        new_rt->is_leaf = 0;
        new_rt->next_offset = new_rt_off;
        new_rt->num_of_keys = 1;
        new_rt->b_f[0].p_offset = new_p_off;
        new_rt->b_f[0].key = key;
        new_p->parent_page_offset = hp->rpo;
        cur->parent_page_offset = hp->rpo;
        new_rt->parent_page_offset = 0;
        pwrite(fd, new_rt, sizeof(page), hp->rpo);
        pwrite(fd, cur, sizeof(page), new_rt_off);
        pwrite(fd, new_p, sizeof(page), new_p_off);
        free(new_rt);
        free(rt);
        rt = load_page(hp->rpo);
        return;
    }
    page *prev = load_page(cur->parent_page_offset);
    I_R tmp[INTERNAL_MAX + 1];
    int cur_pos = prev->num_of_keys;

    for (int i = 0; i < prev->num_of_keys; i++)
    {
        tmp[i] = prev->b_f[i];
        // printf("c[0]: %d, p[i]: %d, %d\n", cur->records[0].key, prev->b_f[i].key, i);
        if (cur->is_leaf)
        {
            if (cur->records[0].key < prev->b_f[i].key)
            {
                cur_pos = i;
                break;
            }
        }
        else
        {
            if (cur->b_f[0].key < prev->b_f[i].key)
            {
                cur_pos = i;
                break;
            }
        }
    }

    for (int i = prev->num_of_keys; i > cur_pos; i--)
    {
        tmp[i] = prev->b_f[i - 1];
    }

    // printf("cur_pos: %d, key: %d\n", cur_pos, key);
    tmp[cur_pos].key = key;
    tmp[cur_pos].p_offset = new_p_off;

    if (prev->num_of_keys >= INTERNAL_MAX)
    { // parent split
        off_t new_off = new_page();
        page *new_pp = load_page(new_off);
        new_pp->is_leaf = 0;
        new_pp->num_of_keys = 0;
        new_pp->parent_page_offset = prev->parent_page_offset;
        prev->num_of_keys = 0;

        for (int i = 0; i <= INTERNAL_MAX; i++)
        {
            // printf("%d ", tmp[i]);
            if (i < INTERNAL_MAX / 2)
            {
                prev->num_of_keys++;
                prev->b_f[i] = tmp[i];
            }
            else if (i == INTERNAL_MAX / 2)
            {
                new_pp->next_offset = tmp[i].p_offset;
            }
            else
            {
                new_pp->num_of_keys++;
                new_pp->b_f[i - INTERNAL_MAX / 2 - 1] = tmp[i];
            }
        }
        // printf("tmp \n");

        {
            pwrite(fd, new_pp, sizeof(page), new_off);
            pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
            split_insert(prev, new_pp, tmp[INTERNAL_MAX / 2].key, new_off);
            free(prev);
            free(new_pp);
        }
    }
    else // 그냥 삽입
    {
        // printf("just insert\n");
        prev->num_of_keys++;
        for (int i = 0; i < prev->num_of_keys; i++)
        {
            prev->b_f[i] = tmp[i];
            // printf("%d ", tmp[i]);
        }
        // printf("\n");
        pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
        free(prev);
    }
}

int db_insert(int64_t key, char *value)
{
    if (rt == NULL)
    {
        record *rec = create_rec(key, value);
        start_new_file(*rec);
        return 0;
    }

    page *cur = load_page(hp->rpo); // rt 로 설정 처음에
    off_t cur_off = hp->rpo;        // 현재 offset
    off_t prev_off = 0;
    int cur_pos;

    while (!cur->is_leaf)
    {
        cur_pos = 0;
        if (cur->parent_page_offset != prev_off)
        {
            cur->parent_page_offset = prev_off;
            pwrite(fd, cur, sizeof(page), cur_off);
        }

        if (key < cur->b_f[0].key)
        {
            prev_off = cur_off;
            cur_off = cur->next_offset;
            cur = load_page(cur->next_offset);
        }
        else
        {
            int flag = 0;
            for (int i = 1; i < cur->num_of_keys; i++)
            {
                if (key < cur->b_f[i].key) // 현재 키보다 작으면 i-1 포인터로 이동
                {
                    prev_off = cur_off;
                    cur_off = cur->b_f[i - 1].p_offset;
                    cur = load_page(cur->b_f[i - 1].p_offset);
                    flag = 1;
                    cur_pos = i;
                    break;
                }
            }
            if (!flag) // 맨 마지막 포인터
            {
                prev_off = cur_off;
                cur_off = cur->b_f[cur->num_of_keys - 1].p_offset;
                cur_pos = cur->num_of_keys;
                cur = load_page(cur->b_f[cur->num_of_keys - 1].p_offset);
            }
        }
    } // 여기까지 위치 찾기
    cur->parent_page_offset = prev_off;

    int pos = cur->num_of_keys;

    if (cur->is_leaf && cur->num_of_keys >= LEAF_MAX) // 현재 page 는 leaf, 꽉차있는 경우
    {

        record tmp[LEAF_MAX + 1];

        for (int i = 0; i < LEAF_MAX; i++)
        {
            if (key == cur->records[i].key)
            {
                // printf("no input\n");
                return -1;
            }
            if (key < cur->records[i].key)
            {
                pos = i;
                break;
            }
            tmp[i] = cur->records[i];
        }

        for (int i = LEAF_MAX; i > pos; i--)
        {
            tmp[i] = cur->records[i - 1];
        }

        tmp[pos].key = key;
        strcpy(tmp[pos].value, value); // 새롭게 정렬

        if (cur_off == hp->rpo) // 루트 노드가 leaf 의 경우 바로 스플릿
        {
            off_t new_off = new_page();
            page *new_p = load_page(new_off);
            off_t new_off2 = new_page();
            page *new_p2 = load_page(new_off2);

            for (int i = 0; i <= LEAF_MAX; i++) // split
            {
                if (i <= LEAF_MAX / 2)
                {
                    new_p->records[i] = tmp[i];
                    new_p->num_of_keys++;
                }
                else
                {
                    new_p2->records[i - (LEAF_MAX / 2) - 1] = tmp[i];
                    new_p2->num_of_keys++;
                }
            }
            new_p->is_leaf = 1;
            new_p2->is_leaf = 1;
            new_p->next_offset = new_off2;
            new_p2->next_offset = 0;
            new_p->parent_page_offset = hp->rpo;
            new_p2->parent_page_offset = hp->rpo;
            rt->is_leaf = 0;
            rt->num_of_keys = 1;
            rt->b_f[0].key = new_p2->records[0].key;
            rt->b_f[0].p_offset = new_off2;
            rt->next_offset = new_off;
            pwrite(fd, rt, sizeof(page), hp->rpo);
            free(rt);
            rt = load_page(hp->rpo);
            pwrite(fd, new_p, sizeof(page), new_off);
            pwrite(fd, new_p2, sizeof(page), new_off2);
            free(new_p2);
            free(new_p);
            free(cur);
        }
        else
        { // sibling 이 존재하는 경우 == 부모 inner 노드가 존재하는 경우
            page *prev = load_page(cur->parent_page_offset);
            int flag = 0;

            // printf("GOOD cur_pos : %d, parent key: %d\n", cur_pos, prev->b_f[0].key);
            if (cur_pos < prev->num_of_keys) // 가장 오른쪽이 아닌 경우
            {
                flag = 1;
                page *right = load_page(prev->b_f[cur_pos].p_offset);
                if (right->num_of_keys == LEAF_MAX) // 오른쪽이 꽉 차있는 경우 왼쪽을 봐야 함
                    flag = 0;
                else // 아닌 경우 rotation 진행.
                {
                    // printf("right\n");
                    for (int i = 0; i < LEAF_MAX; i++)
                    {
                        cur->records[i] = tmp[i];
                    }

                    for (int i = right->num_of_keys; i > 0; i--)
                    {
                        right->records[i] = right->records[i - 1];
                    }

                    right->records[0] = tmp[LEAF_MAX];
                    right->num_of_keys++;
                    if (cur_pos > 0)
                        prev->b_f[cur_pos - 1].key = cur->records[0].key;
                    prev->b_f[cur_pos].key = right->records[0].key;

                    pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                    free(prev);
                    pwrite(fd, right, sizeof(page), prev->b_f[cur_pos].p_offset);
                    free(right);
                    pwrite(fd, cur, sizeof(page), cur_off);
                    free(cur);
                }
            }
            if (flag == 0 || cur_pos == prev->num_of_keys) // 오른쪽 sibling이 없거나 오른쪽 sibling의 자리가 없는 경우 왼쪽 보기
            {

                if (cur_pos == 0) // 왼쪽도 없는 경우(cur == 가장 왼쪽의 경우) 설정
                    flag = 2;
                else
                {

                    page *left;
                    off_t left_off;
                    if (cur_pos == 1) // cur 의 왼쪽이 첫번째(0)인 경우
                    {
                        left = load_page(prev->next_offset);
                        left_off = prev->next_offset;
                    }
                    else
                    {
                        left = load_page(prev->b_f[cur_pos - 2].p_offset);
                        left_off = prev->b_f[cur_pos - 2].p_offset;
                    }
                    if (left->num_of_keys == LEAF_MAX) // 왼쪽 노드의 키가 다 차있는 경우 스플릿
                        flag = 2;
                    else // 아닌 경우 rotation 진행.
                    {
                        // printf("left\n");
                        for (int i = 0; i < LEAF_MAX; i++)
                            cur->records[i] = tmp[i + 1];

                        prev->b_f[cur_pos - 1].key = cur->records[0].key;
                        left->records[left->num_of_keys] = tmp[0];
                        left->num_of_keys++;
                        pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                        free(prev);
                        pwrite(fd, left, sizeof(page), left_off);
                        free(left);
                        pwrite(fd, cur, sizeof(page), cur_off);
                        free(cur);
                    }
                }
            }
            if (flag == 2) // 왼쪽 sibling이 없거나 왼쪽 sibling의 자리가 없는 경우 스플릿
            {
                // printf("split\n");
                off_t new_off = new_page();
                page *new_p = load_page(new_off);
                cur->num_of_keys = 0;
                new_p->parent_page_offset = cur->parent_page_offset;
                new_p->is_leaf = 1;
                new_p->num_of_keys = 0;
                new_p->next_offset = cur->next_offset;
                cur->next_offset = new_off;
                for (int i = 0; i <= LEAF_MAX; i++) // split
                {
                    if (i <= LEAF_MAX / 2)
                    {
                        cur->records[i] = tmp[i];
                        cur->num_of_keys++;
                    }
                    else
                    {
                        // cur->records[i].key = 0;
                        // strcpy(cur->records[i].value, "");
                        new_p->records[i - (LEAF_MAX / 2) - 1] = tmp[i];
                        new_p->num_of_keys++;
                    }
                }
                prev->b_f[cur_pos - 1].key = cur->records[0].key;

                pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                free(prev);
                pwrite(fd, cur, sizeof(page), cur_off);
                pwrite(fd, new_p, sizeof(page), new_off);
                split_insert(cur, new_p, new_p->records[0].key, new_off);
                free(cur);
                free(new_p);
            }
        }
        return 0;
    }
    else // 그냥 삽입 과정
    {

        for (int i = 0; i < cur->num_of_keys; i++) // numofkeys 가 31 보다 작을때 수행 추가
        {
            if (cur->records[i].key == key)
            {
                // printf("no input\n");
                return -1;
            }
            if (cur->records[i].key > key)
            {
                pos = i;
                break;
            }
        }
        for (int i = cur->num_of_keys; i > pos; i--)
            cur->records[i] = cur->records[i - 1];

        cur->records[pos].key = key;
        strcpy(cur->records[pos].value, value);
        cur->num_of_keys++; // 단순 삽입

        // printf("cur offset: %p, num_of_keys: %d, key: %d, value:%s\n", cur_off, cur->num_of_keys, cur->records[pos].key, cur->records[pos].value);

        pwrite(fd, cur, sizeof(page), cur_off); // 디스크에 삽입 저장
        free(cur);
        return 0;
    }

    return -1;
}

void delete_balance(page *cur)
{
    // 재분배 가능 여부 1. 왼쪽 2. 오른쪽 확인
    // 재분배 불가능의 경우 -> merge 시도, cur 가 가장 왼쪽의 경우 오른쪽과 머지,
    // 아닌경우 모두 왼쪽과 머지
    // printf("p: %d, cur: %d\n", cur->parent_page_offset, cur->num_of_keys);
    if (cur->parent_page_offset == 0 && cur->num_of_keys > 0)
        return;

    if (cur->parent_page_offset == 0 && cur->num_of_keys == 0)
    {
        page *new_rt = load_page(cur->next_offset);
        free(rt);
        // 현재 페이지가 root 의 경우, 남아있는
        // pos 에 따라서 왼쪽, 오른쪽을 선택

        new_rt->parent_page_offset = 0;
        pwrite(fd, new_rt, sizeof(page), hp->rpo);
        free(new_rt);
        rt = load_page(hp->rpo);

        return;
    }
    else
    {
        int flag = 0;
        page *prev = load_page(cur->parent_page_offset);

        int cur_pos = prev->num_of_keys;

        for (int i = 0; i < prev->num_of_keys; i++)
        {
            // printf("c[0]: %d, p[i]: %d, %d\n", cur->records[0].key, prev->b_f[i].key, i);
            if (cur->is_leaf)
            {
                if (cur->records[0].key < prev->b_f[i].key)
                {
                    cur_pos = i;
                    break;
                }
            }
            else
            {
                if (cur->b_f[0].key < prev->b_f[i].key)
                {
                    cur_pos = i;
                    break;
                }
            }
        }

        // printf("cur_pos : %d\n", cur_pos);
        if (cur_pos != 0) // 현재 가장 왼쪽이 아닌 경우, 왼쪽을 확인해야함.
        {
            flag = 1;
            page *left;
            if (cur_pos == 1)
                left = load_page(prev->next_offset);
            else
                left = load_page(prev->b_f[cur_pos - 2].p_offset);

            if (left->is_leaf && left->num_of_keys <= LEAF_MAX / 2) // left 에서 받아올 수 없음, 오른쪽 확인 해야한다.
                flag = 0;
            else if (!left->is_leaf && left->num_of_keys <= INTERNAL_MAX / 2)
                flag = 0;
            else
            { // left 에서 key 하나 받아오기 진행   left 는 항상 존재.
                // printf("left distribute\n");
                if (left->is_leaf) // leaf node 의 경우
                {
                    for (int i = cur->num_of_keys; i > 0; i--)
                        cur->records[i] = cur->records[i - 1];

                    cur->records[0] = left->records[left->num_of_keys - 1];
                    left->num_of_keys--;
                    cur->num_of_keys++;
                    prev->b_f[cur_pos - 1].key = cur->records[0].key;
                    pwrite(fd, left, sizeof(page), prev->b_f[cur_pos - 2].p_offset);
                    pwrite(fd, cur, sizeof(page), prev->b_f[cur_pos - 1].p_offset);
                    pwrite(fd, prev, sizeof(page), cur->parent_page_offset); // good test
                    free(left);
                    free(prev);
                }
                else
                {
                    // printf("test\n");
                    for (int i = cur->num_of_keys; i > 0; i--)
                        cur->b_f[i] = cur->b_f[i - 1];
                    cur->b_f[0].p_offset = cur->next_offset;
                    cur->b_f[0].key = prev->b_f[cur_pos - 1].key;
                    prev->b_f[cur_pos - 1].key = left->b_f[left->num_of_keys - 1].key;
                    cur->next_offset = left->b_f[left->num_of_keys - 1].p_offset;
                    prev->b_f[cur_pos - 1].key = left->b_f[left->num_of_keys - 1].key;
                    cur->num_of_keys++;
                    left->num_of_keys--;
                    pwrite(fd, left, sizeof(page), prev->b_f[cur_pos - 2].p_offset);
                    pwrite(fd, cur, sizeof(page), prev->b_f[cur_pos - 1].p_offset);
                    pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                    free(left);
                    free(prev);
                }
            }
        }
        if (flag == 0) // 오른쪽에서 key 하나 받아오기 진행
        {
            flag = 1;
            if (cur_pos >= prev->num_of_keys) // 가장 오른쪽에 있는 경우 오른쪽이 없음. 머지 해야하는 상황
                flag = 2;
            else
            {
                page *right = load_page(prev->b_f[cur_pos].p_offset);

                if (right->is_leaf && right->num_of_keys <= LEAF_MAX / 2) // 오른쪽 노드가 부족한 상황, 가져올 수 없다. 머지해야함.
                    flag = 2;
                else if (!right->is_leaf && right->num_of_keys <= INTERNAL_MAX / 2)
                    flag = 2;
                else
                { //     오른쪽에서 가져오기 진행
                    // printf("right distribute\n");
                    if (right->is_leaf) // leaf node 의 경우
                    {
                        cur->records[cur->num_of_keys] = right->records[0];
                        for (int i = 0; i < right->num_of_keys; i++)
                            right->records[i] = right->records[i + 1];

                        right->num_of_keys--;
                        cur->num_of_keys++;
                        prev->b_f[cur_pos].key = right->records[0].key;
                        pwrite(fd, right, sizeof(page), prev->b_f[cur_pos].p_offset);
                        pwrite(fd, cur, sizeof(page), prev->b_f[cur_pos - 1].p_offset);
                        pwrite(fd, prev, sizeof(page), cur->parent_page_offset); // good test
                        free(right);
                        free(prev);
                    }
                    else // 잘 됨
                    {
                        cur->b_f[cur->num_of_keys].p_offset = right->next_offset;
                        cur->b_f[cur->num_of_keys].key = prev->b_f[cur_pos].key;
                        prev->b_f[cur_pos].key = right->b_f[0].key;

                        right->next_offset = right->b_f[0].p_offset;
                        for (int i = 0; i < right->num_of_keys; i++)
                            right->b_f[i] = right->b_f[i + 1];

                        cur->num_of_keys++;
                        right->num_of_keys--;
                        pwrite(fd, right, sizeof(page), prev->b_f[cur_pos].p_offset);
                        pwrite(fd, cur, sizeof(page), prev->b_f[cur_pos - 1].p_offset);
                        pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                        free(right);
                        free(prev);
                    }
                }
            }
        }
        if (flag == 2) // merge 해야하는 상황.
        {
            if (cur_pos == 0) // 현재 노드가 가장 왼쪽이라면 오른쪽과 merge 해야함.
            {
                page *right = load_page(prev->b_f[cur_pos].p_offset);
                // printf("right merge\n");
                if (!cur->is_leaf) //   INTERNAL node 에 대해서 머지
                {
                    cur->b_f[cur->num_of_keys].key = prev->b_f[cur_pos].key;
                    cur->b_f[cur->num_of_keys].p_offset = right->next_offset;

                    for (int i = cur->num_of_keys + 1; i < cur->num_of_keys + right->num_of_keys + 1; i++)
                    {
                        cur->b_f[i] = right->b_f[i - cur->num_of_keys - 1];
                    }
                    cur->num_of_keys += right->num_of_keys + 1;

                    usetofree(prev->b_f[0].p_offset);
                    prev->num_of_keys--;
                    for (int i = 0; i < prev->num_of_keys; i++)
                        prev->b_f[i] = prev->b_f[i + 1];

                    pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                    pwrite(fd, cur, sizeof(page), prev->next_offset);

                    free(right);

                    if (prev->num_of_keys < INTERNAL_MAX / 2)
                    {
                        delete_balance(prev);
                    }
                }
                else // leaf 노드에 대해 머지
                {
                    for (int i = cur->num_of_keys; i < cur->num_of_keys + right->num_of_keys; i++)
                        cur->records[i] = right->records[i - cur->num_of_keys];

                    cur->num_of_keys += right->num_of_keys;
                    cur->next_offset = right->next_offset;

                    usetofree(prev->b_f[0].p_offset);
                    prev->num_of_keys--;
                    for (int i = 0; i < prev->num_of_keys; i++)
                        prev->b_f[i] = prev->b_f[i + 1];

                    pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                    pwrite(fd, cur, sizeof(page), prev->next_offset);
                    free(right);

                    if (prev->num_of_keys < INTERNAL_MAX / 2)
                    {
                        delete_balance(prev);
                    }
                }
                free(prev);
            }
            else
            { // 그게 아닌 경우 모두 왼쪽과 merge
                // printf("left merge\n");
                page *left;
                off_t left_off;
                if (cur_pos == 1)
                {
                    left_off = prev->next_offset;
                    left = load_page(left_off);
                }
                else
                {
                    left_off = prev->b_f[cur_pos - 2].p_offset;
                    left = load_page(left_off);
                }
                if (!cur->is_leaf)
                {
                    left->b_f[left->num_of_keys].key = prev->b_f[cur_pos - 1].key;
                    left->b_f[left->num_of_keys].p_offset = cur->next_offset;
                    left->num_of_keys++;
                    prev->num_of_keys--;

                    for (int i = 0; i < cur->num_of_keys; i++)
                    {
                        left->b_f[left->num_of_keys] = cur->b_f[i];
                        left->num_of_keys++;
                    }
                    usetofree(prev->b_f[cur_pos - 1].p_offset);
                    for (int i = cur_pos - 1; i < prev->num_of_keys; i++)
                    {
                        prev->b_f[i] = prev->b_f[i + 1]; //
                    }
                    pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                    pwrite(fd, left, sizeof(page), left_off);
                    free(left);
                    if (prev->num_of_keys < INTERNAL_MAX / 2)
                    {
                        delete_balance(prev);
                    }
                }
                else
                { // leaf node 의 경우

                    for (int i = 0; i < cur->num_of_keys; i++)
                    {
                        left->records[left->num_of_keys] = cur->records[i];
                        left->num_of_keys++;
                    }
                    left->next_offset = cur->next_offset;
                    prev->num_of_keys--;
                    usetofree(prev->b_f[cur_pos - 1].p_offset);
                    for (int i = cur_pos - 1; i < prev->num_of_keys; i++)
                    {
                        prev->b_f[i] = prev->b_f[i + 1]; //
                    }
                    pwrite(fd, prev, sizeof(page), cur->parent_page_offset);
                    pwrite(fd, left, sizeof(page), left_off);
                    free(left);
                    if (prev->num_of_keys < INTERNAL_MAX / 2)
                    {
                        delete_balance(prev);
                    }
                }
                free(prev);
            }
        }
    }
}

void update_key(int64_t key, int64_t upadate_key)
// delete 작업에서 inner key 에 해당하는 key 를 update 한다.
{
    page *cur = load_page(hp->rpo); // rt 로 설정 처음에
    off_t cur_off = hp->rpo;        // 현재 offset
    off_t prev_off = 0;
    int cur_pos;

    while (!cur->is_leaf)
    {
        cur_pos = 0;
        if (cur->parent_page_offset != prev_off)
        {
            cur->parent_page_offset = prev_off;
            pwrite(fd, cur, sizeof(page), cur_off);
        }

        if (key == cur->b_f[0].key)
        {
            cur->b_f[0].key = upadate_key;
            pwrite(fd, cur, sizeof(page), cur_off);
            free(cur);
            return;
        }
        if (key < cur->b_f[0].key)
        {
            prev_off = cur_off;
            cur_off = cur->next_offset;
            cur = load_page(cur->next_offset);
        }
        else
        {
            int flag = 0;
            for (int i = 1; i < cur->num_of_keys; i++)
            {
                if (key == cur->b_f[i].key)
                {
                    cur->b_f[i].key = upadate_key;
                    pwrite(fd, cur, sizeof(page), cur_off);
                    free(cur);
                    return;
                }
                else if (key < cur->b_f[i].key) // 현재 키보다 작으면 i-1 포인터로 이동
                {
                    prev_off = cur_off;
                    cur_off = cur->b_f[i - 1].p_offset;
                    cur = load_page(cur->b_f[i - 1].p_offset);
                    flag = 1;
                    cur_pos = i;
                    break;
                }
            }
            if (!flag) // 맨 마지막 포인터
            {
                prev_off = cur_off;
                cur_off = cur->b_f[cur->num_of_keys - 1].p_offset;
                cur_pos = cur->num_of_keys;
                cur = load_page(cur->b_f[cur->num_of_keys - 1].p_offset);
            }
        }
    } // 여기까지 위치 찾기
}

int db_delete(int64_t key)
{
    if (rt == NULL)
        return -1;

    page *cur = load_page(hp->rpo); // rt 로 설정 처음에
    off_t cur_off = hp->rpo;        // 현재 offset
    off_t prev_off = 0;
    int cur_pos;

    while (!cur->is_leaf)
    {
        cur_pos = 0;
        if (cur->parent_page_offset != prev_off)
        {
            cur->parent_page_offset = prev_off;
            pwrite(fd, cur, sizeof(page), cur_off);
        }

        if (key < cur->b_f[0].key)
        {
            prev_off = cur_off;
            cur_off = cur->next_offset;
            cur = load_page(cur->next_offset);
        }
        else
        {
            int flag = 0;
            for (int i = 1; i < cur->num_of_keys; i++)
            {
                if (key < cur->b_f[i].key) // 현재 키보다 작으면 i-1 포인터로 이동
                {
                    prev_off = cur_off;
                    cur_off = cur->b_f[i - 1].p_offset;
                    cur = load_page(cur->b_f[i - 1].p_offset);
                    flag = 1;
                    cur_pos = i;
                    break;
                }
            }
            if (!flag) // 맨 마지막 포인터
            {
                prev_off = cur_off;
                cur_off = cur->b_f[cur->num_of_keys - 1].p_offset;
                cur_pos = cur->num_of_keys;
                cur = load_page(cur->b_f[cur->num_of_keys - 1].p_offset);
            }
        }
    } // 여기까지 위치 찾기
    cur->parent_page_offset = prev_off;

    int pos = cur->num_of_keys;

    // printf("p: %d c: %d\n", cur->parent_page_offset, cur_off);

    for (int i = 0; i < cur->num_of_keys; i++)
    {
        if (cur->records[i].key == key)
        {
            pos = i;
            break;
        }
    }
    if (pos == cur->num_of_keys) // not exist
    {
        free(cur);
        // printf("Not exist\n");
        return -1;
    }
    cur->num_of_keys--;
    for (int i = pos; i < cur->num_of_keys; i++)
    {
        cur->records[i] = cur->records[i + 1];
    }

    if (pos == 0) // 0번 키를 삭제한 경우 key 업데이트
    {
        update_key(key, cur->records[0].key);
    }
    pwrite(fd, cur, sizeof(page), cur_off);

    if (cur->num_of_keys < LEAF_MAX / 2)
    {
        delete_balance(cur);
    }
    free(cur);
    return 0;
} // fin
