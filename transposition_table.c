#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "c4_utils.h"


#define ZOB_TABLE_SIZE      (BOARD_HEIGHT * BOARD_WIDTH * 2)

typedef struct _node_t
{
    c4_bitboard key;
    score_t value;
    struct _node_t* next;
} node_t;

typedef struct _bucket_t
{
    node_t* head;
    size_t size;
} bucket_t;

typedef struct _hash_table_t
{
    bucket_t* buckets;
    size_t size;
} hash_table_t;

typedef uint32_t hash_t;


hash_t g_zob_table[BOARD_HEIGHT][BOARD_WIDTH][2]; // for Zobrist hashing

uint32_t get_32_bit_random()
{
    uint32_t res = 0;

    res |= rand() & 0xff;
    res |= (rand() & 0xff) << 8;
    res |= (rand() & 0xff) << 16;
    res |= (rand() & 0xff) << 24;

    return res;
}

void initialize_zob_table()
{
    hash_t* table = (hash_t*)&g_zob_table[0];


    srand((uint32_t)time(NULL));

    for(int i = 0; i < ZOB_TABLE_SIZE; i++)
    {
        *(table+i) = get_32_bit_random();
    }

    // for (int i = 0; i < ZOB_TABLE_SIZE; i++)
    // {
    //     printf("0x%08x ", table[i]);
    // }
    // printf("\n");
}

hash_t compute_hash(c4_bitboard* board)
{
    hash_t hash = 0;
    piece p = EMPTY;


    for (int row = 1; row <= BOARD_HEIGHT; row++)
    {
        for (int col = 1; col <= BOARD_WIDTH; col++)
        {
            p = get_piece(board, row, col);
            
            if (p != EMPTY)
            {
                hash ^= g_zob_table[row-1][col-1][p];
            }
        }
    }

    return hash;
}

node_t* create_node(c4_bitboard* key, score_t* value)
{
    node_t* node = malloc(sizeof(node_t));

    if (node == NULL)
    {
        return NULL;
    }

    node->key = *key;
    node->value = *value;
    node->next = NULL;

    return node;
}

hash_table_t* ht_create(size_t size)
{
    hash_table_t* ht = NULL;


    if (size == 0)
    {
        return NULL;
    }

    ht = (hash_table_t*)malloc(sizeof(hash_table_t));

    if (ht == NULL)
    {
        return NULL;
    }

    ht->buckets = malloc(size * sizeof(bucket_t));

    if (ht->buckets == NULL)
    {
        free(ht);
        return NULL;
    }

    memset(ht->buckets, 0, size * sizeof(bucket_t));
    ht->size = size;

    return ht;
}

bool ht_put(hash_table_t* ht, c4_bitboard* key, score_t* value)
{
    hash_t hash = 0;
    int index = 0;


    if (ht == NULL || ht->size == 0)
    {
        return false;
    }

    hash = compute_hash(key);
    index = hash % ht->size;

    if (ht->buckets[index].head == NULL)    // empty bucket
    {
        node_t* new_node = create_node(key, value);

        if (new_node == NULL)
        {
            return false;
        }

        ht->buckets[index].head = new_node;
    }
    else                        // non-empty bucket
    {
        node_t* traverse = ht->buckets[index].head;

        while (traverse != NULL) // check if the same key is already stored
        {
            if (boards_equal(&traverse->key, key))
            {                   // same key is stored, override its value
                traverse->value = *value;
                break;
            }
            traverse = traverse->next;
        }

        if (traverse == NULL)   // new key, store at the beginning
        {
            node_t* temp = ht->buckets[index].head;
            node_t* new_node = create_node(key, value);

            if (new_node == NULL)
            {
                return false;
            }

            ht->buckets[index].head = new_node;
            new_node->next = temp;
        }
    }

    return true;
}

score_t* ht_get(hash_table_t* ht, c4_bitboard* key)
{
    score_t* result = NULL;
    hash_t hash = 0;
    int index = 0;
    
    
    if (ht == NULL || ht->size == 0)
    {
        return NULL;
    }

    hash = compute_hash(key);
    index = hash % ht->size;

    if (ht->buckets[index].head == NULL)
    {
        return NULL;
    }
    else
    {
        node_t* traverse = ht->buckets[index].head;

        while (traverse != NULL)
        {
            if (boards_equal(&traverse->key, key))
            {
                result = &traverse->value;
                break;
            }
            traverse = traverse->next;
        }
    }
    
    return result;
}

void ht_free(hash_table_t* ht)
{
    if (ht == NULL)
    {
        return;
    }

    for (int i = 0; i < ht->size; i++)
    {
        if (ht->buckets[i].head != NULL)
        {
            node_t* traverse = ht->buckets[i].head;
            node_t* temp;

            while (traverse != NULL)
            {
                temp = traverse->next;
                free(traverse);
                traverse = temp;
            }
        }
    }

    free(ht->buckets);
    free(ht);
}

void ht_print(hash_table_t* ht)
{
    node_t* traverse = NULL;


    if (ht == NULL || ht->size == 0)
    {
        return;
    }

    for (int i = 0; i < ht->size; i++)
    {
        traverse = ht->buckets[i].head;
        bool empty = true;

        if (traverse)
        {
            empty = false;
            printf("%d: ", i);
        }

        while (traverse != NULL)
        {
            printf("(%d) ", traverse->value);
            traverse = traverse->next;
        }

        if (!empty)
        {
            printf("\n");
        }
    }
}

c4_bitboard generate_random_board()
{
    c4_bitboard b = {0};
    c4_bitboard* pb = &b;
    piece p = RED;
    int rand_column;
    int num_of_pieces;
    
    
    num_of_pieces = (rand() % BOARD_SIZE) + 1;

    while(num_of_pieces--)
    {
        rand_column = (rand() % BOARD_WIDTH) + 1;
        drop_piece(pb, rand_column, p);
        p = OPP_PIECE(p);
    }

    return b;
}

int main()
{
    initialize_zob_table();

    // c4_bitboard board = {0};
    // board.r_board = 0x0000000000010014;
    // board.y_board = 0x0000000000000608;
    // hash_t hash =  compute_hash(&board);
    // printf("hash: 0x%08X\n", hash);
    // board.r_board = 0x0000000000030014;
    // hash ^= zob_table[3][3][0];
    // hash_t new_hash = compute_hash(&board);
    // printf("hash: 0x%08X, new-hash: 0x%08X\n", hash, new_hash);
    // board.r_board = 0x0000000000010014;
    // hash ^= zob_table[3][3][0];
    // new_hash = compute_hash(&board);
    // printf("hash: 0x%08X, new-hash: 0x%08X\n", hash, new_hash);

#define boards_count (2000)
    c4_bitboard b[boards_count];
    score_t s[boards_count];

    hash_table_t* ht = ht_create(20000);

    for (int i = 0; i < boards_count; i++)
    {
        b[i] = generate_random_board();
        s[i] = (rand() % 100) + 1;
        ht_put(ht, &b[i], &s[i]);
    }

    ht_print(ht);
    ht_free(ht);
    // ht = NULL;
    printf("freed\n");
    ht_print(ht);
}