#include "principalui.hpp"
#include "debug.hpp"
#include "ui_principalui.h"
#include <QTime>
#include <QString>
#include <QRandomGenerator>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include "help1.hpp"
#include "central.hpp"
#include"firstlogin.hpp"

principalui::principalui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::principalui)
{
    ui->setupUi(this);
    ui->tableWidget_Carrinho->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_Novos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("database/pf3.db");
        if (!db.open()) {
            QMessageBox::critical(this, "Erro", "Não foi possível conectar ao banco de dados.");
        }
    }
QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS vendas ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "data_hora DATETIME, "
               "valor_total REAL, "
               "numero_pedido TEXT, "
               "produto TEXT, "
               "quantidade INTEGER, "
               "cliente TEXT)");
    connect(this, &principalui::vendaRealizada, this, &principalui::atualizarDashboardGerente);

    // Atualiza os valores assim que abre o programa
    atualizarDashboardGerente();

    popularTabelaDiscosUsados();
     popularTabelaDiscosNovos();
}

principalui::~principalui()
{
    delete ui;
}
//Arualiza o carrinho
void principalui::atualizarCarrinhoVisual() {
    ui->tableWidget_Carrinho->setRowCount(0);
    for (int i = 0; i < carrinho.size(); ++i) {
        const ProdutoCarrinho &p = carrinho[i];
        ui->tableWidget_Carrinho->insertRow(i);
        ui->tableWidget_Carrinho->setItem(i, 0, new QTableWidgetItem(p.nome));
        ui->tableWidget_Carrinho->setItem(i, 1, new QTableWidgetItem(QString("R$ %1").arg(p.preco, 0, 'f', 2)));
        ui->tableWidget_Carrinho->setItem(i, 2, new QTableWidgetItem(QString::number(p.quantidade)));
        double totalLinha = p.preco * p.quantidade;
        ui->tableWidget_Carrinho->setItem(i, 3, new QTableWidgetItem(QString("R$ %1").arg(totalLinha, 0, 'f', 2)));
    }
}


//Adicionar produtos ao carrinho
void principalui::adicionarProduto(QString nome, double preco, int quantidadeSolicitada) {
    QSqlQuery query;
    query.prepare("SELECT quantidade FROM vinis WHERE nome = :nome");
    query.bindValue(":nome", nome);
    int estoqueDisponivel = 0;
    if (query.exec() && query.next()) {
        estoqueDisponivel = query.value("quantidade").toInt();
    }

    int index = -1;
    for (int i = 0; i < carrinho.size(); ++i) {
        if (carrinho[i].nome == nome && qFuzzyCompare(carrinho[i].preco, preco)) {
            index = i;
            break;
        }
    }

    int qtdeNoCarrinho = (index != -1) ? carrinho[index].quantidade : 0;
        if (qtdeNoCarrinho + quantidadeSolicitada > estoqueDisponivel) {
        QMessageBox::warning(this, "Estoque Insuficiente", "Temos apenas " + QString::number(estoqueDisponivel) + " unidades em estoque.");
        return;
    }

    if (index != -1) {
        carrinho[index].quantidade += quantidadeSolicitada;
    } else {
        carrinho.append({nome, preco, quantidadeSolicitada});
    }

    atualizarCarrinhoVisual();
    atualizarTotal();
}

//Remover produtos do carrinho
void principalui::removerProduto(int linhaSelecionada) {
    if (linhaSelecionada >= 0 && linhaSelecionada < carrinho.size()) {
        if (carrinho[linhaSelecionada].quantidade > 1) {
            carrinho[linhaSelecionada].quantidade--;
        } else {
            carrinho.removeAt(linhaSelecionada);
        }
        atualizarCarrinhoVisual();
        atualizarTotal();
    }
}


// Atualizar o total do carrinho
void principalui::atualizarTotal() {
    double total = 0.0;
    for (const auto &p : carrinho) {
        total += p.preco * p.quantidade;
    }
    if (cupomAplicado) {
        total *= (1.0 - desconto);
    }
    ui->label_TotalpriceCart->setText(QString("R$ %1").arg(total, 0, 'f', 2));
}


