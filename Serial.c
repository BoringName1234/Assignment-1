#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Get the functions from the file_reader file
/**
 * Takes a filename as a parameter and returns the number of dimensions 
 * in the file as an integer. 
 */
int read_num_dims(const char *filename);
/**
 * Takes a filename and the number of dimensions as parameters, and returns 
 * an array of dimensions 
 */
int *read_dims(const char *filename, int num_dims);
/**
 * Takes as parameters: a filename, the array of dimensions and the number 
 * of dimensions. This function returns a one-dimensional array of floats that are read 
 * from the given file.
 */
float *read_array(const char *filename, const int *dims, int num_dims);
/**
 * Writes to the output file.
 */
int write_to_output_file(const char *filename, float *output, int *dims, int num_dims);
/**
 * Takes the array of dimensions as an and the number of dimensions as a
 * parameter, and returns the product of them. Used to find the total number of elements
 * given the dimensions
 */
int product(const int *dims, int num_dims);


// Now define the serial functions.

// Simulation functions
void update_grid(){
    for(int i=0; i<size; size++){

    }
}

int check_infection(int *grid[], int num_elements){
    for (int i = 0; i < num_elements;i++)
    {
        if (grid[i] == 1)
        {
            // There is an infected cell
            return 1;
        }
    }

    // There weren't any infected cells
    return 0;
        
}

void simulate(double r, int rec_time, float *pop_density[], int *grid[], int num_elements, int *time_infected[]){
    // Finish simulation when there are no infected cells
    if (check_infection(grid, num_elements) == 0){
        return ;
    }

    // Otherwise update the grid
    // Update recovery
    for(int i = 0; i < num_elements; i++){
        if(grid[i] == 1){
            time_infected[i] += 1;
        }
    }

    // Spread disease
    for(int i = 0; i < num_elements; i++){
        if(grid[i] == 1){
            for(int i = 0; i < num_elements)
        }
    }

    // Probability of cell being infected
    float probability = pop_density[cell] / manhattan(source, target);
}

void initialise(int num_elements, int grid[]){
    int r = rand() % num_elements;
    grid[r] = 1;
}

int main (double r, int  rec_time, int max_runs, char infile)
{
    // Read the file here
    int num_dims = read_num_dims(infile);
    int dims[] = read_dims(infile, num_dims); 
    float *pop_density[] = read_array(infile, dims, num_dims);

    

    // Initialise the random number generation
    srand(time(NULL));   // Initialization, should only be called once.

    // Create the grid, inititalised to zero
    int num_elements = product(dims, num_dims);
    int *grid[] = calloc(num_elements, sizeof(int));
    int *time_infected = calloc(num_elements, sizeof(int));


    // Map it to a 2d array
    //int Two_DArray[] = malloc((TwoD_Array ) [ dims[0] ] = ( double ( * ) [ dims[0] ] ) grid);
    int *Two_D[] =;
    int ( *  ) [ dims[0] ] = ( int ( * ) [ 4 ] ) arr1;


    // Initialise the simulation
    initialise(num_elements, grid);

    // Start the simulation
    for(int i = 0; i < max_runs; i++){
        simulate(r, rec_time, pop_density, grid, num_elements, time_infected);
    }
}
