all: control write

control: control.o
	gcc -o control control.o

write: write.o
	gcc -o write write.o

write.o: write.c
	gcc -c write.c

control.o: control.c
	gcc -c control.c

clean:
	rm control.o
	rm control
	rm write.o
	rm write
	rm *.txt