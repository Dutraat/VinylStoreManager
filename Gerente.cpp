#include "Gerente.hpp"
#include "ui_Gerente.h"
#include "Estoque.hpp"
#include "central.hpp"
#include <QMessageBox>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDebug>
#include <QVariant>
#include "debug.hpp"
#include "central.hpp"


Gerente::Gerente(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::GerenteUI)
{
    ui->setupUi(this);
    ui->tableWidget_clientes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_clientes_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (!iniciarBancoDeDados()) {
        mostrarMensagem("Falha ao conectar ou abrir o banco de dados.", true);
    } else if (!criarTabelas()) {
        mostrarMensagem("Falha ao criar tabelas no banco de dados.", true);
    }

    ui->tableWidget_clientes->setColumnCount(5);
    ui->tableWidget_clientes->setHorizontalHeaderLabels({"Nome", "CPF", "CEP", "Telefone","E-mail"});
    ui->tableWidget_clientes->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_clientes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_clientes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_vinis->setColumnCount(8);
    ui->tableWidget_vinis->setHorizontalHeaderLabels({"Nome", "Artista", "Ano", "Gênero", "Condição", "Preço", "Quantidade","codigo"});
    ui->tableWidget_vinis->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_vinis->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_vinis->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget_clientes_2->setColumnCount(8);
    ui->tableWidget_clientes_2->setHorizontalHeaderLabels({"Nome", "CPF", "Função", "Salário", "PIS", "E-mail", "CEP", "Telefone"});
    ui->tableWidget_clientes_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_clientes_2->setSelectionBehavior(QAbstractItemView::SelectRows);
 ui->tableWidget_clientes_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    atualizarTabelaClientes();
    atualizarTabelaVinis();
    atualizarTabelaFuncionarios();

}

Gerente::~Gerente()
{
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}

bool Gerente::iniciarBancoDeDados()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database/pf3.db");

    if (!db.open()) {
        qDebug() << "Erro ao abrir o BD:" << db.lastError().text();
        return false;
    }
    return true;
}

bool Gerente::criarTabelas()
{
    QSqlQuery query;
    bool sucesso = true;

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

    if (!query.exec("CREATE TABLE IF NOT EXISTS vinis ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "nome TEXT, "
                    "artista TEXT, "
                    "ano TEXT, "
                    "genero TEXT, "
                    "condicao TEXT, "
                    "preco REAL, "
                    "quantidade INTEGER, "
                    "codigo TEXT UNIQUE"
                    ")")) {
        qDebug() << "Erro ao criar vinis:" << query.lastError().text();
        sucesso = false;
    }
    if (!query.exec("CREATE TABLE IF NOT EXISTS funcionarios ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "nome VARCHAR(100) NOT NULL, "
                    "cpf VARCHAR(14) NOT NULL UNIQUE, "
                    "funcao VARCHAR(50) NOT NULL, "
                    "salario VARCHAR REAL NOT NULL, "
                    "pis VARCHAR(11) UNIQUE, "
                    "email VARCHAR(100), "
                    "senha TEXT, "
                    "cep VARCHAR(10), "
                    "telefone VARCHAR(20))")) {
        qDebug() << "Erro ao criar tabela de funcionários:" << query.lastError().text();
        return false;
    }query.exec("ALTER TABLE funcionarios ADD COLUMN senha TEXT");

    return sucesso;
}

void Gerente::on_pushButton_pagina_cliente_clicked()
{
    ui->stackedWidget_paginas_funcionario->setCurrentIndex(0);
}

void Gerente::on_pushButton_pagina_vinil_clicked()
 {
    ui->stackedWidget_paginas_funcionario->setCurrentIndex(2);
 }

void Gerente::on_pushButton_pagina_funcionario_clicked()
{
    ui->stackedWidget_paginas_funcionario->setCurrentIndex(1);
}

void Gerente::on_pushButton_pagina_itens_clicked()
{
    Estoque *e = new Estoque();
    e->setOrigem(Estoque::OrigemGerente);
    e->show();
    this->close();
}
void Gerente::mostrarMensagem(const QString &mensagem, bool erro)
{
    QMessageBox msgBox;
    msgBox.setText(mensagem);
    msgBox.setWindowTitle(erro ? "Erro" : "Sucesso");
    msgBox.setIcon(erro ? QMessageBox::Critical : QMessageBox::Information);
    msgBox.exec();
}

void Gerente::limparCamposCliente()
{
    ui->lineEdit_nome_2->clear();
    ui->lineEdit_CPF->clear();
    ui->lineEdit_CEP->clear();
    ui->lineEdit_Telefone->clear();
    ui->lineEdit_E_mail_2->clear();
    idEdicaoCliente = -1;
    ui->pushButton_salvar_cliente->setText("Salvar");
}

