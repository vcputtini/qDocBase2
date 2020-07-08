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

#include "recorddocs.h"

RecordDocs::RecordDocs(QWidget *parent) :
    QWidget(parent),
    __FIRSTRUN__(true),
    __IDENTIFY__(false),
    __EXPORTABLE__(false),
    __IMGSELECTED__(false),
    __ISVIEWABLE__(false),
    dateBegin(QDate::fromString("01/01/2000","dd/MM/yyyy")),
    dateEnd(QDate::currentDate()),
    dateTipo(true),
    pathImages(""),
    isCPNJ_OR_CPF(true)
{
    setupUi(this);

    connect(toolButtonSave, SIGNAL(clicked()), this,
            SLOT(toolButtonSave_clicked()) );
    connect(toolButtonDel, SIGNAL(clicked()), this,
            SLOT(toolButtonDel_clicked()) );

    connect(toolButtonDir, SIGNAL(clicked()), this,
            SLOT(toolButtonDir_clicked()) );

    connect(radioButtonCheckID, SIGNAL(toggled(bool)), this,
            SLOT(radioButtonCheckID_toggled(bool)));

    connect(toolButtonCopyCNPJ, SIGNAL(clicked(bool)), this,
            SLOT(toolButtonCopyCNPJ_clicked(bool)));


    // Configuracao da lineedit usada FileDialog::DontResolveSymlinksno ComboBox
    // Departamentos
    lineEditDeptos = new QLineEdit;
    lineEditDeptos->setPlaceholderText(tr("departamento"));
    lineEditDeptos->setMaxLength(15);

    labelFileNames->clear();
    labelImageName->clear();

    lineEditOperador->setValidator(new vcpUpperValidator);
    lineEditCustom0->setValidator(new vcpUpperValidator);
    lineEditCustom1->setValidator(new vcpUpperValidator);
    lineEditCustom2->setValidator(new vcpUpperValidator);
    lineEditCustom3->setValidator(new vcpUpperValidator);
    lineEditCustom4->setValidator(new vcpUpperValidator);

    lineEditDocNum->setValidator(new QIntValidator(0,9999999, lineEditDocNum) );
    lineEditDocValor->setValidator(new QDoubleValidator(0.0, 999999999.99, 2, lineEditDocValor));

    /*
     * Tratamento de edicao especializado
     */
    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    // C++11 compliant
    for(QLineEdit *le : allLineEdits) {
        connect(le, SIGNAL(editingFinished()), this,
                SLOT(dynamic_editFinished()) );
    }

    /* PARAMETRIZACAO */
    // Configura os campos custominzados se houverm
    if(CFG.CFldActive(1)) {
       lineEditCustom0->setPlaceholderText(CFG.CFldName(1));
       lineEditCustom0->setMaxLength(CFG.CFldLength(1));
       lineEditCustom0->setEnabled(true);
    } else
        lineEditCustom0->setEnabled(false);

    if(CFG.CFldActive(2)) {
        lineEditCustom1->setPlaceholderText(CFG.CFldName(2));
        lineEditCustom1->setMaxLength(CFG.CFldLength(2));
        lineEditCustom1->setEnabled(true);
    } else
        lineEditCustom1->setEnabled(false);

    if(CFG.CFldActive(3)) {
        lineEditCustom2->setPlaceholderText(CFG.CFldName(3));
        lineEditCustom2->setMaxLength(CFG.CFldLength(3));
        lineEditCustom2->setEnabled(true);
    } else
        lineEditCustom2->setEnabled(false);

    if(CFG.CFldActive(4)) {
        lineEditCustom3->setPlaceholderText(CFG.CFldName(4));
        lineEditCustom3->setMaxLength(CFG.CFldLength(4));
        lineEditCustom3->setEnabled(true);
    } else
        lineEditCustom3->setEnabled(false);

    if(CFG.CFldActive(5)) {
        lineEditCustom4->setPlaceholderText(CFG.CFldName(5));
        lineEditCustom4->setMaxLength(CFG.CFldLength(5));
        lineEditCustom4->setEnabled(true);
    } else
        lineEditCustom4->setEnabled(false);

    groupBoxDocValores->setChecked( CFG.fiscalSetting() );
    /* FIM PARAMETRIZACAO */

    /* prepara lista de arquivos */
    mdlThumbs = new QStandardItemModel;
    listViewFiles->setResizeMode(QListView::Adjust); // ajuste dinamico das largura da listView
    if(CFG.dirImageDefault()) {
        criaListaImagens();
        toolButtonDir->setEnabled(false);
    } else {
        toolButtonDir->setEnabled(true);
    }

    /* parametriza o comboxbox de selecao de arquivos
     * tem que ser apos chamar a funcao criaListaImagens()
     */
    lineEditFilters = new QLineEdit;
    if(CFG.dirImageDefault()) {
        lineEditFilters->setMaxLength(20);
    } else {
        lineEditFilters->setMaxLength(255);
    }
    comboBoxFileFilters->setLineEdit(lineEditFilters);
    comboBoxFileFilters->addItem(QIcon(":/generic.png"),"*.*");
    comboBoxFileFilters->addItem(QIcon(":/jpeg.png"),"*.jpg");
    comboBoxFileFilters->addItem(QIcon(":/jpeg.png"),"*.jpeg");
    comboBoxFileFilters->addItem(QIcon(":/pdf.png"),"*.pdf");
    comboBoxFileFilters->addItem(QIcon(":/doc.png"),"*.doc");
    comboBoxFileFilters->addItem(QIcon(":/docx.png"),"*.docx");
    comboBoxFileFilters->addItem(QIcon(":/xls-121.png"),"*.xls");
    comboBoxFileFilters->addItem(QIcon(":/xlsx.png"),"*.xlsx");
    comboBoxFileFilters->addItem(QIcon(":/zip-128.png"),"*.zip");

    toolButtonDir->setIcon(QIcon::fromTheme("system-file-manager", QIcon(":/w7-file-manager.png")));


    // Limpa os campos e define os valores padrão.
    clearReg();
}

// SLOTS
/**
 * @brief RecordDocs::closeEvent
 * @param event
 */
void RecordDocs::closeEvent(QCloseEvent *event)
{
    /* disconect o sinal antes de fecha a janela.
     * Se nao o Qt entende primeiro o sinal
     * de final de edicao, o que diz ao programa
     * para mostar mensagens na hora errada
     */
    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    // C++11 COMPLIANT
    for(QLineEdit *le : allLineEdits) {
        disconnect(le, SIGNAL(editingFinished()), this, nullptr);
    }

    /*
     * informa que a janela foi fechada.
     * emite o sinal close(false) para o
     * que o toolbar seja colocado no modo
     * invisivel
     */
    emit closed(false);
    emit deleteMenu(ModulesOfApp::ModDocuments);
    event->accept();
}

/**
 * @brief RecordDocs::mfError
 * @param m
 */
void RecordDocs::mfError(const QString& m)
{
  QMessageBox::critical(this, PROGRAM_NAME,
        tr("Houve um erro tentando ler:<br>")+filePicNameCompl+"<BR>"+m,
        QMessageBox::Close );
}

/**
 * @brief RecordDocs::dynamic_editFinished
 *      Este slot ira tratar o final da edicao dos objetos tipo QLineEdit
 *      Cada um dos objetos sera tratado de acordo com as necessidades
 *      do campo digitado em questao, a fim de suprir a facilidades para
 *      os usuarios, tais como checagem ao terminar a digitacao.
 */
