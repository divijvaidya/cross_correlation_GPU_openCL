#pragma OPENCL EXTENSION cl_amd_printf : enable

__kernel void reshuffle(__global const float *a, __global const float *ff,__global float *ans,const int num,const int MEM_SIZE)
{
	const int idx=get_global_id(0);
	const int idy=idx/MEM_SIZE;
	//const int idy=get_group_id(0);
	//printf("Check:%d %d\n",idx,idy);

	
	if(idx%(MEM_SIZE)==0){
                ans[idx]=a[idx]+a[idx+1];
                ans[idx+1]=0.0f;
        }else if(idx<num && idx%2==0 ){
		
		//printf("%d %d %d %d\n",idx,(MEM_SIZE)*(2*idy+1)-idx,idx+1,0);
                ans[idx]=0.5f*(a[idx]+a[(MEM_SIZE)*(2*idy+1)-idx]+(((a[idx+1]+a[(MEM_SIZE)*(2*idy+1)-idx+1])*ff[(idx%(MEM_SIZE))])+((a[idx]-a[(MEM_SIZE)*(2*idy+1)-idx])*ff[(idx%(MEM_SIZE))+1])));
                ans[idx+1]= 0.5f*(a[idx+1]-a[(MEM_SIZE)*(2*idy+1)-idx+1]-(((a[idx]-a[(MEM_SIZE)*(2*idy+1)-idx])*ff[(idx%(MEM_SIZE))])-(ff[(idx%MEM_SIZE)+1]*(a[idx+1]+a[(MEM_SIZE)*(2*idy+1)-idx+1]))));
                //printf("%f %f\n",ans[idx],ans[idx+1]);
        }
		
}
