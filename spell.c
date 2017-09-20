/* Author: Andrea Ortino
   Last Edit: 22/02/2017
   A spelling tool which compare words from an input file or command line, with a Dictionary.txt file containing words in alphabetical order.
*/

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUF_SIZE 100

/* typedef struct - contains an int storing the size of the dictionary, and a double pointer char which will be filled with dictionary words. 
   New types struct are defined for a less redundant code: tDictionary and a pointer to the struct *pDictionary. */

typedef struct
{
    int szDict;
    char **vecDict;
} tDictionary, *pDictionary; 
 
/* char *strip_newline(char *strWord) */
/* strWord - pointer to the string in which we want to strip newlines */
/* returns the same pointer as the argument, useful when we want to nest this function 
   This function iterate through the up to a \n or a \r, then it puts a null pointer, and returning the word. */
 
char *strip_newline(char *strWord) 
{
    int szLen = strlen(strWord);
 
    for(int k=0; k<szLen; k++)
    {
        if(strWord[k] == '\n' || strWord[k] == '\r')
            strWord[k] = '\0';
    }
    return strWord;
}
 
/* void parse_dictionary(pDictionary ptrDict, const char strDictPath[]) */
/* ptrDict - pointer to a tDictionary struct which will be filled with dictionary words */
/* strDictPath - char array containing the path to the dictionary file */
/* It gets the size of the dictionary, and the dictionary words trimmed by the strip_newline function.*/
 
void parse_dictionary(pDictionary ptrDict, const char strDictPath[]) 
{
    FILE *fpDict = fopen(strDictPath, "r");
	char strTmp[BUF_SIZE];
    int szDict = 0;
    int szWord = 0;
    int k=0;
	
	if(fpDict == NULL)
	{
		printf("Error! Could not open file %s." , strDictPath);
		exit(0);
	}
 
    while (fgets(strTmp, sizeof(strTmp), fpDict))
        szDict++;
 
    ptrDict->szDict = szDict;
    ptrDict->vecDict = (char **) malloc(szDict * sizeof(char *));
    printf("There are %d entries in %s\n", szDict, strDictPath);
 
    fseek(fpDict, 0, SEEK_SET);
 
    while (fgets(strTmp, sizeof(strTmp), fpDict))
    {
        szWord = strlen(strip_newline(strTmp));
        ptrDict->vecDict[k] = (char *)malloc(szWord+1);
        strncpy(ptrDict->vecDict[k], strTmp, szWord);
        k++;
    }
    fclose(fpDict);
}

/* bool query_dictionary(pDictionary ptrDict, const char *strWord, bool bCase) */
/* ptrDict - pointer to a tDictionary previously filled struct, which will be used to search for the provided word */
/* strWord - word to search inside the dictionary */
/* bCase - boolean defining search criteria (true for case sensitive search, false for case insensitive search */
/* returns true if the word was found (with specified criteria), false otherwise */
 
bool query_dictionary(pDictionary ptrDict, const char *strWord, bool bCase)
{
    for(int k=0; k<ptrDict->szDict; k++)
    {
        if (bCase && !strcmp(ptrDict->vecDict[k], strWord ) ) 
            return true;
		
        else if (!bCase && !stricmp(ptrDict->vecDict[k], strWord ) )
            return true;
    }
    return false;
}

/* void free_dictionary(pDictionary ptrDict) */
/* ptrDict - pointer to a tDictionary previously filled struct, which will be free from any allocated memory. */
 
void free_dictionary(pDictionary ptrDict)
{
    for (int k=0; k<ptrDict->szDict; k++)
			free(ptrDict->vecDict[k]);
		
    free(ptrDict->vecDict);
}

/* int load_file(const char *strFileName, char **ptrBuffer) */
/* strFileName - pointer to a string containing the file input name to load and check for errors. */
/* ptrBuffer - pointer to a pointer to string which will be filled with words from the input file. */
/* return an int which will be the size of the input text.*/

int load_file(const char *strFileName, char **ptrBuffer) 
{
    FILE *fpText = fopen(strFileName, "r");
    int szSize;
	
	if(fpText == NULL)
	{
		printf("Error! Could not open file %s." , strFileName);
		exit(0);
	}
 
    fseek(fpText, 0, SEEK_END);
    szSize = ftell(fpText);
    fseek(fpText, 0, SEEK_SET);
 
    *ptrBuffer = (char *) calloc(1, szSize+1); 
    fread(*ptrBuffer, 1, szSize, fpText); 
 
    fclose(fpText);
    return szSize; 
}

