
# cp_imp

A C implementation of the Unix `cp` command built from scratch as part of my Linux utilities project.

## Features

* Copy a single file
* Copy multiple files into a destination directory
* Recursive directory copying (`-r`)
* Interactive overwrite prompt (`-i`)
* Buffered file copying using `fread()` and `fwrite()`
* Works with both text and binary files
* Preserves directory structure during recursive copies

## Supported Options

| Option | Description                              |
| ------ | ---------------------------------------- |
| `-r`   | Copy directories recursively             |
| `-i`   | Prompt before overwriting existing files |
| `-f`   | Reserved for future implementation       |

## Building

```bash
gcc -Wall -Wextra -o cp cp.c
```

## Usage

Copy a file:

```bash
./cp source.txt destination.txt
```

Copy multiple files into a directory:

```bash
./cp file1.txt file2.txt destination_directory
```

Recursively copy a directory:

```bash
./cp -r source_directory destination_directory
```

Prompt before overwriting files:

```bash
./cp -i source.txt destination.txt
```

## Concepts Practiced

* File I/O with `FILE *`
* Buffered copying with `fread()` and `fwrite()`
* Command-line argument parsing with `getopt()`
* File metadata using `stat()`
* Directory traversal using `opendir()` and `readdir()`
* Recursive depth-first traversal (DFS)
* Recursive function design
* Path manipulation with `snprintf()`
* Error handling using `perror()`
* Resource management (`fclose()`, `closedir()`)

## Notes

This project is intended as a learning exercise rather than a complete replacement for the GNU `cp` utility. The focus is on understanding how file copying, directory traversal, and recursive algorithms work at a low level.

Future improvements may include:

* Full `-f` (force) implementation
* Preserving file permissions and timestamps
* Symbolic link handling
* Better error recovery
* Additional GNU `cp` options
