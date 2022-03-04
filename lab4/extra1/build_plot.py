import numpy as np
import matplotlib.pyplot as plt

n1 = 75
n2 = 40000
delta = int((n2 - n1) / 10)
n_values = [n1 + delta * i for i in range(0, 11)]
seq_value = []
cores_num = 4
plots_num = 5
start_point = 0 - plots_num / 2
start_bar_x = np.arange(len(n_values)) - start_point
end_bar_x = None
bar_width = 1 / plots_num / 2
repeat_range = 10
confidence_coefficient = 1.96

print(n_values)

with open(f'seqN/seqN.out', 'r') as file:
    values = []
    for line in file:
        val = int(line.strip())
        seq_value.append(1 if val == 0 else val)

# bar plot for time

multiplier = 0

# # bar plot for parallel speedup
#
plt.axhline(y=1, color='red')

multiplier = 0


for core in range(1, cores_num + 1):
    dir_name = f'omp2M{core}'
    min_line_name = f'min-{core}(N)'
    ci_upper_line_name = f'upperCI-{core}(N)'
    ci_lower_line_name = f'lowerCI-{core}(N)'
    with open(f'{dir_name}/{dir_name}.out', 'r') as file:
        values = []
        for line in file:
            values.append(int(line.strip()))

        # Take min value in window of 10
        counter = 0
        range_min = 0
        min_values = []
        ci_window = []
        ci_upper_values = []
        ci_lower_values = []
        for i in range(0, len(values)):
            if counter == 0:
                range_min = values[i]
            range_min = min(range_min, values[i])
            ci_window.append(values[i])
            counter += 1
            if counter % repeat_range == 0:
                min_values.append(1 if range_min == 0 else range_min)
                std_deviation = np.std(ci_window)
                error_limit = confidence_coefficient * (std_deviation / math.sqrt(len(ci_window)))
                window_mean = np.mean(ci_window)
                ci_upper_values.append(1 if window_mean + error_limit <= 1 else window_mean + error_limit)
                ci_lower_values.append(1 if window_mean - error_limit <= 1 else window_mean - error_limit)
                ci_window = []
                counter = 0

        x_min = start_bar_x + bar_width * multiplier
        multiplier = multiplier + 1
        x_ci = start_bar_x + bar_width * multiplier
        multiplier = multiplier + 1
        end_bar_x = x_ci
        x_parallel_speedup = np.asarray(seq_value) / np.asarray(min_values)
        ci_upper_parallel_speedup = np.asarray(seq_value) / np.asarray(ci_upper_values)
        ci_lower_parallel_speedup = np.asarray(seq_value) / np.asarray(ci_lower_values)
        plt.bar(x_min, x_parallel_speedup, width=bar_width, label=min_line_name)
        plt.bar(x_ci, ci_upper_parallel_speedup, width=bar_width, label=ci_upper_line_name)
        # plt.bar(x_ci, ci_lower_parallel_speedup, width=bar_width, label=ci_lower_line_name)



ticks_pos = (end_bar_x + start_bar_x) / 2

plt.xlabel("N")
plt.ylabel('Параллельное ускорение')
plt.xticks(ticks_pos, n_values)
plt.legend()
plt.show()
