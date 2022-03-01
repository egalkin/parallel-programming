import numpy as np
import matplotlib.pyplot as plt

n1 = 1000
n2 = 40000
delta = (n2 - n1) / 10
n_values = [n1 + delta * i for i in range(0, 11)]
seq_value = []
min_cores_num = 401
max_cores_num = 404
plots_num = 5
start_point = 0 - plots_num / 2
start_bar_x = np.arange(len(n_values)) - start_point
end_bar_x = None
bar_width = 1 / plots_num / 2 + 0.05
print(n_values)

# fig, ax = plt.subplots()

with open(f'seqN/seqN.out', 'r') as file:
    values = []
    for line in file:
        seq_value.append(int(line.strip()))

# bar plot for parallel speedup

plt.axhline(y=1, color='red')

with open('par2N/par2N.out', 'r') as file:
    values = []
    for line in file:
        values.append(int(line.strip()))
    x = start_bar_x
    end_bar_x = x
    parallel_speedup = np.asarray(seq_value) / np.asarray(values)
    plt.bar(x, parallel_speedup, width=bar_width, label='par-2(N)')

for core in range(min_cores_num, max_cores_num + 1):
    dir_name = f'fwM{core}'
    line_name = f'fwM-{core}(N)'
    with open(f'{dir_name}/{dir_name}.out', 'r') as file:
        values = []
        for line in file:
            values.append(int(line.strip()))
        x = start_bar_x + bar_width * (core - min_cores_num + 1)
        end_bar_x = x
        parallel_speedup = np.asarray(seq_value) / np.asarray(values)
        plt.bar(x, parallel_speedup, width=bar_width, label=line_name)

ticks_pos = (end_bar_x + start_bar_x) / 2

plt.xlabel("N")
plt.ylabel('Параллельное ускорение')
plt.xticks(ticks_pos, n_values)
plt.legend()
plt.show()
