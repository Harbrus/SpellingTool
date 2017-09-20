SpellCheck Tool by Andrea Silvestro Ortino

Developer Command Prompt for VS2015 was used for compiling and running the tool. You can compile it by type "cl spell.c" (without the " ") and a file.exe will be created. The are several way to run the programs. The default mode is done by typing "spell.exe" and it will enquiry the text written in the command line.
However, it accept also the following command line arguments:

-i inputFileName = the name of the file to spell check.
-o outFileName = the name of the file to write the mispelt word and their line numbers to.
-c = indicate that the case of the words to be checked can be ignored.

Therefore we can spell check the file "inputFileName" using the command:

1) "spell -inputFileName" (The file must be in the same director)
2) "spell -i inputFileName -c" will do the same but -c specify an case insensitive search. 
3) "spell -i inputFileName -o outFileName" will write the output in the file with the provided name. If no output file is present in the directory the tool will create it for you. Also here -c can be added for a case insensitive search.

The rar contains also a makefile. The following shortcut will let you compile and run in different ways the tool:

1) "nmake cl" compiles the spell tool.
2) "nmake clean" deletes every .obj and .exe files in the directory.
3) "nmake words" does a case sensitive search of the file "single_words_test.txt".
4) "nmake caseWords" does a case insensitive search of the file "single_words_test.txt".
5) "nmake sent" does a case sensitive search of the file "sentences_test.txt".
6) "nmake caseSent" does a case insensitive search of the file "senteces_test.txt".

Remember to use the command without any quotation marks.