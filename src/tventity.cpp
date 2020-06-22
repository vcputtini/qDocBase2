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

#include "tventity.h"

tvEntity::tvEntity(QWidget *parent) :
    QTreeView(parent),
    iState(Qt::Checked),
    iAno(2000)
{   
    QPixmap(":/grndiamd.png");
    process();
}

/**
 * @brief tvEntity::update
 * @param b
 */
void tvEntity::update(bool b)
{
    if(b)
      process();
}

/**
 * @brief tvEntity::buttonSearchClicked
 * @param b
 */
void tvEntity::buttonSearchClicked(bool b)
{
    Q_UNUSED(b);

    if(pSearch.first > 0) {
       if(!NS_UTILITIES::uCNPJEmpty(pSearch.second) ||
          !NS_UTILITIES::uIEEmpty(pSearch.second) ||
          !NS_UTILITIES::uCPFEmpty(pSearch.second) ||
          !NS_UTILITIES::uRGEmpty(pSearch.second) ) {
            process(); // so recarrega a view
        }
    } else {
        process();
    }
}

/**
 * @brief tvEntity::tipoIndex
 * @param i
 */
void tvEntity::tipoIndex(int i)
{
    pSearch.first = i;
}

/**
 * @brief tvEntity::lineEditSearchChanged
 * @param text
 */
void tvEntity::lineEditSearchChanged(const QString & text)
{
    pSearch.second = text;
}

/**
 * @brief tvEntity::stateChanged
 * @param state
 */
void tvEntity::stateChanged(int state)
{
    iState = state;

    if(state == Qt::Unchecked)
        emit stateName(tr("Inativos"));
    else if(state == Qt::PartiallyChecked)
        emit stateName(tr("Ambos"));
    else if(state == Qt::Checked)
        emit stateName(tr("Ativos"));
    else
        Q_UNREACHABLE(); // codigo para otimizacao pelo compilador

    process();
}

/**
 * @brief tvEntity::valueYearChanged
 * @param i
 */
void tvEntity::valueYearChanged(int i)
{
    iAno = i;
    process();
}

/**
 * @brief tvEntity::valueE
 * @param i
 */
void tvEntity::valueE(int i)
{
    Q_UNUSED(i);
}

/**
 * @brief tvEntity::specialValueTextE
 * @param text
 */
void tvEntity::specialValueTextE(QString &text)
{
    Q_UNUSED(text);
}

/**
 * @brief tvEntity::process
 */
