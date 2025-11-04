#define _GNU_SOURCE
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expected.h"
#include "../../pa5/Dictionary.h"

#define FIRST_TEST Empty_diagnostic
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
  Empty_diagnostic = 0,
  Insert_diagnostic,
  Unique_keys_insertion,
  Lookup_overwrite,
  Remove_diagnostic,
  Deletion_recycling,
  Copy_independence,
  Equals_consistency,
  Stress_expand,
  Clear_basic,
  Print_insert_order,
  Expand_compact,
  NUM_TESTS
};

char *testName(int test) {
  switch (test) {
  case Empty_diagnostic: return "Empty_diagnostic";
  case Insert_diagnostic: return "Insert_diagnostic";
  case Unique_keys_insertion: return "Unique_keys_insertion";
  case Lookup_overwrite: return "Lookup_overwrite";
  case Remove_diagnostic: return "Remove_diagnostic";
  case Deletion_recycling: return "Deletion_recycling";
  case Copy_independence: return "Copy_independence";
  case Equals_consistency: return "Equals_consistency";
  case Stress_expand: return "Stress_expand";
  case Clear_basic: return "Clear_basic";
  case Print_insert_order: return "Print_insert_order";
  case Expand_compact: return "Expand_compact";
  default: return "";
  }
}

static bool expectValue(Dictionary D, const char *k, int expected) {
  if (!contains(D, k)) return false;
  return getValue(D, k) == expected;
}

static bool equalsIgnoreWhitespace(const char *lhs, const char *rhs) {
  if (!lhs) lhs = "";
  if (!rhs) rhs = "";
  while (true) {
    while (*lhs && isspace((unsigned char)*lhs)) lhs++;
    while (*rhs && isspace((unsigned char)*rhs)) rhs++;
    if (*lhs == '\0' || *rhs == '\0') return *lhs == '\0' && *rhs == '\0';
    if (*lhs != *rhs) return false;
    lhs++;
    rhs++;
  }
}

bool expectDiagnostic(Dictionary D, const char *expected) {
  char *buf = NULL;
  size_t len = 0;
  FILE *mem = open_memstream(&buf, &len);
  printDiagnostic(mem, D);
  fclose(mem);
  bool matches = equalsIgnoreWhitespace(buf, expected);
  free(buf);
  return matches;
}

