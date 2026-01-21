#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <QMainWindow>
#include <QWidget>
#include <QVector>
#include "Cadastro.hpp"

// As variais pra o sql
struct DiscoEstoque {
    QString nome;
    QString artista;
    QString ano;
    QString codigo;
    QString genero;
    QString condicao;
    QString preco;
    QString quantidade;
};

// As variais pra o sql
struct Venda {
    QString data;
    QString disco;
    QString quantidade;
    QString valor;
    QString cliente;
};

class DebugMenu;
namespace Ui {
class EstoqueUI;
}

class Estoque : public QMainWindow
{
    Q_OBJECT

public:
    explicit Estoque(QWidget *parent = nullptr);
    virtual ~Estoque() override;

    void setOrigem(int tipo);
    static const int OrigemGerente = 1;
    static const int OrigemCadastro = 2;
    static const int OrigemCentralFunc = 3;

private slots:
    void on_pushButton_editar_clicked();
    void on_pushButton_excluir_clicked();
    void on_pushButton_fechar_msg_clicked();
    void on_pushButton_voltar_clicked();
    void on_pushButton_Debug_clicked();

private:
    Ui::EstoqueUI *ui;
    QVector<DiscoEstoque> discos;
    QVector<Venda> vendas;
    int indiceEdicao = -1;  // Para controlar se está editandoi
    int origem = 0;
    void carregarDadosVendas();
    void mostrarMensagem(const QString &mensagem, bool erro = false);
    void limparCampos();
    void atualizarTabela();
    void atualizarTabelaVendas();
    void atualizarEstatisticasVendas();
    bool validarCampos();
};

#endif // ESTOQUE_H
