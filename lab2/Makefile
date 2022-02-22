all:
	cc -m64 -c -IFW_1.3.1_Lin64 -O3 -Wall -Werror lab2.c
	cc -m64 -LFW_1.3.1_Lin64/lib -o lab2-fw lab2.o -lfwSignal -lfwBase -lm
clean:
	rm -rf lab2-fw lab2.o