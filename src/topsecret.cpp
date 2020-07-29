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

#include "topsecret.h"

TopSecret::TopSecret(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    leOper->setValidator(new vcpUpperValidator);
    lePassAtual->setEnabled(false);
    lePassNew->setEnabled(false);
    lePassNew2->setEnabled(false);
    pbUpdtAdm->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/redo.png")));
    pbUpdtAdm->setEnabled(false);

    pbSalvar->setIcon(QIcon::fromTheme("document-save", QIcon(":/filesave.png")));
    pbAtualizar->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/redo.png")));
    pbApagar->setIcon(QIcon::fromTheme("edit-delete", QIcon(":/fileclose.png")));

}


void TopSecret::pbUpdtAdm_clicked()
{
  QString pAtual = lePassAtual->text().simplified();
  QString pNew = lePassNew->text().simplified();
  QString pNew2 = lePassNew2->text().simplified();

  if(pAtual.isEmpty() || pNew.isEmpty() || pNew2.isEmpty()) {
    QMessageBox::warning(this, PROGRAM_NAME,
         tr("Todas os campos devem ser informados."),
            QMessageBox::Close );
     return;
  }

  if(pAtual == pNew) {
    QMessageBox::warning(this, PROGRAM_NAME,
         tr("Senha atual não pode ser igual a nova senha."),
            QMessageBox::Close );
     return;
  }

  if(pNew != pNew2) {
    QMessageBox::warning(this, PROGRAM_NAME,
         tr("A confirmação da senha não igual a senha nova."),
            QMessageBox::Close );
     return;
  }

  // Pesquisa pela senha atual para ver se e verdadeira
  QSqlDatabase::database().transaction();
  QSqlQuery q("SELECT DES_DECRYPT(SENHA,'3vcp69') FROM DOCSECURITY WHERE ID=1 AND OPERADOR='admin'");
  q.exec();
  q.next();
  if(q.value(0).toString() == pAtual) {
     QSqlDatabase::database().commit(); // transac anterior
     QSqlDatabase::database().transaction();
     QString STMT="UPDATE DOCSECURITY SET SENHA=DES_ENCRYPT(:SENHA,'3vcp69') WHERE ID=1 AND OPERADOR='admin'";
     QSqlQuery q;
     q.prepare(STMT);
     q.bindValue(":SENHA", pNew);
     q.exec();
     if(q.lastError().isValid()) {
        QMessageBox::warning(this, PROGRAM_NAME,
            q.lastError().text()+tr("Senha não pode ser atualizada."),
            QMessageBox::Close );
        QSqlDatabase::database().rollback();
     }
     QSqlDatabase::database().commit();
    QMessageBox::information(this, PROGRAM_NAME,
         tr("Senha atualizada com sucesso."),
            QMessageBox::Close );
  }
  else {
    QMessageBox::warning(this, PROGRAM_NAME,
         tr("Senha não pode ser encontrada."),
            QMessageBox::Close );
    QSqlDatabase::database().rollback();
  }
}

void TopSecret::pbLiberar_clicked()
{
  if( leAdm->text().isEmpty() || lePassAdm->text().isEmpty()) {
    QMessageBox::warning(this, PROGRAM_NAME,
         tr("Nome do usuário Administrador e senha não foram informados"),
            QMessageBox::Close );
    return;
  }
  QSqlDatabase::database().transaction();
  QSqlQuery q;
  q.prepare("SELECT DES_DECRYPT(SENHA,'3vcp69') FROM DOCSECURITY WHERE ID=1 AND OPERADOR=:OPERADOR");
  q.bindValue(":OPERADOR",leAdm->text().simplified());
  q.exec();
  q.next();
  if(q.value(0).toString() == lePassAdm->text().simplified())  {
     grpbOperadores->setEnabled(true);
     lePassAtual->setEnabled(true);
     lePassNew->setEnabled(true);
     lePassNew2->setEnabled(true);
     pbUpdtAdm->setEnabled(true);
     reload();
  }
  else
    QMessageBox::warning(this, PROGRAM_NAME,
         tr("Usuário e/ou senha não combinam. Tente novamente."),
            QMessageBox::Close );
  QSqlDatabase::database().commit();
}

