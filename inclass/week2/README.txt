- Lecture 0906 - Linux commands

sum.s from command 'gcc -S sum.c'
sum.o from command 'gcc -c sum.c'
sum_disassembled.txt from command 'objdump -d sum.o > sum_disassembled.txt'

- Lecture 0908 - Processes

There are 6 examples of using fork(). To select examplem, compile with defining FORKFN to corresponding integer. Range of FORKFN is 0 < FORKFN <= 6.

ex) To select example function 'fork3()', command would be 'gcc forks.c -o forks.out -DFORKFN=3 && ./forks.out'
