// GPTLINK https://copilot.microsoft.com/shares/be8bLy3D6nMgFZDcRx6JC

//-----------------------------------------------------------------------------
// QueueTest.c
// Another test client for Queue ADT
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"
// yellow New --------------------------------------------------------------------------
// typedef struct {
//    struct ListElement* movePrev;
//    char* data;
//    struct ListElement* moveNext;
// } ListElement;


typedef int ListElement;
typedef struct Node{
   ListElement data;
   struct Node* previousNode;
   struct Node* nextNode;
} Node;

typedef struct ListObj {
   Node* front;
   Node* back;
   Node* cursor;
   int length;
   int index;
} ListObj;

// newList()
// Creates a new empty list.
List newList() {
   List L = calloc(1, sizeof(ListObj));
   L->front = NULL;
   L->back = NULL;
   L->cursor = NULL;

   L->length = 0;
   L->index = -1;
   return L;
}
// freeList()
// Frees heap memory associated with *pL, sets *pL to NULL.
void freeList(List* pL) {
   if (pL == NULL || *pL == NULL) {
      return;
   }

   Node* current = (*pL)->front;
   while (current != NULL) {
      Node* next = current->nextNode;
      free (current);
      current = next;
   }
   free(*pL);
   *pL = NULL;
}
// Access functions -----------------------------------------------------------
// length()
// Returns the number of elements in L.
int length(List L) {
   return L->length;
}
// position()
// If cursor is defined, returns the position of the cursor element, otherwise
// returns -1.
int position(List L) {
   return L->index;
}
// front()
// Returns front element. Pre: length()>0
ListElement front(List L) {
   return L->front->data;
}
// back()
// Returns back element. Pre: length()>0
ListElement back(List L) {
   return L->back->data;
}
// get()
// Returns cursor element. Pre: length()>0, position()>=0
ListElement get(List L) {
   return L->cursor->data;
}
// equals()
// Returns true if A and B are the same integer sequence, false otherwise. The
// cursor is not altered in either List.
bool equals(List A, List B) {
   if (A == NULL || B == NULL || A->length != B->length) {
      return 0;
   } 
   Node* a = A->front;
   Node* b = B->front;

   while (a != NULL) {
      if (a->data != b->data) {
         return 0;
      }
      a = a->nextNode;
      b = b->nextNode;
   }

   return 1;
}
// Manipulation procedures ----------------------------------------------------
// clear()
// Resets L to its original empty state.
void clear(List L) {
   Node* N = L->front;
    while (N != NULL) {
        Node* next = N->nextNode;
        free(N);
        N = next;
    }

    L->front = NULL;
    L->back = NULL;
    L->cursor = NULL;
    L->index = -1;
    L->length = 0;
}
// set()
// Overwrites the cursor element’s data with x. Pre: length()>0, position()>=0
void set(List L, ListElement x) {

}
// moveFront()
// If L is non-empty, places the cursor under the front element, otherwise does
// nothing.
void moveFront(List L) {
   if (L != NULL) {
      L->cursor = L->front;
   }
}
// moveBack()
// If List is non-empty, places the cursor under the back element, otherwise
// does nothing.
void moveBack(List L) {
   if (L != NULL) {
      L->cursor = L->back;
   }
}
// movePrev()
// If cursor is defined and not at front, moves cursor one step toward front of
// L, if cursor is defined and at front, cursor becomes undefined, if cursor is
// undefined does nothing.
void movePrev(List L) {
   if ((L->cursor != L->front) && (L->cursor != -1)) {
      L->cursor = L->cursor->previousNode;
   }
   else if ((L->cursor == L->front) && (L->cursor != -1)){
      L->cursor = -1;
   }
   else {
      return;
   }
}
// moveNext()
// If cursor is defined and not at back, moves cursor one step toward back of
// L, if cursor is defined and at back, cursor becomes undefined, if cursor is
// undefined does nothing.
void moveNext(List L) {
   if (L->cursor != L->front && L->cursor != -1) {
      L->cursor = L->cursor->nextNode;
   }
   else if ((L->cursor == L->back) && (L->cursor != -1)){
      L->cursor = -1;
   }
   else {
      return;
   }
}
// prepend()
// Insert new element into L. If List is non-empty, insertion takes place
// before front element.
void prepend(List L, ListElement data) {
   Node* newNode = malloc(sizeof(Node));
   newNode->data = data;
   newNode->nextNode = NULL;
   newNode->previousNode = NULL;

   if (L->length == 0) {
      L->front = newNode;
      L->back = newNode;
    }
   else {
      newNode->nextNode = L->front;
      L->front->previousNode = newNode;
      L->front = newNode;
   }
   L->length++;
}
// append()
// Insert new element into L. If List is non-empty, insertion takes place
// after back element.
void append(List L, ListElement data) {
   Node* newNode = malloc(sizeof(Node));
   newNode->data = data;
   newNode->nextNode = NULL;
   newNode->previousNode = NULL;

   if (L->length == 0) {
      L->front = newNode;
      L->back = newNode;
    }
   else {
      newNode->previousNode = L->back;
      L->back->nextNode = newNode;
      L->back = newNode;
   }
   L->length++;
}
// insertBefore()
// Insert new element before cursor. Pre: length()>0, position()>=0
void insertBefore(List L, ListElement data) {
   Node* newNode = malloc(sizeof(Node));
   newNode->data = data;
   newNode->nextNode = NULL;
   newNode->previousNode = NULL;

   if (L->cursor == L->front) {
      free(newNode);   
      prepend(L, data);
      L->index++;      
      return;          
   }
   else {
      Node* C = L->cursor;
      Node* P = C->previousNode;
      
      newNode->previousNode = P;
      newNode->nextNode = C;
      P->nextNode = newNode;
      C->previousNode = newNode;
   }
   L->length++;
   L->index++;
}
// insertAfter()
// Inserts new element after cursor. Pre: length()>0, position()>=0
void insertAfter(List L, ListElement data) {
   Node* newNode = malloc(sizeof(Node));
   newNode->data = data;
   newNode->nextNode = NULL;
   newNode->previousNode = NULL;

   if (L->cursor == L->back) {
      free(newNode);   
      append(L, data);
      return;          
   }

   else {
      Node* C = L->cursor;
      Node* N = C->nextNode;
      
      newNode->previousNode = C;
      newNode->nextNode = N;
      C->nextNode = newNode;
      N->previousNode = newNode;

   }
   L->length++;
}
// deleteFront()
// Deletes the front element. Pre: length()>0
void deleteFront(List L) {

}
// deleteBack()
// Deletes the back element. Pre: length()>0
void deleteBack(List L) {

}
// delete()
// Deletes cursor element, making cursor undefined. Pre: length()>0, position()>=0
void delete(List L) {

}
// Other operations -----------------------------------------------------------
// printList()
// Prints a string representation of L consisting of a comma separated sequence
// of integers, surrounded by parentheses, with front on left, to the stream
// pointed to by out.
void printList(FILE* out, List L) {
   Node* N = L->front;
   while (N != NULL) {
   fprintf(out, "%d ", N->data);
   N = N->nextNode;
   }
}
// copyList()
// Returns a new List representing the same integer sequence as L. The cursor
// in the new list is undefined, regardless of the state of the cursor in L. The
// List L is unchanged.
List copyList(List L);
// join()
// Returns the concatenation of A followed by B. The cursor in the new List is
// undefined, regardless of the states of the cursors A in and B. The states of
// A and B are unchanged.
List join(List A, List B);
// split()
// Removes all elements before (in front of but not equal to) the cursor element
// in L. The cursor element in L is unchanged. Returns a new List consisting of
// all the removed elements. The cursor in the returned list is undefined.
// Pre: length(L)>0, position(L)>=0
List split(List L);

