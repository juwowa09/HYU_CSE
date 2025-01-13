#include "bpt.h"

H_P *hp1;
H_P *hp2;

page *rt1 = NULL; // root is declared as global
page *rt2 = NULL; // root is declared as global

int fd1 = -1; // fd is declared as global
int fd2 = -1; // fd is declared as global

H_P *load_header(off_t off, int table_num)
{
    H_P *newhp = (H_P *)calloc(1, sizeof(H_P));
    if (table_num == 1)
    {
        if (sizeof(H_P) > pread(fd1, newhp, sizeof(H_P), 0))
        {

            return NULL;
        }
    }
    else
    {
        if (sizeof(H_P) > pread(fd2, newhp, sizeof(H_P), 0))
        {

            return NULL;
        }
    }
    return newhp;
}

page *load_page(off_t off, int table_num)
{
    page *load = (page *)calloc(1, sizeof(page));
    // if (off % sizeof(page) != 0) printf("load fail : page offset error\n");
    if (table_num == 1)
    {
        if (sizeof(page) > pread(fd1, load, sizeof(page), off))
        {

            return NULL;
        }
    }
    else
    {
        if (sizeof(page) > pread(fd2, load, sizeof(page), off))
        {

            return NULL;
        }
    }
    return load;
}

int open_table(char *pathname1, char *pathname2)
{
    fd1 = open(pathname1, O_RDWR | O_CREAT | O_EXCL | O_SYNC, 0775);
    hp1 = (H_P *)calloc(1, sizeof(H_P));
    if (fd1 > 0)
    {
        // printf("New File created\n");
        hp1->fpo = 0;
        hp1->num_of_pages = 1;
        hp1->rpo = 0;
        pwrite(fd1, hp1, sizeof(H_P), 0);
        free(hp1);
        hp1 = load_header(0, 1);
    }
    else
    {
        fd1 = open(pathname1, O_RDWR | O_SYNC);
        if (fd1 > 0)
        {
            // printf("Read Existed File\n");
            if (sizeof(H_P) > pread(fd1, hp1, sizeof(H_P), 0))
            {
                return -1;
            }
            off_t r_o = hp1->rpo;
            rt1 = load_page(r_o, 1);
        }
        else
            return -1;
    }
    fd2 = open(pathname2, O_RDWR | O_CREAT | O_EXCL | O_SYNC, 0775);
    hp2 = (H_P *)calloc(1, sizeof(H_P));
    if (fd2 > 0)
    {
        // printf("New File created\n");
        hp2->fpo = 0;
        hp2->num_of_pages = 1;
        hp2->rpo = 0;
        pwrite(fd2, hp2, sizeof(H_P), 0);
        free(hp2);
        hp2 = load_header(0, 2);
    }
    else
    {
        fd2 = open(pathname2, O_RDWR | O_SYNC);
        if (fd2 > 0)
        {
            // printf("Read Existed File\n");
            if (sizeof(H_P) > pread(fd2, hp2, sizeof(H_P), 0))
            {
                return -1;
            }
            off_t r_o = hp2->rpo;
            rt2 = load_page(r_o, 2);
            return 0;
        }
        else
            return -1;
    }
}

