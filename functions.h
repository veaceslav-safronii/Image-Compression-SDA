#if !defined(_FUNCTIONS_H_)
#define _FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Structura unui pixel */
typedef struct {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} RGB;
/* -------------------- */

/* Structura unui nod al arborelui */
typedef struct node {
	RGB *data;
	unsigned size;
	struct node *left_up;
	struct node *right_up;
	struct node *right_down;
	struct node *left_down;
} Node;
/* ------------------------------- */

/* Structura nodului de coada */
typedef struct QueueNode {
    Node* elem;
    struct QueueNode *next;
} QueueNode;
/* -------------------------- */

/* Structura cozii */
typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;
/* --------------- */

/* Declaratiile functiilor folosite */
Node* createNode();

void freeTree(Node **root);

void average_value(RGB **grid, int row, int col, int size,
	unsigned* red, unsigned* green, unsigned* blue);

unsigned long long mean_score(RGB **grid, int row, int col, int size,
	unsigned red, unsigned green, unsigned blue);

Node* createTree(Node* root, unsigned factor, RGB **grid, int row, int col,
	int size, unsigned red, unsigned green, unsigned blue);

int get_levels(Node *root);

void undivised_blocks(Node* root, int* nr);

void biggest_undivised_block(Node* root, int cur_lvl,
	int* min_lvl, unsigned *dim);

void readFile(FILE *input, int *size, RGB ***grid);

Queue* createQueue();

int isQueueEmpty(Queue *q);

void enqueue(Queue *q, Node* elem);

Node* front(Queue* q);

void dequeue(Queue* q);

void destroyQueue(Queue *q);

void compress_image(FILE* output, Node* root, unsigned size);

void grid_set(RGB *** grid, Node *root, int x, int y, unsigned size);

void decompress_image(FILE *input, FILE *output);
/* -------------------------------------------------------------------- */

#endif // _FUNCTIONS_H_
