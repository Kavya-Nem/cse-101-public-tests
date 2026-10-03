# Low-Latency Data Structures Tests

A collection of automated test and performance harnesses for data-structure projects. The suites exercise implementations of dictionaries, word-processing programs, sparse matrices, and list/matrix ADTs.

## Test suites

| Directory | Target programs / ADTs | Language | Main coverage |
|---|---|---|---|
| [`binarysearchtree/`](binarysearchtree/) | `Words`, `Dictionary` | C++17 | Functional output, performance, memory checks, build/clean checks |
| [`hashtable/`](hashtable/) | `WordFrequency`, `Dictionary` | C17 | Functional output, performance, memory checks, build/clean checks |
| [`redblacktree/`](redblacktree/) | `Words`, `WordFrequency`, `Dictionary` | C++17 | Functional output, performance, memory checks, build/clean checks |
| [`sparse/`](sparse/) | `Sparse`, `List`, `Matrix` | C17 | Functional output, performance, memory checks, unit tests, build/clean checks |

## How the harness works

The repository contains **tests and reference data**, not the implementations being tested. The shell scripts expect to be run from the corresponding project directory containing the implementation files.

Each suite generally does some combination of:

- Compiling the implementation with `gcc` or `g++`.
- Running supplied input files against the implementation.
- Comparing generated output with reference `model-outfile*.txt` files using `diff`.
- Measuring user CPU time with `/usr/bin/time`.
- Enforcing time limits with `timeout`.
- Checking for memory errors/leaks with `valgrind`.
- Building and then cleaning the expected executable/object files.

The scripts use relative paths such as `../low-latency-data-structures-tests/binarysearchtree`. In the original test environment, this means the test repository is expected to be available at:

```text
../low-latency-data-structures-tests/
├── binarysearchtree/
├── hashtable/
├── redblacktree/
└── sparse/
```

If you move these directories elsewhere, update `RELATIVE_PATH` in the shell scripts accordingly.

## Requirements

Typical dependencies are:

- Bash
- `gcc` / `g++`
- `make`
- `diff`
- `timeout`
- `/usr/bin/time`
- `bc`
- `valgrind`

The exact compiler standard varies by suite:
- C suites use C17.
- C++ suites use C++17.

## Running a suite

From a project directory that contains the implementation files and the relevant test directory is available at the expected relative path:

```bash
cd <your-project>
../low-latency-data-structures-tests/<suite>/main.sh
```

The `main.sh` scripts run the suite's individual checks and return a non-zero status if one or more checks fail.

Some individual scripts accept an optional numeric multiplier that adjusts runtime limits. For example:

```bash
../cse-101-public-tests/binarysearchtree/main.sh 2
```

Refer to the README in each suite for the specific commands and generated artifacts.

## Repository layout

```text
low-latency-data-structures-tests-main/
├── README.md
├── binarysearchtree/
├── hashtable/
├── redblacktree/
└── sparse/
```

Reference input/output files are kept with each suite so that test results are deterministic and can be compared against known-good output.

## Interpreting failures

A non-zero exit status does not necessarily mean the implementation failed a functional test. A suite can also fail because:

- Compilation failed.
- A generated output differs from the model output.
- The program exceeded its time limit.
- Valgrind reported an error or leak.
- An expected executable/object file was not produced or was not cleaned up.

The suite-specific scripts produce diagnostic files such as `diff*.txt`, `time*.txt`, `valgrind-out*.txt`, and test output files. These are useful for determining which check failed.
