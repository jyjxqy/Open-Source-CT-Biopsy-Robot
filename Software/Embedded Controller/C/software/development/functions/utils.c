#include "../main.h"
#include <unistd.h>
#include <sys/time.h>
#include <inttypes.h>


void write_string(char *write_buffer){
	bzero(write_buffer,1024);

	// sprintf(write_buffer,"nn %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f qq", internal_encoders[0], internal_encoders[1], internal_encoders[2],\
	// 	internal_encoders[3], internal_encoders[4], internal_encoders[5], internal_encoders[6], internal_encoders[7], switch_states[0],\
	// 	switch_states[1], switch_states[2], switch_states[3], switch_states[4], switch_states[5], switch_states[6], switch_states[7],\
	// 	arm_encoders1, arm_encoders2, arm_encoders3, arm_encoders4, avg_current_array[0]);


	sprintf(write_buffer,"nn %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f %f %f %f %f %f %f %f %" PRIu64 " qq",internal_encoders[0],\
		internal_encoders[1], internal_encoders[2], internal_encoders[3], internal_encoders[4], internal_encoders[5], internal_encoders[6],\
		internal_encoders[7], switch_states[0], switch_states[1], switch_states[2], switch_states[3], switch_states[4], switch_states[5], switch_states[6], switch_states[7],\
		arm_encoders1, arm_encoders2, arm_encoders3, arm_encoders4, avg_current_array[0],avg_current_array[1],avg_current_array[2],\
		avg_current_array[3],avg_current_array[4],avg_current_array[5],avg_current_array[6],avg_current_array[7], global_loop_start_time);
}

void calc_current_offset(volatile unsigned long *h2p_lw_adc, int *offset){
	int counter = 300,i,j;

	//sample at 100hz for 3 seconds
	for(j = 0; j < counter; j++){
		*(h2p_lw_adc) = 0; //write starts adc read
		for (i=0; i<8; i++){
			offset[i] += *(h2p_lw_adc + i); // 4096 for ADC, 4.096v reference, unit of Volts and 1A/1V for current sensor --> A units too
		}
		usleep(10000);
	}
	
	for(i=0; i<8; i++){
		offset[i] /= counter;
	}
}

uint32_t createMask(uint32_t startBit, int num_bits)
{
   uint32_t  mask;
	mask = ((1 << num_bits) - 1) << startBit;
   return mask;
}

uint32_t createNativeInt(uint32_t input, int size)
{
	int32_t nativeInt;
	const int negative = ((input & (1 << (size - 1))) != 0);
	if (negative)
		  nativeInt = input | ~((1 << size) - 1);
	else 
		  nativeInt = input;	
	return nativeInt;
}


uint64_t GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}
