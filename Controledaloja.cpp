#include "Controledaloja.hpp"
#include "ui_Controledaloja.h"
#include "Gerente.hpp"
#include "debug.hpp"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidgetItem>
#include <QInputDialog>
#include <QLineEdit>
#include <QtSql/QSqlDatabase>
#include "central.hpp"

Controledaloja::Controledaloja(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Controledaloja)
{
    ui->setupUi(this);

    ui->tableWidget_EstoqueCritico->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    if (!db.isOpen()) {
        db.open();
    }

    atualizarDados();
}

Controledaloja::~Controledaloja()
{
    delete ui;
}

void Controledaloja::setGerente(Gerente *g)
{
    gerente = g;
}

void Controledaloja::atualizarDados()
{
    double dia    = obterTotalVendasDia();
    double semana = obterTotalVendasSemana();
    double mes    = obterTotalVendasMes();
    double ticket = obterTicketMedio();

    ui->label_VendasDia->setText(QString("R$ %1").arg(dia, 0, 'f', 2));
    ui->label_VendasSemana->setText(QString("R$ %1").arg(semana, 0, 'f', 2));
    ui->label_VendasMes->setText(QString("R$ %1").arg(mes, 0, 'f', 2));
    ui->label_TicketMedio->setText(QString("R$ %1").arg(ticket, 0, 'f', 2));

    carregarEstoqueCritico();
    carregarFuncionarios();
}

//produtos com estoque crítico (quantidade <= limiteCritico) na tabela
void Controledaloja::carregarEstoqueCritico()
{
    ui->tableWidget_EstoqueCritico->setRowCount(0);
    ui->tableWidget_EstoqueCritico->setColumnWidth(0, 190);
    ui->tableWidget_EstoqueCritico->setColumnWidth(1, 160);

    int limiteCritico = 3;

    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    QSqlQuery query(db);
    query.prepare("SELECT nome, artista, ano, preco, genero, quantidade, codigo "
                  "FROM vinis "
                  "WHERE quantidade <= :limite "
                  "ORDER BY quantidade ASC");
    query.bindValue(":limite", limiteCritico);

    if (!query.exec()) {
        qDebug() << "Erro ao buscar estoque crítico:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget_EstoqueCritico->insertRow(row);

        ui->tableWidget_EstoqueCritico->setItem(
            row, 0, new QTableWidgetItem(query.value("nome").toString()));
        ui->tableWidget_EstoqueCritico->setItem(
            row, 1, new QTableWidgetItem(query.value("artista").toString()));
        ui->tableWidget_EstoqueCritico->setItem(
            row, 2, new QTableWidgetItem(query.value("ano").toString()));
        ui->tableWidget_EstoqueCritico->setItem(
            row, 3, new QTableWidgetItem(
                QString("R$ %1").arg(query.value("preco").toDouble(), 0, 'f', 2)));
        ui->tableWidget_EstoqueCritico->setItem(
            row, 4, new QTableWidgetItem(query.value("genero").toString()));
        ui->tableWidget_EstoqueCritico->setItem(
            row, 5, new QTableWidgetItem(query.value("quantidade").toString()));
        ui->tableWidget_EstoqueCritico->setItem(
            row, 6, new QTableWidgetItem(query.value("codigo").toString()));

        row++;
    }
}



void Controledaloja::carregarFuncionarios()
{
    ui->tableWidget_funcionarios->setRowCount(0);

    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    QSqlQuery query(db);
    query.prepare("SELECT nome, cpf, pis, email, funcao, salario, cep, telefone "
                  "FROM funcionarios ORDER BY nome ASC");

    if (!query.exec()) {
        qDebug() << "Erro ao carregar funcionários:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget_funcionarios->insertRow(row);
        ui->tableWidget_funcionarios->setItem(row, 0,
            new QTableWidgetItem(query.value("nome").toString()));
        ui->tableWidget_funcionarios->setItem(row, 1,
            new QTableWidgetItem(query.value("cpf").toString()));
        ui->tableWidget_funcionarios->setItem(row, 2,
            new QTableWidgetItem(query.value("pis").toString()));
        ui->tableWidget_funcionarios->setItem(row, 3,
            new QTableWidgetItem(query.value("email").toString()));
        ui->tableWidget_funcionarios->setItem(row, 4,
            new QTableWidgetItem(query.value("funcao").toString()));
        ui->tableWidget_funcionarios->setItem(row, 5,
            new QTableWidgetItem(query.value("salario").toString()));
        ui->tableWidget_funcionarios->setItem(row, 6,
            new QTableWidgetItem(query.value("cep").toString()));
        ui->tableWidget_funcionarios->setItem(row, 7,
            new QTableWidgetItem(query.value("telefone").toString()));
        row++;
    }
}


void Controledaloja::on_pushButton_demitir_clicked()
{
    int linhaSelec = ui->tableWidget_funcionarios->currentRow();

    if (linhaSelec < 0) {
        QMessageBox::warning(this,
                             "Atenção",
                             "Por favor, selecione um funcionário para demitir!");
        return;
    }

    QTableWidgetItem *emailItem =
        ui->tableWidget_funcionarios->item(linhaSelec, 3);
    if (!emailItem) {
        QMessageBox::critical(this,
                              "Erro",
                              "Não foi possível obter o e-mail do funcionário.");
        return;
    }

    QString email = emailItem->text();

    QMessageBox::StandardButton resposta;
    resposta = QMessageBox::question(
        this,
        "Confirmação",
        "Tem certeza que deseja demitir o funcionário selecionado?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (resposta != QMessageBox::Yes)
        return;

    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    QSqlQuery query(db);
    query.prepare("DELETE FROM funcionarios WHERE email = :email");
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Erro ao demitir funcionário:" << query.lastError().text();
        QMessageBox::critical(this,
                              "Erro",
                              "Erro ao demitir o funcionário.");
        return;
    }

    carregarFuncionarios();

    if (gerente) {
        gerente->atualizarTabelaFuncionarios();
    }

    QMessageBox::information(this,
                             "Sucesso",
                             "Funcionário demitido com sucesso!");
}


