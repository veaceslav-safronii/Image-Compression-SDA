/* SAFRONII VEACESLAV - 315CD */
#include "functions.h"

/* Constructia matricei de pixeli corespunzatori imaginii */
void grid_set(RGB *** grid, Node *root, int x, int y, unsigned size)
{
	if (!root) return;
	if (root->data) {
		/* Pentru blocul nedivizat cu dimensiunea aflata,
		se initializeaza culorile pentru toti pixelii
		ce fac parte din blocul respectiv*/
		for (int i = x; i < (int)size + x; i++) {
			for (int j = y; j < (int)size + y; j++) {
				(*grid)[i][j].red = root->data->red;
				(*grid)[i][j].green = root->data->green;
				(*grid)[i][j].blue = root->data->blue;
			}
		}
		return;
	}
	/* Micsorarea dimensiunii dupa fiecare nivel parcurs */
	size /= 2;
	/* Parcurgerea recursiva a arborelui */
	grid_set(grid, root->left_up, x, y, size);
	grid_set(grid, root->right_up, x, y + size, size);
	grid_set(grid, root->right_down, x + size, y + size, size);
	grid_set(grid, root->left_down, x + size, y, size);
}
/* ------------------------------------------------------------------ */

/* Constructia arborelui decomprimat si afisarea imaginii decoprimate */
void decompress_image(FILE *input, FILE *output)
{
	unsigned size = 0;
	fread(&size, sizeof(unsigned), 1, input);
	Queue *q = createQueue();
	Node *root = createNode();
	enqueue(q, root);
	Node *node = NULL;
	/* Parcurgerea fisierului si constructia paralela
	a arborelui decoprimat */
	while (!isQueueEmpty(q)) {
		int type = 0;
		fread(&type, sizeof(unsigned char), 1, input);
		/* Verificarea tipului de nod */
		if (type == 0) {
			/* Pentru nodul intern se creeaza
			cei 4 copii si se adauga in coada*/
			node = front(q);
			dequeue(q);
			node->left_up = createNode();
			node->right_up = createNode();
			node->right_down = createNode();
			node->left_down = createNode();
			enqueue(q, node->left_up);
			enqueue(q, node->right_up);
			enqueue(q, node->right_down);
			enqueue(q, node->left_down);
		} else if (type == 1) {
			/* Daca e frunza, se aloca memorie pentru date
			si se citesc valorile RGB */
			node = front(q);
			dequeue(q);
			node->data = malloc(sizeof(RGB));
			fread(node->data, sizeof(char) * 3, 1, input);
		} else {
			break;
		}
	}
	/* Scrierea in fisier PPM */
	int max_value = 255;
	fprintf(output, "P6\n");
	fprintf(output, "%u %u\n", size, size);
	fprintf(output, "%d\n", max_value);
	/* Alocarea memoriei pentru matricea de pixeli */
	RGB **grid = calloc(size, sizeof((*grid)));
	for (int i = 0; i < (int)size; i++) {
		grid[i] = calloc(size, sizeof(RGB));
	}
	/* Reconstruirea matricei de pixeli */
	grid_set(&grid, root, 0, 0, size);
	/* Scrierea valorilor RGB pentru fiecare pixel din imagine */
	for (int i = 0; i < (int) size; i++) {
		for (int j = 0; j < (int) size; j++) {
			fwrite(&grid[i][j].red, sizeof(char), 1, output);
			fwrite(&grid[i][j].green, sizeof(char), 1, output);
			fwrite(&grid[i][j].blue, sizeof(char), 1, output);
		}
	}
	/* Dealocarea memoriei pentru matrice, arbore si coada */
	for (int i = 0; i < (int) size; i++) {
			free(grid[i]);
		}
		free(grid);
	freeTree(&root);
	destroyQueue(q);
}
/* ------------------------------------------------------------------ */
