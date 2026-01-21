# Vinilcius

> **Projeto Final de Programação 3** > **Professor:** Ruben Carlo Benante
> **Instituição:** Escola Politécnica de Pernambuco (POLI/UPE)

## Sobre o Projeto

O **Vinilcius** é uma aplicação Desktop desenvolvida em **C++** utilizando o framework **Qt**, projetada para gerenciar integralmente uma loja de discos de vinil.

O sistema simula um ambiente de vendas, permitindo a comercialização de discos (novos e usados) e oferecendo interfaces distintas para **Clientes** e **Funcionários/Administradores**, garantindo segurança e organização dos dados através de um banco de dados relacional.

## Usuarios Para Teste

* Cliente

- Usuario: caio@gmail.com
- Senha/CPF: 16964010427

* Funcionário

- Usuario: fab@vinilcius.com
- Senha/CPF:6655987413

* Gerente

- usuário: Rodri@vinilcius.com
- Senha/CPF: 71393581412

* Adm Software - Primeiro Registro

- Usuário: Registro liberado com qualquer e-mail.
- Senha: (*****)
- Senha será definada pelo Adm do Software, em seu primeiro login.


## Tecnologias Utilizadas

* **Linguagem:** C++
* **Interface Gráfica:** Qt Framework (Qt Designer)
* **Banco de Dados:** SQLite
* **IDE Recomendada:** Qt Creator

## ⚙️ Arquitetura e Modelagem

O projeto foi estruturado pensando na separação de responsabilidades e persistência de dados.

### Banco de Dados (SQLite)
O sistema conta com um banco de dados composto pelas seguintes tabelas principais:

* `TB_USUARIOS`: Tabela base para login e autenticação.
* `TB_CLIENTES`: Dados pessoais dos compradores.
* `TB_FUNCIONARIOS`: Dados da equipe administrativa.
* `TB_DISCOS`: Catálogo de produtos (inclui flag para Novo/Usado, Gênero, Artista).
* `TB_ESTOQUE`: Controle quantitativo dos discos.
* `TB_VENDAS`: Registro das transações efetuadas.

## Funcionalidades

O sistema possui controle de acesso via Login, direcionando o usuário para a tela correta baseada no seu nível de permissão.

### Módulo do Cliente
* **Visualização de Catálogo:** Navegação pelos discos disponíveis.
* **Filtros:** Busca por discos novos ou usados.
* **Carrinho de Compras:** Adição e remoção de itens antes de fechar o pedido.

### Módulo Administrativo (Funcionário)
* **Gestão de Discos:** Cadastrar novos títulos, editar preços e remover itens do catálogo.
* **Gestão de Estoque:** Dar baixa ou adicionar unidades.
* **Gestão de Pessoas:** Cadastrar e remover outros funcionários e visualizar lista de clientes.
* **Dashboard:** Visualização de informações úteis (Relatórios de vendas, itens mais vendidos, etc).

## Autores

* Gabriel Dutra Chaves           | <gdc1@poli.br>  | @dutraat
* Vinicius Magalhaes De Medeiros | <vmm1@poli.br>  | @Viniciusmedeiros1
* Juliana Silva Fonseca Carneiro | <jsfc1@poli.br> | @juliana373217
* Anthony Lucas Dos Santos Silva | <alss1@poli.br> | @Anthonyldss
* Diego Nascimento Dos Santos    | <dns@poli.br>   | @Dinego05
* Dennis Miller Fidelis Da Silva | <dmfs@poli.br>  | @dmfdsilva
* Igor Guimaraes Monteiro        | <igm1@poli.br>  | @igm22
* Anne Luiza Assuncao Freire     | <alaf@poli.br>  | @AnneLuiza
* Christian Viana Tomaz          | <cvt@poli.br>   | @christianvtomaz
---

