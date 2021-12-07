# Work-17-Semaphone-

Compilation:
make

Execution:

./control -c
Starts the story and sets up the semaphores

./write
Displays the previous line and requests the next line

./control -r
Displays the full story and removes the shared memory and the semaphore

Features:
Clearly marked where the story starts and stops
./write returns Bad File Descriptor when called before ./control -c
./control with any argument other than -c and -r will return a warning with instructions

Oddities:
An unpredictable string will be printed as the previous line the first time ./write is run