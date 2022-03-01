import matplotlib.pyplot as plt

n1 = 1000
n2 = 40000
delta = (n2 - n1) / 10
n_values = [n1 + delta * i for i in range(0, 11)]
seqValue = []
print(n_values)

with open(f'seqN/seqN.out', 'r') as file:
    values = []
    for line in file:
        seqValue.append(int(line.strip()))

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
        parallel_speedup = []
        for i in range(len(seqValue)):
            parallel_speedup.append(seqValue[i] / values[i])
        print(parallel_speedup)
        plt.plot(n_values, values, label=line_name)
        # plt.plot(n_values, parallel_speedup, label='ps(N)', color='red')


plt.xlabel("N")
plt.ylabel('Время (мс)')
# plt.ylabel('Параллельное ускорение')
plt.legend()
plt.show()