void check()
{
    // printf("hp:num_of_page: %d\nhp->rpo: %d\nrt->num_of_keys: %d\nrt->is_leaf: %d\n", hp1->num_of_pages, hp1->rpo, rt1->num_of_keys, rt1->is_leaf);
    // printf("rt1 prev: %d, cur: %d, num_of: %d\n", rt1->parent_page_offset, rt1->is_leaf, rt1->num_of_keys);
    page *p;
    off_t loc = hp1->rpo;

    // printf("left = %ld, key = %ld, right = %ld, is_leaf = %d, now_root = %ld\n", rt->next_offset,
    //   rt->b_f[0].key, rt->b_f[0].p_offset, rt->is_leaf, hp->rpo);

    if (rt1 == NULL)
    {
        // printf("Empty tree.\n");
        return;
    }
    p = load_page(loc, 1);
    // printf("rt->is_leaf: %d\nrt->num_of: %d\n", rt1->is_leaf, rt1->num_of_keys);
    while (!p->is_leaf)
    {
        loc = p->next_offset;
        free(p);
        p = load_page(loc, 1);
    }
    while (p->next_offset != 0)
    {
        printf("key : ");
        for (int i = 0; i < p->num_of_keys; i++)
        {
            printf("%d ", p->records[i].key);
        }
        loc = p->next_offset;
        printf("\n");
        free(p);
        p = load_page(loc, 1);
    }
    free(p);

    loc = hp2->rpo;

    // printf("left = %ld, key = %ld, right = %ld, is_leaf = %d, now_root = %ld\n", rt->next_offset,
    //   rt->b_f[0].key, rt->b_f[0].p_offset, rt->is_leaf, hp->rpo);

    if (rt1 == NULL)
    {
        // printf("Empty tree.\n");
        return;
    }
    p = load_page(loc, 2);

    while (!p->is_leaf)
    {
        loc = p->next_offset;
        free(p);
        p = load_page(loc, 2);
    }
    while (p->next_offset != 0)
    {
        printf("key : ");
        for (int i = 0; i < p->num_of_keys; i++)
        {
            printf("%d ", p->records[i].key);
        }
        printf("\n");
        loc = p->next_offset;
        free(p);
        p = load_page(loc, 2);
    }
    free(p);
    // printf("rt2 prev: %d, cur: %d, num_of: %d\n", rt2->parent_page_offset, rt2->is_leaf, rt2->num_of_keys);
}

// void reset(off_t off)
// {
//     page *reset;
//     reset = (page *)calloc(1, sizeof(page));
//     reset->parent_page_offset = 0;
//     reset->is_leaf = 0;
//     reset->num_of_keys = 0;
//     reset->next_offset = 0;
//     pwrite(fd, reset, sizeof(page), off);
//     free(reset);
//     return;
// }

// void freetouse(off_t fpo)
// {
//     page *reset;
//     reset = load_page(fpo);
//     reset->parent_page_offset = 0;
//     reset->is_leaf = 0;
//     reset->num_of_keys = 0;
//     reset->next_offset = 0;
//     pwrite(fd, reset, sizeof(page), fpo);
//     free(reset);
//     return;
// }

// void usetofree(off_t wbf)
// {
//     page *utf = load_page(wbf);
//     utf->parent_page_offset = hp->fpo;
//     utf->is_leaf = 0;
//     utf->num_of_keys = 0;
//     utf->next_offset = 0;
//     pwrite(fd, utf, sizeof(page), wbf);
//     free(utf);
//     hp->fpo = wbf;
//     pwrite(fd, hp, sizeof(hp), 0);
//     free(hp);
//     hp = load_header(0);
//     return;
// }

// off_t new_page()
// {
//     off_t newp;
//     page *np;
//     off_t prev;
//     if (hp->fpo != 0)
//     {
//         newp = hp->fpo;
//         np = load_page(newp);
//         hp->fpo = np->parent_page_offset;
//         pwrite(fd, hp, sizeof(hp), 0);
//         free(hp);
//         hp = load_header(0);
//         free(np);
//         freetouse(newp);
//         return newp;
//     }
//     // change previous offset to 0 is needed
//     newp = lseek(fd, 0, SEEK_END);
//     // if (newp % sizeof(page) != 0) printf("new page made error : file size error\n");
//     reset(newp);
//     hp->num_of_pages++;
//     pwrite(fd, hp, sizeof(H_P), 0);
//     free(hp);
//     hp = load_header(0);
//     return newp;
// }

// off_t find_leaf(int64_t key)
// {
//     int i = 0;
//     page *p;
//     off_t loc = hp->rpo;

