#define _GNU_SOURCE
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../pa5/Dictionary.h"

#define FIRST_TEST Empty_size
#define MAXSCORE 60
#define CHARITY 10

#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define NC "\033[0m"

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;

enum Test_e {
  Empty_size = 0,
  Insert_size,
  Overwrite_size,
  Contains_basic,
  GetValue_basic,
  RemoveKey_basic,
  Clear_basic,
  Copy_independence,
  Equals_basic,
  Stress_expand,
  Remove_many,
  Print_insert_order,
  NUM_TESTS
};

char *testName(int test) {
  switch (test) {
  case Empty_size: return "Empty_size";
  case Insert_size: return "Insert_size";
  case Overwrite_size: return "Overwrite_size";
  case Contains_basic: return "Contains_basic";
  case GetValue_basic: return "GetValue_basic";
  case RemoveKey_basic: return "RemoveKey_basic";
  case Clear_basic: return "Clear_basic";
  case Copy_independence: return "Copy_independence";
  case Equals_basic: return "Equals_basic";
  case Stress_expand: return "Stress_expand";
  case Remove_many: return "Remove_many";
  case Print_insert_order: return "Print_insert_order";
  default: return "";
  }
}

static bool expectValue(Dictionary D, const char *k, int expected) {
  if (!contains(D, k)) return false;
  return getValue(D, k) == expected;
}

// return 0 if pass otherwise the number of the test that was failed
uint8_t runTest(int test) {
  Dictionary A = newDictionary();
  Dictionary B = newDictionary();
  Dictionary C = NULL;
  uint8_t rc = 0;
  switch (test) {
  case Empty_size: {
    if (size(A) != 0) rc = 1;
    break;
  }
  case Insert_size: {
    setValue(A, "apple", 1);
    setValue(A, "banana", 2);
    setValue(A, "cherry", 3);
    if (size(A) != 3) rc = 1;
    break;
  }
  case Overwrite_size: {
    setValue(A, "k", 5);
    if (size(A) != 1) { rc = 1; break; }
    setValue(A, "k", 7); // overwrite
    if (size(A) != 1) { rc = 2; break; }
    if (!expectValue(A, "k", 7)) rc = 3;
    break;
  }
  case Contains_basic: {
    setValue(A, "one", 1);
    setValue(A, "two", 2);
    if (!contains(A, "one")) { rc = 1; break; }
    if (contains(A, "three")) rc = 2;
    break;
  }
  case GetValue_basic: {
    setValue(A, "alpha", 11);
    setValue(A, "beta", 22);
    setValue(A, "gamma", 33);
    if (!expectValue(A, "beta", 22)) { rc = 1; break; }
    setValue(A, "beta", 44);
    if (!expectValue(A, "beta", 44)) rc = 2;
    break;
  }
  case RemoveKey_basic: {
    setValue(A, "x", 9);
    setValue(A, "y", 8);
    if (size(A) != 2) { rc = 1; break; }
    removeKey(A, "x");
    if (size(A) != 1) { rc = 2; break; }
    if (contains(A, "x")) rc = 3;
    break;
  }
  case Clear_basic: {
    setValue(A, "a", 1);
    setValue(A, "b", 2);
    clear(A);
    if (size(A) != 0) { rc = 1; break; }
    setValue(A, "c", 3);
    if (size(A) != 1 || !contains(A, "c")) rc = 2;
    break;
  }
  case Copy_independence: {
    setValue(A, "foo", 10);
    setValue(A, "bar", 20);
    C = copy(A);
    if (size(C) != 2) { rc = 1; break; }
    setValue(A, "foo", 30); // modify original
    if (!expectValue(A, "foo", 30)) { rc = 2; break; }
    if (!expectValue(C, "foo", 10)) rc = 3; // copy should retain old value
    break;
  }
  case Equals_basic: {
    if (!equals(A, B)) { rc = 1; break; }
    setValue(A, "key", 1);
    if (equals(A, B)) { rc = 2; break; }
    setValue(B, "key", 1);
    if (!equals(A, B)) { rc = 3; break; }
    setValue(A, "key", 2);
    if (equals(A, B)) { rc = 4; break; }
    setValue(B, "key", 2);
    if (!equals(A, B)) rc = 5;
    break;
  }
  case Stress_expand: {
    // insert enough keys to force table expansion
    const int N = 2000;
    char keys[2000][16];
    for (int i = 0; i < N; i++) {
      snprintf(keys[i], 16, "k%04d", i);
      setValue(A, keys[i], i);
    }
    if (size(A) != N) rc = 1;
    if (!expectValue(A, keys[123], 123) || !expectValue(A, keys[1999], 1999)) rc = 2;
    break;
  }
  case Remove_many: {
    char keys[10][4];
    for (int i = 0; i < 10; i++) {
      snprintf(keys[i], sizeof keys[i], "k%d", i);
      setValue(A, keys[i], i);
    }
    // remove even keys
    for (int i = 0; i < 10; i += 2) {
      removeKey(A, keys[i]);
    }
    if (size(A) != 5) { rc = 1; }
    if (!rc) {
      // check that odd keys remain
      for (int i = 1; i < 10; i += 2) {
        if (!contains(A, keys[i])) { rc = 2; break; }
      }
    }
    if (!rc) {
      // should still be possible to indert after deletions and expansions
      setValue(A, "newKey", 42);
      if (!expectValue(A, "newKey", 42)) rc = 3;
    }
    break;
  }
  case Print_insert_order: {
    // checks that insertion order is preserved and skips deleted entries
    setValue(A, "one", 1);
    setValue(A, "two", 2);
    setValue(A, "three", 3);
    setValue(A, "four", 4);
    removeKey(A, "two");

    const char* expected = "one : 1\nthree : 3\nfour : 4\n";

    char* buf = NULL;
    size_t len = 0;
    int cmp = -1;

    FILE* mem = open_memstream(&buf, &len);
    printDictionary(mem, A);
    fclose(mem);

    cmp = strcmp(buf ? buf : "", expected);
    free(buf);
    buf = NULL;

    if (cmp != 0) rc = 1;
    break;
  }
  default: rc = 254; break;
  }

  if (C) freeDictionary(&C);
  freeDictionary(&A);
  freeDictionary(&B);
  return rc;
}

