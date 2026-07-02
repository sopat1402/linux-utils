# linux-utils

A collection of Unix utilities implemented in C while learning systems programming and the Unix programming model.

The goal of this repository is not to perfectly recreate GNU Coreutils, but to understand how common command-line tools work internally by building them from scratch.

## Utilities

* `ls` - Directory listing with support for `-a` and `-l`
* `cat` - File concatenation with support for `-n`, `-E`, and `-T`
* `wc` - Word count, line count and byte count for `-l`,`-w` and `-c`
* `cp` - Copying multiple files and directory trees recursively with `-r` and interactive mode with `-i`

More utilities will be added as I continue learning.


## Build

Each utility can be built independently from its own directory using `gcc`.

