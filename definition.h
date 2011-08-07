// c++ header files
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <complex>
#include <vector>

//c header files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

//opencl header files
#include <CL/cl.h>
#include <clAmdFft.h>

//error handling header files
#include "errorhandle.h"

//Header file for timing from Intel Math Kernel Library
#include <omp.h>

//namespace
using namespace std;

//Macros
#define PI (3.141592653589793)
#define MAX_SOURCE_SIZE (0x100000)


//functions declaration
void generateDataFile(int,int);// The name of the generated data file would be data.txt
float** screate2darray(int,int);
float** sgenerate2darray(int,int);
float** sgenerate2darrayout(int,int,int);


//function definition
float** screate2darray(int x,int y)
{
        float ** point;
        float* p=(float*) malloc(sizeof(float)*x*y);
        point=(float**)malloc(sizeof(float*)*x);

        int i=0;
        for(i=0;i<x;i++){
                point[i]=p+i*y;
        }

        return point;
}
float** sgenerate2darray(int x,int y){
	srand((unsigned)time(0));
	float** arr=screate2darray(x,y);
	int i=0,j=0;

		for(i=0;i<x;i++){
			for(j=0;j<y;j++){
        			arr[i][j]=i+1.0;//(float)rand()/(float)RAND_MAX;
			}
	        	//printf("%lf ",arr[i]);
    		}//printf("\n");

	return arr;
}

float** sgenerate2darrayout(int x,int y,int channelno){

	srand((unsigned)time(0));
	float** arr=screate2darray(x,y);
	int i=0,j=0;
	x/=channelno;
	for(int l=0;l<channelno;l++){
		for(i=0;i<x;i++){
			for(j=0;j<y;j++){
			  arr[i+l*x][j]=i+1.0;//(float)rand()/(float)RAND_MAX;
			  //arr[i+l*x][j]=1.0/8192;//(float)rand()/(float)RAND_MAX;
			}
	        	//printf("%lf ",arr[i]);
    		}//printf("\n");
	}
	return arr;
}

void generateDataFile(int x,int y){// The name of the generated data file would be data.txt
	FILE *fp;

	fp=fopen("data.dat","w");

	srand((unsigned)time(0));
	float** arr=screate2darray(x,y);
        int i=0,j=0;
        for(i=0;i<x;i++){
                for(j=0;j<y;j++){
                        arr[i][j]=(float)rand()/(float)RAND_MAX;
                }
                //printf("%lf ",arr[i]);
        }//printf("\n");


	fwrite(arr,sizeof(float),x*y,fp);
	free(arr);
	fclose(fp);
}

float* screate1darray(int x)
{
	float* point=(float*)malloc(sizeof(float)*x);
	return point;
}

