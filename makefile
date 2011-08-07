AMDSDKROOT=/opt/AMD-APP-SDK-v2.4-lnx64
APPMLFFTROOT =/opt/clAmdFft-1.0.53
INCLUDE   = -I $(AMDSDKROOT)/include -I $(APPMLFFTROOT)/include 


all:
	g++ ${INCLUDE} -L${AMDSDKROOT}/lib/x86_64  -L ${APPMLFFTROOT}/lib64 -o main main.cpp -Wl,-rpath,/opt/AMD-APP-SDK-v2.4-lnx64/lib/x86_64 -lOpenCL -lm -lclAmdFft.Runtime -fopenmp

clean:
	rm -f *.o main