bool Gerente::validarCamposCliente()
{
    return !ui->lineEdit_nome_2->text().isEmpty() &&
           !ui->lineEdit_CPF->text().isEmpty() &&
           !ui->lineEdit_CEP->text().isEmpty();
}


void Gerente::atualizarTabelaClientes()
{
    ui->tableWidget_clientes->setRowCount(0);
    QSqlQuery query("SELECT id, nome, cpf, cep, telefone, email FROM clientes ORDER BY nome ASC");
    int row = 0;

    while (query.next()) {
        ui->tableWidget_clientes->insertRow(row);

        QTableWidgetItem *nomeItem = new QTableWidgetItem(query.value(1).toString());
        nomeItem->setData(Qt::UserRole, query.value(0).toInt());

        ui->tableWidget_clientes->setItem(row, 0, nomeItem);
        ui->tableWidget_clientes->setItem(row, 1, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget_clientes->setItem(row, 2, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget_clientes->setItem(row, 3, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget_clientes->setItem(row, 4, new QTableWidgetItem(query.value(5).toString()));
        row++;
    }
    idEdicaoCliente = -1;
    ui->pushButton_salvar_cliente->setText("Salvar");
}

void Gerente::on_pushButton_salvar_cliente_clicked()
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

void Gerente::on_tableWidget_clientes_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    QTableWidgetItem *nomeItem = ui->tableWidget_clientes->item(row, 0);
    if (!nomeItem)
        return;

    QVariant data = nomeItem->data(Qt::UserRole);
    idEdicaoCliente = data.isValid() ? data.toInt() : -1;
}


void Gerente::on_pushButton_alterar_cliente_clicked()
{
    int linhaSelec = ui->tableWidget_clientes->currentRow();
    if (linhaSelec < 0) {
        mostrarMensagem("Selecione um cliente na tabela para alterar.", true);
        return;
    }

    QTableWidgetItem *nomeItem =
        ui->tableWidget_clientes->item(linhaSelec, 0);
    if (!nomeItem) {
        mostrarMensagem("Não foi possível obter os dados do cliente.", true);
        return;
    }

    QVariant data = nomeItem->data(Qt::UserRole);
    idEdicaoCliente = data.isValid() ? data.toInt() : -1;
    if (idEdicaoCliente == -1) {
        mostrarMensagem("Cliente inválido selecionado.", true);
        return;
    }

    ui->lineEdit_nome_2->setText(
        ui->tableWidget_clientes->item(linhaSelec, 0)->text());
    ui->lineEdit_CPF->setText(
        ui->tableWidget_clientes->item(linhaSelec, 1)->text());
    ui->lineEdit_CEP->setText(
        ui->tableWidget_clientes->item(linhaSelec, 2)->text());
    ui->lineEdit_Telefone->setText(
        ui->tableWidget_clientes->item(linhaSelec, 3)->text());
    ui->lineEdit_E_mail_2->setText(
        ui->tableWidget_clientes->item(linhaSelec, 4)->text());

    ui->pushButton_salvar_cliente->setText("Atualizar");
    mostrarMensagem("Modo de alteração ativo. Edite os campos e clique em Atualizar.", false);
}



void Gerente::on_pushButton_excluir_cliente_clicked()
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

void Gerente::limparCamposVinil()
{
    ui->lineEdit_nome_do_disco->clear();
    ui->lineEdit_Nome_artistas->clear();
    ui->lineEdit_disco_ano->clear();
    ui->comboBox_Genero_discos->setCurrentIndex(0);
    ui->comboBox_condicao_discos->setCurrentIndex(0);
    ui->lineEdit_disco_preco->clear();
    ui->lineEdit_disco_quantidade->clear();
    ui->lineEdit_disco_codigo->clear();

    idEdicaoVinil = -1;
    ui->pushButton_salvar_vinil->setText("Salvar");
}

bool Gerente::validarCamposVinil()
{
    return !ui->lineEdit_nome_do_disco->text().isEmpty() &&
           !ui->lineEdit_Nome_artistas->text().isEmpty() &&
           !ui->lineEdit_disco_preco->text().isEmpty() &&
           !ui->lineEdit_disco_quantidade->text().isEmpty() &&
           !ui->lineEdit_disco_codigo->text().isEmpty() &&
           ui->comboBox_Genero_discos->currentIndex() > 0 &&
           ui->comboBox_condicao_discos->currentIndex() > 0;


}

void Gerente::atualizarTabelaVinis()
{
    ui->tableWidget_vinis->setRowCount(0);
    QSqlQuery query("SELECT id, nome, artista, ano, genero, condicao, preco, quantidade, codigo FROM vinis ORDER BY nome ASC");
    int row = 0;

    while (query.next()) {
        ui->tableWidget_vinis->insertRow(row);

        QTableWidgetItem *nomeItem = new QTableWidgetItem(query.value(1).toString());
        nomeItem->setData(Qt::UserRole, query.value(0).toInt());

        ui->tableWidget_vinis->setItem(row, 0, nomeItem);
        ui->tableWidget_vinis->setItem(row, 1, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget_vinis->setItem(row, 2, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget_vinis->setItem(row, 3, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget_vinis->setItem(row, 4, new QTableWidgetItem(query.value(5).toString()));
        ui->tableWidget_vinis->setItem(row, 5, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget_vinis->setItem(row, 6, new QTableWidgetItem(query.value(7).toString()));
        ui->tableWidget_vinis->setItem(row, 7, new QTableWidgetItem(query.value(8).toString()));
        row++;
    }
    idEdicaoVinil = -1;
    ui->pushButton_salvar_vinil->setText("Salvar");
}
void Gerente::on_pushButton_salvar_vinil_clicked()
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

void Gerente::on_tableWidget_vinis_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    QTableWidgetItem *nomeItem = ui->tableWidget_vinis->item(row, 0);
    if (!nomeItem)
        return;

    QVariant data = nomeItem->data(Qt::UserRole);
    idEdicaoVinil = data.isValid() ? data.toInt() : -1;
}

void Gerente::on_pushButton_alterar_vinil_clicked()
{
    int linhaSelec = ui->tableWidget_vinis->currentRow();
    if (linhaSelec < 0) {
        mostrarMensagem("Selecione um vinil na tabela para alterar.", true);
        return;
    }

    QTableWidgetItem *nomeItem =
        ui->tableWidget_vinis->item(linhaSelec, 0);
    if (!nomeItem) {
        mostrarMensagem("Não foi possível obter os dados do vinil.", true);
        return;
    }

    QVariant data = nomeItem->data(Qt::UserRole);
    idEdicaoVinil = data.isValid() ? data.toInt() : -1;
    if (idEdicaoVinil == -1) {
        mostrarMensagem("Vinil inválido selecionado.", true);
        return;
    }

    ui->lineEdit_nome_do_disco->setText(
        ui->tableWidget_vinis->item(linhaSelec, 0)->text());
    ui->lineEdit_Nome_artistas->setText(
        ui->tableWidget_vinis->item(linhaSelec, 1)->text());
    ui->lineEdit_disco_ano->setText(
        ui->tableWidget_vinis->item(linhaSelec, 2)->text());

    ui->comboBox_Genero_discos->setCurrentText(
        ui->tableWidget_vinis->item(linhaSelec, 3)->text());
    ui->comboBox_condicao_discos->setCurrentText(
        ui->tableWidget_vinis->item(linhaSelec, 4)->text());

    ui->lineEdit_disco_preco->setText(
        ui->tableWidget_vinis->item(linhaSelec, 5)->text());
    ui->lineEdit_disco_quantidade->setText(
        ui->tableWidget_vinis->item(linhaSelec, 6)->text());
    ui->lineEdit_disco_codigo->setText(
        ui->tableWidget_vinis->item(linhaSelec, 7)->text());

    ui->pushButton_salvar_vinil->setText("Atualizar");
    mostrarMensagem("Modo de alteração ativo. Edite os campos e clique em Atualizar.", false);
}


void Gerente::on_pushButton_excluir_vinil_clicked()
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

bool Gerente::validarCamposFuncionario()
{
    if (ui->lineEdit_funcionario_nome->text().isEmpty() ||
        ui->lineEdit_Funcionario_cpf->text().isEmpty() ||
        ui->lineEdit_funcionario_pis->text().isEmpty() ||
        ui->lineEdit_funcionario_email->text().isEmpty() ||
        ui->comboBox_funcionario_funcao->currentText().isEmpty() ||
        ui->lineEdit_E_mail_4->text().isEmpty() || // Salário
        ui->lineEdit_funcionario_cep->text().isEmpty() ||
        ui->lineEdit_funcionario_telefone->text().isEmpty()) {
        mostrarMensagem("Todos os campos de funcionário devem ser preenchidos.", true);
        return false;
    }
   QString email = ui->lineEdit_funcionario_email->text();
    if (!email.endsWith("@vinilcius.com")) {
        mostrarMensagem("O e-mail do funcionário deve terminar com @vinilcius.com", true);
        return false;
    }

    return true;
}
void Gerente::limparCamposFuncionario()
{
    ui->lineEdit_funcionario_nome->clear();
    ui->lineEdit_Funcionario_cpf->clear();
    ui->lineEdit_funcionario_pis->clear();
    ui->lineEdit_funcionario_email->clear();
    ui->comboBox_funcionario_funcao->setCurrentIndex(0);
    ui->lineEdit_E_mail_4->clear(); // Salário
    ui->lineEdit_funcionario_cep->clear();
    ui->lineEdit_funcionario_telefone->clear();
    ui->pushButton_funcionario_salvar->setText("Salvar");
    idEdicaoFuncionario = -1;
}
void Gerente::atualizarTabelaFuncionarios()
{
    ui->tableWidget_clientes_2->setRowCount(0);


  QSqlQuery query("SELECT id, nome, cpf, funcao, salario, pis, email, cep, telefone FROM funcionarios ORDER BY nome", db);
    int row = 0;
    while (query.next()) {
        ui->tableWidget_clientes_2->insertRow(row);

        QTableWidgetItem *nomeItem = new QTableWidgetItem(query.value("nome").toString());
        nomeItem->setData(Qt::UserRole, query.value("id").toInt());
        ui->tableWidget_clientes_2->setItem(row, 0, nomeItem);


        ui->tableWidget_clientes_2->setItem(row, 1, new QTableWidgetItem(query.value("cpf").toString()));
        ui->tableWidget_clientes_2->setItem(row, 2, new QTableWidgetItem(query.value("funcao").toString()));
        ui->tableWidget_clientes_2->setItem(row, 3, new QTableWidgetItem(query.value("salario").toString()));
        ui->tableWidget_clientes_2->setItem(row, 4, new QTableWidgetItem(query.value("pis").toString()));
        ui->tableWidget_clientes_2->setItem(row, 5, new QTableWidgetItem(query.value("email").toString()));
        ui->tableWidget_clientes_2->setItem(row, 6, new QTableWidgetItem(query.value("cep").toString()));
        ui->tableWidget_clientes_2->setItem(row, 7, new QTableWidgetItem(query.value("telefone").toString()));
        row++;
    }
}

void Gerente::on_pushButton_funcionario_salvar_clicked()
{
    if (!validarCamposFuncionario()) {
        return;
    }

    QString nome = ui->lineEdit_funcionario_nome->text();
    QString cpf = ui->lineEdit_Funcionario_cpf->text();
    QString funcao = ui->comboBox_funcionario_funcao->currentText();
    QString salario = ui->lineEdit_E_mail_4->text(); // Campo usado para salário
    QString pis = ui->lineEdit_funcionario_pis->text();
    QString email = ui->lineEdit_funcionario_email->text();
    QString cep = ui->lineEdit_funcionario_cep->text();
    QString telefone = ui->lineEdit_funcionario_telefone->text();

    QSqlQuery query(db);
    bool sucesso = false;

    if (idEdicaoFuncionario == -1) {
        query.prepare("INSERT INTO funcionarios (nome, cpf, pis, email, funcao, salario, cep, telefone, senha) "
                      "VALUES (:nome, :cpf, :pis, :email, :funcao, :salario, :cep, :telefone, :senha)");
        query.bindValue(":nome", nome);
        query.bindValue(":cpf", cpf);
        query.bindValue(":pis", pis);
        query.bindValue(":email", email);
        query.bindValue(":funcao", funcao);
        query.bindValue(":salario", salario);
        query.bindValue(":cep", cep);
        query.bindValue(":telefone", telefone);
        query.bindValue(":senha", cpf);
    }
    else {
        query.prepare("UPDATE funcionarios SET nome = :nome, cpf = :cpf, pis = :pis, "
                      "email = :email, funcao = :funcao, salario = :salario, "
                      "cep = :cep, telefone = :telefone WHERE id = :id");

        query.bindValue(":nome", nome);
        query.bindValue(":cpf", cpf);
        query.bindValue(":pis", pis);
        query.bindValue(":email", email);
        query.bindValue(":funcao", funcao);
        query.bindValue(":salario", salario);
        query.bindValue(":cep", cep);
        query.bindValue(":telefone", telefone);
        query.bindValue(":id", idEdicaoFuncionario);
    }

    if (query.exec()) {
        sucesso = true;
        mostrarMensagem(idEdicaoFuncionario == -1 ? "Funcionário cadastrado com sucesso!" : "Funcionário alterado com sucesso!");
    } else {
        qDebug() << "Erro ao salvar funcionário:" << query.lastError().text();
        if (query.lastError().text().contains("UNIQUE constraint failed")) {
            mostrarMensagem("Erro: Já existe um funcionário com este CPF, PIS ou E-mail cadastrado.", true);
        } else {
            mostrarMensagem("Erro ao salvar o funcionário: " + query.lastError().text(), true);
        }
    }

    if (sucesso) {
        limparCamposFuncionario();
        atualizarTabelaFuncionarios();
    }
}



void Gerente::on_tableWidget_clientes_2_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    QTableWidgetItem *nomeItem = ui->tableWidget_clientes_2->item(row, 0);
    if (!nomeItem)
        return;

    idEdicaoFuncionario = nomeItem->data(Qt::UserRole).toInt();
}

void Gerente::on_pushButton_funcionario_alterar_clicked()
{
    int linhaSelec = ui->tableWidget_clientes_2->currentRow();
    if (linhaSelec < 0) {
        mostrarMensagem("Selecione um funcionário na tabela para alterar.", true);
        return;
    }

    QTableWidgetItem *nomeItem =
        ui->tableWidget_clientes_2->item(linhaSelec, 0);
    if (!nomeItem) {
        mostrarMensagem("Não foi possível obter os dados do funcionário.", true);
        return;
    }

    idEdicaoFuncionario = nomeItem->data(Qt::UserRole).toInt();
    if (idEdicaoFuncionario == -1) {
        mostrarMensagem("Funcionário inválido selecionado.", true);
        return;
    }

    ui->lineEdit_funcionario_nome->setText(
        ui->tableWidget_clientes_2->item(linhaSelec, 0)->text());
    ui->lineEdit_Funcionario_cpf->setText(
        ui->tableWidget_clientes_2->item(linhaSelec, 1)->text());

    QString funcaoSelecionada =
        ui->tableWidget_clientes_2->item(linhaSelec, 2)->text();
    int index = ui->comboBox_funcionario_funcao->findText(funcaoSelecionada);
    if (index != -1)
        ui->comboBox_funcionario_funcao->setCurrentIndex(index);

    ui->lineEdit_E_mail_4->setText(
        ui->tableWidget_clientes_2->item(linhaSelec, 3)->text());

    ui->lineEdit_funcionario_pis->setText(
        ui->tableWidget_clientes_2->item(linhaSelec, 4)->text());
    ui->lineEdit_funcionario_email->setText(
        ui->tableWidget_clientes_2->item(linhaSelec, 5)->text());
    ui->lineEdit_funcionario_cep->setText(
        ui->tableWidget_clientes_2->item(linhaSelec, 6)->text());
    ui->lineEdit_funcionario_telefone->setText(
        ui->tableWidget_clientes_2->item(linhaSelec, 7)->text());

    ui->pushButton_funcionario_salvar->setText("Atualizar");
    mostrarMensagem("Modo de alteração ativo. Edite os campos e clique em Atualizar.", false);
}





void Gerente::on_pushButton_funcionario_excluir_clicked()
{
    int linhaSelec = ui->tableWidget_clientes_2->currentRow();

    if (linhaSelec < 0 || idEdicaoFuncionario == -1) {
        mostrarMensagem("Por favor, selecione um funcionário para excluir!", true);
        return;
    }

    QMessageBox::StandardButton resposta;
    resposta = QMessageBox::question(this, "Confirmação",
                                     "Tem certeza que deseja excluir o funcionário selecionado?",
                                     QMessageBox::Yes | QMessageBox::No);

    if (resposta == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM funcionarios WHERE id = :id");
        query.bindValue(":id", idEdicaoFuncionario);

        if (query.exec()) {
            limparCamposFuncionario();
            atualizarTabelaFuncionarios();
            mostrarMensagem("Funcionário excluído com sucesso!", false);
        } else {
            qDebug() << "Erro ao excluir funcionário:" << query.lastError().text();
            mostrarMensagem("Erro ao excluir o funcionário.", true);
        }
    }
}

void Gerente::on_pushButton_controle_loja_clicked()
{
    Controledaloja *c = new Controledaloja(this);
    c->setGerente(this);
    c->setOrigem(Controledaloja::OrigemGerente);
    c->atualizarDados();
    c->show();
}


void Gerente::on_pushButton_Debug_clicked()

{

DebugMenu *menu = new DebugMenu(this);

if (menu->exec() == QDialog::Accepted) {

this->close();

}

delete menu;

}

void Gerente::on_pushButton_voltar_clicked()
{
    Central *c = new Central("gerente");
    c->setAttribute(Qt::WA_DeleteOnClose);
    c->show();
    this->close();
}


