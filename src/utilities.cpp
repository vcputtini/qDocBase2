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

#include "utilities.h"

namespace NS_UTILITIES {


/**
 * @brief uZoomImage
 *        Amplia ou reduz a escala da imagem.
 *        Usa o valor da escala horizontal (m11)
 *        para determinar os valores minimos e maximos
 * @param zi - Se e para ampliar ou reduzir ZoomImage::zIn | zOut
 * @param gv - Objeto que contem a imagem
 */
void uZoomImage(ZoomImage zi, QGraphicsView *gv)
{
    switch (zi) {
    case zOut:
        if( gv->transform().m11() < 2.15)
            gv->scale(1.2,1.2);
        else {
            gv->resetTransform();
            gv->scale(.5,.5);
        }
        break;
    case zIn:
        if( gv->transform().m11() > 0.02) {
            gv->scale(.5,.5);
        } else {
            gv->resetTransform();
            gv->scale(.2,.2);
        }
        break;
    default:
        Q_UNREACHABLE();
    }
}

/**
 * @brief uRotate
 *        Gira a imagem no sentido horario
 * @param gv
 * @param degrees
 */
void uRotate(QGraphicsView *gv, const qreal degrees)
{
    if(degrees > 360.0)
       return;
    gv->rotate(degrees);
}

/**
 * @brief isExecutable
 *        Verifica se o arquivo e executavel ou nao
 *        arquivos compactados tambem sao considerados
 *        como executaveis
 *        Inicializacao da lista padrao c++11
 *
 * @param fileName
 * @return true | false
 */
bool isExecutable(const QString fileName)
{
    QFileInfo info(fileName);
    QStringList list = {
        "exe", "bat", "com", "msi", "inf", "sys"
        "bin", "dll", "ini", "zip", "7z", "rar"
        "arj", "dat", "rpm", "deb", "ocx"
    };
    return list.contains(info.suffix().toLower());
}

/**
 * @brief uLoadImages - Funcao Standalone
 * @param QString x - nome do arquivo
 * @return QPair
 */
QPair<QImage,QString> uLoadImages(const QString x)
{
   TypeImage ti;

   QImage img;
   QFileInfo fi(x);
   QSize sz;
   QString ext = fi.suffix().toLower();

   if(ti.isImageViewable(x)) {      
        QImageReader reader(x);
        reader.read(&img);
        if(img.size().height() < 100)
            sz = QSize(img.size().width(), 500);
        else
            sz = img.size();
   } else {
       img.load(ti.resourceName(ext));
       sz = QSize(64,64);
   }
   return( qMakePair(img.scaled(sz,Qt::KeepAspectRatio),ext) );

}

/**
 * @brief Converte um numero passado via size, para sua notacao
 *        IEC - comissao eletrotecnica internacional e tambem
 *        formatado com separacao de milhares
 *
 *
 * @param size - tamanho do arquivo em bytes por exemplo
 *
 * NOTE:Retorna os valores por referencia
 * @param s    - retorna ex: 1500 MiB
 * @param f    - retorna ex: 1.555.666
 */
void uToIEC(const qreal size, QString &s, QString &f)
{
    if(size <= 0) {
        s = "";
        f = "";
    }

    if(size <= KiB) {
        s = QString::number(size)+" B";
    } else if(size > KiB && size < MiB) {
        s = QString::number(size/KiB,'f',1)+" KiB";
    } else if(size >= MiB && size < GiB) {
        s = QString::number(size/MiB,'f',1)+" MiB";
    }  else if(size >= GiB && size < TiB) {
        s = QString::number(size/GiB,'f',1)+" GiB";
    } else if(size >= TiB && size < PiB) {
        s = QString::number(size/TiB,'f',1)+" TiB";
    } else
        s = QObject::tr("Fora de Faixa"); // Maior que pentabyte

    if(size >= PiB)
        f = QObject::tr("0");
    else
        f = QString("%L0").arg(size,15,'f',0);
}

/**
 * @brief Usado em conjunto com o sinal QLineEdit::editingFinished()
 *      verificando se o usuario digitou caracters nao permitidos
 * @param s
 * @return true | false
 */
bool isValidChar(const QString s)
{
    return (s.contains('#') ? false : true);
}

/**
 * @brief uFormatCodigoObra - Formata o codigo da obra e o ano
 * @param obra
 * @param ano
 * @return QString formato como "obra-ano" Ex: 00000-00
 */
QString uFormatCodigoObra(const int obra, const int ano)
{
    return QString("%0-%1")
            .arg(QString::number(obra).rightJustified(5,'0'))
            .arg(QString::number(ano).rightJustified(2,'0')) ;
}

/**
 * Verifica se a R.G e vazia ou nao. Faz isto
 * testando se a string passada nao e igual a mascara
 * @param s - RG formatada
 * @return - true|false
 */
bool uRGEmpty(const QString s)
{
    return( s.simplified() == MASK_RG ? true : false );
}

/**
 * Verifica se a I.E e vazia ou nao. Faz isto
 * testando se a string passada nao e igual a mascara
 * @param s - IE formatada
 * @return - true|false
 */
bool uIEEmpty(const QString s)
{
    return( s.simplified() == MASK_IE ? true : false );
}

/**
 * Verifica se o CNPJ e vazio ou nao. Faz isto
 * testando se a string passada nao e igual a mascara
 * @param s - CPNJ 
 * @return - true|false
 */
bool uCNPJEmpty(const QString s)
{
   return( s.simplified() == MASK_CNPJ ? true : false );
}

/**
 * Verifica se o CPF e vazio ou nao. Faz isto
 * testando se a string passada nao e igual a mascara
 * @param s - CPF 
 * @return - true|false
 */
bool uCPFEmpty(const QString s)
{
   return( s.simplified() == MASK_CPF ? true : false );
}

/**
 * @brief uCNPJExists
 * Pesquisa se o CNPJ ja existe no cadastro.
 *
 * @changelog
 * 05 Jan 2016 - Para evitar problemas com o posicionamento correto
 * do cursor na tabela, agora testamos se o q.exec() foi bem sucedido
 * antes de tentar pegar o valor o campo.
 * @param s
 * @return true|false
 */
bool uCNPJExists(const QString s)
{
  QString sql= QString("SELECT HIGH_PRIORITY ET_IDENT FROM DOCENTITY WHERE ET_CNPJ='%0'").arg(s.simplified());
  QSqlQuery q;
  q.prepare(sql);
  if (q.exec() ) {
      q.next();
      return(q.value(0).toInt() == 0 ? false : true);
  }
  return false;
}

/**
 * @brief uCPFExists
 * @param s
 * @return true|false
 */
bool uCPFExists(const QString s)
{
  QString sql= QString("SELECT HIGH_PRIORITY ET_IDENT FROM DOCENTITY WHERE ET_CPF='%0'").arg(s.simplified());
  QSqlQuery q;
  q.prepare(sql);
  if (q.exec() ) {
      q.next();
      return(q.value(0).toInt() == 0 ? false : true);
  }
  return false;
}

/**
 * @brief uOperatorIsValid
 *        Informando ambos os parametros Operador e Senha, pesquisa
 *        se o par informado esta correto.
 *        Se informar apenas o Operador confirma ou nao existencia
 *        do nome no cadastro.
 *
 * @note  A declaracao SQL usa o parametro BINARY na clausuala WHERE
 *        a fim de torna-la case-sensitive.
 *        Ex. 'VOLNEI' sera considereado diferente de 'Volnei'
 *
 *
 * @param Operador
 * @param Senha (opcional)
 * @return true | false
 */
bool uOperatorIsValid(const QString Operador, const QString Senha)
{
  if(Operador.isEmpty() || Operador == "admin")
     return(false);

  QString sql;
  if(Senha.isEmpty())
      sql ="SELECT OPERADOR FROM DOCSECURITY WHERE BINARY OPERADOR=:OPERADOR";
  else
      sql ="SELECT OPERADOR FROM DOCSECURITY WHERE BINARY OPERADOR=:OPERADOR AND SENHA=DES_ENCRYPT(:SENHA,'3vcp69')";

  QSqlQuery q;
  q.prepare(sql);
  q.bindValue(":OPERADOR",Operador.simplified());
  if(!Senha.isEmpty()) {
    q.bindValue(":SENHA",Senha.simplified());
  }

  if (q.exec()) {
    q.next();
    return(q.value(0).toString().isEmpty() ? false : true);
  }
  return false;
}


/**
 * Preenche um combobox dado com o nome e identificacao das entidades
 * no formato NOME#IDENTIFICACAO
 * @return nada
 */
void uFillCBEntidade(QComboBox* cb, TIPO_ENTIDADES tipo, int active)
{
  QString STMT;
  if(tipo == NS_UTILITIES::EnTodos)
      STMT=QString("SELECT ET_IDENT, ET_NOME FROM DOCENTITY WHERE ET_ATIVO=%0 ORDER BY ET_NOME").arg(active);
  else
      STMT=QString("SELECT ET_IDENT, ET_NOME FROM DOCENTITY WHERE ET_CATEG=%0 AND ET_ATIVO=%1 ORDER BY ET_NOME").arg(tipo).arg(active);

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  cb->clear();
  QSqlDatabase::database().transaction();
  QSqlQuery query;
  query.prepare(STMT);
  query.exec();
  if( query.lastError().isValid()) {
     QSqlDatabase::database().rollback();
     QApplication::restoreOverrideCursor();
     return;
  }
  cb->addItem("");
  while( Q_LIKELY(query.next()) )
  {
     cb->addItem( query.value(1).toString()+"#"+query.value(0).toString());
  }
  QSqlDatabase::database().commit();
  QApplication::restoreOverrideCursor();
}

/**
 * Extrai o codigo serial da obra da string NOME-ENTIDADE#CODIGO
 * @param s 
 * @return - Inteiro extraido da string dada
 */
quint64 uSerialEntidade(const QString s)
{   
    return (s.isEmpty() ? 0 : s.section('#',1,1).toULongLong());
}

/**
 * @brief uNameEntities
 * @param et - valor do tipo NS_UTILITIES::TIPO_ENTIDADES
 * @return nome da classe da entitide
 */
QString uNameEntities(const int et)
{
   if(et > uClassesOfEntities().count())
      return("*");

   return uClassesOfEntities().at(et);
}

/**
 * @brief uClassesOfEntities
 *        Retorna uma lista contendo os nomes das classes
 *        das entidades
 * @return lista
 */
QStringList uClassesOfEntities()
{
    return QStringList() << QObject::tr("Empresas") <<
                            QObject::tr("Clientes") <<
                            QObject::tr("Fornecedores") <<
                            QObject::tr("Projetos e Obras") <<
                            QObject::tr("Personalizados") <<
                            QObject::tr("Simples");
}


int uLastEntity()
{
  QSqlQuery q;
  q.prepare("SELECT MAX(ET_IDENT) FROM DOCENTITY");
  q.exec();
  q.next();
  return( q.value(0).toInt());
}

/**
 * @brief Esta subclasse implementa a funcao membro toStandarize()
 *        com a intensao de prover a facilidade de padronizar os
 *        textos digitados pelo usuario.
 *        Neste momento certifica que o texto digitado tenha essas
 *        caracteristicar minimas
 *
 *      - Somente um espaco entre as palavras
 *      - A primeira letra e colocalada em mauiscula
 *
 *      Implemeta isReplicate() com a finalidade de testar se ha
 *      muitos caracteres repetidos no campo. O que pode indicar
 *      que a informacao digitada nao e valida.
 *      Ex.: ///////////////////, XXXXXXXXXXXXXXXXXXX, etc..
 */

/**
 * @brief vcpString::vcpString
 */
vcpString::vcpString() :
    _str(QString())
{
}

/**
 * @brief vcpString::vcpString
 * @param old_str
 */
vcpString::vcpString(const QString &old_str) :
    _str(old_str.simplified())
{
}

/**
 * @brief vcpString::vcpString
 * @param old_str
 */
vcpString::vcpString(const char *old_str) :
    _str(fromUtf8(old_str).simplified())
{
}

/**
 * @brief vcpString::isReplicate
 *          Conta quantos caracteres repetidos exitem
 *          na string passada.
 *          Se a quantidade ultrapassa um certo percentual
 *          em funcao da quantidade total permitida de
 *          caracteres da string informa ao usuario
 *          que ha muitos caracteres repetidos. Podendo
 *          nao ser uma palavra ou frase validas.
 *          Testa na faixa de 0x20 ate 0xFF (255) UTF8
 *
 *          Se o caracter testado nao e imprimivel, ele
 *          e ignorado.
 *
 * @return true | false
 */
bool vcpString::isReplicate(const int fldSize, const qreal percentage) const
{
    if(_str.isEmpty() || fldSize==0)
        return false;

    bool result=false;

    for(int i=0x20; i<=0xff; i++) {
        qreal q = _str.count(QChar(i));
        if(QChar(i).isPrint()) { // somente imprimiveis
            if(q > 0) {
                qreal percent= q/fldSize;
                if(percent > percentage) { // muitas repeticoes
                    result=true;
                }
            }
        }
    }
    return result;
}

/**
 * @brief vcpString::toStandarize
 * @param controls
 * @return
 */
QString vcpString::toStandarize()
{
    if(_str.isEmpty())
        return QString("");

    QList<QString> words;
    QString aux;

    words = _str.split(QChar(' '), Qt::SkipEmptyParts, Qt::CaseInsensitive);

    QList<QString>::iterator it;
    for(it = words.begin(); it != words.end(); ++it) {
        if( (*it).length() == 1  ) {
                (*it).toLower();
        } else {
            for(int i=0; i<(*it).length(); ++i) {
                if( (*it).at(0).isLower() ) {
                    (*it)[0] = (*it).at(0).toUpper();
                } else {
                    (*it)[i] = (*it).at(i).toLower();
                }
         }
        }
        aux += (*it);
        aux += QChar(' ');
    }
    return aux.simplified();
}

/**
 * @brief vcpString::operator =
 * @param __str
 * @return
 */
vcpString &vcpString::operator=(vcpString __str)
{
    if(this == &__str)
        return *this;

    _str = __str._str;

    return *this;
}

/**
 * @brief vcpString::operator =
 * @param __str
 * @return
 */
vcpString &vcpString::operator=(const char *__str)
{
    _str = fromUtf8(__str);

    return *this;
}


} //END - NS_UTILITIES
