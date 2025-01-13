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
        I_R b_f[248];       // inner -> panout
        record records[31]; // leaf_node -> real record
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
int open_table(char *pathname);
H_P *load_header(off_t off);
page *load_page(off_t off);

void reset(off_t off);
off_t new_page();
void freetouse(off_t fpo);
int cut(int length);
int parser();
void start_new_file(record rec);

char *db_find(int64_t key);
void split_insert(page *cur, page *new_p, int key, off_t new_p_off);
int db_insert(int64_t key, char *value);
void delete_balance(page *cur);
int db_delete(int64_t key);

#endif /* __BPT_H__*/
