/***************************************************************************
 * Copyright (c) 2007-2020                                                 *
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

#include "recordentity.h"

RecordEntity::RecordEntity(QWidget *parent) :
    QWidget(parent),
    iClasse(-1)
{
    setupUi(this);

    connect(radioButtonEmpresa, SIGNAL(toggled(bool)),
            this, SLOT(radioButtonEmpresa_toggled(bool)) );
    connect(radioButtonCliente, SIGNAL(toggled(bool)),
            this, SLOT(radioButtonCliente_toggled(bool)) );
    connect(radioButtonFornecedor, SIGNAL(toggled(bool)),
            this, SLOT(radioButtonFornecedor_toggled(bool)) );
    connect(radioButtonOutros, SIGNAL(toggled(bool)),
            this, SLOT(radioButtonOutros_toggled(bool)) );
    connect(radioButtonProjetos, SIGNAL(toggled(bool)),
            this, SLOT(radioButtonProjetos_toggled(bool)) );
    connect(radioButtonSimples, SIGNAL(toggled(bool)),
            this, SLOT(radioButtonSimples_toggled(bool)) );
    connect(checkBoxStatus, SIGNAL(toggled(bool)), this,
            SLOT(checkBoxStatus_toggled(bool)) );

    // configura para todos os campos somente receberem letras
    // maiusculas e caracteres validos
    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    foreach(QLineEdit *le, allLineEdits) {
        connect(le, SIGNAL(editingFinished()), this,
                SLOT(dynamic_editFinished()) );

        // nao seta UpperValidator
        if(le->objectName() == "lineEditEmail1" || le->objectName() == "lineEditEmail2" ||
            le->objectName() == "lineEditURL") {
            continue;
        }

        le->setValidator(new vcpUpperValidator);
    }

    /* padroes */
    tabWidgetModelos->setCurrentIndex(0);

    checkBoxStatus->setIcon( QPixmap(":/grndiamd.png") );
    radioButtonEmpresa->setChecked(true);
    controlTabModelos(0,true);


    lineEditObraCliente = new QLineEdit;
    lineEditObraCliente->setReadOnly(true);
    lineEditObraCliente->setPlaceholderText(tr("Nome do Cliente"));

    // artificio para pode colocar um "placeholder" no comboBox
    comboBoxObraCliente->setLineEdit( lineEditObraCliente );
    comboBoxObraCliente->setEnabled(true);

    uFillCBEntidade(comboBoxObraCliente, NS_UTILITIES::EnCliente, 1);

    spinBoxbObraCrono->setValue(0);

    lcdNumberCodigo->display(QString::number(lastPgID()));
    labelDateReg->setText( lib2.today(VCPLib2::Eur) );

    lineEditObraCod->setText(lastObraID());

    lineEditNome->setFocus();
    /* fim-padroes */
}


/* $SLOTS */
/**
 * @brief RecordEntity::closeEvent
 * @param event
 */
void RecordEntity::closeEvent(QCloseEvent *event)
{
    /* disconect o sinal antes de fecha a janela.
     * Senao o Qt entende primeiro o sinal
     * de final de edicao, o que diz ao programa
     * para mostar mensagens na hora errada
     */
    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    foreach(QLineEdit *le, allLineEdits) {
        disconnect(le, SIGNAL(editingFinished()), this, nullptr);
    }
    /*
     * informa que a janela foi fechada.
     * emite o sinal close(false) para o
     * que o toolbar seja colocado no modo
     * invisivel
     */
    emit closed(false);
    emit deleteMenu(ModulesOfApp::ModEntity);
    event->accept();
}

/**
 * @brief RecordEntity::editFinished
 *          Dinamicamente efetua testes de consistencia
 *          nos campos, ao final da edicao pelo usuario.
 */
void RecordEntity::dynamic_editFinished()
{
    QLineEdit *le = static_cast<QLineEdit*>(sender()); // determina qual o objeto correto que esta enviando o sinal
    if(!isValidChar(le->text())) {
        QMessageBox::warning(this, PROGRAM_NAME,
           tr("O caractér # é restrito ao uso interno pelo "
              "sistema. Por favor substitua-o por outro."),
           QMessageBox::Ok );
        le->setFocus();
    }
    if(le->objectName() == "lineEditContato1" || le->objectName() == "lineEditContato2") {
        if(le->text().contains("@")) {
            QMessageBox::information(this, PROGRAM_NAME,
                  tr("Foi detectado que você digitou o caractér <b>@</b>, o que pode indicar "
                     "que você esta tentando cadastrar um endereço de e-mail. "
                     "Use os campos Email os quais são destinados à este fim."),
               QMessageBox::Ok );
            le->clear();
        }
    }

    if(le->objectName() == "lineEditNome") {
        if(le->text().isEmpty()) {
            controlObjects(false);  // se vazio desativa todos os demais campos para edicao
            le->setFocus();
            return;
        } else {
            controlObjects(true); // se foi preenchido ativa os demais campos para edicao
            return;
        }
        if(le->text().size() < 3) {
                QMessageBox::information(this, PROGRAM_NAME,
                      tr("Você está tentando informar um Nome de Entidade \n"
                         "com menos de 03 caractéres, e isso\n"
                         "pode não estar correto. Por favor verifique."),
                   QMessageBox::Ok );
        }
    }

    if(le->objectName() == "lineEditCity") {
        if(le->text().isEmpty()) {
            le->setFocus();
            return;
        } else {
            if(le->text().size() < 3) {
                QMessageBox::information(this, PROGRAM_NAME,
                      tr("Você está tentando informar um Nome de Cidade \n"
                         "com menos de 03 caractéres, e isso\n"
                         "pode não estar correto. Por favor verifique."),
                   QMessageBox::Ok );
            }
        }
    }


    if(le->objectName() != "lineEditCEP") { // CEP nao considerar
        vcpString s(le->text());
        if(s.isReplicate(le->maxLength())) {
            QMessageBox::warning(this, PROGRAM_NAME,
              tr("Há muitos caractéres repetidos neste campo.\n"
                 "Isso pode indicar que não se trata de uma informação válida."),
           QMessageBox::Ok );
            le->setFocus();
        }
    }
}

/**
 * @brief RecordEntity::nameEntity
 * @param idx
 */
