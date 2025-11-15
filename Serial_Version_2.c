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

/**
 * Given a source cell, try to infect every other cell
 */
void attemptInfection(int *grid, int *next_grid, float *pop_density, int *dims, int r, int source_position,
                            float *probability_grid, int *manhattan_table){                        
    // Limiting the search area to only cells that are in range will help with large grids
    // Using Manhattan distance, this shape will be all of the cells accessible 
    //        4
    //      4 3 4
    //    4 3 2 3 4
    //  4 3 2 1 2 3 4
    //4 3 2 1 0 1 2 3 4
    //  4 3 2 1 2 3 4
    //    4 3 2 3 4
    //      4 3 4 
    //        4

    // (2r + 1) * (2r + 1)
    int num_columns = dims[1];
    int num_rows = dims[0];

    int source_row = source_position / num_columns;
    int source_col = source_position % num_columns;

    // x mod num_colums is the position in the row - 0 is the first, 1 is the second etc
    // It is therefore also the number of elements to the left of i
    // We only care about the columns/rows in maximum range
    int num_cols_left = source_position % num_columns;
    num_cols_left = minOf(num_cols_left, r);
    int num_cols_right = num_columns - 1 - num_cols_left;
    num_cols_right = minOf(num_cols_right, r);

    // Similar logic for rows
    // x / num_columns is the row of x, and since C indexes from 0 it is also the number of rows above
    int rows_above = source_position / num_columns;
    rows_above = minOf(rows_above, r);
    int rows_below = num_rows - 1 - rows_above;
    rows_below = minOf(rows_below, r);

    // First build an array of all of the distances
    // Then calculate the probability of each 

    int total_columns = num_cols_left + 1 + num_cols_right;
    int total_rows = rows_above + 1 + rows_below;
    int start = source_position - (num_columns * rows_above) - num_cols_left;

    // For entries above
    for(int i = 0; i < total_columns * total_rows; i++){
        // position is the actual position in the grid
        int y_pos = i / (total_columns);
        int x_pos = i % (total_columns);
        int position = start + (y_pos * num_columns) + x_pos;

        int row = position / num_columns;
        int col = position % num_columns;

        int x_dist = abs(row - source_row);
        int y_dist = abs(col - source_col);
            
        int manhattan = x_dist + y_dist;

        manhattan_table[i] = manhattan;
        
        probability_grid[i] = pop_density[position] / (manhattan);

    }

    for(int i = 0; i < total_columns * total_rows; i++){
        float random_number = (float)rand()/(float)(RAND_MAX);
        if(random_number <= probability_grid[i]){
            int y_pos = i / (total_columns);
            int x_pos = i % (total_columns);
            int position = start + (y_pos * num_columns) + x_pos;

            if(grid[position] == 0){
                next_grid[position] = 1;
            }
        }
    }

    return;
}

/**
 * Runs an infection simulation, and returns the number of people infected.
 */
int single_simulation(int r, int rec_time, int *dims, float *pop_density){
    // Setup all of the simualtion variables

    // Create the grid, inititalised to zero
    int num_elements = dims[0] * dims[1];
    int *grid = calloc(num_elements, sizeof(int));
    int *time_infected = calloc(num_elements, sizeof(int));

    // Randomly select the infection's starting point
    int random = rand() % num_elements;
    grid[random] = 1;

    // This grid is used to update the current grid
    int *next_grid = malloc(num_elements * sizeof(int));
    memcpy(next_grid, grid, num_elements * sizeof(int));

    printf("\nstart\n");
    output_Grid(grid, dims);

    // These grids are used in the attempInfection function
    // The max space available is (2r + 1)^2
    float *probability_grid = calloc((2*r + 1) * (2*r + 1), sizeof(float));
    int *manhattan_table = calloc(((2*r + 1) * (2*r + 1)), sizeof(int));

    // This is the simulation loop
    int infections = 1;
    while(infections == 1){
        // Finish simulation when there are no infected cells
        if (check_infection(grid, dims) == 0){
            infections = 0;
        }
        
        // Spread disease, from infected cells
        for(int i = 0; i < num_elements; i++){
            if(grid[i] == 1){    
                //printf("Attempting to infect more untits.\n");
                attemptInfection(grid, next_grid, pop_density, dims, r, i, probability_grid, manhattan_table);
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

        
        //printf("\nTime\n");
        //output_Grid(time_infected, dims);

        //printf("\nBefore grid\n");
        //output_Grid(grid, dims);

        
        //printf("\nBefore next_grid\n");
        //output_Grid(next_grid, dims);

        // Copy the next_grid into the grid
        memcpy(grid, next_grid, num_elements * sizeof(int));

        printf("\nAfter\n");
        output_Grid(grid, dims);


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