//Discos Novos
void principalui::popularTabelaDiscosNovos() {
    ui->tableWidget_Novos->setRowCount(0);
    ui->tableWidget_Novos->setColumnCount(9);
    QStringList cabecalhos = {"Nome", "Artista", "Ano", "Gênero", "Qtd.", "Preço", "Condição", "Código", "Comprar"};
    ui->tableWidget_Novos->setHorizontalHeaderLabels(cabecalhos);
    ui->tableWidget_Novos->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_Novos->setColumnWidth(0, 240); // Nome
    ui->tableWidget_Novos->setColumnWidth(1, 180); // Artista

    QSqlQuery query;
    query.prepare("SELECT nome, artista, ano, genero, quantidade, preco, condicao, codigo FROM vinis WHERE condicao = 'Novo' AND quantidade > 0");
    query.exec();

    int linha = 0;
    while (query.next()) {
        ui->tableWidget_Novos->insertRow(linha);

        ui->tableWidget_Novos->setItem(linha, 0, new QTableWidgetItem(query.value("nome").toString()));
        ui->tableWidget_Novos->setItem(linha, 1, new QTableWidgetItem(query.value("artista").toString()));
        ui->tableWidget_Novos->setItem(linha, 2, new QTableWidgetItem(query.value("ano").toString()));
        ui->tableWidget_Novos->setItem(linha, 3, new QTableWidgetItem(query.value("genero").toString()));
        ui->tableWidget_Novos->setItem(linha, 4, new QTableWidgetItem(query.value("quantidade").toString()));
        double precoVal = query.value("preco").toDouble();
        ui->tableWidget_Novos->setItem(linha, 5, new QTableWidgetItem("R$ " + QString::number(precoVal, 'f', 2)));
        ui->tableWidget_Novos->setItem(linha, 6, new QTableWidgetItem(query.value("condicao").toString()));
        ui->tableWidget_Novos->setItem(linha, 7, new QTableWidgetItem(query.value("codigo").toString()));

        // Botão Adicionar
        QPushButton *btnAddCarrinho = new QPushButton("Adicionar");
        btnAddCarrinho->setStyleSheet("background-color: #008CBA; color: white; font-weight: bold;");
        ui->tableWidget_Novos->setCellWidget(linha, 8, btnAddCarrinho);

        connect(btnAddCarrinho, &QPushButton::clicked, [=]() {
            int estoqueDisponivel = ui->tableWidget_Novos->item(linha, 4)->text().toInt();
            bool ok;
            int quantidade = QInputDialog::getInt(
                this,
                "Selecionar Quantidade",
                "Quantas unidades deseja adicionar?",
                1,
                1,
                estoqueDisponivel,
                1,
                &ok
            );
            if (ok) {
                QString nome = ui->tableWidget_Novos->item(linha, 0)->text();
                QString precoTexto = ui->tableWidget_Novos->item(linha, 5)->text();
                double preco = precoTexto.replace("R$ ", "").toDouble();
                adicionarProduto(nome, preco, quantidade);
                QMessageBox::information(this, "Item adicionado", "O item foi adicionado ao carrinho!");
            }
        });

        linha++;
    }
}



//Discos usados

void principalui::popularTabelaDiscosUsados() {
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(9);
    QStringList cabecalhos = {"Nome", "Artista", "Ano", "Gênero", "Qtd.", "Preço", "Condição", "Código", "Comprar"};
    ui->tableWidget_2->setHorizontalHeaderLabels(cabecalhos);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->setColumnWidth(0, 230);
    ui->tableWidget_2->setColumnWidth(1, 180);

    QSqlQuery query;
    query.prepare("SELECT nome, artista, ano, genero, quantidade, preco, condicao, codigo FROM vinis WHERE condicao != 'Novo' AND quantidade > 0");
    query.exec();

    int linha = 0;
    while (query.next()) {
        ui->tableWidget_2->insertRow(linha);

        ui->tableWidget_2->setItem(linha, 0, new QTableWidgetItem(query.value("nome").toString()));
        ui->tableWidget_2->setItem(linha, 1, new QTableWidgetItem(query.value("artista").toString()));
        ui->tableWidget_2->setItem(linha, 2, new QTableWidgetItem(query.value("ano").toString()));
        ui->tableWidget_2->setItem(linha, 3, new QTableWidgetItem(query.value("genero").toString()));
        ui->tableWidget_2->setItem(linha, 4, new QTableWidgetItem(query.value("quantidade").toString()));
        double precoVal = query.value("preco").toDouble();
        ui->tableWidget_2->setItem(linha, 5, new QTableWidgetItem("R$ " + QString::number(precoVal, 'f', 2)));
        ui->tableWidget_2->setItem(linha, 6, new QTableWidgetItem(query.value("condicao").toString()));
        ui->tableWidget_2->setItem(linha, 7, new QTableWidgetItem(query.value("codigo").toString()));

        // Botão Adicionar
        QPushButton *btnAddCarrinho = new QPushButton("Adicionar");
        btnAddCarrinho->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold;");
        ui->tableWidget_2->setCellWidget(linha, 8, btnAddCarrinho);

        connect(btnAddCarrinho, &QPushButton::clicked, [=]() {
            int estoqueDisponivel = ui->tableWidget_2->item(linha, 4)->text().toInt();
            bool ok;
            int quantidade = QInputDialog::getInt(
                this,
                "Selecionar Quantidade",
                "Quantas unidades deseja adicionar?",
                1,
                1,
                estoqueDisponivel,
                1,
                &ok
            );
            if (ok) {
                QString nome = ui->tableWidget_2->item(linha, 0)->text();
                QString precoTexto = ui->tableWidget_2->item(linha, 5)->text();
                double preco = precoTexto.replace("R$ ", "").toDouble();
                adicionarProduto(nome, preco, quantidade);
                QMessageBox::information(this, "Item adicionado", "O item foi adicionado ao carrinho!");
            }
        });

        linha++;
    }
}


