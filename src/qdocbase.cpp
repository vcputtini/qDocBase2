/***************************************************************************
 * Copyright (c) 2007-2019                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase project, a images/documents           *
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

#include "qdocbase.h"

qDocBase::qDocBase(QWidget* parent, Qt::WindowFlags fl)
: QMainWindow( parent, fl ), Ui::MWDocBase()
{
	setupUi(this);

    mdiArea = new vcpMdiArea(this, QString("%1").arg(PROGRAM_NAME));
    setCentralWidget(mdiArea);

#ifdef Q_OS_WIN
    // Instanciamento das taskButton e taskProgress
    taskButton = new QWinTaskbarButton;
    taskButton->setWindow(windowHandle());
    taskProgress = taskButton->progress();
#endif


    dockWidgetEntity->setVisible(false);
    dockWidgetFindDocs->setVisible(false);

    spinBoxAno->setSpecialValueText("Todos");
    comboBoxSearch->setCurrentIndex(0);

    spinBoxFindStartYear->setValue(QDate::currentDate().year());
    spinBoxFindEndYear->setValue(QDate::currentDate().year());

        /**
     * @brief
     *      Sempre que a janela for dock for chamada e estiver visivel
     *      atualiza a view com os dados das entidades para pesquisa
     */
    connect(dockWidgetFindDocs, SIGNAL(visibilityChanged(bool)), treeViewFind,
            SLOT(fillView(bool)) );
    connect(treeViewFind, SIGNAL(showMessage(QString)), statusBar(),
            SLOT(showMessage(QString)) );
    connect(spinBoxFindStartYear, SIGNAL(valueChanged(int)), treeViewFind,
            SLOT(valueStartYearChanged(int)) );
    connect(spinBoxFindEndYear, SIGNAL(valueChanged(int)), treeViewFind,
            SLOT(valueEndYearChanged(int)) );
    connect(toolButtonFind, SIGNAL(clicked(bool)), treeViewFind,
            SLOT(toolButtonFindClicked(bool)) );

    /**
     * @brief connect and emit
     *      Envia o valor inicial dos objetos spinBoxFindStartYear e
     *      spinBoxFindEndYear para o objeto treeViewFind.
     */
    connect(this, SIGNAL(rangeYears(const int &, const int &) ), treeViewFind,
            SLOT(rangeYears(const int &, const int &)) );
    emit rangeYears(spinBoxFindStartYear->value(),
                    spinBoxFindEndYear->value());



    /**
     * @brief
     *      Sempre que a janela for dock for chamada e estiver visivel
     *      atualiza a view com os dados das entidades
     */
    connect(dockWidgetEntity, SIGNAL(visibilityChanged(bool)), treeViewEntity,
            SLOT(update(bool)) );
    /**
     * @brief connect
     *
     * Permite a manipulacao das opcoes do dockWidgetEntity
     * pelo objeto treeViewEntity
     */
    connect(checkBoxStatus, SIGNAL(stateChanged(int)), treeViewEntity,
            SLOT(stateChanged(int)) );
    connect(treeViewEntity, SIGNAL(stateName(QString)), this,
            SLOT(setCheckBoxText(QString)) );
    connect(spinBoxAno, SIGNAL(valueChanged(int)), treeViewEntity,
            SLOT(valueYearChanged(int)) );

    // poe o inputmask correto para o campo escolhido
    connect(comboBoxSearch, SIGNAL(currentIndexChanged(int)), this,
            SLOT(lineEditSearchFormat(int)) );
    // pega o valor do indice
    connect(comboBoxSearch, SIGNAL(currentIndexChanged(int)), treeViewEntity,
            SLOT(tipoIndex(int)) );
    connect(lineEditSearch, SIGNAL(textChanged(QString)), treeViewEntity,
            SLOT(lineEditSearchChanged(QString)) );
    connect(toolButtonSearch, SIGNAL(clicked(bool)), treeViewEntity,
            SLOT(buttonSearchClicked(bool)) );


#ifdef Q_OS_WIN
    /**
     * @brief connect
     *
     * Ligacoes para montar o progress bar na barra de tarefas do
     * Windows
     */
    connect(treeViewEntity, SIGNAL(setMaximumItem(int)), taskProgress,
            SLOT(setMaximum(int)) );
    connect(treeViewEntity, SIGNAL(setVisibleItem(bool)), taskProgress,
            SLOT(setVisible(bool)) );
    connect(treeViewEntity, SIGNAL(setValueItem(int)), taskProgress,
            SLOT(setValue(int)) );

    connect(treeViewFind, SIGNAL(setMaximumItem(int)), taskProgress,
            SLOT(setMaximum(int)) );
    connect(treeViewFind, SIGNAL(setVisibleItem(bool)), taskProgress,
            SLOT(setVisible(bool)) );
    connect(treeViewFind, SIGNAL(setValueItem(int)), taskProgress,
            SLOT(setValue(int)) );
