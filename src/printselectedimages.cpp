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

#include "printselectedimages.h"

/*
PrintSelectedImages::PrintSelectedImages(QObject *parent) :
    VAbastractPrint(parent)
{
}
*/


void PrintSelectedImages::printReport(QPrinter *printer)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QProgressDialog progress(tr("Gerando Relatório ..."), tr("Cancelar"), 0, vpSqlModel.rowCount());
    progress.setWindowModality(Qt::ApplicationModal);
    progress.showNormal();

    if(!vpPainter.begin(printer)) { // failed to open file
        QMessageBox::critical( vpWidget , PROGRAM_NAME,
          tr("Não foi possível abrir impressora para escrita.\n"
             "Pode ser que o dispositivo não esteja instalado."),QMessageBox::Close );
         QApplication::restoreOverrideCursor();
         return;
    }
    QFont font14B("Arial",14, QFont::Bold);
    font14B = QFont(font14B, vpPainter.device());
    QFontMetricsF fontM14B(font14B);
    qreal lineSpacing = fontM14B.lineSpacing();

    // Retangulo que corresponde a area de impressao do papel
    QRectF r1(0,0, pageWidth(), pageHeight());

    header(r1, title()); // cabec para primeira pagina
    for(int i=0; i<vpSqlModel.rowCount();++i) {
       qApp->processEvents(); // libera eventos pendentes
       if (progress.wasCanceled())
             break;
       progress.setValue(i);
       TypeImage ti;
       if( !ti.isImageViewable(vpSqlModel.record(i).value("DL_FNAME").toString()) ) // nao e imprimivel
           continue;

       if( vpSqlModel.record(i).value("IM_IMAGE").toByteArray().isEmpty() )
         continue;

       /* Descompacta bytearray da imagen usando threads */
       QFuture<QByteArray> future = QtConcurrent::run(qUncompress, vpSqlModel.record(i).value("IM_IMAGE").toByteArray() );
       QByteArray ba = future.result();

       QImage image;
       image.loadFromData(ba);
       QSizeF size = image.size();

       size.scale(r1.size(), Qt::KeepAspectRatio);
       if(size.width() > pageWidth() || size.height() > pageHeight()-300.0) {
          qreal fact = fitToPage(image, r1); // reduz ate caber na pagina
          if( fact == 0.0 ) // nao cabe na pagina
             continue;
          size.scale(r1.size()*fact, Qt::KeepAspectRatio);
       }

       vpPainter.setViewport(r1.x(), r1.y()+lineSpacing+350.0,
                            size.width(), size.height());
       vpPainter.setWindow(image.rect());
       vpPainter.drawImage(0,0, image);

       if(!printer->newPage()) {
          QMessageBox::critical( vpWidget , PROGRAM_NAME,
                 tr("Problemas em alocar nova pagina no disco. Disco pode estar cheio."),
                 QMessageBox::Close);
          break;
       }
       header(r1, title()); // cabec para primeira pagina
    } // fim for(;;)

    vpPainter.end();
    progress.setValue(0);
    QApplication::restoreOverrideCursor();
}


void PrintSelectedImages::header(QRectF r, const QString title)
{
    QFont font14B("Arial",14, QFont::Bold);
    font14B = QFont(font14B, vpPainter.device());
    QFont font11B("Arial",11, QFont::Bold);
    font11B = QFont(font11B, vpPainter.device());
    QFont font9("Arial",9, QFont::Normal);
    font9 = QFont(font9, vpPainter.device());
    QFontMetricsF fontM9(font9);

    QFontMetricsF fontM14B(font14B);
    qreal lineSpacing = fontM14B.lineSpacing();


    vpPainter.setViewport(r.x(), r.y(), pageWidth(),  lineSpacing+300.0);
    vpPainter.setWindow(r.x(), r.y(), pageWidth(),  lineSpacing+300.0);
    vpPainter.setBrush(QColor("#e6e6e6")); // cinza 10% HTML #e6e6e6 RGB 230,230,230
    vpPainter.drawRect(r.x(), r.y(), pageWidth(),  lineSpacing+300.0);


    vpPainter.setFont(QFont(font14B, vpPainter.device()));
    vpPainter.drawText(50.0, lineSpacing, tr("Docbase"));

    vpPainter.setFont(QFont(font9, vpPainter.device()));
    vpPainter.drawText( 50.0, fontM14B.height()+lineSpacing, subTitle());

    vpPainter.setBrush(Qt::NoBrush);
}
