#include "Cadastro.hpp"
#include "ui_Cadastro.h"
#include "Estoque.hpp"
#include <QMessageBox>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QDebug>
#include "help1.hpp"
#include <QHeaderView>
#include "debug.hpp"
#include "central.hpp"

Cadastro::Cadastro(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::CadastroUI)
{
    ui->setupUi(this);

if (!iniciarBancoDeDados()) {
        mostrarMensagem("Falha ao conectar ou abrir o banco de dados.", true);
    } else if (!criarTabelas()) {
        mostrarMensagem("Falha ao criar tabelas no banco de dados.", true);
    }

    ui->tableWidget_clientes->setColumnCount(5);
    ui->tableWidget_clientes->setHorizontalHeaderLabels({"Nome", "CPF", "CEP", "Telefonei","E-mail"});
    ui->tableWidget_clientes->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_clientes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_clientes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_vinis->setColumnCount(8);
    ui->tableWidget_vinis->setHorizontalHeaderLabels({"Nome", "Artista", "Ano", "Gênero", "Condição", "Preço", "Quantidade","codigo"});
    ui->tableWidget_vinis->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_vinis->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_vinis->setEditTriggers(QAbstractItemView::NoEditTriggers);


    atualizarTabelaClientes();
    atualizarTabelaVinis();

    }

Cadastro::~Cadastro()
{
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}
bool Cadastro::iniciarBancoDeDados()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database/pf3.db");

    if (!db.open()) {
        qDebug() << "Erro ao abrir o BD:" << db.lastError().text();
        return false;
    }
    return true;
}
// db
bool Cadastro::criarTabelas()
{
    QSqlQuery query;
    bool sucesso = true;

    // Tabela Cliente
    if (!query.exec("CREATE TABLE IF NOT EXISTS clientes ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "nome TEXT, "
                    "cpf TEXT UNIQUE, "
                    "cep TEXT, "
                    "telefone TEXT, "
                    "email TEXT"
                    ")")) {
        qDebug() << "Erro ao criar clientes:" << query.lastError().text();
        sucesso = false;
    }

    // Tabela Vinil
    if (!query.exec("CREATE TABLE IF NOT EXISTS vinis ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "nome TEXT NOT NULL, "
                    "artista TEXT NOT NULL, "
                    "ano TEXT NOT NULL, "
                    "genero TEXT NOT NULL, "
                    "condicao TEXT NOT NULL, "
                    "preco REAL NOT NULL, "
                    "quantidade INTEGER NOT NULL, "
                    "codigo TEXT UNIQUE"
                    ")")) {
        qDebug() << "Erro ao criar vinis:" << query.lastError().text();
        sucesso = false;
    }
    return sucesso;
}
// Clientes
void Cadastro::on_pushButton_pagina_cliente_clicked()
{
    ui->stackedWidget_paginas->setCurrentIndex(0);
}

void Cadastro::on_pushButton_pagina_vinil_clicked()
 {

    ui->stackedWidget_paginas->setCurrentIndex(1);

 }
void Cadastro::on_pushButton_pagina_itens_clicked()
{
    Estoque *e = new Estoque();
    e->setOrigem(Estoque::OrigemCadastro);
    e->show();
this->close();
}



void Cadastro::mostrarMensagem(const QString &mensagem, bool erro)
{
    QMessageBox msgBox;
    msgBox.setText(mensagem);
    msgBox.setWindowTitle(erro ? "Erro" : "Sucesso");
    msgBox.setIcon(erro ? QMessageBox::Critical : QMessageBox::Information);
    msgBox.exec();
}

// Tela do Cadastro do  Cliente
void Cadastro::limparCamposCliente()
{
    ui->lineEdit_nome_2->clear();
    ui->lineEdit_CPF->clear();
    ui->lineEdit_CEP->clear();
    ui->lineEdit_Telefone->clear();
    ui->lineEdit_E_mail_2->clear();
    indiceEdicaoCliente = -1;
    ui->pushButton_salvar_cliente->setText("Salvar");
}

bool Cadastro::validarCamposCliente()
{
    return !ui->lineEdit_nome_2->text().isEmpty() &&
           !ui->lineEdit_CPF->text().isEmpty() &&
           !ui->lineEdit_CEP->text().isEmpty();
}


