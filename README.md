# Toy-Compiler-BabyC
I created this toy compiler for my compiler constructions class, but I enjoyed making it so I am added more features on my free time. As of now it creates ILOC (toy assembly) code for a simple C like language. For a detailed look at the language, consult the Baby.y file containing the grammer for BabyC. If you would like to run this yourself, simply clone the repository, type make to build it, and then do bcc CodeFile.bc to run the program. It will output the building of the AST tree to console and the ILOC code to test.iloc. Features planned to be implemented: Local Register Allocation