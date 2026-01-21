#ifndef GERENTE_H
#define GERENTE_H

#include <QMainWindow>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "Cadastro.hpp"
#include "Estoque.hpp"
#include "Controledaloja.hpp"

struct cliente {
    int id = -1;
    QString nome;
    QString cpf;
    QString cep;
    QString telefone;
    QString email;
};

struct vinil {
    int id = -1;
    QString nome;
    QString artista;
    QString ano;
    QString genero;
    QString preco;
    QString condicao;
    QString quantidade;
    QString codigo;
};

struct funcionario {
    int id = -1;
    QString nome;
    QString cpf;
    QString pis;
    QString email;
    QString funcao;
    QString salario;
    QString cep;
    QString telefone;
};
class DebugMenu;
namespace Ui {
class GerenteUI;
}

class Gerente : public QMainWindow
{
    Q_OBJECT

public:
    explicit Gerente(QWidget *parent = nullptr);
    virtual ~Gerente() override;
     void atualizarDashboard(double dia, double semana, double mes, double ticketMedio);
     void atualizarTabelaFuncionarios();

private slots :
    void on_pushButton_pagina_cliente_clicked();
    void on_pushButton_pagina_vinil_clicked();
    void on_pushButton_pagina_funcionario_clicked();
    void on_pushButton_pagina_itens_clicked();

    void on_pushButton_salvar_cliente_clicked();
    void on_pushButton_alterar_cliente_clicked();
    void on_pushButton_excluir_cliente_clicked();
    void on_tableWidget_clientes_cellClicked(int row, int column);

    void on_pushButton_salvar_vinil_clicked();
    void on_pushButton_alterar_vinil_clicked();
    void on_pushButton_excluir_vinil_clicked();
    void on_tableWidget_vinis_cellClicked(int row, int column);

    void on_pushButton_funcionario_salvar_clicked();
    void on_pushButton_funcionario_alterar_clicked();
    void on_pushButton_funcionario_excluir_clicked();
    void on_tableWidget_clientes_2_cellClicked(int row, int column);
    void on_pushButton_controle_loja_clicked();
    void on_pushButton_Debug_clicked();

    void on_pushButton_voltar_clicked();

private:
    Ui::GerenteUI *ui;
    QSqlDatabase db;

    int idEdicaoCliente = -1;
    int idEdicaoVinil = -1;
    int idEdicaoFuncionario = -1;

    bool iniciarBancoDeDados();
    bool criarTabelas();
    void mostrarMensagem(const QString &mensagem, bool erro = false);

    void limparCamposCliente();
    bool validarCamposCliente();
    void atualizarTabelaClientes();

    void limparCamposVinil();
    bool validarCamposVinil();
    void atualizarTabelaVinis();

    bool criarTabelaFuncionarios();
    void limparCamposFuncionario();
    bool validarCamposFuncionario();
    Controledaloja *controleLoja = nullptr;
    void atualizarTabelaUsuarios();
    void cadastrarUsuarioComoCliente(int row);
};

#endif