void segfault_handler(int signal) {
  testStatus = 255;
  longjmp(test_crash, 1);
}

void exit_attempt_handler(void) {
  if (disable_exit_handler) return;
  testStatus = 255;
  longjmp(test_crash, 2);
}

int main(int argc, char **argv) {
  if (argc > 2 || (argc == 2 && strcmp(argv[1], "-v") != 0)) {
    printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./DictionaryTest"));
    exit(1);
  }

  printf("\n");
  if (argc == 2) printf("\n");

  testsPassed = 0;
  disable_exit_handler = 0;
  atexit(exit_attempt_handler);
  signal(SIGSEGV, segfault_handler);

  for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {
    uint8_t fail_type = setjmp(test_crash);
    if (fail_type == 0) {
      testStatus = runTest(i);
    }
    if (argc == 2) {
      printf("Test %s: %s", testName(i), testStatus == 0 ? GREEN "PASSED" NC : RED "FAILED" NC);
      if (testStatus == 255) {
        printf(": due to a " RED "%s" NC "\n", fail_type == 1 ? "segfault" : fail_type == 2 ? "program exit" : "program interruption");
        printf(RED "\nWARNING: Program will now stop running tests\n\n" NC);
        break;
      } else if (testStatus == 254) {
        printf(": undefined test\n");
      } else if (testStatus != 0) {
        printf(": test" CYAN " %d\n" NC, testStatus);
      } else {
        printf("\n");
      }
    }
    if (testStatus == 0) testsPassed++;
  }

  disable_exit_handler = 1;
  uint8_t totalScore = (MAXSCORE - NUM_TESTS * 5) + testsPassed * 5;

  if (argc == 2) {
    if (testStatus == 255) {
      totalScore = CHARITY;
      printf(RED "Receiving charity points because your program crashes\n" NC);
    } else {
      printf("\nYou passed %d out of %d tests\n", testsPassed, NUM_TESTS);
    }
  }
  printf("\nYou will receive %d out of %d possible points on the DictionaryTest\n\n", totalScore, MAXSCORE);
  return 0;
}
