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

#include "vabstractprint.h"

VAbstractPrint::VAbstractPrint(QObject *parent)
   : QObject(parent),
     leftMargin(10.0), rightMargin(10.0), topMargin(10.0), bottomMargin(10.0),
     subSqlQuery(""), dataStringList(QStringList()),
     vpWidget(nullptr),
     vpPrinter(new QPrinter(QPrinter::HighResolution)),
     __rowsCount(0)
{
   vpPrinter->setPaperSize(QPrinter::A4);
   lstHeaders = QStringList(); // inicializa nome das colunas 
}


VAbstractPrint::~VAbstractPrint()
{
  delete vpPrinter;
}

void VAbstractPrint::setData(QVariant data)
{
    if(data.type() == QVariant::String ) {
        subSqlQuery = data.toString();
    } else if(data.type() == QVariant::StringList ) {
        QStringList list(data.toStringList());
        for(int i=0; i < list.size(); ++i) {
            dataStringList << list.at(i).toUtf8();
        }
    }
}

QVariant VAbstractPrint::data(void)
{
    if(!subSqlQuery.isEmpty()) {
        return subSqlQuery;
    } else if(!dataStringList.isEmpty() ) {
         return dataStringList;
    }

    return QVariant();
}

/**
 * Cria e mostra o dialogo para previsualizacao dos relatorios
 * gerados
 * permite ao usuario setar a orientacao do papel e impressao
 */
void VAbstractPrint::preview(QPrinter::Orientation orientation)
{
    vpPrinter->setOrientation(orientation);

    QPrintPreviewDialog *d = new QPrintPreviewDialog(vpPrinter,
                                  vpWidget,
                                  Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint );

    d->setWindowTitle(tr("Visualização de Relatórios"));
    connect(d, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printSlot(QPrinter *)));
    d->exec();
}


/**
 * Configura o nome do arquivo de saida da impressao.
 * Informe um nome para o arquivo tipo PDF que ira ser 
 * gerado contendo a impressao.
 * @param ofname 
 */
void VAbstractPrint::setOutputFilename(QString ofname)
{
   if(!ofname.isEmpty()) {
      if(!ofname.endsWith(".pdf"))
         ofname += ".pdf";
      vpPrinter->setOutputFileName(ofname);
   }
}


/**
 * Configura o o titulo principal do relatorio
 * @param title 
 */
void VAbstractPrint::setTitle(QString title)
{
   repTitle = title;
}


/**
 * Configura o sub-titulo do relatoiro
 * @param subTitle 
 */
void VAbstractPrint::setSubTitle(QString subTitle)
{
   repSubTitle = subTitle;
}


/**
 * @brief VAbstractPrint::subTitle
 *        Retorna o subtitulo informado pelo usuario
 * @return
 */
QString VAbstractPrint::subTitle()
{
    return(repSubTitle);
}

/**
 * @brief VAbstractPrint::title
 *        Retorna o titulo informado pelo usuario
 * @return
 */
QString VAbstractPrint::title()
{
    return(repTitle);
}

/**
 * Passa uma QString() com a declaracao SQL para
 * o QSqlQueryModel() executar
 *
 * @note Esta funcao pode ser demorada dependendo
 *       da consulta e outros fatores relacionados
 *       ao desempenho do banco de dados
 * @param query 
 */
void VAbstractPrint::setQueryModel(QString query)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    emit message( tr("Aguarde Processando SQL ..."));
    vpSqlModel.setQuery(query);
    if(vpSqlModel.lastError().isValid()  ) {
        emit lastError( tr("#")+vpSqlModel.lastError().text() );
    }
    emit message( tr(""));
    __rowsCount = vpSqlModel.rowCount();
    QApplication::restoreOverrideCursor();
}


int VAbstractPrint::rowsCount()
{
    return __rowsCount;
}

/**
 * Passa uma string list contendo valores que serão
 * usados para gerar o relatorio.
 * Por exemplo, pode-se passar o valor de pesquisa de
 * registros na tabela.
 * Exemplo: stringList << "10" << "15" << "23" << "1";
 * @param strList 
 */
