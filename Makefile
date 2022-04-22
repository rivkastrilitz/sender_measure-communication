
all: sender Measure

sender: sender.c
	gcc -o sender sender.c

Measure: Measure.c
	gcc -o Measure Measure.c

clean:
	rm -f *.o sender Measure file_send

runs:
	./sender

runm:
	./Measure

runs-strace:
	strace -f ./sender

runm-strace:
	strace -f ./Measure