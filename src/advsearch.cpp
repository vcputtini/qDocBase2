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

#include "advsearch.h"

AdvSearch::AdvSearch(QWidget *parent) :
    QWidget(parent),
    __IDENTIFY__(false),
    __ISVIEWABLE__(false)
{
    setupUi(this);

    tempFiles = new TemporaryFiles; // tratamento de arquivos temporarios

    AdvSearchTableViewItemDelegate *delegate = new AdvSearchTableViewItemDelegate(tableViewBusca);
    tableViewBusca->setItemDelegate(delegate);

    connect(checkBoxSortType, SIGNAL(toggled(bool)), this,
            SLOT(checkBoxSortType_toggled(bool)) );
    connect(checkBoxDateType, SIGNAL(toggled(bool)), this,
            SLOT(checkBoxDateType_toggled(bool)) );

    lineEditOperator->setValidator(new vcpUpperValidator);

    lineEditDocNumBegin->setValidator(new QIntValidator(0,9999999, lineEditDocNumBegin) );
    lineEditDocNumEnd->setValidator(new QIntValidator(0,9999999, lineEditDocNumEnd) );

    lineEditDocValueBegin->setValidator(new vcpDoubleValidator(0.0, 999999999.99, 2, lineEditDocValueBegin));
    lineEditDocValueEnd->setValidator(new vcpDoubleValidator(0.0, 999999999.99, 2, lineEditDocValueEnd));


    /* Conecta todos os groupboxes com o sinal toggle(bool) */
    QList<QGroupBox *> allGB = this->findChildren<QGroupBox *>();
    /* C++11 compliant */
    for(QGroupBox *gb : allGB) {
        connect(gb, SIGNAL(toggled(bool)), this,
                SLOT(dynamicGroupBox_toogle(bool)) );
    }

    fillComboBoxSort();

    /*
     * Cria uma propriedade "SORT" para ser usadas com flag
     * quando for criar a instrucao select.
     * SORT = "A" ORDER BY XXX
     * SORT = "D" ORDER BY DESC
     */
    checkBoxSortType->setProperty("SORT","A");
    checkBoxSortType->setChecked(true);

    setDefaults();

}

/* $SLOTS */
/**
 * @brief AdvSearch::dynamicGroupBox_toogle
 *      Usado para configurar o comportamento os QGroubBoxes
 *
 * @param b
 */
void AdvSearch::dynamicGroupBox_toogle(bool b)
{
    QGroupBox *gb = static_cast<QGroupBox*>(sender()); // determina qual o objeto correto que esta enviando o sinal

    /* Se b == true coloca o titulo do groupbox em negrito, senao volta ao padrao */
    if(b) {
        QFont font = gb->font();
        font.setBold(true);
        gb->setFont(font);
    } else {
       QFont font( "Sans Serif,10,-1,5,50,0,0,0,0,0" ); // padrao do Qt
       gb->setFont(font);
    }

}

/**
 * @brief RecordDocs::entityIdentify_clicked
 * @param b
 */
void AdvSearch::entityIdentify_clicked(bool b)
{
    __IDENTIFY__ = b;
}


/**
 * @brief RecordEntity::closeEvent
 * @param event
 */
void AdvSearch::closeEvent(QCloseEvent *event)
{
    /*
     * informa que a janela foi fechada.
     * emite o sinal close(false) para o
     * que o toolbar seja colocado no modo
     * invisivel
     */
    emit closed(false);
    event->accept();

    tempFiles->remove();
    delete tempFiles;
}

/**
 * @brief AdvSearch::fillComboBoxSort
 *        Preenche o combobox com os itens e cria
 *        propriedates para cada um deles, afim de
 *        facilitar a identificacao dos mesmos
 *        quando eles forem posteriormente selecionados
 */
