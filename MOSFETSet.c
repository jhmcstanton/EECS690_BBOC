#define DESD_TEMP // the constant temp to hold the black box
#define TEMP_THRESH // the allowable threshold (high or low) for temperature 
#define SMP_DELAY // the time between individual samples in the sample set
#define WINDOW_Q // the queue structure holding the window of samples
#define CURR_INDEX // the current index of the beginning of the sample window in the queue
#define NUM_SAMPLES // number of samples to take before taking the average temp

MOSFETSet():
	if sample_avg > DESD_TEMP + TEMP_THRESH: // let the box cool
		unpower(mosfet)
		serial(“MOSFET power OFF”)
	elseif sample_avg < DESD_TEMP - TEMP_THRESH: // box is too cold
		power(mosfet)
		serial(“MOSFET power ON”)
	else: // box is within desired temperature, keeping the MOSFET in the same state
		serial(“TEMP is acceptable”)