//     // printf("left = %ld, key = %ld, right = %ld, is_leaf = %d, now_root = %ld\n", rt->next_offset,
//     //   rt->b_f[0].key, rt->b_f[0].p_offset, rt->is_leaf, hp->rpo);

//     if (rt == NULL)
//     {
//         // printf("Empty tree.\n");
//         return 0;
//     }
//     p = load_page(loc);

//     while (!p->is_leaf)
//     {
//         i = 0;

//         while (i < p->num_of_keys)
//         {
//             if (key >= p->b_f[i].key)
//                 i++;
//             else
//                 break;
//         }
//         if (i == 0)
//             loc = p->next_offset;
//         else
//             loc = p->b_f[i - 1].p_offset;
//         // if (loc == 0)
//         //  return NULL;

//         free(p);
//         p = load_page(loc);
//     }

//     free(p);
//     return loc;
// }

// char *db_find(int64_t key)
// {
//     char *value = (char *)malloc(sizeof(char) * 120);
//     int i = 0;
//     off_t fin = find_leaf(key);
//     if (fin == 0)
//     {
//         return NULL;
//     }
//     page *p = load_page(fin);

//     for (; i < p->num_of_keys; i++)
//     {
//         if (p->records[i].key == key)
//             break;
//     }
//     if (i == p->num_of_keys)
//     {
//         free(p);
//         return NULL;
//     }
//     else
//     {
//         strcpy(value, p->records[i].value);
//         free(p);
//         return value;
//     }
// }

// int cut(int length)
// {
//     if (length % 2 == 0)
//         return length / 2;
//     else
//         return length / 2 + 1;
// }

// void start_new_file(record rec)
// {

//     page *root;
//     off_t ro;
//     ro = new_page();
//     rt = load_page(ro);
//     hp->rpo = ro;
//     pwrite(fd, hp, sizeof(H_P), 0);
//     free(hp);
//     hp = load_header(0);
//     rt->num_of_keys = 1;
//     rt->is_leaf = 1;
//     rt->records[0] = rec;
//     pwrite(fd, rt, sizeof(page), hp->rpo);
//     free(rt);
//     rt = load_page(hp->rpo);
//     // printf("new file is made\n");
// }

// int db_insert(int64_t key, char *value)
// {

//     record nr;
//     nr.key = key;
//     strcpy(nr.value, value);
//     if (rt == NULL)
//     {
//         start_new_file(nr);
//         return 0;
//     }

//     char *dupcheck;
//     dupcheck = db_find(key);
//     if (dupcheck != NULL)
//     {
//         free(dupcheck);
//         return -1;
//     }
//     free(dupcheck);

//     off_t leaf = find_leaf(key);

//     page *leafp = load_page(leaf);

//     if (leafp->num_of_keys < LEAF_MAX)
//     {
//         insert_into_leaf(leaf, nr);
//         free(leafp);
//         return 0;
//     }

//     insert_into_leaf_as(leaf, nr);
//     free(leafp);
//     // why double free?
//     return 0;
// }

// off_t insert_into_leaf(off_t leaf, record inst)
// {

//     page *p = load_page(leaf);
//     // if (p->is_leaf == 0) printf("iil error : it is not leaf page\n");
//     int i, insertion_point;
//     insertion_point = 0;
//     while (insertion_point < p->num_of_keys && p->records[insertion_point].key < inst.key)
//     {
//         insertion_point++;
//     }
//     for (i = p->num_of_keys; i > insertion_point; i--)
//     {
//         p->records[i] = p->records[i - 1];
//     }
//     p->records[insertion_point] = inst;
//     p->num_of_keys++;
//     pwrite(fd, p, sizeof(page), leaf);
//     // printf("insertion %ld is complete %d, %ld\n", inst.key, p->num_of_keys, leaf);
//     free(p);
//     return leaf;
// }