void AdvSearch::fillComboBoxSort()
{
    QStringList list = {
            "#ID", "Nome da Entiddade",
            "Classe da Entidade",
            "Departamento",
            "Data do Cadastro",
            "Data do Documento",
            "Valor do Documento"
    };

    comboBoxSortFields->addItems(list);
    comboBoxSortFields->setProperty(pID, "A.DL_IDENT");
    comboBoxSortFields->setProperty(pENTITY, "ET_NOME");
    comboBoxSortFields->setProperty(pCLASSE, "A.DL_TIPO");
    comboBoxSortFields->setProperty(pDPTO, "A.DL_DPTO");
    comboBoxSortFields->setProperty(pDTCADAST, "A.DL_DTCADAST");
    comboBoxSortFields->setProperty(pDTDOC, "A.DL_DTDOC");
    comboBoxSortFields->setProperty(pVALDOC, "A.DL_VALOR");
}

void AdvSearch::setDefaults()
{
    labelEntity->clear();
    labelEntity->setText(tr("Ctrl-E para selecionar uma Entidade"));
    labelRelationShip->clear();

    lineEditDocNumBegin->clear();
    lineEditDocNumEnd->clear();
    lineEditDocValueBegin->clear();
    lineEditDocValueEnd->clear();
    lineEditOperator->clear();
    lineEditPass->clear();
    lineEditDataField->clear();

    dateEditBegin->setDate(QDate::fromString("01/01/2000","dd/MM/yyyy"));
    dateEditEnd->setDate(QDate::currentDate());


    QList<QGroupBox *> groupBoxes = this->findChildren<QGroupBox *>();
    /* C++11 compliant */
    for(QGroupBox *gb : groupBoxes) {
        gb->setChecked(false);
        if(gb->objectName() == "groupBoxHist") {
            lineEditPhrase->clear();
            checkBoxExact->setChecked(false);
        }
        if(gb->objectName() == "groupBoxDateRange")
            checkBoxDateType->setChecked(true);
        if(gb->objectName() == "groupBoxCustomFields")
            checkBoxCFExact->setChecked(false);
    }

    labelRelationShip->clear();

    Department dpto;
    comboBoxDeptos->clear();
    comboBoxDeptos->addItems( dpto.fillList() );

    Configuracao cfg;
    if(!cfg.fillList().isEmpty()) {
        groupBoxCustomFields->setEnabled(true);
        comboBoxFieldNames->clear();
        comboBoxFieldNames->addItems( cfg.fillList() );
    } else {
        groupBoxCustomFields->setEnabled(false);
    }

    QList<QGraphicsItem*> item = graphicsViewImages->items();
    qDeleteAll(item);

    QSqlQueryModel *m = new QSqlQueryModel;
    tableViewBusca->setModel(m);
    delete m;
}

void AdvSearch::checkBoxSortType_toggled(bool b)
{
    checkBoxSortType->setIcon( (b ? QPixmap(":/sortascending.png") : QPixmap(":/sortdescending.png")) );
    checkBoxSortType->setProperty("SORT", (b ? SORT_ASC : SORT_DESC) );
}

void AdvSearch::checkBoxDateType_toggled(bool b)
{
    checkBoxDateType->setText( (b ? tr("Cadastro") : tr("Documento")) );
}