void Controledaloja::on_pushButton_Debug_clicked()
{

DebugMenu *menu = new DebugMenu(this);

if (menu->exec() == QDialog::Accepted) {

this->close();

}

delete menu;
}

void Controledaloja::on_pushButton_promover_clicked()
{
    int linhaSelec = ui->tableWidget_funcionarios->currentRow();

    if (linhaSelec < 0) {
        QMessageBox::warning(this,
                             "Atenção",
                             "Por favor, selecione um funcionário para promover!");
        return;
    }

    QTableWidgetItem *emailItem  = ui->tableWidget_funcionarios->item(linhaSelec, 3);
    QTableWidgetItem *funcaoItem = ui->tableWidget_funcionarios->item(linhaSelec, 4);
    QTableWidgetItem *salarioItem= ui->tableWidget_funcionarios->item(linhaSelec, 5);

    if (!emailItem || !funcaoItem || !salarioItem) {
        QMessageBox::critical(this,
                              "Erro",
                              "Não foi possível obter os dados do funcionário.");
        return;
    }

    QString email        = emailItem->text();
    QString funcaoAtual  = funcaoItem->text();
    QString salarioAtual = salarioItem->text();

    QStringList funcoes;
    funcoes << "Gerente"
            << "Organizador de Estoque"
            << "Caixa"
            << "Supervisor";

    bool okFunc = false;
    QString novaFuncao = QInputDialog::getItem(
        this,
        "Promover funcionário",
        "Selecione a nova função para este funcionário:",
        funcoes,
        qMax(0, funcoes.indexOf(funcaoAtual)),
        false,
        &okFunc
    );

    if (!okFunc || novaFuncao == funcaoAtual)
        return;

    bool okSal = false;
    QString novoSalarioStr = QInputDialog::getText(
        this,
        "Alterar salário",
        "Informe o novo salário para este funcionário:",
        QLineEdit::Normal,
        salarioAtual,
        &okSal
    );

    if (!okSal || novoSalarioStr.trimmed().isEmpty())
        return;

    bool convOk = false;
    double novoSalario = novoSalarioStr.replace(",", ".").toDouble(&convOk);
    if (!convOk || novoSalario < 0) {
        QMessageBox::warning(this,
                             "Valor inválido",
                             "Digite um salário numérico válido.");
        return;
    }

    QMessageBox::StandardButton resp = QMessageBox::question(
        this,
        "Confirmação",
        "Tem certeza que deseja promover o funcionário para a função \""
            + novaFuncao + "\" e atualizar o salário?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (resp != QMessageBox::Yes)
        return;

    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    QSqlQuery query(db);
    query.prepare("UPDATE funcionarios "
                  "SET funcao = :funcao, salario = :salario "
                  "WHERE email = :email");
    query.bindValue(":funcao",  novaFuncao);
    query.bindValue(":salario", novoSalario);
    query.bindValue(":email",   email);

    if (!query.exec()) {
        qDebug() << "Erro ao promover funcionário:" << query.lastError().text();
        QMessageBox::critical(this,
                              "Erro",
                              "Erro ao promover o funcionário.");
        return;
    }

    carregarFuncionarios();

    if (gerente) {
        gerente->atualizarTabelaFuncionarios();
    }

    QMessageBox::information(this,
                             "Sucesso",
                             "Funcionário promovido e salário atualizado com sucesso!");
}



void Controledaloja::on_pushButton_VoltarGerente_clicked()
{
    this->close();

    if (origem == OrigemGerente && gerente) {
        gerente->atualizarTabelaFuncionarios();
        gerente->show();
        gerente->raise();
        gerente->activateWindow();
    } else if (origem == OrigemCentral) {
        Central *cen = new Central("gerente");
        cen->show();
    }
}




double Controledaloja::obterTotalVendasDia()
{
    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    QSqlQuery query(db);
    query.exec("SELECT SUM(valor_total) FROM vendas "
               "WHERE date(data_hora) = date('now','localtime')");
    if (query.next())
        return query.value(0).toDouble();
    return 0.0;
}

double Controledaloja::obterTotalVendasSemana()
{
    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    QSqlQuery query(db);
    query.exec("SELECT SUM(valor_total) FROM vendas "
               "WHERE date(data_hora) >= date('now','localtime','-7 days')");
    if (query.next())
        return query.value(0).toDouble();
    return 0.0;
}

double Controledaloja::obterTotalVendasMes()
{
    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    QSqlQuery query(db);
    query.exec("SELECT SUM(valor_total) FROM vendas "
               "WHERE strftime('%Y-%m', data_hora) = "
               "strftime('%Y-%m', 'now','localtime')");
    if (query.next())
        return query.value(0).toDouble();
    return 0.0;
}

double Controledaloja::obterTicketMedio()
{
    QSqlDatabase db = QSqlDatabase::database("pf3_connection_main");
    QSqlQuery query(db);
    query.exec("SELECT AVG(valor_total) FROM vendas");
    if (query.next())
        return query.value(0).toDouble();
    return 0.0;
}

