/***************************************************************************
 * Copyright (c) 2007-2019                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase project, a images/documents           *
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

#include "tvfinddocs.h"

tvFindDocs::tvFindDocs(QWidget *parent) :
    QTreeView(parent)
{
   // __fillView();
}

/**
 * @brief SLOT tvFindDocs::fillView
 *
 */
void tvFindDocs::fillView(bool b)
{
    Q_UNUSED(b)
    //if(b)
    //    __fillView();
}

void tvFindDocs::valueStartYearChanged(int i)
{
    iStartYear = i;
}

void tvFindDocs::valueEndYearChanged(int i)
{
    iEndYear = i;
}

void tvFindDocs::toolButtonFindClicked(bool)
{
    if(iStartYear <= iEndYear ) {
       __fillView();
    }
}

void tvFindDocs::rangeYears(const int &a, const int &b)
{
    iStartYear = a;
    iEndYear = b;
}

void tvFindDocs::__fillView(void)
{
    /**
     * NOTES: Usa a macro Q_LIKELY() a fim  de tentar otimizar
     *      o codigo gerado pelo compilador.
     */

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    emit showMessage(tr("Montando lista de documentos. Aguarde ..."));

    model = new QStandardItemModel;
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("Classes")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("#ID")));
    model->setHorizontalHeaderItem(2, new QStandardItem(tr("Data\nCadastro")));
    model->setHorizontalHeaderItem(3, new QStandardItem(tr("Operador")));
    model->setHorizontalHeaderItem(4, new QStandardItem(tr("No.\nDocumento")));
    model->setHorizontalHeaderItem(5, new QStandardItem(tr("Data")));
    model->setHorizontalHeaderItem(6, new QStandardItem(tr("Valor R$")));
    model->setHorizontalHeaderItem(7, new QStandardItem("")); // campo vazio para fins de formatacao

    for(qint8 iClasse=0; iClasse<6; iClasse++) {
        QStandardItem *pClasse = new QStandardItem( uClassesOfEntities().at(iClasse) );
        pClasse->setEditable(false);
        pClasse->setBackground(QBrush(Qt::lightGray));
        pClasse->setData("C", Qt::UserRole+1);
        model->appendRow(QList<QStandardItem *>() << pClasse );

        QString stmt = QString("SELECT HIGH_PRIORITY ET_IDENT, ET_NOME FROM DOCENTITY "
                               "WHERE ET_CATEG=%0 AND ET_ATIVO=1 ORDER BY ET_NOME").arg(iClasse);

        QSqlQuery query;
        query.prepare(stmt);
        query.exec();
        if( query.lastError().isValid()) {
           QApplication::restoreOverrideCursor();
           break;
        }

        if( query.size() > 0) {
            emit setMaximumItem(query.size());
            emit setVisibleItem(true);
        }

        while( Q_LIKELY(query.next()) )
        {
          QString sItem =  query.value(1).toString()+"#"+query.value(0).toString();
          QStandardItem *child = new QStandardItem( sItem );
          child->setEditable( false );
          child->setData("I", Qt::UserRole+1);
          pClasse->appendRow(QList<QStandardItem *>() << child);

          QString stmt = QString("SELECT HIGH_PRIORITY "
                                 "(SELECT CONCAT_WS('#',A.ET_NOME,A.ET_IDENT) FROM DOCENTITY A WHERE A.ET_IDENT=B.DL_ETRELA) AS RELACAO, "
                                 "B.DL_IDENT, DATE_FORMAT(B.DL_DTCADAST, '%d/%m/%Y') AS DTCADAST, "
                                 "B.DL_OPERADOR, B.DL_NUMERO, DATE_FORMAT(B.DL_DTDOC, '%d/%m/%Y') AS DTDOC, "
                                 "FORMAT(B.DL_VALOR,2) AS DOCVALOR, B.DL_TOPSEC "
                                 "FROM DOCLIBRARY B WHERE B.ET_IDENT=%0 AND "
                                 "YEAR(DL_DTCADAST) BETWEEN %1 AND %2 ORDER BY B.DL_IDENT")
                             .arg(query.value(0).toULongLong())
                             .arg(iStartYear).arg(iEndYear);

          QSqlQuery q;
          q.prepare(stmt);
          q.exec();

          int i = 0;
          while( Q_LIKELY(q.next()) ) {
              QStandardItem *pNomeRelacao = new QStandardItem(q.value(0).toString());
              pNomeRelacao->setEditable( false );
              pNomeRelacao->setData("R", Qt::UserRole+1);
              pNomeRelacao->setIcon( (q.value(7).toBool() ?
                                          QIcon(":/lock_close_32x32.png") :
                                          QIcon(":/lock_open_32x32.png")
                                      ) );
              QStandardItem *pId;
              QStandardItem *pDtCadast;
              QStandardItem *pOperator;
              QStandardItem *pNumDoc;
              QStandardItem *pDtDoc;
              QStandardItem *pDocValor;

              if(q.value(7).toBool()) { // se tem senha mostra com restricoes
                 pId = new QStandardItem( q.value(1).toString() );
                 pDtCadast = new QStandardItem( q.value(2).toString() );
                 pOperator = new QStandardItem( q.value(3).toString() );
                 pNumDoc = new QStandardItem( "" );
                 pDtDoc = new QStandardItem( "" );
                 pDocValor = new QStandardItem( "" );
              } else { // liberado
                 pId = new QStandardItem( q.value(1).toString() );
                 pDtCadast = new QStandardItem( q.value(2).toString() );
                 pOperator = new QStandardItem( q.value(3).toString() );
                 pNumDoc = new QStandardItem( q.value(4).toString() );
                 pDtDoc = new QStandardItem( q.value(5).toString() );
                 pDocValor = new QStandardItem( q.value(6).toString() );
              }

              pId->setEditable( false );
              pId->setTextAlignment(Qt::AlignRight);
              pDtCadast->setEditable( false );
              pOperator->setEditable( false );
              pNumDoc->setEditable( false );
              pNumDoc->setTextAlignment(Qt::AlignRight);
              pDtDoc->setEditable( false );
              pDocValor->setEditable( false );
              pDocValor->setTextAlignment(Qt::AlignRight);

              child->appendRow(QList<QStandardItem*>() << pNomeRelacao <<
                               pId << pDtCadast << pOperator << pNumDoc <<
                               pDtDoc << pDocValor);

              emit setValueItem(i++);

          }
        }
    }
    setModel(model);
    setColumnWidth(0, 500); // fixa a largua da coluna 0
    show();

    QApplication::restoreOverrideCursor();
    emit showMessage(QString());
    emit setVisibleItem(false);
}