void AdvSearch::nameEntity(QModelIndex idx)
{
    sTypeEntity = idx.parent().data().toString();
    sNameEntity = idx.data().toString();

    if(!sTypeEntity.isEmpty()) {
        if(idx.parent().data().toString() == uNameEntities(NS_UTILITIES::EnEmpresa))
           iTypeEntity = NS_UTILITIES::EnEmpresa;
        else if(idx.parent().data().toString() == uNameEntities(NS_UTILITIES::EnCliente))
            iTypeEntity = NS_UTILITIES::EnCliente;
        else if(idx.parent().data().toString() == uNameEntities(NS_UTILITIES::EnFornecedor))
            iTypeEntity = NS_UTILITIES::EnFornecedor;
        else if(idx.parent().data().toString() == uNameEntities(NS_UTILITIES::EnProjObras))
            iTypeEntity = NS_UTILITIES::EnProjObras;
        else if(idx.parent().data().toString() == uNameEntities(NS_UTILITIES::EnOutros))
            iTypeEntity = NS_UTILITIES::EnOutros;
        else if(idx.parent().data().toString() == uNameEntities(NS_UTILITIES::EnTipoSimples))
            iTypeEntity = NS_UTILITIES::EnTipoSimples;
    }
    else  {
        sTypeEntity = sNameEntity;
        sNameEntity = "";
        iTypeEntity = -1;
    }

    if(!__IDENTIFY__) {
        labelEntity->setText(
                QString("%1").arg(sNameEntity)
                );
    } else {
        if(groupBoxRelatoionship->isChecked()) {
            labelRelationShip->setText(
                QString("%1").arg(sNameEntity)
                );
            __IDENTIFY__ = false;
        }
    }
    emit hide(NS_UTILITIES::DockEntity);
}

void AdvSearch::zoomIn()
{
    if(__ISVIEWABLE__)
        uZoomImage(NS_UTILITIES::zOut, graphicsViewImages);
}

void AdvSearch::zoomOut()
{
    if(__ISVIEWABLE__)
        uZoomImage(NS_UTILITIES::zIn, graphicsViewImages);
}

void AdvSearch::imageRotate()
{
    if(__ISVIEWABLE__)
        uRotate(graphicsViewImages, 90.0);
}

void AdvSearch::tableViewBusca_clicked(QModelIndex idx)
{
    if(idx.row() < 0)
      return;

    int seq = vModel->data(vModel->index(idx.row(),0)).toInt();
    QString fname = vModel->data(vModel->index(idx.row(),8)).toString();
    tempFiles->add(QString("%0/%1").arg(QDir::tempPath()).arg(fname));

    TypeImage timg;

    graphicsViewImages->resetMatrix();
    scene = new QGraphicsScene(graphicsViewImages);

    if(timg.isImageViewable(fname)) {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QSqlQuery q;
        q.prepare( QString("SELECT SQL_SMALL_RESULT IM_IMAGE FROM DOCIMAGES WHERE DL_IDENT=%0").arg(seq) );
        q.exec();
        q.next();
        if(q.lastError().isValid()) {
           QApplication::restoreOverrideCursor();
           return;
        }
        QByteArray ba = qUncompress(q.value(0).toByteArray());
        QPixmap pic;
        pic.loadFromData(ba);

        gItem = new ItemInterceptDoubleClick;
        connect(gItem, SIGNAL(openUrlError(QString)), this, SLOT(urlError(QString)) );
        gItem->setProperty(seq, fname);
        gItem->setPixmap(pic);
        scene->addItem(gItem);
        graphicsViewImages->scale( 0.2,0.2 );
        graphicsViewImages->setScene(scene);
        QApplication::restoreOverrideCursor();
        __ISVIEWABLE__ = true;
    } else {
        gItem = new ItemInterceptDoubleClick;
        connect(gItem, SIGNAL(openUrlError(QString)), this, SLOT(urlError(QString)) );
        gItem->setProperty(seq, fname);
        gItem->setPixmap(timg.type(fname));
        scene->addItem(gItem);
        graphicsViewImages->scale( 0.5,0.5 );
        graphicsViewImages->setScene(scene);
        __ISVIEWABLE__ = false;
    }
}

void AdvSearch::urlError(const QString e)
{
    QMessageBox::warning(this , PROGRAM_NAME,
          e,QMessageBox::Cancel);
}

