#include "ui_firstlogin.h"
#include <QIcon>
#include "firstlogin.hpp"
#include "mainwindow.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include "debug.hpp"
#include "Cadastro.hpp"
#include "central.hpp"
#include "principalui.hpp"
#include "help2.hpp"
#include "registro.hpp"

firstlogin::firstlogin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::firstloginUI)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->toolButton_3->setIcon(QIcon("images/eye_close.jpg"));
    ui->toolButton_3->setIconSize(QSize(32, 32));
    connect(ui->toolButton_3, &QToolButton::clicked, this, &firstlogin::togglePasswordVisibility);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

firstlogin::~firstlogin()
{
    delete ui;
}
bool firstlogin::openDatabase()
{
    QSqlDatabase db;

    if (QSqlDatabase::contains("pf3_connection")) {
        db = QSqlDatabase::database("pf3_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "pf3_connection");
        db.setDatabaseName("database/pf3.db");
    }

    if (!db.open()) {
        qDebug() << "Erro ao abrir banco:" << db.lastError().text();
        QMessageBox::critical(this, "Erro",
                              "Não foi possível abrir o banco de dados.");
        return false;
    }

    return true;
}

void firstlogin::togglePasswordVisibility()
{
    if (ui->passwordLineEdit->echoMode() == QLineEdit::Password) {
        // Mostra a senha
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->toolButton_3->setIcon(QIcon("images/eye_open.png"));
    } else {
        // Oculta a senha
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        ui->toolButton_3->setIcon(QIcon("images/eye_close.jpg"));
    }
}
void firstlogin::on_pushButton_clicked()
{
    QString email = ui->emailLineEdit->text().trimmed();
    QString senhaDigitada = ui->passwordLineEdit->text();

    if (email.isEmpty() || senhaDigitada.isEmpty()) {
        QMessageBox::warning(this, "Erro", "Informe e-mail e senha.");
        return;
    }

    if (!openDatabase()) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("pf3_connection");

    // 1) FUNCIONÁRIOS: email + CPF (como senha)
    QSqlQuery queryFunc(db);
    queryFunc.prepare("SELECT funcao, email FROM funcionarios "
                      "WHERE email = :email AND cpf = :senha");
    queryFunc.bindValue(":email", email);
    queryFunc.bindValue(":senha", senhaDigitada);

    if (!queryFunc.exec()) {
        QMessageBox::warning(this, "Erro",
                             "Erro ao consultar funcionarios: "
                             + queryFunc.lastError().text());
        return;
    }

    if (queryFunc.next()) {
        QString cargoNoBanco = queryFunc.value("funcao").toString();

        QMessageBox::information(this, "Login",
                                 "Bem-vindo Colaborador! Cargo: " + cargoNoBanco);

        if (cargoNoBanco.compare("Gerente", Qt::CaseInsensitive) == 0) {
            Central *centralWindow = new Central("gerente");
            centralWindow->setAttribute(Qt::WA_DeleteOnClose);
            centralWindow->show();
            this->close();
        } else {
            Central *centralWindow = new Central("funcionario");
            centralWindow->setAttribute(Qt::WA_DeleteOnClose);
            centralWindow->show();
            this->close();
        }
        return;
    }

    // 2) DONO DA LOJA (Administrador) - email + password
    QSqlQuery queryDono(db);
    queryDono.prepare("SELECT name FROM dono_da_loja "
                      "WHERE email = :email AND password = :senha");
    queryDono.bindValue(":email", email);
    queryDono.bindValue(":senha", senhaDigitada);

    if (!queryDono.exec()) {
        QMessageBox::warning(this, "Erro",
                             "Erro ao consultar dono_da_loja: "
                             + queryDono.lastError().text());
        return;
    }

    if (queryDono.next()) {
        QString nomeDono = queryDono.value("name").toString();

        QMessageBox::information(this, "Login",
                                 "Bem-vindo Administrador: " + nomeDono);

        Central *centralWindow = new Central("gerente");
        centralWindow->setAttribute(Qt::WA_DeleteOnClose);
        centralWindow->show();
        this->close();
        return;
    }

    // 3) CLIENTE: email + CPF (como senha)
    QSqlQuery queryCliente(db);
    queryCliente.prepare("SELECT nome FROM clientes "
                         "WHERE email = :email AND cpf = :senha");
    queryCliente.bindValue(":email", email);
    queryCliente.bindValue(":senha", senhaDigitada);

    if (!queryCliente.exec()) {
        QMessageBox::warning(this, "Erro",
                             "Erro ao consultar clientes: "
                             + queryCliente.lastError().text());
        return;
    }

    if (queryCliente.next()) {
        QString nomeCliente = queryCliente.value("nome").toString();
        QMessageBox::information(this, "Login",
                                 "Bem-vindo Cliente: " + nomeCliente);

        principalui *telaCliente = new principalui();
        telaCliente->setAttribute(Qt::WA_DeleteOnClose);
        telaCliente->show();
        this->close();
        return;
    }

    // Se chegou aqui, nenhum usuário bateu
    QMessageBox::warning(
        this,
        "Erro",
        "Login inválido.\n\n"
        "Se for Cliente: Use seu E-mail e CPF (como senha).\n"
        "Se for Funcionário: Use seu E-mail da loja e CPF (como senha).\n"
        "Se for Dono da Loja: Use o E-mail e a Senha cadastrados no registro."
    );
}





void firstlogin::on_backButton_clicked()
{
    registro *registroWindow = new registro();
    registroWindow->setAttribute(Qt::WA_DeleteOnClose);
    registroWindow->show();
    this->close();
}

void firstlogin::on_pushButton_Debug_clicked()

{

DebugMenu *menu = new DebugMenu(this);

if (menu->exec() == QDialog::Accepted) {

this->close();

}

delete menu;

}

void firstlogin::on_pushButton_Debug_2_clicked()
{
    Help2 *helpWindow = new Help2(this);
    helpWindow->setModal(true);
    helpWindow->show();
}
