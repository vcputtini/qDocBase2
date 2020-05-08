/***************************************************************************
 * Copyright (c) 2007-2019                                                 *
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

#ifndef VCPDOUBLEVALIDATOR_H
#define VCPDOUBLEVALIDATOR_H

#include "vcplib2.h"

#include <QObject>
#include <QDoubleValidator>
#include <QString>

#include <limits.h>
#include <math.h>


/**
  * @brief Para uso na classe vcpDoubleValidator
  *
  */
#ifndef LLONG_MAX
#   define LLONG_MAX Q_INT64_C(0x7fffffffffffffff)
#endif

using namespace std;


/**
  * @class vcpDoubleValidator
  * @author Volnei Cervi Puttini
  * @date 2011-02-17
  * @update 2013-05-23
  * @brief Valida corretamente a digitacao de numero com ponto decimal
  *
  * Faz com que os parametros top, bottol e decimals sejam rigorosamente
  * respeitados durante a digitacao do numero decimal.
  * Ou seja, se um numero for definido como por exemplo: bottom=0.0 top=9999.99 decimals=2
  * ira permitir a digitacao do numero apenas dentro dos limites estabelecidos.
  * no formato 9999.99 como dado neste exemplo.
  *
  * %ChangeLog%
  * 2013-05-23 Atualizacao para versao Qt5
  *
  */

class VCPLIB2SHARED_EXPORT vcpDoubleValidator : public QDoubleValidator
{
    Q_OBJECT
public:
    vcpDoubleValidator( double bottom, double top, int decimals, QObject* parent = nullptr );

    virtual QValidator::State validate(QString &input, int &pos) const;

private:

  /* Funcoes auxiliares */
  enum NumberMode { IntegerMode, DoubleStandardMode, DoubleScientificMode };
  bool validateChars(const QString &str, NumberMode numMode, QByteArray *buff,
                                      int decDigits) const;

  inline int numDigits(qlonglong n) const;
  inline char digitToCLocale(const QChar &in) const;

  qlonglong pow10(int n) const;

signals:

public slots:

};


#endif // VCPDOUBLEVALIDATOR_H