// off_t insert_into_leaf_as(off_t leaf, record inst)
// {
//     off_t new_leaf;
//     record *temp;
//     int insertion_index, split, i, j;
//     int64_t new_key;
//     new_leaf = new_page();
//     // printf("\n%ld is new_leaf offset\n\n", new_leaf);
//     page *nl = load_page(new_leaf);
//     nl->is_leaf = 1;
//     temp = (record *)calloc(LEAF_MAX + 1, sizeof(record));
//     if (temp == NULL)
//     {
//         perror("Temporary records array");
//         exit(EXIT_FAILURE);
//     }
//     insertion_index = 0;
//     page *ol = load_page(leaf);
//     while (insertion_index < LEAF_MAX && ol->records[insertion_index].key < inst.key)
//     {
//         insertion_index++;
//     }
//     for (i = 0, j = 0; i < ol->num_of_keys; i++, j++)
//     {
//         if (j == insertion_index)
//             j++;
//         temp[j] = ol->records[i];
//     }
//     temp[insertion_index] = inst;
//     ol->num_of_keys = 0;
//     split = cut(LEAF_MAX);

//     for (i = 0; i < split; i++)
//     {
//         ol->records[i] = temp[i];
//         ol->num_of_keys++;
//     }

//     for (i = split, j = 0; i < LEAF_MAX + 1; i++, j++)
//     {
//         nl->records[j] = temp[i];
//         nl->num_of_keys++;
//     }

//     free(temp);

//     nl->next_offset = ol->next_offset;
//     ol->next_offset = new_leaf;

//     for (i = ol->num_of_keys; i < LEAF_MAX; i++)
//     {
//         ol->records[i].key = 0;
//         // strcpy(ol->records[i].value, NULL);
//     }

//     for (i = nl->num_of_keys; i < LEAF_MAX; i++)
//     {
//         nl->records[i].key = 0;
//         // strcpy(nl->records[i].value, NULL);
//     }

//     nl->parent_page_offset = ol->parent_page_offset;
//     new_key = nl->records[0].key;

//     pwrite(fd, nl, sizeof(page), new_leaf);
//     pwrite(fd, ol, sizeof(page), leaf);
//     free(ol);
//     free(nl);
//     // printf("split_leaf is complete\n");

//     return insert_into_parent(leaf, new_key, new_leaf);
// }

// off_t insert_into_parent(off_t old, int64_t key, off_t newp)
// {

//     int left_index;
//     off_t bumo;
//     page *left;
//     left = load_page(old);

//     bumo = left->parent_page_offset;
//     free(left);

//     if (bumo == 0)
//         return insert_into_new_root(old, key, newp);

//     left_index = get_left_index(old);

//     page *parent = load_page(bumo);
//     // printf("\nbumo is %ld\n", bumo);
//     if (parent->num_of_keys < INTERNAL_MAX)
//     {
//         free(parent);
//         // printf("\nuntil here is ok\n");
//         return insert_into_internal(bumo, left_index, key, newp);
//     }
//     free(parent);
//     return insert_into_internal_as(bumo, left_index, key, newp);
// }

// int get_left_index(off_t left)
// {
//     page *child = load_page(left);
//     off_t po = child->parent_page_offset;
//     free(child);
//     page *parent = load_page(po);
//     int i = 0;
//     if (left == parent->next_offset)
//         return -1;
//     for (; i < parent->num_of_keys; i++)
//     {
//         if (parent->b_f[i].p_offset == left)
//             break;
//     }

//     if (i == parent->num_of_keys)
//     {
//         free(parent);
//         return -10;
//     }
//     free(parent);
//     return i;
// }

// off_t insert_into_new_root(off_t old, int64_t key, off_t newp)
// {