void RecordEntity::nameEntity(QModelIndex idx)
{

    sTypeEntity = sNameEntity = idx.data().toString();

    /*
     * Se foi clicado sobre o nome da classe
     * entao desconsiderar a escolha do usuario
     *
     * C++11 compliant
     */
    for(const QString &s : uClassesOfEntities()) {
        if(s==sTypeEntity) {
            sNameEntity = "";
            iTypeEntity = -1;
            return;
        }
    }

    // recupera os dados da entidade e mostra na janela

    QString stmt=QString("SELECT ET_IDENT, DATE_FORMAT(ET_DATA,'%d/%m/%Y') AS ET_DATA , ET_ATIVO, ET_CATEG, "
            "ET_NOME, ET_ALIAS, ET_ENDER, ET_BAIR, ET_CITY, ET_UF, ET_CEP, "
            "ET_TELMOD1, ET_TELNUM1, ET_TELMOD2, ET_TELNUM2, "
            "ET_TELMOD3, ET_TELNUM3, ET_TELMOD4, ET_TELNUM4, "
            "ET_CONTATO1, ET_EMAIL1, ET_CONTATO2, ET_EMAIL2, "
            "ET_CPF, ET_RG, ET_CNPJ, ET_IE, ET_IM, ET_URL, "
            "ET_OBCOD, ET_OBANO, "
            "(SELECT CONCAT_WS('#', B.ET_NOME, B.ET_IDENT) FROM DOCENTITY B WHERE B.ET_IDENT=A.ET_OBCLI) AS ET_OBCLI, "
            "ET_OBCEI, ET_OBCRONO, ET_OUTESPEC, ET_OUTNFLD1, "
            "ET_OUTDFLD1, ET_OUTNFLD2, ET_OUTDFLD2, ET_OUTOBS, ET_OBSGER "
            "FROM DOCENTITY A WHERE ET_IDENT=%0").arg(sNameEntity.section('#',1,1));

    QSqlQueryModel qmodel;
    qmodel.setQuery(stmt);
    if(qmodel.lastError().isValid()) {
        QMessageBox::critical(this, PROGRAM_NAME,
               qmodel.lastError().text(), QMessageBox::Close );
    } else {
        lcdNumberCodigo->display(qmodel.record(0).value("ET_IDENT").toString() );
        labelDateReg->setText(qmodel.record(0).value("ET_DATA").toString());
        checkBoxStatus->setChecked( (qmodel.record(0).value("ET_ATIVO").toBool() ? true : false) );
        checkBoxStatus->setIcon(
                    (qmodel.record(0).value("ET_ATIVO").toBool() ? QPixmap(":/grndiamd.png") : QPixmap(":/reddiamd.png")) );
        switch(qmodel.record(0).value("ET_CATEG").toInt()) {
        case NS_UTILITIES::EnEmpresa:
            radioButtonEmpresa->setChecked(true);
            break;
        case NS_UTILITIES::EnCliente:
            radioButtonCliente->setChecked(true);
            break;
        case NS_UTILITIES::EnFornecedor:
            radioButtonFornecedor->setChecked(true);
            break;
        case NS_UTILITIES::EnProjObras:
            radioButtonProjetos->setChecked(true);
            break;
        case NS_UTILITIES::EnOutros:
            radioButtonOutros->setChecked(true);
            break;
        case NS_UTILITIES::EnTipoSimples:
            radioButtonSimples->setChecked(true);
            break;
        default:
            Q_UNREACHABLE();
        }

        lineEditNome->setText(qmodel.record(0).value("ET_NOME").toString());
        lineEditAlias->setText(qmodel.record(0).value("ET_ALIAS").toString());
        lineEditEnder->setText(qmodel.record(0).value("ET_ENDER").toString());
        lineEditBair->setText(qmodel.record(0).value("ET_BAIR").toString());
        lineEditCity->setText(qmodel.record(0).value("ET_CITY").toString());
        comboBoxUF->setCurrentText(qmodel.record(0).value("ET_UF").toString());
        lineEditCEP->setText(qmodel.record(0).value("ET_CEP").toString());

        comboBoxTel1->setCurrentText(qmodel.record(0).value("ET_TELMOD1").toString());
        lineEditTelNum1->setText(qmodel.record(0).value("ET_TELNUM1").toString());
        comboBoxTel2->setCurrentText(qmodel.record(0).value("ET_TELMOD2").toString());
        lineEditTelNum2->setText(qmodel.record(0).value("ET_TELNUM2").toString());
        comboBoxTel3->setCurrentText(qmodel.record(0).value("ET_TELMOD3").toString());
        lineEditTelNum3->setText(qmodel.record(0).value("ET_TELNUM3").toString());
        comboBoxTel4->setCurrentText(qmodel.record(0).value("ET_TELMOD4").toString());
        lineEditTelNum4->setText(qmodel.record(0).value("ET_TELNUM4").toString());

        lineEditContato1->setText(qmodel.record(0).value("ET_CONTATO1").toString());
        lineEditEmail1->setText(qmodel.record(0).value("ET_EMAIL1").toString());
        lineEditContato2->setText(qmodel.record(0).value("ET_CONTATO2").toString());
        lineEditEmail2->setText(qmodel.record(0).value("ET_EMAIL2").toString());

        lineEditCNPJ->setText(qmodel.record(0).value("ET_CNPJ").toString());
        lineEditIE->setText(qmodel.record(0).value("ET_IE").toString());
        lineEditIM->setText(qmodel.record(0).value("ET_IM").toString());
        lineEditCPF->setText(qmodel.record(0).value("ET_CPF").toString());
        lineEditRG->setText(qmodel.record(0).value("ET_RG").toString());
        lineEditURL->setText(qmodel.record(0).value("ET_URL").toString());

        lineEditObraCod->setText(uFormatCodigoObra(
                                    qmodel.record(0).value("ET_OBCOD").toInt(),
                                    qmodel.record(0).value("ET_OBANO").toInt()) );

        comboBoxObraCliente->setCurrentText(qmodel.record(0).value("ET_OBCLI").toString());

        lineEditObraCEI->setText(qmodel.record(0).value("ET_OBCEI").toString());
        spinBoxbObraCrono->setValue( qmodel.record(0).value("ET_OBCRONO").toInt());

        lineEditOutEspec->setText(qmodel.record(0).value("ET_OUTESPEC").toString());
        lineEditOutNFld1->setText(qmodel.record(0).value("ET_OUTNFLD1").toString());
        lineEditOutDFld1->setText(qmodel.record(0).value("ET_OUTDFLD1").toString());
        lineEditOutNFld2->setText(qmodel.record(0).value("ET_OUTNFLD2").toString());
        lineEditOutDFld2->setText(qmodel.record(0).value("ET_OUTDFLD2").toString());
        lineEditOutObs->setText(qmodel.record(0).value("ET_OUTOBS").toString());

        textEditHistorico->setText(qmodel.record(0).value("ET_OBSGER").toString());

        emit hide(NS_UTILITIES::DockEntity);
    }
}



