/***************************************************************************
 * Copyright (c) 2007-2020                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com
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

#ifndef VABSTRACTPRINT_H
#define VABSTRACTPRINT_H

/**
	@author Volnei C Puttini <vcputtini@gmail.com>

	Esta classe abstrata proporciona uma base minima funcional para facilitar
	a criacao de relatorios.
	Os dados para o relatorio devem vir de uma QSqlQueryModel(),
        ou poderao ser passados via um string list que ira passar os 
	valores para um declaracao SQL SELECT que pegara os registros.

        A funcao virtual pura \b printReport(QPrinter* printer) se nao for
	redefinida pelo usuario, imprime uma listagem na forma de tabela
        colunada continua.

        %ChangeLog
        2011-11-24 - Adicionado o metodo void setData(QVariant data)
                     O qual permite passar dados para a classe para
                     usos especiais. No momento somente trata QString
                     e QStringList. O metodo QVariant data() retorna
                     os valores ao usuario

        2011-12-12 - Alterado metodo void preview(void) para
                     void preview(Qt::Orientation=Qt::Vertical).
                     O padrao e Vertical
                     O qual permite configurar a orientacao do papel.
                     Impressao em Landscape ou Portrait.
*/

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QPrinter>
#include <QPainter>
#include <QPointF>
#include <QFont>
#include <QFontMetricsF>
#include <QRectF>
#include <QSizeF>
#include <QPen>
#include <QProgressDialog>
#include <QPrintPreviewDialog>
#include <QStringList>
#include <QString>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QVariant>

#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>

#include <QDebug>

/**
 * @brief The VAbstractPrint class
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 *
 *
 * %Changelog%
 *
 * 2013-05-13 - + Adicao do membro int rowsCount(), que retorna a quantidade
 *              de linhas geradas pela consulta efetuada.
 *              + Adicao do membro  qreal fitToPage(QImage image, QRectF r1)
 *              o qual dimensiona a imagem para caber dentro da area de
 *              impressao da pagina selecionada
 *              + Melhorias na funcao setQueryModel(QString) a fim dela
 *              retornar mais informacoes sobre o que ocorreu durante o processo
 *              da monstagem da consulta
 * 2013-05-10 - + Adicao dos membros QString subTitle() e QString title()
 *              retornam o subtitulo e titulo definido para relatorio
 *              definido pelo usuario respectivamente.
 *              + Adicao do sinal message(const QString &m) que envia
 *              mensagens de texto de uso geral para outros objetos
 *
 */
class VAbstractPrint : public QObject {
    Q_OBJECT

public:
    explicit VAbstractPrint(QObject *parent = nullptr);
    ~VAbstractPrint();

    virtual void printReport(QPrinter *printer) = 0;

    void preview(QPrinter::Orientation=QPrinter::Portrait);

    void setTitle(QString title=QString());
    void setSubTitle(QString subTitle=QString());
    void setOutputFilename(QString ofname=QString());

    QString title();
    QString subTitle();
    int rowsCount();

    // Passagem e recuperacao de dados passados pelo usuario
    void setData(QVariant data);
    QVariant data(void);

    void setQueryModel(QString query=QString());
    void setStringList(QStringList strList=QStringList(), QString query=QString());
    void setColumnHeaders(QStringList list=QStringList());

    qreal fitToPage(QImage image, QRectF r1);

    inline qreal getLeftMargin() { return(leftMargin); }
    inline qreal getRightMargin() { return(rightMargin); }
    inline qreal getTopMargin() { return(topMargin); }
    inline qreal getBottomMargin() { return(bottomMargin); }
  
    qreal pageHeight();  // altura maxima de impressao 
    qreal pageWidth();   // largura maxima de impressao

    qreal getFontRectHeight(QFontMetricsF fontMetrics, QString str);
    qreal getFontRectWidth(QFontMetricsF fontMetrics, QString str);

private:
   QPrintPreviewDialog *dialog;
   QStringList lstHeaders;

   qreal leftMargin;
   qreal rightMargin;
   qreal topMargin;
   qreal bottomMargin;

   QString repTitle;
   QString repSubTitle;
   QString qsColHeader;

   QString subSqlQuery;
   QStringList dataStringList;

protected:
    QWidget *vpWidget;
    QPrinter *vpPrinter;
    QPainter vpPainter;
    QSqlQueryModel vpSqlModel;

    // variaveis usadas para processar queries individuais
    QStringList vpStrList; // Lista contendo valores para pesquisa de registros
    QString vpStrQuery;    // Decl SELECT para ser usada na consulta do registros

    qreal repHeader(int npag=0, bool plotHeaders=false);
    void columnHeaders();

    int __rowsCount;

private slots:
    void printSlot(QPrinter *printer);

signals:
    void lastError(const QString &e); // emite mensagens de erro sobre o sqlquery
    void message(const QString &m); // emite mensagens sobre os processos


};

#endif
