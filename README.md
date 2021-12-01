## Instalando e compilando

(testado no Ubuntu LTS 20.04)

### Instalar `g++` versão 11

Instala o g++ e gcc versão 11 e configura como padrão. Note que esses comandos
talvez não funcionem se os compiladores já estiverem configurados com outras
versões com maior prioridade. Portanto, após executar os comandos abaixo,
verifique `g++ --version` para ver se a operação foi bem sucedida.

```sh
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update && sudo apt install gcc-11 g++-11
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 20
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 20
```

### Instalar o `SFML`

```sh
sudo apt install libsfml-dev
```

### Baixar e compilar o projeto

Primeiro clone o repositório

```sh
git clone https://github.com/GabrielDertoni/not-dual
cd not-dual
```

E então, para compilar o projeto simplesmente execute

```sh
# Irá usar o número de CPUS virtuais para agilizar a compilação.
make -j$(nproc)
```

Por fim, o jogo pode ser executado

```sh
./not_dual
```

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

## TODOs

- [ ] Finalizar o `README.md`.
- [ ] Tela de início/fim/jogar novamente.
- [ ] Desenho das naves/tiros/poderes.
- [ ] Soundtrack.
- [ ] Background.
