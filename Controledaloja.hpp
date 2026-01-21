#ifndef CONTROLEDALOJA_HPP
#define CONTROLEDALOJA_HPP

#include <QMainWindow>

class Gerente;
class Central;

namespace Ui {
class Controledaloja;
}

class Controledaloja : public QMainWindow
{
    Q_OBJECT

public:
    enum OrigemJanela {
        OrigemCentral,
        OrigemGerente
    };

    explicit Controledaloja(QWidget *parent = nullptr);
    ~Controledaloja();

    void setGerente(Gerente *g);
    void setOrigem(OrigemJanela o)     { origem = o; }

    void atualizarDados();

private slots:
    void on_pushButton_demitir_clicked();
    void on_pushButton_promover_clicked();
    void on_pushButton_VoltarGerente_clicked();
    void on_pushButton_Debug_clicked();

private:
    Ui::Controledaloja *ui;

    Gerente *gerente = nullptr;
    OrigemJanela origem = OrigemCentral;

    double obterTotalVendasDia();
    double obterTotalVendasSemana();
    double obterTotalVendasMes();
    double obterTicketMedio();

    void carregarEstoqueCritico();
    void carregarFuncionarios();
};

#endif

