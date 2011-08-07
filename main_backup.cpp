/*
 * Author: Divij Vaidya
 * Contact: divijvaidya13@gmail.com
 * Date: 24-July-2011
**/

#include "definition.h"

#define NO_THREAD_PER_BLOCK 256 // Maximum value depends on the hardware used
#define MEM_SIZE 16384 //Maximum can be 16384 since 8192 is the maximum an image buffer can store and half of this value is stored in image buffer
#define ITER_GEMM 1
#define ITER_FFT 1
#define NO_INPUTS 4096 //Maximum can be 4096 since 8192 is the maximum an image buffer can store and double of this value is stored in image buffer
#define CHANSIZE 128 //Number of inputs per channel // Change this in kernel also name NO_INPUTS
#define CHANNELNO (NO_INPUTS/CHANSIZE) //Numbers of channel
#define NO_SAMPLES 16384
int main()
{
        //Control Variables
        bool showStartInput=false;// Setting it to true shows the original Input
        bool showFftOutput=false;// Shows the output after the FFT but before the Reshuffle
        bool showReshuffleOutput=false;// Shows the output after the reshuffle
        bool showFinalResult=false; // Shows final result after cross-correlation
        bool showGemmInput=false; // Shows output after the reshuffle but before the matrix multiplication
	bool showReformatOutputAfterReshuffle=false; // Shows output after it has been reformatted after the reshuffling

	//openCL State
        cl_platform_id platform_id=NULL;
	cl_device_id device_id=NULL;
        cl_context context=NULL;
        cl_command_queue queue=NULL;
        cl_program program=NULL;
        cl_kernel kernel=NULL;
        cl_uint ret_num_devices;
        cl_uint ret_num_platforms;
        cl_int ret=0; // Stores the error values retuned by many functions
	cl_event event = NULL;
        cl_event events[10];
	cl_kernel clKernel;

	//FFT state
	clAmdFftPlanHandle plHandle;
        clAmdFftResultLocation place = CLFFT_OUTOFPLACE; //Alternative CLFFT_INPLACE
        clAmdFftLayout inLayout  = CLFFT_COMPLEX_INTERLEAVED;
        clAmdFftLayout outLayout = CLFFT_COMPLEX_INTERLEAVED;
        clAmdFftDim dim = CLFFT_1D;
	size_t clStrides[3]={0,0,0};
        size_t clLengths[3];
	clLengths[0]=(MEM_SIZE/2);//Length of first dimension of fft
	clLengths[1]=1;//length of second dimension of fft
	clLengths[2]=1;
	clStrides[ 0 ] = 1;
        clStrides[ 1 ] = clStrides[ 0 ] * clLengths[ 0 ];
        clStrides[ 2 ] = clStrides[ 1 ] * clLengths[ 1 ];
        clStrides[ 3 ] = clStrides[ 2 ] * clLengths[ 2 ];
	size_t batchSize=CHANSIZE;//number of discreet fft's to be calculated simultaneously


	//Initialise openCL
	OPENCL_V_THROW(clGetPlatformIDs(1, &platform_id, &ret_num_platforms),"clGetPlatformIDs Failed");
        OPENCL_V_THROW(clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id,&ret_num_devices),"clGetDeviceIDs Failed");
        context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
        OPENCL_V_THROW(ret, "Creating Context failed" );
        queue = clCreateCommandQueue(context, device_id, 0, &ret);
        OPENCL_V_THROW(ret, "Creating command queue failed" );


//===========Initialise the host buffers======================================

/*
 * The functions sgenerate2darray(), screate2darray() and sgenerate2darrayout() are defined and declared in definition.h
 */

	float** src_a_h=sgenerate2darray(NO_INPUTS,MEM_SIZE);//To be used to store the original input

	float** answer=screate2darray(NO_INPUTS,MEM_SIZE);//To be used to store the answer after the reshuffling

        float** corr_h=sgenerate2darrayout(NO_INPUTS,CHANSIZE << 1,CHANNELNO);// To be used to store the final answer

        if(showStartInput){
            cout << "Initial Input Buffer" << "\n";
            for(int j=0;j<NO_INPUTS;j++){
                for(int i=0;i<MEM_SIZE;i++){
                        cout << src_a_h[j][i] << " ";
                }cout << "\n";
            }printf("\n");
        }


