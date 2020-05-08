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

#ifndef NS_UTILITIES_H
#define NS_UTILITIES_H

/* STL */
#include <math.h>
#include <limits.h>

#include <QObject>
#include <QApplication>
#include <QString>
#include <QComboBox>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QPair>
#include <QImage>
#include <QFileInfo>
#include <QGraphicsView>
#include <QImageReader>

#include <QtSql>
#include <QtSql/QSqlQuery>

#include "../vcplib2/vcplib2.h"
#include "configuracao.h"
#include "typeimage.h"

extern QSqlDatabase db;
extern QSqlError    sqlError;

using namespace std;

/**
 * @brief namespace que contem funcoes de uso geral
 *        no sistema
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 */
namespace NS_UTILITIES {
        static const QString NAMEPROGRAM = "DocBase";

        enum TIPO_ENTIDADES { EnEmpresa, EnCliente, EnFornecedor,
                              EnProjObras, EnOutros, EnTipoSimples,
                              EnTodos };
        enum DockType { DockEntity, DockFindDocs, DockAdvSearch, DockModulos };

        /**
         * @brief The ModulesOfApp enum class
         *        Classe tipo enumerador padrao para
         *        identificacao dos modulos do sistema
         *        Padrao C++11
         */
        enum class ModulesOfApp : int
        {
            ModEntity,
            ModDocuments,
            ModSearch,
            ModConfig,
            ModSecurity
        };

        // NEVER MUST BE CHANGED
        static const QString SettingsAppName = "qDocBase2";


        static const qint64 KiB                 = pow(2,10);
        static const qint64 MiB                 = pow(2,20); 	// 1048576
        static const qint64 GiB                 = pow(2,30);
        static const qint64 TiB                 = pow(2,40);
        static const qint64 PiB                 = pow(2,50);
        static const qint64 MAX_SIZE_IMG_16MB	= 16777216;     // 16MiB
        static const qint64 MAX_SIZE_IMG_2GB	= 2147483648;	// 2MiB

        static const QString CUSTOM_NOTUSED = QObject::tr("(não usado)");

        static const QString MASK_CNPJ  = "../-";
        static const QString MASK_CPF   = "..-";
        static const QString MASK_CEP   = ".-";
        static const QString MASK_IE    = "...";
        static const QString MASK_RG    = "..-/-";

        /*
         * limitacao da quantidade de registros que poderao ser
         * gravados. Usado para fins de compilacao para
         * demonstracao
         */
        static const qint64 MAXREGISTERS    = 0;

        // cores
        static const QString LightCyan = "#A8DDE0";

        /**
         * @brief DefaultVCPColor
         * CMYK = 60 40 0 40
         * RGB  = 61 92 153 Alpha=250
         * HTML = #3d5c99
         */
        static const QColor DefaultVCPColor = QColor(61,62,153,250);

        int uLastEntity();
        quint64 uSerialEntidade(const QString s);
        void uFillCBEntidade(QComboBox* cb, TIPO_ENTIDADES tipo=EnTodos, int active=1);
        QString uFormatCodigoObra(const int obra, const int ano);

        QString uNameEntities(const int et);
        QStringList uClassesOfEntities();

        bool uRGEmpty(const QString s);
        bool uIEEmpty(const QString s);
        bool uCNPJEmpty(const QString s);
        bool uCNPJExists(const QString s);
        bool uCPFEmpty(const QString s);
        bool uCPFExists(const QString s);
        bool uOperatorIsValid(const QString Operador="", const QString Senha="");


        bool isExecutable(const QString fileName);
        bool isValidChar(const QString s);

        /**
         * @brief Converte um numero passado via size, para sua notacao
         *        IEC - comissao eletrotecnica internacional e tambem
         *        formatado com separacao de milhares
         * @param size - tamanho do arquivo em bytes por exemplo
         *
         * NOTE:Retorna os valores por referencia
         * @param s    - retorna ex: 1500 MiB
         * @param f    - retorna ex: 1.555.666
         */
        void uToIEC(const qreal size, QString &s, QString &f);

        /* Tratamento de Imagens */
        enum ZoomImage { zIn, zOut };
        QPair<QImage,QString> uLoadImages(const QString x);
        void uZoomImage(ZoomImage zi, QGraphicsView *gv);
        void uRotate(QGraphicsView *gv, const qreal degrees);


        /**
         * @brief The vcpString class
         * @brief Esta subclasse implementa a funcao membro toStandarize()
         *        com a intensao de prover a facilidade de padronizar os
         *        textos digitados pelo usuario.
         *        Neste momento certifica que o texto digitado tenha essas
         *        caracteristicar minimas
         *
         *      - Somente um espaco entre as palavras
         *      - A primeira letra e colocada em mauiscula
         */
        class vcpString : public QString
        {
            QString _str;
        public:
            explicit vcpString();
            explicit vcpString(const QString &old_str);
            explicit vcpString(const char *old_str);

        public:
            QString toStandarize();
            bool isReplicate(const int fldSize=0, const qreal percentage=0.40) const;

            vcpString &operator=(vcpString __str);
            vcpString &operator=(const char *__str);

        };
}

#endif
