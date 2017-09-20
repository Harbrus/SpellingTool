clean :
	del *.obj
	del *.exe

cl :
	cl spell.c
		
caseWords:
	spell -i single_words_test.txt -c

caseSent:
	spell -i sentences_test.txt -c

sent:
	spell -i sentences_test.txt

words:
	spell -i single_words_test.txt
	