#endif

    // Cria as acoes e menus principais
    createActions();
    createMenus();
}

qDocBase::~qDocBase()
{
}

/*$SPECIALIZATION$*/

/**
 * @brief qDocBase::showEvent
 * @param e
 */
void qDocBase::showEvent(QShowEvent *e)
{
#ifdef Q_OS_WIN
    taskButton->setWindow(windowHandle());
#endif
    e->accept();
}

/*
 *
 *
 */
void qDocBase::lineEditSearchFormat(int i)
{
    lineEditSearch->clear();

    switch(i) {
    case 0: // Vazio
        lineEditSearch->setInputMask("");
        lineEditSearch->setEnabled(false);
        break;
    case 1: // CNPJ
        lineEditSearch->setEnabled(true);
        lineEditSearch->setInputMask("00.000.000/0000-00");
        break;
    case 2: // IE
        lineEditSearch->setEnabled(true);
        lineEditSearch->setInputMask("000.000.000.000");
        break;
    case 3: // CPF
        lineEditSearch->setEnabled(true);
        lineEditSearch->setInputMask("000.000.000-00");
        break;
    case 4: // RG
        lineEditSearch->setEnabled(true);
        lineEditSearch->setInputMask("000.000.000-/XXX-XX");
        break;
    }
}

void qDocBase::setCheckBoxText(const QString s)
{
    checkBoxStatus->setText(s);
}

void qDocBase::createMenus()
{
     menuArquivos->addAction(actionModulos);
     menuArquivos->addAction(actionEntidades);
     menuArquivos->addAction(actionFind);
     menuArquivos->addSeparator();
     menuArquivos->addAction(actionSair);

     menuFerramentas->addAction(actionConfig);
     menuFerramentas->addAction(actionSecur);

     menuAjuda->addAction(actionAbout);
     menuAjuda->addAction(actionAboutQt);

     // popup menu de impressao
     popupPrintMenu = new QMenu;

}

void qDocBase::createActions()
{   
     // Arquivos
     actionModulos = dockWModulos->toggleViewAction();
     actionModulos->setShortcut(QString("Ctrl+M"));
     actionModulos->setStatusTip(tr("Módulos do Sistema"));

     actionEntidades = dockWidgetEntity->toggleViewAction();
     actionEntidades->setShortcut(QString("Ctrl+E"));
     actionEntidades->setStatusTip(tr("Pesquisa/Visualização por Entidades Cadastradas"));

     actionFind = dockWidgetFindDocs->toggleViewAction();
     actionFind->setShortcut(QString("Ctrl+F"));
     actionFind->setStatusTip(tr("Pesquisa por Documentos"));

     actionSair = new QAction(QIcon::fromTheme("application-exit", QIcon(":/exit.png")),
                              tr("Sai&r"), this);
     actionSair->setShortcut(QString("Ctrl+Q"));
     actionSair->setStatusTip(tr("Sair do Sistema"));
     connect(actionSair, SIGNAL(triggered()), this, SLOT(actionSair_triggered()));

     // Ferramentas
     actionConfig = new QAction(QIcon(":/configure.png"), tr("C&onfigurações"),this);
     actionConfig->setShortcut(QString("Ctrl+O"));
     actionConfig->setStatusTip(tr("Configurações do Sistema"));
     connect(actionConfig, SIGNAL(triggered()), this, SLOT(actionConfig_triggered()));

     actionSecur = new QAction(QIcon(":/cadeado64x64.png"), tr("Seg&urança"),this);
     actionSecur->setShortcut(QString("Ctrl+U"));
     actionSecur->setStatusTip(tr("Administração de Usuários"));
     connect(actionSecur, SIGNAL(triggered()), this, SLOT(actionSecur_triggered()));

     // Ajuda
     //help-about
     actionAbout = new QAction(QIcon::fromTheme("help-about", QIcon(":/messagebox_info.png")),
                               tr("Sobre DocBase..."), this);
     actionAbout->setStatusTip(tr("Informações sobre o DocBase"));
     connect(actionAbout, SIGNAL(triggered()), this, SLOT(actionAbout_triggered()));


     actionAboutQt = new QAction(QIcon::fromTheme("help-about", QIcon(":/messagebox_info.png")),
                               tr("About Qt..."), this);
     actionAboutQt->setStatusTip(tr("About Qt"));
     connect(actionAboutQt, SIGNAL(triggered()), this, SLOT(actionAboutQt_triggered()));
}

