#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define WORD_LENGTH 50

void checkArgumentsValidity(int argc, char **argv)
{
    if(argc==1)
    {
        printf("Too few arguments!");
        exit(-1);
    }
    else if(argc>2)
    {
        printf("Too many arguments!");
        exit(-1);
    }
}

void checkFileValidity(FILE* file)
{
    if(file==NULL)
    {
        printf("Cannot open file!");
        exit(-1);
    }
}

int countLinesInFile(FILE *file)
{
    int linesCounter = 0;
    char c=0;

    for (c = getc(file); c != EOF; c = getc(file))
        if (c == '\n')
            ++linesCounter;

    return linesCounter;
}

void printWordWithFirstLetterRed(char* word)
{
    HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY );
    printf("%c", word[0]);
    SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
    printf("%s", word+1);
}

void drawRandomWord(FILE *file, int fileLength, char* result)
{
    char buffer[WORD_LENGTH];
    int randomLineNumber, i;

    randomLineNumber=rand()%fileLength;
    rewind(file);

    for(i=0; i<randomLineNumber; ++i)
        fgets(buffer, WORD_LENGTH, file);

    /* result := buffer */
    strcpy(result, buffer);

    rewind(file);
}

void drawRandomWordStartingWithCertainLetter(FILE *file, int fileLength, char letter, char* result)
{
    char buffer[WORD_LENGTH];
    int randomLineStartingWithCertainLetter=0;
    int i=0;
    int lowerBoundary=0;
    int upperBoundary=0;

    /* Finding out, from which line START words, that begins with variable "letter". */
    /* To sum up: finding lower boundary of the range */
    do
    {
        fgets(buffer, WORD_LENGTH, file);
        ++lowerBoundary;
    }
    while(buffer[0]!=letter);



    /* Finding out where in the file END words, that begins with variable "letter". */
    /* To sum up: finding upper boundary of the range */
    upperBoundary=lowerBoundary;
    do
    {
        fgets(buffer, WORD_LENGTH, file);
        ++upperBoundary;
    }
    while((buffer[0]!=(char)((int)(letter)+1))&&(upperBoundary!=fileLength-1));

    /* Random word from the range [lowerBoundary, upperBoundary] */
    randomLineStartingWithCertainLetter=rand()%(upperBoundary-lowerBoundary)+lowerBoundary;

    rewind(file);

    /* Acquiring line nr "randomLineStartingWithCertainLetter"  */
    for(i=0; i<randomLineStartingWithCertainLetter; ++i)
        fgets(buffer, WORD_LENGTH, file);

    rewind(file);

    /* result := buffer */
    strcpy(result, buffer);
}

void arrangeCrossword(FILE* file, int fileLength, char* coreWord)
{
    int i=0;
    char word[WORD_LENGTH];

    for(i=0; i<strlen(coreWord); ++i)
    {
        drawRandomWordStartingWithCertainLetter(file, fileLength, coreWord[i], word);
        printWordWithFirstLetterRed(word);
    }
}

int main(int argc, char **argv)
{
    FILE *file;
    char word[WORD_LENGTH];
    int fileLength;

    checkArgumentsValidity(argc, **argv);

    file = fopen(argv[1],"r");
    checkFileValidity(file);

    fileLength = countLinesInFile(file);

    /* Initializing pseudo-random number generator (PRNG)*/
    srand( (unsigned) time(NULL) * getpid() );

    /* Random word from file, which will be the core of the crossword*/
    drawRandomWord(file, fileLength, word);

    /* Arrange crossword, where core is variable "word"*/
    arrangeCrossword(file, fileLength, word);

    fclose(file);

    return 0;
}
