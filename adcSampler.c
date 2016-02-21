#define DESD_TEMP // the constant temp to hold the black box
#define TEMP_THRESH // the allowable threshold (high or low) for temperature 
#define SMP_DELAY // the time between individual samples in the sample set
#define WINDOW_Q // the queue structure holding the window of samples
#define CURR_INDEX // the current index of the beginning of the sample window in the queue
#define NUM_SAMPLES // number of samples to take before taking the average temp

adcSampler():
	sample_avg = 0
	new_sample = sample(adc)

	// this window will more likely be an array in final code, and will need
	// to maintain the CURR_INDEX of the sample in the array
	if size(WINDOW_Q) == NUM_SAMPLES: 
		pop(WINDOW_Q) // remove the oldest sample
	
	push(new_sample, WINDOW_Q)