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

#include "printdocgeneral.h"

void PrintDocGeneral::printReport(QPrinter *printer)
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

    QString etNome = "Entidade: ["+vpSqlModel.record(0).value("ENTIDADE").toString()+"]";

    double grandTotal = 0.0;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    for(int i=0; i<=vpSqlModel.rowCount()-1;++i) {
        qApp->processEvents(); // libera eventos pendentes
        if (progress.wasCanceled())
              break;
        progress.setValue(i);
        QString dl_ident = vpSqlModel.record(i).value("DL_IDENT").toString();
        QString dl_dtcadast = vpSqlModel.record(i).value("DL_DTCADAST").toString();
        QString dl_operador = vpSqlModel.record(i).value("DL_OPERADOR").toString();
        QString dl_dpto = vpSqlModel.record(i).value("DL_DPTO").toString();
        QString dl_dtdoc = vpSqlModel.record(i).value("DL_DTDOC").toString();
        QString dl_numero = vpSqlModel.record(i).value("DL_NUMERO").toString();
        QString dl_valor = vpSqlModel.record(i).value("DL_VALOR").toString();
        QString dl_fname = vpSqlModel.record(i).value("DL_FNAME").toString();
        QString relacao = vpSqlModel.record(i).value("RELACAO").toString();
        grandTotal += dl_valor.toFloat();

        QString l1 = tr("Ident   : %0 Cadastro: %1").arg(dl_ident,10, ' ').arg(dl_dtcadast);
        QString l2 = tr("Operador: %0").arg(dl_operador);
        QString l21 = tr("N.Fiscal: %0").arg(dl_numero);
        QString l22 = tr("Emissão : %0").arg(dl_dtdoc);
        QString l23 = tr("Valor R$: %0").arg( QString::number(dl_valor.toFloat(),'f',2) );
        QString l3 = tr("Depart. : %0").arg(dl_dpto);
        QString l4 = tr("Imagem  : %0").arg(dl_fname);
        QString l6 = tr("Relação : %0").arg(relacao);

        if( y < alturapag-1000) {
           if( i == 0) { // so na primeira pagina
               vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,etNome)), etNome);
               y+=getFontRectHeight(fontMetrics,etNome);
           }

           vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,l1)), l1);
           vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,l6)), l6);
           vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,l2)), l2);
           qreal x = pageWidth()-2500.0;
           vpPainter.drawText(QPointF(x, y), l21);
           vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,l3)), l3);
           vpPainter.drawText(QPointF(x, y), l22);
           vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,l4)), l4);
           vpPainter.drawText(QPointF(x, y), l23);

           vpPainter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
           vpPainter.drawLine( QLineF(leftMargin, y+50,pageWidth(), y+50) );
        }
        if( y >= alturapag-1000 ) {
            vpPrinter->newPage();
            y = repHeader(++npag);
            vpPainter.setFont(font);
            vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,etNome)), etNome);
            y+=getFontRectHeight(fontMetrics,etNome);
        }
    }
    // Imprime pagina adicional com o resumo dos valores dos documentos
    if(grandTotal > 0.0) {
       QString totalDocs    = QString(tr("Total de Documentos...: %L0")).arg(vpSqlModel.rowCount());
       QString totalValores = QString(tr("Total Geral.........R$: %L0")).arg(  QString::number(grandTotal,'f',2) );
       vpPrinter->newPage();
       y = repHeader(++npag);
       vpPainter.setFont(font);
       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,etNome)), etNome);
       y+=getFontRectHeight(fontMetrics,etNome)*2;
       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,etNome)), tr("* RESUMO GERAL *"));
       y+=getFontRectHeight(fontMetrics,etNome)*2;
       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,etNome)), totalDocs);
       vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics,etNome)), totalValores);
    }

    vpPainter.end();
    progress.setValue(vpSqlModel.rowCount()-1);
    QApplication::restoreOverrideCursor();
}
