import matplotlib.pyplot as plt

n1 = 1000
n2 = 40000
delta = (n2 - n1) / 10
n_values = [n1 + delta * i for i in range(0, 11)]
print(n_values)

for dir_name, line_name in [('fwM1', 'fwM-1(N)'),
                            ('fwM2', 'fwM-2(N)'),
                            ('fwM5', "fwM-5(N)"),
                            ]:
    with open(f'{dir_name}/{dir_name}.out', 'r') as file:
        values = []
        for line in file:
            values.append(int(line.strip()))
        plt.plot(n_values, values, label=line_name)

plt.xlabel("N")
plt.ylabel('Время (мс)')
plt.legend()
plt.show()
