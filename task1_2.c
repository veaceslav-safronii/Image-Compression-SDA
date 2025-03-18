/* SAFRONII VEACESLAV - 315CD */
#include "functions.h"

/* Citirea fisierului binar si salvarea datelor pixelilor */
void readFile(FILE *input, int *size, RGB ***grid)
{
	char file_type[2] = {};
	int width = 0;
	int height = 0;
	int max_value = 0;
	fscanf(input, "%s", file_type);
	fscanf(input, "%d", &width);
	fscanf(input, "%d", &height);
	fscanf(input, "%d", &max_value);
	*size = width;
	fseek(input, sizeof(char), SEEK_CUR);
	(*grid) = calloc(height,sizeof((*grid)));
	for (int i = 0; i < height; i++) {
		(*grid)[i] = calloc(width, sizeof(RGB));
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			fread(&(*grid)[i][j].red, sizeof(char), 1, input);
			fread(&(*grid)[i][j].green, sizeof(char), 1, input);
			fread(&(*grid)[i][j].blue, sizeof(char), 1, input);
		}
	}
}
/* ----------------------------------------------------------------*/

/* Calcularea mediilor culorilor */
void average_value(RGB **grid, int row, int col, int size,
    unsigned* red, unsigned* green, unsigned* blue)
{
	unsigned sum_red = 0;
	unsigned sum_blue = 0;
	unsigned sum_green = 0;
	for (int i = row; i < size + row; i++) {
		for (int j = col; j < size + col; j++) {
			sum_red += grid[i][j].red;
			sum_green += grid[i][j].green;
			sum_blue += grid[i][j].blue;
		}
	}
	*red = floor((1 / pow(size, 2)) * sum_red);
	*green = floor((1 / pow(size, 2)) * sum_green);
	*blue = floor((1 / pow(size, 2)) * sum_blue);
}
/* -------------------------------------------------- */

/* Calcularea valorii scorului de similaritate a blocului */
unsigned long long mean_score(RGB **grid, int row, int col, int size,
    unsigned red, unsigned green, unsigned blue)
{
	unsigned long long sum = 0;
	for (int i = row; i < size + row; i++) {
		for (int j = col; j < size + col; j++) {
			sum += (red - grid[i][j].red)*(red - grid[i][j].red);
			sum += (green - grid[i][j].green)*(green - grid[i][j].green);
			sum += (blue - grid[i][j].blue)*(blue - grid[i][j].blue);
		}
	}
	unsigned long long mean = floor((1 / (3 * pow(size, 2))) * sum);
	return mean;
}
/* --------------------------------------------------------------------- */

/* Construirea recursiva a arborelui folosind
informatiile extrase din fisierul PPM */
Node* createTree(Node* root, unsigned factor, RGB **grid, int row, int col,
	int size, unsigned red, unsigned green, unsigned blue)
{
	if (!root) {
		root = createNode();
	}
	/* Compararea scorului de similaritate cu pragul dat ca parametru.
	In cazul in care este mai mic decat factorul, nodul/blocul nu va fi
	impartit si ii vor fi atribuite valorile medii RGB.
	Altfel nodul/blocul va fi impartit in 4 copii/blocuri */
	unsigned mean = mean_score(grid, row, col, size, red, green, blue);
	if (mean <= factor) {
		root->data = malloc(sizeof(RGB));
		root->data->red = red;
		root->data->green = green;
		root->data->blue = blue;
		root->size = size;
		return root;
	}
	/* Schimbarea dimensiunii blocurilor dupa fiecare nivel parcurs */
	size /= 2;
	/* Recalcularea valorilor RGB pentru fiecare bloc,
	in dependenta de pozitia acestuia in imagine */
	average_value(grid, row, col, size, &red, &green, &blue);
	root->left_up = createTree(root->left_up, factor, grid, row,
					col, size, red, green, blue);

	average_value(grid, row, col + size, size, &red, &green, &blue);
	root->right_up = createTree(root->right_up, factor, grid, row ,
					col + size, size, red, green, blue);

	average_value(grid, row + size, col + size, size, &red, &green, &blue);
	root->right_down = createTree(root->right_down, factor, grid, row + size,
					col + size, size, red, green, blue);

	average_value(grid, row + size, col, size, &red, &green, &blue);
	root->left_down = createTree(root->left_down, factor, grid, row + size,
					col, size, red, green, blue);
	return root;
}
/* ------------------------------------------------------------------------ */

