
## Contribuindo

### Estilo

- Funções e variáveis `cammelCase`, classes `PascalCase`, namespace `snake_case`.
- `#include` de `.h` locais sempre no formato `"includes/nome.h"`.
- Ordem dos includes sempre 1o includes da `std`, depois da `SFML` e por fim os
    locais. Cada grupo deve ser separado por uma linha em branco.
- Inicio de blocos na mesma linha do `if`, `for`, `while` ou `função`.

```c++
if (cond) {
    /* ação */
}

UmaClasseAi fazAlgo() {
    return UmaClasseAi();
}
```