//     off_t new_root;
//     new_root = new_page();
//     page *nr = load_page(new_root);
//     nr->b_f[0].key = key;
//     nr->next_offset = old;
//     nr->b_f[0].p_offset = newp;
//     nr->num_of_keys++;
//     // printf("key = %ld, old = %ld, new = %ld, nok = %d, nr = %ld\n", key, old, newp,
//     //   nr->num_of_keys, new_root);
//     page *left = load_page(old);
//     page *right = load_page(newp);
//     left->parent_page_offset = new_root;
//     right->parent_page_offset = new_root;
//     pwrite(fd, nr, sizeof(page), new_root);
//     pwrite(fd, left, sizeof(page), old);
//     pwrite(fd, right, sizeof(page), newp);
//     free(nr);
//     nr = load_page(new_root);
//     rt = nr;
//     hp->rpo = new_root;
//     pwrite(fd, hp, sizeof(H_P), 0);
//     free(hp);
//     hp = load_header(0);
//     free(left);
//     free(right);
//     return new_root;
// }

// off_t insert_into_internal(off_t bumo, int left_index, int64_t key, off_t newp)
// {

//     page *parent = load_page(bumo);
//     int i;

//     for (i = parent->num_of_keys; i > left_index + 1; i--)
//     {
//         parent->b_f[i] = parent->b_f[i - 1];
//     }
//     parent->b_f[left_index + 1].key = key;
//     parent->b_f[left_index + 1].p_offset = newp;
//     parent->num_of_keys++;
//     pwrite(fd, parent, sizeof(page), bumo);
//     free(parent);
//     if (bumo == hp->rpo)
//     {
//         free(rt);
//         rt = load_page(bumo);
//         // printf("\nrt->numofkeys%lld\n", rt->num_of_keys);
//     }
//     return hp->rpo;
// }

// off_t insert_into_internal_as(off_t bumo, int left_index, int64_t key, off_t newp)
// {

//     int i, j, split;
//     int64_t k_prime;
//     off_t new_p, child;
//     I_R *temp;

//     temp = (I_R *)calloc(INTERNAL_MAX + 1, sizeof(I_R));

//     page *old_parent = load_page(bumo);

//     for (i = 0, j = 0; i < old_parent->num_of_keys; i++, j++)
//     {
//         if (j == left_index + 1)
//             j++;
//         temp[j] = old_parent->b_f[i];
//     }

//     temp[left_index + 1].key = key;
//     temp[left_index + 1].p_offset = newp;

//     split = cut(INTERNAL_MAX);
//     new_p = new_page();
//     page *new_parent = load_page(new_p);
//     old_parent->num_of_keys = 0;
//     for (i = 0; i < split; i++)
//     {
//         old_parent->b_f[i] = temp[i];
//         old_parent->num_of_keys++;
//     }
//     k_prime = temp[i].key;
//     new_parent->next_offset = temp[i].p_offset;
//     for (++i, j = 0; i < INTERNAL_MAX + 1; i++, j++)
//     {
//         new_parent->b_f[j] = temp[i];
//         new_parent->num_of_keys++;
//     }

//     new_parent->parent_page_offset = old_parent->parent_page_offset;
//     page *nn;
//     nn = load_page(new_parent->next_offset);
//     nn->parent_page_offset = new_p;
//     pwrite(fd, nn, sizeof(page), new_parent->next_offset);
//     free(nn);
//     for (i = 0; i < new_parent->num_of_keys; i++)
//     {
//         child = new_parent->b_f[i].p_offset;
//         page *ch = load_page(child);
//         ch->parent_page_offset = new_p;
//         pwrite(fd, ch, sizeof(page), child);
//         free(ch);
//     }

//     pwrite(fd, old_parent, sizeof(page), bumo);
//     pwrite(fd, new_parent, sizeof(page), new_p);
//     free(old_parent);
//     free(new_parent);
//     free(temp);
//     // printf("split internal is complete\n");
//     return insert_into_parent(bumo, k_prime, new_p);
// }

// int db_delete(int64_t key)
// {

//     if (rt->num_of_keys == 0)
//     {
//         // printf("root is empty\n");
//         return -1;
//     }
//     char *check = db_find(key);
//     if (check == NULL)
//     {
//         free(check);
//         // printf("There are no key to delete\n");
//         return -1;
//     }
//     free(check);
//     off_t deloff = find_leaf(key);
//     delete_entry(key, deloff);
//     return 0;