void AdvSearch::printImages()
{
    // Seleciona os itens marcados e monta a string list com os
    // nomes dos arquivos selecionados
    QItemSelectionModel *selectionModel = tableViewBusca->selectionModel();
    if(!selectionModel) // nada foi selecionado. Ignorar
       return;

    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;
    if(indexes.count()==0) {// nao seleciono nada
     QMessageBox::information( this , PROGRAM_NAME,
           tr("Selecione ao menos um documento."),QMessageBox::Close);
       return;
    }

    if(indexes.count() > 100) {
        QMessageBox m;
        m.setText(tr("Esta consulta irá gerar um relatório com %0 páginas").arg(indexes.count()));
        m.setInformativeText( tr("É aconselhável cancelar e gerar novamente em partes menores.") );
        m.setWindowTitle(tr("Impressão de Relatórios"));
        QPushButton *canceltButton = m.addButton( tr("Cancelar"), QMessageBox::ActionRole);
        QPushButton *okButton = m.addButton(tr("Continuar"), QMessageBox::ActionRole);
        Q_UNUSED(okButton);
        m.setDefaultButton(canceltButton);
        m.exec();
        if (m.clickedButton() == canceltButton) {
            tableViewBusca->clearSelection();
            return;
        }
    }

    TypeImage ti;
    PrintSelectedImages2 print;
    connect( &print, SIGNAL(message(const QString&)), this,
             SLOT(printMessage(const QString &)) );
    connect( &print, SIGNAL(lastError(const QString&)), this,
             SLOT(printLastError(const QString &)) );

    print.setTitle( PROGRAM_NAME );
    print.setSubTitle(tr("Listagem de Documentos: %0").arg( labelEntity->text().section("#",0,0)) );
    /* Monta a stringlist com os codigos selecionados.
     * apenas com os arquivos que podem ser imprimidos
     */
    QStringList strL;
    foreach(index, indexes) {
        if( !ti.isImageViewable(vModel->data(vModel->index(index.row(),hAnexo)).toString()))
          continue;
        QString seq = vModel->data(vModel->index(index.row(),hID)).toString();
        strL << seq;
    } // fim-foreach()

    print.setStringList(strL, "SELECT HIGH_PRIORITY IM_IMAGE FROM DOCIMAGES WHERE DL_IDENT=");
    print.preview();
    disconnect(&print, SIGNAL(message(const QString)), nullptr, nullptr);
    disconnect(&print, SIGNAL(lastError(const QString)), nullptr, nullptr);


    tableViewBusca->clearSelection();
}

void AdvSearch::printMessage(const QString &m)
{
    emit showStatusBarMessage(m);
}

void AdvSearch::printLastError(const QString &e)
{
    QMessageBox::warning(this , PROGRAM_NAME,
          e,QMessageBox::Cancel);
}

