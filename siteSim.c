#include <stdio.h>
#include <stdlib.h>



float *powerAvail, *batLevel,*centerUse;

void timeStep(int time){
}

void main(int argc, char **argv) {
	float *powerAvail, *batLevel,*centerUse;
	powerAvail=(float*)malloc(10000*sizeof(float));
	batLevel=(float*)malloc(10000*sizeof(float));
	centerUse=(float*)malloc(10000*sizeof(float));

	fprintf(stdout, "Entering\n");
	FILE *inp=fopen("GLEAM-KW.txt","r");
	int i=1;
	powerAvail[0]=0;
	while (fscanf(inp,"%f",&powerAvail[i++]));
	for (int j=1; j<i;j++) {
		fprintf(stdout,"powerAvail[%d] = %f\n",j,powerAvail[j]);
		timeStep(j);
	}

}
