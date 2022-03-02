import numpy as np
import matplotlib.pyplot as plt

n1 = 1000
n2 = 40000
delta = (n2 - n1) / 10
n_values = [n1 + delta * i for i in range(0, 11)]
seq_value = []
cores_num = 4
plots_num = 5
start_point = 0 - plots_num / 2
start_bar_x = np.arange(len(n_values)) - start_point
end_bar_x = None
bar_width = 1 / plots_num / 2 + 0.05
print(n_values)

with open(f'seqN/seqN.out', 'r') as file:
    values = []
    for line in file:
        seq_value.append(int(line.strip()))

# bar plot for parallel speedup

plt.axhline(y=1, color='red')

multiplier = 0

with open('ompM4/ompM4.out', 'r') as file:
    values = []
    for line in file:
        values.append(int(line.strip()))
    x = start_bar_x + bar_width * multiplier
    end_bar_x = x
    parallel_speedup = np.asarray(seq_value) / np.asarray(values)
    plt.bar(x, parallel_speedup, width=bar_width, label='ompM-2(N)')
    multiplier = multiplier + 1

for filename, line_name in [('static/ompStatic4/ompStatic4.out', 'static-4(N)'),
                            ('dynamic/ompDynamic4/ompDynamic4.out', 'dynamic-4(N)'),
                            ('guided/ompGuided4/ompGuided4.out', 'guided-4(N)')
                            ]:
    with open(filename, 'r') as file:
        values = []
        for line in file:
            values.append(int(line.strip()))
        x = start_bar_x + bar_width * multiplier
        end_bar_x = x
        parallel_speedup = np.asarray(seq_value) / np.asarray(values)
        plt.bar(x, parallel_speedup, width=bar_width, label=line_name)
    multiplier = multiplier + 1

ticks_pos = (end_bar_x + start_bar_x) / 2

plt.xlabel("N")
plt.ylabel('Параллельное ускорение')
plt.xticks(ticks_pos, n_values)
plt.legend()
plt.show()
