all:
	gcc -O3 -Wall -Werror -o lab1-seq lab1.c -\lm
	gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=1 lab1.c -o lab1-par-1 -lm
	gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 lab1.c -o lab1-par-4 -lm
	gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=8 lab1.c -o lab1-par-8 -lm
	gcc -O3 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=10 lab1.c -o lab1-par-10 -lm

clean:
	rm -rf lab1-*