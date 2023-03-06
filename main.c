#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[]) {
	double clockstart, clockend;

	//read args
	if (argc != 3 && argc != 4)
	{
		// if we don't get the right number, get mad!
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
		return 0;
	} else if (strcmp(argv[1], "S") == 0)
	{
		if (argc == 4) {
			//we got too many args for this mode
			printf("too many arguments\n");
			printf("Usage: ./mmm <mode> [num threads] <size>\n");
		}
		//we are doing sequential, set size
		int size;
		sscanf(argv[2], "%d", &size);
		//print header, used multiple lines so it doesnt look like a mess
		printf("========\n");
		printf("mode: sequential\n");
		printf("thread count: 1\n");
		printf("size: %i\n", size);
		printf("========\n");
		//warm it up
		mmm_init(1,size);
		mmm_seq();
		mmm_freeup();
		//now we go through and calculate time
		double total_time = 0;
		for (int i = 0; i < 3; i++)
		{
			mmm_init(1,size);
			clockstart = rtclock();
			mmm_seq();
			clockend = rtclock();
			total_time += clockend-clockstart;
			mmm_freeup();
		}
		total_time = total_time/3;
		//print it
		printf("Sequential Time (avg of 3 runs): %.6f\n", total_time);
		return 0;
	} else if (strcmp(argv[1], "P") == 0)
	{
		//we are doing paralell, first find num threads and size
		int num_threads;
		sscanf(argv[2], "%d", &num_threads);
		int size;
		sscanf(argv[3], "%d", &size);
		//print header, used multiple lines so it doesnt look like a mess
		printf("========\n");
		printf("mode: parallel\n");
		printf("thread count: %i\n", num_threads);
		printf("size: %i\n", size);
		printf("========\n");
		//warm it up
		mmm_init(1,size);
		mmm_seq();
		mmm_freeup();
		//now we go through and calculate time for seq
		double total_time_seq = 0;
		for (int i = 0; i < 3; i++)
		{
			mmm_init(1,size);
			clockstart = rtclock();
			mmm_seq();
			clockend = rtclock();
			total_time_seq += clockend-clockstart;
			mmm_freeup();
		}
		total_time_seq = total_time_seq/3;
		printf("Sequential Time (avg of 3 runs): %.6f\n", total_time_seq);
		//calculate time for par
		double total_time_par = 0;
		for (int i = 0; i < 3; i++)
		{
			mmm_init(num_threads,size);
			clockstart = rtclock();
			mmm_par(NULL);
			clockend = rtclock();
			total_time_par += clockend-clockstart;
			mmm_freeup();
		}
		total_time_par = total_time_par/3;
		printf("Paralell Time (avg of 3 runs): %.6f\n", total_time_par);
		//find the speedup
		double speedup = total_time_seq/total_time_par;
		printf("Speedup: %.6f\n", speedup);
		//create a final one to verify error margin
		mmm_init(num_threads,size);
		mmm_seq();
		mmm_par(NULL);
		double error_margin = mmm_verify();
		printf("Verifying... largest error between parallel and sequential matrix: %.6f\n", error_margin);
		mmm_freeup();
		return 0;

	} else {
		//if neither of those went, we got an invalid letter
		printf("mode must be S for sequential or P for paralell\n");
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
		return 0;
	}
	

	return 0;
}
