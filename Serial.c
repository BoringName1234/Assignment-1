#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
int check_infection(int *grid[], int dims[]){
    for (int i = 0; i < dims[0];i++)
    {
        for(int j = 0; j < dims[1]; j++){
            if (grid[i][j] == 1)
            {
                // There is an infected cell
                return 1;
            }
        }
        
    }

    // There weren't any infected cells
    return 0;
        
}

/**
 * Given a source cell, try to infect every other cell
 */
void attemptInfection(int *grid[], int *next_grid[], float *pop_density[], int dims[], double r,
                        int source_x, int source_y){
    for(int i = 0; i < dims[0]; i++){
        for(int j = 0; j < dims[1]; j++){
            // First check if it is not immune or infected already
            if(grid[i][j] == 0){
                int x_dist = abs(i - source_x);
                int y_dist = abs(j - source_y);
                int manhattan = x_dist + y_dist;
                if(manhattan <= r){
                    float probability = pop_density[i][j] / (x_dist + y_dist);
                    // Get a random number
                    float ra = (float)rand()/(float)(RAND_MAX);
                    if(ra <= probability){
                        next_grid[source_x][source_y] = 1;
                    }
                }

            }
        }
    }
    return;
}

/**
 * Performs one step of a simulation. Returns 0 if there are no infected cells left, and 1 otherwise.
 */
int simulate_step(double r, int rec_time, float *pop_density[], int *grid[], int dims[], int *time_infected[]){
    // Finish simulation when there are no infected cells
    if (check_infection(grid, dims) == 0){
        return 0;
    }

    int num_elements = (dims[0] * dims[1]);
    // This will store the changes to the grid
    int *next_grid_1d = malloc(num_elements * sizeof(int));
    // Copy the grid to next_grid
    memcpy(next_grid_1d, grid, num_elements * sizeof(int));
    // Create a 2d pointer
    int ( * next_grid) [dims[1]] = ( int ( * ) [dims[0]] ) next_grid_1d;

    // Otherwise update the grid
    // Update recovery and immunity
    for(int i = 0; i < dims[0]; i++){
        for(int j = 0; j < dims[1]; j++){
            if(grid[i][j] == 1){
                // Check for immunity condition before updating the time
                if(time_infected[i][j] == rec_time){
                    next_grid[i][j] = 2;
                }
                time_infected[i][j] += 1;
            }
        }
    }

    // Spread disease
    for(int i = 0; i < dims[0]; i++){
        for(int j = 0; j < dims[1]; j++){
            if(grid[i][j] == 1){
                attemptInfection(grid, next_grid, pop_density, dims, r, i, j);
            }
        }
    }

    // Update the grid
    memcpy(grid, next_grid, num_elements * sizeof(int));

    // Return 1 if there is an infected cell
    return 1;
}

void initialise(int num_elements, int grid[]){
    int r = rand() % num_elements;
    grid[r] = 1;
}

/**
 * Runs an infection simulation, and returns the number of people infected.
 */
int single_simulation(double r, int rec_time, int dims[], float pop_density[]){
    // Setup all of the simualtion variables

    // Create the grid, inititalised to zero
    int num_elements = dims[0] * dims[1];
    int *grid_1d[] = calloc(num_elements, sizeof(int));
    int *time_infected_1d = calloc(num_elements, sizeof(int));

    // Map them to 2d arrays
    int ( * grid) [dims[1]] = ( int ( * ) [dims[0]] ) grid_1d;
    int ( * time_infected) [dims[1]] = ( int ( * ) [dims[0]] ) time_infected_1d;
    float ( * pop_density) [dims[1]] = ( int ( * ) [dims[0]] ) pop_density_1d;
    
    // Initialise the simulation
    initialise(num_elements, grid);
    
    // Run the simulation until there are no infections
    int infections = 1;
    while(infections == 1){
        infections = simulate_step(r, rec_time, pop_density, grid, dims, time_infected);
    }

    // Count the number of infections
    int count = 0;
    for(int i = 0; i < dims[0]; i++){
        for(int j = 0; j < dims[1]; j++){
            if(grid[i][j] == 2){
                count += 1;
            }
        }
    }

    return count;
}

int main (double r, int  rec_time, int max_runs, char infile)
{
    // Read the file here
    int num_dims = read_num_dims(infile);
    int dims[] = read_dims(infile, num_dims); 
    float *pop_density_1d[] = read_array(infile, dims, num_dims);

    // Initialise the random number generation
    srand(time(NULL));   // Initialization, should only be called once.

    int num = 0;
    int temp = 0;
    // Each loop runs a single simulation
    for(int i = 0; i < max_runs; i++){
        num += single_simulation(r, rec_time, dims, pop_density);
    }

    float avg = (float)(num / max_runs);

    printf("The average number of infections: %f", avg);
}
