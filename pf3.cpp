/***************************************************************************
 *   exN.cpp                                  Version 20230620.225755      *
 *                                                                         *
 *   Programa Principal                                                    *
 *   Copyright (C) 2023         by Ruben Carlo Benante                     *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *   To contact the author, please write to:                               *
 *   Ruben Carlo Benante                                                   *
 *   Email: rcb@beco.cc                                                    *
 *   Webpage: http://www.beco.cc                                           *
 *   Phone: +55 (81) 3184-7555                                             *
 ***************************************************************************/

/*
 * Instrucoes para compilar:
 *   $ make exN.out
 * Leia mais detalhes no arquivo INSTALL
 */

/* ---------------------------------------------------------------------- */
/* includes */


#include <QApplication>

#include "firstlogin.hpp"
#include "mainwindow.hpp"
#include "Cadastro.hpp"
#include "Estoque.hpp"
#include "principalui.hpp"
#include "Gerente.hpp"
#include "Controledaloja.hpp"
#include "central.hpp"
#include"registro.hpp"
/* ---------------------------------------------------------------------- */
/* definitions */

/* limits */
#define SBUFF 256 /* string buffer */

/* ---------------------------------------------------------------------- */
/* This function does ...
 * It receives as input ...
 * It returns ...
 */
static bool donoJaCadastrado()
{
    QSqlDatabase db;

    if (QSqlDatabase::contains("pf3_connection_main")) {
        db = QSqlDatabase::database("pf3_connection_main");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "pf3_connection_main");
        db.setDatabaseName("database/pf3.db");
    }

    if (!db.open()) {
        qDebug() << "Erro ao abrir banco no main:" << db.lastError().text();
        return false; // Se der erro, abre tela de cadastro
    }

    QSqlQuery q(db);

    // Garante que a tabela exista
    q.exec("CREATE TABLE IF NOT EXISTS dono_da_loja ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "name TEXT,"
           "email TEXT UNIQUE,"
           "password TEXT)"
           );

    // Conta usuários
    if (!q.exec("SELECT COUNT(*) FROM dono_da_loja")) {
        qDebug() << "Erro ao contar donos:" << q.lastError().text();
        return false;
    }

    int total = 0;
    if (q.next())
        total = q.value(0).toInt();

    return (total > 0);
}
int main(int argc, char **argv)
{
    /* local declarations */

    /* Q_INIT_RESOURCE(x); */

  QApplication app(argc, argv);

    if (donoJaCadastrado()) {
      firstlogin fl;
      fl.show();
        return app.exec();
    } else {
        MainWindow win;
        win.show();


        return app.exec();
    }
}







/* ---------------------------------------------------------------------- */
/* vi: set ai et ts=4 sw=4 tw=0 wm=0 fo=croql : CPP config - Vim modeline */
/* Template by Dr. Beco <rcb at beco dot cc> Version 20220718.172615      */