void RecordEntity::checkBoxStatus_toggled(bool b)
{
    checkBoxStatus->setText( (b ? tr("Ativo") : tr("Inativo")) );
    checkBoxStatus->setIcon( (b ? QPixmap(":/grndiamd.png") : QPixmap(":/reddiamd.png")) );
}

void RecordEntity::radioButtonEmpresa_toggled(bool b)
{
    controlTabModelos(0,b);
    controlObjects(true);
    tabWidgetModelos->setEnabled(true);
    lineEditNome->setFocus();
}

void RecordEntity::radioButtonCliente_toggled(bool b)
{
    controlTabModelos(0,b);
    controlObjects(true);
    // casos especiais
    lineEditCNPJ->clear();
    lineEditIE->clear();
    lineEditIM->clear();
    tabWidgetModelos->setEnabled(true);
    lineEditNome->setFocus();
}

void RecordEntity::radioButtonFornecedor_toggled(bool b)
{
    controlTabModelos(0,b);
    controlObjects(true);
    tabWidgetModelos->setEnabled(true);
    lineEditNome->setFocus();
}

void RecordEntity::radioButtonProjetos_toggled(bool b)
{
    controlTabModelos(1,b);
    controlObjects(true);
    tabWidgetModelos->setEnabled(true);
    lineEditNome->setFocus();
}

void RecordEntity::radioButtonOutros_toggled(bool b)
{
    controlTabModelos(2,b);
    controlObjects(true);
    tabWidgetModelos->setEnabled(true);
    lineEditNome->setFocus();
}

void RecordEntity::radioButtonSimples_toggled(bool b)
{
    if(b) {
        controlObjects(false);
        tabWidgetModelos->setEnabled(false);
        tabWidgetModelos->setCurrentIndex(0);
        clearReg();
        lineEditNome->setFocus();
    }
}

void RecordEntity::comboBoxEntities_currentIndexChanged(int idx)
{
    switch(idx) {
    case 0: // todas
        iClasse = -1;
        break;
    case 1: // Empresas
        iClasse = NS_UTILITIES::EnEmpresa;
        break;
    case 2: // Clientes
        iClasse = NS_UTILITIES::EnCliente;
        break;
    case 3: // Fornecedores
        iClasse = NS_UTILITIES::EnFornecedor;
        break;
    case 4: // P & O
        iClasse = NS_UTILITIES::EnProjObras;
        break;
    case 5: // Personalizados
        iClasse = NS_UTILITIES::EnOutros;
        break;
    case 6: // simples
        iClasse = NS_UTILITIES::EnTipoSimples;
        break;
    }
}

