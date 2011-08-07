#pragma OPENCL EXTENSION cl_amd_printf : enable

/*Constraints
--Each thread results in a block of size 8x4
*/

#define BLOCKSIZE 3 //In powers of 2
#define TILEX_SHIFT 3//In power of 2 since its 8x8
#define TILEY_SHIFT 3//In power of 2 since its 8x8
#define NO_INPUTS 7 //in powers of two, this is same as CHANSIZE

__constant sampler_t imageSampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;


__kernel void mmmKernel(__read_only image2d_t matrixA,__write_only image2d_t matrixC,uint widthA,uint channelSize)
{
	int x=get_global_id(0);

	channelSize=channelSize >> BLOCKSIZE;//Get the dimension after using 8x8
	int M=(channelSize*(channelSize+1)) >> 1;//no. of blocks in c above the diagonal

	int k=x/M;//Channel index for answer c
	x%=M;

	int j=floor((-1.0f+sqrt(1.0f+8*x))/2.0f);//Row index of 8x8
	int i=x-((j*(j+1)) >> 1);//Column index of 8x8
//	printf("%d %d %d %d\n",get_global_id(0),i,j,k);

	int xcol=i;//column number of 8x8
	int ypos=j;//row number of 8x8	

	int offsetk=k << NO_INPUTS;	
	//printf("%d\n",offsetk);
	float4 sum0 = (float4)(0);
	float4 sum1 = (float4)(0);
	float4 sum2 = (float4)(0);
	float4 sum3 = (float4)(0);
	float4 sum4 = (float4)(0);
	float4 sum5 = (float4)(0);
	float4 sum6 = (float4)(0);
	float4 sum7 = (float4)(0);
	
    for(int l = 0; l < widthA; l=l+4)
      {

	int li=l >> 2;
        float4 tempA0 = read_imagef(matrixA, imageSampler, (int2)(li, (ypos << TILEY_SHIFT) + offsetk));
        float4 tempA1 = read_imagef(matrixA, imageSampler, (int2)(li, (ypos << TILEY_SHIFT) + 1 + offsetk));
        float4 tempA2 = read_imagef(matrixA, imageSampler, (int2)(li, (ypos << TILEY_SHIFT) + 2 + offsetk));
        float4 tempA3 = read_imagef(matrixA, imageSampler, (int2)(li, (ypos << TILEY_SHIFT) + 3 + offsetk));
        float4 tempA4 = read_imagef(matrixA, imageSampler, (int2)(li, (ypos << TILEY_SHIFT) + 4 + offsetk));
        float4 tempA5 = read_imagef(matrixA, imageSampler, (int2)(li, (ypos << TILEY_SHIFT) + 5 + offsetk));
        float4 tempA6 = read_imagef(matrixA, imageSampler, (int2)(li, (ypos << TILEY_SHIFT) + 6 + offsetk));
        float4 tempA7 = read_imagef(matrixA, imageSampler, (int2)(li, (ypos << TILEY_SHIFT) + 7 + offsetk));

        float4 tempB0 = read_imagef(matrixA, imageSampler, (int2)(li,(xcol << TILEX_SHIFT) + offsetk));
        float4 tempB1 = read_imagef(matrixA, imageSampler, (int2)(li,(xcol << TILEX_SHIFT) + 1 + offsetk));
        float4 tempB2 = read_imagef(matrixA, imageSampler, (int2)(li,(xcol << TILEX_SHIFT) + 2 + offsetk));
        float4 tempB3 = read_imagef(matrixA, imageSampler, (int2)(li,(xcol << TILEX_SHIFT) + 3 + offsetk));
	
 
	sum0+=(float4)(dot(tempA0,tempB0)-dot(tempA1,tempB1),dot(tempA0,tempB1)+dot(tempA1,tempB0),dot(tempA0,tempB2)-dot(tempA1,tempB3),dot(tempA0,tempB3)+dot(tempA1,tempB2));
	sum1+=(float4)(dot(tempA2,tempB0)-dot(tempA3,tempB1),dot(tempA2,tempB1)+dot(tempA3,tempB0),dot(tempA2,tempB2)-dot(tempA3,tempB3),dot(tempA2,tempB3)+dot(tempA3,tempB2));
	sum2+=(float4)(dot(tempA4,tempB0)-dot(tempA5,tempB1),dot(tempA4,tempB1)+dot(tempA5,tempB0),dot(tempA4,tempB2)-dot(tempA5,tempB3),dot(tempA4,tempB3)+dot(tempA5,tempB2));
	sum3+=(float4)(dot(tempA6,tempB0)-dot(tempA7,tempB1),dot(tempA6,tempB1)+dot(tempA7,tempB0),dot(tempA6,tempB2)-dot(tempA7,tempB3),dot(tempA6,tempB3)+dot(tempA7,tempB2));
	
	tempB0 = read_imagef(matrixA, imageSampler, (int2)(li ,(xcol << TILEX_SHIFT) + 4 + offsetk));
        tempB1 = read_imagef(matrixA, imageSampler, (int2)(li ,(xcol << TILEX_SHIFT) + 5 + offsetk));
        tempB2 = read_imagef(matrixA, imageSampler, (int2)(li ,(xcol << TILEX_SHIFT) + 6 + offsetk));
        tempB3 = read_imagef(matrixA, imageSampler, (int2)(li ,(xcol << TILEX_SHIFT) + 7 + offsetk));

	sum4+=(float4)(dot(tempA0,tempB0)-dot(tempA1,tempB1),dot(tempA0,tempB1)+dot(tempA1,tempB0),dot(tempA0,tempB2)-dot(tempA1,tempB3),dot(tempA0,tempB3)+dot(tempA1,tempB2));
	sum5+=(float4)(dot(tempA2,tempB0)-dot(tempA3,tempB1),dot(tempA2,tempB1)+dot(tempA3,tempB0),dot(tempA2,tempB2)-dot(tempA3,tempB3),dot(tempA2,tempB3)+dot(tempA3,tempB2));
	sum6+=(float4)(dot(tempA4,tempB0)-dot(tempA5,tempB1),dot(tempA4,tempB1)+dot(tempA5,tempB0),dot(tempA4,tempB2)-dot(tempA5,tempB3),dot(tempA4,tempB3)+dot(tempA5,tempB2));
	sum7+=(float4)(dot(tempA6,tempB0)-dot(tempA7,tempB1),dot(tempA6,tempB1)+dot(tempA7,tempB0),dot(tempA6,tempB2)-dot(tempA7,tempB3),dot(tempA6,tempB3)+dot(tempA7,tempB2));


    }
//	offsetk=offsetk >> 1;
	xcol=xcol<<1;//dividing 8x8 into two blocks
	j=j << 2;//Creating an offset to jumo into next 8x8 block. But we have to jump  into next 4 rows since 2 rows of complex combine to get 1
	write_imagef(matrixC, (int2)(xcol, (j) + offsetk), sum0);
	write_imagef(matrixC, (int2)(xcol, (j) + 1 + offsetk), sum1);
	write_imagef(matrixC, (int2)(xcol, (j) + 2 + offsetk), sum2);
	write_imagef(matrixC, (int2)(xcol, (j) + 3 + offsetk), sum3);
	
	write_imagef(matrixC, (int2)(xcol+1, (j) + offsetk), sum4);
	write_imagef(matrixC, (int2)(xcol+1, (j) + 1 + offsetk), sum5);
	write_imagef(matrixC, (int2)(xcol+1, (j) + 2 + offsetk), sum6);
	write_imagef(matrixC, (int2)(xcol+1, (j) + 3 + offsetk), sum7);

}

