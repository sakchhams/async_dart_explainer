#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Simple program to read two integers from files and add them.
// 
// to complie: gcc add_num.c -o add_num
// to run: ./add_num one two

typedef struct {
    char *filename;
    int *result;
} file_args;


// Opens a file and reads content from it into an integer.
// The process is repeated 100000 times.
// Each iteration adds the result to a result pointer.
void *read_integer_from_file(void *args) {
    file_args *actual_args = (file_args *)args;

    for(int i = 0; i < 100000; i++) {
        FILE *file = fopen(actual_args->filename, "r");
        if (file == NULL) {
            printf("error opening file: %s\n", actual_args->filename);
            exit(1);
        }

        int number;
        fscanf(file, "%d", &number);
        fclose(file);
        
        // Add the number to the result.
        *(actual_args->result) += number;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    // Check if the number of arguments is correct.
    if (argc != 3) {
        printf("Usage: %s num1 num2\n", argv[0]);
        return 1;
    }

    // Construct the filenames.
    char filename1[16], filename2[16];
    sprintf(filename1, "numbers/%s.txt", argv[1]);
    sprintf(filename2, "numbers/%s.txt", argv[2]);

    int result = 0;
    file_args args1 = {filename1, &result};
    file_args args2 = {filename2, &result};

    // Read the integers from the files.
    read_integer_from_file(&args1);
    read_integer_from_file(&args2);

    // Add the numbers and print the result.
    printf("The result is: %d\n", result);

    return 0;
}