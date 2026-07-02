# wc_imp

A simple implementation of the Unix `wc` (word count) command written in C as part of my systems programming journey.

## Features

* Count lines (`-l`)
* Count words (`-w`)
* Count bytes (`-c`)
* Read from one or more files
* Read from standard input (`stdin`) when no file is provided

## Concepts Practiced

* File I/O (`fopen`, `fgetc`, `fclose`)
* Standard input (`stdin`)
* Character classification
* Command-line argument parsing with `getopt()`
* Bit fields for storing command-line options
* State-based stream processing

## Build

```bash
gcc wc.c -o wc
```

## Run

```bash
./wc file.txt
./wc file1.txt file2.txt
./wc -w file.txt
./wc -l file.txt
./wc -c file.txt
./wc -lw file.txt
./wc
```

## Future Improvements

* Match GNU `wc` output formatting more closely
* Support additional command-line options
* Improve portability and edge-case handling

