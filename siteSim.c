#include <stdio.h>
#include <stdlib.h>
#include<assert.h>



float *powerAvail, // powerAvail is the amount of power available from the turbines and other sources, read in from a file.
      *batLevel,	// The amount of charge in the battery
      *centerUse;	// How much power the center was given at time t
// These constants will be turned into command line parameters later
const float battery=1000;	// The max capacity in KWH of the battery
const float load=500;		// The amount of power consumed by the data center when running flat out
const float mCR=50;		// The maximum charge rate of the battery system.
const float mDR=50;		// The maximum rate that power can be extracted from the battery.

float min(float x, float y) {
	if (x>y) return y;
	return x;
}

void timeStep(int time){
	float pa=powerAvail[time];
	if (pa >= load) {
		// Extra power, lets load up the battery
		centerUse[time]=load;	// full power 
		float lo=pa-load;	// lo stands for left over power
		if (lo>mCR) lo=mCR;	// can we use all of that left over power?
		batLevel[time]=min(batLevel[time-1]+lo,battery); // update battery power
	} 
	else {
		// Not enough power - check the batteries
		float sp=load-pa;	// sp stands for short power .. how much do we need to run at full power
		float mfb=min(mDR,batLevel[time-1]); 	// mfb stands for max from battery - how much can we get out this time
		if (mfb>sp) {
			// We can make up the power shortage from the battery
			batLevel[time]=batLevel[time-1]-sp; 	// update batLevel to reflect what we are taking from the battery
			centerUse[time]=load;
		}
		else {
			// power available is less than we need
			batLevel[time]=batLevel[time-1]-mfb; 	// update batLevel to reflect what we are taking from the battery
			centerUse[time]=pa+mfb;
		}

	}
	assert(batLevel[time]>=0);
}

void main(int argc, char **argv) {
	powerAvail=(float*)malloc(10000*sizeof(float));
	batLevel=(float*)malloc(10000*sizeof(float));
	centerUse=(float*)malloc(10000*sizeof(float));

	fprintf(stdout, "Entering\n");
	FILE *inp=fopen("GLEAM-KW.txt","r");
	int i=1;
	int zeroPower=0;
	float availAccumulator=0.0;
	powerAvail[0]=0;
	batLevel[0]=1000;
	while (fscanf(inp,"%f",&powerAvail[i++]));
	int j=1;
	while (j<i) {
		timeStep(j);
		fprintf(stdout,"Time %d:  powerAvail = %f,\tcenterUse = %f,\tbatLevl = %f\n",j,powerAvail[j],centerUse[j],batLevel[j]);
		if (centerUse[j]<1.0) zeroPower++;
		availAccumulator+=centerUse[j]/load;
		j++;
	}
	fprintf(stdout,"Average Availability is %f, time periods where center had no power is %d\n", availAccumulator/i,zeroPower);

}
