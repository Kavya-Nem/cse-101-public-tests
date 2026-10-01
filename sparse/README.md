# Sparse Matrix Test Suite

Automated tests for a C sparse-matrix project. The suite tests the `Sparse` program and includes standalone model clients for the `List` and `Matrix` ADTs.

## What is tested

### `Sparse`

Five input cases are supplied:

```text
infile1.txt ... infile5.txt
```

The generated output is compared with:

```text
model-outfile1.txt ... model-outfile5.txt
```

The suite checks compilation, output correctness, runtime, and Valgrind memory behavior.

### `List`

`ModelListTest.c` is a standalone unit-test client for the List ADT. It checks operations including:

- Length.
- Append/prepend.
- Insert before/after.
- Front/back deletion.
- Position movement.
- Clearing.
- Accessors and front/back behavior.

The test runner reports individual tests and also checks runtime and memory behavior.

### `Matrix`

`ModelMatrixTest.c` is a standalone unit-test client for the Matrix ADT. It checks operations including:

- Dimension and non-zero counts.
- Entry changes and zeroing.
- Copy and transpose.
- Sum and difference.
- Scalar multiplication.
- Matrix product.
- Equality.

Runtime and Valgrind checks are also applied.

## Scripts

| Script | Purpose |
|---|---|
| `sparse.sh` | Runs the complete sparse suite. |
| `sparse-matrix.sh` | Builds/tests `Sparse` against five reference cases, including timing and Valgrind. |
| `model-list-test.sh` | Compiles/runs the List model client, with runtime and Valgrind checks. |
| `model-matrix-test.sh` | Compiles/runs the Matrix model client, with runtime and Valgrind checks. |
| `sparse-build.sh` | Verifies that `make` builds `Sparse` and that `make clean` removes generated files. |

## Running

From the project directory containing `Sparse.c`, `Matrix.c`, and `List.c`:

```bash
../cse-101-public-tests/sparse/sparse.sh
```

Individual checks:

```bash
../cse-101-public-tests/sparse/sparse-matrix.sh
../cse-101-public-tests/sparse/model-list-test.sh
../cse-101-public-tests/sparse/model-matrix-test.sh
../cse-101-public-tests/sparse/sparse-build.sh
```

## Expected project files

The scripts expect an implementation containing at least:

```text
Sparse.c
Matrix.c
Matrix.h
List.c
List.h
Makefile
```

The build check expects an executable named `Sparse` and object files to be produced by `make`, followed by their removal when `make clean` runs.

## Test data and diagnostics

Reference inputs and outputs are kept in this directory. Test execution can create:

```text
outfile*.txt
diff*.txt
time*.txt
Sparse-mem*.txt
ListTest-out.txt
ListTest-mem.txt
MatrixTest-out.txt
MatrixTest-mem.txt
listtime.txt
```

The model List/Matrix clients print the unit tests they execute and identify failed tests.

## Pass criteria

A `Sparse` case must compile, execute successfully within its runtime threshold, produce output matching the model output, and pass its Valgrind check.

The List and Matrix model tests must compile and run successfully, stay within their configured time limits, and pass Valgrind.
