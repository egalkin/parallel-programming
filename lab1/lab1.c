#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

// A = 240
// X = 1 + ((240 % 47) % B) = 1 + (5 % B)

const int A = 240;

struct array_d {
    size_t size;
    double* restrict array;
};

void do_generate(struct array_d m1, struct array_d m2, unsigned int *seed) {
    for (int i = 0; i < m1.size; ++i) {
        m1.array[i] = (rand_r(seed) % A) + 1;
    }
    for (int i = 0; i < m2.size; ++i) {
        m2.array[i] = (rand_r(seed) % (A * 9)) + A;
    }
}

void do_map(struct array_d m1, struct array_d m2) {
    // ВариантM1 = 1 + (5 % 7) = 6 - Кубический корень после деления на число e
    for (int i = 0; i < m1.size; ++i) {
        double tmp = m1.array[i] / M_E;
        m1.array[i] = cbrt(tmp);
    }
    // ВариантM2 = 1 + (5 % 8) = 6 - Десятичный логарив, возведенный в степень e
    double* restrict temp_arr = malloc(m2.size * sizeof(double));
    for (int i = 0; i < m2.size; ++i) {
        temp_arr[i] = m2.array[i];
    }
    m2.array[0] = pow(log10(0 + temp_arr[0]), M_E);
    for (int i = 1; i < m2.size; ++i) {
        m2.array[i] = pow(log10(temp_arr[i - 1] + temp_arr[i]), M_E);
    }
    free(temp_arr);
}

void do_merge(struct array_d m1, struct array_d m2) {
    // Вариант = 1 + (5 % 6) = 6 = Модуль разности
    for (int i = 0; i < m2.size; ++i) {
        m2.array[i] = fabs(m1.array[i] - m2.array[i]);
    }
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
    if (m.size <= 0) {
        return 0.0;
    }
    double min_element = m.array[0];
    for (int i = 1; i < m.size; ++i) {
        if (m.array[i] != 0) {
            min_element = m.array[i] < min_element ? m.array[i] : min_element;
        }
    }
    double sinus_sum = 0.0;
    for (int i = 0; i < m.size; ++i) {
        if ((int) floor(m.array[i] / min_element) % 2 == 0) {
            sinus_sum += sin(m.array[i]);
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
    int i, N;
    struct timeval T1, T2;
    long delta_ms;
    N = atoi(argv[1]); // N равен первому параметру командной строки
    if (N < 0) {
        return -1;
    }
    double experiments_result[5];
    struct array_d m1, m2;
    m1.size = N;
    m1.array = malloc(m1.size * sizeof(double));
    m2.size = N / 2;
    m2.array = malloc(m2.size * sizeof(double));
    gettimeofday(&T1, NULL); // запомнить текущее время
    for (i = 0; i < 100; ++i) {
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
    if (argc >= 3) {
        write_execution_time_to_file(argv[2], delta_ms);
    }
    if (argc >= 4) {
        size_t experiments_size = sizeof(experiments_result)/sizeof(experiments_result[0]);
        write_first_five_experiments_x(argv[3], experiments_result, N, experiments_size);
    }
    free(m1.array);
    free(m2.array);
    return 0;
}

