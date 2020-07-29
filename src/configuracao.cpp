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

#include "configuracao.h"

Configuracao::Configuracao()
{
   readSettings();
}


Configuracao::~Configuracao()
{
}

void Configuracao::readSettings()
{
#ifdef Q_OS_WIN32
    dfltTmpDir = "c:/temp";
#else
    dfltTmpDir = "/tmp";
#endif
      QSettings settings(lib2.organization(), NS_UTILITIES::SettingsAppName);
      rootDir = settings.value("rootdir","/").toString();
      exportDir = settings.value("exportdir",dfltTmpDir).toString();
      tempDir = settings.value("tempdir",dfltTmpDir).toString();

      livre = settings.value("livre",true).toInt();
      readOnly = settings.value("readonly",false).toInt();
      roSave = settings.value("rosave",false).toInt();

      fiscal = (settings.value("fiscal",false).toInt() ? true : false) ;

      identCadDflt =  settings.value("idcaddflt").toString();
      identDefault =  settings.value("iddefault").toString();

      updtAuto = (settings.value("updtAuto").toInt() ? true : false) ;
      dirImgDflt = (settings.value("setDirImgDflt").toInt() ? true : false) ;

      dirDownload = settings.value("dirDownload").toString();

      bRemoveImageFromDir = (settings.value("removeImageFromDir").toInt() ? true : false);

      bKeepEntity = (settings.value("keepEntity").toInt() ? true : false);
}


QString Configuracao::CFldName(const qint8 f)
{
  QSettings settings(lib2.organization(), NS_UTILITIES::SettingsAppName);
  return( settings.value(QString("field0%0Name").arg(f)).toString() );
}

int Configuracao::CFldLength(const qint8 f)
{
  QSettings settings(lib2.organization(), NS_UTILITIES::SettingsAppName);
  return( settings.value(QString("field0%0Len").arg(f)).toInt() );
}

bool Configuracao::CFldActive(const qint8 f)
{
  QSettings settings(lib2.organization(), NS_UTILITIES::SettingsAppName);
  if(settings.value(QString("field0%0Act").arg(f)).toInt() == 1)
     return(true);

  return(false);
}

QStringList Configuracao::fillList()
{
    QStringList list;
    QSettings settings(lib2.organization(), NS_UTILITIES::SettingsAppName);

    for(int i=0; i<6; ++i) {
        if(settings.value(QString("field0%0Act").arg(i)).toBool())
            list << settings.value(QString("field0%0Name").arg(i)).toString();
    }
    if(list.count() > 0)
        return list;

    return QStringList();
}