void VAbstractPrint::setStringList(QStringList strList, QString query)
{
  vpStrList = strList;
  vpStrQuery = query;
}

/**
 * Passa uma lista contendo os nomes das colunas que
 * irao ser impressas.
 * Se esta funcao nao for usada, os nomes dos campos
 * da tabela SQL serao usados.
 * @param list 
 */
void VAbstractPrint::setColumnHeaders(QStringList list)
{
  lstHeaders = list;
}


/**
 * Cria uma QString() com os nomes dos titulos das colunas
 * do relatorio
 */
void VAbstractPrint::columnHeaders()
{
  qsColHeader.clear();
  if(lstHeaders.count() == 0) {
     for(int i=0; i<vpSqlModel.columnCount(); ++i) {
         qsColHeader += vpSqlModel.record(0).fieldName(i)+"\t";
     }
  }
  else {
    if(lstHeaders.count() == vpSqlModel.columnCount()) {
       for(int i=0; i<lstHeaders.count(); ++i) {
         qsColHeader += lstHeaders.at(i)+"\t";
       }
    }
  }
}


/**
 * Retorna a altura ocupada pela fonte na linha a ser impressa
 * @param fontMetrics 
 * @param str 
 * @return a altura ocupada pela fonte
 */
qreal VAbstractPrint::getFontRectHeight(QFontMetricsF fontMetrics, QString str)
{
   QRectF rect = fontMetrics.boundingRect(str);
   return( rect.height() );
}


/**
 * Retorna a largura do texto na linha a ser impressa
 * @param fontMetrics 
 * @param str 
 * @return a largura do texto na linha a ser impressa
 */
qreal VAbstractPrint::getFontRectWidth(QFontMetricsF fontMetrics, QString str)
{
   QRectF rect = fontMetrics.boundingRect(str);
   return( rect.width() );
}


/**
 * Retorna a altura da area de impressao.
 * Ou seja, da area que efetivamente pode ser impressa no papel
 * @return 
 */
qreal VAbstractPrint::pageHeight()
{
  return(vpPrinter->paperRect().height() - ( vpPrinter->paperRect().height() - vpPrinter->pageRect().height()));
}

/**
 * Retorna a largura da area de impressao.
 * Ou seja, da area que efetivamente pode ser impressa no papel
 * @return 
 */
qreal VAbstractPrint::pageWidth()
{
  return(vpPrinter->paperRect().width() - ( vpPrinter->paperRect().width() - vpPrinter->pageRect().width()));
};


/**
 * Slot utilizado pela class QPrintPreviewDialog para 
 * gerar o relatorio antes da previsalizacao
 * Este slot(funcao) nao gera o relatorio diretamente,
 * ele chama a funcao virtual printReport() para fazer
 * este trabalho.
 * @param Printer 
 */
void VAbstractPrint::printSlot(QPrinter *Printer)
{
  printReport(Printer);
}

/**
 * @brief VAbstractPrint::fitToPage
 *        Dimensiona a imagem passada para caber dentro
 *        da area de impressao da pagina
 * @param image
 * @param r1
 * @return
 */
qreal VAbstractPrint::fitToPage(QImage image, QRectF r1)
{
  static qreal factor=1.0; // tamanho original
  QSizeF size = image.size();
  size.scale(r1.size()*factor, Qt::KeepAspectRatio);
  if(size.height()<= pageHeight()-300.0 && size.width() <= pageWidth()) {
     return(factor);
  }
  else {
     if(factor == 0.0)
        return(factor);
     factor -= 0.1;
     factor = fitToPage(image, r1);
  }
  return(factor);
}

/**
 * Esta funcao virtual pura e usada para se gerar o relatorio
 * baseado no desenho desejado pelo usuario.
 * O padrao dela e gerar um relatorio em forma de tabela de colunas
 * @param vpPrinter 
 */
