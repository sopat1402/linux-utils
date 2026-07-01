# cat_imp

A simple implementation of the Unix `cat` command written in C as part of my systems programming journey.

## Features

* Read and display one or more files
* Read from standard input (`stdin`)
* `-n` : Number output lines
* `-E` : Display `$` at the end of each line
* `-T` : Display tabs as `^I`

## Concepts Practiced

* File I/O (`fopen`, `fgetc`, `fclose`)
* Standard input (`stdin`)
* Command-line argument parsing with `getopt()`
* C structs for program configuration
* Character stream processing

## Build

```bash
gcc cat.c -o cat
```

## Run

```bash
./cat file.txt
./cat file1.txt file2.txt
./cat -n file.txt
./cat -ET file.txt
```

## Future Improvements

* Support `-` as standard input between files
* Additional GNU `cat` options
* Better error handling

