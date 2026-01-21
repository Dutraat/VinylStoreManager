#include "Estoque.hpp"
#include "ui_Estoque.h"
#include <QMessageBox>
#include "Cadastro.hpp"
#include "Gerente.hpp"
#include <QScrollBar>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "debug.hpp"
#include"central.hpp"
#include <QDateTime>
#include <QSqlRecord>

Estoque::Estoque(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::EstoqueUI)
{
    ui->setupUi(this);

    ui->scrollAreaWidgetContents_vendas->setMinimumHeight(600);
    ui->frame_mensagem->setVisible(false);
    ui->tableWidget_discos->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_vendas->horizontalHeader()->setStretchLastSection(true);
    atualizarEstatisticasVendas();

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("database/pf3.db");
        db.open();
    }

    carregarDadosVendas();

    ui->tableWidget_discos->horizontalHeader()->setStretchLastSection(true);

    atualizarTabela();
}

Estoque::~Estoque()
{
    delete ui;
}

void Estoque::on_pushButton_editar_clicked()
{
    int linhaSelec = ui->tableWidget_discos->currentRow();

    if (linhaSelec < 0) {
        mostrarMensagem("Por favor, selecione um disco para editar!", true);
        return;
    }

    mostrarMensagem("Função de edição será implementada em breve!", false);
}

void Estoque::on_pushButton_excluir_clicked()
{
    int linhaSelec = ui->tableWidget_discos->currentRow();

    if (linhaSelec < 0) {
        mostrarMensagem("Por favor, selecione um disco para excluir!", true);
        return;
    }

    QMessageBox::StandardButton resposta = QMessageBox::question(
        this,
        "Confirmar Exclusão",
        "Tem certeza que deseja excluir este disco?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (resposta == QMessageBox::Yes) {
        discos.removeAt(linhaSelec);
        atualizarTabela();
        mostrarMensagem("Disco excluído com sucesso!", false);
    }
}

void Estoque::on_pushButton_fechar_msg_clicked()
{
    ui->frame_mensagem->setVisible(false);
}

void Estoque::mostrarMensagem(const QString &mensagem, bool erro)
{
    ui->label_mensagem->setText(mensagem);

   if (erro) {
        ui->label_mensagem->setStyleSheet(
            "background-color: rgb(255, 100, 100); "
            "color: white; "
            "padding: 8px; "
            "border-radius: 5px; "
            "font-weight: bold;"
        );
    } else {
        ui->label_mensagem->setStyleSheet(
            "background-color: rgb(100, 200, 100); "
            "color: white; "
            "padding: 8px; "
            "border-radius: 5px; "
            "font-weight: bold;"
        );
    }

    ui->frame_mensagem->setVisible(true);
}

void Estoque::atualizarTabela()
{
    ui->tableWidget_discos->setRowCount(0);

    QSqlQuery query("SELECT nome, artista, ano, genero, condicao, preco, quantidade, codigo FROM vinis ORDER BY nome ASC");

    int row = 0;
    while (query.next()) {
        ui->tableWidget_discos->insertRow(row);

        ui->tableWidget_discos->setItem(row, 0, new QTableWidgetItem(query.value("nome").toString()));
        ui->tableWidget_discos->setItem(row, 1, new QTableWidgetItem(query.value("artista").toString()));
        ui->tableWidget_discos->setItem(row, 2, new QTableWidgetItem(query.value("ano").toString()));
        ui->tableWidget_discos->setItem(row, 3, new QTableWidgetItem(query.value("genero").toString()));
        ui->tableWidget_discos->setItem(row, 4, new QTableWidgetItem(query.value("quantidade").toString()));
        double preco = query.value("preco").toDouble();
        ui->tableWidget_discos->setItem(row, 5, new QTableWidgetItem("R$ " + QString::number(preco, 'f', 2)));
        ui->tableWidget_discos->setItem(row, 6, new QTableWidgetItem(query.value("condicao").toString()));
        ui->tableWidget_discos->setItem(row, 7, new QTableWidgetItem(query.value("codigo").toString()));

        row++;
    }
}
void Estoque::atualizarTabelaVendas()
{
    ui->tableWidget_vendas->setRowCount(0);

    for (int i = 0; i < vendas.size(); i++) {
        ui->tableWidget_vendas->insertRow(i);

        ui->tableWidget_vendas->setItem(i, 0, new QTableWidgetItem(vendas[i].data));
        ui->tableWidget_vendas->setItem(i, 1, new QTableWidgetItem(vendas[i].disco));
        ui->tableWidget_vendas->setItem(i, 2, new QTableWidgetItem(vendas[i].quantidade));
        ui->tableWidget_vendas->setItem(i, 3, new QTableWidgetItem("R$ " + vendas[i].valor));
        ui->tableWidget_vendas->setItem(i, 4, new QTableWidgetItem(vendas[i].cliente));
    }
}

