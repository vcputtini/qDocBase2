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

#include "vcpdoublevalidator.h"

vcpDoubleValidator::vcpDoubleValidator( double bottom, double top, int decimals, QObject* parent)
    : QDoubleValidator( bottom, top, decimals, parent)
{
}

QValidator::State vcpDoubleValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos)

    QString buff(input);

    if(buff.isEmpty())
        return QValidator::Acceptable;

    double t;
    double b;
    double d;


    QByteArray abuff;
    if(!validateChars(input,  DoubleStandardMode, &abuff, decimals())) {
        return Invalid;
    }

    t = buff.section('.',0,0).length();
    d = buff.section('.',1,2).length();


    if (bottom() >= 0 && buff.startsWith('-'))
        return Invalid;

    if (top() < 0 && buff.startsWith('+'))
        return Invalid;

    if(d > decimals())
        return QValidator::Invalid;

    if(buff.toDouble() < bottom() || buff.toDouble() > top())
        return QValidator::Invalid;

    /* Testa os limites do numero
       ex: se maximo e 999999999.99 permite digitar ate 999999999.00
    */
    double i = buff.toDouble();
    b = bottom();
    t = top();

    if (notation() == StandardNotation) {
        double max = qMax(qAbs(b), qAbs(t));
        if (max < LLONG_MAX) {
            double n = pow10(numDigits(qlonglong(max)));
            if (qAbs(i) > n)
                return Invalid;
        }
    }

    return QValidator::Intermediate;
}

/**
  * @bried Retorna o numero de digitos de um dado numero inteiro
  *
  */
int vcpDoubleValidator::numDigits(qlonglong n) const
{
    return floor(log10(abs(n != 0 ? n : 1))) + 1;
}

/**
  * @bried Adaptação do codigo original dos fontes da Nokia Qt Framework Qt 4.7.1
  *        a fim de suprir algumas necessidades especificas
  *        O codigo foi alterado para minhas necessidades particularidades
  */
bool vcpDoubleValidator::validateChars(const QString &str, NumberMode numMode, QByteArray *buff,
                                    int decDigits) const
{
    buff->clear();
    buff->reserve(str.length());

    const bool scientific = numMode == DoubleScientificMode;
    bool lastWasE = false;
    bool lastWasDigit = false;
    int decPointCnt = 0;
    bool dec = false;
    int decDigitCnt = 0;

    Q_UNUSED(lastWasDigit);

    for (int i = 0; i < str.length(); ++i) {
        char c = digitToCLocale(str.at(i));

        if (c >= '0' && c <= '9') {
            if (numMode != IntegerMode) {
                // If a double has too many digits after decpt, it shall be Invalid.
                if (dec && decDigits != -1 && decDigits < ++decDigitCnt)
                    return false;
            }
            lastWasDigit = true;
        } else {
            switch (c) {
                case ',': // nao faz nada. A virgula DEVE ter o mesmo comportamento do ponto decimal
                case '.':
                    if (numMode == IntegerMode) {
                        // If an integer has a decimal point, it shall be Invalid.
                        return false;
                    } else {
                        // If a double has more than one decimal point, it shall be Invalid.
                        if (++decPointCnt > 1)
                            return false;
#if 0
                        // If a double with no decimal digits has a decimal point, it shall be
                        // Invalid.
                        if (decDigits == 0)
                            return false;
#endif                  // On second thoughts, it shall be Valid.

                        dec = true;
                    }
                    break;

                case '+':
                case '-':
                    if (scientific) {
                        // If a scientific has a sign that's not at the beginning or after
                        // an 'e', it shall be Invalid.
                        if (i != 0 && !lastWasE)
                            return false;
                    } else {
                        // If a non-scientific has a sign that's not at the beginning,
                        // it shall be Invalid.
                        if (i != 0)
                            return false;
                    }
                    break;
                default:
                    // If it's not a valid digit, it shall be Invalid.
                    return false;
            }
            lastWasDigit = false;
        }

        lastWasE = c == 'e';
        if (c != ',')
            buff->append(c);
    }

    return true;
}

/**
  * @bried Adaptação do codigo original dos fontes da Nokia Qt Framework Qt 4.7.1
  *        a fim de suprir algumas necessidades especificas
  *        O codigo foi alterado para minhas necessidades particularidades
  */
char vcpDoubleValidator::digitToCLocale(const QChar &in) const
{
    const QChar _zero = '0';
    const QChar _group = ',';
    const ushort zeroUnicode = _zero.unicode();
    const ushort tenUnicode = zeroUnicode + 10;

    if (in.unicode() >= zeroUnicode && in.unicode() < tenUnicode)
        return '0' + in.unicode() - zeroUnicode;

    if (in.unicode() >= '0' && in.unicode() <= '9')
        return in.toLatin1();

    if (in == '+')
        return '+';

    if (in == '-')
        return '-';

    if (in == '.')
        return '.';

    if (in == ',')
        return ',';

    //if (in == exponential() || in == exponential().toUpper())
    //    return 'e';

    // In several languages group() is the char 0xA0, which looks like a space.
    // People use a regular space instead of it and complain it doesn't work.
    if (_group.unicode() == 0xA0 && in.unicode() == ' ')
        return ',';

    return 0;
}

/**
  * @bried Funcao para calculo de uma potencia de 10
  */
qlonglong vcpDoubleValidator::pow10(int exp) const
{
    qlonglong result = 1;
    for (int i = 0; i < exp; ++i)
        result *= 10;
    return result;
}
