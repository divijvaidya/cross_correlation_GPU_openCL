#include <iostream>
using namespace std;

string prettyPrintclFFTStatus( const cl_int& );
cl_int OpenCL_V_Throw ( cl_int res, const string& msg, size_t lineno );

inline cl_int OpenCL_V_Throw ( cl_int res, const string& msg, size_t lineno )
{
        switch( res )
        {
                case    CL_SUCCESS:         //    **< No error 
                        break;
                default:
                {
                        stringstream tmp;
                        tmp << "OPENCL_V_THROWERROR< ";
                        tmp << prettyPrintclFFTStatus( res );
                        tmp << " > (";
                        tmp << lineno;
                        tmp << "): ";
                        tmp << msg;
                        string errorm (tmp.str());
                        cout << errorm<< std::endl;
                        throw   runtime_error( errorm );
                }
        }

        return  res;
}

#define OPENCL_V_THROW(_status,_message) OpenCL_V_Throw (_status, _message, __LINE__)

string prettyPrintclFFTStatus( const cl_int& status )
{
        switch( status )
        {
                case CLFFT_SUCCESS:
                        return "CLFFT_SUCCESS";
                case CLFFT_NOTIMPLEMENTED:
                        return "CLFFT_NOTIMPLEMENTED";
                case CLFFT_FILE_NOT_FOUND:
                        return "CLFFT_FILE_NOT_FOUND";
                case CLFFT_FILE_CREATE_FAILURE:
                        return "CLFFT_FILE_CREATE_FAILURE";
                case CLFFT_VERSION_MISMATCH:
                        return "CLFFT_VERSION_MISMATCH";
                case CLFFT_INVALID_PLAN:
                        return "CLFFT_INVALID_PLAN";
		case CL_DEVICE_NOT_FOUND:
			return	"CL_DEVICE_NOT_FOUND";
		case CL_DEVICE_NOT_AVAILABLE:
			return "CL_DEVICE_NOT_AVAILABLE";                     
		case CL_COMPILER_NOT_AVAILABLE:
			return "CL_COMPILER_NOT_AVAILABLE";                   
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:            
			return "CL_MEM_OBJECT_ALLOCATION_FAILURE";                  
		case CL_OUT_OF_RESOURCES:                         
			return "CL_OUT_OF_RESOURCES";                   
		case CL_OUT_OF_HOST_MEMORY:                       
			return "CL_OUT_OF_HOST_MEMORY";                   
		case CL_PROFILING_INFO_NOT_AVAILABLE:
			return "CL_PROFILING_INFO_NOT_AVAILABLE";                   
		case CL_MEM_COPY_OVERLAP:
			return "CL_MEM_COPY_OVERLAP";                   
		case CL_IMAGE_FORMAT_MISMATCH:
			return "CL_IMAGE_FORMAT_MISMATCH";                   
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:            
			return "CL_IMAGE_FORMAT_NOT_SUPPORTED";                   
		case CL_BUILD_PROGRAM_FAILURE:               
			return "CL_BUILD_PROGRAM_FAILURE";                   
		case CL_MAP_FAILURE:               
			return "CL_MAP_FAILURE";                   
		case CL_MISALIGNED_SUB_BUFFER_OFFSET:
			return "CL_MISALIGNED_SUB_BUFFER_OFFSET";                   
		case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
			return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";                   
		case CL_INVALID_VALUE:
			return "CL_INVALID_VALUE";                  
		case CL_INVALID_DEVICE_TYPE:                  
			return "CL_INVALID_DEVICE_TYPE";                   
		case CL_INVALID_PLATFORM:                        
			return "CL_INVALID_PLATFORM";                   
		case CL_INVALID_DEVICE:                          
			return "CL_INVALID_DEVICE";                   
		case CL_INVALID_CONTEXT:                         
			return "CL_INVALID_CONTEXT";                   
		case CL_INVALID_QUEUE_PROPERTIES:                
			return "CL_INVALID_QUEUE_PROPERTIES";                   
		case CL_INVALID_COMMAND_QUEUE:                   
			return "CL_INVALID_COMMAND_QUEUE";                  
		case CL_INVALID_HOST_PTR:                        
			return "CL_INVALID_HOST_PTR";                   
		case CL_INVALID_MEM_OBJECT:                      
			return "CL_INVALID_MEM_OBJECT";                   
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
			return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";                  
		case CL_INVALID_IMAGE_SIZE:                      
			return "CL_INVALID_IMAGE_SIZE";                   
		case CL_INVALID_SAMPLER:                         
			return "CL_INVALID_SAMPLER";                  
		case CL_INVALID_BINARY:                          
			return "CL_INVALID_BINARY";                   
		case CL_INVALID_BUILD_OPTIONS:                   
			return "CL_INVALID_BUILD_OPTIONS";                   
		case CL_INVALID_PROGRAM:                         
			return "CL_INVALID_PROGRAM";                   
		case CL_INVALID_PROGRAM_EXECUTABLE:              
			return "CL_INVALID_PROGRAM_EXECUTABLE";                   
		case CL_INVALID_KERNEL_NAME:                     
			return "CL_INVALID_KERNEL_NAME";                  
		case CL_INVALID_KERNEL_DEFINITION:               
			return "CL_INVALID_KERNEL_DEFINITION";                  
		case CL_INVALID_KERNEL:                          
			return "CL_INVALID_KERNEL";                   
		case CL_INVALID_ARG_INDEX:                       
			return "CL_INVALID_ARG_INDEX";                   
		case CL_INVALID_ARG_VALUE:                       
			return "CL_INVALID_ARG_VALUE";                   
		case CL_INVALID_ARG_SIZE:
			return "CL_INVALID_ARG_SIZE";                   
		case CL_INVALID_KERNEL_ARGS:                     
			return "CL_INVALID_KERNEL_ARGS";                   
		case CL_INVALID_WORK_DIMENSION:                  
			return "CL_INVALID_WORK_DIMENSION";                   
		case CL_INVALID_WORK_GROUP_SIZE:                 
			return "CL_INVALID_WORK_GROUP_SIZE";                  
		case CL_INVALID_WORK_ITEM_SIZE:                  
			return "CL_INVALID_WORK_ITEM_SIZE";                   
		case CL_INVALID_GLOBAL_OFFSET:                   
			return "CL_INVALID_GLOBAL_OFFSET";                  
		case CL_INVALID_EVENT_WAIT_LIST:                 
			return "CL_INVALID_EVENT_WAIT_LIST";                  
		case CL_INVALID_EVENT:                           
			return "CL_INVALID_EVENT";                
		case CL_INVALID_OPERATION:                       
			return "CL_INVALID_OPERATION";               
		case CL_INVALID_GL_OBJECT:                       
			return "CL_INVALID_GL_OBJECT";               
		case CL_INVALID_BUFFER_SIZE:                     
			return "CL_INVALID_BUFFER_SIZE";               
		case CL_INVALID_MIP_LEVEL:                       
			return "CL_INVALID_MIP_LEVEL";               
		case CL_INVALID_GLOBAL_WORK_SIZE:                
			return "CL_INVALID_GLOBAL_WORK_SIZE";               
		case CL_INVALID_PROPERTY:                        
                        return "CL_INVALID_PROPERTY";
                default:
                        return "Error code not defined";
                break;
        }
}

