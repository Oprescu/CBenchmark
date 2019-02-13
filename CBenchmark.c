#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h> 

void main(int argc, char **argv){
	
	if(argc<2){
		printf("Incorrect usage : ./CPUBenchmark <NrOfThreads>\n");
		return;
	}
	
	int THREADS= strtol(argv[1], NULL, 10);
	long double number=2305843008139952128; //number to check
	long double total=0;
	int pipe_fd[2];			//pipe for communication between forks
	pipe(pipe_fd);

	printf("\nCrunching using %d threads\n\n", THREADS);	

	//Forks for as many threads needed
	//Check if number is perfect number
	for(long double x=0;x<THREADS;x++){
		if(fork()==0){
			if(x==0) total++; //If first thread, add one for first divisor
			for(long double i=x+2;i<=sqrt(number);i+=THREADS){	//Iterates jumping by THREADS amount of i's
				if(fmod(number,i)==0){
				total+=i;
				total+=number/i;
			}
		}
		//Flush total through pipe to parent
		write(pipe_fd[1],&total,sizeof(total));
		//Close pipe descriptors
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		exit(0);
		}	
	}
	
	//Read all pipes from children
	//into buffer
	long double buffer;
	for(int i=0;i<THREADS;i++){
		read(pipe_fd[0],&buffer,sizeof(buffer));
		total+=buffer; //Add buffer to total
	}
	//Close pipe descriptors
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	if(total==number) //it is a perfect number
		printf("Benchmark done\n");
}
