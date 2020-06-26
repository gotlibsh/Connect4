#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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
    size_t buckets_count;
    size_t nodes_count;
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

uint64_t get_64_bit_random()
{
    uint64_t res = 0;

    res |= (uint64_t)rand() & 0xff;
    res |= (uint64_t)(rand() & 0xff) << 8;
    res |= (uint64_t)(rand() & 0xff) << 16;
    res |= (uint64_t)(rand() & 0xff) << 24;
    res |= (uint64_t)(rand() & 0xff) << 32;
    res |= (uint64_t)(rand() & 0xff) << 40;
    res |= (uint64_t)(rand() & 0xff) << 48;
    res |= (uint64_t)(rand() & 0xff) << 56;

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

    ht->buckets = (bucket_t*)malloc(size * sizeof(bucket_t));

    if (ht->buckets == NULL)
    {
        free(ht);
        return NULL;
    }

    memset(ht->buckets, 0, size * sizeof(bucket_t));
    ht->buckets_count = size;
    ht->nodes_count = 0;

    return ht;
}

bool ht_put(hash_table_t* ht, c4_bitboard* key, score_t* value)
{
    hash_t hash = 0;
    int index = 0;


    if (ht == NULL || ht->buckets_count == 0)
    {
        return false;
    }

    hash = compute_hash(key);
    index = hash % ht->buckets_count;

    if (ht->buckets[index].head == NULL)    // empty bucket
    {
        node_t* new_node = create_node(key, value);

        if (new_node == NULL)
        {
            return false;
        }

        ht->buckets[index].head = new_node;
    }
    else                         // non-empty bucket
    {
        node_t* traverse = ht->buckets[index].head;

        while (traverse != NULL) // check if the same key is already stored
        {
            if (boards_equal(&traverse->key, key))
            {                    // same key is stored, override its value
                traverse->value = *value;
                break;
            }
            traverse = traverse->next;
        }

        if (traverse == NULL)    // new key, store at the beginning
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

    ht->buckets[index].size++;
    ht->nodes_count++;

    return true;
}

score_t* ht_get(hash_table_t* ht, c4_bitboard* key)
{
    score_t* result = NULL;
    hash_t hash = 0;
    int index = 0;
    
    
    if (ht == NULL || ht->buckets_count == 0)
    {
        return NULL;
    }

    hash = compute_hash(key);
    index = hash % ht->buckets_count;

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

void ht_free(hash_table_t** ht)
{
    if (ht == NULL || *ht == NULL)
    {
        return;
    }

    for (int i = 0; i < (*ht)->buckets_count; i++)
    {
        if ((*ht)->buckets[i].head != NULL)
        {
            node_t* traverse = (*ht)->buckets[i].head;
            node_t* temp;

            while (traverse != NULL)
            {
                temp = traverse->next;
                free(traverse);
                traverse = temp;
            }
        }
    }

    free((*ht)->buckets);
    (*ht)->buckets = NULL;
    (*ht)->buckets_count = 0;
    (*ht)->nodes_count = 0;
    free(*ht);
    *ht = NULL;
}

void ht_print(hash_table_t* ht)
{
    node_t* traverse = NULL;


    if (ht == NULL || ht->buckets_count == 0)
    {
        return;
    }

    for (int i = 0; i < ht->buckets_count; i++)
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

void ht_stats(hash_table_t* ht)
{
    double avg, var;
    double temp_sum = 0.0;
    size_t cnt20 = 0, cnt100 = 0, cnt1000 = 0, max = 0;

    if (ht == NULL || ht->buckets_count == 0)
    {
        return;
    }

    avg = (double)ht->nodes_count / ht->buckets_count;

    for (int i = 0; i < ht->buckets_count; i++)
    {
        temp_sum += pow(ht->buckets[i].size - avg, 2);

        if (ht->buckets[i].size > max)
        {
            max = ht->buckets[i].size;
        }

        if (ht->buckets[i].size - avg > 1000)
        {
            cnt1000++;
        }
        else if (ht->buckets[i].size - avg > 100)
        {
            cnt100++;
        }
        else if (ht->buckets[i].size - avg > 20)
        {
            cnt20++;
        }
    }

    var = temp_sum / ht->buckets_count;

    printf("biggest-bucket:            %lld\n", max);
    printf("buckets-greater-than-20:   %lld\n", cnt20);
    printf("buckets-greater-than-100:  %lld\n", cnt100);
    printf("buckets-greater-than-1000: %lld\n", cnt1000);
    printf("buckets-count:             %lld\n", ht->buckets_count);
    printf("nodes-count:               %lld\n", ht->nodes_count);
    printf("average:                   %.02f\n", avg);
    printf("variance:                  %.02f\n", var);
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

    int boards_count = 60000000;
    clock_t t;
    double time_taken = 0.0;

    c4_bitboard b = {0};
    score_t s = 0;

    t = clock();
    hash_table_t* ht = ht_create(20000000);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("ht_create took: %f seconds\n", time_taken);

    t = clock();

    for (int i = 0; i < boards_count; i++)
    {
        b = generate_random_board();
        s = (rand() % 100) + 1;
        ht_put(ht, &b, &s);
    }

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("generate_random_board + ht_put took: %f seconds\n", time_taken);

    ht_stats(ht);

    t = clock();

    ht_free(&ht);

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("ht_free took: %f seconds\n", time_taken);
}