/* int load_stdin(const char **ptrBuffer) */
/* ptrBuffer - pointer to a pointer to char which will be filled with words from the command line. */
/* return an int which will be the length of the text.*/
 
int load_stdin(const char **ptrBuffer)
{
    char c;
    int szBufSize = BUF_SIZE;
    int k = 0;
 
    char *tmpBuf = (char *) malloc(szBufSize);
 
    while ((c = fgetc(stdin)) != '\n' )
    {
        if (k >= szBufSize)
        {
            szBufSize *= 2;
            tmpBuf = realloc(tmpBuf, szBufSize);
        }
        tmpBuf[k] = c;
		tmpBuf[k+1] = '\0';
        k++;
    }
	
    *ptrBuffer = tmpBuf;
    return k;
}

/* void check_file(char *ptrBuffer, int szSize, FILE *fpOut, pDictionary ptrDict, bool bCase) */
/* ptrBuffer - char pointer previously filled with words either from the command line or an input file. */
/* szSize - the size of the input text file or command line text, passed from the loading functions.*/
/* fpOut - pointer to a file output which will be stdout by default or a file txt if the tool is runned with the -o command.*/
/* ptrDict - pointer to a tDictionary previously filled struct, which will be free from any allocated memory. */
/* bCase - boolean defining search criteria (true for case sensitive search, false for case insensitive search 
   The function will first gather the word (following the bool checks)and then call for the query_dictionary to see if it matches with any word in the dictionary. */

void check_file(char *ptrBuffer, int szSize, FILE *fpOut, pDictionary ptrDict, bool bCase)
{
    char *pStart = NULL;
    bool bNextRow = false;
    bool bSentEnd = true;
    bool bWordEnd = false;
    bool bIgnoreCase = false;
    int iRow = 1;
 
    for (int k=0; k<=szSize; k++)
    {   
		if (pStart && ( bWordEnd || k == szSize) ) //handle last word in every case
        {
            if (!query_dictionary(ptrDict, pStart, bCase && !bIgnoreCase) )
                fprintf(fpOut, "Line %d: %s\n", iRow, pStart);
 
            pStart = NULL;
            bWordEnd = false;
            bIgnoreCase = false;
        }
		
        if (bSentEnd && bCase)
        {
            bSentEnd = false;
            bIgnoreCase = true;
        }
 
        if (bNextRow)
        {
            bNextRow = false;
            iRow++;
        }
	
        if ( (ptrBuffer[k] >= 'A' && ptrBuffer[k] <= 'Z') ||
                (ptrBuffer[k] >= 'a' && ptrBuffer[k] <= 'z') )
        {
            if (!pStart)
                pStart = ptrBuffer+k;
        }
 
        else
        {
            if (ptrBuffer[k] == '.' || ptrBuffer[k] == '?' || ptrBuffer[k] == '!' || ptrBuffer[k] == ':' || ptrBuffer[k] == ';' || ptrBuffer[k] == '\n')
                bSentEnd = true;
 
            if (ptrBuffer[k] == '\n')
                bNextRow = true;
 
            ptrBuffer[k] = '\0';
            if (pStart)
                bWordEnd = true;
        }
    }
}
 
int main(int argc, char *argv[])
{	
	clock_t begin = clock();
    const char strDictPath[] = "dictionary.txt";
    char *strInFile = NULL;
    char *strOutFile = NULL;
    FILE *fpOut = stdout;
    bool bCaseSensitive = true;
    tDictionary Dict; 
    char *ptrBuffer = NULL;
    int szSize = 0;
   
   for(int i = 0; i < argc; ++i)
    {
        if(!strcmp(argv[i], "-i"))
        {
            strInFile = argv[i+1];
        }
        if(!strcmp(argv[i], "-o"))
        {
            strOutFile = argv[i+1];
        }
        if(!strcmp(argv[i], "-c"))
        {
            bCaseSensitive = false;
        }
    }
	
    if (strInFile)
        szSize = load_file(strInFile, &ptrBuffer);
    else
        szSize = load_stdin(&ptrBuffer);
 
    if (strOutFile)
        fpOut = fopen(strOutFile, "w");
 
    parse_dictionary(&Dict, strDictPath);
    check_file(ptrBuffer, szSize, fpOut, &Dict, bCaseSensitive);
 
    if (strOutFile)
        fclose(fpOut);
 
    free(ptrBuffer);
    free_dictionary(&Dict);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\nExecuted in %f", time_spent);
    return 0;
}