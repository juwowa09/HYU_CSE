#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#define LEAF_MAX 31
#define INTERNAL_MAX 248

typedef struct record
{
    int64_t key;
    char value[120];
} record;

typedef struct inter_record
{
    int64_t key;
    off_t p_offset;
} I_R;

typedef struct Page
{
    off_t parent_page_offset;
    int is_leaf;
    int num_of_keys;
    char reserved[104];
    off_t next_offset;
    union
    {
        I_R b_f[248];
        record records[31];
    };
} page;

typedef struct Header_Page
{
    off_t fpo;
    off_t rpo;
    int64_t num_of_pages;
    char reserved[4072];
} H_P;

extern int fd;

extern page *rt;

extern H_P *hp;
// FUNCTION PROTOTYPES.
int open_table(char *pathname1, char *pathname2);
H_P *load_header(off_t off, int table_num);
page *load_page(off_t off, int table_num);
void check();

// void reset(off_t off);
// off_t new_page(int table_num);
// off_t find_leaf(int64_t key);
// // char *db_find(int64_t key);
// void freetouse(off_t fpo);
// int cut(int length);
// int parser();

// void start_new_file(record rec, int table_num);
// // int db_insert(int64_t key, char *value);
// off_t insert_into_leaf(off_t leaf, record inst);
// off_t insert_into_leaf_as(off_t leaf, record inst);
// off_t insert_into_parent(off_t old, int64_t key, off_t newp);
// int get_left_index(off_t left);
// off_t insert_into_new_root(off_t old, int64_t key, off_t newp);
// off_t insert_into_internal(off_t bumo, int left_index, int64_t key, off_t newp);
// off_t insert_into_internal_as(off_t bumo, int left_index, int64_t key, off_t newp);

// int db_delete(int64_t key);
// void delete_entry(int64_t key, off_t deloff);
// void redistribute_pages(off_t need_more, int nbor_index, off_t nbor_off, off_t par_off, int64_t k_prime, int k_prime_index);
// void coalesce_pages(off_t will_be_coal, int nbor_index, off_t nbor_off, off_t par_off, int64_t k_prime);
// void adjust_root(off_t deloff);
// void remove_entry_from_page(int64_t key, off_t deloff);
// void usetofree(off_t wbf);

// Assignment 4
void db_join();

#endif /* __BPT_H__*/
