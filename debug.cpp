#include "debug.hpp"
#include "Controledaloja.hpp"
#include "Estoque.hpp"
#include "Gerente.hpp"
#include "central.hpp"
#include "firstlogin.hpp"
#include "principalui.hpp"
#include "ui_debug.h"
#include "registro.hpp"

DebugMenu::DebugMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugMenu)
{
    ui->setupUi(this);
}

DebugMenu::~DebugMenu()
{
    delete ui;
}

void DebugMenu::on_pushButton_Close_clicked()
{
    this->reject();
}

void DebugMenu::on_pushButton_Tela0_clicked()
{
    Help1 *help = new Help1();
    help->setAttribute(Qt::WA_DeleteOnClose);
    help->show();
    this->accept();
}

void DebugMenu::on_pushButton_Tela1_clicked()
{
    this->hide();
     Cadastro *cad = new Cadastro();
    cad->setAttribute(Qt::WA_DeleteOnClose);
    cad->show();
    this->accept();

}
void DebugMenu::on_pushButton_Tela2_clicked()
{
    this->hide();
    Estoque *est = new Estoque();
    est->setAttribute(Qt::WA_DeleteOnClose);
    est->show();
    this->accept();
}

void DebugMenu::on_pushButton_Tela3_clicked()
{
    this->hide();
    Gerente *ger = new Gerente();
    ger->setAttribute(Qt::WA_DeleteOnClose);
    ger->show();
    this->accept();
}

void DebugMenu::on_pushButton_Tela4_clicked()
{
    this->hide();
    firstlogin *fir = new firstlogin();
    fir->setAttribute(Qt::WA_DeleteOnClose);
    fir->show();
    this->accept();
}
void DebugMenu::on_pushButton_Tela5_clicked()
{
    this->hide();
    Controledaloja *con = new Controledaloja();
    con->setAttribute(Qt::WA_DeleteOnClose);
    con->show();
    this->accept();
}

void DebugMenu::on_pushButton_Tela6_clicked()
{
    this->hide();
    MainWindow *mai = new MainWindow();
    mai->setAttribute(Qt::WA_DeleteOnClose);
    mai->show();
    this->accept();
}

void DebugMenu::on_pushButton_Tela7_clicked()
{
    this->hide();
    principalui *pri = new principalui();
    pri->setAttribute(Qt::WA_DeleteOnClose);
    pri->show();
    this->accept();
}

void DebugMenu::on_pushButton_Tela8_clicked()
{
    this->hide();
   Central *cen = new Central("Gerente");
    cen->setAttribute(Qt::WA_DeleteOnClose);
    cen->show();
    this->accept();
}

void DebugMenu::on_pushButton_Tela9_clicked()
{
    this->hide();
    registro *reg = new registro();
    reg->setAttribute(Qt::WA_DeleteOnClose);
    reg->show();
    this->accept();
}