//PushButtons

void principalui::on_pushButton_Backtocatalog_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}



void principalui::on_pushButton_Pay_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->Pagamento);
     ui->label_TotalPrice->setText(ui->label_TotalpriceCart->text());
}


void principalui::on_pushButton_Backtocart_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Carrinho_2);
}

void principalui::on_pushButton_RemoverSelecionado_clicked()
{
    int row = ui->tableWidget_Carrinho->currentRow();
    if (row >= 0) {
        removerProduto(row);
    }
}

void principalui::setTipoUsuario(QString tipo)
{
    this->tipoUsuario = tipo;
}


void principalui::on_pushButton_Discos_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}


void principalui::on_pushButton_shopcart_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->Carrinho_2);
}
void principalui::on_pushButton_confirm_clicked()
{
    if (carrinho.isEmpty()) {
        QMessageBox::warning(this, "Carrinho vazio", "Adicione produtos antes de finalizar!");
        return;
    }

    // Calcula o total real da venda
    double totalVenda = 0.0;
    for (const auto &p : carrinho) {
        totalVenda += p.preco * p.quantidade;
    }
    if (cupomAplicado) {
        totalVenda *= (1.0 - desconto);
    }

    quint32 randomNumber = QRandomGenerator::global()->bounded(10000000, 100000000);
    QString numeroPedido = QString("#%1").arg(randomNumber);

    // INÍCIO DA TRANSAÇÃO (Segurança)
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    bool estoqueOk = BaixaEstoque();
    bool vendaOk = RegistrarVenda(numeroPedido, totalVenda);

    if (estoqueOk && vendaOk) {
        db.commit(); // Salva tudo

        // Atualiza UI
        popularTabelaDiscosNovos();
        popularTabelaDiscosUsados();

        carrinho.clear();
        atualizarCarrinhoVisual();
        atualizarTotal();

        // Emite sinal para atualizar o gerente automaticamente
        emit vendaRealizada();

        ui->label_NumeroPedido->setText(numeroPedido);
        ui->stackedWidget->setCurrentWidget(ui->page_confirmacao);
    } else {
        db.rollback(); // Cancela tudo se der erro
        QMessageBox::critical(this, "Erro", "Erro ao processar a compra. Nada foi cobrado.");
    }
}

void principalui::on_pushButton_ContinuarCompra_clicked()
{
    cupomAplicado = false;
    desconto = 0.0;
    ui->text_Cupom->clear();
    ui->stackedWidget->setCurrentWidget(ui->page);
    ui->textEdit->clear();
    ui->textEdit_2->clear();
}

void principalui::on_pushButton_cupom_clicked()
{
    QString codigo = ui->text_Cupom->toPlainText().trimmed();

    if (cupomAplicado) {
        QMessageBox::information(this, "Cupom já aplicado", "O cupom já foi aplicado!");
        return;
    }

    if (codigo.compare("AJUDADO PELO VINILCIUS", Qt::CaseInsensitive) == 0) {
        cupomAplicado = true;
        desconto = 0.10;
        QMessageBox::information(this, "Cupom aplicado", "Cupom aplicado com sucesso! 10% de desconto no total.");
        atualizarTotal();
        ui->text_Cupom->clear();
    } else {
        QMessageBox::warning(this, "Cupom inválido", "O código de cupom está incorreto.");
    }
}
void principalui::on_pushButton_Debug_clicked()
{
    DebugMenu *menu = new DebugMenu(this);
    if (menu->exec() == QDialog::Accepted) {
    this->close();
}

delete menu;
}

