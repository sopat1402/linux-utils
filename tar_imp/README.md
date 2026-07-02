# tar_imp

A minimal implementation of the Unix `tar` archiving utility written in C using POSIX system calls and standard C libraries.

This project was built as a systems programming exercise to understand the TAR file format rather than to replicate every feature of GNU `tar`.

## Features

* Creates valid uncompressed `.tar` archives.
* Archives individual files.
* Recursively archives directories while preserving the directory structure.
* Stores file metadata in POSIX USTAR headers.
* Produces archives that can be listed and extracted by GNU `tar`.
* Uses 512-byte TAR blocks and proper end-of-archive markers.

## Example

Create an archive:

```bash
./tar archive.tar file1.txt directory/
```

Verify the archive:

```bash
tar -tf archive.tar
```

Extract using GNU tar:

```bash
tar -xf archive.tar
```

## Implementation Details

The archive writer currently implements:

* Recursive directory traversal using depth-first search (DFS)
* POSIX `stat()` metadata collection
* USTAR header generation
* Octal encoding of numeric header fields
* TAR checksum calculation
* 512-byte header layout
* File data padding to 512-byte boundaries
* Two 512-byte zero blocks marking the end of the archive

## Project Structure

```
tar_imp/
├── tar.c            # Program entry point
├── archive.c        # TAR header generation and file serialization
├── archive.h
├── traversal.c      # Recursive filesystem traversal
├── traversal.h
```

## Current Limitations

This implementation is intentionally minimal and does not yet support:

* Archive extraction
* Compression (`.tar.gz`)
* Long path names requiring the USTAR prefix field
* Symbolic links
* Hard links
* Special device files
* Extended TAR formats (GNU/PAX)

## Testing

Archives produced by this implementation have been successfully verified with the GNU `tar` utility.

Example:

```bash
./tar test.tar test/
tar -tf test.tar
tar -xf test.tar
```

The extracted directory structure and file contents match the original input.

## Motivation

This project was written to gain a deeper understanding of:

* Binary file formats
* Filesystem traversal
* POSIX APIs
* Metadata serialization
* Block-based storage formats
* Systems programming in C

Rather than relying on external libraries, every archive is constructed manually according to the USTAR specification.

## Future Work

Planned projects include:

* `gzip_imp` — implementation of the GZIP/DEFLATE compression format.
* `untar_imp` — extraction of TAR archives.
* Combining both projects to support `.tar.gz` archives.
