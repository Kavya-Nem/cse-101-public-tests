//GPT LINK https://copilot.microsoft.com/shares/be8bLy3D6nMgFZDcRx6JC

//-----------------------------------------------------------------------------
// FileIO.c
// Finds the unique tokens contained in infile. Tokens are defined as the 
// maximal substrings of a line not containing any character in delim[]. 
// Unique tokens are printed to outfile in the order of first appearance in 
// infile. Upper and lower case chars are not distinguished. To distinguish 
// upper from lower, comment out the call to function lower()
//
// compile:       gcc -std=c17 -Wall -o FileIO FileIO.c
//
//     run:       FileIO file1 file2
//
// This program illustrates File input-output operations in C like fgets()
// and fprintf(). It also illustrates string handling functions in C, such 
// as strlen(), strcmp(), strcat() and strtok(), all in string.h, and also 
// heap memory operations such as malloc(), calloc(), realloc() and free()
// in stdlib.h.  
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "List.h"

// findWord()
// Returns true if char array buf contains word, false otherwise. Array buf 
// must consist of consecutive strings separated by the null '\0' character.
bool find(char* word, char** words, size_t n){
   bool next = false;
   int i;
   for( i=0; i<n; i++ ){
      if( strcmp(word, words[i])==0 ){
         next = true;
         break;
      }
   }
   return next;
}

// lower()
// Converts all chars in word to lower case.
void lower(char* word){
   int i;
   for ( i=0; word[i] != '\0'; i++) {
      word[i] = tolower( (unsigned char)word[i] );
   }
}

int main(int argc, char * argv[]){

   FILE *infile, *outfile;

   const size_t lineBufferSize = 5000;
   const size_t tokenBufferSizeIncrement = 1000;
   char lineBuffer[lineBufferSize];
   char* token;                       // a single token in lineBuffer
   // char* tokenBuffer;                 // concatenation of all tokens, separated by \0
   // size_t tokenBufferSize;            // current physical size of tokenBuffer
   // int tokenBufferNext;               // index of next open element in tokenBuffer
   int tokenCount;
   // int i;
   char delim[] = " \t\n\r\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
   // My code
   size_t arraySize = tokenBufferSizeIncrement;
   char **A = calloc(arraySize, sizeof(char*));
   List L = newList();
   //
   

   // check command line for correct number of arguments
   if( argc != 3 ){
      fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   // open files for reading and writing 
   infile  = fopen(argv[1], "r");
   outfile = fopen(argv[2], "w");
   if( infile==NULL ){
      fprintf(stderr, "Unable to open file %s for reading\n", argv[1]);
      exit(EXIT_FAILURE);
   }
   if( outfile==NULL ){
      fprintf(stderr, "Unable to open file %s for writing\n", argv[2]);
      exit(EXIT_FAILURE);
   }

   // read each line of infile, parse tokens, add unique tokens to tokenBuffer
   tokenCount = 0;
   // tokenBufferSize = tokenBufferSizeIncrement;
   // tokenBuffer = calloc(tokenBufferSize, sizeof(char));
   // tokenBufferNext = 0;
   while( fgets(lineBuffer, lineBufferSize, infile) != NULL ) {

      // get first token in lineBuffer
    token = strtok(lineBuffer, delim);
    
    while( token!=NULL ){ // a token exists
        
        // check if new
        if( find(token, A, tokenCount) ){  // already have it
            token = strtok(NULL, delim);              //    try another
            continue;
        }
        
        int arrayNextIndex = tokenCount; // “Where do I put the next word in the array?”
        // expand tokenBuffer if necessary
        if( arrayNextIndex >=  arraySize){  // #bytes needed > #bytes available
            arraySize += tokenBufferSizeIncrement;
            A = realloc(A, arraySize*sizeof(char*));
            // tokenBuffer = realloc(A, tokenBufferSize*sizeof(char*));
        }
        
        // Add to the array
        A[arrayNextIndex] = calloc( strlen(token)+1, sizeof(char));
        strcpy( A[arrayNextIndex], token);
        tokenCount++;
        
        append(L, arrayNextIndex);
        // get next token in lineBuffer
        token = strtok(NULL, delim);        
    }
    // End of Token Logic --------------------------------------------------------------------------------- (LOCATED:   while( fgets(lineBuffer, lineBufferSize, infile) != NULL ))
   }

   // append(L, 0); 

   // print to outfile
   moveFront(L);
   while(position(L) != -1) {
      int k = get(L);
      fprintf(outfile, "%s\n", A[k]);
      moveNext(L);
   }

   // // print tokens to outfile
   // token = tokenBuffer;
   // for( i=0; i<tokenCount; i++ ){
   //    fprintf(outfile, "%s\n", token);
   //    token += (strlen(token)+1);
   // }

   
   // free memory
   // free(tokenBuffer);
   freeList(&L);
   for (int i = 0; i < tokenCount; i++) {
      free(A[i]);
   }
   free(A);

   // close files 
   fclose(infile);
   fclose(outfile);

   return(0);
}