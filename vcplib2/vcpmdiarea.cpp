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

#include "vcpmdiarea.h"

// ***************************************************************************
// vcpMdiarea -- Cria e mantem uma interface MDI padrao para aplicacoes com
//               Logo grafico
// ***************************************************************************
vcpMdiArea::vcpMdiArea(QWidget *parent, QString programName)
    : QMdiArea(parent), progName(programName)
{
       setBackground(Qt::white);
}

void vcpMdiArea::paintEvent( QPaintEvent * event)
{
   QMdiArea::paintEvent(event);
   QPainter painter(viewport());

   painter.setPen(QColor(230,230,230)); // Cinza 10%
   QFont arialLogo("Arial", 65, QFont::Bold);
   QFontMetricsF fmL(arialLogo);
   painter.setFont(arialLogo);
   int x = (geometry().width() - fmL.width(progName) ) /2;
   int y = (geometry().height() - fmL.height() ) /2;
   painter.drawText(x,y,progName);

   QImage vcp(":/logo-vcp.jpg");
   int x0 = (geometry().width() - vcp.width() );
   painter.save();
   painter.translate(x0,10);
   painter.drawImage(0,0,vcp);
   painter.restore();
}