void tvEntity::process(void)
{
    /**
     * NOTES: Usa a macro Q_LIKELY() a fim  de tentar otimizar
     *      o codigo gerado pelo compilador.
     */

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    model = new VStandardItemModel_P;
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("Classes")));
    model->setAlternateColor(true);

    QString stmt_aux;
    switch(pSearch.first) {
    case 1:
        stmt_aux = QString(" AND ET_CNPJ = '%2'").arg(pSearch.second);
        break;
    case 2:
        stmt_aux = QString(" AND ET_IE = '%2'").arg(pSearch.second);
        break;
    case 3:
        stmt_aux = QString(" AND ET_CPF = '%2'").arg(pSearch.second);
        break;
    case 4:
        stmt_aux = QString(" AND ET_RG = '%2'").arg(pSearch.second);
    [[clang::fallthrough]]; default: // instrucao adiciona para evitar warning
        stmt_aux = "";
    }

    for( qint8 r=0; r<6; r++ ) {
        QStandardItem *item = new QStandardItem( uClassesOfEntities().at(r) );
        item->setEditable(false);

        QString stmt = "";

        if(iState == Qt::PartiallyChecked) {
            if(iAno==2000)
                stmt = QString("SELECT ET_IDENT, ET_NOME, ET_ATIVO, ET_CNPJ, ET_CPF FROM DOCENTITY WHERE ET_CATEG=%0 %1 "
                           "ORDER BY ET_NOME").arg(r).arg(stmt_aux);
            else
                stmt = QString("SELECT ET_IDENT, ET_NOME, ET_ATIVO, ET_CNPJ, ET_CPF FROM DOCENTITY WHERE ET_CATEG=%0 AND "
                           "YEAR(ET_DATA)=%1 %2 ORDER BY ET_NOME").arg(r).arg(iAno).arg(stmt_aux);
        } else if(iState == Qt::Checked) {
            if(iAno == 2000)
                stmt = QString("SELECT ET_IDENT, ET_NOME, ET_ATIVO, ET_CNPJ, ET_CPF FROM DOCENTITY WHERE ET_CATEG=%0 AND ET_ATIVO=1 %1 "
                      "ORDER BY ET_NOME").arg(r).arg(stmt_aux);
            else
                stmt = QString("SELECT ET_IDENT, ET_NOME, ET_ATIVO, ET_CNPJ, ET_CPF FROM DOCENTITY WHERE ET_CATEG=%0 AND ET_ATIVO=1 AND "
                          "YEAR(ET_DATA)=%1 %2 ORDER BY ET_NOME").arg(r).arg(iAno).arg(stmt_aux);
        } else if(iState == Qt::Unchecked) {
            if(iAno == 2000)
                stmt = QString("SELECT ET_IDENT, ET_NOME, ET_ATIVO, ET_CNPJ, ET_CPF FROM DOCENTITY WHERE ET_CATEG=%0 AND ET_ATIVO=0 %1"
                          "ORDER BY ET_NOME").arg(r).arg(stmt_aux);
            else
                stmt = QString("SELECT ET_IDENT, ET_NOME, ET_ATIVO, ET_CNPJ, ET_CPF FROM DOCENTITY WHERE ET_CATEG=%0 AND ET_ATIVO=0 AND "
                          "YEAR(ET_DATA)=%1 %2 ORDER BY ET_NOME").arg(r).arg(iAno).arg(stmt_aux);
        }

        QSqlDatabase::database().transaction();
        QSqlQuery query;
        query.prepare(stmt);
        query.exec();
        if( query.lastError().isValid()) {
            QSqlDatabase::database().rollback();
            QApplication::restoreOverrideCursor();
            break;
        }

        int max = query.size();
        if( max > 0) {
            emit setMaximumItem(query.size());
            emit setVisibleItem(true);
        }

        int i=0;
        QString _aux;
        while( Q_LIKELY( query.next() ) )
        {
           if( query.value(3).toString() == "" &&  query.value(4).toString() == "" ) {
                _aux = "";
           } else {
                if(!uCNPJEmpty(query.value(3).toString())) {
                   _aux = "["+query.value(3).toString()+"]";
                } else if(!uCPFEmpty(query.value(3).toString())) {
                   _aux = "["+query.value(4).toString()+"]";
                }
           }

           QString sItem;
           if(_aux != "") {
               sItem = query.value(1).toString()+_aux+"#"+query.value(0).toString();
           } else {
               sItem = query.value(1).toString()+"#"+query.value(0).toString();
           }
           QStandardItem *child = new QStandardItem( sItem );
           child->setEditable( false );
           if(iState == Qt::PartiallyChecked && query.value(2).toInt()==0)
               child->setIcon(QPixmap(":/reddiamd.png"));
           else if(iState == Qt::PartiallyChecked && query.value(2).toInt()==1 || iState == Qt::Checked)
               child->setIcon(QPixmap(":/grndiamd.png"));
           else if(iState == Qt::Unchecked)
               child->setIcon(QPixmap(":/reddiamd.png"));
           else
               Q_UNREACHABLE();
           item->appendRow( child );

           i++;
           if(max > 0) {
              emit setValueItem(i);
           }

         }
         QSqlDatabase::database().commit();
         model->setItem(r, 0, item);
    }
    setModel(model);
    show();
    QApplication::restoreOverrideCursor();
    emit setVisibleItem(false);
}
