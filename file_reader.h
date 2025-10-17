#ifndef _file_reader_h
#define _file_reader_h


int read_num_dims(const char *filename);
int *read_dims(const char *filename, int num_dims);
float *read_array(const char *filename, const int *dims, int num_dims);
int write_to_output_file(const char *filename, float *output, int *dims, int num_dims);
int product(const int *dims, int num_dims);

int test_func(int e);


#endif