#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <fwSignal.h>

// A = 240
// X = 1 + ((240 % 47) % B) = 1 + (5 % B)

const int A = 240;

struct array_d {
    size_t size;
    double* restrict array;
};

void do_generate(struct array_d m1, struct array_d m2, unsigned int *seed) {
    for (int j = 0; j < m1.size; ++j) {
        m1.array[j] = (rand_r(seed) % A) + 1;
    }
    for (int j = 0; j < m2.size; ++j) {
        m2.array[j] = (rand_r(seed) % (A * 10)) + 1;
    }
}

void do_map(struct array_d m1, struct array_d m2) {
    // ВариантM1 = 1 + (5 % 7) = 6 - Кубический корень после деления на число e
    fwsDivC_64f(m1.array, M_E, m1.array, m1.size);
    fwsCbrt_64f_A50(m1.array, m1.array, m1.size);
    // ВариантM2 = 1 + (5 % 8) = 6 - Десятичный логарив, возведенный в степень e
    double* restrict temp_arr = malloc(m2.size * sizeof(double));
    fwsCopy_64f(m2.array, temp_arr, m2.size);
    fwsAdd_64f_I(temp_arr, m2.array + 1, m2.size - 1);
    fwsLog10_64f_A50(m2.array, m2.array, m2.size);
    fwsPowx_64f_A50(m2.array, M_E, m2.array, m2.size);
    free(temp_arr);
}

void do_merge(struct array_d m1, struct array_d m2) {
    // Вариант = 1 + (5 % 6) = 6 = Модуль разости
    fwsSub_64f_I(m1.array, m2.array, m2.size);
    fwsAbs_64f_I(m2.array, m2.size);
}

void do_sort(struct array_d m) {
    // Вариант = 6 - Сортировка вставками
    // Реализация взята с https://www.geeksforgeeks.org/insertion-sort/
    int i, j;
    double key;
    for (i = 1; i < m.size; i++) {
        key = m.array[i];
        j = i - 1;
        while (j >= 0 && m.array[j] > key) {
            m.array[j + 1] = m.array[j];
            j--;
        }
        m.array[j + 1] = key;
    }
}

double do_reduce(struct array_d m) {
    double min_element = 1000000000.0;
    for (int i = 0; i < m.size; ++i) {
        if (m.array[i] != 0) {
            min_element = m.array[i] < min_element ? m.array[i] : min_element;
        }
    }
    double sinus_sum = 0.0;
    for (int i = 0; i < m.size; ++i) {
        if ((int) floor(m.array[i] / min_element) % 2 == 0) {
            sinus_sum = sin(m.array[i]);
        }
    }
    return sinus_sum;
}

void write_execution_time_to_file(char *file_name, long execution_time) {
    FILE *out_file = fopen(file_name, "a+");
    if (out_file == NULL) {
        printf("Error! Could not open file\n");
        return;
    }
    fprintf(out_file, "%ld\n", execution_time);
}

void write_first_five_experiments_x(char *file_name, double *experiments_result, int N, size_t exp_num) {
    FILE *out_file = fopen(file_name, "a+");
    if (out_file == NULL) {
        printf("Error! Could not open file\n");
        return;
    }
    fprintf(out_file, "X results for N = %d\n", N);
    for (size_t i = 0; i < exp_num; ++i) {
        fprintf(out_file, "%f\n", experiments_result[i]);
    }
}

int main(int argc, char *argv[]) {

    int i, N, M;
    struct timeval T1, T2;
    long delta_ms;
    N = atoi(argv[1]); // N равен первому параметру командной строки
    if (N < 0) {
        return -1;
    }
    M = 0;
    if (argc >= 3) {
        M = atoi(argv[2]);
    }
    M = M != 0 ? M : 1;
    fwSetNumThreads(M);
    gettimeofday(&T1, NULL); // запомнить текущее время
    double experiments_result[5];
    for (i = 0; i < 100; ++i) {
        struct array_d m1, m2;
        m1.size = N;
        m1.array = malloc(m1.size * sizeof(double));
        m2.size = N / 2;
        m2.array = malloc(m2.size * sizeof(double));
        srand(i);
        unsigned int seed = i;
        do_generate(m1, m2, &seed);
        do_map(m1, m2);
        do_merge(m1, m2);
        do_sort(m2);
        double X = do_reduce(m2);
        if (i < 5) {
            experiments_result[i] = X;
        }
        printf("Verification: X = %f\n", X);
    }
    gettimeofday(&T2, NULL);
    delta_ms = 1000 * (T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec) / 1000;
    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms);
    if (argc >= 4) {
        write_execution_time_to_file(argv[3], delta_ms);
    }
    if (argc >= 5) {
        size_t experiments_size = sizeof(experiments_result)/sizeof(experiments_result[0]);
        write_first_five_experiments_x(argv[4], experiments_result, N, experiments_size);
    }
    return 0;
}