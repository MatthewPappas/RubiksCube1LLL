Rubiks Cube 3x3x3 One Look Last Layer (1LLL) Algorithm Generator.

Copyright 2020 by Matthew Pappas, all rights reserved.

test.exe [options]

Options:

  -f [file.txt]      process input file, scan for 'Scramble:' before proceeding.
  
  -r                 combinations are generated and tried randomly.
  
  -r3,12             same as -r except we choose random number 0-3 and add 12 for number of moves.
  
                     note: F2 counts as 1 move in this application.
                     
  -m[number of threads]
  
                     choose number of threads to create 1 is the default.
                     
                     note: application creates numbered folders per thread for which to append its output data,
                     
                           however, only thread 1 will output data, so only 1 folder will be created.
                           
                           Also use max #of virtual cores - 2 for system stability.
                           
  -k                 -f by itself, without -r, will simply read input.txt, then recreate the output
  
                     files in folder 1. Adding -k instead of -r means to proceed incrementally, not randomly,
                     
                     thus using -start and -stop or their defaults.
                     
  -start 0 0 0       for incremental search (not random) start at this algorithm.
  
  -stop 15 15 15     for incremental search (not random) stop at this algorithm.
  
                     note: application shouldn't need more than 30 digits but I believe 60 are supported.
                     
                     use this for -start and -stop, numerical mapping to moves:
                     
                     0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
                     
                     F  F' F2 B  B' B2 L  L' L2 R  R' R2 U  U' U2 D  D' D2
                     
  -aU -aU' -ax -ax' -ay -ay' -az -az'
  
                     Using one of these options says you are researching the output folders and want to
                     
                     perform a change to an algorithm, example:
                     
                     test.exe -aU "R U F U' F' R'"
                     
                     Output: F U L U'L'F'
                     
                     And yes, more will be added later.
                     
Examples:

  test.exe -stop 17 17 17 17 17 17
  
  type 1\* > input.txt
  
  test.exe -f input.txt -k -stop 17 17 17 17 17 17 1
  
  test.exe -f input.txt -k
  
  test.exe -f input.txt -r
  
  test.exe -m3 -f input.txt -r
  
  type 1\* > input.txt
  
  test.exe -f input.txt
  
  It takes 4-5 minutes on an i3 to generate all 16416 1LLLs with 1 thread,
  
  however it takes time and lots of CPUs to shorten the algorithms,
  
  merging all the data daily and restarting.
  
  Improvements to come, thank you for playing.
  

