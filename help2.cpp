#include "help2.hpp"
#include "ui_help2.h"
#include "debug.hpp"
Help2::Help2(QWidget *parent)
    : QDialog(parent), ui(new Ui::Help2)
{
    ui->setupUi(this);
    connect(ui->okButton, &QPushButton::clicked, this, &Help2::accept);
    // Faz o botão responder ao Enter
    ui->okButton->setDefault(true);
    ui->okButton->setAutoDefault(true);
}
Help2::~Help2()
{
    delete ui;
}
void Help2::on_pushButton_Debug_clicked()
{
DebugMenu *menu = new DebugMenu(this);
if (menu->exec() == QDialog::Accepted) {
this->close();
}
delete menu;
}
