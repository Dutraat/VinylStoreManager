#ifndef DEBUGMENU_HPP
#define DEBUGMENU_HPP

#include <QDialog>
#include <QMessageBox>
/* Include das telas */
#include "help1.hpp" // tela 1
#include "Cadastro.hpp" // tela 2
#include "Estoque.hpp" // tela 3
#include "Gerente.hpp" // tela 4
#include "Controledaloja.hpp" // tela 5
#include "firstlogin.hpp" // tela 5
#include "mainwindow.hpp" // tela 7
#include "principalui.hpp" //tela 8
#include "central.hpp" //tela 9
/*-------------------  */


namespace Ui {
class DebugMenu;
}

class DebugMenu : public QDialog
{
    Q_OBJECT

public:
    explicit DebugMenu(QWidget *parent = nullptr);
    ~DebugMenu();

private slots:
    void on_pushButton_Tela0_clicked();
    void on_pushButton_Tela1_clicked();
    void on_pushButton_Tela2_clicked();
    void on_pushButton_Tela3_clicked();
    void on_pushButton_Tela4_clicked();
    void on_pushButton_Tela5_clicked();
    void on_pushButton_Tela6_clicked();
    void on_pushButton_Tela7_clicked();
    void on_pushButton_Tela8_clicked();
    void on_pushButton_Tela9_clicked();
    void on_pushButton_Close_clicked();
private:
    Ui::DebugMenu *ui;
};

#endif // DEBUGMENU_HPP
