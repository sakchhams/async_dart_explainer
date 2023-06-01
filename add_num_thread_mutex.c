#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Struct to pass parameters to the thread function.
typedef struct {
    char *filename;
    int *result;
    pthread_mutex_t *mutex;
} thread_args;

// Function to read the integer from a file in a separate thread.
void *read_integer_from_file(void *args) {
    thread_args *actual_args = (thread_args *)args;

    for (int i = 0; i < 100000; i++) {
        FILE *file = fopen(actual_args->filename, "r");
        if (file == NULL) {
            printf("Error opening file: %s\n", actual_args->filename);
            exit(1);
        }

        int number;
        fscanf(file, "%d", &number);
        fclose(file);

        // Lock the mutex before accessing the result.
        pthread_mutex_lock(actual_args->mutex);

        // Add the number to the result.
        *(actual_args->result) += number;

        // Unlock the mutex.
        pthread_mutex_unlock(actual_args->mutex);
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
    pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;

    // Create threads to read the integers from the files.
    pthread_t thread1, thread2;
    thread_args args1 = {filename1, &result, &result_mutex};
    thread_args args2 = {filename2, &result, &result_mutex};

    pthread_create(&thread1, NULL, read_integer_from_file, &args1);
    pthread_create(&thread2, NULL, read_integer_from_file, &args2);

    // Wait for both threads to finish.
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Print the result.
    printf("The result is: %d\n", result);

    // Destroy the mutex.
    pthread_mutex_destroy(&result_mutex);

    return 0;
}
