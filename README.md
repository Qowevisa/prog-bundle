# C programs bundle
Similar to pipe-bundle, but these programs are not specialized in reading stdin stream.
This is a bundle of minimalistic programs written in C.
Each program IGNORES stdin.
Default stream for each program is stdout.
Everything else about compilation is written in Makefile, so just run
```
make
```
and read the output.
Beaware that Makefile will try to gcc every source file into ~/.local/bin folder, so check if it is in your $PATH variable or change folder in Makefile to appropriate. You can also run
```
make compile-all
```
and then copy programs by yourself whenever you want, or call 'compile-and-send' options and it will move everything in one go to ~/.local/bin foldef.
```
make compile-and-send
```
You can make individual programs by calling
```
make <PROGRAM>
```
## Sinopsis for each program
### bytes
prints raw bytes in files from args in \{binary, singed decimal, unsigned decimal, hex\} format
for help menu use only --help in first argument
### chunks
reads raw bytes in files from args and prints them in chunk. So if file contains only FF bytes the output will be
```
FF:N
```
where N is number of bytes in file. You can check it with stat program.
It also can read only N first bytes. As always just, check --help menu.