// } // fin

// void delete_entry(int64_t key, off_t deloff)
// {

//     remove_entry_from_page(key, deloff);

//     if (deloff == hp->rpo)
//     {
//         adjust_root(deloff);
//         return;
//     }
//     page *not_enough = load_page(deloff);
//     int check = not_enough->is_leaf ? cut(LEAF_MAX) : cut(INTERNAL_MAX);
//     if (not_enough->num_of_keys >= check)
//     {
//         free(not_enough);
//         // printf("just delete\n");
//         return;
//     }

//     int neighbor_index, k_prime_index;
//     off_t neighbor_offset, parent_offset;
//     int64_t k_prime;
//     parent_offset = not_enough->parent_page_offset;
//     page *parent = load_page(parent_offset);

//     if (parent->next_offset == deloff)
//     {
//         neighbor_index = -2;
//         neighbor_offset = parent->b_f[0].p_offset;
//         k_prime = parent->b_f[0].key;
//         k_prime_index = 0;
//     }
//     else if (parent->b_f[0].p_offset == deloff)
//     {
//         neighbor_index = -1;
//         neighbor_offset = parent->next_offset;
//         k_prime_index = 0;
//         k_prime = parent->b_f[0].key;
//     }
//     else
//     {
//         int i;

//         for (i = 0; i <= parent->num_of_keys; i++)
//             if (parent->b_f[i].p_offset == deloff)
//                 break;
//         neighbor_index = i - 1;
//         neighbor_offset = parent->b_f[i - 1].p_offset;
//         k_prime_index = i;
//         k_prime = parent->b_f[i].key;
//     }

//     page *neighbor = load_page(neighbor_offset);
//     int max = not_enough->is_leaf ? LEAF_MAX : INTERNAL_MAX - 1;
//     int why = neighbor->num_of_keys + not_enough->num_of_keys;
//     // printf("%d %d\n",why, max);
//     if (why <= max)
//     {
//         free(not_enough);
//         free(parent);
//         free(neighbor);
//         coalesce_pages(deloff, neighbor_index, neighbor_offset, parent_offset, k_prime);
//     }
//     else
//     {
//         free(not_enough);
//         free(parent);
//         free(neighbor);
//         redistribute_pages(deloff, neighbor_index, neighbor_offset, parent_offset, k_prime, k_prime_index);
//     }

//     return;
// }
// void redistribute_pages(off_t need_more, int nbor_index, off_t nbor_off, off_t par_off, int64_t k_prime, int k_prime_index)
// {

//     page *need, *nbor, *parent;
//     int i;
//     need = load_page(need_more);
//     nbor = load_page(nbor_off);
//     parent = load_page(par_off);
//     if (nbor_index != -2)
//     {

//         if (!need->is_leaf)
//         {
//             // printf("redis average interal\n");
//             for (i = need->num_of_keys; i > 0; i--)
//                 need->b_f[i] = need->b_f[i - 1];

//             need->b_f[0].key = k_prime;
//             need->b_f[0].p_offset = need->next_offset;
//             need->next_offset = nbor->b_f[nbor->num_of_keys - 1].p_offset;
//             page *child = load_page(need->next_offset);
//             child->parent_page_offset = need_more;
//             pwrite(fd, child, sizeof(page), need->next_offset);
//             free(child);
//             parent->b_f[k_prime_index].key = nbor->b_f[nbor->num_of_keys - 1].key;
//         }
//         else
//         {
//             // printf("redis average leaf\n");
//             for (i = need->num_of_keys; i > 0; i--)
//             {
//                 need->records[i] = need->records[i - 1];
//             }
//             need->records[0] = nbor->records[nbor->num_of_keys - 1];
//             nbor->records[nbor->num_of_keys - 1].key = 0;
//             parent->b_f[k_prime_index].key = need->records[0].key;
//         }
//     }
//     else
//     {
//         //
//         if (need->is_leaf)
//         {
//             // printf("redis leftmost leaf\n");
//             need->records[need->num_of_keys] = nbor->records[0];
//             for (i = 0; i < nbor->num_of_keys - 1; i++)
//                 nbor->records[i] = nbor->records[i + 1];
//             parent->b_f[k_prime_index].key = nbor->records[0].key;
//         }
//         else
//         {
//             // printf("redis leftmost internal\n");
//             need->b_f[need->num_of_keys].key = k_prime;
//             need->b_f[need->num_of_keys].p_offset = nbor->next_offset;
//             page *child = load_page(need->b_f[need->num_of_keys].p_offset);
//             child->parent_page_offset = need_more;
//             pwrite(fd, child, sizeof(page), need->b_f[need->num_of_keys].p_offset);
//             free(child);

