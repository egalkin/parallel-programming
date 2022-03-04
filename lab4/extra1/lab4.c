#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#ifdef _OPENMP
    #include <omp.h>
#else
    int omp_get_num_procs() { return 1; }

    void omp_set_num_threads (int threads_num) {}

    double omp_get_wtime() {
        struct timeval T;
        gettimeofday(&T, NULL);
        return ((double) T.tv_sec + (double) T.tv_usec / 1000000);
    }
#endif

// A = 240
// X = 1 + ((240 % 47) % B) = 1 + (5 % B)

const int A = 240;
const int EXPERIMENTS_NUMBER = 10;
const int STAGES = 5;
double percentage_step = (double) 100 / EXPERIMENTS_NUMBER / (STAGES - 1);
double work_percentage = 0.0;
int still_works = 1;

struct array_d {
    size_t size;
    double* restrict array;
};

void* display_work_percentage() {
    while (still_works) {
        printf("Percentage of work done: %.2f% \n", work_percentage);
        sleep(1);
    }
    printf("Percentage of work done: %.2f% \n", work_percentage);
    pthread_exit(NULL);
}


void do_generate_m1(struct array_d m1, const int experiment_number) {
    unsigned int seedM1;
    #pragma omp parallel default(none) shared(m1, experiment_number) private(seedM1)
    {
        seedM1 = 25234 + 17 + experiment_number;
        #pragma omp for
        for (int i = 0; i < m1.size; ++i) {
            m1.array[i] = (rand_r(&seedM1) % A) + 1;
        }
    }
}

void do_generate_m2(struct array_d m2, const int experiment_number) {
    unsigned int seedM2;
    #pragma omp parallel default(none) shared(m2, experiment_number) private(seedM2)
    {
        seedM2 = 25234 + 23 + experiment_number;
        #pragma omp for
        for (int i = 0; i < m2.size; ++i) {
            m2.array[i] = (rand_r(&seedM2) % (A * 9)) + A;
        }
    }
}

void do_generate(struct array_d m1, struct array_d m2, const int experiment_number) {

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            do_generate_m1(m1, experiment_number);
        }
        #pragma omp section
        {
            do_generate_m2(m2, experiment_number);
        }
    }
    work_percentage += percentage_step;
}

void do_map(struct array_d m1, struct array_d m2) {
    // ВариантM1 = 1 + (5 % 7) = 6 - Кубический корень после деления на число e
    #pragma omp parallel for default(none) shared(m1)
    for (int i = 0; i < m1.size; ++i) {
        double tmp = m1.array[i] / M_E;
        m1.array[i] = cbrt(tmp);
    }
    // ВариантM2 = 1 + (5 % 8) = 6 - Десятичный логарив, возведенный в степень e
    double* restrict temp_arr = malloc(m2.size * sizeof(double));
    #pragma omp parallel for default(none) shared(temp_arr, m2)
    for (int i = 0; i < m2.size; ++i) {
        temp_arr[i] = m2.array[i];
    }
    m2.array[0] = pow(log10(0 + temp_arr[0]), M_E);
    #pragma omp parallel for default(none) shared(temp_arr, m2)
    for (int i = 1; i < m2.size; ++i) {
        m2.array[i] = pow(log10(temp_arr[i - 1] + temp_arr[i]), M_E);
    }
    work_percentage += percentage_step;
    free(temp_arr);
}

void do_merge(struct array_d m1, struct array_d m2) {
    // Вариант = 1 + (5 % 6) = 6 = Модуль разности
    #pragma omp parallel for default(none) shared(m1, m2)
    for (int j = 0; j < m2.size; ++j) {
        m2.array[j] = fabs(m1.array[j] - m2.array[j]);
    }
}

void do_sort(struct array_d m, int start_idx, int end_idx) {
    // Вариант = 6 - Сортировка вставками
    // Реализация взята с https://www.geeksforgeeks.org/insertion-sort/
    int i, j;
    double key;
    for (i = start_idx + 1; i < end_idx; i++) {
        key = m.array[i];
        j = i - 1;
        while (j >= start_idx && m.array[j] > key) {
            m.array[j + 1] = m.array[j];
            j--;
        }
        m.array[j + 1] = key;
    }
}

