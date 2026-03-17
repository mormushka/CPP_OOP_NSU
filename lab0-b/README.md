# Word counter

This program counts the words in the specified txt file and outputs the result to the specified csv file in the form of a table:
1) Word
2) Frequency
3) Frequency as a percentage

## Example for use

`./word_count input.txt output.csv`

## Valgrind's memcheck result:

==960== Memcheck, a memory error detector
==960== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==960== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==960== Command: ./lab0-b input.txt output.csv
==960== Parent PID: 852
==960== 
==960== 
==960== HEAP SUMMARY:
==960==     in use at exit: 0 bytes in 0 blocks
==960==   total heap usage: 31,782 allocs, 31,782 frees, 134,135,023 bytes allocated
==960== 
==960== All heap blocks were freed -- no leaks are possible
==960== 
==960== For lists of detected and suppressed errors, rerun with: -s
==960== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
