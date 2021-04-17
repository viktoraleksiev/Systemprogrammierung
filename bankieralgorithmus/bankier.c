#include "bankier.h"

int check_initial_state(matrix* f, matrix* G)
{
	matrix* probe = duplicate_matrix(f);
	clear_row_of_matrix(probe,0);
	
	// adds all colums in G
	for (int y = 0; y < G->n; y++){
		for(int x = 0; x< G->m; x++){
			probe->elements[y] += get_elem_of_matrix(G,x,y);
		}
	}
	// adds all colums in f
	for (int i = 0; i < G->n; i++){
		probe->elements[i] += f->elements[i];
	}

	int check = 1;
	for (int y = 0; y < G->n; y++){
		for(int x = 0; x< G->m; x++){
			if( probe->elements[y] < get_elem_of_matrix(G,x,y) ) check = 0;
		}
	}
	
	free(probe->elements);
	free(probe);
	return check;
}

int check_allocation(matrix* B, matrix* R, matrix* f, matrix* allocation, unsigned int ID)
{
	matrix * alloc = get_row_from_matrix(allocation,ID);

	// if the new alloc ist greater than the free BM
	for (int i =0; i < f->n; i++){
		if( alloc->elements[i] > f->elements[i])
		{ 

			clear_row_of_matrix(R,ID);
			free(alloc->elements);
			free(alloc);
			return 0;
		}
	}

	add_matrix(B,allocation);
	subtract_matrix(R,allocation);
	subtract_matrix(f,alloc);


	matrix * row_R = get_row_from_matrix(R,ID);

	// if process is ready 
	if(check_matrix(row_R)==0){
			matrix * row_B = get_row_from_matrix(B,ID);
			clear_row_of_matrix(B,ID);
			add_matrix(f,row_B);
			free(row_B->elements);
			free(row_B);
	}

	// Bankieralgorithmus
	int check = check_state(B,R,f);

	// clean
	free(row_R->elements);
	free(row_R);
	free(alloc->elements);
	free(alloc);


 	// if bankirer says secure 
	if (check == 1){

			matrix * row_R = get_row_from_matrix(R,ID);

			if(check_matrix(row_R)==0){
			matrix * row_B = get_row_from_matrix(B,ID);
			clear_row_of_matrix(B,ID);
			add_matrix(f,row_B);

			free(row_B->elements);
			free(row_B);
			}

			// clean
			free(row_R->elements);
			free(row_R);

	 		return 1;
	}
	//if not 
	else {
			matrix* row_B = get_row_from_matrix(B, ID);
			matrix* B1 = create_matrix_from_row(B->m,ID,row_B);
			//add_matrix(R,B1);
			clear_row_of_matrix(R,ID);
			add_matrix(f,row_B);

			//clear_row_of_matrix(B,ID);
			subtract_matrix(B,allocation);

			free(row_B->elements);
			free(row_B);
			free(B1->elements);
			free(B1);
			return 0;
	}
}

int check_state(matrix* B, matrix* R, matrix* f)
{
	matrix* copy_B = duplicate_matrix(B);
	matrix* copy_R = duplicate_matrix(R);
	matrix* copy_f = duplicate_matrix(f);

	// check all row until the matrix is empty
	while(check_matrix(copy_R)!=0){

		for(int i = 0; i < R->m; i++){
			// current row
			matrix * row = get_row_from_matrix(copy_R,i);

			// if current row is empty continue, if empty and last row return not secure (0)
			if(check_matrix(row)==0) {
				if(i == R->m - 1) {
					free(row->elements);
					free(row);
					free(copy_B->elements);
					free(copy_R->elements);
					free(copy_f->elements);
					free(copy_B);
					free(copy_R);
					free(copy_f);
					return 0;
				}
				free(row->elements);
				free(row);
				continue;
			}

			int row_found = 1;
			// checks the row if executable
			for(int k =0; k < f->n; k++){
				if(row->elements[k] > copy_f->elements[k]) row_found = 0;
			}
			// if yes: clears the corresponding rows in B and R, f += B_i  
			if(row_found){

				matrix* row_B = get_row_from_matrix(copy_B,i);
				add_matrix(copy_f,row_B);

				clear_row_of_matrix(copy_B,i);
				clear_row_of_matrix(copy_R,i);

				free(row_B->elements);
				free(row->elements);	
				free(row_B);
				free(row);
				break;
			} 
			// if not: continues, if last row return not secure(0) 
			else {
				if(i == R->m - 1) {
					free(row->elements);
					free(row);
					free(copy_B->elements);
					free(copy_R->elements);
					free(copy_f->elements);
					free(copy_B);
					free(copy_R);
					free(copy_f);
					return 0;
				}
				free(row->elements);
				free(row);
				continue;
			}


		}
	}

    // if R is empty in the end then the situation is secure 
	free(copy_B->elements);
	free(copy_R->elements);
	free(copy_f->elements);
	free(copy_B);
	free(copy_R);
	free(copy_f);
	return 1;
}

//===============================================================================

/*TODO:
	Hier koennen bei Bedarf eigene Funktionen implementiert werden
*/


//===============================================================================

int abs(int n)
{
	return (n>0) ? n : -n;
}

