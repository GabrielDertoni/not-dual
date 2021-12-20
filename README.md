## Not-Dual

O jogo not-dual é um jogo multiplayer local que consiste no confronto entre dois
jogadores, cada um controlando uma nave diferente através de um conjunto de teclas.
O objetivo de cada jogador é destruir a nave adversária atirando projéteis.

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

## Como jogar?

Após instalado e compilado o jogo, o jogador da esquerda se movimenta utilizando-se
das teclas W, A, S, D e atira seus projéteis com a tecla Ctrl-Esquerdo. Enquanto que
o jogador da direita se movimenta usando as setas UP, LEFT, DOWN, RIGHT e atira seus
projéteis com a tecla Ctrl-Direito. De tempos em tempos, aparece na tela um Super-Poder
que quando obtido por algum jogador aumenta o dano do seu próximo projétil.

## Threads e semáforos

### Threads:

Foram utilizadas duas threads para o funcionamento do jogo:
- MainThread / RenderThread que é responsável por fazer toda a renderização do jogo.
- GameThread que é responsável por tratar toda a lógica que o jogo necessita para funcionar corretamente.

Com tal tipo de implementação, consegue-se otimizar o uso da CPU para realizar as duas coisas
que o jogo Not-Dual mais precisa, isto é, as atualizações em tempo real de todos os GameObjects (RenderThread)
para os players e o tratamento de toda a lógica do jogo (movimento, colisões, super-poderes, dano recebido, etc...)
para que assim o jogo funcione corretamente (GameThread).

### Semáforos

Para tratar a sincronização dessas duas threads foram utilizados dois semáforos binários, permitindo que todos os GameObjects que estão em uma fila de renderização possam ser desenhadas ao mesmo tempo que o Loop principal do jogo ocorre. Ou seja, todos os GameObjects estão sendo calculados para o próximo frame enquanto o frame anterior está sendo desenhado.

Os dois semáforos binários são:
- GameLoopStart que basicamente sincroniza o início do gameLoop (lógica do jogo) com o desenho de todos os gameObjects que estão em uma fila de renderização.
- GameLoopDone que espera até o gameLoop terminar e, assim, evita erros na sincronização.

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
