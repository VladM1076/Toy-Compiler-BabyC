# Toy-Compiler-BabyC
I created this toy compiler for my compiler constructions class, but I enjoyed making it so I am adding more features on my free time. As of now it creates ILOC (toy assembly) code for a simple C like language. For a detailed look at the language, consult the Baby.y file containing the grammer for BabyC. 

If you would like to run this yourself, download the repository as .zip, extract it, type `make` to build it, and then do `bcc tests/test1.bc` to run the program using provided example inputs (test1.bc - test8.bc). It will output the building of the AST tree to console and the ILOC code to test1.iloc. 

Features planned to be implemented: Local Register Allocation

*****DISCLAIMER*****
For some reason, using git clone <repo> breaks the make process. When cloning the repo and running `make` the build tries to call YACC instead of BISON, resulting in an error. This error does not happen when simply copying the source code.
I have no idea what causes this but I am trying to figure out and fix this issue.