void RecordDocs::dynamic_editFinished()
{
    QLineEdit *le = static_cast<QLineEdit *>(sender());

    if(!isValidChar(le->text())) {
        QMessageBox::warning(this, PROGRAM_NAME,
           tr("O caractér # é restrito ao uso interno pelo sistema. Por favor substitua-o por outro."),
           QMessageBox::Ok );
        le->setFocus();
        return;
    } else  if(le->objectName() == "lineEditOperador") {
        if( !uOperatorIsValid(le->text().simplified())) {
            QMessageBox::warning(this, PROGRAM_NAME,
                       tr("Operador não autorizado ou não informado. Verifique!"),
                       QMessageBox::Close );
            le->setFocus();
            return;
        }
    } else if(le->objectName() == "lineEditSenha" && !le->text().isEmpty()) {
        if( !uOperatorIsValid(lineEditOperador->text().simplified(), le->text().simplified()) ) {
            QMessageBox::warning(this, PROGRAM_NAME,
                       tr("Senha não pertence ao operador informado."),
                       QMessageBox::Close );
            le->setFocus();
            return;
        }
    } else if(le->objectName() == "lineEditDocValor" ) {
        /*
         * se o numero for inteiro ou nao for digitado ,00
         * o sistema coloca para o usuario
         */
        if(le->text().toInt() == brazil.toFloat(le->text())) {
            le->setText( le->text()+",00" );
        }
        if(le->text().contains('.')) {
            QMessageBox::warning(this, PROGRAM_NAME,
                          tr("Use apenas a virgula como separador "
                             "para as casas decimais.\nEx: 123,45"),
                          QMessageBox::Close );
            le->clear();
            le->setFocus();
            return;
        }
        if( brazil.toFloat(le->text()) == 0.0) {
            QMessageBox::information(this, PROGRAM_NAME,
                       tr("Você não informou um valor para este documento. "
                          "Se isso não estiver correto informe o valor antes de gravá-lo!"),
                       QMessageBox::Close );
        }
    } else if(le->objectName() == "lineEditCNPJ_CPF") {
        if(isCPNJ_OR_CPF) {
            if(uCNPJEmpty(lineEditCNPJ_CPF->text())) {
                QMessageBox::information(this, PROGRAM_NAME,
                      tr("Obrigatório informar o CNPJ!"),
                   QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
            if(!uCNPJExists(lineEditCNPJ_CPF->text())) {
                QMessageBox::information(this, PROGRAM_NAME,
                      tr("O <b>CNPJ</b> do documento informado não é o mesmo \n"
                         "da Entidade selecionada ou está errado. Por favor verifique."),
                   QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }

            if( lineEditCNPJ_CPF->text() != labelEntityIDNum->text()) {
                QMessageBox::information(this, PROGRAM_NAME,
                      tr("O <b>CNPJ</b> do documento informado não é o mesmo \n"
                         "da Entidade. Por favor verifique."),
                   QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
        } else {
            if(uCPFEmpty(lineEditCNPJ_CPF->text())) {
                QMessageBox::information(this, PROGRAM_NAME,
                      tr("Obrigatório informar o CPF!"),
                   QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
            if(!uCPFExists(lineEditCNPJ_CPF->text())) {
                QMessageBox::information(this, PROGRAM_NAME,
                      tr("O <b>CPF</b> do documento informado não é o mesmo \n"
                         "da Entidade selecionada ou está errado. Por favor verifique."),
                   QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
            if( lineEditCNPJ_CPF->text() != labelEntityIDNum->text()) {
                QMessageBox::information(this, PROGRAM_NAME,
                      tr("O <b>CPF</b> do documento informado não é o mesmo \n"
                         "da Entidade. Por favor verifique."),
                   QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
        }
    }

}

void RecordDocs::dateEditBegin_dateChanged(QDate date)
{
    if(date.isValid())
       dateBegin = date;
    else
       dateBegin = QDate::currentDate();
}

void RecordDocs::dateEditEnd_dateChanged(QDate date)
{
    if(date.isValid())
       dateEnd = date;
    else
       dateEnd = QDate::currentDate();
}

void RecordDocs::checkBoxDataTipo_toggled(bool b)
{
    dateTipo = b;
}

void RecordDocs::radioButtonCheckID_toggled(bool b) {
    lineEditCNPJ_CPF->clear();
    if(b) {
        radioButtonCheckID->setText("CNPJ");
        lineEditCNPJ_CPF->setInputMask("00.000.000/0000-00");
        lineEditCNPJ_CPF->setToolTip("CNPJ: 00.000.000/0000-00");
    } else {
        radioButtonCheckID->setText("CPF");
        lineEditCNPJ_CPF->setInputMask("000.000.000-00");
        lineEditCNPJ_CPF->setToolTip("CPF: 000.000.000-00");
    }
    isCPNJ_OR_CPF = b;
}

void RecordDocs::toolButtonCopyCNPJ_clicked(bool b)
{
   Q_UNUSED(b);
   if(!labelEntityIDNum->text().isEmpty()) {
      lineEditCNPJ_CPF->setText(labelEntityIDNum->text());
   }
}

void RecordDocs::toolButtonSave_clicked()
{
    if(comboBoxDepartamentos->currentText().isEmpty()) {
        QMessageBox::critical(this, PROGRAM_NAME,
                tr("Nome do departamento não pode ser deixado em branco"), QMessageBox::Cancel );
        comboBoxDepartamentos->setFocus();
    }
    else {        
        Department dpto;
        connect(&dpto, SIGNAL(lastError(QString)), this,
                SLOT(deptoLastError(QString)) );
        dpto.insertName(vcpString(comboBoxDepartamentos->currentText()).toStandarize());

        comboBoxDepartamentos->clear();
        comboBoxDepartamentos->addItems( dpto.fillList() );
        comboBoxDepartamentos->setCurrentIndex(0);
        disconnect(&dpto, SIGNAL(lastError(QString)));
    }
}

void RecordDocs::toolButtonDel_clicked()
{
    if(!comboBoxDepartamentos->currentText().isEmpty()) {
        Department dpto;
        connect(&dpto, SIGNAL(lastError(QString)), this,
                SLOT(deptoLastError(QString)) );
        dpto.deleteName(comboBoxDepartamentos->currentText());

        comboBoxDepartamentos->clear();
        comboBoxDepartamentos->addItems( dpto.fillList() );
        comboBoxDepartamentos->setCurrentIndex(0);
        disconnect(&dpto, SIGNAL(lastError(QString)));
    }
}

void RecordDocs::deptoLastError(const QString &e)
{
    QMessageBox::critical(this, PROGRAM_NAME,
            e, QMessageBox::Close );
}

/**
 * @brief RecordDocs::identifyEntity
 * @param idx
 */
void RecordDocs::identifyEntity(QModelIndex idx)
{
    int r = idx.row();
    if( idx.data(Qt::UserRole+1).toString() == "R") { // e RELACAO
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        qint64 ident = idx.model()->sibling(r,1,idx).data().toLongLong(); // pega a coluna 1 - #ID
        QString stmt = QString("SELECT HIGH_PRIORITY DL_IDENT, DATE_FORMAT(A.DL_DTCADAST,'%d/%m/%Y') AS DL_DTCADAST,"
                               "A.DL_OPERADOR AS DL_OPERADOR, "
                               "DATE_FORMAT(A.DL_DTDOC,'%d/%m/%Y') AS DL_DTDOC, A.DL_NUMERO AS DL_NUMERO, A.DL_VALOR AS DL_VALOR,"
                               "A.DL_DPTO AS DL_DPTO, A.DL_OBS AS DL_OBS,"
                               "A.DL_FIELD01DATA AS DL_FIELD01DATA, A.DL_FIELD02DATA AS DL_FIELD02DATA,"
                               "A.DL_FIELD03DATA AS DL_FIELD03DATA, A.DL_FIELD04DATA AS DL_FIELD04DATA,"
                               "A.DL_FIELD05DATA AS DL_FIELD05DATA, "
                               "A.DL_FNAME AS DL_FNAME, DL_TOPSEC, "
                               "(SELECT CONCAT_WS('#',B.ET_NOME,B.ET_IDENT) FROM DOCENTITY B WHERE B.ET_IDENT=A.DL_ETRELA) AS RELACAO, "
                               "(SELECT CONCAT_WS('#',B.ET_NOME,B.ET_IDENT) FROM DOCENTITY B WHERE B.ET_IDENT=A.ET_IDENT) AS ENTIDADE, "
                               "(SELECT C.IM_IMAGE FROM DOCIMAGES C WHERE C.DL_IDENT=A.DL_IDENT) AS IM_IMAGE "
                               "FROM DOCLIBRARY A WHERE A.DL_IDENT=%0" ).arg(ident);
        QSqlQueryModel qmodel;
        emit showStatusBarMessage("Executando SQL");
        qmodel.setQuery(stmt);
        if(qmodel.lastError().isValid()) {
            QMessageBox::critical(this, PROGRAM_NAME,
                   qmodel.lastError().text(), QMessageBox::Close );
            emit showStatusBarMessage("");
        } else {
            /*
             * Se o registro foi gravado com senha
             *    entao solicita a senha para desbloquea-lo
             *    senao informa que operador/senha nao conbina e nao pode
             *          desbloquear o registro
             */
            if(qmodel.record(0).value("DL_TOPSEC").toBool() ) {
                lineEditOperador->setText(qmodel.record(0).value("DL_OPERADOR").toString());

                QInputDialog *iDialog = new QInputDialog(this, Qt::Dialog);
                connect(iDialog, SIGNAL(textValueSelected(QString)), this,
                       SLOT(inputDialog_textValueSelected(QString)) );
                iDialog->setWindowTitle(tr("Desbloqueio de Registro"));
                iDialog->setCancelButtonText(tr("Cancelar"));
                iDialog->setOkButtonText(tr("Confirmar"));
                iDialog->setInputMode(QInputDialog::TextInput);
                iDialog->setLabelText(tr("Informe a senha para desbloqueio:"));
                iDialog->setTextEchoMode(QLineEdit::Password);
                int ok = iDialog->exec();

                if(ok) {
                    if(!uOperatorIsValid(lineEditOperador->text().simplified(), senhaDesbloqueio.simplified())) {
                        QMessageBox::warning(this, PROGRAM_NAME,
                                              tr("Senha e/ou nome do operador são inválidos."
                                                 "Tente novamente."), QMessageBox::Close );
                        senhaDesbloqueio="";
                        disconnect(iDialog, SIGNAL(textValueSelected(QString)), nullptr, nullptr);
                        return;
                    }
                } else { // cancel
                    disconnect(iDialog, SIGNAL(textValueSelected(QString)), nullptr, nullptr);
                    senhaDesbloqueio="";
                    return;
                }
                disconnect(iDialog, SIGNAL(textValueSelected(QString)), nullptr, nullptr);
                senhaDesbloqueio="";
            }


            labelEntity->setText(qmodel.record(0).value("ENTIDADE").toString());
            labelEntityIdentify->setText(qmodel.record(0).value("RELACAO").toString());

            lcdNumberCodigo->display(qmodel.record(0).value("DL_IDENT").toString());
            labelDate->setText(qmodel.record(0).value("DL_DTCADAST").toString());
            lineEditOperador->setText(qmodel.record(0).value("DL_OPERADOR").toString());
            comboBoxDepartamentos->setCurrentText( qmodel.record(0).value("DL_DPTO").toString() );
            lineEditDocNum->setText(qmodel.record(0).value("DL_NUMERO").toString());

            dateEditDocDate->setDate( QDate::fromString(qmodel.record(0).value("DL_DTDOC").toString(),"dd/MM/yyyy") );

            lineEditDocValor->setText( QString("%0").number(qmodel.record(0).value("DL_VALOR").toDouble(),'f',2).replace('.',',') );

            lineEditCustom0->setText(qmodel.record(0).value("DL_FIELD01DATA").toString());
            lineEditCustom1->setText(qmodel.record(0).value("DL_FIELD02DATA").toString());
            lineEditCustom2->setText(qmodel.record(0).value("DL_FIELD03DATA").toString());
            lineEditCustom3->setText(qmodel.record(0).value("DL_FIELD04DATA").toString());
            lineEditCustom4->setText(qmodel.record(0).value("DL_FIELD05DATA").toString());

            textEditHistorico->setText(qmodel.record(0).value("DL_OBS").toString());

            labelImageName->setText(qmodel.record(0).value("DL_FNAME").toString() );

            graphicsViewImages->resetTransform();
            scene = new QGraphicsScene(graphicsViewImages);
            TypeImage ti;
            if( ti.isImageViewable(qmodel.record(0).value("DL_FNAME").toString())) {
                emit showStatusBarMessage("Processando arquivo");
                QByteArray ba = qUncompress( qmodel.record(0).value("IM_IMAGE").toByteArray() );
                QPixmap pic;
                pic.loadFromData(ba);
                scene->addPixmap(pic);
                graphicsViewImages->scale(0.2,0.2 );
            } else { // nao e imagem visualizavel, entao mostra icone
                TypeImage tImg;
                scene->addPixmap(tImg.type(qmodel.record(0).value("DL_FNAME").toString()));
            }

            graphicsViewImages->setScene(scene); // mostra a imagem
            __EXPORTABLE__= true;
            emit isExportable(true);
            emit hide(NS_UTILITIES::DockFindDocs);
            QApplication::restoreOverrideCursor();
            emit showStatusBarMessage("");
        }
    }
}

/**
 * @brief RecordDocs::nameEntity
 * @param idx
 */
void RecordDocs::nameEntity(QModelIndex idx)
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
        QString s0 = sNameEntity.left( sNameEntity.indexOf("["));
        QString s1 = QString::number(uSerialEntidade(sNameEntity));

        labelEntityIdentify->setText(
                QString("%1").arg(s0+"#"+s1)
                );

        /* pega CNPJ ou CPF do nome da entidade */
        int posStart = sNameEntity.indexOf("[")+1;
        int posEnd = sNameEntity.indexOf("]");
        QString idPart(sNameEntity);
        QStringRef sub(&idPart, posStart, (posEnd-posStart) );
        labelEntityIDNum->setText(sub.toString());

        __IDENTIFY__ = false;
    }
    emit hide(NS_UTILITIES::DockEntity);
}

/**
 * @brief RecordDocs::entityIdentify_clicked
 * @param b
 */
void RecordDocs::entityIdentify_clicked(bool b)
{

    __IDENTIFY__ = b;
}

void RecordDocs::inputDialog_textValueSelected(const QString &text)
{
    senhaDesbloqueio = text;
}

void RecordDocs::inputDialog_operatorSelected(const QString &text)
{
    operatorsName = text;
}


/**
 * @brief RecordDocs::saveReg
 *
 * 2013-06-21 - A consistencia dos campos lineEditOperador e lineEditSenha
 *              sao agora efetuas pelo slot dynamic_finished()
 */
void RecordDocs::saveReg()
{
    if(NS_UTILITIES::MAXREGISTERS > 0) {
        if(totalRegs() >= NS_UTILITIES::MAXREGISTERS) {
            QMessageBox::warning(this, PROGRAM_NAME,
                                 tr("Você alcançou o limite máximo de registros permitidos"
                                    " para sua Licença de Uso.\n"
                                    "Entre em contato com o desenvolvedor para liberação"
                                    " de mais registros.")
                                , QMessageBox::Close );
            return;
        }
    }

    if(groupBoxDocValores->isChecked()) {
        if( isCPNJ_OR_CPF ) { // e CNPJ
            if(uCNPJEmpty(lineEditCNPJ_CPF->text())) {
                QMessageBox::information(this, PROGRAM_NAME,
                  tr("Obrigatório informar o CNPJ!"),
                  QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
        } else { // e CPF
            if(uCPFEmpty(lineEditCNPJ_CPF->text())) {
                QMessageBox::information(this, PROGRAM_NAME,
                  tr("Obrigatório informar o CPF!"),
                   QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
        }
     }

    if(labelEntity->text().isEmpty() || labelEntity->text().contains("Ctrl-E") ) {
        QMessageBox::warning(this , PROGRAM_NAME,
              tr("Você não escolheu uma entidade para este documento."),QMessageBox::Ok);
        return;
    }

    if( comboBoxDepartamentos->currentText().isEmpty() ) {
      QMessageBox::warning(this, PROGRAM_NAME,
             tr("Departamento não informado."),
             QMessageBox::Close );
      comboBoxDepartamentos->setFocus();
      return;
    }

    if(groupBoxDocValores->isChecked()) {
       if(lineEditDocNum->text().toInt() <= 0) {
          QMessageBox::warning(this, PROGRAM_NAME,
             tr("Número do documento não informado."),
             QMessageBox::Close );
          lineEditDocNum->setFocus();
          return;
       }
    }

    if(graphicsViewImages->items().count()==0) {
        QMessageBox::information(this, PROGRAM_NAME,
             tr("Voce nao escolheu um arquivo para ser anexado a este registro"),
             QMessageBox::Ok );
        return;
    }

    QString STMT="INSERT INTO DOCLIBRARY "
            "(DL_DTCADAST, DL_TIPO, DL_OPERADOR, ET_IDENT, DL_ETRELA, DL_DTDOC, DL_NUMERO, DL_VALOR, "
            "DL_DPTO, DL_OBS, DL_FNAME, DL_TOPSEC, DL_FIELD01DATA,DL_FIELD02DATA, "
            "DL_FIELD03DATA, DL_FIELD04DATA, DL_FIELD05DATA) VALUES (:DL_DTCADAST, :DL_TIPO, :DL_OPERADOR, "
            ":ET_IDENT, :DL_ETRELA, :DL_DTDOC, :DL_NUMERO, :DL_VALOR, :DL_DPTO, "
            ":DL_OBS, :DL_FNAME, :DL_TOPSEC, "
            ":DL_FIELD01DATA, :DL_FIELD02DATA, :DL_FIELD03DATA, :DL_FIELD04DATA, :DL_FIELD05DATA)";
    QString STMTI="INSERT INTO DOCIMAGES (DL_IDENT, IM_IMAGE) VALUES (:DL_IDENT, :IM_IMAGE)";

    QFileInfo fi(filePicNameCompl);
    QString fnameNormalized = fi.fileName().replace(" ","_");

    QSqlDatabase::database().transaction();
    QSqlQuery q;
    q.prepare(STMT);
    q.bindValue(":DL_DTCADAST", lib2.today(VCPLib2::Iso));
    q.bindValue(":DL_TIPO", (groupBoxDocValores->isChecked() ? 1 : 0) );
    q.bindValue(":DL_OPERADOR", lineEditOperador->text() );

    q.bindValue(":ET_IDENT", uSerialEntidade(labelEntity->text()) );
    if(labelEntityIdentify->text().isEmpty() )
       q.bindValue(":DL_ETRELA", uSerialEntidade(labelEntity->text()) ); // rela. padrao como entidade principal
    else
       q.bindValue(":DL_ETRELA",  uSerialEntidade(labelEntityIdentify->text()));

    q.bindValue(":DL_DTDOC", (groupBoxDocValores->isChecked() ? lib2.toISO(dateEditDocDate->text()) : "0000-00-00"));
    q.bindValue(":DL_NUMERO", lineEditDocNum->text().toInt());
    q.bindValue(":DL_VALOR", brazil.toFloat(lineEditDocValor->text()) );    //lineEditDocValor->text().toFloat());
    q.bindValue(":DL_DPTO",  vcpString(comboBoxDepartamentos->currentText()).toStandarize() );
    q.bindValue(":DL_OBS", textEditHistorico->toPlainText());
    q.bindValue(":DL_FNAME", fnameNormalized );
    q.bindValue(":DL_TOPSEC", (!lineEditSenha->text().isEmpty() ? true : false) );

    q.bindValue(":DL_FIELD01DATA", (lineEditCustom0->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom0->text()) );
    q.bindValue(":DL_FIELD02DATA", (lineEditCustom1->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom1->text()) );
    q.bindValue(":DL_FIELD03DATA", (lineEditCustom2->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom2->text()) );
    q.bindValue(":DL_FIELD04DATA", (lineEditCustom3->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom3->text()) );
    q.bindValue(":DL_FIELD05DATA", (lineEditCustom4->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom4->text()) );

    q.exec();
    if(q.lastError().isValid()) {
      QMessageBox::critical(this, PROGRAM_NAME,
             q.lastError().text()+tr("Registro não foi gravado."),
             QMessageBox::Close );
       return;
    }
    quint64 last_insert_id = q.lastInsertId().toUInt();
    QSqlDatabase::database().commit();

    if(last_insert_id == 0) {
      QMessageBox::critical(this, PROGRAM_NAME,
             tr("Registro não foi gravado.(last_insert_id)"),
             QMessageBox::Close );
       return;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    /** Le o arquivo selecionado do disco */
    QvpMultiFiles *mfInput = new QvpMultiFiles;
    connect(mfInput, SIGNAL(error(const QString&)),this,SLOT(mfError(const QString&)));
    mfInput->setInputFile(filePicNameCompl);
    emit showStatusBarMessage(tr("Lendo arquivo"));
    mfInput->execute();
    QByteArray ba;
    ba = mfInput->data();

    emit showStatusBarMessage(tr("Executando SQL"));
    QSqlDatabase::database().transaction();
    QSqlQuery qi;
    qi.prepare(STMTI);
    qi.bindValue(":DL_IDENT", last_insert_id);
    qi.bindValue(":IM_IMAGE", qCompress(ba), QSql::In | QSql::Binary);
    qi.exec();
    if(qi.lastError().isValid()) {
      QMessageBox::critical(this, PROGRAM_NAME,
             qi.lastError().text()+tr("Imagem não foi gravada."),
             QMessageBox::Close );
       QSqlDatabase::database().transaction();
       QSqlQuery del;
       del.prepare(QString("DELETE FROM DOCLIBRARY WHERE DL_IDENT=%0").arg(last_insert_id));
       del.exec();
       QSqlDatabase::database().commit();
       showStatusBarMessage("");
       QApplication::restoreOverrideCursor();
       return;
    }
    QSqlDatabase::database().commit();
    emit showStatusBarMessage("");

    // Deleta a imagem selecionada na lista
    QModelIndex cur = listViewFiles->currentIndex();
    if( cur.isValid() && !mdlThumbs->removeRows(cur.row(), 1, cur.parent()) ) {
      QMessageBox::critical(this, PROGRAM_NAME,
             tr("Erro Interno: listViewImages. Comunique o desenvolvendor."),
             QMessageBox::Close );
    }

    /* Apaga a imagem do disco se foi configurado para isso */
    if(CFG.removeImageFromDir()) {
        QDir d;
        d.remove(filePicNameCompl);
    }

    filePicNameCompl.clear();
    filePicName.clear();
    QApplication::restoreOverrideCursor();

    clearReg();
}

/**
 * @brief RecordDocs::updateReg
 */
void RecordDocs::updateReg()
{
    if(labelEntity->text().isEmpty() || labelEntity->text().contains("Ctrl-E") ) {
        QMessageBox::warning(this , PROGRAM_NAME,
              tr("Você não escolheu uma entidade para este documento."),QMessageBox::Ok);
        return;
    }

    if(groupBoxDocValores->isChecked()) {
        if( isCPNJ_OR_CPF ) { // e CNPJ
            if(uCNPJEmpty(lineEditCNPJ_CPF->text())) {
                QMessageBox::information(this, PROGRAM_NAME,
                  tr("Obrigatório informar o CNPJ!"),
                  QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
        } else { // e CPF
            if(uCPFEmpty(lineEditCNPJ_CPF->text())) {
                QMessageBox::information(this, PROGRAM_NAME,
                  tr("Obrigatório informar o CPF!"),
                   QMessageBox::Ok );
                lineEditCNPJ_CPF->setFocus();
                return;
            }
        }
     }

    if( lineEditOperador->text().isEmpty()) {
      QMessageBox::warning(this, PROGRAM_NAME,
             tr("Nome do Operador não informado."),
             QMessageBox::Close );
      lineEditOperador->setFocus();
      return;
    } else if( !uOperatorIsValid(lineEditOperador->text().simplified())) {
        QMessageBox::warning(this, PROGRAM_NAME,
               tr("Nome do Operador não foi cadastrado."),
               QMessageBox::Close );
        lineEditOperador->setFocus();
        return;
    }

    if( comboBoxDepartamentos->currentText().isEmpty() ) {
      QMessageBox::warning(this, PROGRAM_NAME,
             tr("Departamento não informado."),
             QMessageBox::Close );
      comboBoxDepartamentos->setFocus();
      return;
    }


    if(groupBoxDocValores->isChecked()) {
       if(lineEditDocNum->text().toInt() <= 0) {
          QMessageBox::warning(this, PROGRAM_NAME,
             tr("Número do documento não informado."),
             QMessageBox::Close );
          lineEditDocNum->setFocus();
          return;
       }      
    }

    SysActivity *sact = new SysActivity(this); // passar ponteiro para poder centralizar janela
    if(!sact->logger(ModulesOfApp::ModDocuments, SysActivity::Operations::Update, lcdNumberCodigo->intValue())) {
        delete sact;
        return;
    }

    QFileInfo fi(filePicNameCompl);
    QString fnameNormalized = fi.fileName().replace(" ","_");
    QString sql="UPDATE DOCLIBRARY SET DL_DTCADAST=:DL_DTCADAST, DL_ETRELA=:DL_ETRELA, "
            "DL_TIPO=:DL_TIPO, DL_OPERADOR=:DL_OPERADOR, DL_DTDOC=:DL_DTDOC, "
            "DL_NUMERO=:DL_NUMERO, DL_VALOR=:DL_VALOR, DL_DPTO=:DL_DPTO, "
            "DL_OBS=:DL_OBS";
    sql += (__IMGSELECTED__ ? ", DL_FNAME=:DL_FNAME," : ", ");
    sql += "DL_TOPSEC=:DL_TOPSEC, "
           "DL_FIELD01DATA=:DL_FIELD01DATA,DL_FIELD02DATA=:DL_FIELD02DATA, "
           "DL_FIELD03DATA=:DL_FIELD03DATA,DL_FIELD04DATA=:DL_FIELD04DATA, "
           "DL_FIELD05DATA=:DL_FIELD05DATA WHERE DL_IDENT=:DL_IDENT";

    QSqlDatabase::database().transaction();
    QSqlQuery q;
    q.prepare(sql);

    q.bindValue(":DL_DTCADAST", lib2.today(VCPLib2::Iso));
    if(labelEntityIdentify->text().isEmpty() )
       q.bindValue(":DL_ETRELA", uSerialEntidade(labelEntity->text()) ); // rela. padrao como entidade principal
    else
       q.bindValue(":DL_ETRELA",  uSerialEntidade(labelEntityIdentify->text()));

    q.bindValue(":DL_TIPO", (groupBoxDocValores->isChecked() ? 1 : 0) );
    q.bindValue(":DL_OPERADOR", lineEditOperador->text() );

    q.bindValue(":DL_DTDOC", (groupBoxDocValores->isChecked() ? lib2.toISO(dateEditDocDate->text()) : "0000-00-00"));
    q.bindValue(":DL_NUMERO", lineEditDocNum->text().toInt());
    q.bindValue(":DL_VALOR", brazil.toFloat(lineEditDocValor->text()) ); //lineEditDocValor->text().toFloat());
    q.bindValue(":DL_DPTO",  vcpString(comboBoxDepartamentos->currentText()).toStandarize() );
    q.bindValue(":DL_OBS", textEditHistorico->toPlainText());
    if(__IMGSELECTED__)
       q.bindValue(":DL_FNAME", fnameNormalized );
    q.bindValue(":DL_TOPSEC", (!lineEditSenha->text().isEmpty() ? true : false) );

    q.bindValue(":DL_IDENT", lcdNumberCodigo->intValue()); // PARTE DO WHERE

    q.bindValue(":DL_FIELD01DATA", (lineEditCustom0->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom0->text()) );
    q.bindValue(":DL_FIELD02DATA", (lineEditCustom1->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom1->text()) );
    q.bindValue(":DL_FIELD03DATA", (lineEditCustom2->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom2->text()) );
    q.bindValue(":DL_FIELD04DATA", (lineEditCustom3->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom3->text()) );
    q.bindValue(":DL_FIELD05DATA", (lineEditCustom4->placeholderText() == NS_UTILITIES::CUSTOM_NOTUSED ? nullptr : lineEditCustom4->text()) );

    q.exec();
    if(q.lastError().isValid()) {
      QMessageBox::critical(this, PROGRAM_NAME,
             q.lastError().text()+tr("Registro não foi gravado."),
             QMessageBox::Close );
       QSqlDatabase::database().rollback();
       return;
    }
    QSqlDatabase::database().commit();


    QList<QGraphicsItem*> item = graphicsViewImages->items();
    if(item.count() > 0) {        
        /*
         * Se exite uma imagem no graphicsViewImages, porem ela
         * e nova, ou seja, foi seleciona na listViewFiles
         * entao essa imagem e regravada no banco de dados.
         * Imagens que ja existem no banco da dados, ou seja
         * as que foram lidas do banco de dados pelo findDocs
         * nao sao regravadas
         */
        if(__IMGSELECTED__) {
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            emit showStatusBarMessage(tr("Lendo arquivo"));
            /** Le o arquivo selecionado do disco para atualizar nova imagem*/
            QvpMultiFiles *mfInput = new QvpMultiFiles;
            connect(mfInput, SIGNAL(error(const QString&)),this,SLOT(mfError(const QString&)));
            mfInput->setInputFile(filePicNameCompl);
            mfInput->execute();

            QByteArray ba;
            ba = mfInput->data();

            showStatusBarMessage(tr("Executando SQL"));
            QString sql="UPDATE DOCIMAGES SET IM_IMAGE=:IM_IMAGE WHERE DL_IDENT=:DL_IDENT";
            QSqlDatabase::database().transaction();
            QSqlQuery qi;
            qi.prepare(sql);
            qi.bindValue(":IM_IMAGE", qCompress(ba,9), QSql::In | QSql::Binary );
            qi.bindValue(":DL_IDENT", lcdNumberCodigo->intValue()); // PARTE DO WHERE
            qi.exec();
            if(qi.lastError().isValid()) {
                QMessageBox::critical(this, PROGRAM_NAME,
                        q.lastError().text()+tr("Registro não foi gravado."),
                        QMessageBox::Close );
                QSqlDatabase::database().rollback();
                __IMGSELECTED__ = false;
                emit showStatusBarMessage("");
                QApplication::restoreOverrideCursor();
                return;
            }
            QSqlDatabase::database().commit();
            // Deleta a imagem selecionada na lista
            QModelIndex cur = listViewFiles->currentIndex();
            if(cur.isValid() && !mdlThumbs->removeRows(cur.row(), 1, cur.parent()) ) {
                QMessageBox::critical(this, PROGRAM_NAME,
                         tr("Erro Interno: listViewImages. Comunique o desenvolvendor."),
                         QMessageBox::Close );
            }

            /* Apaga arquivo do disco */
            if(CFG.removeImageFromDir()) {
                QDir d;
                d.remove(filePicNameCompl);
            }
            emit showStatusBarMessage("");
            QApplication::restoreOverrideCursor();
        }
        __IMGSELECTED__ = false;
    }

    filePicNameCompl.clear();
    filePicName.clear();
}


/**
 * @brief RecordDocs::deleteReg
 */
void RecordDocs::deleteReg()
{
    if(scene) {
        QList<QGraphicsItem*> item = graphicsViewImages->items();
        if(item.count() ==0) {
            QMessageBox::warning(this, PROGRAM_NAME,
                         tr("Nenhuma imagem foi selecionada."),
                         QMessageBox::Close );
        } else {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle( PROGRAM_NAME);
            messageBox.setText(tr("Este procedimento é irreversível!\nVocê confirma a operação?"));
            QAbstractButton *cancelButton =
                  messageBox.addButton(tr("Cancelar"), QMessageBox::ActionRole);
            QAbstractButton *continueButton =
                  messageBox.addButton(tr("Prossegir"), QMessageBox::ActionRole);
            messageBox.setEscapeButton(cancelButton);

            messageBox.exec();
            if(messageBox.clickedButton() == continueButton) {
                SysActivity *sact = new SysActivity(this); // passar ponteiro para poder centralizar janela              
                if(!sact->logger(ModulesOfApp::ModDocuments, SysActivity::Operations::Delete, lcdNumberCodigo->intValue())) {
                    delete sact;
                    return;
                }

                QString STMT=QString("DELETE FROM DOCLIBRARY WHERE DL_IDENT=%0").arg(lcdNumberCodigo->intValue());
                QSqlQuery q;
                q.prepare(STMT);
                q.exec();
                if( q.numRowsAffected() == 0) {
                    QMessageBox::critical(this, PROGRAM_NAME,
                             tr("Registro não foi apagado. "
                                "Provavelmente ele não exista no banco de dados"),
                            QMessageBox::Close );
                    return;
                }
                if(q.lastError().isValid()) {
                    QMessageBox::critical(this, PROGRAM_NAME,
                             q.lastError().text()+tr("Registro não foi apagado."),
                            QMessageBox::Close );
                } else {
                    QMessageBox::information(this, PROGRAM_NAME,
                                            tr("Registro apagado com sucesso."),
                                            QMessageBox::Close );                        
                }
            }
         }
    }
}

/**
 * @brief RecordDocs::clearReg
 */
void RecordDocs::clearReg()
{
    lcdNumberCodigo->display(QString::number(lastPgID()));
    labelDate->setText(lib2.today());

    if(CFG.keepEntity() ) {
        if(__FIRSTRUN__) {
            labelEntity->setText(tr("Ctrl-E selecionar Entidade ou Ctrl-F pesquisa por documento"));
            labelEntityIdentify->clear();
        }
    } else {
        labelEntity->setText(tr("Ctrl-E selecionar Entidade ou Ctrl-F pesquisa por documento"));
        labelEntityIdentify->clear();
    }
    __FIRSTRUN__ = false;

    lineEditOperador->clear();
    lineEditSenha->clear();

    Department dpto;
    comboBoxDepartamentos->clear();
    comboBoxDepartamentos->addItems( dpto.fillList() );
    comboBoxDepartamentos->setCurrentIndex(0);
    comboBoxDepartamentos->setLineEdit(lineEditDeptos);

    groupBoxDocValores->setChecked(true);
    lineEditDocNum->clear();
    dateEditDocDate->setDate( QDate::currentDate() );
    lineEditDocValor->clear();
    lineEditCNPJ_CPF->clear();


    lineEditCustom0->clear();
    lineEditCustom1->clear();
    lineEditCustom2->clear();
    lineEditCustom3->clear();
    lineEditCustom4->clear();

    textEditHistorico->clear();

    QList<QGraphicsItem*> item = graphicsViewImages->items();
    qDeleteAll(item);

    labelImageName->clear();
}


/**
 * @brief RecordDocs::zoomIn
 */
void RecordDocs::zoomIn()
{
    if(__ISVIEWABLE__)
        uZoomImage(NS_UTILITIES::zIn, graphicsViewImages);
}

/**
 * @brief RecordDocs::zoomOut
 */
void RecordDocs::zoomOut()
{
    if(__ISVIEWABLE__)
        uZoomImage(NS_UTILITIES::zOut, graphicsViewImages);
}

/**
 * @brief RecordDocs::imageRotate
 */
void RecordDocs::imageRotate()
{
    if(__ISVIEWABLE__)
        uRotate(graphicsViewImages, 90.0);
}

/**
 * @brief RecordDocs::comboBoxFileFilters_currentTextChanged
 * @param text
 */
void RecordDocs::comboBoxFileFilters_currentTextChanged(QString text)
{
    if(text.length()<3) {
        return;
    } else if(text.contains("\\") ) {
        QMessageBox::warning(this, PROGRAM_NAME,
                 tr("Caractér Inválido: \\ <br>Exemplo: imagens/*" ),
                QMessageBox::Close );
            return;
    }
    criaListaImagens();
}

void RecordDocs::toolButtonDir_clicked()
{
    QString pathImages = QFileDialog::getExistingDirectory(this,
        tr("Imagens"), CFG.getRootDir() );
    QString aux = pathImages;
    comboBoxFileFilters->setCurrentText(aux+"/*");
    criaListaImagens();
}

/**
 * @brief RecordDocs::toolButtonFileFilters_clicked
 */
void RecordDocs::toolButtonFileFilters_clicked()
{
    criaListaImagens();
}

// FUNCTIONS
/**
 * @brief RecordDocs::lastPgID
 * Retorna um inteiro sem sinal de 64bits contendo o DL_IDENT maximo+1
 * @return inteiro contendo o valor maximo do DL_IDENT+1
 */
quint64 RecordDocs::lastPgID()
{
   QSqlQuery q;
   q.prepare("SELECT MAX(DL_IDENT) FROM DOCLIBRARY");
   q.exec();
   q.next();
   return( (q.value(0).toLongLong()==0 ? 1 : q.value(0).toLongLong()+1) );
}

/**
 * @brief Retorna um inteiro sem sinal de 64bits contendo o total de registros
 *        armazenados na tabela
 * @return inteiro contendo o total de registros COUNT()
 */
qint64 RecordDocs::totalRegs()
{
   QSqlQuery q;
   q.prepare("SELECT COUNT(*) FROM DOCLIBRARY");
   q.exec();
   q.next();
   return( q.value(0).toLongLong() );
}


/**
 * @brief RecordDocs::criaListaImagens
 */
void RecordDocs::criaListaImagens()
{
    mdlThumbs->clear();

    QStringList banished = {"scr", "reg", "pif",
                            "crt", "cpl", "bas",
                            "chm", "cmd", "com",
                            "prf", "scr"
                           };

    QDir dir;
    if(CFG.dirImageDefault()) {
        dir.setCurrent(CFG.getRootDir());
    } else {
        dir.setCurrent(comboBoxFileFilters->currentText().remove("*"));
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot );
    dir.setSorting(QDir::Name);  
    if(comboBoxFileFilters->currentText() !="*" || comboBoxFileFilters->currentText() != "*.*" ||
            !comboBoxFileFilters->currentText().isEmpty() ) {
        int sep = comboBoxFileFilters->currentText().count("/");
        if(sep > 0) {
            dir.setNameFilters(QStringList() << comboBoxFileFilters->currentText().section("/",sep));
        } else {
            dir.setNameFilters(QStringList() << comboBoxFileFilters->currentText());
        }
    }
    QFileInfoList list = dir.entryInfoList();

    if(list.size() <= 0)
        return;

    qint32 max=0;
    for(int i = 0; i < list.size(); ++i) {
       QFileInfo fileInfo = list.at(i);
       ++max;
    }

    QStringList files;
    for(int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.isDir())
           continue;

        // Ignora possiveis arquivos temporarios
        if(fileInfo.fileName().startsWith("~") || fileInfo.fileName().endsWith("$$$")
                || fileInfo.fileName().startsWith(".")
                || fileInfo.fileName().toLower().endsWith("tmp"))
            continue;

        // Ignora arquvos com extensoes do tipo .~xx ou seja
        // que possuem o ~ na extensao. Possivel temporario
        if(fileInfo.suffix().contains("~") )
            continue;

        // Ignora arquivo com tamanho zero
        if(fileInfo.size()==0)
            continue;

        // Se o arquivos possuir mais que um unico ponto no nome
        // deve ser ignorado
        if(fileInfo.fileName().count(".") > 1) {            
           continue;
        }

        // tamanha max do nome do arquivo incluindo a extensao
        if(fileInfo.fileName().size()>254) {
            continue;
        }

        // possiveis virus
        if( banished.contains(fileInfo.suffix().toLower()) )
            continue;

        QString x = fileInfo.absolutePath()+"/"+fileInfo.fileName();
        files << x;
    }

    imagesWatcher *images = new imagesWatcher(this);
    // Cancel and wait if we are already loading images.
    if(images->isRunning()) {
       images->cancel();
       images->waitForFinished();
    }

    if(files.count() == 0)
       return;

    // Lanca a thread ==========================================================
    QProgressDialog progress(tr("Carregando Imagens ... "), tr("Cancelar"), 0, images->progressMaximum());
    connect(images, SIGNAL(finished()), &progress, SLOT(reset()) );
    connect(images, SIGNAL(canceled()), &progress, SLOT(cancel()) );
    connect(images, SIGNAL(progressRangeChanged(int,int)), &progress, SLOT(setRange(int,int)) );
    connect(images, SIGNAL(progressValueChanged(int)), &progress, SLOT(setValue(int)) );

    QFuture< QPair<QImage,QString> > result = QtConcurrent::mapped(files, uLoadImages);
    images->setFuture(result);

    //==========================================================================
    progress.exec();

    int row=0;
    for(int i=0; i<= files.size()-1;++i) {
        if(files.at(i).isEmpty() )
           break;
        QFileInfo fi(files.at(i));
        QStandardItem *item = new QStandardItem;
        item->setIcon(QPixmap::fromImage(images->resultAt(i).first));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignBottom);
        item->setText(fi.fileName());

        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setData(files.at(i), Qt::UserRole); // seta o path completo do arquivo para uso em showPixmap()
        item->setData(images->resultAt(i).second, Qt::UserRole+1); // set o tipo do arquivo
        item->setData(fi.size(), Qt::UserRole+1); // seta o tamanho do arquivo
        mdlThumbs->setItem(row, 0, item);
        ++row;
    }
    listViewFiles->setModel(mdlThumbs);
    listViewFiles->show();
}

/**
 * @brief RecordDocs::listViewFiles_clicked
 *        Seleciona uma imagem via click
 *        seta o flag __IMGSELECTED__ para true
 * @param idx
 */
void RecordDocs::listViewFiles_clicked(QModelIndex idx)
{

    /* Mostra o nome do arquivo selecionado juntamente com
     * seu tamanho
     */
    QString textIEC, sizeFormated;
    uToIEC(idx.data(Qt::UserRole+1).toReal(), textIEC, sizeFormated );
    QString sFN = QString("%0\t Tam:%1 (%2)").
           arg(idx.data().toString()).
           arg(textIEC).
           arg(sizeFormated.simplified());
    if(idx.data().toString().length() > 100)
        labelFileNames->setText( idx.data().toString() );
    else
        labelFileNames->setText( sFN );
    /* ---------------------------------------------------------------- */

    QDir d;
    filePicNameCompl = d.filePath(idx.data(Qt::UserRole).toString());

    QFileInfo fi(filePicNameCompl);
    QImage img;

    scene = new QGraphicsScene(graphicsViewImages);
    graphicsViewImages->resetTransform();

    TypeImage ti;
    if(ti.isImageViewable(filePicNameCompl)) {
       QImageReader reader(filePicNameCompl);
       reader.read(&img);
       QPixmap pixTmp(QPixmap::fromImage(img));
       scene->addPixmap(pixTmp);
       graphicsViewImages->scale(0.2,0.2);
       __ISVIEWABLE__ = true;
    } else {
       QPixmap pixTmp( ti.type(filePicNameCompl) );
       pixTmp.scaled(QSize(64,64));
       scene->addPixmap(pixTmp);
       __ISVIEWABLE__ = false;
    }

    graphicsViewImages->setScene(scene);
    __IMGSELECTED__ = true;
}

/**
 * @brief RecordDocs::listViewFiles_doubleClicked
 * @param idx
 */
void RecordDocs::listViewFiles_doubleClicked(QModelIndex idx)
{
    QDir d;
    QFileInfo info(idx.data(Qt::UserRole).toString());

    if(isExecutable(idx.data(Qt::UserRole).toString())) {
        QMessageBox::warning( this , PROGRAM_NAME,
          tr("Para sua segurança o qDocBase2 não permite a execução direta de programas.\n\n"),QMessageBox::Ok);
        return;
    }

    QString outFile = d.filePath(idx.data(Qt::UserRole).toString());
    if(outFile.isNull() || outFile.isEmpty())
       return;

#ifdef Q_OS_LINUX
    if( !QDesktopServices::openUrl(QUrl("file://"+idx.data(Qt::UserRole).toString(), QUrl::TolerantMode)) ) {
        QMessageBox::critical( this , PROGRAM_NAME,
          tr("Não foi possível abri o arquivo. Talvez não haja um aplicativo "
             "capaz de abri-lo instalado no seu computador.\n\n"),QMessageBox::Ok);
    }
#else
    if( !QDesktopServices::openUrl(QUrl("file:///"+idx.data(Qt::UserRole).toString(), QUrl::TolerantMode)) ) {
        QMessageBox::critical( this , PROGRAM_NAME,
          tr("Não foi possível abri o arquivo. Talvez não haja um aplicativo "
             "capaz de abri-lo instalado no seu computador.\n\n"),QMessageBox::Ok);
    }
#endif
}

/**
 * @brief RecordDocs::listViewFiles_entered
 * @param idx
 */
void RecordDocs::listViewFiles_entered(QModelIndex idx)
{
    listViewFiles->setCurrentIndex(idx);

    QString sSize;
    labelFileNames->setText( idx.data().toString() );
}

/**
 * @brief RecordDocs::documentExport
 */
void RecordDocs::documentExport()
{
    if(!__EXPORTABLE__) {
        QMessageBox::warning(this , PROGRAM_NAME,
                tr("Somente imagens que foram recuperadas do banco"
                   "de dados poderão ser exportadas."),QMessageBox::Close);
        return;
    }

    QString direxpo = CFG.getExportDir();

    QMessageBox msgBox;
    msgBox.setText(tr("Você está prestes a exportar documentos para %0").arg(direxpo));
    msgBox.setInformativeText(tr("Você confirma esta operação?"));
    QPushButton *abortButton = msgBox.addButton(tr("Abortar"), QMessageBox::ActionRole);
    QPushButton *okButton = msgBox.addButton(tr("Confirmar"), QMessageBox::ActionRole);
    Q_UNUSED(okButton);
    msgBox.setDefaultButton(abortButton);
    msgBox.exec();
    if(msgBox.clickedButton() == abortButton)
        return;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    emit showStatusBarMessage(tr("Exportando ..."));
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare( QString("SELECT HIGH_PRIORITY IM_IMAGE FROM DOCIMAGES WHERE DL_IDENT=%0").arg(lcdNumberCodigo->value()) );
    query.exec();
    query.next();
    if( query.lastError().isValid()) {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this , PROGRAM_NAME,
              query.lastError().text()+tr("Não encontrou a imagem"),QMessageBox::Close);
          QSqlDatabase::database().rollback();
          emit showStatusBarMessage("");
    } else {
        // descompacta arquivos via thread
        QFuture<QByteArray> future = QtConcurrent::run(qUncompress,query.value(0).toByteArray());
        QByteArray ba = future.result();

        // Salva os arquivos no disco
        QvpMultiFiles mf;
        mf.setOutputFile( QString("%0/%1").arg(direxpo).arg(labelImageName->text().simplified()) );
        mf.setData(ba);
        mf.execute();
    }
    QSqlDatabase::database().commit();
    __EXPORTABLE__ = false;
    emit isExportable(false);
    QApplication::restoreOverrideCursor();
    emit showStatusBarMessage("");
}

void RecordDocs::printOneImageReg()
{
    if(labelEntity->text().isEmpty() || labelEntity->text().contains("Ctrl-E")) {
        QMessageBox::warning(this , PROGRAM_NAME,
              tr("Use Ctrl-E e selecione ao menos uma Entidade"),QMessageBox::Ok);
        return;
    }

    TypeImage ti;
    if( ti.isImageViewable(labelImageName->text().simplified())) {
        PrintImage print;
        print.setStringList(QStringList() << QString::number(lcdNumberCodigo->value()),
                        "SELECT HIGH_PRIORITY IM_IMAGE FROM DOCIMAGES WHERE DL_IDENT=");
        print.preview();
    } else
        QMessageBox::warning(this , PROGRAM_NAME,
              tr("Arquivo selecionado não é pode ser impresso"),QMessageBox::Ok);

}

void RecordDocs::printImagesReg()
{
    if(labelEntity->text().isEmpty() || labelEntity->text().contains("Ctrl-E") ) {
        QMessageBox::warning(this , PROGRAM_NAME,
              tr("Use Ctrl-E e selecione ao menos uma Entidade"),QMessageBox::Ok);
        return;
    }

    qint64 dlIdent = labelEntity->text().section("#",1,1).toLongLong(); // principal


    // monta logica para selecionar registros protegidos
    QString unblock;
    if(!lineEditOperador->text().isEmpty() && !lineEditSenha->text().isEmpty()) {
        if(uOperatorIsValid(lineEditOperador->text(), lineEditSenha->text()))
           unblock = QString("A.DL_OPERADOR='%0'").arg(lineEditOperador->text().simplified());
        else
           unblock = "A.DL_TOPSEC=0";
    } else
        unblock = "A.DL_TOPSEC=0";

    // Monta logica para selecionar apenas arquivos imprimiveis
    QString aux;
    TypeImage ti;
    foreach(QString s, ti.stringListViewable()) {
        aux += QString("RIGHT(A.DL_FNAME,3)='%0' OR ").arg(s);
    }
    aux.remove(aux.length()-3,4);

    QString stmt=QString("SELECT HIGH_PRIORITY A.DL_FNAME, B.IM_IMAGE FROM DOCLIBRARY A, DOCIMAGES B "
                         "WHERE %0 AND "
                         "%1 BETWEEN '%2' AND '%3' AND "
                         "B.DL_IDENT=A.DL_IDENT AND "
                         "A.ET_IDENT=%4 AND %5 "
                         "ORDER BY %6").
            arg( unblock ).
            arg( (dateTipo ? "A.DL_DTCADAST" : "A.DL_DTDOC") ).
            arg( dateBegin.toString("yyyy-MM-dd")).
            arg( dateEnd.toString("yyyy-MM-dd") ).
            arg( dlIdent ).
            arg( aux ).
            arg( (dateTipo ? "A.DL_DTCADAST" : "A.DL_DTDOC") );

    PrintSelectedImages print;
    connect( &print, SIGNAL(message(const QString&)), this,
             SLOT(printMessage(const QString &)) );
    connect( &print, SIGNAL(lastError(const QString&)), this,
             SLOT(printLastError(const QString &)) );

    print.setQueryModel(stmt);
    if(print.rowsCount() > 200) {
        QMessageBox m;
        m.setText(tr("Esta consulta irá gerar um relatório com %0 páginas").arg(print.rowsCount()));
        m.setInformativeText( tr("É aconselhável cancelar e gerar novamente em partes menores.") );
        m.setWindowTitle(tr("Impressão de Relatórios"));
        QPushButton *canceltButton = m.addButton( tr("Cancelar"), QMessageBox::ActionRole);
        QPushButton *okButton = m.addButton(tr("Continuar"), QMessageBox::ActionRole);
        Q_UNUSED(okButton);
        m.setDefaultButton(canceltButton);
        m.exec();
        if (m.clickedButton() == canceltButton) {
            return;
        }
    }
    print.setTitle( PROGRAM_NAME );
    print.setSubTitle(tr("Listagem de Documentos: %0").arg( labelEntity->text().section("#",0,0)) );
    print.preview();
    disconnect(&print, SIGNAL(message(const QString)), nullptr, nullptr);
    disconnect(&print, SIGNAL(lastError(const QString)), nullptr, nullptr);
}

void RecordDocs::printMessage(const QString &m)
{
    emit showStatusBarMessage(m);
}

void RecordDocs::printLastError(const QString &e)
{
    QMessageBox::warning(this , PROGRAM_NAME,
          e,QMessageBox::Cancel);
}

void RecordDocs::printDocGeneral()
{
    if(labelEntity->text().isEmpty() || labelEntity->text().contains("Ctrl-E") ) {
        QMessageBox::warning(this , PROGRAM_NAME,
              tr("Use Ctrl-E e selecione ao menos uma Entidade"),QMessageBox::Ok);
        return;
    }

    QString stmt = QString("SELECT HIGH_PRIORITY "
                           "DL_IDENT, DATE_FORMAT(DL_DTCADAST,'%d/%m/%Y') AS DL_DTCADAST, "
                           "DL_OPERADOR, DL_DPTO, DATE_FORMAT(DL_DTDOC,'%d/%m/%Y') AS DL_DTDOC, "
                           "DL_NUMERO, DL_VALOR, DL_FNAME, "
                           "(SELECT ET_NOME FROM DOCENTITY A WHERE A.ET_IDENT=B.ET_IDENT) AS ENTIDADE, "
                           "(SELECT ET_NOME FROM DOCENTITY A WHERE A.ET_IDENT=B.DL_ETRELA) AS RELACAO "
                           "FROM DOCLIBRARY B WHERE ET_IDENT=%0").arg(labelEntity->text().section('#',1,1).toLongLong());
    PrintDocGeneral print;
    print.setQueryModel(stmt);
    print.setTitle( PROGRAM_NAME );
    print.setSubTitle(tr("Listagem Geral do Cadastro de Documentos"));
    print.preview();
}

void RecordDocs::printDocAnalitic()
{
    if(labelEntity->text().isEmpty() || labelEntity->text().contains("Ctrl-E")) {
        QMessageBox::warning(this , PROGRAM_NAME,
              tr("Use Ctrl-E e selecione ao menos uma Entidade"),QMessageBox::Ok);
        return;
    } else {
        if(labelEntityIdentify->text().isEmpty())
            labelEntityIdentify->setText( labelEntity->text() );
    }

    PrintDocAnalitic print;

    QString stmt;
    QString sqlWhere;

    sqlWhere = QString("AND %0 BETWEEN '%1' AND '%2'").
            arg( (dateTipo ? "DL_DTCADAST" : "DL_DTDOC") ).
            arg(dateBegin.toString("yyyy-MM-dd")).
            arg(dateEnd.toString("yyyy-MM-dd"));
    stmt = QString("SELECT HIGH_PRIORITY DL_IDENT, DATE_FORMAT(DL_DTCADAST,'%d/%m/%Y') AS DL_DTCADAST, "
                   "DL_OPERADOR, DL_DPTO, DATE_FORMAT(DL_DTDOC,'%d/%m/%Y') AS DL_DTDOC, "
                   "DL_NUMERO, DL_VALOR, DL_ARQUIVO, DL_FNAME, "
                   "(SELECT ET_NOME FROM  DOCENTITY A WHERE A.ET_IDENT=B.ET_IDENT) AS ENTIDADE, "
                   "(SELECT ET_NOME FROM  DOCENTITY A WHERE A.ET_IDENT=B.DL_ETRELA) AS RELACAO, "
                   "DL_ETRELA, ET_IDENT FROM DOCLIBRARY B "
                   "WHERE ET_IDENT=%0 AND DL_ETRELA=%1 AND DL_VALOR > 0 %2 GROUP BY DL_ETRELA ORDER BY DL_IDENT").
            arg( labelEntity->text().section('#',1,1) ).
            arg( labelEntityIdentify->text().section('#',1,1) ).
            arg(sqlWhere);

    print.setQueryModel(stmt);
    print.setSqlWhere(sqlWhere);
    print.setTitle( PROGRAM_NAME );
    print.setSubTitle(tr("Listagem Analítica de Documentos"));
    print.preview();
}