/* Calcularea nr-lui de niveluri ale arborelui */
int get_levels(Node *root)
{
	if (!root) {
		return 0;
	}
	int depth1 = get_levels(root->left_up);
	int depth2 = get_levels(root->right_up);
	int depth3 = get_levels(root->right_down);
	int depth4 = get_levels(root->left_down);
	/* Calcularea recursiva a adancimii tuturor copiilor
	si returnarea valorii maxime dintre cele 4 */
	int depth = 0;
	if (depth1 > depth2) {
		depth = depth1;
	} else depth = depth2;
	if (depth3 > depth) {
		depth = depth3;
	}
	if (depth4 > depth) {
		depth = depth4;
	}
	return depth + 1;
}
/* --------------------------------------------- */

/* Calcularea nr-ului de blocuri nedivizate / frunzelor din arbore */
void undivised_blocks(Node* root, int* nr)
{
	if (!root) {
		return;
	}
	if (!root->left_up) {
		(*nr)++;
	}
	undivised_blocks(root->left_up, nr);
	undivised_blocks(root->right_up, nr);
	undivised_blocks(root->right_down, nr);
	undivised_blocks(root->left_down, nr);
}
/* --------------------------------------------------------------- */

/* Gasirea frunzei / blocului nedivizat, cu cea mai mare dimensiune
si pastrarea dimensiunii acesteia(-uia) */
void biggest_undivised_block(Node* root, int cur_lvl,
	int* min_lvl, unsigned *dim)
{
	if (!root) return;
	if (root->left_up == NULL) {
		/* Actualizarea nivelului minim al frunzelor gasite
		si pastrarea dimensiunii frunzei, de pe un nivel mai
		apropiat de radacina. */
		if (cur_lvl < *min_lvl) {
			*min_lvl = cur_lvl;
			*dim = root->size;
			return;
		}
	}
	biggest_undivised_block(root->left_up, cur_lvl + 1, min_lvl, dim);
	biggest_undivised_block(root->right_up, cur_lvl + 1, min_lvl, dim);
	biggest_undivised_block(root->right_down, cur_lvl + 1, min_lvl, dim);
	biggest_undivised_block(root->left_down, cur_lvl + 1, min_lvl, dim);
}
/* ----------------------------------------------------------------------- */

/* Crearea fisierului comprimat */
void compress_image(FILE* output, Node* root, unsigned size)
{
	if (!root) {
		return;
	}
	/* Se parcurge arborele, creat anterior, pe niveluri.
	Se verifica fiecare nod daca e intern sau frunza.*/
	Queue *q = createQueue();
	enqueue(q, root);
	fwrite(&size, sizeof(size), 1, output);
	while (!isQueueEmpty(q)) {
		Node *node = front(q);
		dequeue(q);
		if (node->left_up) {
			/* In caz ca e nod intern se adauga copii acestuia
			in coada si se scrie tipul nodului - 0, in fisier. */
			enqueue(q, node->left_up);
			enqueue(q, node->right_up);
			enqueue(q, node->right_down);
			enqueue(q, node->left_down);
			unsigned char node_type = (unsigned char) 0;
			fwrite(&node_type, sizeof(unsigned char), 1, output);
			
		} else {
			/* Daca e frunza se indica in fisier
			tipul "1" si valorile RGB a frunzei */
			unsigned char red = node->data->red;
			unsigned char green = node->data->green;
			unsigned char blue = node->data->blue;
			unsigned char node_type = (unsigned char) 1;
			fwrite(&node_type, sizeof(unsigned char), 1, output);
			fwrite(&red, sizeof(unsigned char), 1, output);
			fwrite(&green, sizeof(unsigned char), 1, output);
			fwrite(&blue, sizeof(unsigned char), 1, output);
		}
	}
	destroyQueue(q);
}
/* ------------------------------------------------------------------- */