// operadores
void TopSecret::pbSalvar_clicked()
{
   if(leOper->text() == "ADMIN") {
      QMessageBox::warning(this, PROGRAM_NAME,
         tr("Nome do Operador não pode ser <b>ADMIN</b>"),
            QMessageBox::Close );
      return;
   }

   if(leOper->text().isEmpty()) {
      QMessageBox::warning(this, PROGRAM_NAME,
         tr("Nome do Operador é obrigatório."),
            QMessageBox::Close );
      return;
   }

   if(leOperPass->text().isEmpty()) {
      QMessageBox::warning(this, PROGRAM_NAME,
         tr("Senha do Operador é obrigatória."),
            QMessageBox::Close );
      return;
   }

   if(!leOperPass2->text().isEmpty()) {
      QMessageBox::warning(this, PROGRAM_NAME,
         tr("Confirmação da senha somente é usada na atualização."),
            QMessageBox::Close );
      return;
   }

   QString STMT="INSERT INTO DOCSECURITY (OPERADOR, SENHA) VALUES (:OPERADOR, DES_ENCRYPT(:SENHA,'3vcp69'))";
   QSqlDatabase::database().transaction();
   QSqlQuery q;
   q.prepare(STMT);
   q.bindValue(":OPERADOR", leOper->text().simplified());
   q.bindValue(":SENHA", leOperPass->text().simplified());
   q.exec();
   if(q.lastError().isValid()) {
      QMessageBox::critical(this, PROGRAM_NAME,
            q.lastError().text()+tr("Operador não pode ser gravado."),
            QMessageBox::Close );
        QSqlDatabase::database().rollback();
      return;
   }
   QSqlDatabase::database().commit();
   reload();
}

void TopSecret::pbAtualizar_clicked()
{
   if(leOper->text().isEmpty()) {
      QMessageBox::warning(this, PROGRAM_NAME,
         tr("Nome do Operador é obrigatório."),
            QMessageBox::Close );
      return;
   }

   if(leOperPass->text().isEmpty()) {
      QMessageBox::warning(this, PROGRAM_NAME,
         tr("Senha do Operador é obrigatória."),
            QMessageBox::Close );
      return;
   }

   if(leOperPass2->text().isEmpty()) {
      QMessageBox::warning(this, PROGRAM_NAME,
         tr("Senha de confirmação é obrigatória."),
            QMessageBox::Close );
      return;
   }

   if(leOperPass->text().simplified() != leOperPass2->text().simplified()) {
      QMessageBox::warning(this, PROGRAM_NAME,
         tr("Senha de confirmação não combina com a senha nova."),
            QMessageBox::Close );
      return;
   }

   QString STMT="UPDATE DOCSECURITY SET SENHA=DES_ENCRYPT(:SENHA,'3vcp69') WHERE OPERADOR=:OPERADOR";
   QSqlDatabase::database().transaction();
   QSqlQuery q;
   q.prepare(STMT);
   q.bindValue(":OPERADOR", leOper->text().simplified());
   q.bindValue(":SENHA", leOperPass->text().simplified());
   q.exec();
   if(q.lastError().isValid()) {
      QMessageBox::critical(this, PROGRAM_NAME,
            q.lastError().text()+tr("Operador não pode ser gravado."),
            QMessageBox::Close );
        QSqlDatabase::database().rollback();
      return;
   }
   QSqlDatabase::database().commit();
   QMessageBox::information(this, PROGRAM_NAME,
            q.lastError().text()+tr("Operação executada com sucesso!"),
            QMessageBox::Close );
}

void TopSecret::pbApagar_clicked()
{

   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   QSqlDatabase::database().transaction();
   QSqlQuery q1;
   q1.prepare(QString("SELECT HIGH_PRIORITY DL_OPERADOR FROM DOCLIBRARY WHERE DL_OPERADOR='%0' LIMIT 1").arg(leOper->text().simplified()) );
   q1.exec();
   q1.next();
   if(!q1.value(0).toString().isEmpty()) {
      QApplication::restoreOverrideCursor();
      QMessageBox::critical(this, PROGRAM_NAME,
            tr("Não é permitido apagar este operador pois existem documentos relacionados a ele."),
            QMessageBox::Close );
      return;
   }
   QSqlDatabase::database().commit();
   QApplication::restoreOverrideCursor();


   QString STMT="DELETE FROM DOCSECURITY WHERE OPERADOR=:OPERADOR";
   QSqlDatabase::database().transaction();
   QSqlQuery q;
   q.prepare(STMT);
   q.bindValue(":OPERADOR", leOper->text().simplified());
   q.exec();
   if(q.lastError().isValid()) {
      QMessageBox::critical(this, PROGRAM_NAME,
            q.lastError().text()+tr("Operador não pode ser apagado."),
            QMessageBox::Close );
        QSqlDatabase::database().rollback();
      return;
   }
   QSqlDatabase::database().commit();
   QMessageBox::information(this, PROGRAM_NAME,
            q.lastError().text()+tr("Operação executada com sucesso!"),
            QMessageBox::Close );
   reload();
}

void TopSecret::tvOperadores_doubleClicked(QModelIndex idx)
{
   leOper->setText(pModel->data(pModel->index(idx.row(),0)).toString());
   leOperPass->clear();
   leOperPass2->clear();
}

void TopSecret::reload()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  QString STMT="SELECT OPERADOR FROM DOCSECURITY WHERE OPERADOR<>'admin' ORDER BY OPERADOR";
  pModel = new QSqlQueryModel;
  pModel->setQuery(STMT);
  pModel->setHeaderData(0, Qt::Horizontal, tr("Nome do Operador"));
  tvOperadores->setModel(pModel);
  tvOperadores->resizeColumnToContents(0);
  QApplication::restoreOverrideCursor();
}



