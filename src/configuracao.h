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

#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#include <QString>
#include <QStringList>
#include <QSettings>

#include "vcplib/vcplib2.h"

#include "utilities.h"

/**
    @author Volnei Cervi Puttini <vcputtini@gmail.com>
*/

class Configuracao {
public:
    Configuracao();

    ~Configuracao();

    QString getRootDir() const { return(rootDir); }
    QString getExportDir() const  { return(exportDir); }
    QString getTempDir() const { return(tempDir); }

    bool removeImageFromDir() { return(bRemoveImageFromDir); }
    bool keepEntity() { return(bKeepEntity); }

    int getLivre() const { return(livre); }
    int getReadOnly() const { return(readOnly); }
    int getLimited() const { return(roSave); }

    bool fiscalSetting() const { return(fiscal); }
    QString identCadPadrao() const { return(identCadDflt); }
    QString identPadrao() const { return(identDefault); }

    bool updateAuto() const { return(updtAuto); }
    bool dirImageDefault() const { return(dirImgDflt); }
    
    QString dirDfltDownload() const { return(dirDownload); }

    /* campos customizados */
    QString CFldName(const qint8 f=0);
    int CFldLength(const qint8 f=0);
    bool CFldActive(const qint8 f=0);
    QStringList fillList();


private:
    VCPLib2 lib2;

    QString rootDir;
    QString exportDir;
    QString tempDir;

    int livre;      // uso liberado
    int readOnly;   // somente leitura
    int roSave;     // permite apenas inclusao, sem alteracao ou remocao de registros

    bool fiscal;
    QString identCadDflt;
    QString identDefault;
    QString dirDownload;

    QString dfltRootDir;
    QString dfltTmpDir;
    
    bool bRemoveImageFromDir;
    bool bKeepEntity;           // se true nao limpa label da entidade

    bool updtAuto;
    bool dirImgDflt;

protected:
   void readSettings();

};

#endif