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
print(n_values)

with open(f'seqN/seqN.out', 'r') as file:
    values = []
    for line in file:
        seq_value.append(int(line.strip()))

# bar plot for time

# multiplier = 0
#
# with open('par2N/par2N.out', 'r') as file:
#     values = []
#     for line in file:
#         values.append(int(line.strip()))
#     x = start_bar_x + bar_width * multiplier
#     end_bar_x = x
#     plt.bar(x, values, width=bar_width, label='par-2(N)')
#     multiplier = multiplier + 1
#
#
# with open('fwM2/fwM2.out', 'r') as file:
#     values = []
#     for line in file:
#         values.append(int(line.strip()))
#     x = start_bar_x + bar_width * multiplier
#     end_bar_x = x
#     plt.bar(x, values, width=bar_width, label='fwM-2(N)')
#     multiplier = multiplier + 1
#
# with open('ompM4/ompM4.out', 'r') as file:
#     values = []
#     for line in file:
#         values.append(int(line.strip()))
#     x = start_bar_x + bar_width * multiplier
#     end_bar_x = x
#     plt.bar(x, values, width=bar_width, label='ompM-4(N)')
#     multiplier = multiplier + 1
#
#
# for core in range(1, cores_num + 1):
#     dir_name = f'omp2M{core}'
#     line_name = f'omp2M-{core}(N)'
#     with open(f'{dir_name}/{dir_name}.out', 'r') as file:
#         values = []
#         for line in file:
#             values.append(int(line.strip()))
#         x = start_bar_x + bar_width * multiplier
#         end_bar_x = x
#         plt.bar(x, values, width=bar_width, label=line_name)
#     multiplier = multiplier + 1
#
#
# ticks_pos = (end_bar_x + start_bar_x) / 2
#
# plt.xlabel("N")
# plt.ylabel('Время (мс)')
# plt.xticks(ticks_pos, n_values)
# plt.legend()
# plt.show()


# # bar plot for parallel speedup
#
# plt.axhline(y=1, color='red')
#
# multiplier = 0
#
# with open('par2N/par2N.out', 'r') as file:
#     values = []
#     for line in file:
#         values.append(int(line.strip()))
#     x = start_bar_x + bar_width * multiplier
#     end_bar_x = x
#     parallel_speedup = np.asarray(seq_value) / np.asarray(values)
#     plt.bar(x, parallel_speedup, width=bar_width, label='par-2(N)')
#     multiplier = multiplier + 1
#
#
# with open('fwM2/fwM2.out', 'r') as file:
#     values = []
#     for line in file:
#         values.append(int(line.strip()))
#     x = start_bar_x + bar_width * multiplier
#     end_bar_x = x
#     parallel_speedup = np.asarray(seq_value) / np.asarray(values)
#     plt.bar(x, parallel_speedup, width=bar_width, label='fwM-2(N)')
#     multiplier = multiplier + 1
#
# with open('ompM4/ompM4.out', 'r') as file:
#     values = []
#     for line in file:
#         values.append(int(line.strip()))
#     x = start_bar_x + bar_width * multiplier
#     end_bar_x = x
#     parallel_speedup = np.asarray(seq_value) / np.asarray(values)
#     plt.bar(x, parallel_speedup, width=bar_width, label='ompM-4(N)')
#     multiplier = multiplier + 1
#
# for core in range(1, cores_num + 1):
#     dir_name = f'omp2M{core}'
#     line_name = f'omp2M-{core}(N)'
#     with open(f'{dir_name}/{dir_name}.out', 'r') as file:
#         values = []
#         for line in file:
#             values.append(int(line.strip()))
#         x = start_bar_x + bar_width * multiplier
#         end_bar_x = x
#         parallel_speedup = np.asarray(seq_value) / np.asarray(values)
#         plt.bar(x, parallel_speedup, width=bar_width, label=line_name)
#     multiplier = multiplier + 1
#
#
# ticks_pos = (end_bar_x + start_bar_x) / 2
#
# plt.xlabel("N")
# plt.ylabel('Параллельное ускорение')
# plt.xticks(ticks_pos, n_values)
# plt.legend()
# plt.show()

# bar plot for parallel effectivity
#

multiplier = 0

plt.axhline(y=1, color='red')

with open('par2N/par2N.out', 'r') as file:
    values = []
    for line in file:
        values.append(int(line.strip()))
    x = start_bar_x
    end_bar_x = x
    parallel_speedup = np.asarray(seq_value) / (np.asarray(values) * 2)
    plt.bar(x, parallel_speedup, width=bar_width, label='par-2(N)')
    multiplier = multiplier + 1

with open('fwM2/fwM2.out', 'r') as file:
    values = []
    for line in file:
        values.append(int(line.strip()))
    x = start_bar_x + bar_width * multiplier
    end_bar_x = x
    parallel_speedup = np.asarray(seq_value) / (np.asarray(values) * 2)
    plt.bar(x, parallel_speedup, width=bar_width, label='fwM-2(N)')
    multiplier = multiplier + 1

with open('ompM4/ompM4.out', 'r') as file:
    values = []
    for line in file:
        values.append(int(line.strip()))
    x = start_bar_x + bar_width * multiplier
    end_bar_x = x
    parallel_speedup = np.asarray(seq_value) / (np.asarray(values) * 4)
    plt.bar(x, parallel_speedup, width=bar_width, label='ompM-4(N)')
    multiplier = multiplier + 1

for core in range(1, cores_num + 1):
    dir_name = f'omp2M{core}'
    line_name = f'omp2M-{core}(N)'
    with open(f'{dir_name}/{dir_name}.out', 'r') as file:
        values = []
        for line in file:
            values.append(int(line.strip()))
        x = start_bar_x + bar_width * multiplier
        end_bar_x = x
        parallel_speedup = np.asarray(seq_value) / (np.asarray(values) * core)
        plt.bar(x, parallel_speedup, width=bar_width, label=line_name)
    multiplier = multiplier + 1


ticks_pos = (end_bar_x + start_bar_x) / 2

plt.xlabel("N")
plt.ylabel('Параллельная эффективность')
plt.xticks(ticks_pos, n_values)
plt.legend()
plt.show()
