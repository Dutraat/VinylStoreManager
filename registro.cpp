#include "registro.hpp"
#include "ui_registro.h"
#include "debug.hpp"

#include <QScrollBar>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QIcon>

#include "firstlogin.hpp"
#include "principalui.hpp"

registro::registro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::registro)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->centralwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

registro::~registro()
{
    delete ui;
}

bool registro::openDatabase()
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

void registro::on_pushButton_clicked()
{
    QString nome     = ui->nameLineEdit->text().trimmed();
    QString email    = ui->emailLineEdit->text().trimmed();
    QString cpf      = ui->passwordLineEdit->text().trimmed();   // CPF
    QString telefone = ui->emailLineEdit_2->text().trimmed();
    QString cep      = ui->emailLineEdit_3->text().trimmed();

    if (nome.isEmpty() || email.isEmpty() || cpf.isEmpty()) {
        QMessageBox::warning(this, "Erro",
                             "Preencha nome, email e CPF.");
        return;
    }

    if (!openDatabase()) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("pf3_connection");
    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO clientes (nome, cpf, cep, telefone, email) "
        "VALUES (:nome, :cpf, :cep, :telefone, :email)"
    );

    query.bindValue(":nome", nome);
    query.bindValue(":cpf", cpf);
    query.bindValue(":cep", cep);
    query.bindValue(":telefone", telefone);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Erro ao inserir cliente:" << query.lastError().text();
        QMessageBox::warning(this, "Erro ao cadastrar",
                             "Não foi possível salvar o cliente.\n\nDetalhes: "
                             + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Sucesso",
                             "Cliente cadastrado com sucesso!");

    principalui *principalWindow = new principalui();
    principalWindow->setAttribute(Qt::WA_DeleteOnClose);
    principalWindow->show();
    this->close();
}

void registro::on_pushButton_2_clicked()
{
    DebugMenu *menu = new DebugMenu(this);
    if (menu->exec() == QDialog::Accepted) {
        this->close();
    }
    delete menu;
}

void registro::on_backButton_clicked()
{
    firstlogin *loginWindow = new firstlogin();
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();
    this->close();
}