void qDocBase::createActionsPrint(ModulesOfApp modulo)
{
     // Popup menus
     switch(modulo) {
     case ModulesOfApp::ModEntity:
         actionPrintActiveEntities = new QAction(QIcon(":/generic.png"), tr("Imprime Entidades Ativas"), this );
         actionPrintInactiveEntities = new QAction(QIcon(":/generic.png"), tr("Imprime Entidades Inativas"), this );
         break;
     case ModulesOfApp::ModDocuments:
         actionPrintOneImage = new QAction(QIcon(":/image-generic.png"), tr("Imprime atual"), this );
         actionPrintOneImage->setShortcut(QString("Ctrl+P"));
         actionPrintImages = new QAction(QIcon(":/image-several-generic.png"), tr("Imprime selecionadas"), this );
         actionPrintListDocs = new QAction(QIcon(":/reportgreen.png"), tr("Listagem Geral"), this );
         actionPrintListDocs->setStatusTip(tr("Listagem do geral do cadastro de Entidades sem imagens"));
         actionPrintListValDocs = new QAction(QIcon(":/reportgreen.png"), tr("Listagem Analítica"), this );
         actionPrintListValDocs->setStatusTip(tr("Listagem do cadastro de Entidades para análise de valores"));
         break;
     }
}

void qDocBase::createMenusPrint(ModulesOfApp modulo)
{
     switch(modulo) {
     case ModulesOfApp::ModEntity:
         popupPrintMenu->addAction(actionPrintActiveEntities);
         popupPrintMenu->addAction(actionPrintInactiveEntities);
         break;
     case ModulesOfApp::ModDocuments:
         popupPrintMenu->addAction(actionPrintOneImage);
         popupPrintMenu->addAction(actionPrintImages);
         popupPrintMenu->addSeparator();
         popupPrintMenu->addAction(actionPrintListDocs);
         popupPrintMenu->addAction(actionPrintListValDocs);
         break;
     }
}

