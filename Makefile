all: find_cycle.so

find_cycle.so: find_cycle.c
	gcc -o find_cycle.so -fPIC -shared -O2 find_cycle.c

clean:
	rm -f find_cycle.so
	find . | grep -E "(/__pycache__|\.pyc|\.pyo)" | xargs rm -rf
