# my FTP - File Transfer Protocol Server

## Project Overview

**Network**  
**NT-101**

This project aims to implement a simple FTP (File Transfer Protocol) server. The server is designed to handle file transfers, directory navigation, and basic user authentication.

### Binary Name

`myftp`

### Language

C

### Compilation

Compilation is handled via a Makefile with the following rules:

- `make`: Compiles the project.
- `make re`: Re-compiles the project, first cleaning previous compilations.
- `make clean`: Cleans the project (removing object files and temporary files).
- `make fclean`: Cleans the project fully (including the binary).

**Note**: All source files must be included in the delivery, excluding binary files, temporary files, object files, etc. Bonus files, if any, should be located in a directory named `bonus`.

### Usage

```sh
./myftp -help
USAGE: ./myftp port path
  port: Port number on which the server socket listens.
  path: Path to the home directory for the Anonymous user.
```
