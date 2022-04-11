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
Beaware that Makefile will try to sudo gcc every source file, so either call 'compile-all' option
```
make compile-all
```
and then sudo cp it by yourself whenever you want, or call 'compile-and-send' options and it will move everything in one go to /usr/local/bin foldef.
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