void qDocBase::createToolBarRegistroDocumentos(const QString title, ModulesOfApp modulo)
{
    Configuracao cfg;

    createActionsPrint(modulo); // primeiro criar actions
    createMenusPrint(modulo);   // depois criar menus

    toolButtonSave = new QToolButton;
    toolButtonSave->setIcon(QIcon::fromTheme("document-save", QIcon(":/filesave.xpm")));
    toolButtonSave->setToolTip(tr("Grava o registro [F10]"));
    toolButtonSave->setShortcut(QKeySequence(Qt::Key_F10));
    if(cfg.getReadOnly())
        toolButtonSave->setEnabled(false);

    toolButtonUpdate = new QToolButton;
    toolButtonUpdate->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/redo.png")));
    toolButtonUpdate->setToolTip(tr("Grava as alterações\nefetuadas [F9]"));
    toolButtonUpdate->setShortcut(QKeySequence(Qt::Key_F9));
    if(cfg.getReadOnly() || cfg.getLimited())
        toolButtonUpdate->setEnabled(false);

    toolButtonClear = new QToolButton;
    toolButtonClear->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/cls.png")));
    toolButtonClear->setToolTip(tr("Limpa os campos\nna janela [F12]"));
    toolButtonClear->setShortcut(QKeySequence(Qt::Key_F12));

    toolButtonDelete = new QToolButton;
    toolButtonDelete->setIcon(QIcon::fromTheme("edit-delete", QIcon(":/fileclose.png"))); //setIcon(QIcon(":/fileclose.png"));
    toolButtonDelete->setToolTip(tr("Remove o registro\ndo banco de dados"));
    if(cfg.getReadOnly() || cfg.getLimited())
        toolButtonDelete->setEnabled(false);


    toolButtonPrint = new QToolButton;
    toolButtonPrint->setIcon(QIcon::fromTheme("document-print-preview", QIcon(":/fileprint.png")));
    toolButtonPrint->setToolTip(tr("Imprime ... [Ctrl-P]"));
    toolButtonPrint->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
    if(modulo == ModulesOfApp::ModDocuments) {
        toolButtonPrint->setVisible(false);
    }

    toolButtonZoomOut = new QToolButton;
    toolButtonZoomOut->setIcon(QIcon::fromTheme("zoom-in", QIcon(":/zoomout.png")));
    toolButtonZoomOut->setToolTip(tr("Amplia imagem [Ctrl-+]"));
    toolButtonZoomOut->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Plus));
    if(modulo == ModulesOfApp::ModEntity) {
        toolButtonZoomOut->setEnabled(false);
        toolButtonZoomOut->setVisible(false);
    }

    toolButtonZoomIn = new QToolButton;
    toolButtonZoomIn->setIcon(QIcon::fromTheme("zoom-out", QIcon(":/zoomin.png")));
    toolButtonZoomIn->setToolTip(tr("Reduz imagem [Ctrl--]"));
    toolButtonZoomIn->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Minus));
    if(modulo == ModulesOfApp::ModEntity) {
        toolButtonZoomIn->setEnabled(false);
        toolButtonZoomIn->setVisible(false);
    }

    toolButtonRotate = new QToolButton;
    toolButtonRotate->setIcon(QIcon::fromTheme("object-rotate_right", QIcon(":/rotate_cw.png")));
    toolButtonRotate->setToolTip(tr("Gira imagem [Ctrl-R]"));
    toolButtonRotate->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
    if(modulo == ModulesOfApp::ModEntity) {
        toolButtonRotate->setEnabled(false);
        toolButtonRotate->setVisible(false);
    }

    toolButtonExport = new QToolButton;
    toolButtonExport->setIcon(QIcon(":/fileexport.png"));
    toolButtonExport->setToolTip(tr("Exportação de documentos"));
    toolButtonExport->setEnabled(false);
    if(modulo == ModulesOfApp::ModEntity) {
        toolButtonExport->setEnabled(false);
        toolButtonExport->setVisible(false);
    }

    toolButtonPrintMenu = new QToolButton;
    toolButtonPrintMenu->setIcon(QIcon::fromTheme("document-print-preview", QIcon(":/fileprint.png")));
    toolButtonPrintMenu->setToolTip(tr("Impressão"));
    toolButtonPrintMenu->setMenu(popupPrintMenu);
    toolButtonPrintMenu->setPopupMode(QToolButton::InstantPopup);


    toolBarCadastro = addToolBar(title);
    toolBarCadastro->setFloatable(false);
    toolBarCadastro->setMovable(false);
    toolBarCadastro->addWidget(toolButtonSave);
    toolBarCadastro->addWidget(toolButtonUpdate);
    toolBarCadastro->addWidget(toolButtonClear);
    toolBarCadastro->addSeparator();
    toolBarCadastro->addWidget(toolButtonDelete);


    toolBarCadastro->addSeparator();
    toolBarCadastro->addWidget(toolButtonZoomOut);
    toolBarCadastro->addWidget(toolButtonZoomIn);
    toolBarCadastro->addWidget(toolButtonRotate);
    (modulo == ModulesOfApp::ModDocuments ?  toolBarCadastro->addSeparator() : nullptr);
    toolBarCadastro->addWidget(toolButtonExport);
    toolBarCadastro->addSeparator();
    toolBarCadastro->addWidget(toolButtonPrintMenu);

    if(modulo == ModulesOfApp::ModEntity) {
        comboBoxEntities = new QComboBox;
        comboBoxEntities->setToolTip(tr("Escolha uma classe de Entidade para listagem"));
        comboBoxEntities->addItems(QStringList() << tr("Todas") <<
                                   tr("Empresas") << tr("Clientes") <<
                                   tr("Fornecedores") << tr("Projetos e Obras") <<
                                   tr("Personalizados") << tr("Simples") );
        toolBarCadastro->addWidget(comboBoxEntities);
    }

    if(modulo == ModulesOfApp::ModDocuments) {
        dateEditBegin = new QDateEdit;
        dateEditBegin->setToolTip(tr("Informe a data inicial"));
        dateEditBegin->setStatusTip(tr("Data inicial para seleção dos documentos que serão selecionados"));
        dateEditBegin->setCalendarPopup(true);
        dateEditBegin->setDisplayFormat("dd/MM/yyyy");
        dateEditBegin->setDate( QDate::fromString("01/01/2000","dd/MM/yyyy") );

        dateEditEnd = new QDateEdit;
        dateEditEnd->setToolTip(tr("Informe a data final"));
        dateEditEnd->setStatusTip(tr("Data final para seleção dos documentos "
                                     "que serão selecionados"));
        dateEditEnd->setCalendarPopup(true);
        dateEditEnd->setDisplayFormat("dd/MM/yyyy");
        dateEditEnd->setDate( QDate::currentDate() );

        checkBoxDataTipo = new QCheckBox;
        checkBoxDataTipo->setStatusTip(tr("Informa se deve selecionar os documentos "
                                          "pela data de cadastro ou do documento"));
        checkBoxDataTipo->setChecked(true);
        checkBoxDataTipo->setText(checkBoxDataTipoTitleChecked);
        connect(checkBoxDataTipo, SIGNAL(toggled(bool)), this,
                SLOT(checkBoxDataTipo_toggled(bool)) );

        toolBarCadastro->addWidget(dateEditBegin);
        toolBarCadastro->addWidget(dateEditEnd);
        toolBarCadastro->addWidget(checkBoxDataTipo);
    }

    toolBarCadastro->addSeparator();
}

