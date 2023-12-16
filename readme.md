# Requisitos

- Linux ou WSL.
- ncurses5

# SGDB - ITP

O SGDB - ITP foi um projeto desenvolvido durante a matéria Introdução as Técnicas de Programação (ITP) do Instituto Metrópole Digital (IMD/UFRN).

O projeto consiste na implementação de um Sistema de Gerênciamento de Banco de Dados (SGDB) de pequeno porte.

> A utilização desse programa não é recomendada para ambientes de produção. (vai que, né?)

As funcionalidades desenvolvidas foram:

- Criar um tabela
- Listar todas as tabelas
- Criar uma nova linha na tabela.
- Listar todos os dados de uma tabela
- Pesquisar valor em uma tabela
- Apagar uma tupla (registro ou linha) de uma tabela
- Apagar uma tabela

## Como funciona?

Ao executar o código, o usuário verá um menu com várias opções. Ele pode se movimentar usando as setas do teclado e confirmar a opção selecionada pressionando Enter.

### Criar um tabela

- Ao selecionar esta opção, o usuário será solicitado a fornecer um nome para a nova tabela.

- O usuário deve escolher um nome da PK.

- Depois, o usuário deverá informar os dados da coluna no formato "coluna:tipo"

- Quando ele criar todas as colunas, poderá apertar Ctrl+D ou escrever "quit" para finalizar a criação de tabela.

### Listar todas as tabelas

- Esta opção irá listar todos os esquemas das tabelas existentes.

### Listar uma tabelas

- Esta opção irá mostrar o esquema das tabela escolhida.

### Criar uma nova linha na tabela

- Ao selecionar esta opção, o usuário será solicitado a escolher uma tabela e, em seguida, a fornecer os dados para a nova linha. Os dados devem ser fornecidos no formato correto para a tabela selecionada.

### Listar todos os dados de uma tabela

- O usuário será solicitado a escolher uma tabela e, em seguida, esta opção irá listar todos os dados na tabela selecionada.

### Pesquisar valor em uma tabela

- Ao selecionar esta opção, o usuário será solicitado a fornecer uma tabela e valor de pesquisa.
- O usuário verá várias opções de filtros para pesquisa e deverá escolher uma.
- Depois de escolher o filtro, o programa retornará todos os resultados que correspondem a pesquisa com o filtro desejado.

### Apagar uma tupla (registro ou linha) de uma tabela

- Ao selecionar esta opção, o usuário será solicitado a fornecer um nome de tabela e Primary Key.
- Depois de fornecer os dados, o registro será apagado da tabela.

### Apagar uma tabela

- Ao selecionar esta opção, o usuário será solicitado a fornecer um nome de tabela.
- Depois de fornecer o nome da tablea, o arquivo de registros da tabela será apagado, e a linha no arquivo de tabelas que correspondia a tabela alvo, será apagado.

## Como rodar o projeto?

#### Tutorial testado apenas em Ubuntu 22.04.

Primeiro, instale as dependências. Rode o comando em seu terminal abaixo para instalar o ncurses5:

```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Agora, compile o código:

```
make
```

Depois de compilado, é só rodar:

```
./app
```