void Cadastro::atualizarTabelaClientes()
{
    ui->tableWidget_clientes->setRowCount(0);
    QSqlQuery query("SELECT id, nome, cpf, cep, telefone, email FROM clientes ORDER BY nome ASC");
    int row = 0;

    while (query.next()) {
        ui->tableWidget_clientes->insertRow(row);

        QTableWidgetItem *nomeItem = new QTableWidgetItem(query.value(1).toString()); // Nome
        nomeItem->setData(Qt::UserRole, query.value(0).toInt());

        ui->tableWidget_clientes->setItem(row, 0, nomeItem);
        ui->tableWidget_clientes->setItem(row, 1, new QTableWidgetItem(query.value(2).toString())); // CPF
        ui->tableWidget_clientes->setItem(row, 2, new QTableWidgetItem(query.value(3).toString())); // CEP
        ui->tableWidget_clientes->setItem(row, 3, new QTableWidgetItem(query.value(4).toString())); // Telefone
        ui->tableWidget_clientes->setItem(row, 4, new QTableWidgetItem(query.value(5).toString())); // E-mail
        row++;
    }
    idEdicaoCliente = -1;
   // ui->pushButton_salvar_cliente->setText("Salvar");
}

void Cadastro::on_pushButton_salvar_cliente_clicked()
{
    if (!validarCamposCliente()) {
        mostrarMensagem("Preencha o Nome, o CPF e o CEP do cliente.", true);
        return;
    }

    QSqlQuery query;
    QString mensagemSucesso;

if (idEdicaoCliente == -1) {
        query.prepare("INSERT INTO clientes (nome, cpf, cep, telefone, email) "
                      "VALUES (:nome, :cpf, :cep, :telefone, :email)");
        mensagemSucesso = "Cliente cadastrado com sucesso!";
    } else {
        query.prepare("UPDATE clientes SET nome = :nome, cpf = :cpf, cep = :cep, "
                      "telefone = :telefone, email = :email WHERE id = :id");
        query.bindValue(":id", idEdicaoCliente);
        mensagemSucesso = "Cliente alterado com sucesso!";
    }

    query.bindValue(":nome", ui->lineEdit_nome_2->text());
    query.bindValue(":cpf", ui->lineEdit_CPF->text());
    query.bindValue(":cep", ui->lineEdit_CEP->text());
    query.bindValue(":telefone", ui->lineEdit_Telefone->text());
    query.bindValue(":email", ui->lineEdit_E_mail_2->text());

    if (!query.exec()) {
        mostrarMensagem("Erro ao salvar/atualizar cliente. Verifique se o CPF já existe. Erro: " + query.lastError().text(), true);
        return;
    }
    limparCamposCliente();
    atualizarTabelaClientes();
    mostrarMensagem(mensagemSucesso);
}

void Cadastro::on_tableWidget_clientes_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    QVariant data = ui->tableWidget_clientes->item(row, 0)->data(Qt::UserRole);
    idEdicaoCliente = data.isValid() ? data.toInt() : -1;
    if (idEdicaoCliente != -1) {
        ui->lineEdit_nome_2->setText(ui->tableWidget_clientes->item(row, 0)->text());
        ui->lineEdit_CPF->setText(ui->tableWidget_clientes->item(row, 1)->text());
        ui->lineEdit_CEP->setText(ui->tableWidget_clientes->item(row, 2)->text());
        ui->lineEdit_Telefone->setText(ui->tableWidget_clientes->item(row, 3)->text());
        ui->lineEdit_E_mail_2->setText(ui->tableWidget_clientes->item(row, 4)->text());

      //  ui->pushButton_salvar_cliente->setText("Atualizar");
    }
}
void Cadastro::on_pushButton_alterar_cliente_clicked()
{
    if (idEdicaoCliente == -1) {
        mostrarMensagem("Selecione um cliente na tabela para alterar.", true);
        return;
    } ui->pushButton_salvar_cliente->setText("Atualizar");
    mostrarMensagem("Modo de alteração ativo. Volte para cadastrado e edite os campos e clique em 'Atualizar' (Salvar).", false);

}