void qDocBase::createToolBarBuscaAvancada()
{
    toolButtonExec = new QToolButton;
    toolButtonExec->setIcon(QIcon::fromTheme("start-here", QIcon(":/exec.png")));
    toolButtonExec->setToolTip(tr("Executa a busca [F3]"));
    toolButtonExec->setShortcut(QKeySequence(Qt::Key_F3));

    toolButtonClear = new QToolButton;
    toolButtonClear->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/cls.png")));
    toolButtonClear->setToolTip(tr("Restaura os valores padrão"));
    toolButtonClear->setShortcut(QKeySequence(Qt::Key_F12));


    toolButtonPrint = new QToolButton;
    toolButtonPrint->setIcon(QIcon::fromTheme("document-print-preview", QIcon(":/fileprint.png")));
    toolButtonPrint->setToolTip(tr("Imprime ... [Ctrl-P]"));
    toolButtonPrint->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
    toolButtonPrint->setEnabled(false);

    toolButtonExport = new QToolButton;
    toolButtonExport->setIcon(QIcon(":/fileexport.png"));
    toolButtonExport->setToolTip(tr("Exportação de documentos"));
    toolButtonExport->setEnabled(false);

    toolButtonZoomOut = new QToolButton;
    toolButtonZoomOut->setIcon(QIcon::fromTheme("zoom-in", QIcon(":/zoomout.png")));
    toolButtonZoomOut->setToolTip(tr("Amplia imagem [Ctrl-+]"));
    toolButtonZoomOut->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Plus));
    toolButtonZoomOut->setEnabled(false);

    toolButtonZoomIn = new QToolButton;
    toolButtonZoomIn->setIcon(QIcon::fromTheme("zoom-out", QIcon(":/zoomin.png")));
    toolButtonZoomIn->setToolTip(tr("Reduz imagem [Ctrl--]"));
    toolButtonZoomIn->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Minus));
    toolButtonZoomIn->setEnabled(false);

    toolButtonRotate = new QToolButton;
    toolButtonRotate->setIcon(QIcon::fromTheme("object-rotate_right", QIcon(":/rotate_cw.png")));
    toolButtonRotate->setToolTip(tr("Gira imagem [Ctrl-R]"));
    toolButtonRotate->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
    toolButtonRotate->setEnabled(false);

    toolBarBuscaAvancada = addToolBar(tr("Busca Avançada"));
    toolBarBuscaAvancada->setFloatable(false);
    toolBarBuscaAvancada->setMovable(false);
    toolBarBuscaAvancada->addWidget(toolButtonExec);
    toolBarBuscaAvancada->addWidget(toolButtonClear);
    toolBarBuscaAvancada->addSeparator();
    toolBarBuscaAvancada->addWidget(toolButtonPrint);
    toolBarBuscaAvancada->addWidget(toolButtonExport);
    toolBarBuscaAvancada->addSeparator();
    toolBarBuscaAvancada->addWidget(toolButtonZoomOut);
    toolBarBuscaAvancada->addWidget(toolButtonZoomIn);
    toolBarBuscaAvancada->addWidget(toolButtonRotate);
    toolBarBuscaAvancada->addSeparator();


}

/*$SLOTS$*/
void qDocBase::actionSair_triggered()
{
  if( !isWindowsActived() )
      close();
}

void qDocBase::actionConfig_triggered()
{
    dlgConfig = new DialogConfig;
    dlgConfig->show();
}


void qDocBase::actionSecur_triggered()
{
    topSecret = new TopSecret;
    topSecret->show();
}

void qDocBase::actionAbout_triggered()
{
    dlgAbout = new DialogAbout;
    dlgAbout->setWindowFlags( Qt::Dialog |Qt::FramelessWindowHint);
    dlgAbout->show();
}

void qDocBase::actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

/**
 * @brief checkBoxDataTipo_toggled
 */
void qDocBase::checkBoxDataTipo_toggled(bool b)
{
    if(b)
        checkBoxDataTipo->setText(checkBoxDataTipoTitleChecked);
    else
        checkBoxDataTipo->setText(checkBoxDataTipoTitleUnchecked);
}

/**
 * @brief Este slot recebe o sinal hide(DockType) emitido
 *        pelas janelas a fim de fechar as areas docks
 *        de pesquisa de entidades e documentos e modulos
 * @param doc
 */
void qDocBase::hideDocks(DockType doc)
{
    switch(doc) {
    case NS_UTILITIES::DockEntity:
        dockWidgetEntity->hide();
        break;
    case NS_UTILITIES::DockFindDocs:
        dockWidgetFindDocs->hide();
        break;
    case NS_UTILITIES::DockAdvSearch:
        dockWidgetEntity->hide();
        break;
    case NS_UTILITIES::DockModulos:
        dockWModulos->setVisible(false);
        break;
    }
}