//             parent->b_f[k_prime_index].key = nbor->b_f[0].key;
//             nbor->next_offset = nbor->b_f[0].p_offset;
//             for (i = 0; i < nbor->num_of_keys - 1; i++)
//                 nbor->b_f[i] = nbor->b_f[i + 1];
//         }
//     }
//     nbor->num_of_keys--;
//     need->num_of_keys++;
//     pwrite(fd, parent, sizeof(page), par_off);
//     pwrite(fd, nbor, sizeof(page), nbor_off);
//     pwrite(fd, need, sizeof(page), need_more);
//     free(parent);
//     free(nbor);
//     free(need);
//     return;
// }

// void coalesce_pages(off_t will_be_coal, int nbor_index, off_t nbor_off, off_t par_off, int64_t k_prime)
// {

//     page *wbc, *nbor, *parent;
//     off_t newp, wbf;

//     if (nbor_index == -2)
//     {
//         // printf("leftmost\n");
//         wbc = load_page(nbor_off);
//         nbor = load_page(will_be_coal);
//         parent = load_page(par_off);
//         newp = will_be_coal;
//         wbf = nbor_off;
//     }
//     else
//     {
//         wbc = load_page(will_be_coal);
//         nbor = load_page(nbor_off);
//         parent = load_page(par_off);
//         newp = nbor_off;
//         wbf = will_be_coal;
//     }

//     int point = nbor->num_of_keys;
//     int le = wbc->num_of_keys;
//     int i, j;
//     if (!wbc->is_leaf)
//     {
//         // printf("coal internal\n");
//         nbor->b_f[point].key = k_prime;
//         nbor->b_f[point].p_offset = wbc->next_offset;
//         nbor->num_of_keys++;

//         for (i = point + 1, j = 0; j < le; i++, j++)
//         {
//             nbor->b_f[i] = wbc->b_f[j];
//             nbor->num_of_keys++;
//             wbc->num_of_keys--;
//         }

//         for (i = point; i < nbor->num_of_keys; i++)
//         {
//             page *child = load_page(nbor->b_f[i].p_offset);
//             child->parent_page_offset = newp;
//             pwrite(fd, child, sizeof(page), nbor->b_f[i].p_offset);
//             free(child);
//         }
//     }
//     else
//     {
//         // printf("coal leaf\n");
//         int range = wbc->num_of_keys;
//         for (i = point, j = 0; j < range; i++, j++)
//         {

//             nbor->records[i] = wbc->records[j];
//             nbor->num_of_keys++;
//             wbc->num_of_keys--;
//         }
//         nbor->next_offset = wbc->next_offset;
//     }
//     pwrite(fd, nbor, sizeof(page), newp);

//     delete_entry(k_prime, par_off);
//     free(wbc);
//     usetofree(wbf);
//     free(nbor);
//     free(parent);
//     return;

// } // fin

// void adjust_root(off_t deloff)
// {

//     if (rt->num_of_keys > 0)
//         return;
//     if (!rt->is_leaf)
//     {
//         off_t nr = rt->next_offset;
//         page *nroot = load_page(nr);
//         nroot->parent_page_offset = 0;
//         usetofree(hp->rpo);
//         hp->rpo = nr;
//         pwrite(fd, hp, sizeof(H_P), 0);
//         free(hp);
//         hp = load_header(0);

