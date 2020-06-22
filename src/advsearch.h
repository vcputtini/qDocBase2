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

#ifndef AdvSearch_H
#define AdvSearch_H

#include "ui_formadvsearch.h"

#include <QObject>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>

#include <QtSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QModelIndex>

#include <QMessageBox>
#include <QByteArray>
#include <QProgressDialog>
#include <QPushButton>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QGroupBox>

<<<<<<< HEAD

#include "vcplib/vcplib2.h"
=======
<<<<<<< HEAD
#include "vcplib/vcplib2.h"
=======
#include "../vcplib2/vcplib2.h"
>>>>>>> f3834ea18c73921b0cb662a98523ccf016904561
>>>>>>> dab3220d751d9d8e6a4c0956d8e51c857fc24141

#include "utilities.h"
#include "configuracao.h"
#include "department.h"
#include "vabstractprint.h"
#include "advsearchtableviewitemdelegate.h"
#include "typeimage.h"
#include "qvpmultifiles.h"
#include "temporaryfiles.h"
#include "printselectedimages2.h"

#include <QDebug>

extern QSqlDatabase	db;
extern QSqlError	sqlError;

using namespace  NS_UTILITIES;


/**
 * @brief The ItemInterceptDoubleClick class
 *        Esta subclasse e usadas para interceptar o duploclick
 *        do mouse pelo usuario sobre a imagem selecionada
 *        e abri-la com o aplicativo correspondente via o
 *        sistema operacional.
 *        Exemplo: um arquivo .xls sera aberto usando o Excel
 */
class ItemInterceptDoubleClick :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:

    void setProperty(const qint64 ident, const QString fileName);

private:
    QString fname;
    qint64 id;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void internalError(QString);

signals:
    void openUrlError(QString);
    void fileError(QString);

};


class AdvSearch : public QWidget, private Ui::AdvSearch
{
    Q_OBJECT
    
public:
    explicit AdvSearch(QWidget *parent = nullptr);

private:
    QSqlQueryModel *vModel;
    QGraphicsScene *scene;

    TemporaryFiles *tempFiles;

    ItemInterceptDoubleClick *gItem;

    enum Headers { hID, hDtcadast, hOperador, hDpto,
                   hRelacao, hDtDoc, hNumDoc, hValDoc,
                   hAnexo };

    const QString SORT_ASC = "A";
    const QString SORT_DESC = "D";

    enum SortFields {iID, iENTITY, iCLASSE, iDPTO, iDTCADAST,
               iDTDOC, iVALDOC };

    const char *pID = "#ID";
    const char *pENTITY = "Nome da Entidade";
    const char *pCLASSE = "Classe da Entidade";
    const char *pDPTO = "Departamento";
    const char *pDTCADAST = "Data do Cadastro";
    const char *pDTDOC = "Datado do Documento";
    const char *pVALDOC = "Valor do Documento";


    QString sTypeEntity;
    QString sNameEntity;
    int iTypeEntity;
    bool __IDENTIFY__;
    bool __ISVIEWABLE__;

    void fillComboBoxSort();

public slots:
    void executeSearch();
    void setDefaults();
    void nameEntity(QModelIndex);

    void zoomIn();
    void zoomOut();
    void imageRotate();

    void documentExport();
    void printImages();

    virtual void checkBoxSortType_toggled(bool);
    virtual void checkBoxDateType_toggled(bool);

    virtual void tableViewBusca_clicked(QModelIndex);
    virtual void entityIdentify_clicked(bool b);

    void urlError(QString e);

    void printMessage(const QString &m);
    void printLastError(const QString &e);


    void dynamicGroupBox_toogle(bool b);

signals:
    void closed(bool);
    void hide(DockType);
    void activeTools(bool);
    void showStatusBarMessage(const QString &m);

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // AdvSearch_H
