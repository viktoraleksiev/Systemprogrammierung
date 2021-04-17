#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "simple_ray/ray_default_scene.h"
//#define DEBUG
int main(int argc, char** argv)
{
	/*check and parse command line arguments*/
        int num_processes, square_side, samples;
	 if (argc != 4) {
                perror( "Not enough arguments were passed!" );
        }
	else {
	
		num_processes = atoi(argv[1]);
		square_side = atoi(argv[2]);
		samples = atoi(argv[3]);
	
		if( (num_processes < 1 || num_processes > 128 )||( square_side < 1 || square_side > 10000 )|| samples < 1){
			perror("Incorrect input!");
		}else{

			printf("Anzahl der Prozesse: %d\n",num_processes);
			printf("Aufloesung: %dx%d\n",square_side,square_side);
			printf("Samples: %d\n",samples);
		}
	}

	/* initialize scene */
	ray_Scene* scene = ray_createDefaultScene();	

	/*naming the child files*/
	char names[num_processes][10];
	for (int i = 0; i < num_processes; i++){
	sprintf(names[i],"%d",i);
        strcat(names[i], ".bmp");
	}
	
	/*dividing the workload (cosidering also the rest in division)*/
	int coordinates[num_processes][4];
	int k =0;
	int rest = square_side % num_processes;
	int int_division = square_side / num_processes;
	for(k = 0; k < rest; k++){
		coordinates[k][0] = k*(int_division + 1);
		coordinates[k][1] = 0;
		coordinates[k][2] = int_division + 1;
		coordinates[k][3] = square_side;
	}
	for(int j = 0; j < num_processes - rest; j++){
		coordinates[k][0] = j*int_division + rest*(int_division + 1);
		coordinates[k][1] = 0;
		coordinates[k][2] = int_division;
		coordinates[k][3] = square_side;
		k++;
	}

	/* coordinates for testing*/ 
#ifdef DEBUG
	for (int i = 0; i < num_processes;i++){
		for (int j = 0; j < 4; j++){
			printf("%d ",coordinates[i][j]);
		}
		printf("\n");
	}
#endif
	
	
	pid_t pids[num_processes];
	for (int i=0; i < num_processes;i++){
		pids[i] = 0;
	}

	/*creating n child processes,
	each child process renders it's part of the image and saves it to an individual file
	*/
	for (int i = 0; i < num_processes; i++){		
		if ((pids[i]=fork())== 0){	// child process
			pids[i] = getpid();
			char buff[10];
			sprintf(buff,"PID: %d",pids[i]);
			bmp_Rect* new_Rect = malloc(sizeof(bmp_Rect));
			new_Rect->x = coordinates[i][0];
			new_Rect->y = coordinates[i][1];
			new_Rect->w = coordinates[i][2];
			new_Rect->h = coordinates[i][3];	
				
			bmp_Image* new = ray_renderScene(new_Rect,square_side,square_side,samples,scene,buff);
			bmp_saveToFile(new,names[i]);
			
			free(new_Rect);
			bmp_free(new);

			ray_freeScene(scene); 
            		exit(0); 
		}
	}
	/*parent waits for child processes to terminate*/
	for (int i = 0; i< num_processes; i++){
			waitpid(pids[i], NULL, 0);
	}
	/* creating empty image */
	bmp_Image * final_Image = bmp_loadFromData(square_side,square_side,NULL);
	
	/* loading and merging all files into single image */
	for (int i = 0; i < num_processes; i++){
				
			bmp_Image * bild = bmp_loadFromFile(names[i]);
			bmp_stamp(bild,final_Image,coordinates[i][0],0);
			bmp_free(bild);

	}
	/* saving final image */
	bmp_saveToFile(final_Image,"final.bmp");
	bmp_free(final_Image);
	
	/* free memory */
	ray_freeScene(scene);
	
	return 0;
}