void RecordEntity::saveReg()
{
    if(NS_UTILITIES::MAXREGISTERS > 0) {
        if(uLastEntity() >= NS_UTILITIES::MAXREGISTERS) {
            QMessageBox::warning(this, PROGRAM_NAME,
                                 tr("Você alcançou o limite máximo de registros permitidos"
                                    " para sua Licença de Uso.\n"
                                    "Entre em contato com o desenvolvedor para liberação"
                                    " de mais registros.")
                                , QMessageBox::Close );
            return;
        }
    }
     if(!checkBoxStatus->isChecked()) {
        QMessageBox::warning(this, PROGRAM_NAME,
                            tr("Você não pode gravar uma entidade inativada!"
                               "Mas somente atualizar."
                               "Se este for o caso então ative e grave, senão atualize.")
                            , QMessageBox::Close );
        return;
     }

     if(lineEditNome->text().isEmpty()) {
        QMessageBox::warning(this, PROGRAM_NAME,
                            tr("NOME: Este campo não pode ser deixado em branco.")
                            , QMessageBox::Close );
        lineEditNome->setFocus();
        return;
     }

     if(!radioButtonSimples->isChecked()) {

        if(lineEditCity->text().isEmpty()) {
            QMessageBox::warning(this, PROGRAM_NAME,
                                tr("CIDADE: Este campo não pode ser deixado em branco.")
                                , QMessageBox::Close );
            lineEditCity->setFocus();
            return;
        }

        if(radioButtonEmpresa->isChecked() ||
           radioButtonFornecedor->isChecked() || radioButtonCliente->isChecked()) {
            if( !uCNPJEmpty(lineEditCNPJ->text()) && !uCPFEmpty(lineEditCPF->text()) ) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("Não é permitido preencher ambos os campos CNPJ e CPF. Escolha um deles apenas.")
                                    , QMessageBox::Close );
                return;
            }

            if(uCNPJEmpty(lineEditCNPJ->text()) && uCPFEmpty(lineEditCPF->text())) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("Você deve preencher o CNPJ ou o CPF. Não é permitido deixá-los em branco.")
                                    , QMessageBox::Close );
                lineEditCNPJ->setFocus();
                return;
            }

            if(!lineEditCNPJ->text().isEmpty() && !uCNPJEmpty(lineEditCNPJ->text())) {
                if(uCNPJExists(lineEditCNPJ->text().simplified())) {
                    QMessageBox::warning(this, PROGRAM_NAME,
                                        tr("CNPJ: Já existe no banco de dados.")
                                        , QMessageBox::Close );
                    lineEditCNPJ->setFocus();
                    return;
                }
                CNPJ cnpj(lineEditCNPJ->text());
                if(!cnpj.isValid()) {
                    QMessageBox::warning(this, PROGRAM_NAME,
                                         tr("CNPJ: Inválido. DV não confere.\n\nInformado: %0\nCalculado: %1"
                                            "\n\nDigitando o DV calculado o CNPJ irá ser validado, porém isso"
                                            " não garante sua autenticidade.")
                                         .arg(cnpj.section().dv).arg(cnpj.DV())
                                         , QMessageBox::Close);

                    lineEditCNPJ->setFocus();
                    return;
                }


            }
            if(!lineEditCPF->text().isEmpty() && !uCPFEmpty(lineEditCPF->text())) {
                if(uCPFExists(lineEditCPF->text().simplified())) {
                    QMessageBox::warning(this, PROGRAM_NAME,
                                        tr("CPF: Já existe no banco de dados.")
                                        , QMessageBox::Close );
                    lineEditCPF->setFocus();
                    return;
                }

                CPF cpf(lineEditCPF->text());
                if(!cpf.isValid()) {
                    QMessageBox::warning(this, PROGRAM_NAME,
                                         tr("CPF: Inválido. DV não confere.\n\nInformado: %0\nCalculado: %1"
                                            "\n\nDigitando o DV calculado o CPF irá ser validado, porém isso"
                                            " não garante sua autenticidade.")
                                         .arg(cpf.section().dv).arg(cpf.DV())
                                        , QMessageBox::Close );
                    lineEditCPF->setFocus();
                    return;
                }

            }
        } // radio

        if(radioButtonProjetos->isChecked()) {
            QString cod = lineEditObraCod->text().section('-',0,0);
            QString ano = lineEditObraCod->text().section('-',1,1);

            if(cod.toInt() <= 0 || cod.toInt() > 32767) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("CÓDIGO DA OBRA: Este campo deve ser preenchido na faixa de 1 a 32767")
                                    , QMessageBox::Close );
                lineEditObraCod->setFocus();
                return;
            }
            if(cod.toInt() == 0 && ano.toInt() == 0) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("CÓDIGO DA OBRA: Este campo não pode ser deixado em branco.")
                                    , QMessageBox::Close );
                lineEditObraCod->setFocus();
                return;
            }

            if(cod.toInt() == 0 && ano.toInt() > 0) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("CÓDIGO DA OBRA: Código não foi preenchido.")
                                    , QMessageBox::Close );
                lineEditObraCod->setFocus();
                return;
            }
            if(cod.toInt() > 0 && ano.toInt() == 0) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("CÓDIGO DA OBRA: Ano não foi preenchido.")
                                    , QMessageBox::Close );
                lineEditObraCod->setFocus();
                return;
            }

            if(comboBoxObraCliente->currentText().isEmpty()) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("Você deve escolher um cliente para esta obra.")
                                    , QMessageBox::Close );
                comboBoxObraCliente->setFocus();
                return;

            }
        }

        if(radioButtonOutros->isChecked()) {
            if( lineEditOutEspec->text().isEmpty() ) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("ESPECIFICAÇÃO: Este campo não pode ser deixado em branco.")
                                    , QMessageBox::Close );
                lineEditOutEspec->setFocus();
                return;
            }
        }
    }

    QString stmt="INSERT INTO DOCENTITY ( \
            ET_DATA, ET_ATIVO, ET_CATEG, ET_NOME, ET_ALIAS, \
            ET_ENDER, ET_BAIR, ET_CITY, ET_UF, ET_CEP, \
            ET_TELMOD1, ET_TELNUM1, ET_TELMOD2, ET_TELNUM2, ET_TELMOD3, ET_TELNUM3, ET_TELMOD4, ET_TELNUM4, \
            ET_CONTATO1, ET_EMAIL1, ET_CONTATO2, ET_EMAIL2, \
            ET_CPF, ET_RG, ET_CNPJ, ET_IE, ET_IM, ET_URL, \
            ET_OBCOD, ET_OBANO, ET_OBCLI, ET_OBCRONO, \
            ET_OUTESPEC, ET_OUTNFLD1, ET_OUTDFLD1, ET_OUTNFLD2, ET_OUTDFLD2, ET_OUTOBS, \
            ET_OBSGER) \
            VALUES (:ET_DATA, :ET_ATIVO, :ET_CATEG, :ET_NOME, :ET_ALIAS, \
                    :ET_ENDER, :ET_BAIR, :ET_CITY, :ET_UF, :ET_CEP, \
                    :ET_TELMOD1, :ET_TELNUM1, :ET_TELMOD2, :ET_TELNUM2, :ET_TELMOD3, :ET_TELNUM3, :ET_TELMOD4, :ET_TELNUM4, \
                    :ET_CONTATO1, :ET_EMAIL1, :ET_CONTATO2, :ET_EMAIL2, \
                    :ET_CPF, :ET_RG, :ET_CNPJ, :ET_IE, :ET_IM, :ET_URL, \
                    :ET_OBCOD, :ET_OBANO, :ET_OBCLI, :ET_OBCRONO, \
                    :ET_OUTESPEC, :ET_OUTNFLD1, :ET_OUTDFLD1, :ET_OUTNFLD2, :ET_OUTDFLD2, :ET_OUTOBS, \
                    :ET_OBSGER)";         

    QSqlDatabase::database().transaction();
    QSqlQuery q;
    q.prepare(stmt);
    q.bindValue(":ET_DATA",  lib2.today(VCPLib2::Iso));
    q.bindValue(":ET_ATIVO", checkBoxStatus->isChecked());
    q.bindValue(":ET_CATEG", classEntity());
    q.bindValue(":ET_NOME", lineEditNome->text().simplified());
    q.bindValue(":ET_ALIAS", lineEditAlias->text().simplified());
    q.bindValue(":ET_ENDER", lineEditEnder->text().simplified());
    q.bindValue(":ET_BAIR", lineEditBair->text().simplified());
    q.bindValue(":ET_CITY", lineEditCity->text().simplified());
    q.bindValue(":ET_UF", (comboBoxUF->currentText().isEmpty() ? nullptr : comboBoxUF->currentText()) );
    q.bindValue(":ET_CEP", (lineEditCEP->text() == NS_UTILITIES::MASK_CEP) ? nullptr : lineEditCEP->text()  );
    q.bindValue(":ET_TELMOD1", comboBoxTel1->currentText());
    q.bindValue(":ET_TELNUM1", lineEditTelNum1->text().simplified());
    q.bindValue(":ET_TELMOD2", comboBoxTel2->currentText());
    q.bindValue(":ET_TELNUM2", lineEditTelNum2->text().simplified());
    q.bindValue(":ET_TELMOD3", comboBoxTel3->currentText());
    q.bindValue(":ET_TELNUM3", lineEditTelNum3->text().simplified());
    q.bindValue(":ET_TELMOD4", comboBoxTel4->currentText());
    q.bindValue(":ET_TELNUM4", lineEditTelNum4->text().simplified());
    q.bindValue(":ET_CONTATO1", lineEditContato1->text().simplified());
    q.bindValue(":ET_EMAIL1", lineEditEmail1->text().simplified());
    q.bindValue(":ET_CONTATO2", lineEditContato2->text().simplified());
    q.bindValue(":ET_EMAIL2", lineEditEmail2->text().simplified());

    q.bindValue(":ET_CPF", (uCPFEmpty(lineEditCPF->text()) ? nullptr : lineEditCPF->text()) );
    q.bindValue(":ET_RG",  (uRGEmpty(lineEditRG->text()) ? nullptr : lineEditRG->text()) );
    q.bindValue(":ET_CNPJ", (uCNPJEmpty(lineEditCNPJ->text()) ? nullptr : lineEditCNPJ->text()) );
    q.bindValue(":ET_IE", (uIEEmpty(lineEditIE->text()) ? nullptr : lineEditIE->text()) );
    q.bindValue(":ET_IM", (lineEditIM->text().simplified().isEmpty() ? nullptr : lineEditIM->text().simplified()) );
    q.bindValue(":ET_URL", (lineEditURL->text().simplified().isEmpty() ? nullptr : lineEditURL->text().simplified()) );

    if(!comboBoxObraCliente->currentText().isEmpty()) {
        q.bindValue(":ET_OBCOD", lineEditObraCod->text().section('-',0,0).toShort() );
        q.bindValue(":ET_OBANO", lineEditObraCod->text().section('-',1,1).toInt() );
    } else {
        q.bindValue(":ET_OBCOD", "0");
        q.bindValue(":ET_OBANO", "0" );
    }

    if( classEntity() == NS_UTILITIES::EnProjObras) {
        q.bindValue(":ET_OBCLI",  (!comboBoxObraCliente->currentText().isEmpty()  ?
                                   comboBoxObraCliente->currentText().section('#',1,1).toInt() : 0) );
    } else {
        q.bindValue(":ET_OBCLI","0" );
    }

    q.bindValue(":ET_OBCEI", lineEditObraCEI->text());
    q.bindValue(":ET_OBCRONO", spinBoxbObraCrono->value());
    q.bindValue(":ET_OUTESPEC", lineEditOutEspec->text().simplified());
    q.bindValue(":ET_OUTNFLD1", lineEditOutNFld1->text().simplified());
    q.bindValue(":ET_OUTDFLD1", lineEditOutDFld1->text().simplified());
    q.bindValue(":ET_OUTNFLD2", lineEditOutNFld2->text().simplified());
    q.bindValue(":ET_OUTDFLD2", lineEditOutDFld2->text().simplified());
    q.bindValue(":ET_OUTOBS", lineEditOutObs->text().simplified());
    q.bindValue(":ET_OBSGER", textEditHistorico->toPlainText());

    q.exec();

    if(q.lastError().isValid()) {
      QMessageBox::critical(this, PROGRAM_NAME,
             q.lastError().text()+tr("Registro não foi gravado."),
             QMessageBox::Close );
       return;
    }
    QSqlDatabase::database().commit();

    clearReg();
}