//===================================================================
        //Calculation of facs for reshuffling
	complex <float>* facs_h=(complex <float>*) malloc(sizeof(complex <float>)*(MEM_SIZE/2));
	complex<float> I=1.0i;
        complex <float> xx=2.0*PI;
        for(int i=0;i<MEM_SIZE/2;i++){
                facs_h[i]=(1.0*i)/(1.0*MEM_SIZE);
                facs_h[i]=exp(xx*(-I*facs_h[i]));
        }
//===================================================================
	//Initialise GPU memory buffers
	size_t sizeofgpumem=NO_INPUTS*MEM_SIZE*sizeof(float);
	size_t sizeoffacsmem=MEM_SIZE*sizeof(float);
	cl_mem clMemBuffersIn = clCreateBuffer(context,CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,sizeofgpumem,src_a_h[0],&ret);
	OPENCL_V_THROW( ret, "Creating clMemBuffersIn Buffer failed" );

	cl_mem clMemBuffersOut = clCreateBuffer(context,CL_MEM_READ_WRITE,sizeofgpumem,NULL,&ret);
	OPENCL_V_THROW (ret, "Creating fft output Buffer failed");

	cl_mem facs = clCreateBuffer(context,CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,sizeoffacsmem,facs_h,&ret);
	OPENCL_V_THROW (ret, "Creating facs Buffer failed");

	//===========================Starting the fft=============================//
	clAmdFftSetupData setupData;
	OPENCL_V_THROW( clAmdFftInitSetupData( &setupData ),"clAmdFftInitSetupData failed" );
        OPENCL_V_THROW( clAmdFftSetup( &setupData ), "clAmdFftSetup failed" );
	OPENCL_V_THROW( clAmdFftCreateDefaultPlan( &plHandle, context, dim, clLengths ), "clAmdFftCreateDefaultPlan failed" );
	OPENCL_V_THROW (clAmdFftSetPlanBatchSize (plHandle, batchSize),"Setting BatchSize Failed");
        OPENCL_V_THROW (clAmdFftSetResultLocation( plHandle, place ), "clAmdFftSetResultLocation failed" );
	OPENCL_V_THROW (clAmdFftSetPlanInStride  ( plHandle, dim, clStrides ), "clAmdFftSetPlanInStride failed" );
        OPENCL_V_THROW (clAmdFftSetPlanOutStride ( plHandle, dim, clStrides ), "clAmdFftSetPlanOutStride failed" );
        OPENCL_V_THROW (clAmdFftSetPlanDistance  ( plHandle, clStrides[ dim ], clStrides[ dim ]), "clAmdFftSetPlanDistance failed" );

	OPENCL_V_THROW( clAmdFftBakePlan( plHandle, 1, &queue, NULL, NULL ), "clAmdFftBakePlan failed" );

	size_t tempbuffersize=0;
        OPENCL_V_THROW( clAmdFftGetTmpBufSize(plHandle, &tempbuffersize ), "clAmdFftGetTmpBufSize failed" );

	//allocate the intermediate buffer
        cl_mem clMedBuffer=NULL;
        if (tempbuffersize)
        {
                cl_int medstatus;
                clMedBuffer = clCreateBuffer ( context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR,tempbuffersize, 0, &medstatus);
                OPENCL_V_THROW( medstatus, "Creating fft intermediate Buffer failed" );
        }
	if (( place == CLFFT_INPLACE )&& ( inLayout != outLayout )) {
                switch( inLayout )
                {
                case CLFFT_COMPLEX_INTERLEAVED:
                        {
                                assert (CLFFT_COMPLEX_PLANAR == outLayout);
                                throw std::runtime_error( "Cannot use the same buffer for interleaved->planar in-place transforms" );
                                break;
                        }
                case CLFFT_COMPLEX_PLANAR:
                        {
                                assert (CLFFT_COMPLEX_INTERLEAVED == outLayout);
                                throw std::runtime_error( "Cannot use the same buffer for planar->interleaved in-place transforms" );
                                break;
                        }
                }
        }
	cl_mem * BuffersOut = ( place == CLFFT_INPLACE ) ? NULL :  &clMemBuffersOut;

	//========Timimg fft============//

	double time_fft_start=omp_get_wtime();

	for(int i=0;i<ITER_FFT;i++){
		OPENCL_V_THROW( clAmdFftEnqueueTransform( plHandle, CLFFT_FORWARD, 1,&queue,0,NULL,&event,&clMemBuffersIn,BuffersOut,clMedBuffer ),"clAmdFftEnqueueTransform failed" );
	}
	ret=clWaitForEvents(1,&event);

        double time_fft_end=omp_get_wtime();

	//Cleaning up fft
        OPENCL_V_THROW( clAmdFftDestroyPlan( &plHandle ), "clAmdFftDestroyPlan failed" );
        OPENCL_V_THROW( clAmdFftTeardown( ), "clAmdFftTeardown failed" );

        //displaying results
        if(showFftOutput){
            OPENCL_V_THROW( clEnqueueReadBuffer( queue, clMemBuffersOut, CL_TRUE, 0, sizeofgpumem,answer [0], 0, NULL, NULL ),"Reading the result buffer failed" );

            cout << "**FFT Output**" << endl;
            for(int j=0;j<NO_INPUTS;j++){
                    for(int i=0;i<MEM_SIZE;i++){
                            cout << answer[j][i] << " ";
                    } printf("\n");
            }printf("\n");
        }
	//==================End of FFT=============================================//

	//==================Start the Reshuffling==================================//
	FILE *fp;
	char fileName[]="./reshuffle.cl";
	char* source_str=NULL;
	size_t source_size;

	//Load the source code containing the kernel/
 	fp = fopen(fileName, "r");
 	if (!fp) {
 		fprintf(stderr, "Failed to load reshuffle kernel.¥n");
	 	exit(1);
 	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	//Preparation for building the Kernel
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size, &ret);
        OPENCL_V_THROW( ret, "Creating program with source failed for Reshuffle" );
	OPENCL_V_THROW( clBuildProgram(program, 1, &device_id, NULL, NULL, NULL),"Build Program Failed for Reshuffle");
	kernel = clCreateKernel(program, "reshuffle", &ret);
	OPENCL_V_THROW( ret, "Creating kernel failed for Reshuffle" );

	//Set kernel parameters
	const int num=NO_INPUTS*MEM_SIZE;
	const int block=MEM_SIZE;
	OPENCL_V_THROW(clSetKernelArg(kernel, 0, sizeof(cl_mem), (float *)&clMemBuffersIn),"Passing argument 0 of reshuffle failed");
 	OPENCL_V_THROW(clSetKernelArg(kernel, 1, sizeof(cl_mem), (float *)&facs),"Passing arg 1 of reshuffle failed");
        OPENCL_V_THROW(clSetKernelArg(kernel, 2, sizeof(cl_mem), (float *)&clMemBuffersOut),"Passing arg2 of reshuffle failed");
 	OPENCL_V_THROW(clSetKernelArg(kernel, 3, sizeof(int), (int *)&num),"Passing arg3 of reshuffle failed");
        OPENCL_V_THROW(clSetKernelArg(kernel, 4, sizeof(int), (int *)&block),"Passing arg4 of reshuffle failed");


	// Execute OpenCL Kernel //
	const size_t local_ws=NO_INPUTS*MEM_SIZE;
	const size_t global_ws=min(NO_THREAD_PER_BLOCK,MEM_SIZE);//ceil(MEM_SIZE/local_ws);

	//===========timing the reshuffle===============//
	double time_reshuffle_start=omp_get_wtime();
	for(int i=0;i<ITER_FFT;i++){
		OPENCL_V_THROW(clEnqueueNDRangeKernel(queue,kernel, 1, NULL,&local_ws,&global_ws, 0, NULL, NULL),"Reshuffle Kernel execution failed");
	}
	double time_reshuffle_end=omp_get_wtime();

	//Read back data
	OPENCL_V_THROW(clEnqueueReadBuffer(queue, clMemBuffersOut, CL_TRUE, 0, sizeofgpumem,answer[0], 0, NULL, NULL),"Reading back reshuffled data failed");

	//====================Finish the reshuffling================================//

        if(showReshuffleOutput){
                cout << "Output after reshuffling" << endl;
                for(int j=0;j<NO_INPUTS;j++){
                        for(int i=0;i<MEM_SIZE;i++){
                                cout << answer[j][i] << " ";
                        } printf("\n");
                }printf("\n");
        }
        //=================Reformatting the input given to the matrix multiply===================================//
        float** answer_final=screate2darray(NO_INPUTS*2,MEM_SIZE/2);
        for(int i=0;i<NO_INPUTS;i++){
            for(int j=0;j<MEM_SIZE;j++){
                if(j&1)
                        answer_final[(i<<1)+1][j >> 1]=answer[i][j];
                else
                        answer_final[(i<<1)][j >> 1]=answer[i][j];
            }
        }

	if(showReformatOutputAfterReshuffle){
        	for(int i=0;i<NO_INPUTS*2;i++){
        	    for(int j=0;j<MEM_SIZE/2;j++){
               		 cout << answer_final[i][j] << " ";
	            }cout << "\n";
        	}
	}

        //===================Clean up after reshuffling=============================//
        OPENCL_V_THROW(clReleaseMemObject(clMemBuffersIn),"Error in releasing clMembufferin");
	OPENCL_V_THROW(clReleaseMemObject(clMemBuffersOut),"Error in releasing clMembufferout");
        OPENCL_V_THROW(clReleaseMemObject(facs),"Error in releasing facs");
        //====================Starting sgemm========================================//
	//Initialise GPU memory buffers
	cl_image_format imageFormat;
     	imageFormat.image_channel_data_type = CL_FLOAT;
	imageFormat.image_channel_order = CL_RGBA;

	sizeofgpumem=(NO_INPUTS << 1)*(MEM_SIZE >> 1)*sizeof(float);
	cl_mem clMemBuffersIn_image=clCreateImage2D(context,CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,&imageFormat,(MEM_SIZE >> 3),(NO_INPUTS << 1),0,answer_final[0],&ret);
	OPENCL_V_THROW( ret, "Creating clMemBuffersIn Buffer failed" );

	if(showGemmInput){
                printf("\n");
                float** chk_inp1=screate2darray((NO_INPUTS<< 1),(MEM_SIZE >> 1));
		size_t origin[] = {0, 0, 0};
	        size_t region[] = {(MEM_SIZE >> 3), (NO_INPUTS << 1), 1};
        	OPENCL_V_THROW(clEnqueueReadImage(queue,clMemBuffersIn_image,1,origin,region,0,0,chk_inp1[0],0,0,0),"Reading image error 1");
		for(int i=0;i<(NO_INPUTS << 1);i++){
			for(int j=0;j<(MEM_SIZE >> 1);j++){
				cout << chk_inp1[i][j] << " ";
			}cout << endl;
		}
		cout << endl;
	}

	cl_mem clMemBuffersOut_image=clCreateImage2D(context,CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,&imageFormat,(CHANSIZE >> 1),(NO_INPUTS),0,corr_h[0],&ret);
	OPENCL_V_THROW (ret, "Creating output Buffer failed");


	int M1=CHANNELNO*(((NO_INPUTS<<1)/(CHANNELNO*8))*(((NO_INPUTS<<1)/(CHANNELNO*8))+1))/2;
	size_t globalThreads[1]= {M1};
	size_t localThreads[1] = {NO_THREAD_PER_BLOCK};

	FILE *fp1;
        char fileName_gemm[]="./matrixkernel1.cl";

        source_str=NULL;

        //Load the source code containing the kernel/
        fp1 = fopen(fileName_gemm, "r");
        if (!fp1) {
                fprintf(stderr, "Failed to load gemm kernel.\n");
                exit(1);
        }
        source_str = (char*)malloc(MAX_SOURCE_SIZE);
        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp1);
        fclose(fp1);

        //Preparation for building the Kernel
        program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size, &ret);
        OPENCL_V_THROW( ret, "Creating program with source failed" );
        ret=clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

        size_t log_size;
        // First call to know the proper size
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* build_log=(char*)malloc(sizeof(char)*log_size+1);
        // Second call to get the log
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, (void*)build_log, NULL);
        build_log[log_size] = '\0';
        printf("errors: %s\n",build_log);
        free(build_log);

        clKernel = clCreateKernel(program, "mmmKernel", &ret);

        OPENCL_V_THROW( ret, "Creating kernel failed" );

	int temp_mem_size=MEM_SIZE >> 1;
	int temp_imputs_size=(NO_INPUTS << 1)/CHANNELNO;
	OPENCL_V_THROW(clSetKernelArg(clKernel, 0, sizeof(cl_mem), (void *)&clMemBuffersIn_image),"Setting Arg 0 of matrix multiply failed");
	OPENCL_V_THROW(clSetKernelArg(clKernel, 1, sizeof(cl_mem), (void *)&clMemBuffersOut_image),"Setting Arg 2 of matrix multiply failed");
	OPENCL_V_THROW(clSetKernelArg(clKernel, 2, sizeof(cl_int), (void *)&temp_mem_size),"Setting Arg 3 of matrix multiply failed");
	OPENCL_V_THROW(clSetKernelArg(clKernel, 3, sizeof(cl_int), &temp_imputs_size),"Setting Arg 4 of matrix multiply failed");


	double t1_gemm=omp_get_wtime();
	for (int iter=0;iter<ITER_GEMM;iter++) {
	  OPENCL_V_THROW(clEnqueueNDRangeKernel(queue,clKernel,1,NULL,globalThreads,localThreads,0,NULL,&events[0]),"Kernel failed");
	}

	  OPENCL_V_THROW(clWaitForEvents(1, &events[0]),"clWaitforEvents failed");

	double t2_gemm=omp_get_wtime();


        size_t origin_out_image[] = {0, 0, 0};
	size_t region_out_image[] = {(CHANSIZE >> 1), (NO_INPUTS), 1};
	OPENCL_V_THROW(clEnqueueReadImage(queue,clMemBuffersOut_image,1,origin_out_image,region_out_image,0,0,corr_h[0],0,0,0),"Reading image error");

	//==================END of SGEMM========================================//


        if(showFinalResult){
                cout << "Final Output" << endl;
                for(int i=0;i<(NO_INPUTS);i++){
                        for(int j=0;j<(CHANSIZE << 1);j++){
                                cout << corr_h[i][j] << " ";
                        }cout << endl;
                }printf("\n");
        }

	printf("Time for FFT only: %lf\n",time_fft_end-time_fft_start);
	printf("Time for reshuffle: %lf\n",time_reshuffle_end-time_reshuffle_start);
        printf("Time for matrix multiplication: %lf\n",t2_gemm-t1_gemm);
        double nflops=(double)(ITER_GEMM*(CHANNELNO*(CHANSIZE*CHANSIZE*MEM_SIZE)*10e-9));
        printf("GFlops for matrix multiplication: %lf\n",nflops/(t2_gemm-t1_gemm));


        //=================cleaning up openCL===========================//
	ret = clFlush(queue);
        OPENCL_V_THROW( ret, "Problem Flushing the queue" );
        ret=clFinish(queue);
        OPENCL_V_THROW( ret, "Problem Finishing the queue" );
	OPENCL_V_THROW(clReleaseMemObject(clMemBuffersIn_image),"Error in releasing clMembufferin image");
	OPENCL_V_THROW(clReleaseMemObject(clMemBuffersOut_image),"Error in releasing clMembufferout image");
        ret = clReleaseCommandQueue(queue);
        OPENCL_V_THROW( ret, "Problem Releasing the command queue" );
	ret = clReleaseContext(context);
        OPENCL_V_THROW( ret, "Problem releasing the context" );

	free(answer);
	free(src_a_h);
        free(corr_h);
}
