#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <fwSignal.h>

// A = 240
// X = 1 + ((240 % 47) % B) = 1 + (5 % B)

const int A = 240;

void do_generate(double *M1, double *M2, int N, unsigned int *seed) {
    for (int j = 0; j < N; ++j) {
        M1[j] = (rand_r(seed) % A) + 1;
    }
    for (int j = 0; j < N / 2; ++j) {
        M2[j] = (rand_r(seed) % (A * 10)) + 1;
    }
}

void do_map(double *M1, double *M2, const int N) {
    // ВариантM1 = 1 + (5 % 7) = 6 - Кубический корень после деления на число e
    fwsDivC_64f_I(M_E, M1, N);
    fwsCbrt_64f_A53(M1, M1, N);
    // ВариантM2 = 1 + (5 % 8) = 6 - Десятичный логарив, возведенный в степень e
    int M2_LEN = N / 2;
    double M2_COPY[M2_LEN];
    for (int j = 0; j < M2_LEN; ++j) {
        M2_COPY[j] = M2[j];
    }
    for (int j = 1; j < M2_LEN; ++j) {
        M2[j] = M2_COPY[j] + M2_COPY[j - 1];
    }
    fwsLog10_64f_A53(M2, M2, M2_LEN);
    fwsPowx_64f_A53(M2, M_E, M2, M2_LEN);
}

void do_merge(double *M1, double *M2, const int N) {
    // Вариант = 1 + (5 % 6) = 6 = Модуль разности
    fwsSub_64f_I(M1, M2, N);
    fwsAbs_64f_I(M2, N);
}

void do_sort(double *M, const int n) {
    // Вариант = 6 - Сортировка вставками
    // Реализация взята с https://www.geeksforgeeks.org/insertion-sort/
    int i, j;
    double key;
    for (i = 1; i < n; i++) {
        key = M[i];
        j = i - 1;
        while (j >= 0 && M[j] > key) {
            M[j + 1] = M[j];
            j--;
        }
        M[j + 1] = key;
    }
}

double do_reduce(const double *M, const int n) {
    double min_element = 1000000000.0;
    for (int i = 0; i < n; ++i) {
        if (M[i] != 0) {
            min_element = M[i] < min_element ? M[i] : min_element;
        }
    }
    double sinus_sum = 0.0;
    for (int i = 0; i < n; ++i) {
        if ((int) floor(M[i] / min_element) % 2 == 0) {
            sinus_sum = sin(M[i]);
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
        double M1[N];
        double M2[N / 2];
        srand(i);
        unsigned int seed = i;
        do_generate(M1, M2, N, &seed);
        do_map(M1, M2, N);
        do_merge(M1, M2, N / 2);
        do_sort(M2, N / 2);
        double X = do_reduce(M2, N / 2);
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