void RecordEntity::updateReg()
{
    if(!checkBoxStatus->isChecked()) {
        QMessageBox::warning(this, PROGRAM_NAME,
                             tr("Você está desativando esta entidade."
                                "Desse modo todos os documentos relacionados "
                                "deixarão de estar disponíveis para consulta.")
                            , QMessageBox::Close );
    }

    if(lineEditNome->text().isEmpty()) {
        QMessageBox::warning(this, PROGRAM_NAME,
                            tr("NOME: Este campo não pode ser deixado em branco.")
                            , QMessageBox::Close );
        lineEditNome->setFocus();
        return;
    }

    if(!radioButtonSimples->isChecked()) {

        if(lineEditCity->text().isEmpty()) {
            QMessageBox::warning(this, PROGRAM_NAME,
                                tr("CIDADE: Este campo não pode ser deixado em branco.")
                                , QMessageBox::Close );
            lineEditCity->setFocus();
            return;
        }

        if(radioButtonEmpresa->isChecked() ||
           radioButtonFornecedor->isChecked() || radioButtonCliente->isChecked()) {
            if( !uCNPJEmpty(lineEditCNPJ->text()) && !uCPFEmpty(lineEditCPF->text()) ) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("Não é permitido preencher ambos os campos CNPJ e CPF. Escolha um deles apenas.")
                                    , QMessageBox::Close );
                return;
            }

            if(uCNPJEmpty(lineEditCNPJ->text()) && uCPFEmpty(lineEditCPF->text())) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("Você deve preencher o CNPJ ou o CPF. Não é permitido deixá-los em branco.")
                                    , QMessageBox::Close );
                lineEditCNPJ->setFocus();
                return;
            }

            if( lineEditCNPJ->text() != "../-" ) { //!lineEditCNPJ->text().isEmpty() && !uCNPJEmpty(lineEditCNPJ->text())) {
                if(uCNPJExists(lineEditCNPJ->text().simplified())) {
                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText(tr("CNPJ Já existe no cadastro!"));
                    msgBox.setInformativeText(tr("Deseja manter o mesmo ou altera-lo?"));
                    QPushButton *keepButton = msgBox.addButton(tr("Manter"), QMessageBox::ActionRole);
                    QPushButton *changeButton = msgBox.addButton(tr("Trocar"), QMessageBox::ActionRole);
                    msgBox.exec();
                    if(msgBox.clickedButton() == changeButton) {
                        lineEditCNPJ->setFocus();
                        return;
                    }
                }
                CNPJ cnpj(lineEditCNPJ->text());
                if(!cnpj.isValid()) {
                    QMessageBox::warning(this, PROGRAM_NAME,
                                         tr("CNPJ: Inválido. DV não confere.\n\nInformado: %0\nCalculado: %1"
                                            "\n\nDigitando o DV calculado o CNPJ irá ser validado, porém isso"
                                            " não garante sua autenticidade.")
                                         .arg(cnpj.section().dv).arg(cnpj.DV())
                                        , QMessageBox::Close );
                    lineEditCNPJ->setFocus();
                    return;
                }
            }
            if(!lineEditCPF->text().isEmpty() && !uCPFEmpty(lineEditCPF->text())) {
                if(uCPFExists(lineEditCPF->text().simplified())) {
                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText(tr("CPF Já existe no cadastro!"));
                    msgBox.setInformativeText(tr("Deseja manter o mesmo ou altera-lo?"));
                    QPushButton *keepButton = msgBox.addButton(tr("Manter"), QMessageBox::ActionRole);
                    QPushButton *changeButton = msgBox.addButton(tr("Trocar"), QMessageBox::ActionRole);
                    msgBox.exec();
                    if(msgBox.clickedButton() == changeButton) {
                        lineEditCPF->setFocus();
                        return;
                    }
                }
                CPF cpf(lineEditCPF->text());
                if(!cpf.isValid()) {
                    QMessageBox::warning(this, PROGRAM_NAME,
                                         tr("CPF: Inválido. DV não confere.\n\nInformado: %0\nCalculado: %1"
                                            "\n\nDigitando o DV calculado o CPF irá ser validado, porém isso"
                                            " não garante sua autenticidade.")
                                         .arg(cpf.section().dv).arg(cpf.DV())
                                        , QMessageBox::Close );
                    lineEditCPF->setFocus();
                    return;
                }
            }
        } // radio

        if(radioButtonProjetos->isChecked()) {
            QString cod = lineEditObraCod->text().section('-',0,0);
            QString ano = lineEditObraCod->text().section('-',1,1);

            if(cod.toInt() <= 0 || cod.toInt() > 32767) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("CÓDIGO DA OBRA: Este campo deve ser preenchido na faixa de 1 a 32767")
                                    , QMessageBox::Close );
                lineEditObraCod->setFocus();
                return;
            }
            if(cod.toInt() == 0 && ano.toInt() == 0) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("CÓDIGO DA OBRA: Este campo não pode ser deixado em branco.")
                                    , QMessageBox::Close );
                lineEditObraCod->setFocus();
                return;
            }

            if(cod.toInt() == 0 && ano.toInt() > 0) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("CÓDIGO DA OBRA: Código não foi preenchido.")
                                    , QMessageBox::Close );
                lineEditObraCod->setFocus();
                return;
            }
            if(cod.toInt() > 0 && ano.toInt() == 0) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("CÓDIGO DA OBRA: Ano não foi preenchido.")
                                    , QMessageBox::Close );
                lineEditObraCod->setFocus();
                return;
            }

            if(comboBoxObraCliente->currentText().isEmpty()) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("Você deve escolher um cliente para esta obra.")
                                    , QMessageBox::Close );
                comboBoxObraCliente->setFocus();
                return;

            }
        }

        if(radioButtonOutros->isChecked()) {
            if( lineEditOutEspec->text().isEmpty() ) {
                QMessageBox::warning(this, PROGRAM_NAME,
                                    tr("ESPECIFICAÇÃO: Este campo não pode ser deixado em branco.")
                                    , QMessageBox::Close );
                lineEditOutEspec->setFocus();
                return;
            }
        }
    }

    SysActivity *sact = new SysActivity(this);
    if(!sact->logger(ModulesOfApp::ModEntity, SysActivity::Operations::Update, lcdNumberCodigo->value())) {
        delete sact;
        return;
    }

    QString stmt=QString("UPDATE DOCENTITY SET "
                         "ET_DATA=:ET_DATA, ET_ATIVO=:ET_ATIVO, ET_CATEG=:ET_CATEG, ET_NOME=:ET_NOME, ET_ALIAS=:ET_ALIAS, "
                         "ET_ENDER=:ET_ENDER, ET_BAIR=:ET_BAIR, ET_CITY=:ET_CITY, ET_UF=:ET_UF, ET_CEP=:ET_CEP, "
                         "ET_TELMOD1=:ET_TELMOD1, ET_TELNUM1=:ET_TELNUM1, "
                         "ET_TELMOD2=:ET_TELMOD2, ET_TELNUM2=:ET_TELNUM2, "
                         "ET_TELMOD3=:ET_TELMOD3, ET_TELNUM3=:ET_TELNUM3, "
                         "ET_TELMOD4=:ET_TELMOD4, ET_TELNUM4=:ET_TELNUM4, "
                         "ET_CONTATO1=:ET_CONTATO1, ET_EMAIL1=:ET_EMAIL1, "
                         "ET_CONTATO2=:ET_CONTATO2, ET_EMAIL2=:ET_EMAIL2, "
                         "ET_CPF=:ET_CPF, ET_RG=:ET_RG, ET_CNPJ=:ET_CNPJ, ET_IE=:ET_IE, "
                         "ET_IM=:ET_IM, ET_URL=:ET_URL, "
                         "ET_OBCOD=:ET_OBCOD, ET_OBANO=:ET_OBANO, "
                         "ET_OBCLI=:ET_OBCLI, ET_OBCRONO=:ET_OBCRONO, "
                         "ET_OUTESPEC=:ET_OUTESPEC, "
                         "ET_OUTNFLD1=:ET_OUTNFLD1, ET_OUTDFLD1=:ET_OUTDFLD1, "
                         "ET_OUTNFLD2=:ET_OUTNFLD2, ET_OUTDFLD2=:ET_OUTDFLD2, "
                         "ET_OUTOBS=:ET_OUTOBS, ET_OBSGER=:ET_OBSGER "
                         "WHERE ET_IDENT=:ET_IDENT"
                         );

    QSqlDatabase::database().transaction();
    QSqlQuery q;
    q.prepare(stmt);
    q.bindValue(":ET_IDENT", lcdNumberCodigo->value()); // WHERE

    q.bindValue(":ET_DATA",  lib2.today(VCPLib2::Iso));
    q.bindValue(":ET_ATIVO", checkBoxStatus->isChecked());
    q.bindValue(":ET_CATEG", classEntity());
    q.bindValue(":ET_NOME", lineEditNome->text().simplified());
    q.bindValue(":ET_ALIAS", lineEditAlias->text().simplified());
    q.bindValue(":ET_ENDER", lineEditEnder->text().simplified());
    q.bindValue(":ET_BAIR", lineEditBair->text().simplified());
    q.bindValue(":ET_CITY", lineEditCity->text().simplified());
    q.bindValue(":ET_UF", (comboBoxUF->currentText().isEmpty() ? nullptr : comboBoxUF->currentText()) );
    q.bindValue(":ET_CEP", (lineEditCEP->text() == NS_UTILITIES::MASK_CEP) ? "" : lineEditCEP->text() );
    q.bindValue(":ET_TELMOD1", comboBoxTel1->currentText());
    q.bindValue(":ET_TELNUM1", lineEditTelNum1->text().simplified());
    q.bindValue(":ET_TELMOD2", comboBoxTel2->currentText());
    q.bindValue(":ET_TELNUM2", lineEditTelNum2->text().simplified());
    q.bindValue(":ET_TELMOD3", comboBoxTel3->currentText());
    q.bindValue(":ET_TELNUM3", lineEditTelNum3->text().simplified());
    q.bindValue(":ET_TELMOD4", comboBoxTel4->currentText());
    q.bindValue(":ET_TELNUM4", lineEditTelNum4->text().simplified());
    q.bindValue(":ET_CONTATO1", lineEditContato1->text().simplified());
    q.bindValue(":ET_EMAIL1", lineEditEmail1->text().simplified());
    q.bindValue(":ET_CONTATO2", lineEditContato2->text().simplified());
    q.bindValue(":ET_EMAIL2", lineEditEmail2->text().simplified());
    q.bindValue(":ET_CPF", (uCPFEmpty(lineEditCPF->text()) ? nullptr : lineEditCPF->text()) );
    q.bindValue(":ET_RG",  (uRGEmpty(lineEditRG->text()) ? nullptr : lineEditRG->text()) );
    q.bindValue(":ET_CNPJ", (uCNPJEmpty(lineEditCNPJ->text()) ? nullptr : lineEditCNPJ->text()) );
    q.bindValue(":ET_IE", (uIEEmpty(lineEditIE->text()) ? nullptr : lineEditIE->text()) );
    q.bindValue(":ET_IM", (lineEditIM->text().simplified().isEmpty() ? nullptr : lineEditIM->text().simplified()) );
    q.bindValue(":ET_URL", (lineEditURL->text().simplified().isEmpty() ? nullptr : lineEditURL->text().simplified()) );

    if( classEntity() == NS_UTILITIES::EnProjObras) {
        q.bindValue(":ET_OBCOD", lineEditObraCod->text().section('-',0,0).toShort() );
        q.bindValue(":ET_OBANO", lineEditObraCod->text().section('-',1,1).toInt() );
        q.bindValue(":ET_OBCLI",  (!comboBoxObraCliente->currentText().isEmpty() ?
                                   comboBoxObraCliente->currentText().section('#',1,1).toInt() : 0) );
    } else {
        q.bindValue(":ET_OBCOD", "" );
        q.bindValue(":ET_OBANO", "" );
        q.bindValue(":ET_OBCLI", "0");
    }
    q.bindValue(":ET_OBCEI", lineEditObraCEI->text());
    q.bindValue(":ET_OBCRONO", spinBoxbObraCrono->value());
    q.bindValue(":ET_OUTESPEC", lineEditOutEspec->text().simplified());
    q.bindValue(":ET_OUTNFLD1", lineEditOutNFld1->text().simplified());
    q.bindValue(":ET_OUTDFLD1", lineEditOutDFld1->text().simplified());
    q.bindValue(":ET_OUTNFLD2", lineEditOutNFld2->text().simplified());
    q.bindValue(":ET_OUTDFLD2", lineEditOutDFld2->text().simplified());
    q.bindValue(":ET_OUTOBS", lineEditOutObs->text().simplified());
    q.bindValue(":ET_OBSGER", textEditHistorico->toPlainText());
    q.exec();
    if(q.lastError().isValid()) {
      QMessageBox::critical(this, PROGRAM_NAME,
             q.lastError().text()+tr("\n\nRegistro não foi gravado."),
             QMessageBox::Close );
       return;
    }
    QSqlDatabase::database().commit();
}

