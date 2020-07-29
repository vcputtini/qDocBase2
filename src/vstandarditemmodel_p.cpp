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

#include "vstandarditemmodel_p.h"

VStandardItemModel_P::VStandardItemModel_P(QObject *parent) :
    QStandardItemModel(parent),
    alterColor(false)
{
}

QVariant VStandardItemModel_P::data(const QModelIndex &index, int role) const
{
  QVariant value = QStandardItemModel::data(index, role);

  if(role ==  Qt::BackgroundRole) { // trata plano de fundo da linha
      // Se forem nomes das classes pinta de cinza claro
      if( uClassesOfEntities().contains( index.data().toString() ))
          return QVariant::fromValue(QColor(Qt::lightGray));

      // coloca as linhas pares e impares nas cores escolhidas pelo usuario
      if(alterColor) {
        if(index.row() % 2 == 1)
            return QVariant::fromValue(QColor(255,255,204)); // odd
        else
            return QVariant::fromValue(QColor(255,255,104) ); // even
      }
  }

  return value;
}

void VStandardItemModel_P::setAlternateColor(bool b)
{
    alterColor = b;
}

