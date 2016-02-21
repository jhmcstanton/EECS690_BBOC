#define DESD_TEMP 85 // the constant temp to hold the black box
#define TEMP_THRESH 5 // the allowable threshold (high or low) for temperature 
#define SMP_DELAY 500 // the time between individual samples in the sample set
#define CURR_INDEX // the current index of the beginning of the sample window in the queue
#define NUM_SAMPLES // number of samples to take before taking the average temp

typdef int* WINDOW_Q ; // the queue structure holding the window of samples

/**
 * Switches the power MOSFET into an ON state (closes the circuit to
 * the power resistor)
 * @returns nothing
 */
void power(){

}

/**
 * Switches the power MOSFET into an OFF state (opens the circuit to
 * the power resistor). 
 * @returns nothing
 */
void unpower(){

}

/**
 * Sends a null-terminated string to the controlling PC the TIVA board
 * is connected to via the USB serial interface.
 * @param msg - the null-terminated string to send via serial. If NULL then nothing is sent
 * and serial communication is never performed
 * @returns nothing
 */
void serial(char* msg){

}


/**
 * This checks to see if the sample_avg value is inside the desired
 * threshold. If it is below the threshold then the power MOSFET is
 * switch ON, if it is above it is switched off, otherwise the MOSFET
 * is left alone. 
 * @returns nothing
 */
void MOSFETSet(){
  if (sample_avg > DESD_TEMP + TEMP_THRESH){ // let the box cool
    unpower(mosfet);
    serial(“MOSFET power OFF”);
  } else if(sample_avg < DESD_TEMP - TEMP_THRESH){ // box is too cold
    power(mosfet);
    serial(“MOSFET power ON”);
  } else{ // box is within desired temperature, keeping the MOSFET in the same state
    serial(“TEMP is acceptable”);
  }
}
