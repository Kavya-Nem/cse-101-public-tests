/****************************************************************************************
*  ListClient.c
*  Test client for List ADT
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"

List convolution(List A, List B){
   List C = newList();
   int a = length(A);
   int b = length(B);
   int i, n = a+b-1;

   for( i=0; i<n; i++ ){
      append(C, 0);
   }
   moveFront(C);
   for( moveFront(A); position(A)>=0; moveNext(A) ){
      for( i=0; i<position(A); i++ ){
         moveNext(C);
      }
      for( moveFront(B); position(B)>=0; moveNext(B) ){
         set( C, get(C)+get(A)*get(B) );
         moveNext(C);
      }
      moveFront(C);
   }

   return C;
}

void reverse(List L){
   moveBack(L);
   while( position(L)>=0 ){
      insertAfter(L, front(L));
      deleteFront(L);
   }
}

int main(int argc, char* argv[]){
   int i;
   
   int a = 6;
   int dataA[] = {3, 8, 12, -7, 0, 4};
   List A = newList();
   for( i=0; i<a; i++ ) append(A, dataA[i]);

   int b = 4;
   int dataB[] = {5, 4, -2, -1};
   List B = newList();
   for( i=0; i<b; i++ ) append(B, dataB[i]);


   List C = convolution(A, B);

   fprintf(stdout, "\n");
   printList(stdout, A);
   fprintf(stdout, "\n");
   printList(stdout, B);
   fprintf(stdout, "\n");
   printList(stdout, C);
   fprintf(stdout, "\n\n");

   List D = copyList(C);
   reverse(C);

   printList(stdout, C);
   fprintf(stdout, "\n");
   printList(stdout, D);
   fprintf(stdout, "\n\n");

   List E = join(C, D);

   printList(stdout, E);
   fprintf(stdout, "\n\n");

   moveFront(E);
   while( position(E)<length(E)/3 ){
      moveNext(E);
   }
   List F = split(E);

   printList(stdout, F);
   fprintf(stdout, "\n");
   printList(stdout, E);
   fprintf(stdout, "\n\n");

   freeList(&A);
   freeList(&B);
   freeList(&C);
   freeList(&D);
   freeList(&E);
   freeList(&F);

   return(EXIT_SUCCESS);
}

/*  Program output:

(3, 8, 12, -7, 0, 4)
(5, 4, -2, -1)
(15, 52, 86, -6, -60, 22, 23, -8, -4)

(-4, -8, 23, 22, -60, -6, 86, 52, 15)
(15, 52, 86, -6, -60, 22, 23, -8, -4)

(-4, -8, 23, 22, -60, -6, 86, 52, 15, 15, 52, 86, -6, -60, 22, 23, -8, -4)

(-4, -8, 23, 22, -60, -6)
(86, 52, 15, 15, 52, 86, -6, -60, 22, 23, -8, -4)

*/
 