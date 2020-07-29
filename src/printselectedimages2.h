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

#ifndef PRINTSELECTEDIMAGES2_H
#define PRINTSELECTEDIMAGES2_H

#include <QObject>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>

#include <QtSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>

#include <QMessageBox>
#include <QByteArray>
#include <QProgressDialog>
#include <QFont>
#include <QFontMetricsF>
#include <QRectF>

#include <QPainter>
#include <QImage>
#include <QPixmap>

#include <QPrinter>

#include "utilities.h"
#include "vabstractprint.h"

#include <QDebug>

extern QSqlDatabase	db;
extern QSqlError	sqlError;

using namespace  NS_UTILITIES;

/**
 * @brief The PrintSelectedImages2 class
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 *
 */
class PrintSelectedImages2 : public VAbstractPrint
{
    Q_OBJECT
public:
    //explicit PrintSelectedImages2(QObject *parent = nullptr);

    virtual void printReport(QPrinter *printer);

private:
    void header(QRectF r, const QString title=QString());

signals:
    
public slots:
    
};

#endif // PRINTSELECTEDIMAGES_H