void AdvSearch::documentExport()
{
   Configuracao cfg;

   QMessageBox msgBox;
   msgBox.setText(tr("Você está prestes a exportar documentos para %0").arg(cfg.getExportDir()));
   msgBox.setInformativeText(tr("Você confirma esta operação?"));
   QPushButton *abortButton = msgBox.addButton(tr("Abortar"), QMessageBox::ActionRole);
   QPushButton *okButton = msgBox.addButton(tr("Confirmar"), QMessageBox::ActionRole);
   Q_UNUSED(okButton);
   msgBox.setDefaultButton(abortButton);
   msgBox.exec();
   if(msgBox.clickedButton() == abortButton)
       return;

   // Seleciona os itens marcados e monta a string list com os
   // nomes dos arquivos selecionados
   QItemSelectionModel *selectionModel = tableViewBusca->selectionModel();
   if(!selectionModel) // nada foi selecionado. Ignorar
      return;

   QModelIndexList indexes = selectionModel->selectedRows();  //selectedIndexes();
   QModelIndex index;
   int total = indexes.count();  // total de indices div pelas colunas.

   int seq;
   QString fname;
   QSqlQuery query;
   QStringList listExported;

   QProgressDialog progress(tr("Exportando ..."),
                            tr("Cancelar"), 0, total, this);
   progress.setWindowModality(Qt::WindowModal);
   progress.show();
   int i=0;
   bool canceled=false;
   foreach(index, indexes) {
     fname = cfg.getExportDir()+"/"+vModel->data(vModel->index(index.row(),hAnexo)).toString();
     seq = vModel->data(vModel->index(index.row(),hID)).toInt();
     listExported << QString("%0").arg(fname);

     progress.setValue(i);
     progress.setLabelText(tr("Arquivo: %0").arg(fname));
     qApp->processEvents();
     if(progress.wasCanceled()) {
        canceled=true;
        break;
     }

     QSqlDatabase::database().transaction();
     query.prepare( QString("SELECT HIGH_PRIORITY IM_IMAGE FROM DOCIMAGES WHERE DL_IDENT=%0").arg(seq) );
     query.exec();
     query.next();
     if( query.lastError().isValid()) {
      QMessageBox::information( this , PROGRAM_NAME,
           query.lastError().text(),QMessageBox::Close);
      QSqlDatabase::database().rollback();
      break;
     } else {
       QSqlDatabase::database().commit();

       // descompacta arquivos via thread
       QFuture<QByteArray> future = QtConcurrent::run(qUncompress,query.value(0).toByteArray());
       QByteArray ba = future.result();

       // Salva os arquivos no disco
       QvpMultiFiles mf;
       mf.setOutputFile(fname.simplified());
       mf.setData(ba);
       mf.execute();
       i++;
     }
   } // fim-foreach()
   if(total>0)
      progress.setValue(total);

   // se a exportacao for cancela apaga os arquivos exportados
   if(canceled) {
       //foreach(QString f, listExported) {
       // C++11 compliant
       for(const QString &f : listExported) {
           QFile fn(f);
           fn.remove();
       }
   }
   tableViewBusca->clearSelection();
   listExported.clear();
}