void RecordEntity::deleteReg()
{
    QApplication::beep();
    QMessageBox messageBox(this);
    messageBox.setIconPixmap(QPixmap(":/messagebox_warning.png"));
    messageBox.setWindowTitle( PROGRAM_NAME);
    messageBox.setTextFormat(Qt::AutoText);
    messageBox.setText(tr("<b>** Este procedimento é irreversível! **</b>"));
    messageBox.setInformativeText(tr("Se você confirmar que deseja apagar esta entidade, "
                                     "todos os documentos relacionados à ela serão "
                                     "também removidos do banco de dados!")
                                  );
    QAbstractButton *cancelButton =
          messageBox.addButton(tr("Cancelar"), QMessageBox::ActionRole);
    QAbstractButton *continueButton =
          messageBox.addButton(tr("Prossegir"), QMessageBox::ActionRole);
    messageBox.setEscapeButton(cancelButton);

    messageBox.exec();
    if(messageBox.clickedButton() == continueButton) {
        SysActivity *sact = new SysActivity(this);
        if(!sact->logger(ModulesOfApp::ModEntity, SysActivity::Operations::Delete, lcdNumberCodigo->value())) {
            delete sact;
            return;
        }

        QString stmt=QString("DELETE FROM DOCENTITY WHERE ET_IDENT=:ET_IDENT");
        QSqlQuery q;
        q.prepare(stmt);
        q.bindValue(":ET_IDENT", lcdNumberCodigo->value()); // WHERE
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
                 q.lastError().text()+tr("\nRegistro não foi apagado."),
                 QMessageBox::Close );
            return;
        }
        QMessageBox::information(this, PROGRAM_NAME,
               tr("Registro apagado com sucesso!"),
                QMessageBox::Ok );
        clearReg();
        lineEditNome->setFocus();
    }
}

