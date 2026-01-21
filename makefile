#***************************************************************************
#*   Autoversion makefile                   v.20250612.152755 (qmake)      *
#*   Copyright (C) 2014-2025 by Ruben Carlo Benante <rcb@beco.cc>          *
#*                                                                         *
#*   This makefile sets BUILD and allows to set MAJOR.MINOR version,       *
#*   DEBUG and OBJ to compile a range of different targets                 *
#***************************************************************************
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License as published by  *
#*   the Free Software Foundation; version 2 of the License.               *
#*                                                                         *
#*   This program is distributed in the hope that it will be useful,       *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU General Public License for more details.                          *
#*                                                                         *
#*   You should have received a copy of the GNU General Public License     *
#*   along with this program; if not, write to the                         *
#*   Free Software Foundation, Inc.,                                       *
#*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************
#*   To contact the author, please write to:                               *
#*   Ruben Carlo Benante                                                   *
#*   Email: rcb@beco.cc                                                    *
#*   Webpage: http://beco.cc/                                              *
#***************************************************************************
#
# Usage:
#    make clean : remove arquivos automaticos
#    make exN.out :
#       4. compila e cria exN.out, e para isso
#       3. antes cria Makefile, e para isso
#       2. antes edita o pf3-james-kirk.pro, trocando TARGET e QT
#       1. antes cria pf3-james-kirk.pro, fazendo uma varredura na pasta

# Regra 4: para criar exN.out, dependendo de Makefile
pf3.out: Makefile
	@echo "4. gerar binario pf3.out a partir do Makefile"
	make -f Makefile

# Regra 3: para criar Makefile, dependendo de pf3-james-kirk.pro
Makefile: pf3-james-kirk.pro
	@echo "3. gerar Makefile a partir do arquivo de projeto .pro"
	qmake pf3-james-kirk.pro

# Regra 2 e 1: para editar/criar pf3-james-kirk.pro, nao depende de nada
pf3-james-kirk.pro:
	@echo "1. gerar arquivo de projeto .pro a partir do conteudo do diretorio"
	qmake -project
	@echo "2. ajustar variaveis TARGET e QT do arquivo de projeto .pro"
	echo "QT += gui widgets sql" >> pf3-james-kirk.pro
	sed -i 's/TARGET = .*/TARGET = pf3.out/' pf3-james-kirk.pro

# Regra para limpar a pasta de trabalho (working dir)
clean:
	rm Makefile *.o *.out pf3-james-kirk.pro ui_*.h moc_help1.cpp moc_mainwin.cpp moc_predefs.h qrc_vinil.cpp moc_Cadastro.cpp pf3.db  moc*

#* ------------------------------------------------------------------- *
#* makefile config for Vim modeline                                    *
#* vi: set ai noet ts=4 sw=4 tw=0 wm=0 fo=croqlt list :                *
#* Template by Dr. Beco <rcb at beco dot cc> Version 20220521.001405   *
