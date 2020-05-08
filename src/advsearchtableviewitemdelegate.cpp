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

#include "advsearchtableviewitemdelegate.h"

AdvSearchTableViewItemDelegate::AdvSearchTableViewItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void AdvSearchTableViewItemDelegate::updateEditorGeometry(QWidget *editorWidget, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
   editorWidget->setGeometry(option.rect);
}


void AdvSearchTableViewItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // alinha dados numericos a direita referentes a:
    // ID, numero do documento e valor do documento
    // Tambem colore o numdoc, valor doc e data doc de azul
    if(index.column() == hID) {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QStyleOptionViewItem myOption = option;
        //myOption.palette.setColor(QPalette::Text, Qt::blue);
        myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    } else if(index.column() == hNumDoc || index.column() == hValDoc) {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QStyleOptionViewItem myOption = option;
        myOption.palette.setColor(QPalette::Text, Qt::blue);
        myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    } else if(index.column() == hDtDoc) {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QStyleOptionViewItem myOption = option;
        myOption.palette.setColor(QPalette::Text, Qt::blue);
        drawDisplay(painter, myOption, myOption.rect, text);
        drawFocus(painter, myOption, myOption.rect);
    } else if(index.column() == hAnexo) {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        TypeImage ti;
        if( !ti.isImageViewable(text) ) {
            QStyleOptionViewItem myOption = option;
            myOption.palette.setColor(QPalette::Text, Qt::darkGray);
            drawDisplay(painter, myOption, myOption.rect, text);
            drawFocus(painter, myOption, myOption.rect);
        } else {
            QItemDelegate::paint(painter, option, index);
        }
    }
    else {
        QItemDelegate::paint(painter, option, index);
    }
}

