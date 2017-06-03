## Opitimizing Compiler 

This is an optimizing compiler written in C targeted to SPARC assembly.

A number of compiler optimizations are implemented.  They can be found in the opts directory.

To build the optimizer, run `make` in the opts directory.

There are a number of assembly files to optimize in the `test` directory.  To optimize them, compile them, and run them to gather results, execute the `checkallres` script in the test directory.