/**
 * @brief Recebe o sinal deleteMenu(int modulo) emitido pelos modulos do
 *      sistema para dessa forma remover items do menu de impressao a
 *      fim de que eles nao se misturem quando os modulos forem chamados
 * @param modulo
 */
void qDocBase::deleteMenu(ModulesOfApp modulo)
{
    switch(modulo) {
    case ModulesOfApp::ModEntity:
        popupPrintMenu->removeAction(actionPrintActiveEntities);
        popupPrintMenu->removeAction(actionPrintInactiveEntities);
        break;
    case ModulesOfApp::ModDocuments:
        popupPrintMenu->removeAction(actionPrintOneImage);
        popupPrintMenu->removeAction(actionPrintImages);
        popupPrintMenu->removeAction(actionPrintListDocs);
        popupPrintMenu->removeAction(actionPrintListValDocs);
        break;
    default:
        break;
    }
}

/**
 * Trata os eventos de fechamento da Mainwindow.
 * Pega todos os botoes que fecham a janela.
 * @param event
 */
void qDocBase::closeEvent(QCloseEvent *event)
{
   if(!isWindowsActived() ) {
      event->accept();
      db.close();
      close();
   }
   else
      event->ignore();
}

/**
 * Verifica se ha janelas abertas.
 * Retorna true se sim e false se nao
 * @return true|false
 */
bool qDocBase::isWindowsActived()
{
  if(mdiArea->activeSubWindow()) { // ha janelas abertas
     QMessageBox::warning(this , PROGRAM_NAME,
                  tr("Para a segurança das suas informações "
                     "\nnão é permitido encerrar o programa "
                     "\nsem que todas as janelas estejam fechadas.")
                  ,QMessageBox::Close);
      return(true);
  }
  return(false);
}


/**
 * @brief qDocBase::showStatusBarMessage
 *        Recebe uma mensagem dos demais objetos
 *        para mostrar no statusBar da MainWindow
 *
 * @param m
 */
void qDocBase::showStatusBarMessage(const QString &m)
{
    statusBar()->showMessage(m);
}

/**
 * @brief qDocBase::lvModulos_doubleClicked
 *          Interpretacao principal da escolha do usuario
 *          pelo modulo.
 *          Cria todas os callbacks necessarios e parametrizacoes
 * @param idx
 */
