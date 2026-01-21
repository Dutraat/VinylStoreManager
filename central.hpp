#ifndef CENTRALL_H
#define CENTRALL_H
#include <QMainWindow>
#include "Estoque.hpp"
#include "Gerente.hpp"
#include "principalui.hpp"
#include "Controledaloja.hpp"
#include "debug.hpp"
#include <QMessageBox>
#include <QDialog>

class DebugMenu;

namespace Ui {
class Central;
}

class Central : public QMainWindow
{
    Q_OBJECT
public:
    explicit Central(QString cargo, QWidget *parent = nullptr);
    ~Central();
    void atualizarDashboard(double dia, double semana, double mes, double ticketMedio);

private slots:
    void on_pushButton_controle_loja_clicked();
    void on_pushButton_cadastro_clicked();
    void on_pushButton_tela_cliente_clicked();
    void on_pushButton_tela_cliente_2_clicked();
    void on_pushButton_Debug_clicked();  // Apenas a declaração, sem implementação

private:
    Ui::Central *ui;
    QString cargoUsuario;
};

#endif // CENTRALL_H