void AdvSearch::executeSearch()
{
    if(labelEntity->text().contains("Ctrl-E") ) {
        QMessageBox::warning(this , PROGRAM_NAME,
              tr("Use Ctrl-E e selecione ao menos uma Entidade"),QMessageBox::Ok);
        return;
    }

    QString sql = QString("SELECT A.DL_IDENT, DATE_FORMAT(A.DL_DTCADAST, '%d/%m/%Y') AS DL_DTCADAST, "
            "A.DL_OPERADOR, A.DL_DPTO, "
            "(SELECT B.ET_NOME FROM DOCENTITY B WHERE B.ET_IDENT=A.DL_ETRELA) AS ET_NOME, "
            "DATE_FORMAT(A.DL_DTDOC,'%d/%m/%Y') AS DL_DTDOC, A.DL_NUMERO, FORMAT(A.DL_VALOR,2) AS DL_VALOR,  "
            "A.DL_FNAME ");

    if(groupBoxCustomFields->isChecked()) {
        if(!comboBoxFieldNames->currentText().isEmpty() && !lineEditDataField->text().isEmpty()) {
            for(int i=0; i<comboBoxFieldNames->count(); ++i)
                sql += QString(" ,A.DL_FIELD0%0DATA").arg(i+1);
        }
    }

    sql += QString(" FROM DOCLIBRARY A WHERE A.ET_IDENT=%0").arg(labelEntity->text().section('#',1,1).toLongLong());


    if(groupBoxRelatoionship->isChecked()) {
        sql += QString(" AND A.DL_ETRELA=%0").arg(labelRelationShip->text().section('#',1,1).toLongLong());
    } // groupBoxRelationship

    if(groupBoxDateRange->isChecked()) {
        QDate begin(dateEditBegin->date());
        QDate end(dateEditEnd->date());

        if(checkBoxDateType->isChecked())
            sql += QString(" AND A.DL_DTCADAST BETWEEN '%0' AND '%1'")
                    .arg(begin.toString("yyyy-MM-dd"))
                    .arg(end.toString("yyyy-MM-dd"));
        else
            sql += QString(" AND A.DL_DTDOC BETWEEN '%0' AND '%1'")
                    .arg(begin.toString("yyyy-MM-dd"))
                    .arg(end.toString("yyyy-MM-dd"));
    } // groupBoxDateRange

    if(groupBoxSecurity->isChecked()) {
        if(!lineEditOperator->text().isEmpty() && !lineEditPass->text().isEmpty()) {
            if(uOperatorIsValid(lineEditOperator->text().simplified(), lineEditPass->text().simplified()) ) {
                sql += QString(" AND A.DL_OPERADOR='%0' AND A.DL_TOPSEC=1")
                        .arg(lineEditOperator->text().simplified());
            } else {
                QMessageBox::warning(this, PROGRAM_NAME,
                            tr("O par Operador e/ou Senha não combinam. Tente novamente."),
                            QMessageBox::Close );
                return;
            }
        } else if(!lineEditOperator->text().isEmpty() && lineEditPass->text().isEmpty()) {
            sql += QString(" AND A.DL_OPERADOR='%0' AND A.DL_TOPSEC=0")
                    .arg(lineEditOperator->text().simplified());
        }
    } else {
        sql += " AND A.DL_TOPSEC=0"; // so pega os liberados
    } // groupBoxSecurity

    if(groupBoxDataDoc->isChecked()) {
        // numero do documento
        if(lineEditDocNumBegin->text().toInt()>=0 && lineEditDocNumEnd->text().toInt()>0) {
            if(lineEditDocNumBegin->text().toInt() > lineEditDocNumEnd->text().toInt()) {
                QMessageBox::warning(this , PROGRAM_NAME,
                      tr("Número do documento final deve ser maior que o inicial"),QMessageBox::Ok);
                lineEditDocNumBegin->setFocus();
                return;
            }
            sql += QString(" AND A.DL_NUMERO BETWEEN %0 AND %1" )
                    .arg(lineEditDocNumBegin->text().toInt())
                    .arg(lineEditDocNumEnd->text().toInt());
        }
        if(lineEditDocNumBegin->text().toInt()>0 && lineEditDocNumEnd->text().toInt()==0) {
            QMessageBox::warning(this, PROGRAM_NAME,
                  tr("Informe a faixa inicial e final para pesquisa por número do documento"),QMessageBox::Ok);
            lineEditDocNumEnd->setFocus();
            return;
        }

        // Valor do documento
        if(lineEditDocValueBegin->text().toFloat()>=0.0 && lineEditDocValueEnd->text().toFloat()>0.0) {
            if(lineEditDocValueBegin->text().toFloat() > lineEditDocValueEnd->text().toFloat()) {
                QMessageBox::warning(this , PROGRAM_NAME,
                      tr("Valor final deve ser maior que o inicial"),QMessageBox::Ok);
                lineEditDocValueBegin->setFocus();
                return;
            }
            sql += QString(" AND A.DL_VALOR BETWEEN %0 AND %1" )
                    .arg(lineEditDocValueBegin->text().toFloat())
                    .arg(lineEditDocValueEnd->text().toFloat());
        }
        if(lineEditDocValueBegin->text().toFloat()>0 && lineEditDocValueEnd->text().toFloat()==0) {
            QMessageBox::warning(this , PROGRAM_NAME,
                  tr("Informe a faixa inicial e final para pesquisa por valores R$"),QMessageBox::Ok);
            lineEditDocValueEnd->setFocus();
            return;
        }
    } // groupBoxDataDoc

    if(groupBoxHist->isChecked()) {
        if(checkBoxExact->isChecked()) {
            sql += QString(" AND DL_OBS='%0'")
                    .arg(lineEditPhrase->text());
        } else {
            sql += QString(" AND DL_OBS LIKE '%%0%'")
                    .arg(lineEditPhrase->text());
        }
    } // groupBoxHist

    if(groupBoxDpto->isChecked()) {
        sql += QString(" AND A.DL_DPTO='%0'")
                .arg(comboBoxDeptos->currentText());
    } // groupBoxDpto

    if(groupBoxCustomFields->isChecked()) {
        switch (comboBoxFieldNames->currentIndex()) {
        case 0:
            if(checkBoxCFExact->isChecked())
               sql += QString(" AND A.DL_FIELD01DATA='%0'").arg(lineEditDataField->text().simplified());
            else
               sql += QString(" AND A.DL_FIELD01DATA LIKE '%%0%'").arg(lineEditDataField->text().simplified());
            break;
        case 1:
            if(checkBoxCFExact->isChecked())
               sql += QString(" AND A.DL_FIELD02DATA='%0'").arg(lineEditDataField->text().simplified());
            else
               sql += QString(" AND A.DL_FIELD02DATA LIKE '%%0%'").arg(lineEditDataField->text().simplified());
            break;
        case 2:
            if(checkBoxCFExact->isChecked())
               sql += QString(" AND A.DL_FIELD03DATA='%0'").arg(lineEditDataField->text().simplified());
            else
               sql += QString(" AND A.DL_FIELD03DATA LIKE '%%0%'").arg(lineEditDataField->text().simplified());
            break;
        case 3:
            if(checkBoxCFExact->isChecked())
               sql += QString(" AND A.DL_FIELD04DATA='%0'").arg(lineEditDataField->text().simplified());
            else
               sql += QString(" AND A.DL_FIELD04DATA LIKE '%%0%'").arg(lineEditDataField->text().simplified());
            break;
        case 4:
            if(checkBoxCFExact->isChecked())
               sql += QString(" AND A.DL_FIELD05DATA='%0'").arg(lineEditDataField->text().simplified());
            else
               sql += QString(" AND A.DL_FIELD05DATA LIKE '%%0%'").arg(lineEditDataField->text().simplified());
            break;
        default:
            break;
        }
    } // groupBoxCustomFields


    // ORDERNACAO
    if(comboBoxSortFields->currentIndex() == iID ) {
        sql += QString(" ORDER BY %0 %1")
                .arg(comboBoxSortFields->property(pID).toString())
                .arg( (checkBoxSortType->property("SORT")==SORT_DESC ? " DESC" : "" )  );
    } else if(comboBoxSortFields->currentIndex() == iENTITY ) {
        sql += QString(" ORDER BY %0 %1")
                .arg(comboBoxSortFields->property(pENTITY).toString())
                .arg( (checkBoxSortType->property("SORT")==SORT_DESC ? " DESC" : "" )  );
    } else if(comboBoxSortFields->currentIndex() == iCLASSE ) {
        sql += QString(" ORDER BY %0 %1")
                .arg(comboBoxSortFields->property(pCLASSE).toString())
                .arg( (checkBoxSortType->property("SORT")==SORT_DESC ? " DESC" : "" )  );
    } else if(comboBoxSortFields->currentIndex() == iDPTO) {
        sql += QString(" ORDER BY %0 %1")
                .arg(comboBoxSortFields->property(pDPTO).toString())
                .arg( (checkBoxSortType->property("SORT")==SORT_DESC ? " DESC" : "" )  );
    } else if(comboBoxSortFields->currentIndex() == iDTCADAST ) {
        sql += QString(" ORDER BY %0 %1")
                .arg(comboBoxSortFields->property(pDTCADAST).toString())
                .arg( (checkBoxSortType->property("SORT")==SORT_DESC ? " DESC" : "" )  );
    } else if(comboBoxSortFields->currentIndex() == iDTDOC ) {
        sql += QString(" ORDER BY %0 %1")
                .arg(comboBoxSortFields->property(pDTDOC).toString())
                .arg( (checkBoxSortType->property("SORT")==SORT_DESC ? " DESC" : "" )  );
    } else if(comboBoxSortFields->currentIndex() == iVALDOC ) {
        sql += QString(" ORDER BY %0 %1")
                .arg(comboBoxSortFields->property(pVALDOC).toString())
                .arg( (checkBoxSortType->property("SORT")==SORT_DESC ? " DESC" : "" )  );
    } else
        Q_UNREACHABLE();

    vModel = new QSqlQueryModel;
    vModel->setQuery(sql);
    vModel->setHeaderData(hID, Qt::Horizontal, tr("#ID"));
    vModel->setHeaderData(hDtcadast, Qt::Horizontal, tr("Data\nCadastro"));
    vModel->setHeaderData(hOperador, Qt::Horizontal, tr("Operador"));
    vModel->setHeaderData(hDpto, Qt::Horizontal, tr("Departamento"));
    vModel->setHeaderData(hRelacao, Qt::Horizontal, tr("Entidade\nRelacionada"));
    vModel->setHeaderData(hDtDoc, Qt::Horizontal, tr("Data Emissão\nDocumento"));
    vModel->setHeaderData(hNumDoc, Qt::Horizontal, tr("Número"));
    vModel->setHeaderData(hValDoc, Qt::Horizontal, tr("Valor R$"));
    vModel->setHeaderData(hAnexo, Qt::Horizontal, tr("Anexo"));
    int c=hAnexo+1;
    if(groupBoxCustomFields->isChecked()) {
        for(int i=0; i<comboBoxFieldNames->count(); ++i) {
            QString _header = comboBoxFieldNames->itemText(i);
            vModel->setHeaderData(c, Qt::Horizontal, _header);
            c++;
        }
    }
    tableViewBusca->setModel(vModel);

    for(int i=0; i<vModel->columnCount();++i)
          tableViewBusca->resizeColumnToContents(i);

    emit activeTools(true);
}