void qDocBase::lvModulos_doubleClicked(QModelIndex idx)
{
   bool ok = false;

   ModulesOfApp modulos = static_cast<ModulesOfApp>(idx.data(Qt::UserRole).toInt());

   switch(modulos) {
    case ModulesOfApp::ModEntity:
       // C++11 compliant
       for(QMdiSubWindow *w : mdiArea->subWindowList()) {
          if(w->windowTitle().contains(tr("Entidades")) ) {
             ok = true;
             break;
          }
       }
       if(!ok) {
           createToolBarRegistroDocumentos(tr("Registro de Entidades"),ModulesOfApp::ModEntity);
           recEntity = new RecordEntity;

           connect(treeViewEntity, SIGNAL(doubleClicked(QModelIndex)), recEntity,
                   SLOT(nameEntity(QModelIndex)) );

           connect(toolButtonSave, SIGNAL(clicked()), recEntity,
                   SLOT(saveReg()) );
           connect(toolButtonUpdate, SIGNAL(clicked()), recEntity,
                   SLOT(updateReg()) );

           connect(toolButtonClear, SIGNAL(clicked()), recEntity,
                   SLOT(clearReg()) );
           connect(toolButtonDelete, SIGNAL(clicked()), recEntity,
                   SLOT(deleteReg()) );

           connect(actionPrintActiveEntities, SIGNAL(triggered()), recEntity,
                   SLOT(printActiveReg()) );
           connect(actionPrintInactiveEntities, SIGNAL(triggered()), recEntity,
                   SLOT(printInactiveReg()) );

           /* Parametros para impreassao */
           connect(comboBoxEntities, SIGNAL(currentIndexChanged(int)), recEntity,
                   SLOT(comboBoxEntities_currentIndexChanged(int)) );

           /* Trata o sinal emitdo pela janela RecordDocs para o fechamento
            * das areas dockWidgetEntity apos a
            * selecao ser efetuada pelo usuario via duplo-clique
            */
           connect(recEntity, SIGNAL(hide(DockType)), this,
                   SLOT(hideDocks(DockType)) );

           /*
            * O recEntity ao ser fechado pelo usuario emite um sinal informando
            * ao toolbar que ele deve ser posto no modo invisivel.
            * o toolbar somente deve ficar visivel quando for necessario utiliza-lo
            *
            * O valor boleano do mesmo sinal e tambem interceptado para
            * desmarca o checkBoxEntityIdentify
            *
            * Ao fechar a janela tambem esconde a dockWidgetEntity e dockWidgetFindDocs
            */
           connect(recEntity, SIGNAL(closed(bool)),toolBarCadastro, SLOT(setVisible(bool)) );
           connect(recEntity, SIGNAL(closed(bool)), dockWidgetEntity, SLOT(hide()) );
           connect(recEntity, SIGNAL(closed(bool)), dockWidgetFindDocs, SLOT(hide()) );

           // Remove as acoes do menu de impressao ao fechar o modulo
           connect(recEntity, SIGNAL(deleteMenu(ModulesOfApp)), this, SLOT(deleteMenu(ModulesOfApp)) );


           dockWModulos->setVisible(false);

           // Carrega a Janela
           QMdiSubWindow *cWin = new QMdiSubWindow;
           cWin->setWidget(recEntity);
           cWin->setAttribute(Qt::WA_DeleteOnClose);
           mdiArea->addSubWindow(cWin);
           cWin->showMaximized();
       }
       break;
    case ModulesOfApp::ModDocuments:
       // C++11 compliant
       for(QMdiSubWindow *w : mdiArea->subWindowList()) {
          if(w->windowTitle().contains(tr("Registro")) ) {
             ok = true;
             break;
          }
       }
       if(!ok) {
           createToolBarRegistroDocumentos(tr("Registro de Documentos"),ModulesOfApp::ModDocuments);

           recDocs = new RecordDocs;
           connect(recDocs, SIGNAL(showStatusBarMessage(const QString&)), this,
                   SLOT(showStatusBarMessage(const QString&)) );

           connect(treeViewEntity, SIGNAL(doubleClicked(QModelIndex)), recDocs,
                   SLOT(nameEntity(QModelIndex)) );

           connect(checkBoxEntityIdentify, SIGNAL(clicked(bool)), recDocs,
                   SLOT(entityIdentify_clicked(bool)) );

           /* Trata o sinal emitdo pela janela RecordDocs para o fechamento
            * das areas dockWidgetEntity e dockWidgetFindDocs apos a
            * selecao ser efetuada pelo usuario via duplo-clique
            */
           connect(recDocs, SIGNAL(hide(DockType)), this,
                   SLOT(hideDocks(DockType)) );

           connect(treeViewFind, SIGNAL(doubleClicked(QModelIndex)), recDocs,
                   SLOT(identifyEntity(QModelIndex)) );

           connect(toolButtonSave, SIGNAL(clicked()), recDocs,
                   SLOT(saveReg()) );
           connect(toolButtonUpdate, SIGNAL(clicked()), recDocs,
                   SLOT(updateReg()) );
           connect(toolButtonClear, SIGNAL(clicked()), recDocs,
                   SLOT(clearReg()) );
           connect(toolButtonDelete, SIGNAL(clicked()), recDocs,
                   SLOT(deleteReg()) );

           connect(toolButtonZoomIn, SIGNAL(clicked()), recDocs,
                   SLOT(zoomIn()) );
           connect(toolButtonZoomOut, SIGNAL(clicked()), recDocs,
                   SLOT(zoomOut()) );
           connect(toolButtonRotate, SIGNAL(clicked()), recDocs,
                   SLOT(imageRotate()) );

           connect(toolButtonExport, SIGNAL(clicked()), recDocs,
                   SLOT(documentExport()) );
           // trata o sinal enviado por recDocs informando se
           // o botao de exportar imagem deva estar habilitado ou nao
           connect(recDocs, SIGNAL(isExportable(bool)), toolButtonExport,
                   SLOT(setEnabled(bool)) );

           connect(actionPrintOneImage, SIGNAL(triggered()), recDocs,
                   SLOT(printOneImageReg()) );
           connect(actionPrintImages, SIGNAL(triggered()), recDocs,
                   SLOT(printImagesReg()) );

           connect(actionPrintListDocs, SIGNAL(triggered()), recDocs,
                   SLOT(printDocGeneral()) );
           connect(actionPrintListValDocs, SIGNAL(triggered()), recDocs,
                   SLOT(printDocAnalitic()) );

           /* Parametros para impressao */
           connect(dateEditBegin, SIGNAL(dateChanged(QDate)), recDocs,
                   SLOT(dateEditBegin_dateChanged(QDate)) );
           connect(dateEditEnd, SIGNAL(dateChanged(QDate)), recDocs,
                   SLOT(dateEditEnd_dateChanged(QDate)) );
           connect(checkBoxDataTipo, SIGNAL(toggled(bool)), recDocs,
                   SLOT(checkBoxDataTipo_toggled(bool)) );

           /*
            * O recDocs ao ser fechado pelo usuario emite um sinal informando
            * ao toolbar que ele deve ser posto no modo invisivel.
            * o toolbar somente deve ficar visivel quando for necessario utiliza-lo
            *
            * O valor boleano do mesmo sinal e tambem interceptado para
            * desmarca o checkBoxEntityIdentify
            *
            * Ao fechar a janela tambem esconde a dockWidgetEntity e dockWidgetFindDocs
            */
           connect(recDocs, SIGNAL(closed(bool)),toolBarCadastro, SLOT(setVisible(bool)) );
           connect(recDocs, SIGNAL(closed(bool)),checkBoxEntityIdentify, SLOT(setChecked(bool)) );
           connect(recDocs, SIGNAL(closed(bool)), dockWidgetEntity, SLOT(hide()) );
           connect(recDocs, SIGNAL(closed(bool)), dockWidgetFindDocs, SLOT(hide()) );

           // Remove as acoes do menu de impressao ao fechar o modulo
           connect(recDocs, SIGNAL(deleteMenu(ModulesOfApp)), this, SLOT(deleteMenu(ModulesOfApp)) );

           dockWModulos->setVisible(false);

           QMdiSubWindow *cWin = new QMdiSubWindow;
           cWin->setWidget(recDocs);
           cWin->setAttribute(Qt::WA_DeleteOnClose);
           mdiArea->addSubWindow(cWin);
           cWin->showMaximized();           
       }
       break;
   case ModulesOfApp::ModSearch:
       // C++11 compliant
       for(QMdiSubWindow *w : mdiArea->subWindowList()) {
          if(w->windowTitle().contains(tr("Busca")) ) {
             ok = true;
             break;
          }
       }
       if(!ok) {
            createToolBarBuscaAvancada();

            buscaAvancada = new AdvSearch;
            connect(toolButtonExec, SIGNAL(clicked()), buscaAvancada,
                    SLOT(executeSearch()) );
            connect(toolButtonClear, SIGNAL(clicked()), buscaAvancada,
                    SLOT(setDefaults()) );

            connect(toolButtonZoomIn, SIGNAL(clicked()), buscaAvancada,
                    SLOT(zoomOut()) );
            connect(toolButtonZoomOut, SIGNAL(clicked()), buscaAvancada,
                    SLOT(zoomIn()) );
            connect(toolButtonRotate, SIGNAL(clicked()), buscaAvancada,
                    SLOT(imageRotate()) );


            connect(toolButtonPrint, SIGNAL(clicked()), buscaAvancada,
                    SLOT(printImages()) );
            connect(toolButtonExport, SIGNAL(clicked()), buscaAvancada,
                    SLOT(documentExport()) );

            connect(treeViewEntity, SIGNAL(doubleClicked(QModelIndex)), buscaAvancada,
                    SLOT(nameEntity(QModelIndex)) );

            connect(checkBoxEntityIdentify, SIGNAL(clicked(bool)), buscaAvancada,
                    SLOT(entityIdentify_clicked(bool)) );


            /* Trata o sinal emitido pela janela AdvSearch para o fechamento
             * das areas dockWidgetEntityapos a
             * selecao ser efetuada pelo usuario via duplo-clique
             */
            connect(buscaAvancada, SIGNAL(hide(DockType)), this,
                    SLOT(hideDocks(DockType)) );

            connect(buscaAvancada, SIGNAL(activeTools(bool)), toolButtonExport,
                    SLOT(setEnabled(bool)) );
            connect(buscaAvancada, SIGNAL(activeTools(bool)), toolButtonPrint,
                    SLOT(setEnabled(bool)) );
            connect(buscaAvancada, SIGNAL(activeTools(bool)), toolButtonZoomIn,
                    SLOT(setEnabled(bool)) );
            connect(buscaAvancada, SIGNAL(activeTools(bool)), toolButtonZoomOut,
                    SLOT(setEnabled(bool)) );
            connect(buscaAvancada, SIGNAL(activeTools(bool)), toolButtonRotate,
                    SLOT(setEnabled(bool)) );

            connect(buscaAvancada, SIGNAL(closed(bool)),toolBarBuscaAvancada, SLOT(setVisible(bool)) );

            dockWModulos->setVisible(false);

            QMdiSubWindow *cWin = new QMdiSubWindow;
            cWin->setWidget(buscaAvancada);
            cWin->setAttribute(Qt::WA_DeleteOnClose);
            mdiArea->addSubWindow(cWin);
            cWin->showMaximized();
       }
       break;
   }
}

void qDocBase::dockWidgetEntity_visibilityChanged(bool /*b*/)
{
}


void qDocBase::tvEntity_doubleClicked(QModelIndex /*idx*/)
{
}
