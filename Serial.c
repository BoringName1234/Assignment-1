#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "file_reader.h"

// Get the functions from the file_reader file
/**
 * Takes a filename as a parameter and returns the number of dimensions 
 * in the file as an integer. 
 */
//int read_num_dims(const char *filename);
/**
 * Takes a filename and the number of dimensions as parameters, and returns 
 * an array of dimensions 
 */
//int *read_dims(const char *filename, int num_dims);
/**
 * Takes as parameters: a filename, the array of dimensions and the number 
 * of dimensions. This function returns a one-dimensional array of floats that are read 
 * from the given file.
 */
//float *read_array(const char *filename, const int *dims, int num_dims);
/**
 * Writes to the output file.
 */
//int write_to_output_file(const char *filename, float *output, int *dims, int num_dims);
/**
 * Takes the array of dimensions as an and the number of dimensions as a
 * parameter, and returns the product of them. Used to find the total number of elements
 * given the dimensions
 */
//int product(const int *dims, int num_dims);


// Now define the serial functions.

// Simulation functions
int check_infection(int *grid, int *dims){
    for(int i = 0; i < dims[0] * dims[1]; i++){
        if (grid[i] == 1){
            //printf("Returned with 1\n");
            return 1;
        }
    }

    //printf("Returned with 0\n");
    // There weren't any infected cells
    return 0;
        
}

/**
 * Given a source cell, try to infect every other cell
 */
void attemptInfection(int *grid, int *next_grid, float *pop_density, int *dims, double r, int source_x){                        
                   
    int source_row = source_x / dims[0];
    int source_col = source_x % dims[0];
                         
    for(int i = 0; i < dims[0] * dims[1]; i++){
        if(grid[i] == 0){
            int row = i / dims[0];
            int col = i % dims[0];

            int x_dist = abs(row - source_row);
            int y_dist = abs(col - source_col);
            
            int manhattan = x_dist + y_dist;
            if(manhattan <= r){
                    float probability = pop_density[i] / (manhattan);
                    //printf("manhattan = %d and pop_density = %f\n", manhattan, pop_density[i]);
                    // Get a random number
                    float ra = (float)rand()/(float)(RAND_MAX);
                    //printf("ra = %f and probability = %f\n", ra, probability);
                    if(ra <= probability){
                        next_grid[i] = 1;
                        //printf("Infected %d", i);
                    }
                }
        }
    }

    return;
}

/**
 * Performs one step of a simulation. Returns 0 if there are no infected cells left, and 1 otherwise.
 */
int simulate_step(double r, int rec_time, int *dims, float *pop_density, int *grid, int *time_infected){
    // Finish simulation when there are no infected cells
    if (check_infection(grid, dims) == 0){
        return 0;
    }
    //printf("Infection check failed.\n");

    int num_elements = (dims[0] * dims[1]);
    // This will store the changes to the grid
    int *next_grid = malloc(num_elements * sizeof(int));
    // Copy the grid to next_grid
    memcpy(next_grid, grid, num_elements * sizeof(int));
    
    //printf("time infected is:");
    //output_Grid(time_infected, dims);
    //printf("next_grid is: ");
    //output_Grid(next_grid, dims);

    // Spread disease, from infected cells
    for(int i = 0; i < num_elements; i++){
        if(grid[i] == 1){    
            //printf("Attempting to infect more untits.\n");
            attemptInfection(grid, next_grid, pop_density, dims, r, i);
        }
    }
    
    // The recovery timer should be set up so that you update it for every cell that is infectious THIS TURN
    // And then try to make them recover
    // Update recovery and immunity
    for(int i = 0; i < num_elements; i++){
        if(grid[i] == 1){
            time_infected[i] += 1;
            if(time_infected[i] == rec_time){
                next_grid[i] = 2;
            }
        }
    }
    //printf("next_grid is: ");
    //output_Grid(next_grid, dims);
    // Update the grid
    memcpy(grid, next_grid, num_elements * sizeof(int));
    
    //printf("grid is now: ");
    //output_Grid(grid, dims);

    // Return 1 if there is an infected cell
    return 1;
}


/**
 * Runs an infection simulation, and returns the number of people infected.
 */
int single_simulation(double r, int rec_time, int *dims, float *pop_density){
    // Setup all of the simualtion variables

    // Create the grid, inititalised to zero
    int num_elements = dims[0] * dims[1];
    int *grid = calloc(num_elements, sizeof(int));
    int *time_infected = calloc(num_elements, sizeof(int));

    // Randomly select the infection's starting point
    int random = rand() % num_elements;
    grid[random] = 1;

    //output_Grid(grid, dims);
    // Run the simulation until there are no infections
    int infections = 1;
    while(infections == 1){
        infections = simulate_step(r, rec_time, dims, pop_density, grid,  time_infected);
    }

    // Count the number of infections
    int count = 0;
    for(int i = 0; i < num_elements; i++){
        if(grid[i] == 2){
            count += 1;
        }
    }
    //printf("Societal collapse!\n");
    return count;
}

int main (int argc, char *argv[])
{
    // Set the user provided values
    //double r, int  rec_time, int max_runs, const char *infile
    double r = atof(argv[1]);
    int rec_time = atoi(argv[2]);
    int max_runs = atoi(argv[3]);
    char *infile = argv[4];
    
    printf("Variables - r:%f, rec_time:%d, max_runs:%d, infile:%s \n", r, rec_time, max_runs, infile);
    printf("Num args: %d\n", argc);
    // Read the file here
    int num_dims = read_num_dims(infile);
    printf("num_dims: %d\n", num_dims);
    int *dims = read_dims(infile, num_dims); 
    printf("dims: %d, %d\n", dims[0], dims[1]);
    float *pop_density = read_array(infile, dims, num_dims);

    // Initialise the random number generation
    srand(time(NULL));   // Initialization, should only be called once.

    // Initialise the time variables
    struct timeval start_time, end_time;
    
    int num = 0;
    int temp = 0;
    
    gettimeofday(&start_time, NULL);
    // Each loop runs a single simulation
    for(int i = 0; i < max_runs; i++){
        num += single_simulation(r, rec_time, dims, pop_density);
    }
    gettimeofday(&end_time, NULL);

    
  	double seconds_taken = (end_time.tv_sec - start_time.tv_sec);           // just integral number of seconds
  	double total_time_taken = 1.0E-06 * ((seconds_taken*1000000) + (end_time.tv_usec - start_time.tv_usec)); // and now with any microseconds
  	printf("Time taken: %f seconds  \n", total_time_taken);

    float avg = ((float)num / max_runs);
    printf("The average number of infections: %f", avg);
}

void output_Grid(int *grid, int *dims){
    int prev = 0;
    printf("{");
    for(int i = 0; i < dims[0] * dims[1]; i++){
        if(i / dims[0] > prev){
            printf("}\n{");
            prev++;
        }
        printf("%d ", grid[i]);
    }
    printf("}\n");
}