void print_BRf(matrix* B, matrix* R, matrix* f)
{
	unsigned int m = B->m;
	unsigned int n = B->n;

	if(B==NULL || R==NULL || f==NULL)
	{
		printf("ERROR: print_BRf() | given matrices = NULL.\n");
		return;
	}

	if(f->m != 1 || f->n != n || R->m !=m || R->n != n)
	{
		printf("ERROR: print_BRf() | given matrices dont have matching dimensions.\n");
		return;
	}

	int max_B = get_elem_of_matrix(B, 0,0);		//largest elements
	int max_R = get_elem_of_matrix(R, 0,0);
	int max_f = get_elem_of_matrix(f, 0,0);

	int grade_B = 1;	//represents the number of digits of the largest element
	int grade_R = 1;
	int grade_f = 1;

	int width_B;
	int width_R;
	int width_f;


	for(unsigned int i = 0; i< B->m; i++)
	{
		for(unsigned int j = 0; j < B->n; j++)
		{
			int current = get_elem_of_matrix(B, i,j);
			max_B = (abs(current)>max_B) ? current : max_B;
		}
	}

	for(unsigned int i = 0; i< R->m; i++)
	{
		for(unsigned int j = 0; j < R->n; j++)
		{
			int current = get_elem_of_matrix(R, i,j);
			max_R = (abs(current)>max_R) ? current : max_R;
		}
	}

	for(unsigned int j = 0; j < f->n; j++)
	{
		int current = get_elem_of_matrix(f, 0,j);
		max_f = (abs(current)>max_f) ? current : max_f;
	}

	if(max_B < 0) grade_B++;
	while(max_B > 9 || max_B < -9)
	{
		max_B/=10;
		grade_B++;
	}
	width_B = 3 + n*(grade_B+1);

	if(max_R < 0) grade_R++;
	while(max_R > 9 || max_R < -9)
	{
		max_R/=10;
		grade_R++;
	}
	width_R = 3 + n*(grade_R+1);

	if(max_f < 0) grade_f++;
	while(max_f > 9 || max_f < -9)
	{
		max_f/=10;
		grade_f++;
	}
	width_f = 3 + n*(grade_f+1);

	//print upper line (B: R: f:)
	for(int i = 0; i < width_B + width_R + width_f + 2*3; i++)
	{
		if(i==(int)((width_B-1)/2)) printf("B");
		else if(i==(int)(width_B + 3 + (width_R-1)/2)) printf("R");
		else if(i==(int)(width_B + 3 + width_R + (width_f-1)/2)) printf("f");
		else printf(" ");
	}
	printf("\n");

	//================================ print B ================================= 
	//prints upper line
	printf(" ");
	for(int i = 0; i< width_B-2; i++) 	printf("-");

	printf("\n");

	//prints body
	for(unsigned int i = 0; i< m; i++)
	{
		printf("| ");
		for(unsigned int j = 0; j < n; j++)
		{
			int elem = get_elem_of_matrix(B, i, j);
			if(elem < 0) printf("\033[1;31m%*i\033[0m ", grade_B, elem);
			else 		 printf("%*i ", grade_B, elem);
			
		}
		printf("|\n");
	}

	//prints lower line
	printf(" ");
	for(int i = 0; i< width_B-2; i++)	printf("-");
	
	printf("\r\033[%iC\033[%iA", (width_B+3), (m+1));	//move curser forwards and upwards

	//================================ print R ================================= 
	//prints upper line
	printf(" ");
	for(int i = 0; i< width_R-2; i++) 	printf("-");

	printf("\033[%iD\033[%iB", (width_R-1), 1);		//move curser backwards and downwards

	//prints body
	for(int i = 0; i< m; i++)
	{
		printf("| ");
		for(int j = 0; j < n; j++)
		{
			int elem = get_elem_of_matrix(R, i, j);
			if(elem < 0) printf("\033[1;31m%*i\033[0m ", grade_R, elem);
			else 		 printf("%*i ", grade_R, elem);
		}
		printf("|\033[%iD\033[%iB", width_R, 1);	//move curser backwards and downwards
	}

	//prints lower line
	printf(" ");
	for(int i = 0; i< width_R-2; i++)	printf("-");
	
	printf("\033[%iC\033[%iA", 4, (int)(m/2 +2));		//move curser forwards and upwards


	//================================ print f ================================= 
	//prints upper line
	printf(" ");
	for(int i = 0; i< width_f-2; i++) 	printf("-");

	printf("\033[%iD\033[%iB", (width_f-1), 1);		//move curser backwards and downwards

	//prints body
	printf("| ");
	for(int j = 0; j < n; j++)
	{
		int elem = get_elem_of_matrix(f, 0, j);
		if(elem < 0) printf("\033[1;31%*i\033[0m ", grade_f, elem);
		else 		 printf("%*i ", grade_f, elem);
	}
	printf("|\033[%iD\033[%iB", width_f, 1);	//move curser backwards and downwards

	//prints lower line
	printf(" ");
	for(int i = 0; i< width_f-2; i++)	printf("-");
	
	printf("\r\033[%iB\n\n", (int)(m/2 +1));	//move curser backwards and downwards

}