void RecordEntity::clearReg()
{
    /*
     * C++11 compliant
     */

    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    QList<QComboBox *> allCombos = this->findChildren<QComboBox *>();

    for(QLineEdit *le : allLineEdits) {
        le->clear();
    }

    for(QComboBox *cb : allCombos) {
        cb->setCurrentIndex(0);
    }

    checkBoxStatus->setCheckState(Qt::Checked);  
    checkBoxStatus->setIcon( QPixmap(":/grndiamd.png") );

    spinBoxbObraCrono->setValue(0);

    textEditHistorico->clear();

    lcdNumberCodigo->display(QString::number(lastPgID()));
    labelDateReg->setText( lib2.today(VCPLib2::Eur) );

    lineEditObraCod->setText(lastObraID());

    lineEditNome->setFocus();
}

void RecordEntity::printActiveReg(void)
{
    QString stmt = QString("SELECT "
                           "ET_IDENT, DATE_FORMAT(ET_DATA,'%d/%m/%Y') AS ET_DATA, "
                           "IF(ET_CATEG=0, 'Empresa', "
                           " IF(ET_CATEG=1, 'Cliente', "
                           "  IF(ET_CATEG=2, 'Fornecedor', "
                           "   IF(ET_CATEG=3, 'Projeto/Obra', "
                           "    IF(ET_CATEG=4, 'Outros', "
                           "     IF(ET_CATEG=5, 'Simples', 'NÃO CLASSIFICADO')))))) AS ET_CATEG, "
                           "ET_NOME, ET_ALIAS, ET_ENDER, ET_BAIR, ET_CITY, ET_UF, ET_CEP, "
                           "ET_TELMOD1, ET_TELNUM1, ET_TELMOD2, ET_TELNUM2, "
                           "ET_TELMOD3, ET_TELNUM3, ET_TELMOD4, ET_TELNUM4, "
                           "ET_CONTATO1, ET_EMAIL1, ET_CONTATO2, ET_EMAIL2, "
                           "ET_CPF, ET_RG, ET_CNPJ, ET_IE, ET_IM, "
                           "ET_URL, CONCAT_WS('-',LPAD(ET_OBCOD,5,'0'),LPAD(ET_OBANO,2,'0')) AS OBRA_CODIGO, "
                           "(SELECT B.ET_NOME FROM DOCENTITY B WHERE B.ET_IDENT=A.ET_OBCLI) AS ET_OBCLI, "
                           "ET_OBCEI, ET_OBCRONO, "
                           "ET_OUTESPEC, ET_OUTNFLD1, ET_OUTDFLD1, ET_OUTNFLD2, ET_OUTDFLD2, ET_OUTOBS ");
    if(iClasse == -1)
        stmt += "FROM DOCENTITY A WHERE ET_ATIVO=1 ORDER BY ET_CATEG, ET_NOME";
    else
        stmt += QString("FROM DOCENTITY A WHERE ET_CATEG=%0 AND ET_ATIVO=1 ORDER BY ET_NOME").arg(iClasse);

    PrintEntity print;
    print.setQueryModel(stmt);
    print.setTitle( PROGRAM_NAME );
    print.setSubTitle(tr("Listagem do Cadastro de Entidades - Ativas"));
    print.preview(QPrinter::Landscape);

}


