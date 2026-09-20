#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../pa4/List.h"
#define FIRST_TEST Empty_length
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define NC "\033[0m"
static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;
enum Test_e {
  Empty_length = 0,
  Append_length,
  Prepend_length,
  InsertAfter_length,
  InsertBefore_length,
  DeleteFront_length,
  DeleteBack_length,
  Delete_length,
  EmptyList_position,
  MoveFront_position,
  MoveBack_position,
  MoveNext_position,
  MovePrev_position,
  Append_position,
  Prepend_position,
  InsertAfter_position,
  InsertBefore_position,
  DeleteFront_position,
  DeleteBack_position,
  Delete_position,
  Empty_clear,
  NonEmpty_clear,
  Set_get,
  Set_front,
  NonEmpty_front,
  Set_back,
  NonEmpty_back,
  NUM_TESTS,
};
char *testName(int test) {
  if (test == Empty_length)
    return "Empty_length";
  if (test == Append_length)
    return "Append_length";
  if (test == Prepend_length)
    return "Prepend_length";
  if (test == InsertAfter_length)
    return "InsertAfter_length";
  if (test == InsertBefore_length)
    return "InsertBefore_length";
  if (test == DeleteFront_length)
    return "DeleteFront_length";
  if (test == DeleteBack_length)
    return "DeleteBack_length";
  if (test == Delete_length)
    return "Delete_length";
  if (test == EmptyList_position)
    return "EmptyList_position";
  if (test == MoveFront_position)
    return "MoveFront_position";
  if (test == MoveBack_position)
    return "MoveBack_position";
  if (test == MoveNext_position)
    return "MoveNext_position";
  if (test == MovePrev_position)
    return "MovePrev_position";
  if (test == Append_position)
    return "Append_position";
  if (test == Prepend_position)
    return "Prepend_position";
  if (test == InsertAfter_position)
    return "InsertAfter_position";
  if (test == InsertBefore_position)
    return "InsertBefore_position";
  if (test == DeleteFront_position)
    return "DeleteFront_position";
  if (test == DeleteBack_position)
    return "DeleteBack_position";
  if (test == Delete_position)
    return "Delete_position";
  if (test == Empty_clear)
    return "Empty_clear";
  if (test == NonEmpty_clear)
    return "NonEmpty_clear";
  if (test == Set_get)
    return "Set_get";
  if (test == Set_front)
    return "Set_front";
  if (test == NonEmpty_front)
    return "NonEmpty_front";
  if (test == Set_back)
    return "Set_back";
  if (test == NonEmpty_back)
    return "NonEmpty_back";
  return "";
}
int *newData(int data) {
  int *N = malloc(sizeof(int));
  *N = data;
  return N;
}
uint8_t runTest(List *pA, int test) {
  List A = *pA;
  switch (test) {
  case Empty_length: {
    if (length(A) != 0)
      return 1;
    return 0;
  }
  case Append_length: {
    int *B = newData(1);
    int *C = newData(2);
    int *D = newData(3);
    int *E = newData(4);
    append(A, B);
    append(A, C);
    append(A, D);
    append(A, E);
    if (length(A) != 4)
      free(B);
      free(C);
      free(D);
      free(E);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    return 0;
  }
  case Prepend_length: {
    int *B = newData(6);
    int *C = newData(4);
    int *D = newData(2);
    int *E = newData(1);
    prepend(A, B);
    prepend(A, C);
    prepend(A, D);
    prepend(A, E);
    if (length(A) != 4)
      free(B);
      free(C);
      free(D);
      free(E);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    return 0;
  }
  case InsertAfter_length: {
    int *B = newData(1);
    int *C = newData(2);
    int *D = newData(3);
    int *E = newData(5);
    int *F = newData(12);
    append(A, B);
    append(A, C);
    append(A, D);
    append(A, E);
    moveFront(A);
    insertAfter(A, F);
    if (length(A) != 5)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    return 0;
  }
  case InsertBefore_length: {
    int *B = newData(76);
    int *C = newData(4);
    int *D = newData(3);
    int *E = newData(1);
    int *F = newData(100);
    prepend(A, B);
    prepend(A, C);
    prepend(A, D);
    prepend(A, E);
    moveFront(A);
    insertBefore(A, F);
    if (length(A) != 5)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    return 0;
  }
  case DeleteFront_length: {
    int *B = newData(76);
    int *C = newData(4);
    int *D = newData(3);
    int *E = newData(1);
    int *F = newData(115);
    prepend(A, B);
    prepend(A, C);
    deleteFront(A);
    prepend(A, D);
    prepend(A, E);
    moveFront(A);
    insertBefore(A, F);
    deleteFront(A);
    if (length(A) != 3)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    return 0;
  }
  case DeleteBack_length: {
    int *B = newData(1);
    int *C = newData(2);
    int *D = newData(3);
    int *E = newData(5);
    int *F = newData(12);
    append(A, B);
    deleteBack(A);
    append(A, C);
    append(A, D);
    append(A, E);
    moveFront(A);
    insertAfter(A, F);
    deleteBack(A);
    if (length(A) != 3)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    return 0;
  }
  case Delete_length: {
    int *B = newData(1);
    int *C = newData(2);
    int *D = newData(3);
    int *E = newData(5);
    int *F = newData(12);
    append(A, B);
    append(A, C);
    moveFront(A);
    delete (A);
    append(A, D);
    append(A, E);
    moveFront(A);
    insertAfter(A, F);
    delete (A);
    if (length(A) != 3)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    return 0;
  }
  case EmptyList_position: {
    if (position(A) != -1)
      return 1;
    return 0;
  }
  case MoveFront_position: {
    int *B = newData(1);
    int *C = newData(5);
    int *D = newData(16);
    int *E = newData(176);
    int *F = newData(3214);
    append(A, B);
    append(A, C);
    append(A, D);
    append(A, E);
    append(A, F);
    moveFront(A);
    if (position(A) != 0)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    return 0;
  }
  case MoveBack_position: {
    int *B = newData(1);
    int *C = newData(5);
    int *D = newData(16);
    int *E = newData(176);
    int *F = newData(3214);
    append(A, B);
    append(A, C);
    append(A, D);
    append(A, E);
    append(A, F);
    moveBack(A);
    if (position(A) != 4)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    return 0;
  }
  case MoveNext_position: {
    int *B = newData(1);
    int *C = newData(5);
    int *D = newData(16);
    int *E = newData(176);
    int *F = newData(3214);
    append(A, B);
    append(A, C);
    append(A, D);
    append(A, E);
    append(A, F);
    moveFront(A);
    moveNext(A);
    moveNext(A);
    if (position(A) != 2)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    moveNext(A);
    moveNext(A);
    moveNext(A);
    if (position(A) != -1)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 2;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    return 0;
  }
  case MovePrev_position: {
    int *B = newData(1);
    int *C = newData(5);
    int *D = newData(3214);
    append(A, B);
    append(A, C);
    append(A, D);
    moveBack(A);
    movePrev(A);
    if (position(A) != 1)
      free(B);
      free(C);
      free(D);
      return 1;
    movePrev(A);
    movePrev(A);
    if (position(A) != -1)
      free(B);
      free(C);
      free(D);
      return 2;
    free(B);
    free(C);
    free(D);
    return 0;
  }
  case Append_position: {
    int *B = newData(1);
    int *C = newData(5);
    int *D = newData(7);
    int *E = newData(45);
    int *F = newData(51);
    int *G = newData(3214);
    append(A, B);
    append(A, C);
    append(A, D);
    moveBack(A);
    append(A, E);
    append(A, F);
    append(A, G);
    if (position(A) != 2)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 1;
    moveBack(A);
    movePrev(A);
    movePrev(A);
    if (position(A) != 3)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 2;
    moveFront(A);
    movePrev(A);
    if (position(A) != -1)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 3;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    free(G);
    return 0;
  }
  case Prepend_position: {
    int *B = newData(1);
    int *C = newData(5);
    int *D = newData(7);
    int *E = newData(45);
    int *F = newData(51);
    int *G = newData(3214);
    int *H = newData(314);
    int *I = newData(324);
    int *J = newData(234);
    prepend(A, B);
    prepend(A, C);
    prepend(A, D);
    moveFront(A);
    prepend(A, E);
    prepend(A, F);
    prepend(A, G);
    prepend(A, H);
    prepend(A, I);
    if (position(A) != 5)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      free(H);
      free(I);
      return 1;
    moveBack(A);
    movePrev(A);
    prepend(A, J);
    movePrev(A);
    if (position(A) != 6)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      free(H);
      free(I);
      free(J);
      return 2;
    moveFront(A);
    movePrev(A);
    if (position(A) != -1)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      free(H);
      free(I);
      free(J);
      return 3;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    free(G);
    free(H);
    free(I);
    free(J);
    return 0;
  }
  case InsertAfter_position: {
    int *B = newData(5);
    int *C = newData(6);
    int *D = newData(4);
    int *E = newData(33);
    int *F = newData(2);
    int *G = newData(1);
    int *H = newData(75);
    int *I = newData(345);
    append(A, B);
    append(A, C);
    append(A, D);
    append(A, E);
    append(A, F);
    append(A, G);
    moveBack(A);
    insertAfter(A, H);
    moveNext(A);
    if (position(A) != 6)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      free(H);
      return 1;
    insertAfter(A, I);
    moveBack(A);
    if (position(A) != 7)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      free(H);
      free(I);
      return 2;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    free(G);
    free(H);
    free(I);
    return 0;
  }
  case InsertBefore_position: {
    int *B = newData(34);
    int *C = newData(4);
    int *D = newData(354);
    int *E = newData(3674);
    int *F = newData(435);
    int *G = newData(324);
    int *H = newData(33464);
    int *I = newData(3498);
    int *J = newData(67);
    prepend(A, B);
    prepend(A, C);
    prepend(A, D);
    prepend(A, E);
    moveBack(A);
    insertBefore(A, F);
    if (position(A) != 4)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      return 1;
    prepend(A, G);
    prepend(A, H);
    prepend(A, I);
    moveFront(A);
    insertBefore(A, J);
    if (position(A) != 1)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      free(H);
      free(I);
      free(J);
      return 2;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    free(G);
    free(H);
    free(I);
    free(J);
    return 0;
  }
  case DeleteFront_position: {
    int *B = newData(5);
    int *C = newData(65);
    int *D = newData(43);
    int *E = newData(2);
    int *F = newData(8);
    int *G = newData(1);
    prepend(A, B);
    prepend(A, C);
    prepend(A, D);
    prepend(A, E);
    prepend(A, F);
    prepend(A, G);
    moveFront(A);
    deleteFront(A);
    if (position(A) != -1)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 1;
    moveBack(A);
    deleteFront(A);
    if (position(A) != 3)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 2;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    free(G);
    return 0;
  }
  case DeleteBack_position: {
    int *B = newData(5);
    int *C = newData(65);
    int *D = newData(43);
    int *E = newData(2);
    int *F = newData(8);
    int *G = newData(1);
    prepend(A, B);
    prepend(A, C);
    prepend(A, D);
    prepend(A, E);
    prepend(A, F);
    prepend(A, G);
    moveBack(A);
    deleteBack(A);
    if (position(A) != -1)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 1;
    moveFront(A);
    deleteBack(A);
    moveNext(A);
    if (position(A) != 1)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 2;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    free(G);
    return 0;
  }
  case Delete_position: {
    int *B = newData(5);
    int *C = newData(65);
    int *D = newData(43);
    int *E = newData(2);
    int *F = newData(8);
    int *G = newData(1);
    prepend(A, B);
    prepend(A, C);
    prepend(A, D);
    moveBack(A);
    delete (A);
    if (position(A) != -1)
      free(B);
      free(C);
      free(D);
      return 1;
    prepend(A, E);
    prepend(A, F);
    prepend(A, G);
    moveBack(A);
    if (position(A) != 4)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 2;
    delete (A);
    moveBack(A);
    if (position(A) != 3)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 3;
    moveFront(A);
    delete (A);
    moveFront(A);
    if (position(A) != 0)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 4;
    delete (A);
    if (position(A) != -1)
      free(B);
      free(C);
      free(D);
      free(E);
      free(F);
      free(G);
      return 5;
    free(B);
    free(C);
    free(D);
    free(E);
    free(F);
    free(G);
    return 0;
  }
  case Empty_clear: {
    clear(A);
    if (position(A) != -1 || length(A) != 0)
      return 1;
    return 0;
  }
  case NonEmpty_clear: {
    int *B = newData(1);
    int *C = newData(2);
    append(A, B);
    prepend(A, C);
    moveFront(A);
    clear(A);
    if (position(A) != -1 || length(A) != 0)
      free(B);
      free(C);
      return 1;
    free(B);
    free(C);
    return 0;
  }
  case Set_get: {
    append(A, newData(1));
    prepend(A, newData(2));
    deleteFront(A);
    moveBack(A);
    if (*(int *)get(A) != 1)
      return 1;
    return 0;
  }
  case Set_front: {
    append(A, newData(1));
    prepend(A, newData(5));
    moveBack(A);
    if (*(int *)front(A) != 5)
      return 1;
    return 0;
  }
  case NonEmpty_front: {
    prepend(A, newData(5));
    append(A, newData(7));
    prepend(A, newData(2));
    moveFront(A);
    insertBefore(A, newData(43));
    deleteFront(A);
    delete (A);
    if (*(int *)front(A) != 5)
      return 1;
    return 0;
  }
  case Set_back: {
    prepend(A, newData(1));
    append(A, newData(5));
    moveFront(A);
    if (*(int *)back(A) != 5)
      return 1;
    return 0;
  }
  case NonEmpty_back: {
    append(A, newData(5));
    prepend(A, newData(7));
    append(A, newData(2));
    moveBack(A);
    insertAfter(A, newData(43));
    deleteBack(A);
    delete (A);
    if (*(int *)back(A) != 5)
      return 1;
    return 0;
  }
  }
  return 255;
}
void segfault_handler(int signal) { // everyone knows what this is
  testStatus = 255;
  longjmp(test_crash, 1);
}
void exit_attempt_handler(void) { // only I decide when you are done
  if (disable_exit_handler)
    return; // allow this to be disabled
  testStatus = 255;
  longjmp(test_crash, 2);
}
void abrupt_termination_handler(int signal) { // program killed externally
  testStatus = 255;
  longjmp(test_crash, 3);
}
int main(int argc, char **argv) {
  if (argc > 2 || (argc == 2 && strcmp(argv[1], "-v") != 0)) {
    printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./ListTest"));
    exit(1);
  }
  if (argc == 2)
    printf("\n"); // consistency in verbose mode
  testsPassed = 0;
  disable_exit_handler = 0;
  atexit(exit_attempt_handler);
  signal(SIGSEGV, segfault_handler);
  for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {
    List A = newList();
    testStatus = runTest(&A, i);
    freeList(&A);
    uint8_t fail_type = setjmp(test_crash);
    if (argc == 2) { // it's verbose mode
      printf("Test %s: %s", testName(i),
             testStatus == 0 ? GREEN "PASSED" NC : RED "FAILED" NC);
      if (testStatus == 255) {
        printf(": due to a " RED "%s" NC "\n", fail_type == 1 ? "segfault"
                                               : fail_type == 2
                                                   ? "program exit"
                                                   : "program interruption");
        printf(RED "\nWARNING: Program will now stop running tests\n\n" NC);
        break;
      } else if (testStatus != 0) {
        printf(": test" CYAN " %d\n" NC, testStatus);
      } else {
        printf("\n");
      }
    }
    if (testStatus == 0) {
      testsPassed++;
    }
  }
  disable_exit_handler = 1;
  if (argc == 2 && testStatus != 255)
    printf("\nYou passed %d out of %d tests\n", testsPassed, NUM_TESTS); 
  exit(NUM_TESTS - testsPassed);
}
