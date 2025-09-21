# cse101-pt.f25/pa5

The following is a set of checks to run on your pa5 submission (WordFrequency/Dictionary). It
runs WordFrequency on several inputs and compares your output files to our correct model
outputs. We have made this available to you to check your work before making
your final submission.

## Usage

After installation, you can run the script with this line:

```bash
chmod +x ./../cse-101-public-tests/pa5/pa5.sh
./../cse-101-public-tests/pa5/pa5.sh
```

It will print out the difference between your output and the correct output,
using the `diff` command. Lack of any output between the set of "=========="
means that your program is running correctly.

Any lines prefixed with `-` are from your own output, and are incorrect. Any
lines prefixed with `+` are from the correct output, and are missing in your
output.

## Removal

The following command will remove all text files and shell scripts in your
directory. Since you should not have any files that end in `.txt` or `.sh`
anyway, this should serve to delete all the files we gave you.

```bash
rm -f *.txt *.sh
```
