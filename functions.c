/* SAFRONII VEACESLAV - 315CD */
#include "functions.h"

/* Crearea unui nod al arborelui */
Node* createNode()
{
	Node *root = calloc(1, sizeof(Node));
	return root;
}
/* -------------------------------------- */

/* Dealocarea arborelui */
void freeTree(Node **root)
{
	if (!(*root)) return;
	freeTree(&(*root)->left_up);
	freeTree(&(*root)->right_up);
	freeTree(&(*root)->right_down);
	freeTree(&(*root)->left_down);
	free((*root)->data);
	free((*root)->left_up);
	free((*root)->right_up);
	free((*root)->right_down);
	free((*root)->left_down);
	free(*root);
	*root = NULL;
}
/* ------------------------------------ */

/* Crearea cozii */
Queue* createQueue()
{
	Queue *queue = calloc(1, sizeof(Queue));
	return queue;
}
/* ----------------------------------------- */

/* Verificarea golirii cozii */
int isQueueEmpty(Queue *q)
{
	// if (q == NULL) return 1;
	if (q->front == NULL || q->rear == NULL) {
		return 1;
	}
	return 0;
}
/* -------------------------------------------- */

/* Introducearea unui element in coada */
void enqueue(Queue *q, Node* elem)
{
	if (!q) return;
	QueueNode *node = calloc(1, sizeof(QueueNode));
	node->elem = elem;
	if (isQueueEmpty(q)) {
		q->front = q->rear = node;
		return;
	}
	q->rear->next = node;
	q->rear = node;
}
/* ------------------------------------------------- */

/* Intoarcerea elementului din capul cozii */
Node* front(Queue* q)
{
	if (isQueueEmpty(q)) {
		return NULL;
	}
	return q->front->elem;
}
/* --------------------------------------- */

/* Scoaterea unui element din coada */
void dequeue(Queue* q)
{
	if (!q) return;
	if (isQueueEmpty(q)) {
		return;
	}
	QueueNode *temp = q->front;
	if (q->front == q->rear) {
		q->rear = NULL;
	}
	q->front = q->front->next;
	free(temp);
}
/* -------------------------------- */

/* Dealocarea cozii */
void destroyQueue(Queue *q)
{
	while (!isQueueEmpty(q)) {
		dequeue(q);
	}
	free(q);
}
/* ---------------------------- */
