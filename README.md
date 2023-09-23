# Biblioteca Teoria dos Grafos

# Passos execução

Em qualquer código (assumindo ser um arquivo chamado `test.cpp` fora da pasta `src`).

```
.
├── README.md
├── src
│   ├── mylib.cpp
│   └── mylib.hpp
├── test.cpp
```

## 1. Compilar arquivos biblioteca

```bash
g++ -c src/mylib.cpp -o mylib.o
ar rcs mylib.a mylib.o
```

## 2. Compilar código com biblioteca

Incluir a biblioteca no arquivo a ser executado:

```cpp
#include "src/my_lib.cpp"
```
Então, compilar:
```bash
g++ -c test.cpp -o test.o
g++ -o test test.o -L. mylib.o
```