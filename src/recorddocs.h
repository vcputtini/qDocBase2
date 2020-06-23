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

#ifndef RECORDDOCS_H
#define RECORDDOCS_H

#include "ui_formRecordDocs.h"

#include <QObject>
#include <QApplication>

#include <QLocale>

#include <QtSql>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>


#include <QString>
#include <QByteArray>
#include <QList>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QListView>
#include <QProgressDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMessageBox>
#include <QInputDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <QToolButton>
#include <QProgressDialog>
#include <QLineEdit>
#include <QCloseEvent>
#include <QFileDialog>

#include <QDir>
#include <QPair>
#include <QDesktopServices>

#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QImageReader>


#include <QtConcurrent/QtConcurrentRun>
#include <QtConcurrent/QtConcurrentMap>
#include <QFutureWatcher>
#include <QFuture>


#include "vcplib/vcplib2.h"

#include "utilities.h"
#include "configuracao.h"

#include "tventity.h"
#include "typeimage.h"
#include "qvpmultifiles.h"
#include "vabstractprint.h"

#include "department.h"
#include "sysactivity.h"

#include "printimage.h"
#include "printselectedimages.h"
#include "printdocgeneral.h"
#include "printdocanalitic.h"

extern QSqlDatabase	db;
extern QSqlError	sqlError;


using namespace NS_UTILITIES;

/**
 Prototipo da funcao standalone usada pela funcao load
 para carregar as imagens do disco usando QtConcurrent.
 Retorna a imagen ja na escala correta para ser usada.
*/
QPair<QImage,QString> loadImages(QString x);


/**
 * @brief The RecordDocs class
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 */
class RecordDocs : public QWidget, private Ui::RecordDocs
{
    Q_OBJECT
    
public:
    explicit RecordDocs(QWidget *parent = nullptr);

    QString filePicNameCompl;	// path()+nome com extensao
    QString filePicName;		// somente nome
    int filePicSize;		    // tamanho do arquivo

private:
    VCPLib2 lib2;
    Configuracao CFG;
    QLocale brazil;

    QLineEdit *lineEditDeptos;
    QLineEdit *lineEditFilters;

    QString sNameEntity;
    QString sTypeEntity;
    int iTypeEntity;
    bool __FIRSTRUN__;         // Primeira chamada deste objeto
    bool __IDENTIFY__;
    bool __EXPORTABLE__;
    bool __IMGSELECTED__;      // Nova imagem foi selecionada na listiew
    bool __ISVIEWABLE__;       // Se a imgem selecinada e um simples icone ou nao
    QDate dateBegin;
    QDate dateEnd;
    bool dateTipo;             // true se data do cadastro; false se data do valor R$
    QString senhaDesbloqueio;
    QString operatorsName;
    QString pathImages;


    QGraphicsScene *scene;
    QGraphicsScene *sceneP;

    TypeImage *typeImage;      // funcoes de tratamento da imagem

    // usado por criaListaImagens()
    typedef QFutureWatcher< QPair<QImage,QString> > imagesWatcher;
    QStandardItemModel *mdlThumbs;
    QImage image;
    QPainter paint;
    void criaListaImagens();

    quint64 lastPgID();
    qint64 totalRegs();

    bool isCPNJ_OR_CPF;

public slots:
    void mfError(const QString& m); // Usado por QvpMultiFiles

    // sinais enviados por Department
    void deptoLastError(const QString &e);

    // sinais enviados por VAbstractPrint
    void printMessage(const QString &m); // mensagens diversas
    void printLastError(const QString &e); // erros sql

    // usado na listview dockable
    void nameEntity(QModelIndex); // recebe o dados sobre a entidade escolhida pelo usuario
    void entityIdentify_clicked(bool);

    void identifyEntity(QModelIndex);

    // emitido pelo QInputDialog quando o botao OK e clicado
    void inputDialog_textValueSelected(const QString &);
    void inputDialog_operatorSelected(const QString &);

    void saveReg();
    void updateReg();
    void deleteReg();

    void clearReg();

    void zoomIn();
    void zoomOut();
    void imageRotate();

    void documentExport();

    void printOneImageReg();
    void printImagesReg();
    void printDocGeneral();
    void printDocAnalitic();

public slots:
    virtual void listViewFiles_clicked(QModelIndex);
    virtual void listViewFiles_doubleClicked(QModelIndex);
    virtual void listViewFiles_entered(QModelIndex);

    virtual void comboBoxFileFilters_currentTextChanged(QString);
    virtual void toolButtonFileFilters_clicked();
    virtual void toolButtonDir_clicked();

    virtual void toolButtonSave_clicked();
    virtual void toolButtonDel_clicked();

    virtual void radioButtonCheckID_toggled(bool);
    virtual void toolButtonCopyCNPJ_clicked(bool);


    /*
     * Trata os lineEdit´s quando o usuario mudar de campo
     */
    void dynamic_editFinished();

    // Recebe as datas inicial e final para pesquisa no relatorio
    // enviada pelos objetos dateEditBegins e dateEditEnd
    void dateEditBegin_dateChanged(QDate);
    void dateEditEnd_dateChanged(QDate);
    // Se deve pesquisar pela data de cadastro ou do valor R$
    void checkBoxDataTipo_toggled(bool);

signals:
    void closed(bool);
    void isExportable(bool);
    void hide(DockType);
    void deleteMenu(ModulesOfApp);

    void showStatusBarMessage(const QString &m);


protected:
  void closeEvent(QCloseEvent *event);
};

#endif // RECORDDOCS_H
