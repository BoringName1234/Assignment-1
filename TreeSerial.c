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
 * Finds the minimum of two integers
 */
int minOf(int one, int two){
    if(one <= two){
        return one;
    }
    else{
        return two;
    }

}

struct infected_cell {
    int **nearby_cells;
    int index;
    int time_infected;
};

/**
 * Given a source cell, try to infect every other cell
 */
void attemptInfection(int **int_grids, float **float_grids, int *dims, int r, int source_position){                        
    
    
    return;
}

/**
 * Performs one step of a simulation. Returns 0 if there are no infected cells left, and 1 otherwise.
 */
int simulate_step(int r, int rec_time, int *dims, int **int_grids, float **float_grids){
    
    return 1;
}


/**
 * Runs an infection simulation, and returns the number of people infected.
 */
int single_simulation(int r, int rec_time, int *dims, int **int_grids, float **float_grids){
    // Declare some variables that will be reused here
    int *grid = int_grids[0];
    int num_elements = dims[0] * dims[1];

    // Randomly select the infection's starting point
    int random = rand() % num_elements;
    grid[random] = 1;
    

    // Run the simulation until there are no infections
    int infections = 1;
    while(infections == 1){
        infections = simulate_step(r, rec_time, dims, int_grids, float_grids);
        
        //printf("After After grid: %d and next_grid: %d\n", grid, next_grid);
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
    int r = atof(argv[1]);
    int rec_time = atoi(argv[2]);
    int max_runs = atoi(argv[3]);
    char *infile = argv[4];
    
    printf("Variables - r:%d, rec_time:%d, max_runs:%d, infile:%s \n", r, rec_time, max_runs, infile);
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
    
    // Create the grids that will be used
    int num_elements = dims[0] * dims[1];
    // grid is stores the infection status for each element
    int *grid = calloc(num_elements, sizeof(int));
    // This will store the changes to the grid
    int *next_grid = malloc(num_elements * sizeof(int));
    // Counts the time that each element has been infected for
    int *time_infected = calloc(num_elements, sizeof(int));
    // Create the grids used for spreading infection
    float *probability_grid = calloc((2*r + 1) * (2*r + 1), sizeof(float));
    int *manhattan_grid = calloc(((2*r + 1) * (2*r + 1)), sizeof(int));
    
    // Format: {grid, next_grid, time_infected, manhattan_grid}
    int *int_grids[] = {grid, next_grid, time_infected, manhattan_grid};

    // Format: {pop_density, probability_grid}
    float *float_grids[] = {pop_density, probability_grid};
    
    // The number of infections
    int num_infections = 0;
    
    gettimeofday(&start_time, NULL);
    // Each loop runs a single simulation
    for(int i = 0; i < max_runs; i++){
        num_infections += single_simulation(r, rec_time, dims, int_grids, float_grids);
        // Perform cleanup so that the arrays can be reused
        memset(grid, 0, num_elements * sizeof(int));
        memset(time_infected, 0, num_elements * sizeof(int));
    }
    gettimeofday(&end_time, NULL);

    
  	double seconds_taken = (end_time.tv_sec - start_time.tv_sec);           // just integral number of seconds
  	double total_time_taken = 1.0E-06 * ((seconds_taken*1000000) + (end_time.tv_usec - start_time.tv_usec)); // and now with any microseconds
  	printf("Time taken: %f seconds  \n", total_time_taken);

    float avg = ((float)num_infections / max_runs);
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
