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

#include "printdocanalitic.h"

void PrintDocAnalitic::printReport(QPrinter *printer)
{
   vpPainter.begin(printer);

   qreal leftMargin = getLeftMargin();

   qreal y=0;

   QFont font("Courier New",10);
   font = QFont(font, vpPainter.device());
   QFontMetricsF fontMetrics(font);

   columnHeaders(); // monta o nome das colunas

   int npag = 1;
   y = repHeader(npag);

   qreal alturapag = pageHeight();

   vpPainter.setFont(font);

   QProgressDialog progress(tr("Gerando Relatório ..."), tr("Cancelar"), 0, vpSqlModel.rowCount()-1);
   progress.setWindowModality(Qt::ApplicationModal);

   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

   for(int i=0; i<=vpSqlModel.rowCount();++i) {
       qApp->processEvents(); // libera eventos pendentes
       if (progress.wasCanceled())
             break;
       progress.setValue(i);

       QString relaNome = tr("Entidade: ")+vpSqlModel.record(0).value("ENTIDADE").toString();
       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,relaNome)), relaNome);

       QString STMT = QString("SELECT HIGH_PRIORITY "
                              "DL_IDENT, ET_IDENT, DATE_FORMAT(DL_DTDOC,'%d/%m/%Y') AS DL_DTDOC, "
                              "DL_NUMERO, DL_VALOR, "
                              "(SELECT ET_NOME FROM  DOCENTITY A WHERE A.ET_IDENT=B.DL_ETRELA) AS RELACAO, "
                              "(SELECT ET_CATEG FROM DOCENTITY  A WHERE A.ET_IDENT=B.DL_ETRELA) AS ET_CATEG "
                              "FROM DOCLIBRARY B WHERE ET_IDENT=%0 AND DL_ETRELA=%1 AND DL_VALOR > 0.0 %2").
               arg(vpSqlModel.record(i).value("ET_IDENT").toString()).
               arg(vpSqlModel.record(i).value("DL_ETRELA").toString()).
               arg(sqlWhere);

       QSqlQueryModel m;
       m.setQuery(STMT);
       if(m.rowCount() == 0)
          continue;

       double soma=0.0;

       QString relaNome1 = QString();
       if(m.record(0).value("ET_CATEG").toInt() == 0)
          relaNome1 = tr("Empresa: ");
       else if(m.record(0).value("ET_CATEG").toInt() == 1)
          relaNome1 = tr("Cliente: ");
       else if(m.record(0).value("ET_CATEG").toInt() == 2)
          relaNome1 = tr("Fornecedor: ");
       else if(m.record(0).value("ET_CATEG").toInt() == 3)
          relaNome1 = tr("Projeto/Obra: ");
       else if(m.record(0).value("ET_CATEG").toInt() == 4)
          relaNome1 = tr("Outros: ");
       else if(m.record(0).value("ET_CATEG").toInt() == 5)
          relaNome1 = tr("Simples: ");

       relaNome1 += m.record(0).value("RELACAO").toString();
       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,relaNome1)), "   "+relaNome1);

       QString subTitle = QString(40,' ')+tr("ID         Documento  Data Emissão Valor R$");
       QString subTitle1 = QString(40,' ')+"---------- ---------- ------------ --------------";

       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,subTitle)), subTitle);
       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,subTitle1)), subTitle1);

       int cLinha=0;
       for(int j=0; j<=m.rowCount(); ++j) {
          QString c0 = QString("%0").arg(m.record(j).value("DL_IDENT").toString()).leftJustified(10,' ');
          QString c1 = QString("%0").arg(m.record(j).value("DL_NUMERO").toString()).rightJustified(10,' ');
          QString c2 = QString("%0").arg(m.record(j).value("DL_DTDOC").toString()).leftJustified(10,' ');
          double c3 =m.record(j).value("DL_VALOR").toFloat();
          QString detail = QString(40,' ')+QString("%0 %1 %2   %L3").arg(c0).arg(c1).arg(c2).arg(c3,14,'f',2);
          soma += m.record(j).value("DL_VALOR").toFloat();

          if(c3>0.0)
             vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,detail)), detail);
          ++cLinha;
          if(cLinha==55) {
           vpPrinter->newPage();
           y = repHeader(++npag);
           vpPainter.setFont(font);
           vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,relaNome)), relaNome);
           vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,relaNome1)), "   "+relaNome1);
           cLinha=0;
          }
       }
       QString qsTotal = QString(55,' ')+QString("=======> Total R$");
       QString qsSoma = QString("%L0").arg(soma,14,'f',2);
       qsSoma = QString(75,' ')+qsSoma.rightJustified(14,' ');
       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,qsTotal)), qsTotal);
       vpPainter.drawText(QPointF(leftMargin, y), qsSoma);

       if( y < alturapag-1000) {
           vpPrinter->newPage();
           y = repHeader(++npag);
           vpPainter.setFont(font);
       }

   }
   vpPainter.end();
   progress.setValue(vpSqlModel.rowCount()-1);
   QApplication::restoreOverrideCursor();
}

