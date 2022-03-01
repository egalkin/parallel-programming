all:
	cc -m64 -c -I${FW_HOME} -O3 -Wall -Werror lab2.c
	cc -m64 -L${FW_HOME}/lib -o lab2-fw lab2.o -lfwSignal -lfwBase -lm
clean:
	rm -rf lab2-fw lab2.o