/***************************************************************************
 * Copyright (c) 2007-2020                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase2 project, a images/documents           *
 * database program.                                                       *
 *                                                                         *
 * Redistribution and use in source and binary forms, with or without      *
 * modification, are permitted provided that the following conditions      *
 * are met:                                                                *
 * 1. Redistributions of source code must retain the above copyright       *
 *    notice, this list of conditions and the following disclaimer.        *
 * 2. Redistributions in binary form must reproduce the above copyright    *
 *    notice, this list of conditions and the following disclaimer in the  *
 *    documentation and/or other materials provided with the distribution. *
 * 4. Neither the name of the Author     nor the names of its contributors *
 *    may be used to endorse or promote products derived from this software*
 *    without specific prior written permission.                           *
 *                                                                         *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS  *
 * BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR   *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS*
 * INTERRUPTION)                                                           *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,     *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING   *
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
 * POSSIBILITY OFSUCH DAMAGE.                                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "sysactivity.h"

SysActivity::SysActivity(QWidget *parent) :
    QWidget(parent)
{
}

bool SysActivity::logger(ModulesOfApp modules, Operations operations, qint64 docid)
{
    QInputDialog *iDialog = new QInputDialog(this, Qt::Dialog);
    iDialog->setWindowModality(Qt::ApplicationModal);
    iDialog->resize(QSize(300,300));
    connect(iDialog, SIGNAL(textValueSelected(QString)), this,
           SLOT(inputDialog_operatorSelected(QString)) );
    iDialog->setWindowTitle(tr("Seguranca do Sistema"));
    iDialog->setCancelButtonText(tr("Cancelar"));
    iDialog->setOkButtonText(tr("Confirmar"));
    iDialog->setInputMode(QInputDialog::TextInput);
    iDialog->setLabelText(tr("Esta operação requer que "
                             "você informe o nome do Operador:"));
    iDialog->setTextEchoMode(QLineEdit::Normal);
    int ok = iDialog->exec();

    if( ok == QDialog::Accepted) {
        if(!uOperatorIsValid(operatorsName.simplified()) ) { // somente teste nome do usuario
            QMessageBox::warning(this, PROGRAM_NAME,
                                  tr("Nome do Operador inválido."
                                     "Tente novamente."), QMessageBox::Close );
            operatorsName="";
            disconnect(iDialog, SIGNAL(textValueSelected(QString)), nullptr, nullptr);
            return false;
        }
        save_P(operatorsName, modules, operations, docid);
        disconnect(iDialog, SIGNAL(textValueSelected(QString)), nullptr, nullptr);
        return true;
    }
    disconnect(iDialog, SIGNAL(textValueSelected(QString)), nullptr, nullptr);
    return false;
}

void SysActivity::inputDialog_operatorSelected(const QString &text)
{
    operatorsName = text;
}


void SysActivity::save_P(const QString user, ModulesOfApp modules, Operations operations, qint64 docid)
{
    QString sql = "INSERT INTO DOCSYSACTIVITY "
            "(ACT_HOSTNAME, ACT_USER, ACT_MODULE, ACT_OPERATION, ACT_DOCID) "
            "VALUES (:ACT_HOSTNAME, :ACT_USER, :ACT_MODULE, :ACT_OPERATION, :ACT_DOCID)";

    /*
     * Necessario para converter explicitamente a class enum ModulesOfApp
     * para seu tipo inteiro correspondente
     * no padrao C++11 isso nao ocorre mais automaticamente
     */
    ModulesOfApp m = modules;
    int i = static_cast<int>(m);

    Operations o = operations;
    int oper = static_cast<int>(o);


    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":ACT_HOSTNAME", QHostInfo::localHostName());
    query.bindValue(":ACT_USER", user);
    query.bindValue(":ACT_MODULE", i );
    query.bindValue(":ACT_OPERATION", oper);
    query.bindValue(":ACT_DOCID", docid);
    query.exec();
    if(query.lastError().isValid()) {
        emit lastError(query.lastError().text());
        QSqlDatabase::database().rollback();
    }

    QSqlDatabase::database().commit();
}
