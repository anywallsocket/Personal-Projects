/*Simple Evolutionary Algorithm designed to interface with Vesta Software, run by VASP for crystal structure optimization*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define randlimit 0.27
#define natoms 4
#define maxbondlength 0.25
#define minbondlength 0.22
#define pop 1000 				    //population size
#define win 6				       //bonds required for global condition (fit enough to inspect)
#define fit 3				      //bonds required for local condition (fit enough to reproduce)
#define PI 3.1415
#define A 1.047				  //angle preference (radians)

#define degree 0.1			//angle preference precission

float coords[pop][3*natoms];//pure
float coordz[pop][3*natoms];//mixings

int parents[pop];

int bonds, angles, cops; 				//generation number and mixes per gen

//Functions

void Fitness(int n);		//selection
	float distanceBetween(float ax, float ay, float az, float bx, float by, float bz);
	float angleBetween(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz);

void Mixing();				//reproduction
	void copulate(char sign, int a, int b);
	void printbest(int n, char* file);

//Driver

int main(){

	int gen = 0, best = 0;
	int i, n;
	FILE* f;

 //Generation Loop

	while(1){

	    srand((unsigned int)time(NULL));
	   	printf("\nGen [%d] Stats:\n", gen);

	 //Species Loop

	    for (n = 0; n<pop; n++){			//randomize unfit coordinates, average fit ones

	    	bonds = 0; angles = 0;

			for (i=0; i<3*natoms; i++){

				if(coordz[n][i]==0){
					coords[n][i] = 0.50 - 0.25/2 + (float)((double)rand()/(double)(RAND_MAX)) * randlimit;
					i++;
				}
				else{coords[n][i] = coordz[n][i]; i++;}
				
				if(coordz[n][i]==0){			
					coords[n][i] = 0.50 - 0.25/2 + (float)((double)rand()/(double)(RAND_MAX)) * randlimit;
					i++;
				}
				else{coords[n][i] = coordz[n][i]; i++;}

				if(coordz[n][i]==0){			
					coords[n][i] = 0.50 - 0.25/2 + (float)((double)rand()/(double)(RAND_MAX)) * randlimit;
				}
				else{coords[n][i] = coordz[n][i];}
			}

		 //Count Bonds and Angles in Species

			Fitness(n);

			//printf("%d\n", angles);
			//printf("[%d] bonds in species [%d]\n", bonds, n);

			if (bonds > best){
				best = bonds;
				printbest(n, "best.vasp");
			}

		 //Fit Become Parents

			if (bonds >= fit || angles >=fit)
				parents[n] = 1;
			else 
				parents[n] = 0;
		}

		for (int i=0; i<pop; i++)
			printf("[%d]", parents[i]);
		printf("\n");

		Mixing(); // tallys fit parents and averages their coordinates
		printf("%d Successful Mixings\n", cops);

		gen++;

		if (cops > 50*pop){
			printf("Over Populated\n\n"); break;}
		else if ((cops == 0) && (gen != 1)){
			printf("Under Populated\n\n"); break;
		}
		else if (best >= win){
			//printbest(n, n + '0');
			printf("Best POSCAR written to best.vasp\n\n"); break;
		}
		else printf("best: %d out of %d", best, win);

		printf("\n");
	}

	return 0;
}

//Functions

void Fitness(int n){

	int i, j, k;
	float dis = 0, deg = 0;			//distance and degree bewtween atoms

	angles = 0, bonds = 0;

	for (i = 0; i<3*natoms; i+=3){
		
		for (j = 0; j<3*(natoms-2) && j!=i; j+=3){

			dis = distanceBetween(coords[n][i], coords[n][i+1], coords[n][i+2], coords[n][j], coords[n][j+1], coords[n][j+2]);

			if(dis != 0 && dis <= maxbondlength && dis >= minbondlength){
				//printf("bond d:%.3f\tatom#%d\t(%.3f, %.3f, %.3f) & atom#%d\t(%.3f, %.3f, %.3f)\n", d, 1+(i+1)/3, coords[n][i], coords[n][i+1], coords[n][i+2], 1+(j+1)/3, coords[n][j], coords[n][j+1], coords[n][j+2]);
				bonds++;
			}

			for (k = 0; k<3*(natoms-2) && k!=j && k!=i; k+=3){

				deg = angleBetween(coords[n][i], coords[n][i+1], coords[n][i+2], coords[n][j], coords[n][j+1], coords[n][j+2], coords[n][k], coords[n][k+1], coords[n][k+2]);

				if (fabs(deg-A) <= degree || fabs(deg-(PI-A)) <= degree){
					//printf("g%d - Angle between A(%d)B(%d)C(%d) = %f\n", n, (i+1)/3, (j+1)/3, (k+1)/3, deg);
					angles++; 

				}
			}
		}
	}
}

float distanceBetween(float ax, float ay, float az, float bx, float by, float bz){

	return (fabs(pow((pow((ax-bx),2)+pow((ay-by),2)+pow((az-bz),2)), 0.5)));
}

float angleBetween(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz){

	//A-B-C where angle is determined from B (BA = v1, BC = v2)

	float v1x = ax-bx, v1y = ay-by, v1z = az-bz;
	float v2x = cx-bx, v2y = cy-by, v2z = cz-bz;

	float v1mag = pow((pow((v1x), 2) + pow((v1y), 2) + pow((v1z), 2)), 0.5);
	float v2mag = pow((pow((v2x), 2) + pow((v2y), 2) + pow((v2z), 2)), 0.5);

	float v1normx = v1x/v1mag, v1normy = v1y/v1mag, v1normz = v1z/v1mag;
	float v2normx = v2x/v2mag, v2normy = v2y/v2mag, v2normz = v2z/v2mag;

	float dot = (v1normx * v2normx) + (v1normy * v2normy) + (v1normz * v2normz);

	float angle = acos(dot);

	return angle;
}

void Mixing(){

	cops = 0;

	for (int i = 0; i < pop; i++){
		
		if(parents[i] == 1){				//1st partner

			for (int j = 0; j < pop && j!=i; j++){
				
				if(parents[j] == 1){		//2nd partner

					copulate('+', i, j);	//average fit
					cops++;
				}
				else copulate('-', i, j);	//re-randomize unfit
			}
		}
	}
}

void copulate(char sign, int a, int b){

	for (int i = 0; i < 3*natoms; i++){

		//printf("(%f) = ((%f) + (%f))/2\n", coordz[a][i], coords[a][i], coords[b][i]);

		if (sign == '+'){
			//printf("%c", sign);
			coordz[a][i] = 0.5*(coords[a][i] + coords[b][i]);
		}
		else {coordz[a][i] = 0; 
			//printf("%c", sign);
		}
	}
}

void printbest(int n, char* file){

	FILE* f;

	f = fopen(file, "w");
		if (f==NULL) {printf("Cannot open %s\n", file); exit(1);}

		fprintf(f, "MOD-A\n3.511000\n\t2.121320   0.000000   0.000000\n\t0.000000   2.121320   0.000000\n\t0.000000   0.000000   2.121320\n\tC\n\t4\nSelective dynamics\nDirect\n");

		for (int i=0; i<3*natoms; i+=3){

			fprintf(f, "%f   %f   %f   T   T   T\n", coords[n][i], coords[n][i+1], coords[n][i+2]);
		}
	fclose(f);
}
