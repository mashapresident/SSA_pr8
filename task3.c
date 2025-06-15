#include <stdio.h>
#include <stdlib.h>
#include <time.h>   
#include <string.h> 

static int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

static void generate_sorted_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
}

static void generate_reverse_sorted_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - 1 - i;
    }
}

static void generate_random_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % size;
    }
}

static void generate_repeating_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i % 10;
    }
}

static double measure_qsort_time(int arr[], int size) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort(arr, size, sizeof(int), compare_ints);
    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

static int is_array_sorted(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i+1]) {
            return 0;
        }
    }
    return 1;
}

static int check_elements_preserved(int original_arr[], int sorted_arr[], int size, int max_val) {
    int *counts_original = (int *)calloc(max_val + 1, sizeof(int));
    int *counts_sorted = (int *)calloc(max_val + 1, sizeof(int));

    if (!counts_original || !counts_sorted) {
        perror("calloc failed in check_elements_preserved");
        free(counts_original);
        free(counts_sorted);
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (original_arr[i] < 0 || original_arr[i] > max_val) {
            fprintf(stderr, "warning: element %d out of expected range [0, %d] in original_arr\n", original_arr[i], max_val);
            free(counts_original);
            free(counts_sorted);
            return 0;
        }
        counts_original[original_arr[i]]++;
    }

    for (int i = 0; i < size; i++) {
        if (sorted_arr[i] < 0 || sorted_arr[i] > max_val) {
            fprintf(stderr, "warning: element %d out of expected range [0, %d] in sorted_arr\n", sorted_arr[i], max_val);
            free(counts_original);
            free(counts_sorted);
            return 0;
        }
        counts_sorted[sorted_arr[i]]++;
    }

    int result = 1;
    for (int i = 0; i < max_val + 1; i++) {
        if (counts_original[i] != counts_sorted[i]) {
            result = 0;
            break;
        }
    }

    free(counts_original);
    free(counts_sorted);
    return result;
}

int main() {
    srand(time(NULL));

    int sizes[] = {1000, 5000, 10000, 50000, 100000, 200000, 500000, 1000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        int *original_arr = (int *)malloc(size * sizeof(int));
        int *test_arr = (int *)malloc(size * sizeof(int));

        if (!original_arr || !test_arr) {
            perror("malloc failed");
            return 1;
        }
        printf("array size: %d\n", size);
        generate_sorted_array(original_arr, size);
        memcpy(test_arr, original_arr, size * sizeof(int));
        double time_sorted = measure_qsort_time(test_arr, size);
        printf("sorted array: %.6f seconds\n", time_sorted);
        if (!is_array_sorted(test_arr, size)) printf("sorting error: not sorted\n");
        if (!check_elements_preserved(original_arr, test_arr, size, size - 1)) printf("sorting error: elements lost/changed\n");

        generate_reverse_sorted_array(original_arr, size);
        memcpy(test_arr, original_arr, size * sizeof(int));
        double time_reverse = measure_qsort_time(test_arr, size);
        printf("reverse-sorted array: %.6f seconds\n", time_reverse);
        if (!is_array_sorted(test_arr, size)) printf("sorting error: not sorted\n");
        if (!check_elements_preserved(original_arr, test_arr, size, size - 1)) printf("sorting error: elements lost/changed\n");

        generate_random_array(original_arr, size);
        memcpy(test_arr, original_arr, size * sizeof(int));
        double time_random = measure_qsort_time(test_arr, size);
        printf("random array: %.6f seconds", time_random);
        if (!is_array_sorted(test_arr, size)) printf("sorting error: not sorted\n");
        if (!check_elements_preserved(original_arr, test_arr, size, size - 1)) printf("sorting error: elements lost/changed\n");

        generate_repeating_array(original_arr, size);
        memcpy(test_arr, original_arr, size * sizeof(int));
        double time_repeating = measure_qsort_time(test_arr, size);
        printf("array with repetitions: %.6f seconds\n", time_repeating);
        if (!is_array_sorted(test_arr, size)) printf("sorting error: not sorted\n");

        if (!check_elements_preserved(original_arr, test_arr, size, 9)) printf("sorting error: elements lost/changed\n");

        free(original_arr);
        free(test_arr);
    }
    return 0;
}