void prettyPrintPlatformInfo( const cl_platform_id& pId )
{
        size_t platformProfileSize      = 0;
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_PROFILE, 0, NULL, &platformProfileSize ),
                "Getting CL_PLATFORM_PROFILE Platform Info string size ( ::clGetPlatformInfo() )" );

        std::vector< char > szPlatformProfile( platformProfileSize );
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_PROFILE, platformProfileSize, &szPlatformProfile[ 0 ], NULL),
                "Getting CL_PLATFORM_PROFILE Platform Info string ( ::clGetPlatformInfo() )" );

        size_t platformVersionSize      = 0;
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_VERSION, 0, NULL, &platformVersionSize ),
                "Getting CL_PLATFORM_VERSION Platform Info string size ( ::clGetPlatformInfo() )" );

        std::vector< char > szPlatformVersion( platformVersionSize );
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_VERSION, platformVersionSize, &szPlatformVersion[ 0 ], NULL),
                "Getting CL_PLATFORM_VERSION Platform Info string ( ::clGetPlatformInfo() )" );

        size_t platformNameSize = 0;
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_NAME, 0, NULL, &platformNameSize ),
                "Getting CL_PLATFORM_NAME Platform Info string size ( ::clGetPlatformInfo() )" );

        std::vector< char > szPlatformName( platformNameSize );
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_NAME, platformNameSize, &szPlatformName[ 0 ], NULL),
                "Getting CL_PLATFORM_NAME Platform Info string ( ::clGetPlatformInfo() )" );

        size_t vendorStringSize = 0;
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_VENDOR, 0, NULL, &vendorStringSize ),
                "Getting CL_PLATFORM_VENDOR Platform Info string size ( ::clGetPlatformInfo() )" );

        std::vector< char > szPlatformVendor( vendorStringSize );
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_VENDOR, vendorStringSize, &szPlatformVendor[ 0 ], NULL),
                "Getting CL_PLATFORM_VENDOR Platform Info string ( ::clGetPlatformInfo() )" );

        size_t platformExtensionsSize   = 0;
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_EXTENSIONS, 0, NULL, &platformExtensionsSize ),
                "Getting CL_PLATFORM_EXTENSIONS Platform Info string size ( ::clGetPlatformInfo() )" );

        std::vector< char > szPlatformExtensions( platformExtensionsSize );
        OPENCL_V_THROW( ::clGetPlatformInfo( pId, CL_PLATFORM_EXTENSIONS, platformExtensionsSize, &szPlatformExtensions[ 0 ], NULL),
                "Getting CL_PLATFORM_EXTENSIONS Platform Info string ( ::clGetPlatformInfo() )" );

        const int indent =0;// countOf( "    CL_PLATFORM_EXTENSIONS: " );
        std::cout << std::left << std::setw( indent ) << "    CL_PLATFORM_PROFILE: " << &szPlatformProfile[ 0 ] << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_PLATFORM_VERSION: " << &szPlatformVersion[ 0 ] << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_PLATFORM_NAME: " << &szPlatformName[ 0 ] << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_PLATFORM_VENDOR: " << &szPlatformVendor[ 0 ] << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_PLATFORM_EXTENSIONS: " << &szPlatformExtensions[ 0 ] << std::endl;
        std::cout << std::endl;
}