void Cadastro::on_pushButton_excluir_cliente_clicked()
{
    int linhaSelec = ui->tableWidget_clientes->currentRow();

    if (linhaSelec < 0) {
        mostrarMensagem("Por favor, selecione um cliente para excluir!", true);
        return;
    }
int idParaExcluir = ui->tableWidget_clientes->item(linhaSelec, 0)->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton resposta;
    resposta = QMessageBox::question(this, "Confirmação",
                                     "Tem certeza que deseja excluir o cliente selecionado?",
                                     QMessageBox::Yes | QMessageBox::No);

if (resposta == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM clientes WHERE id = :id");
        query.bindValue(":id", idParaExcluir);

        if (query.exec()) {
            limparCamposCliente();
            atualizarTabelaClientes();
            mostrarMensagem("Cliente excluído com sucesso!");
        } else {
            mostrarMensagem("Erro ao excluir cliente: " + query.lastError().text(), true);
        }
    }
}

// Tela Cadastro Vinil
void Cadastro::limparCamposVinil()
{
    ui->lineEdit_nome_do_disco->clear();
    ui->lineEdit_Nome_artistas->clear();
    ui->lineEdit_disco_ano->clear();
    ui->comboBox_Genero_discos->setCurrentIndex(0);
    ui->comboBox_condicao_discos->setCurrentIndex(0);
    ui->lineEdit_disco_preco->clear();
    ui->lineEdit_disco_quantidade->clear();
    ui->lineEdit_disco_codigo->clear();

    indiceEdicaoVinil = -1;
    ui->pushButton_salvar_vinil->setText("Salvar");
}

bool Cadastro::validarCamposVinil()
{
    return !ui->lineEdit_nome_do_disco->text().isEmpty() &&
           !ui->lineEdit_Nome_artistas->text().isEmpty() &&
           !ui->lineEdit_disco_preco->text().isEmpty() &&
           !ui->lineEdit_disco_quantidade->text().isEmpty() &&
           !ui->lineEdit_disco_codigo->text().isEmpty() &&
           ui->comboBox_Genero_discos->currentIndex() > 0 &&
           ui->comboBox_condicao_discos->currentIndex() > 0;


}

void Cadastro::atualizarTabelaVinis()
{
    ui->tableWidget_vinis->setRowCount(0);
    QSqlQuery query("SELECT id, nome, artista, ano, genero, condicao, preco, quantidade, codigo FROM vinis ORDER BY nome ASC");
    int row = 0;

    while (query.next()) {
        ui->tableWidget_vinis->insertRow(row);

        QTableWidgetItem *nomeItem = new QTableWidgetItem(query.value(1).toString()); // Nome
        nomeItem->setData(Qt::UserRole, query.value(0).toInt());

        ui->tableWidget_vinis->setItem(row, 0, nomeItem);
        ui->tableWidget_vinis->setItem(row, 1, new QTableWidgetItem(query.value(2).toString())); // Artista
        ui->tableWidget_vinis->setItem(row, 2, new QTableWidgetItem(query.value(3).toString())); // Ano
        ui->tableWidget_vinis->setItem(row, 3, new QTableWidgetItem(query.value(4).toString())); // Gênero
        ui->tableWidget_vinis->setItem(row, 4, new QTableWidgetItem(query.value(5).toString())); // Condição
        ui->tableWidget_vinis->setItem(row, 5, new QTableWidgetItem(query.value(6).toString())); // Preço
        ui->tableWidget_vinis->setItem(row, 6, new QTableWidgetItem(query.value(7).toString())); // Quantidade
        ui->tableWidget_vinis->setItem(row, 7, new QTableWidgetItem(query.value(8).toString())); // Código
        row++;
    }
    idEdicaoVinil = -1;
    ui->pushButton_salvar_vinil->setText("Salvar");
}
void Cadastro::on_pushButton_salvar_vinil_clicked()
{
    if (!validarCamposVinil()) {
        mostrarMensagem("Preencha Nome, Artista, Preço ,Quantidade do vinil e o codigo", true);
        return;
    }
QSqlQuery query;
    QString mensagemSucesso;

    double preco = ui->lineEdit_disco_preco->text().toDouble();
    int quantidade = ui->lineEdit_disco_quantidade->text().toInt();


    if (idEdicaoVinil == -1) {
        query.prepare("INSERT INTO vinis (nome, artista, ano, genero, condicao, preco, quantidade, codigo) "
                      "VALUES (:nome, :artista, :ano, :genero, :condicao, :preco, :quantidade, :codigo)");
        mensagemSucesso = "Vinil cadastrado com sucesso!";
    } else {
        query.prepare("UPDATE vinis SET nome = :nome, artista = :artista, ano = :ano, "
                      "genero = :genero, condicao = :condicao, preco = :preco, "
                      "quantidade = :quantidade, codigo = :codigo WHERE id = :id");
        query.bindValue(":id", idEdicaoVinil);
        mensagemSucesso = "Vinil alterado com sucesso!";
    }

    query.bindValue(":nome", ui->lineEdit_nome_do_disco->text());
    query.bindValue(":artista", ui->lineEdit_Nome_artistas->text());
    query.bindValue(":ano", ui->lineEdit_disco_ano->text());
    query.bindValue(":genero", ui->comboBox_Genero_discos->currentText());
    query.bindValue(":condicao", ui->comboBox_condicao_discos->currentText());
    query.bindValue(":preco", preco);
    query.bindValue(":quantidade", quantidade);
    query.bindValue(":codigo", ui->lineEdit_disco_codigo->text());


    if (!query.exec()) {
        mostrarMensagem("Erro ao salvar/atualizar vinil. Verifique se o código já existe. Erro: " + query.lastError().text(), true);
        return;
    }

    limparCamposVinil();
    atualizarTabelaVinis();
    mostrarMensagem(mensagemSucesso);
}

