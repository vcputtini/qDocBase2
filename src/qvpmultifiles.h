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

#ifndef QVPMULTIFILES_H
#define QVPMULTIFILES_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QString>
#include <QPair>

#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>

#include <QDebug>

class QFile;
class QFileInfo;
class QByteArray;

/**
 * @brief The QvpMultiFilesRead_p class
 *        Interface de uso interno da QvpMultiFiles
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 */
class QvpMultiFilesRead_p
{
public:
   QPair<int,QByteArray> read(const QString fname);

private:
   QByteArray ba;
};

/**
 * @brief The QvpMultiFilesWrite_p class
 *        Interface de uso interno da QvpMultiFiles
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 */
class QvpMultiFilesWrite_p
{
public:
   int write(QByteArray baOut, const QString fname);
};

/**
 * @brief The QvpMultiFiles class
 *        Permite a leitura e gravacao de arquivos de qualquer tipo
 *
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 */
class QvpMultiFiles : public QFile
{
   Q_OBJECT
public:
    QvpMultiFiles();
    ~QvpMultiFiles();

    void setInputFile(const QString fIn=QString());
    void setOutputFile(const QString fOut=QString());

    int read();
    int write(QByteArray baOut);
    void execute();

    void setData(QByteArray ba);
    QByteArray data();

    quint64 sizeIn() const;
    quint64 sizeOut() const;
    bool exists(QString fname=QString());

private:
   QFile fileIn;
   QFile fileOut;
   QByteArray baData;
   QString fInput;
   QString fOutput;

   bool fileError(int fe);

   QvpMultiFilesRead_p QvpMultiFilesRead_P;
   QvpMultiFilesWrite_p QvpMultiFilesWrite_P;
   /* ponteiro para as funcoes membro (callbacks) */
   QPair<int,QByteArray> (QvpMultiFilesRead_p::*pt2read)(QString);
   int (QvpMultiFilesWrite_p::*pt2write)(QByteArray, QString);

signals:
   void error(const QString&);
};

#endif
