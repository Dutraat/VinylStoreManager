#ifndef PRINCIPALUI_H
#define PRINCIPALUI_H

#include <QMainWindow>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class principalui;
}
QT_END_NAMESPACE

enum class PeriodoVenda {
    Dia,
    Semana,
    Mes
};


class principalui : public QMainWindow
{
    Q_OBJECT

public:
    principalui(QWidget *parent = nullptr);
    ~principalui();
    void adicionarProduto(QString nome, double preco, int quantidade);
    void removerProduto(int linhaSelecionada);
    void atualizarTotal();
    void popularTabelaDiscosUsados();
    void popularTabelaDiscosNovos();
    void atualizarCarrinhoVisual();
    bool BaixaEstoque();
    void setTipoUsuario(QString tipo);

signals:
    void vendaRealizada();

private slots:
    void on_pushButton_Backtocatalog_clicked();
    void on_pushButton_Pay_clicked();
    void on_pushButton_Backtocart_clicked();
    void on_pushButton_RemoverSelecionado_clicked();
    void on_pushButton_Discos_clicked();
    void on_pushButton_shopcart_clicked();
    void on_pushButton_confirm_clicked();
    void on_pushButton_ContinuarCompra_clicked();
    void on_pushButton_cupom_clicked();
    void on_pushButton_Debug_clicked(); // funcao debug
    void atualizarDashboardGerente();
    void on_toolButton_clicked();
    void on_pushButton_voltar_clicked();
private:
    Ui::principalui *ui;
    bool cupomAplicado = false;
    double desconto = 0.0;

    struct ProdutoCarrinho {
        QString nome;
        double preco;
        int quantidade;
    };
    QVector<ProdutoCarrinho> carrinho;
    bool RegistrarVenda(QString numeroPedido, double valorTotal);
    double ObterTotalVendas(PeriodoVenda periodo);
    QString tipoUsuario;
};

#endif // PRINCIPALUI_H

