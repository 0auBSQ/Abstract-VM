# Abstract-VM
(42 Project) Basic pseudo-ASM interpreter in C++

## Usage
Build with make and launch the `abstractvm` executable :
```
make
./abstractvm
./abstractvm [filename]
```

Executing without argument reads instructions on the standard input.<\br>
Executing with an argument reads the given filename.<\br>
You can close the stdin using `;;`, `exit` or `CTRL+D`.