void Cadastro::on_tableWidget_vinis_cellClicked(int row, int column)
{
    Q_UNUSED(column)

    QVariant data = ui->tableWidget_vinis->item(row, 0)->data(Qt::UserRole);
    idEdicaoVinil = data.isValid() ? data.toInt() : -1;

    if (idEdicaoVinil != -1) {
        ui->lineEdit_nome_do_disco->setText(ui->tableWidget_vinis->item(row, 0)->text());
        ui->lineEdit_Nome_artistas->setText(ui->tableWidget_vinis->item(row, 1)->text());
        ui->lineEdit_disco_ano->setText(ui->tableWidget_vinis->item(row, 2)->text());
        ui->comboBox_Genero_discos->setCurrentText(ui->tableWidget_vinis->item(row, 3)->text());
        ui->comboBox_condicao_discos->setCurrentText(ui->tableWidget_vinis->item(row, 4)->text());
        ui->lineEdit_disco_preco->setText(ui->tableWidget_vinis->item(row, 5)->text());
        ui->lineEdit_disco_quantidade->setText(ui->tableWidget_vinis->item(row, 6)->text());
        ui->lineEdit_disco_codigo->setText(ui->tableWidget_vinis->item(row, 7)->text());

      //  ui->pushButton_salvar_vinil->setText("Atualizar");
    }
}
void Cadastro::on_pushButton_alterar_vinil_clicked()
{
    if (idEdicaoVinil == -1) {
        mostrarMensagem("Selecione um vinil na tabela para alterar.", true);
        return;
    }ui->pushButton_salvar_vinil->setText("Atualizar");
    mostrarMensagem("Modo de alteração ativo. Volte para Cadastro e edite os campos e clique em 'Atualizar' (Salvar).", false);
}
void Cadastro::on_pushButton_excluir_vinil_clicked()
{
    int linhaSelec = ui->tableWidget_vinis->currentRow();

    if (linhaSelec < 0) {
        mostrarMensagem("Por favor, selecione um vinil para excluir!", true);
        return;
    }
    int idParaExcluir = ui->tableWidget_vinis->item(linhaSelec, 0)->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton resposta;
    resposta = QMessageBox::question(this, "Confirmação",
                                     "Tem certeza que deseja excluir o vinil selecionado?",
                                     QMessageBox::Yes | QMessageBox::No);

    if (resposta == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM vinis WHERE id = :id");
        query.bindValue(":id", idParaExcluir);

        if (query.exec()) {
            limparCamposVinil();
            atualizarTabelaVinis();
            mostrarMensagem("Vinil excluído com sucesso!");
        } else {
            mostrarMensagem("Erro ao excluir vinil: " + query.lastError().text(), true);
        }
    }
}

void Cadastro::on_pushButton_Debug_clicked()

{

DebugMenu *menu = new DebugMenu(this);

if (menu->exec() == QDialog::Accepted) {

this->close();

}

delete menu;

}
void Cadastro::on_pushButton_voltar_clicked()
{
    Central *c = new Central("funcionario");
    c->setAttribute(Qt::WA_DeleteOnClose);
    c->show();
    this->close();
}

