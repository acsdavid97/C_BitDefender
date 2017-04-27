This text file is about how to compile and run the code of this server client application.

Create a solution.
In the default visual studio project created with the solution, add the code of the server.

Add a new project to the solution and add the code of the client there.
For both projects modify the include folders, such that they include the INCLUDE folder from WSP4_Examples.

Build the solution.

In the debug folder you will have the executables, and some example files (cred.txt, log.log)
To test the application you can use asdf.txt, a copy of cred.txt.
To check the correctness of the program, encrypt a file with a key, and then ecrypt again with the same key. The resulting file should be the same as the original file.
