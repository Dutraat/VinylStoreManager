#include "central.hpp"
#include "Controledaloja.hpp"
#include "ui_central.h"
#include "Gerente.hpp"
#include "principalui.hpp"
#include "ui_principalui.h"
#include <QMessageBox>
#include "debug.hpp"
#include "Cadastro.hpp"
#include "Estoque.hpp"

 Central::Central(QString cargo, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Central)
{
    ui->setupUi(this);
    this->cargoUsuario = cargo;
}

Central::~Central()
{
    delete ui;
}
void Central::on_pushButton_cadastro_clicked()
{
    if (cargoUsuario == "gerente") {
        Gerente *g  = new Gerente();
        g->show();
        this->close();
    }
    else {
        Cadastro *cad = new Cadastro();
        cad->show();
        this->close();
    }
}

void Central::on_pushButton_controle_loja_clicked()
{
    if (cargoUsuario == "gerente") {
        Controledaloja *c = new Controledaloja;
        c->setOrigem(Controledaloja::OrigemCentral);
        c->show();
        c->atualizarDados();
        this->close();
    }
    else {
        Estoque *e = new Estoque();
        e->setOrigem(Estoque::OrigemCentralFunc);
        e->show();
        this->close();
    }
}
void Central::on_pushButton_tela_cliente_clicked()
{
    principalui *pri = new principalui();
    pri->setTipoUsuario(this->cargoUsuario);
    pri->setAttribute(Qt::WA_DeleteOnClose);
    pri->show();
    this->close();
}


void Central::on_pushButton_tela_cliente_2_clicked()
{
    DebugMenu *menu = new DebugMenu(this);
    if (menu->exec() == QDialog::Accepted) {
        this->close();
    }
    delete menu;
}

void Central::on_pushButton_Debug_clicked()
{
    DebugMenu *menu = new DebugMenu(this);
    if (menu->exec() == QDialog::Accepted) {
        this->close();
    }
    delete menu;
}
