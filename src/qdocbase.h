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

#ifndef QDOCBASE_H
#define QDOCBASE_H

#include <QMainWindow>
#include "ui_formDocBase.h"

#include <QShowEvent>
#include <QMdiSubWindow>
#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QToolBar>
#include <QToolButton>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QIcon>

// Windows Extras ---------------
#ifdef Q_OS_WIN
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif
// -------------------------------

#include <QDebug>

#include "vcplib/vcplib2.h"
#include "vcplib/vcpmdiarea.h"

#include "utilities.h"
#include "tventity.h"
#include "tvfinddocs.h"


// Modulos do sistema
#include "recordentity.h"
#include "recorddocs.h"
#include "advsearch.h"
#include "dialogconfig.h"
#include "topsecret.h"

// Janelas secondarias
#include "dialogabout.h"
#include "configuracao.h"



extern QSqlDatabase db;
extern QSqlError    sqlError;

using namespace NS_UTILITIES;

/**
 * @brief The qDocBase class
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 */
class qDocBase : public QMainWindow, private Ui::MWDocBase
{
  Q_OBJECT

public:
  qDocBase(QWidget* parent = nullptr, Qt::WindowFlags fl = nullptr );
  ~qDocBase();
  /*$PUBLIC_FUNCTIONS$*/ 

private:
  vcpMdiArea* mdiArea;

  // Actions for menus
  QAction *actionAbout;
  QAction *actionAboutQt;
  QAction *actionSair;
  QAction *actionModulos;
  QAction *actionEntidades;
  QAction *actionFind;

  QAction *actionPrintOneImage;
  QAction *actionPrintImages;
  QAction *actionPrintListDocs;
  QAction *actionPrintListValDocs;

  QAction *actionPrintActiveEntities;
  QAction *actionPrintInactiveEntities;

  // Ferramentas
  QAction *actionConfig;
  QAction *actionSecur;

  // Opcoes cadastro de documentos
  QToolBar *toolBarCadastro;
  QToolButton *toolButtonSave;
  QToolButton *toolButtonUpdate;
  QToolButton *toolButtonClear;
  QToolButton *toolButtonDelete;

  QToolButton *toolButtonZoomIn;
  QToolButton *toolButtonZoomOut;
  QToolButton *toolButtonRotate;

  QToolButton *toolButtonExport;
  QToolButton *toolButtonPrint;

  QToolButton *toolButtonPrintMenu;

  // Opcoes Busca Avancada
  QToolBar *toolBarBuscaAvancada;
  QToolButton *toolButtonExec;


  // Popup menu
  QMenu *popupPrintMenu;

  // Objectos para toolbar
  QComboBox *comboBoxEntities;
  QDateEdit *dateEditBegin;
  QDateEdit *dateEditEnd;
  QCheckBox *checkBoxDataTipo;
  const QString checkBoxDataTipoTitleChecked = tr("Data do Cadastro");
  const QString checkBoxDataTipoTitleUnchecked = tr("Data do Documento");

  // Functions
  void createMenus();
  void createActions();
  void createToolBarRegistroDocumentos(const QString title, ModulesOfApp modulo);
  void createToolBarBuscaAvancada();

  void createMenusPrint(ModulesOfApp modulo);
  void createActionsPrint(ModulesOfApp modulo);

  // Janelas
  RecordEntity *recEntity;
  RecordDocs *recDocs;
  AdvSearch *buscaAvancada;

  DialogConfig *dlgConfig;
  TopSecret *topSecret;
  DialogAbout *dlgAbout;


  // Diversos
  void semPermissao() {
         QMessageBox::warning(this , QString("qDocBase2"),
              QString("Você não possui permissão para acessar este módulo."),
                              QMessageBox::Close);
  }

#ifdef Q_OS_WIN
  // Windows Extras
  // Botao e progreesbar na barra de tarefas
  void showEvent(QShowEvent *);
  QWinTaskbarButton *taskButton;
  QWinTaskbarProgress *taskProgress;
#endif

public slots:
  /*$PUBLIC_SLOTS$*/
  void actionAbout_triggered();
  void actionAboutQt_triggered();
  void actionSair_triggered();
  void actionConfig_triggered();
  void actionSecur_triggered();

  void setCheckBoxText(const QString);
  void checkBoxDataTipo_toggled(bool);


  void lineEditSearchFormat(int);

  virtual void lvModulos_doubleClicked(QModelIndex);
  virtual void tvEntity_doubleClicked(QModelIndex);
  virtual void dockWidgetEntity_visibilityChanged(bool);

  void hideDocks(DockType);

  void deleteMenu(ModulesOfApp modulo);

  void showStatusBarMessage(const QString &m);

signals:
    void rangeYears(const int&, const int&);

protected:
  /*$PROTECTED_FUNCTIONS$*/
  bool isWindowsActived();
  void closeEvent(QCloseEvent *event);

#ifdef Q_OS_LINUX
  void showEvent(QShowEvent *e);
#endif

protected slots:
  /*$PROTECTED_SLOTS$*/


};

#endif

