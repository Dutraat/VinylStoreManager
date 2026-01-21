#include "mainwindow.hpp"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->centralwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    ui->toolButton_3->setIcon(QIcon("images/eye_close.jpg")); // Botão da senha
    ui->toolButton_2->setIcon(QIcon("images/eye_close.jpg")); // Botão de confirmar senha

    ui->toolButton_3->setIconSize(QSize(32, 32));
    ui->toolButton_2->setIconSize(QSize(32, 32));


  //  connect(ui->toolButton_3, &QToolButton::clicked, this, &MainWindow::togglePasswordVisibility);
  //  connect(ui->toolButton_2, &QToolButton::clicked, this, &MainWindow::toggleConfirmPasswordVisibility);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openDatabase()
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

    QSqlQuery query(db);
    QString sqlCreate =
            "CREATE TABLE IF NOT EXISTS dono_da_loja ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT,"
            "email TEXT UNIQUE,"
            "password TEXT"
            ")";
    if (!query.exec(sqlCreate)) {
        qDebug() << "Erro ao criar tabela:" << query.lastError().text();
        QMessageBox::critical(this, "Erro",
                              "Não foi possível preparar a tabela de usuários.");
        return false;
    }

    return true;
}
void MainWindow::on_toolButton_3_clicked()
{
    if (ui->passwordLineEdit->echoMode() == QLineEdit::Password) {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->toolButton_3->setIcon(QIcon("images/eye_open.png"));
    } else {
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        ui->toolButton_3->setIcon(QIcon("images/eye_close.jpg"));
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString nome     = ui->nameLineEdit->text().trimmed();
    QString email    = ui->emailLineEdit->text().trimmed();
    QString senha    = ui->passwordLineEdit->text();
    QString confirma = ui->confirmPasswordLineEdit->text();

    if (nome.isEmpty() || email.isEmpty() || senha.isEmpty() || confirma.isEmpty()) {
        QMessageBox::warning(this, "Erro",
                             "Preencha todos os campos.");
        return;
    }

    if (senha != confirma) {
        QMessageBox::warning(this, "Erro",
                             "As senhas não coincidem. Digite novamente.");
        ui->passwordLineEdit->clear();
        ui->confirmPasswordLineEdit->clear();
        ui->passwordLineEdit->setFocus();
        return;
    }

    if (!openDatabase()) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("pf3_connection");
    QSqlQuery query(db);

    query.prepare("INSERT INTO dono_da_loja (name, email, password) "
                  "VALUES (:name, :email, :password)");
    query.bindValue(":name", nome);
    query.bindValue(":email", email);
    query.bindValue(":password", senha);

    if (!query.exec()) {
        qDebug() << "Erro ao inserir usuário:" << query.lastError().text();
        QMessageBox::warning(this, "Erro ao cadastrar",
                             "Não foi possível salvar o usuário.\n\nDetalhes: "
                             + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Sucesso",
                             "Usuário cadastrado com sucesso!");

    // Abre a tela principal e fecha a tela de cadastro
    firstlogin *loginWindow = new firstlogin();
    loginWindow->show();
    this->close();
}

void MainWindow::on_toolButton_2_clicked()
{
    if (ui->confirmPasswordLineEdit->echoMode() == QLineEdit::Password) {
        ui->confirmPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->toolButton_2->setIcon(QIcon("images/eye_open.png"));
    } else {
        ui->confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
        ui->toolButton_2->setIcon(QIcon("images/eye_close.jpg"));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    DebugMenu *menu = new DebugMenu(this);
    if (menu->exec() == QDialog::Accepted) {
        this->close();
    }
    delete menu;
}

void MainWindow::on_backButton_clicked()
{
    firstlogin *loginWindow = new firstlogin();
    loginWindow->show();
    this->close();
}