// typedef struct Node{
//    QueueElement data;
//    struct Node* next;
// } Node;
// yellow End Of New -------------------------------------------------------------------

// int main(int argc, char* argv[]){

//    int i;
//    Queue Q = newQueue();
//    Queue R = newQueue();

//    fprintf(stdout, "\n");

//    for(i=1; i<=10; i++){
//       Enqueue(Q, i);
//    }
//    printQueue(stdout, Q);
//    fprintf(stdout, "\n");

//    for(i=1; i<=5; i++){
//       Dequeue(Q);
//    }
//    printQueue(stdout, Q);
//    fprintf(stdout, "\n");

//    for(i=11; i<=15; i++){
//       Enqueue(Q, i);
//    }
//    printQueue(stdout, Q);
//    fprintf(stdout, "\n");

//    for(i=16; i<=19; i++){
//       Enqueue(Q, i);
//    }
//    printQueue(stdout, Q);
//    fprintf(stdout, "\n\n");

//    for(i=13; i<=19; i++){
//       Dequeue(Q);
//       Enqueue(R, i);
//    }
//    printQueue(stdout, Q);
//    fprintf(stdout, "\n");
//    printQueue(stdout, R);
//    fprintf(stdout, "\n");
//    fprintf(stdout, "Q %s R\n\n", equals(Q, R)?"equals":"does not equal");

//    Enqueue(Q, 20);
//    Enqueue(R, 200);
//    printQueue(stdout, Q);
//    fprintf(stdout, "\n");
//    printQueue(stdout, R);
//    fprintf(stdout, "\n");
//    fprintf(stdout, "Q %s R\n\n", equals(Q, R)?"equals":"does not equal");

//    freeQueue(&Q);
//    freeQueue(&R);
//    return(EXIT_SUCCESS);
// }
/* Output:

(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
(6, 7, 8, 9, 10)
(6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
(6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19)

(13, 14, 15, 16, 17, 18, 19)
(13, 14, 15, 16, 17, 18, 19)
Q equals R

(13, 14, 15, 16, 17, 18, 19, 20)
(13, 14, 15, 16, 17, 18, 19, 200)
Q does not equal R

*/