/**
 * Reimplementacao de QGraphicsPixmapItem
 * para detectar o douplo click do mouse na Scene
 * Quando o usuario fizer o duplo clique o arquivo
 * anexado deve ser mostrado no desktop pelo
 * programa gerador
 *
 */
void ItemInterceptDoubleClick::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(isExecutable(fname)) {
        emit openUrlError(tr("Para sua segurança o qDocBase2 não permite a execução direta de programas.") );
        event->ignore();
        return;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QSqlQuery q;
    q.prepare( QString("SELECT HIGH_PRIORITY IM_IMAGE FROM DOCIMAGES WHERE DL_IDENT=%0").arg(id) );
    q.exec();
    q.next();
    if(q.lastError().isValid()) {
        QApplication::restoreOverrideCursor();
        event->ignore();
        return;
    }

    QFuture<QByteArray> future = QtConcurrent::run(qUncompress, q.value(0).toByteArray());
    QByteArray ba = future.result();

    QvpMultiFiles *mf = new QvpMultiFiles;
    connect(mf,SIGNAL(error(QString)), this ,SLOT(internalError(QString)));
    QString locTemp = QDir::tempPath();

    QString outFile;
  #ifdef Q_OS_WIN32
     outFile = QString("%0/%1").arg(locTemp).arg(fname);
  #else
     outFile = QString("/%0/%1").arg(locTemp).arg(fname);
  #endif

    mf->setOutputFile(outFile);
    mf->setData(ba);
    mf->execute();
    QApplication::restoreOverrideCursor();

    if( !QDesktopServices::openUrl(QUrl("file:///"+outFile, QUrl::TolerantMode)) ) {
         emit openUrlError(tr("Não foi possível abri o arquivo. "
              "Talvez não haja um aplicativo capaz de abri-lo "
              "instalado no seu computador ou associado ao tipo.") );
         event->ignore();
         return;
    }

    QApplication::restoreOverrideCursor();
    event->accept();
}

void ItemInterceptDoubleClick::setProperty(const qint64 ident, const QString fileName)
{
    id = ident;
    fname = fileName;
}

void ItemInterceptDoubleClick::internalError(QString e)
{
    emit fileError(e);
}
