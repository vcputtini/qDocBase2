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

#include <QApplication>
#include "qdocbase.h"

#include <QtSql>

#include <QSettings>
#include <QLocale>

#include "vcplib/vcplib2.h"
#include <utilities.h>

/**
 * Activate the test mode.
 * Uses the MariaDB/MySQL default 'test' database.
 * For that it's necessary that you create the tables inside db.
 */

//#define TEST

class QSettings;
class QLocale;

QSqlDatabase db;
QSqlError    sqlError;

using namespace NS_UTILITIES;

int main(int argc, char *argv[])
{
      Q_INIT_RESOURCE(application);
      QApplication app(argc, argv);

      VCPLib2 lib2;

      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

      QSettings settings(lib2.organization(), NS_UTILITIES::SettingsAppName);

      db = QSqlDatabase::addDatabase("QMARIADB");
      db.setHostName( settings.value("hostname").toString() ); // see source: configuracao.cpp

#ifdef TEST
      db.setDatabaseName("test");
#else
      db.setDatabaseName("DOCBASE");
#endif
      db.setUserName(settings.value("dbuser").toString()); // see source: configuracao.cpp
      db.setPassword(settings.value("dbpass").toString()); // see source: configuracao.cpp

      bool erro=false;
      qDocBase * mw = new qDocBase();
      if(!db.open() ) {
         if(db.lastError().nativeErrorCode() == 2003) {
            QMessageBox::critical(mw,"qDocBase2",
                 QObject::tr("Você deve configurar os parâmetros de conexão com o banco de dados."
                             " Feito isto reinicie o aplicativo.\n\n")+db.lastError().text(),
                 QMessageBox::Close);
            QApplication::restoreOverrideCursor();
            erro=false;
         } else {
             QMessageBox::critical(mw,QString("%0").arg(PROGRAM_NAME),
                         db.lastError().text(),
                          QMessageBox::Close);
            QApplication::restoreOverrideCursor();
            erro=true;
         }
      }

      QApplication::restoreOverrideCursor();

      if(erro)
          return -1;

      QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));


#ifdef TEST
      mw->setWindowTitle( mw->windowTitle()+QObject::tr(" [TEST MODE ONLY]") );
#endif
      mw->showMaximized();
      return app.exec();
}