//         pwrite(fd, nroot, sizeof(page), nr);
//         free(nroot);
//         free(rt);
//         rt = load_page(nr);

//         return;
//     }
//     else
//     {
//         free(rt);
//         rt = NULL;
//         usetofree(hp->rpo);
//         hp->rpo = 0;
//         pwrite(fd, hp, sizeof(hp), 0);
//         free(hp);
//         hp = load_header(0);
//         return;
//     }
// } // fin

// void remove_entry_from_page(int64_t key, off_t deloff)
// {

//     int i = 0;
//     page *lp = load_page(deloff);
//     if (lp->is_leaf)
//     {
//         // printf("remove leaf key %ld\n", key);
//         while (lp->records[i].key != key)
//             i++;

//         for (++i; i < lp->num_of_keys; i++)
//             lp->records[i - 1] = lp->records[i];
//         lp->num_of_keys--;
//         pwrite(fd, lp, sizeof(page), deloff);
//         if (deloff == hp->rpo)
//         {
//             free(lp);
//             free(rt);
//             rt = load_page(deloff);
//             return;
//         }

//         free(lp);
//         return;
//     }
//     else
//     {
//         // printf("remove interanl key %ld\n", key);
//         while (lp->b_f[i].key != key)
//             i++;
//         for (++i; i < lp->num_of_keys; i++)
//             lp->b_f[i - 1] = lp->b_f[i];
//         lp->num_of_keys--;
//         pwrite(fd, lp, sizeof(page), deloff);
//         if (deloff == hp->rpo)
//         {
//             free(lp);
//             free(rt);
//             rt = load_page(deloff);
//             return;
//         }

//         free(lp);
//         return;
//     }
// }

// Assignment 4
void db_join()
{
    // clock_t start, end;
    // start = clock();
    // double cpu_time_used;
    page *p1;
    page *p2;
    off_t loc1 = hp1->rpo;
    off_t loc2 = hp2->rpo;

    // printf("left = %ld, key = %ld, right = %ld, is_leaf = %d, now_root = %ld\n", rt->next_offset,
    //   rt->b_f[0].key, rt->b_f[0].p_offset, rt->is_leaf, hp->rpo);
    if (rt1 == NULL)
    {
        // printf("Empty tree.\n");
        return;
    }

    p1 = load_page(loc1, 1);
    p2 = load_page(loc2, 2);
    // printf("rt->is_leaf: %d\nrt->num_of: %d\n", rt1->is_leaf, rt1->num_of_keys);
    while (!p1->is_leaf)
    {
        loc1 = p1->next_offset;
        free(p1);
        p1 = load_page(loc1, 1);
    }
    while (!p2->is_leaf)
    {
        loc2 = p2->next_offset;
        free(p2);
        p2 = load_page(loc2, 2);
    }
    int i = 0;
    int j = 0;
    // merge join
    while (!(p1->next_offset == 0 && i == p1->num_of_keys) && !(p2->next_offset == 0 && j == p2->num_of_keys))
    {
        if (p1->records[i].key == p2->records[j].key)
        {
            printf("%d,%s,%s\n", p1->records[i].key, p1->records[i].value, p2->records[j].value);
            j++;
        }
        else if (p1->records[i].key < p2->records[j].key)
        {
            i++;
        }
        else if (p1->records[i].key > p2->records[j].key)
            j++;
        if (i == p1->num_of_keys)
        {
            loc1 = p1->next_offset;
            free(p1);
            p1 = load_page(loc1, 1);
            i = 0;
        }
        if (j == p2->num_of_keys)
        {
            loc2 = p2->next_offset;
            free(p2);
            p2 = load_page(loc2, 2);
            j = 0;
        }
    }
    free(p1);
    free(p2);
    // end = clock();
    // cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    // printf("Execution time: %f seconds\n", cpu_time_used);
}

// fin
