# Red-Black Tree Test Suite

Automated tests for a C++ `Dictionary` implementation and the `Words` and `WordFrequency` programs. This suite combines functional comparisons, performance checks, memory checks, model dictionary tests, and build/clean validation.

## What is tested

### `Words`

Five input cases are provided:

```text
infile1.txt ... infile5.txt
```

Their expected outputs are:

```text
model-outfile1.txt ... model-outfile5.txt
```

The `Words` test compiles `Words.cpp` and `Dictionary.cpp`, runs all five cases, compares output with `diff`, checks runtime, and runs Valgrind.

### `WordFrequency`

Three input cases are provided:

```text
WF-infile1.txt
WF-infile2.txt
WF-infile3.txt
```

Their expected outputs are:

```text
Model-WF-outfile1.txt
Model-WF-outfile2.txt
Model-WF-outfile3.txt
```

The `WordFrequency` test performs the same compilation, output, runtime, and Valgrind checks.

### Model dictionary test

`ModelDictionaryTest.cpp` is compiled against `Dictionary.cpp` and exercises the dictionary API. The test also applies a runtime threshold and a Valgrind check.

### Build test

`redblacktree-build.sh` verifies that `make` builds both `Words` and `WordFrequency`, creates object files, and that `make clean` removes them.

## Scripts

| Script | Purpose |
|---|---|
| `main.sh` | Runs all red-black-tree suite checks. |
| `words.sh` | Tests `Words` with five inputs, timing, and Valgrind. |
| `wordfrequency.sh` | Tests `WordFrequency` with three inputs, timing, and Valgrind. |
| `model-redblacktree-test.sh` | Runs the model `Dictionary` client with runtime and Valgrind checks. |
| `redblacktree-build.sh` | Verifies the build and clean targets. |

## Running

From the project directory containing `Words.cpp`, `WordFrequency.cpp`, `Dictionary.cpp`, and the corresponding `Makefile`:

```bash
../cse-101-public-tests/redblacktree/main.sh
```

Individual checks:

```bash
../cse-101-public-tests/redblacktree/words.sh
../cse-101-public-tests/redblacktree/wordfrequency.sh
../cse-101-public-tests/redblacktree/model-redblacktree-test.sh
../cse-101-public-tests/redblacktree/redblacktree-build.sh
```

`main.sh` accepts an optional runtime multiplier and passes it to the checks that support it:

```bash
../cse-101-public-tests/redblacktree/main.sh 2
```

## Additional word-frequency data

The directory also contains:

```text
WF-infile1.txt ... WF-infile3.txt
Model-WF-outfile1.txt ... Model-WF-outfile3.txt
```

The `nored_model-outfile*.txt` files provide additional reference outputs associated with non-red-tree/model scenarios.

## Generated diagnostics

Test runs may produce:

```text
outfile*.txt
WF-outfile*.txt
diff*.txt
WF-diff*.txt
time*.txt
WF-time*.txt
valgrind-out*.txt
valgrind-out-WF*.txt
DictionaryTest-out.txt
DictionaryTest-mem.txt
```

These are generated test artifacts rather than source/reference inputs.

## Pass criteria

A functional case passes only when compilation, execution, runtime, and output comparison all succeed. Valgrind must also complete without the configured error exit code. The model and build checks have their own corresponding requirements.
