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

#include "printentity.h"

void PrintEntity::printReport(QPrinter *printer)
{
    vpPainter.begin(printer);

    qreal leftMargin = getLeftMargin();

    qreal y=0;

    QFont font10("Courier New",10);
    font10 = QFont(font10, vpPainter.device());
    QFontMetricsF fontMetrics10(font10);

    QFont font8("Courier New",8);
    font8 = QFont(font8, vpPainter.device());
    QFontMetricsF fontMetrics8(font8);

    QFont font8Bold("Courier New",8, QFont::Bold );
    font8Bold = QFont(font8Bold, vpPainter.device());
    QFontMetricsF fontMetrics8Bold(font8Bold);


    int npag = 1;
    y = repHeader(npag);

    qreal alturapag = pageHeight();

    vpPainter.setFont(font8);

    QProgressDialog progress(tr("Gerando Relatório ..."), tr("Cancelar"), 0, vpSqlModel.rowCount()-1);
    progress.setWindowModality(Qt::ApplicationModal);


    for(int i=0; i<=vpSqlModel.rowCount()-1;++i) {
        qApp->processEvents(); // libera eventos pendentes
        if(progress.wasCanceled())
           break;
        progress.setValue(i);



        QString l1 = QString(tr("#ID: %0 DATA: %1 CLASSE: %2")).
               arg(vpSqlModel.record(i).value("ET_IDENT").toString().rightJustified(10,' ')).
               arg(vpSqlModel.record(i).value("ET_DATA").toString()).
               arg(vpSqlModel.record(i).value("ET_CATEG").toString());

        QString l2 = QString(tr("NOME: %0 APELIDO: %1")).
               arg(vpSqlModel.record(i).value("ET_NOME").toString().leftJustified(60,' ') ).
               arg(vpSqlModel.record(i).value("ET_ALIAS").toString());

        QString l3 = QString(tr("ENDEREÇO: %0 BAIRRO: %1")).
               arg(vpSqlModel.record(i).value("ET_ENDER").toString().leftJustified(60,' ') ).
               arg(vpSqlModel.record(i).value("ET_BAIR").toString().leftJustified(60,' ') );

        QString l4 = QString(tr("CIDADE: %0 UF: %1 CEP: %2")).
               arg(vpSqlModel.record(i).value("ET_CITY").toString().leftJustified(60,' ') ).
               arg(vpSqlModel.record(i).value("ET_UF").toString().leftJustified(2,' ') ).
               arg(vpSqlModel.record(i).value("ET_CEP").toString().leftJustified(10,' ') );

        QString l5 = QString(tr("TELS: [%0-%1] [%2-%3]")).
               arg(vpSqlModel.record(i).value("ET_TELMOD1").toString().leftJustified(25,' ') ).
               arg(vpSqlModel.record(i).value("ET_TELNUM1").toString().leftJustified(30,' ') ).
               arg(vpSqlModel.record(i).value("ET_TELMOD2").toString().leftJustified(25,' ') ).
               arg(vpSqlModel.record(i).value("ET_TELNUM2").toString().leftJustified(30,' ') );

        QString l6 = QString(tr("TELS: [%0-%1] [%2-%3]")).
               arg(vpSqlModel.record(i).value("ET_TELMOD3").toString().leftJustified(25,' ') ).
               arg(vpSqlModel.record(i).value("ET_TELNUM3").toString().leftJustified(30,' ') ).
               arg(vpSqlModel.record(i).value("ET_TELMOD4").toString().leftJustified(25,' ') ).
               arg(vpSqlModel.record(i).value("ET_TELNUM4").toString().leftJustified(30,' ') );

        QString l7 = QString(tr("CONTATOS: %0 EMAIL: %1")).
                arg(vpSqlModel.record(i).value("ET_CONTATO1").toString().leftJustified(40,' ') ).
                arg(vpSqlModel.record(i).value("ET_EMAIL1").toString() );

        QString l8 = QString(tr("CONTATOS: %0 EMAIL: %1")).
                arg(vpSqlModel.record(i).value("ET_CONTATO2").toString().leftJustified(40,' ') ).
                arg(vpSqlModel.record(i).value("ET_EMAIL2").toString() );

        QString l9 = QString(tr("CPF/RG: %0 / %1 CPNJ/IE/IM: %2 / %3 / %4")).
                arg(vpSqlModel.record(i).value("ET_CPF").toString().leftJustified(19,' ') ).
                arg(vpSqlModel.record(i).value("ET_RG").toString().leftJustified(21,' ') ).
                arg(vpSqlModel.record(i).value("ET_CNPJ").toString().leftJustified(19,' ') ).
                arg(vpSqlModel.record(i).value("ET_IE").toString().leftJustified(20,' ') ).
                arg(vpSqlModel.record(i).value("ET_IM").toString().leftJustified(20,' ') );

        QString l10 = tr("URL:")+vpSqlModel.record(i).value("ET_URL").toString();


        QString l11, l12;
        l11= QString(tr("CÓDIGO: %0 CLIENTE: %1")).
                    arg(vpSqlModel.record(i).value("OBRA_CODIGO").toString().leftJustified(8,' ') ).
                    arg(vpSqlModel.record(i).value("ET_OBCLI").toString().leftJustified(60,' ') );
        l12= QString(tr("CEI: %0 CRONOGRAMA: %1")).
                    arg(vpSqlModel.record(i).value("ET_OBCEI").toString().leftJustified(20,' ') ).
                    arg(vpSqlModel.record(i).value("ET_OBCRONO").toString().rightJustified(6,' ') );


        QString l13 = QString(tr("ESPECIFICAÇÃO: %0")).
                arg(vpSqlModel.record(i).value("ET_OUTESPEC").toString().rightJustified(50,' ') );

        QString l14 = QString(tr("%0: %1")).
                arg(vpSqlModel.record(i).value("ET_OUTNFLD1").toString().rightJustified(15,' ') ).
                arg(vpSqlModel.record(i).value("ET_OUTDFLD1").toString());

        QString l15 = QString(tr("%0: %1")).
                arg(vpSqlModel.record(i).value("ET_OUTNFLD2").toString().rightJustified(15,' ') ).
                arg(vpSqlModel.record(i).value("ET_OUTDFLD2").toString());

        QString l16 = QString(tr("%0:")).
                arg(vpSqlModel.record(i).value("ET_OUTOBS1").toString());


        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l1)), l1);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l2)), l2);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l3)), l3);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l4)), l4);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l5)), l5);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l6)), l6);

        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l7)), l7);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l8)), l8);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l9)), l9);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l10)), l10);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l11)), l11);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l12)), l12);

        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l13)), l13);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l14)), l14);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l15)), l15);
        vpPainter.drawText(QPointF(leftMargin, y+=getFontRectHeight(fontMetrics8,l16)), l16);

        qreal y1 = y+=getFontRectHeight(fontMetrics8,"X");
        vpPainter.drawLine(QPointF(leftMargin, y1), QPointF(pageWidth(), y1) );

        if( y >= alturapag-1000 ) {
            vpPrinter->newPage();
            y = repHeader(++npag);
            vpPainter.setFont(font8);
        }
    }
    vpPainter.end();
    progress.setValue(vpSqlModel.rowCount()-1);

}