//Baixa Estoque
bool principalui::BaixaEstoque() {
    QSqlQuery query;
    bool tudoCerto = true;

    for (const auto &p : carrinho) {
        query.prepare("UPDATE vinis SET quantidade = quantidade - :qtd WHERE nome = :nome");
        query.bindValue(":qtd", p.quantidade);
        query.bindValue(":nome", p.nome);

        if (!query.exec()) {
            qDebug() << "Erro ao dar baixa: " << query.lastError().text();
            tudoCerto = false;
        }
    }
    return tudoCerto;
}
// Em principalui.cpp

bool principalui::RegistrarVenda(QString numeroPedido, double valorTotal)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString resumoProdutos = "";
    int qtdTotal = 0;
    for (const ProdutoCarrinho &item : carrinho) {
        resumoProdutos += item.nome + " (x" + QString::number(item.quantidade) + "), ";
        qtdTotal += item.quantidade;
    }
    if (resumoProdutos.endsWith(", ")) {
        resumoProdutos.chop(2);
    }

    QString cliente = "Cliente Balcão";

    QSqlQuery query;
    query.prepare("INSERT INTO vendas (data_hora, valor_total, numero_pedido, produto, quantidade, cliente) "
                  "VALUES (:data, :valor, :pedido, :prod, :qtd, :cli)");

    query.bindValue(":data", QDateTime::currentDateTime());
    query.bindValue(":valor", valorTotal);
    query.bindValue(":pedido", numeroPedido);
    query.bindValue(":prod", resumoProdutos);
    query.bindValue(":qtd", qtdTotal);
    query.bindValue(":cli", cliente);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erro", "Erro ao salvar venda no banco: " + query.lastError().text());
        return false;
    }
    return true;
}
void principalui::atualizarDashboardGerente() {
    double dia = ObterTotalVendas(PeriodoVenda::Dia);
    double semana = ObterTotalVendas(PeriodoVenda::Semana);
    double mes = ObterTotalVendas(PeriodoVenda::Mes);

    /* Atualiza a interface (Certifique-se que criou estas labels no Qt Designer!)
    if (ui->label_VendasDia) ui->label_VendasDia->setText(QString("Dia: R$ %1").arg(dia, 0, 'f', 2));
    if (ui->label_VendasSemana) ui->label_VendasSemana->setText(QString("Semana: R$ %1").arg(semana, 0, 'f', 2));
    if (ui->label_VendasMes) ui->label_VendasMes->setText(QString("Mês: R$ %1").arg(mes, 0, 'f', 2));*/
}

void principalui::on_toolButton_clicked()
{
    Help1 *helpWindow = new Help1(this);
    helpWindow->setAttribute(Qt::WA_DeleteOnClose);
    helpWindow->show();
}
void principalui::on_pushButton_voltar_clicked()
{
    if (tipoUsuario == "gerente") {
        Central *c = new Central("gerente");
        c->setAttribute(Qt::WA_DeleteOnClose);
        c->show();
    }
    else if (tipoUsuario == "funcionario") {
        Central *c = new Central("funcionario");
        c->setAttribute(Qt::WA_DeleteOnClose);
        c->show();
    }
    else {
        firstlogin *f = new firstlogin();
        f->setAttribute(Qt::WA_DeleteOnClose);
        f->show();
    }

    this->close();
}


double principalui::ObterTotalVendas(PeriodoVenda periodo)
{
    QSqlQuery query;
    QString sql;

    switch (periodo) {
    case PeriodoVenda::Dia:
        sql = "SELECT SUM(valor_total) FROM vendas WHERE date(data_hora) = date('now', 'localtime')";
        break;
    case PeriodoVenda::Semana:
        sql = "SELECT SUM(valor_total) FROM vendas WHERE date(data_hora) >= date('now', 'localtime', '-7 days')";
        break;
    case PeriodoVenda::Mes:
        sql = "SELECT SUM(valor_total) FROM vendas WHERE strftime('%Y-%m', data_hora) = strftime('%Y-%m', 'now', 'localtime')";
        break;
    }

    query.prepare(sql);
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}