void VAbstractPrint::printReport(QPrinter *vpPrinter)
{
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

   vpPainter.begin(vpPrinter);
   QProgressDialog progress(tr("Gerando Relatório ..."), tr("Cancelar"), 0, vpSqlModel.rowCount());
   progress.setWindowModality(Qt::ApplicationModal);

   qreal y=0;

   QFont font("Courier New",10);
   font = QFont(font, vpPainter.device());
   QFontMetricsF fontMetrics(font);

   columnHeaders(); // monta o nome das colunas

   int npag = 1;
   y = repHeader(npag,true);

   qreal alturapag = vpPrinter->paperRect().height() - ( vpPrinter->paperRect().height() - vpPrinter->pageRect().height());

   vpPainter.setFont(font);
   QString texto;
   for(int i=0; i<=vpSqlModel.rowCount()-1;++i) {
       qApp->processEvents(); // libera eventos pendentes
       if (progress.wasCanceled())
             break;
       progress.setValue(i);

       texto.clear();
       for(int col=0; col<vpSqlModel.columnCount();++col) {
           texto += vpSqlModel.record(i).value(col).toString()+"\t";
       }

       QRectF rect = fontMetrics.boundingRect(texto);
       y += rect.height();

       if( y <= alturapag)
           vpPainter.drawText(QPointF(leftMargin, y), texto); // plota na pagina corrente

       if( y > alturapag ) {
           vpPrinter->newPage();
           y = repHeader(++npag,true);
           vpPainter.setFont(font);
           vpPainter.drawText(QPointF(leftMargin,  y += rect.height()), texto);
       }
   }

   vpPainter.end();
   progress.setValue(vpSqlModel.rowCount());
   QApplication::restoreOverrideCursor();
}


/**
 * Imprime o cabecalho padrao do relatorio
 * @param npag -> numero da pagina corrente
 * @param plotHeaders -> Imprime ou nao o titulo nas colunas
 *                       default: false
 * @return -> a altura em pixels ocupada pelo cabacalho.
 */
qreal VAbstractPrint::repHeader(int npag, bool plotHeaders)
{
   qreal lin = 0;
   QFont font14B("Arial",14, QFont::Bold);
   QFont font11N("Arial",11);
   QFont font8N("Arial",8);

   font14B = QFont(font14B, vpPainter.device());
   QFontMetricsF fontM14B(font14B);
   font11N = QFont(font11N, vpPainter.device());
   QFontMetricsF fontM11N(font11N);
   font8N = QFont(font8N, vpPainter.device());
   QFontMetricsF fontM8N(font8N);

   vpPainter.setFont(font14B);
   vpPainter.drawText(leftMargin, leftMargin+fontM14B.lineSpacing(), repTitle);

   lin = leftMargin+fontM14B.lineSpacing();

   vpPainter.setFont(font11N);
   vpPainter.drawText(leftMargin, lin+fontM11N.lineSpacing(), repSubTitle);

   qreal max = pageWidth(); 

   vpPainter.setPen(QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
   lin += fontM11N.lineSpacing() / 2;
   vpPainter.drawLine( QLineF(leftMargin, lin+fontM11N.lineSpacing(),max, lin+fontM11N.lineSpacing()) );

   int dia = QDate::currentDate().day();
   int mes = QDate::currentDate().month();
   QString data = QString("%0/%1/%2").arg((dia<10? "0"+QString::number(dia) : QString::number(dia))).arg((mes<10? "0"+QString::number(mes) : QString::number(mes))).arg(QDate::currentDate().year());

   QString hora =  QString("%0").arg(QTime::currentTime().toString());
   QString pag = QString::number(npag);
   QString info = QString("%0-%1-P:%2").arg(data).arg(hora).arg(pag);

   vpPainter.setFont(font8N);
   lin += fontM11N.lineSpacing();
   vpPainter.drawText(QRect(max-fontM11N.width(info), lin, fontM11N.width(info), fontM11N.height()), Qt::AlignRight, info);

   // plota cabecalho das columas
   if(plotHeaders) {
      lin += fontM11N.lineSpacing();
      vpPainter.setFont(font11N);
      vpPainter.drawText(leftMargin, lin+fontM11N.lineSpacing(), qsColHeader);
   }
   return(lin+fontM11N.lineSpacing());
}


