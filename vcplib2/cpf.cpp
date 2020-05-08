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

#include "cpf.h"

CPF::CPF(const QString cpf) :
    _cpf(cpf.simplified()),
    _dv(0),
    _valid(false)
{
    normalize_P();
}


// operator overload
/**
 * @brief CPF::operator =
 *        Atribuicao de valores entre tipos CPF
 * @param cpf2
 * @return
 */
CPF CPF::operator=(CPF cpf2)
{
    if(this == &cpf2)
        return *this;

    if(_valid)
        _cpf = cpf2._cpf;

    return *this;
}

/**
 * @brief CPF::operator ==
 *        Esta sobrecarga compara byte a byte entre
 *        dois CPF.
 *        Se qualquer um dos bytes forem diferentes
 *        interrompe o processamento e retorna false
 * @param cpf2
 * @return true | false
 *
 * %Changelog%
 * 2014 May 03 Volnei <vcputtini@gmail.com>
 * + Substituido for(int i=0; i<14; i++) por
 *   for(int i=0; i<temp.size(); i++).
 *   A quantidade de bytes do cpf devera ser
 *   sempre 11, porem para evitar um loop
 *   out-of-range a quantidade de bytes e
 *   calculada em tempo real.
 */
bool CPF::operator==(const CPF &cpf2) const
{
    if(_valid) {
        bool rc=true;
        QString temp = _cpf;
        for(int i=0; i<temp.size(); i++) {
            if(temp[i] != cpf2._cpf[i]) {
                rc = false;
                break;
            }
        }
        return rc;
    }
    return false;
}

/**
 * @brief CPF::operator !=
 *        Oferece um modo de comparacao adicional, alem
 *        do igual (==)
 * @param cpf2
 * @return true | false
 */
bool CPF::operator!=(const CPF &cpf2) const {
   return !(*this == cpf2);
}
// end - operator overload

/**
 * @brief CPF::section
 *        Retorna as partes significativas do CPF
 *        via uma estrutura.
 *
 *        Membros:
 *        first = Numero do Registro
 *        dv   = Valor do digito verificador
 *
 * @return struct Section
 */
CPF::Section CPF::section()
{
    Section sec;
    if(_valid) {
        int len = _cpf.length()-2;
        sec.first = _cpf.left(len);
        sec.dv    = _cpf.right(2);
    } else {
        sec.first = "";
        sec.dv = "";
    }
    return sec;
}


/**
 * @brief CPF::normalize_P
 *        Efetua diversas checagens com o valor inforamado
 *        a fim se assegurar que o valor e valido para
 *        ser calculado. De maneira geral tenta se certificar
 *        o valor, e esta estando correto, o converte para o
 *        formato interno para ser calculado.
 */

void CPF::normalize_P()
{
    if(_cpf.isEmpty()) {
        _valid = false;
        _dv = -1;
        return;
    }

    if(_cpf.length() < 11) {
        _valid = false;
        _dv = -1;
        return;
    }
    /* testa se o cpf passado e composto por
     * numeros na faixa de 00000000000 ate 99999999999
     */
    for(int i=0; i<_cpf.length(); i++) {
        if(_cpf.count(QString::number(i))==_cpf.length() ) {
            _valid = false;
            _cpf = "CPF::invalid";
            return;
        }
    }

    if(_cpf.length() < 9 || _cpf.length() > 14) {
        _valid = false;
        _cpf = "CPF::invalid";
        return;
    }

    /* Verifica se foi passado formatado ou nao */
    if(_cpf.contains(".") && _cpf.contains("-")) {
        if(_cpf.count(".")==2 && _cpf.count("-")==1) { // right
            _valid = true;
            _cpf.replace(".","").replace("-","");
        } else { // wrong
            _valid = false;
            _cpf = "CPF::invalid";
        }
    }

    _valid = true;
}

int CPF::calcDV()
{
    if(_valid) {
        int len = _cpf.length();
        QString aux = _cpf.left(len-2);
        QString dv = _cpf.right(2);

        int weight1[10] = { 10,9,8,7,6,5,4,3,2 };
        int weight2[11] = { 11,10,9,8,7,6,5,4,3,2 };

        /* DV 1 */
        int p[10];
        int sum=0;
        for(int i=0; i<10; i++) {
            p[i] = aux.mid(i,1).toInt() * weight1[i];
            sum += p[i];
        }
        int mod1 = sum%11;
        int dig1=0;
        if(mod1 < 2)
            dig1 = 0;
        else
            dig1 = 11-mod1;


        /* DV 2 */
        aux+=QString::number(dig1); // add DV 1
        sum = 0;
        int p1[11]; // Resolve bug de loop infinito
        for(int i=0; i<11; i++) {
            p1[i] = aux.mid(i,1).toInt() * weight2[i];
            sum += p1[i];
        }
        int mod2 = sum%11;
        int dig2=0;
        if(mod2 < 2)
            dig2 = 0;
        else
            dig2 = 11-mod2;

        return QString("%0%1").arg(dig1).arg(dig2).toInt();
    }
    return -1;
}

/**
 * @brief CPF::isValid
 *        Compara o DV informado pelo usuario com o
 *        calculado se forem diferentes retorna false
 *        representando que o CPF provavelmente e
 *        falso.
 * @return true | false
 */

bool CPF::isValid()
{
    if(_valid) {
        int _dv = _cpf.right(2).toInt();
        if(_dv != calcDV())
            return false;
        return true;
    }
    return _valid;
}
/**
 * @brief CPF::DV
 * @return inteiro representando DV
 */

int CPF::DV()
{
    if(_valid) {
        return calcDV();
    }

    return -1;
}

/**
 * @brief CPF::data
 *        Retorna o valor do CPF passado via o construtor do tipo.
 *        Permite que o usuario escolha entre dois formatos
 *        de apresentacao
 * @param format
 * @return QString(CPF)
 */

QString CPF::data(CPFFormats format)
{
    if(_valid) {
        stDelimiter stDel[][2] {
            {{3, "."}},
            {{7, "."}},
            {{11, "-"}}
        };
        switch(format) {
        case NumericOnly:
            if(_valid) {
                if(_cpf.count(".")==2 && _cpf.count("-")==1) {
                    _cpf.replace(".","").replace("-","");
                }
            }
            break;
        case Delimited:
            if(_cpf.length()==11) {
                for(int i=0; i<3; i++) {
                   _cpf.insert(stDel[i][0].pos,stDel[i][0].delemiter);
                }
            }
            break;
        }
    } // _valid

    return _cpf;
}
