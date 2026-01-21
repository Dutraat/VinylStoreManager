#include "help1.hpp"
#include "ui_help1.h"
#include "debug.hpp"

Help1::Help1(QWidget *parent)
    : QDialog(parent), ui(new Ui::Help1)
{
    ui->setupUi(this);

    connect(ui->okButton, &QPushButton::clicked, this, &Help1::accept);

    // Faz o botão responder ao Enter
    ui->okButton->setDefault(true);
    ui->okButton->setAutoDefault(true);
}

Help1::~Help1()
{
    delete ui;
}

void Help1::on_pushButton_Debug_clicked()
{

DebugMenu *menu = new DebugMenu(this);

if (menu->exec() == QDialog::Accepted) {

this->close();

}

delete menu;

}