void RecordEntity::printInactiveReg(void)
{
    QString stmt = QString("SELECT "
                           "ET_IDENT, DATE_FORMAT(ET_DATA,'%d/%m/%Y') AS ET_DATA, "
                           "IF(ET_CATEG=0, 'Empresa', "
                           " IF(ET_CATEG=1, 'Cliente', "
                           "  IF(ET_CATEG=2, 'Fornecedor', "
                           "   IF(ET_CATEG=3, 'Projeto/Obra', "
                           "    IF(ET_CATEG=4, 'Outros', "
                           "     IF(ET_CATEG=5, 'Simples', 'NÃO CLASSIFICADO')))))) AS ET_CATEG, "
                           "ET_NOME, ET_ALIAS, ET_ENDER, ET_BAIR, ET_CITY, ET_UF, ET_CEP, "
                           "ET_TELMOD1, ET_TELNUM1, ET_TELMOD2, ET_TELNUM2, "
                           "ET_TELMOD3, ET_TELNUM3, ET_TELMOD4, ET_TELNUM4, "
                           "ET_CONTATO1, ET_EMAIL1, ET_CONTATO2, ET_EMAIL2, "
                           "ET_CPF, ET_RG, ET_CNPJ, ET_IE, ET_IM, "
                           "ET_URL, CONCAT_WS('-',LPAD(ET_OBCOD,5,'0'),LPAD(ET_OBANO,2,'0')) AS OBRA_CODIGO, "
                           "(SELECT B.ET_NOME FROM DOCENTITY B WHERE B.ET_IDENT=A.ET_OBCLI) AS ET_OBCLI, "
                           "ET_OBCEI, ET_OBCRONO, "
                           "ET_OUTESPEC, ET_OUTNFLD1, ET_OUTDFLD1, ET_OUTNFLD2, ET_OUTDFLD2, ET_OUTOBS ");
    if(iClasse == -1)
        stmt += "FROM DOCENTITY A WHERE ET_ATIVO=0 ORDER BY ET_CATEG, ET_NOME";
    else
        stmt += QString("FROM DOCENTITY A WHERE ET_CATEG=%0 AND ET_ATIVO=0 ORDER BY ET_NOME").arg(iClasse);

    PrintEntity print;
    print.setQueryModel(stmt);
    print.setTitle( PROGRAM_NAME );
    print.setSubTitle(tr("Listagem do Cadastro de Entidades - Inativas"));
    print.preview(QPrinter::Landscape);
}

// FUNCTIONS
int RecordEntity::classEntity(void)
{
    if(radioButtonEmpresa->isChecked())
        return NS_UTILITIES::EnEmpresa;
    else if(radioButtonFornecedor->isChecked())
        return NS_UTILITIES::EnFornecedor;
    else if(radioButtonCliente->isChecked())
        return NS_UTILITIES::EnCliente;
    else if(radioButtonProjetos->isChecked())
        return NS_UTILITIES::EnProjObras;
    else if(radioButtonOutros->isChecked())
        return NS_UTILITIES::EnOutros;
    else if(radioButtonSimples->isChecked())
        return NS_UTILITIES::EnTipoSimples;
    else
        return -1;
}

/**
 * @brief RecordEntity::controlObjects
 *        Habilita ou desabilita os objetos da janela
 *        conforme a necessidade
 * @param b true|false
 */
void RecordEntity::controlObjects(bool b)
{
    QList<QLineEdit *> allLineEdits = this->findChildren<QLineEdit *>();
    QList<QComboBox *> allCombos = this->findChildren<QComboBox *>();

    for(int i=0; i< allLineEdits.count(); i++ ) {
        QString obname = allLineEdits.at(i)->objectName();
          if( obname.startsWith("lineEdit") ) {
            if(obname != "lineEditNome")
               allLineEdits.at(i)->setEnabled(b);
        }
    }

    for(int i=0; i<allCombos.count(); i++) {
        QString obname = allCombos.at(i)->objectName();
        if(obname.startsWith("comboBox")) {
            allCombos.at(i)->setEnabled(b);
        }
    }
}


/**
 * @brief RecordEntity::controlTabModelos
 * @param index
 * @param b
 */
void RecordEntity::controlTabModelos(int index, bool b)
{
    for(int i=0; i< tabWidgetModelos->count(); i++) {
        if(i == index) {
            tabWidgetModelos->setTabEnabled(i, b);
            continue;
        }
        tabWidgetModelos->setTabEnabled(i, !b);
    }
}

/**
 * @brief RecordEntity::lastPgID
 * Retorna um inteiro sem sinal de 64bits contendo o ET_IDENT maximo+1
 * @return inteiro contendo o valor maximo do ET_IDENT+1
 */
quint64 RecordEntity::lastPgID()
{
   QSqlQuery q;
   q.prepare("SELECT MAX(ET_IDENT) FROM DOCENTITY");
   if(q.exec()) {
      q.next();
      return q.value(0).toLongLong()+1;
   }
   return(1);
}


/**
 * @brief RecordEntity::lastObraID
 * Retorna ET_OBCOD+1 ja formatado 00000-00
 * @return
 *
 * NOTA: Retorna somente os codigos referentes ao ano corrente
 */
QString RecordEntity::lastObraID()
{
    QString sql=QString("SELECT MAX(ET_OBCOD) FROM DOCENTITY WHERE ET_OBANO=%0").arg(QDate::currentDate().year()-2000);
    QSqlQuery q;
    q.prepare(sql);
    q.exec();
    q.next();

    if(q.value(0).toLongLong()>0) {
        QString r;
        quint64 i = q.value(0).toLongLong()+1;
        r = QString("%0").arg(i);

        r = r + "-" + QString::number(QDate::currentDate().year()-2000);
        return r;
    }
    return QString();

}


