#ifndef CADASTRO_H
#define CADASTRO_H

#include <QMainWindow>
#include <QWidget>
#include"Estoque.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DebugMenu;

struct Cliente {
    int id = -1;
    QString nome;
    QString cpf;
    QString cep;
    QString telefone;
    QString email;
};
struct Vinil {

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



namespace Ui {
class CadastroUI;
}

class Cadastro : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cadastro(QWidget *parent = nullptr);
    virtual ~Cadastro() override;

private slots :
       void on_pushButton_pagina_cliente_clicked();
       void on_pushButton_pagina_vinil_clicked();
       void on_pushButton_pagina_itens_clicked();

       void on_pushButton_salvar_cliente_clicked();
       void on_pushButton_alterar_cliente_clicked();
       void on_pushButton_excluir_cliente_clicked();
       void on_tableWidget_clientes_cellClicked(int row, int column);
       void on_pushButton_salvar_vinil_clicked();
       void on_pushButton_alterar_vinil_clicked();
       void on_pushButton_excluir_vinil_clicked();
       void on_tableWidget_vinis_cellClicked(int row, int column);
       void on_pushButton_Debug_clicked();
       void on_pushButton_voltar_clicked();
private:
    Ui::CadastroUI *ui;
    QSqlDatabase db;
    int idEdicaoCliente = -1;
    int idEdicaoVinil = -1;
    int indiceEdicaoCliente = -1;
    int indiceEdicaoVinil = -1;
    bool iniciarBancoDeDados();
    bool criarTabelas();
    void mostrarMensagem(const QString &mensagem, bool erro = false);
    void limparCamposCliente();
    bool validarCamposCliente();
    void atualizarTabelaClientes();
    void limparCamposVinil();
    bool validarCamposVinil();
    void atualizarTabelaVinis();
    void atualizarTabelaUsuarios();
};

#endif // CADASTRO_H
