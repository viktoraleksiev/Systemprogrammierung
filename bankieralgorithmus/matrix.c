#include "matrix.h"

matrix* create_matrix(unsigned int m, unsigned int n, int* numbers)
{	
	
	if( m <= 0 || n <=0 || numbers == NULL){
		perror("Inkorrekte Eingabe!");
	 	return NULL;
	}	
	
	matrix* mat = malloc(sizeof(matrix));
	int* elements = malloc(sizeof(int)*n*m);

	for(int i=0; i < n*m ;i++){
		elements[i] = numbers[i];
	}
	mat->m = m;
	mat->n = n;
	mat->elements = elements;

	return mat;
}

matrix* create_matrix_from_row(unsigned int m, unsigned int row_nr, matrix* row)
{
	if( m <=0 || row_nr <0 || m <= row_nr || row == NULL){
		perror("Inkorrekte Eingabe(create matrix)!");
		return NULL;
	}
		
	matrix* mat = malloc(sizeof(matrix));
	int* elements = malloc(sizeof(int)*m*row->n);

	int i=0;
	for(i=0; i < row_nr*row->n ;i++){
		elements[i] = 0;
	}
	int j=0;
	while(i < row->n + row_nr*row->n){
		elements[i] = row->elements[j];
		i++;
		j++;
	}
	
	while(i < m*row->n){
		elements[i] = 0;
		i++;
	}
	
	mat->m = m;
	mat->n = row->n;
	mat->elements = elements;
		
	return mat;
}

void free_matrix(matrix* matrix)
{

	if(matrix != NULL){
		free(matrix->elements);
		free(matrix);
	}
	
	
}

matrix* duplicate_matrix(matrix* old)
{
	if(old == NULL){
		perror("Inkorrekte Eingabe(duplicete)");
		return NULL;
	}
	
	matrix* dup = malloc(sizeof(matrix));
	int* elements = malloc(sizeof(int)*old->m*old->n);
	for(int i=0; i < old->m*old->n ;i++){
		elements[i] = old->elements[i];
	}
	dup->m = old->m;
	dup->n = old->n;
	dup->elements = elements;

	return dup;
}

matrix* add_matrix(matrix* a, matrix* b)
{
	if(a == NULL || b == NULL ){
		perror("Inkorrekte Eingabe(addmatrix)");
		return NULL;
	}

	if( a->n != b->n || a->m != b->m){
		perror("Matrizen mit verschieden Groessen koennen nicht addiert werden!");
		return NULL;
	}

	for (int i=0; i < a->n*a->m; i++){
		a->elements[i] += b->elements[i];
	}

	return a;
}

matrix* subtract_matrix(matrix* a, matrix* b)
{	
	if(a == NULL || b == NULL ){
		perror("Inkorrekte Eingabe(submatrix)");
		return NULL;
	}
	
	if( a->n != b->n || a->m != b->m){
		perror("Matrizen mit verschieden Groessen koennen nicht subtrahiert werden!");
		return NULL;
	}

	for (int i=0; i < a->n*a->m; i++){
		a->elements[i] -= b->elements[i];
	}

	return a;
}

int get_elem_of_matrix(matrix* matrix, unsigned int i, unsigned int j)
{
		
	if(matrix == NULL || i < 0 || j < 0){
		perror("Inkorrekte Eingabe(getelem)");
	}

	if( matrix->n <= j || matrix->m <= i){
		return -1;
	}

	return matrix->elements[i*(matrix->n)+j];
}

int check_matrix(matrix* matrix)
{
	if(matrix == NULL){
		perror("Inkorrekte Eingabe(check)");
	}

	int n = 0;
	for (int i =0 ; i < matrix->n*matrix->m; i++){
		if(matrix->elements[i] < 0) return -1;
		else if(matrix->elements[i] > 0) n++;
	}

	if(n>0) return 1;
	else return 0;
}

matrix* get_row_from_matrix(matrix* mat, unsigned int m)
{
	if(mat == NULL){
		perror("Inkorrekte Eingabe(getrow)");
		return NULL;
	}

	if( m >= mat->m ){
		perror("Inkorrekte Eingabe(getrow)");
		return NULL;
	}

	matrix* get_row = malloc(sizeof(matrix));
	int* elements = malloc(sizeof(int)*mat->n);
	get_row->n = mat->n;
	get_row->m = 1;

	int i = m*mat->n;

	for (int j =0; j <  mat->n; j++){
		elements[j] = mat->elements[i];
		i++;
	}

	get_row->elements = elements;
	

	return get_row;

}

void clear_row_of_matrix(matrix* matrix, unsigned int m)
{

	if( matrix==NULL || m < 0){
		perror("Inkorrekte Eingabe(clear)");	
		return;
	}
	
	if( matrix->m <= m){
		perror("Inkorrekte Eingabe(clear)");	
		return;
	}
	
	int j =  m*matrix->n;
	for(int i = 0; i < matrix->n; i++){
		matrix->elements[j] = 0;
		j++;
	}
}

void print_matrix(matrix* matrix)
{
	if(matrix == NULL)
	{
		printf("ERROR: print_matrix() | given matrix is NULL\n");
		return;
	}


	//determines the largest element
	int max = get_elem_of_matrix(matrix, 0,0);
	for(int i = 0; i< matrix->m; i++)
	{
		for(int j = 0; j < matrix->n; j++)
		{
			int current = get_elem_of_matrix(matrix, i,j);
			max = (current>max) ? current : max;
		}
	}

	int grade = 1;	//represents the number of digits of the largest element
	while(max > 9)
	{
		max/=10;
		grade++;
	}

	//prints upper line
	printf(" ");
	for(int i = 0; i< ((grade + 1)*matrix->n +1); i++)
	{
		printf("-");
	}
	printf("\n");

	//prints body
	for(int i = 0; i< matrix->m; i++)
	{
		printf("| ");
		for(int j = 0; j < matrix->n; j++)
		{
			printf("%*i ", grade, get_elem_of_matrix(matrix, i, j));
		}
		printf("|\n");
	}

	//prints lower line
	printf(" ");
	for(int i = 0; i< ((grade + 1)*matrix->n +1); i++)
	{
		printf("-");
	}
	printf("\n");
}


//===============================================================================

/*TODO:
	Hier koennen bei Bedarf eigene Funktionen implementiert werden
*/


//===============================================================================
