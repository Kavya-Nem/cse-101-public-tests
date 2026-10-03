# Binary Search Tree Test Suite

Automated tests for a C++ `Dictionary` implementation and the `Words` program, with an emphasis on correctness, runtime, memory safety, and build hygiene.

## What is tested

The suite includes:

- `Words.cpp` + `Dictionary.cpp` compilation.
- Five `Words` input cases (`infile1.txt` through `infile5.txt`).
- Exact comparison against `model-outfile1.txt` through `model-outfile5.txt`.
- Runtime checks for each `Words` case.
- Valgrind checks for each `Words` case.
- A model `Dictionary` unit-test client in `ModelDictionaryTest.cpp`.
- A build/clean check using `make`.

The model dictionary client exercises operations such as insertion/removal, lookup, iteration, clearing, and current-key/current-value behavior.

## Scripts

| Script | Purpose |
|---|---|
| `main.sh` | Runs the complete binary-search-tree suite. |
| `binarysearchtree.sh` | Builds and tests `Words` against five reference cases, including timing and Valgrind. |
| `model-binarysearchtree-test.sh` | Compiles/runs `ModelDictionaryTest.cpp`, checks runtime, and runs Valgrind. |
| `binarysearchtree-build.sh` | Verifies that `make` creates `Words`/object files and that `make clean` removes them. |

## Running

From the project directory containing `Words.cpp` and `Dictionary.cpp`:

```bash
../low-latency-data-structures-tests/binarysearchtree/main.sh
```

An optional multiplier can be supplied to adjust the runtime limits:

```bash
../cse-101-public-tests/binarysearchtree/main.sh 2
```

Individual checks can also be run directly:

```bash
../cse-101-public-tests/binarysearchtree/binarysearchtree.sh
../cse-101-public-tests/binarysearchtree/model-binarysearchtree-test.sh
../cse-101-public-tests/binarysearchtree/binarysearchtree-build.sh
```

## Expected project files

The scripts expect the project being tested to provide at least:

```text
Words.cpp
Dictionary.cpp
Makefile
```

The build test expects `make` to produce an executable named `Words` and object files, and `make clean` to remove the generated executable/object files.

## Test data and generated files

Reference data lives in this directory:

```text
infile1.txt ... infile5.txt
model-outfile1.txt ... model-outfile5.txt
```

Running the tests may create local artifacts such as:

```text
outfile*.txt
diff*.txt
time*.txt
valgrind-out*.txt
DictionaryTest-out.txt
DictionaryTest-mem.txt
```

These files are diagnostics/results and are not reference outputs.

## Pass criteria

A `Words` case passes when it:

1. Compiles successfully.
2. Executes successfully within its configured runtime limit.
3. Produces output with no differences from the corresponding model output.
4. Passes the Valgrind check.

The model dictionary test additionally checks the dictionary API behavior and runtime/memory requirements.
