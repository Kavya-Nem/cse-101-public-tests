# Hash Table Test Suite

Automated tests for a C `Dictionary` implementation and the `WordFrequency` program. The suite checks compilation, functional output, runtime, memory safety, and build/clean behavior.

## What is tested

The suite includes:

- `WordFrequency.c` + `Dictionary.c` compilation with C17.
- Five `WordFrequency` input cases.
- Comparison against five reference output files.
- Runtime limits for each case.
- Valgrind checks for each case.
- A model dictionary test client in `ModelDictionaryTest.c`.
- A build/clean check using `make`.

The model dictionary client covers dictionary operations including insertion/removal, lookup, iteration, clearing, and current-entry behavior.

## Scripts

| Script | Purpose |
|---|---|
| `main.sh` | Runs the complete hash-table suite. |
| `hashtable.sh` | Builds and tests `WordFrequency` against five reference cases, including timing and Valgrind. |
| `model-hashtable-test.sh` | Compiles/runs `ModelDictionaryTest.c`, checks runtime, and runs Valgrind. |
| `hashtable-build.sh` | Verifies that `make` builds `WordFrequency` and that `make clean` removes generated files. |

## Running

From the project directory containing `WordFrequency.c` and `Dictionary.c`:

```bash
../cse-101-public-tests/hashtable/main.sh
```

Individual checks can be run directly:

```bash
../cse-101-public-tests/hashtable/hashtable.sh
../cse-101-public-tests/hashtable/model-hashtable-test.sh
../cse-101-public-tests/hashtable/hashtable-build.sh
```

## Expected project files

The scripts expect the project being tested to provide at least:

```text
WordFrequency.c
Dictionary.c
Makefile
```

The build check expects `make` to create an executable named `WordFrequency` and object files, followed by successful cleanup with `make clean`.

## Test data

The suite contains:

```text
infile1.txt ... infile5.txt
model-outfile1.txt ... model-outfile5.txt
```

The test runner writes generated output files such as `out1.txt` through `out5.txt` and diagnostic files such as `diff*.txt`, `time*.txt`, and `valgrind-out*.txt`.

## Pass criteria

Each `WordFrequency` case must:

1. Compile successfully.
2. Finish within the configured runtime limit.
3. Exit successfully.
4. Match the corresponding model output.
5. Pass the Valgrind memory check.

The model dictionary test must also compile, execute successfully, satisfy its runtime threshold, and pass Valgrind.