void prettyPrintDeviceInfo( const cl_device_id& dId )
{
        size_t deviceNameSize   = 0;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_NAME, 0, NULL, &deviceNameSize ),
                "Getting CL_DEVICE_NAME Platform Info string size ( ::clGetDeviceInfo() )" );

        std::vector< char > szDeviceName( deviceNameSize );
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_NAME, deviceNameSize, &szDeviceName[ 0 ], NULL ),
                "Getting CL_DEVICE_NAME Platform Info string ( ::clGetDeviceInfo() )" );

        cl_device_type devType = CL_DEVICE_TYPE_DEFAULT;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_TYPE, sizeof( cl_device_type ), &devType, NULL ),
                "Getting CL_DEVICE_TYPE device info ( ::clGetDeviceInfo() )" );

        cl_uint devAddrBits = 0;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_ADDRESS_BITS, sizeof( cl_uint ), &devAddrBits, NULL ),
                "Getting CL_DEVICE_ADDRESS_BITS device info ( ::clGetDeviceInfo() )" );

        cl_bool devAvailable = CL_FALSE;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_AVAILABLE, sizeof( cl_bool ), &devAvailable, NULL ),
                "Getting CL_DEVICE_AVAILABLE device info ( ::clGetDeviceInfo() )" );

        cl_bool devCompAvailable = CL_FALSE;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_COMPILER_AVAILABLE, sizeof( cl_bool ), &devCompAvailable, NULL ),
                "Getting CL_DEVICE_COMPILER_AVAILABLE device info ( ::clGetDeviceInfo() )" );

        size_t devMaxWorkGroup  = 0;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof( size_t ), &devMaxWorkGroup, NULL ),
                "Getting CL_DEVICE_MAX_WORK_GROUP_SIZE devoce info ( ::clGetDeviceInfo() )" );

        cl_uint devMaxWorkItemDim = CL_FALSE;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof( cl_uint ), &devMaxWorkItemDim, NULL ),
                "Getting CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS device info ( ::clGetDeviceInfo() )" );

        std::vector< size_t >   devMaxWorkItemSizes( devMaxWorkItemDim );
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof( size_t )*devMaxWorkItemSizes.size( ), &devMaxWorkItemSizes[0], NULL),
                "Getting CL_DEVICE_MAX_WORK_ITEM_SIZES device info ( ::clGetDeviceInfo() )" );

        cl_ulong devMaxConstantBuffer   = 0;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof( cl_ulong ), &devMaxConstantBuffer, NULL ),
                "Getting CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE devoce info ( ::clGetDeviceInfo() )" );

        cl_ulong devLocalMemSize        = 0;
        OPENCL_V_THROW( ::clGetDeviceInfo( dId, CL_DEVICE_LOCAL_MEM_SIZE, sizeof( cl_ulong ), &devLocalMemSize, NULL ),
                "Getting CL_DEVICE_LOCAL_MEM_SIZE devoce info ( ::clGetDeviceInfo() )" );


        const int indent = 0;//countOf( "    CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: " );
        std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_NAME: " << &szDeviceName[ 0 ] << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_TYPE: "
                << (CL_DEVICE_TYPE_DEFAULT     & devType ? "default"     : "")
                << (CL_DEVICE_TYPE_CPU         & devType ? "CPU"         : "")
                << (CL_DEVICE_TYPE_GPU         & devType ? "GPU"         : "")
                << (CL_DEVICE_TYPE_ACCELERATOR & devType ? "Accelerator" : "")
                << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_ADDRESS_BITS: " << devAddrBits << std::endl;
	std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_AVAILABLE: " << ( devAvailable ? "TRUE": "FALSE") << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_COMPILER_AVAILABLE: " << ( devCompAvailable ? "TRUE": "FALSE") << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_MAX_WORK_GROUP_SIZE: " << devMaxWorkGroup << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: " << devMaxWorkItemDim << std::endl;
        for( cl_uint wis = 0; wis < devMaxWorkItemSizes.size( ); ++wis )
        {
                std::stringstream dimString;
                dimString << "Dimension[ " << wis << " ]  ";
                std::cout << std::right << std::setw( indent ) << dimString.str( ) << devMaxWorkItemSizes[wis] << std::endl;
        }
        std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE: " << devMaxConstantBuffer << std::endl;
        std::cout << std::left << std::setw( indent ) << "    CL_DEVICE_LOCAL_MEM_SIZE: " << devLocalMemSize << std::endl;

        std::cout << std::endl;
}