void do_parallel_sort(struct array_d m) {
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            do_sort(m, 0, m.size / 2);
        }
        #pragma omp section
        {
            do_sort(m, m.size / 2, m.size);
        }
    }
    int idx, start1, start2, end1, end2;
    double* temp_arr = malloc(m.size * sizeof(double));
    idx = 0;
    start1 = 0;
    end1 = m.size / 2;
    start2 = m.size / 2;
    end2 = m.size;
//    for (int i = start1; i < end1; ++i) {
//        printf("%f ", m.array[i]);
//    }
//    printf("\n");
//    for (int i = start2; i < end2; ++i) {
//        printf("%f ", m.array[i]);
//    }
//    printf("\n");
    while (start1 < end1 && start2 < end2) {
        if (m.array[start1] < m.array[start2]) {
            temp_arr[idx] = m.array[start1];
            start1++;
        } else {
            temp_arr[idx] = m.array[start2];
            start2++;
        }
        idx++;
    }
    while (start1 < end1) {
        temp_arr[idx] = m.array[start1];
        start1++;
        idx++;
    }
    while (start2 < end2) {
        temp_arr[idx] = m.array[start2];
        start2++;
        idx++;
    }
    int sorted = 1;
    for (int i = 0; i < m.size; ++i) {
//        if (i > 0 && temp_arr[i] < temp_arr[i - 1]) {
//            sorted = 0;
//        }
        m.array[i] = temp_arr[i];
//        printf("%f ", m.array[i]);
    }
    free(temp_arr);
    work_percentage += percentage_step;
//    printf("\n");
//    if (!sorted) {
//        printf("AUCHTUNG!!!!!\n");
//    }
}

double do_reduce(struct array_d m) {
    if (m.size <= 0) {
        return 0.0;
    }
    double min_element = m.array[0];
    #pragma omp parallel default(none) shared(m, min_element)
    {
        double min_local = min_element;
        #pragma omp for
        for (int i = 1; i < m.size; ++i) {
            if (m.array[i] != 0 && m.array[i] < min_local) {
                min_local = m.array[i];
            }
        }
        #pragma omp critical
        {
            if (min_local < min_element) {
                min_element = min_local;
            }
        }
    }
    double sinus_sum = 0.0;
    #pragma omp parallel for default(none) shared(m, min_element) reduction(+:sinus_sum)
    for (int i = 0; i < m.size; ++i) {
        if ((int) floor(m.array[i] / min_element) % 2 == 0) {
            sinus_sum += sin(m.array[i]);
        }
    }
    work_percentage += percentage_step;
    return sinus_sum;
}

void write_execution_time_to_file(char* file_name, long execution_time) {
    FILE *out_file = fopen(file_name, "a+");
    if (out_file == NULL) {
        printf("Error! Could not open file\n");
        return;
    }
    fprintf(out_file, "%ld\n", execution_time);
}

void write_first_five_experiments_x(char* file_name, double* experiments_result, int N, size_t exp_num) {
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
    double start, end;
    long delta_ms;
    N = atoi(argv[1]); // N равен первому параметру командной строки

    pthread_t id;
    pthread_create(&id, NULL, display_work_percentage, NULL);

    if (N < 0) {
        return -1;
    }
    M = 0;
    if (argc >= 3) {
        M = atoi(argv[2]);
    }
    M = M != 0 ? M : 1;
    omp_set_num_threads(M);
    double experiments_result[5];
    struct array_d m1, m2;
    m1.size = N;
    m1.array = malloc(m1.size * sizeof(double));
    m2.size = N / 2;
    m2.array = malloc(m2.size * sizeof(double));
    start = omp_get_wtime();
    for (i = 0; i < EXPERIMENTS_NUMBER; ++i) {
        do_generate(m1, m2, i);
        do_map(m1, m2);
        do_merge(m1, m2);
        do_parallel_sort(m2);
        double X = do_reduce(m2);
        if (i < 5) {
            experiments_result[i] = X;
        }
        printf("Verification: X = %f\n", X);
    }
    end = omp_get_wtime();
    delta_ms = (long) ((end - start) * 1000);
    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms);
    if (argc >= 4) {
        write_execution_time_to_file(argv[3], delta_ms);
    }
    if (argc >= 5) {
        size_t experiments_size = sizeof(experiments_result)/sizeof(experiments_result[0]);
        write_first_five_experiments_x(argv[4], experiments_result, N, experiments_size);
    }
    free(m1.array);
    free(m2.array);
    still_works = 0;
    pthread_join(id, NULL);
    return 0;
}
