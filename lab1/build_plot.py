import numpy as np
import matplotlib.pyplot as plt

n1 = 10
n2 = 1000
delta = (n2 - n1) / 10
n_values = [n1 + delta * i for i in range(0, 11)]
seqValue = []
plots_num = 5
start_point = 0 - plots_num / 2
start_bar_x = np.arange(len(n_values)) - start_point
end_bar_x = None
bar_width = 0.1
print(n_values)

with open(f'seqN/seqN.out', 'r') as file:
    values = []
    for line in file:
        seqValue.append(int(line.strip()))

i = 1
for dir_name, line_name in [('seqN', 'seq(N)'),
                            ('par1N', 'par-1(N)'),
                            ('par2N', 'par-2(N)'),
                            ('par4N', 'par-4(N)'),
                            ('par8N', 'par-8(N)'),
                            ]:
    with open(f'{dir_name}/{dir_name}.out', 'r') as file:
        values = []
        for line in file:
            values.append(int(line.strip()))
        x = start_bar_x + bar_width * (i - 1)
        end_bar_x = x
        plt.bar(x, values, width=bar_width, label=line_name)
        i = i + 1

ticks_pos = (end_bar_x + start_bar_x) / 2

plt.xlabel("N")
plt.ylabel('Время (мс)')
plt.xticks(ticks_pos, n_values)
plt.legend()
plt.show()
