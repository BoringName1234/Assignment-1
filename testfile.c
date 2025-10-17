#include <stdio.h>
#include <stdlib.h>

#include "file_reader.h"

//int test_func(int e);

int main (){
    int arr1[] = {1, 2, 3, 4, 
                    5, 6, 7, 8};
    int ( * arr2 ) [ 4 ] = ( int ( * ) [ 4 ] ) arr1;
    printf("%d", arr2[1][2]);

    int dims[] = {5, 4};
    int *grid = calloc(8, sizeof(int));
    
    for(int i = 0; i < 5*4; i++)
    {
        grid[i] = i;
    }
    
    // Map it to a 2d array
    int ( * grid2) [dims[1]] = ( int ( * ) [dims[0]] ) grid;
    printf("h %d", grid2[4][3]);

    // 0 1 2
    // 3 4 5
    // 6 7 8 
    // 9 10 11
    // coordinate = [x % 4,]
    // manhattan = 

    printf("%d", test_func(1));
}