void Estoque::atualizarEstatisticasVendas()
{
    int totalVendas = vendas.size();
    double lucroTotal = 0.0;
    int discosVendidos = 0;

    for (const Venda &venda : vendas) {
        lucroTotal += venda.valor.toDouble();
        discosVendidos += venda.quantidade.toInt();
    }

    ui->label_total_vendas_valor->setText(QString::number(totalVendas));
    ui->label_lucro_valor->setText(QString("R$ %1").arg(lucroTotal, 0, 'f', 2));
    ui->label_discos_vendidos_valor->setText(QString::number(discosVendidos));
}

void Estoque::setOrigem(int tipo)
{
    origem = tipo;
}

void Estoque::on_pushButton_voltar_clicked()
{
    if (origem == OrigemGerente) {
        Gerente *g = new Gerente();
        g->setAttribute(Qt::WA_DeleteOnClose);
        g->show();
    }
    else if (origem == OrigemCadastro) {
        Cadastro *cad = new Cadastro();
        cad->setAttribute(Qt::WA_DeleteOnClose);
        cad->show();
    }
    else if (origem == OrigemCentralFunc) {
        Central *c = new Central("funcionario");
        c->setAttribute(Qt::WA_DeleteOnClose);
        c->show();
    }
    this->close();
}

void Estoque::on_pushButton_Debug_clicked()

{

DebugMenu *menu = new DebugMenu(this);

if (menu->exec() == QDialog::Accepted) {

this->close();

}

delete menu;


}
void Estoque::carregarDadosVendas()
{
    vendas.clear();

    QSqlDatabase db;
    if (QSqlDatabase::contains("pf3_connection")) {
        db = QSqlDatabase::database("pf3_connection");
    } else {
        db = QSqlDatabase::database();
    }

    if (!db.isOpen()) {
        qDebug() << "Banco fechado, tentando abrir...";
        if(!db.open()){
             QMessageBox::critical(this, "Erro Fatal", "Não foi possível conectar ao banco para ler as vendas.");
             return;
        }
    }

    QSqlQuery query(db);

    query.prepare("SELECT * FROM vendas ORDER BY id DESC");

    if (!query.exec()) {
        QString erro = query.lastError().text();

        if (erro.contains("no such column")) {
            QMessageBox::warning(this, "Aviso de Estrutura",
                "Seu banco de dados é antigo e não tem as colunas 'produto' ou 'cliente'.\n"
                "Apenas o valor e a data serão mostrados.\n\n"
                "Para corrigir definitivamente: Apague o arquivo pf3.db e cadastre tudo de novo.");

            query.prepare("SELECT data_hora, valor_total FROM vendas ORDER BY id DESC");
            query.exec();
        } else {
            QMessageBox::critical(this, "Erro SQL", "Erro ao buscar vendas: " + erro);
            return;
        }
    }

    int idxData = query.record().indexOf("data_hora");
    int idxProd = query.record().indexOf("produto");
    int idxQtd  = query.record().indexOf("quantidade");
    int idxVal  = query.record().indexOf("valor_total");
    int idxCli  = query.record().indexOf("cliente");

    while (query.next()) {
        Venda v;
        v.data = (idxData != -1) ? query.value(idxData).toDateTime().toString("dd/MM/yyyy HH:mm") : "---";
        v.disco = (idxProd != -1) ? query.value(idxProd).toString() : "Venda Realizada";
        if(v.disco.isEmpty()) v.disco = "Venda Geral";
        v.quantidade = (idxQtd != -1) ? query.value(idxQtd).toString() : "1";
        v.valor = (idxVal != -1) ? QString::number(query.value(idxVal).toDouble(), 'f', 2) : "0.00";
        v.cliente = (idxCli != -1) ? query.value(idxCli).toString() : "Cliente/Balcão";
        if(v.cliente.isEmpty()) v.cliente = "Balcão";

        vendas.append(v);
    }

    atualizarTabelaVendas();
    atualizarEstatisticasVendas();
}
