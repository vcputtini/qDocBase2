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

#include "dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    graphicsViewAbout->resetTransform();

    QFont fontArial15("Arial", 15, QFont::Bold);
    QFont fontArial12("Arial", 12, QFont::Normal);
    QFont fontArial10("Arial", 10, QFont::Normal);
    QFontMetricsF fmArial10(fontArial10);
    QFontMetricsF fmArial12(fontArial12);
    QFontMetricsF fmArial15(fontArial15);

    scene = new QGraphicsScene;
    scene->setSceneRect(-290, -175, 580, 350);
    graphicsViewAbout->setScene(scene);
    scene->clear();

    pixItemLogo = new QGraphicsPixmapItem;

    QColor cor;
    cor.setCmyk(60,40,0,40);
    scene->setBackgroundBrush(QBrush(cor));

    QImage img;
    QPixmap pixTmp;

    QImageReader reader(":/logo-docbase.png");
    reader.read(&img);
    pixTmp.convertFromImage(img);

    QRectF rect = scene->sceneRect();

    QTransform tf;
    tf.translate(-290,-175); // move (0,0) para o canto superior esquerdo

    qreal posH;
    QString sText;

    posH = (rect.width()-pixTmp.width())/2;
    pixItemLogo->setTransform(tf);
    pixItemLogo->setPixmap(pixTmp);
    pixItemLogo->setPos( posH, pixTmp.height()/4 );
    scene->addItem(pixItemLogo);


    qreal linha = pixTmp.height()+20;
    textItem = new QGraphicsTextItem;
    sText = tr("qDocBase2: Document Database Manager");
    textItem->setTransform(tf);
    textItem->setFont(fontArial15);
    textItem->setDefaultTextColor(Qt::white);
    posH = centerLine(rect, fmArial15, sText);
    textItem->setPos(posH, linha);
    textItem->setPlainText(sText);
    scene->addItem(textItem);


    QGraphicsTextItem *text[10];
    text[0] = new QGraphicsTextItem;
    sText = tr("Copyrigh(C)'2007-%0, Volnei Cervi Puttini <vcputtini@gmail.com.br>").arg(QDate::currentDate().year());
    text[0]->setTransform(tf);
    text[0]->setFont(fontArial10);
    text[0]->setDefaultTextColor(Qt::black);
    posH = centerLine(rect, fmArial10, sText);
    text[0]->setPos(posH, linha+=fmArial15.lineSpacing());
    text[0]->setPlainText(sText);
    scene->addItem(text[0]);

    text[1] = new QGraphicsTextItem;
    sText = tr("All Rights Reserved");
    text[1]->setTransform(tf);
    text[1]->setFont(fontArial10);
    text[1]->setDefaultTextColor(Qt::black);
    posH = centerLine(rect, fmArial10, sText);
    text[1]->setPos(posH, linha+=fmArial15.lineSpacing());
    text[1]->setPlainText(sText);
    scene->addItem(text[1]);


    text[2] = new QGraphicsTextItem;
    sText = tr("Version: %0").arg(VERSION_NUMBER);   // From Git. Definido no arquivo qdocbase2.pro
    text[2]->setTransform(tf);
    text[2]->setFont(fontArial10);
    text[2]->setDefaultTextColor(Qt::black);
    posH = centerLine(rect, fmArial10, sText);
    text[2]->setPos(posH, linha+=fmArial10.lineSpacing());
    text[2]->setPlainText(sText);
    scene->addItem(text[2]);

    QString verGCC;
#ifdef Q_OS_LINUX
    verGCC = tr("Compiler: GNU GCC %1.%2.%3").arg(__GNUC__).
             arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
#else
    verGCC = tr("Compiler: MINGW GCC %1.%2.%3").arg(__GNUC__).
             arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
#endif


    QStringList lText;
    lText << verGCC
          << QSysInfo::prettyProductName() << ""
          << tr("Use: MySQL(tm)/MariaDB(tm)")
          << "" << "" << ""
          << tr("The program is provided AS IS whith NO WARRANTY OF ANY KIND")
          << tr("INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND")
          << tr("FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.");

    int i=2;
    linha+=fmArial10.lineSpacing()*2;

    // C++11 compliant
    for(const QString &s : lText) {
        text[i] = new QGraphicsTextItem;
        text[i]->setTransform(tf);
        text[i]->setFont(fontArial10);
        text[i]->setDefaultTextColor(Qt::black);
        posH = centerLine(rect, fmArial10, s);
        text[i]->setPos(posH, linha+=fmArial10.lineSpacing());
        text[i]->setPlainText(s);
        scene->addItem(text[i]);
        ++i;
    }

    graphicsViewAbout->setScene(scene);
}

qreal DialogAbout::centerLine(QRectF r, QFontMetricsF fm, const QString s)
{
    return (r.width()-fm.width(s))/2;
}