// return 0 if pass otherwise the number of the test that was failed
uint8_t runTest(int test) {
  Dictionary A = newDictionary();
  Dictionary B = newDictionary();
  Dictionary C = NULL;
  uint8_t rc = 0;
  switch (test) {
  case Empty_diagnostic: {
    if (size(A) != 0) { rc = 1; break; }
    if (!expectDiagnostic(A, empty_expected)) rc = 2;
    break;
  }
  case Insert_diagnostic: {
    setValue(A, "apple", 1);
    setValue(A, "banana", 2);
    setValue(A, "cherry", 3);
    if (size(A) != 3) { rc = 1; break; }
    if (!expectDiagnostic(A, insert_expected)) rc = 2;
    break;
  }
  case Unique_keys_insertion: {
    // empty key
    setValue(A, "", 123);
    if (!expectValue(A, "", 123)) { rc = 1; break; }
  
    // long key (~1.5KB)
    char big[1600];
    memset(big, 'x', sizeof big - 1);
    big[sizeof big - 1] = '\0';
    setValue(A, big, 777);
    if (!expectValue(A, big, 777)) rc = 2;

    char tmp[8]; 
    strcpy(tmp, "dup");
    setValue(A, tmp, 7);
    if (size(A) != 3 || !expectValue(A, "dup", 7)) rc = 3;
    break;
  }
  case Lookup_overwrite: {
    setValue(A, "one", 1);
    setValue(A, "two", 2);
    setValue(A, "three", 3);
    if (size(A) != 3) { rc = 1; break; }
    if (!contains(A, "one") || !expectValue(A, "one", 1)) { rc = 2; break; }
    if (contains(A, "four")) { rc = 3; break; }
    if (!expectDiagnostic(A, lookup1_expected)) { rc = 4; break; }
    setValue(A, "two", 22);
    if (size(A) != 3) { rc = 5; break; }
    if (!expectValue(A, "two", 22)) { rc = 6; break; }
    setValue(A, "four", 44);
    if (size(A) != 4) { rc = 7; break; }
    if (!expectValue(A, "four", 44) || !contains(A, "four")) { rc = 8; break; }
    if (!expectDiagnostic(A, lookup2_expected)) rc = 9;
    break;
  }
  case Remove_diagnostic: {
    const char *keys[] = {"one", "two", "three"};
    for (int i = 0; i < 3; i++) {
      setValue(A, keys[i], i + 1);
    }
    removeKey(A, "one");
    removeKey(A, "three");
    if (contains(A, "one") || contains(A, "three")) { rc = 1; break; }
    if (size(A) != 1) { rc = 2; break; }
    if (!expectValue(A, "two", 2)) { rc = 3; break; }
    if (!expectDiagnostic(A, remove_expected)) { rc = 4; break; }
    setValue(A, "one", 10);
    if (!expectValue(A, "one", 10)) { rc = 5; break; }
    if (size(A) != 2) { rc = 5; break; }
    setValue(A, "three", 30);
    if (!expectValue(A, "three", 30)) rc = 6;
    break;
  }
  case Deletion_recycling: {
    const int BATCH = 256;
    const int ROUNDS = 3;
    char tkeys[BATCH][16];
    char ukeys[ROUNDS][BATCH][24];
  
    for (int i = 0; i < BATCH; i++) {
      snprintf(tkeys[i], sizeof tkeys[i], "t%03d", i);
      setValue(A, tkeys[i], i);
    }
    for (int i = 0; i < BATCH; i += 2) {
      removeKey(A, tkeys[i]);
    }
  
    int survivors = BATCH / 2;
    int total_newcomers = 0;
    int total_unique_reinserts = 0;
  
    // Make sure no overlap between rounds
    const int R0[] = {  8,  12,  24,  36,  44,  52,  60,  72 };
    const int R1[] = { 100, 104, 112, 124, 132, 144, 156, 168 };
    const int R2[] = { 200, 204, 212, 220, 224, 228, 232, 236 };
  
    const int* Rset[3] = { R0, R1, R2 };
    const int  Rcnt[3] = { 8, 8, 8 };
  
    // Rounds: insert newcomers and reinsert a distinct subset each round
    for (int r = 0; r < ROUNDS; r++) {
      for (int i = 0; i < BATCH; i++) {
        snprintf(ukeys[r][i], sizeof ukeys[r][i], "u%d_%03d", r, i);
        setValue(A, ukeys[r][i], 10000 + r*1000 + i);
      }
      total_newcomers += BATCH;
      for (int i = 0; i < BATCH; i += 37) {
        if (!expectValue(A, ukeys[r][i], 10000 + r*1000 + i)) { rc = 1; break; }
      }
      if (rc) break;
  
      // Reinsert the round’s even t-keys with new values
      for (int k = 0; k < Rcnt[r]; k++) {
        int idx = Rset[r][k];
        setValue(A, tkeys[idx], 7000 + r*100 + k);
      }
      total_unique_reinserts += Rcnt[r];
  
      for (int k = 0; k < Rcnt[r]; k++) {
        int idx = Rset[r][k];
        if (!expectValue(A, tkeys[idx], 7000 + r*100 + k)) { rc = 2; break; }
      }
      if (rc) break;
  
      for (int i = 1; i < BATCH; i += 17) {
        if ((i % 2) == 0) continue;
        if (!contains(A, tkeys[i])) { rc = 3; break; }
      }
      if (rc) break;
    }
    if (rc) break;
  
    int expected = survivors + total_newcomers + total_unique_reinserts;
    if (size(A) != expected) { rc = 4; break; }
  
    // Pick a couple of evens not in any R-set
    int should_be_gone[] = { 2, 14, 26, 58, 190, 250 };
    for (int i = 0; i < 6; i++) {
      if (contains(A, tkeys[should_be_gone[i]])) { rc = 5; break; }
    }
    if (rc) break;
  
    // Check newcomers from the last round
    for (int i = 5; i < BATCH; i += 51) {
      if (!expectValue(A, ukeys[ROUNDS-1][i], 10000 + (ROUNDS-1)*1000 + i)) { rc = 6; break; }
    }
  
    break;
  }
  case Copy_independence: {
    setValue(A, "10", 10);
    setValue(A, "20", 20);
    setValue(A, "30", 30);
    C = copy(A);
    if (size(C) != 3) { rc = 1; break; }
    setValue(A, "10", 100);
    removeKey(A, "20");
    setValue(A, "new", 500);
    if (!expectValue(A, "10", 100)) { rc = 2; break; }
    if (contains(A, "20")) { rc = 3; break; }
    if (contains(C, "new")) { rc = 4; break; }
    if (!expectValue(C, "20", 20) || !expectValue(C, "30", 30)) { rc = 5; break; }
    removeKey(C, "30");
    if (!expectValue(A, "30", 30)) { rc = 6; break; }
    if (contains(C, "30")) { rc = 7; break; }
    if (size(A) != 3) { rc = 8; break; }
    if (size(C) != 2) rc = 9;
    break;
  }
  case Equals_consistency: {
    const char *seqA[] = {"one", "two", "three", "four"};
    for (int i = 0; i < 4; i++) setValue(A, seqA[i], i + 1);
    
    if (!equals(A, A)) { rc = 1; break; }
    setValue(B, "three", 3);
    setValue(B, "one", 1);
    setValue(B, "four", 4);
    setValue(B, "two", 2);
    if (!equals(A, B) || !equals(B, A)) { rc = 2; break; }
    setValue(A, "five", 5);
    if (equals(A, B)) { rc = 3; break; }
    setValue(B, "five", 5);
    if (!equals(A, B)) { rc = 4; break; }
    removeKey(B, "two");
    if (equals(A, B)) { rc = 5; break; }
    removeKey(A, "two");
    if (!equals(A, B)) rc = 6;
    break;
  }
  case Stress_expand: {
    const int N = 4096;
    char keys[4096][16];
    for (int i = 0; i < N; i++) {
      snprintf(keys[i], sizeof keys[i], "k%05d", i);
      setValue(A, keys[i], i);
    }
    if (size(A) != N) rc = 1;
    if (rc) break;
    int sampleIdx[] = {0, 57, 1023, 2048, N - 1};
    for (size_t i = 0; i < sizeof(sampleIdx)/sizeof(sampleIdx[0]); i++) {
      int idx = sampleIdx[i];
      if (!expectValue(A, keys[idx], idx)) { rc = 2; break; }
    }
    if (rc) break;
    int removed = 0;
    for (int i = 0; i < N; i += 97) {
      removeKey(A, keys[i]);
      removed++;
    }
    if (size(A) != N - removed) { rc = 3; break; }
    for (int i = 0; i < N; i += 97) {
      if (contains(A, keys[i])) { rc = 4; break; }
    }
    if (rc) break;
    if (!expectValue(A, keys[58], 58) || !expectValue(A, keys[2050], 2050)) rc = 5;
    break;
  }
  case Clear_basic: {
    const int total = 40;
    char keys[total][8];
    for (int i = 0; i < total; i++) {
      snprintf(keys[i], sizeof keys[i], "k%d", i);
      setValue(A, keys[i], i);
    }
    if (size(A) != total) { rc = 1; break; }

    clear(A);
    if (size(A) != 0) { rc = 2; break; }
    for (int i = 0; i < total; i++) {
      if (contains(A, keys[i])) { rc = 3; break; }
    }
    if (rc) break;

    clear(A);
    if (size(A) != 0) { rc = 4; break; }

    for (int i = 0; i < total; i += 2) {
      setValue(A, keys[i], i * 10);
      if (!expectValue(A, keys[i], i * 10)) { rc = 5; break; }
    }
    if (rc) break;

    int expectedSize = (total + 1) / 2;
    if (size(A) != expectedSize) { rc = 6; break; }
    for (int i = 1; i < total; i += 2) {
      if (contains(A, keys[i])) { rc = 7; break; }
    }
    break;
  }
  case Print_insert_order: {
    const char *keys[] = {"one", "two", "three", "four", "five"};
    for (int i = 0; i < 5; i++) setValue(A, keys[i], i + 1);
    
    removeKey(A, "two");
    removeKey(A, "four");
    setValue(A, "two", 22);
    setValue(A, "six", 6);

    const char *expected = "one : 1\nthree : 3\nfive : 5\ntwo : 22\nsix : 6\n";
    char *buf = NULL;
    size_t len = 0;
    FILE *mem = open_memstream(&buf, &len);
    printDictionary(mem, A);
    fclose(mem);

    int cmp = strcmp(buf ? buf : "", expected);
    free(buf);
    if (cmp != 0) rc = 1;
    break;
  }
  case Expand_compact: {
    char keys[16][6];
    for (int i = 0; i < 16; i++) {
      snprintf(keys[i], sizeof keys[i], "k%02d", i);
      setValue(A, keys[i], i);
    }
    if (!expectDiagnostic(A, expand1_expected)) { rc = 1; break; }

    for (int i = 0; i < 12; i++) removeKey(A, keys[i]);
    if (!expectDiagnostic(A, expand2_expected)) { rc = 2; break; }
    // numPairs=4, numDeleted=12
    // dataDensity = 0.25

    char newKeys[18][6];
    for (int i = 0; i < 18; i++) {
      snprintf(newKeys[i], sizeof newKeys[i], "n%02d", i);
      setValue(A, newKeys[i], i + 100);
    }
    // numPairs=22, numDeleted=12
    // tableLoadFactor = 22/32 approx 0.69 > 0.67
    // dataDensity = 22/(22+12) approx 0.65 > 0.8
    // so compactify should happen
    
    if (!expectDiagnostic(A, compact_expected)) rc = 3;
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
