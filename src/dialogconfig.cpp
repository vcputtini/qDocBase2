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

#include "dialogconfig.h"

DialogConfig::DialogConfig(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);


    pushButtonSalvar->setIcon( QIcon::fromTheme("document-save", QIcon(":/filesave.png")));

    readSettings();

    SALVO=false;
    tabWidgetConfig->setCurrentIndex(0);
    for(int i=1; i <= tabWidgetConfig->count(); ++i) {
       tabWidgetConfig->setTabEnabled(i, false );
    }

    lineEditFld01Name->setValidator(new vcpUpperValidator);
    lineEditFld02Name->setValidator(new vcpUpperValidator);
    lineEditFld03Name->setValidator(new vcpUpperValidator);
    lineEditFld04Name->setValidator(new vcpUpperValidator);
    lineEditFld05Name->setValidator(new vcpUpperValidator);

}

void DialogConfig::closeEvent(QCloseEvent *event)
{
  event->accept();
  if(!SALVO)
     QMessageBox::information(this , PROGRAM_NAME ,
          tr("As configurações não foram salvas."),QMessageBox::Close);
  close();
}



void DialogConfig::pushButtonOk_clicked()
{
  // Pesquisa pela senha atual para ver se e verdadeira
  QSqlDatabase::database().transaction();
  QSqlQuery q("SELECT DES_DECRYPT(SENHA,'3vcp69') FROM DOCSECURITY WHERE ID=1 AND OPERADOR='admin'");
  q.exec();
  q.next();
  if(q.value(0).toString() == lineEditAdmin->text().toLower()) {
     lineEditHostname->setEnabled(true);
     lineEditDBUser->setEnabled(true);
     lineEditDBPass->setEnabled(true);
     radioButtonLivre->setEnabled(true);
     radioButtonReadOnly->setEnabled(true);
     radioButtonROSave->setEnabled(true);
     for(int i=1; i<=tabWidgetConfig->count(); ++i) {
        tabWidgetConfig->setTabEnabled(i, true );
     }
  }
  else {
     QMessageBox::critical(this ,  PROGRAM_NAME,
          tr("Somente o administrador pode alterar as informações abaixo."),QMessageBox::Close);
     lineEditHostname->setEnabled(false);
     lineEditDBUser->setEnabled(false);
     lineEditDBPass->setEnabled(false);
     radioButtonLivre->setEnabled(true);
     radioButtonReadOnly->setEnabled(false);
     radioButtonROSave->setEnabled(false);
     for(int i=1; i<=tabWidgetConfig->count(); ++i) {
         tabWidgetConfig->setTabEnabled(i, false );
     }
  }
  QSqlDatabase::database().commit(); // transac anterior

}

void DialogConfig::pushButtonSalvar_clicked()
{
  writeSettings();
  close();
}



// FUNCTIONS
void DialogConfig::readSettings()
{
      QSettings settings(lib2.organization(), NS_UTILITIES::SettingsAppName);
      lineEditRootDir->setText(settings.value("rootdir").toString() );
      lineEditExportDir->setText(settings.value("exportdir").toString() );

      if(settings.value("tempdir").toString().isEmpty())
         lineEditTempDir->setText(QDir::tempPath() );
      else
        lineEditTempDir->setText(settings.value("tempdir").toString() );


      lineEditHostname->setText(settings.value("hostname").toString().simplified() );
      lineEditDBUser->setText(settings.value("dbuser").toString().simplified() );
      lineEditDBPass->setText(settings.value("dbpass").toString().simplified() );

      radioButtonLivre->setChecked(settings.value("livre").toInt());
      radioButtonReadOnly->setChecked(settings.value("readonly").toInt());
      radioButtonROSave->setChecked(settings.value("rosave").toInt());

      checkBoxSetFiscal->setChecked( settings.value("fiscal").toInt() ? true : false );

      checkBoxUpdtAuto->setChecked( settings.value("updtAuto").toInt() ? true : false );
      checkBoxSetDirImg->setChecked( settings.value("setDirImgDflt").toInt() ? true : false );
      checkBoxRemoveImageFromDir->setChecked( settings.value("removeImageFromDir").toInt() ? true : false );
      checkBoxKeepEntity->setChecked( settings.value("keepEntity").toInt() ? true : false );


      lineEditDfltDownload->setText( settings.value("dirDownload").toString().simplified() );

      if( !settings.value("field01Name").toString().isEmpty() ) {
         lineEditFld01Name->setText(settings.value("field01Name").toString());
         spinBox01Len->setValue(settings.value("field01Len").toInt());
         checkBox01Act->setChecked( (settings.value("field01Act").toInt() == 1 ? true : false) );
      }
      if( !settings.value("field02Name").toString().isEmpty() ) {
         lineEditFld02Name->setText(settings.value("field02Name").toString());
         spinBox02Len->setValue(settings.value("field02Len").toInt());
         checkBox02Act->setChecked( (settings.value("field02Act").toInt() == 1 ? true : false) );
      }
      if( !settings.value("field03Name").toString().isEmpty() ) {
         lineEditFld03Name->setText(settings.value("field03Name").toString());
         spinBox03Len->setValue(settings.value("field03Len").toInt());
         checkBox03Act->setChecked( (settings.value("field03Act").toInt() == 1 ? true : false) );
      }
      if( !settings.value("field04Name").toString().isEmpty() ) {
         lineEditFld04Name->setText(settings.value("field04Name").toString());
         spinBox04Len->setValue(settings.value("field04Len").toInt());
         checkBox04Act->setChecked( (settings.value("field04Act").toInt() == 1 ? true : false) );
      }
      if( !settings.value("field05Name").toString().isEmpty() ) {
         lineEditFld05Name->setText(settings.value("field05Name").toString());
         spinBox05Len->setValue(settings.value("field05Len").toInt());
         checkBox05Act->setChecked( (settings.value("field05Act").toInt() == 1 ? true : false) );
      }
}


void DialogConfig::writeSettings()
{
  // NAO GRAVA O ULTIMO CARACTER SE ELE FOR UMA '/'
  // NAO PERMITE A DIGITACAO DO BARRA INVERTIDA

  QSettings settings(lib2.organization(), NS_UTILITIES::SettingsAppName);

  // Cria os diretorios se necessario
  QDir rootDir(lineEditRootDir->text());
  QDir exportDir(lineEditExportDir->text());
  QDir tempDir(lineEditTempDir->text());

  QString dirErro;
  if( !rootDir.exists() ) {
      if( !rootDir.mkpath(lineEditRootDir->text()) ) {
        QMessageBox::warning(this , PROGRAM_NAME ,
          tr("Não foi possível criar o diretório informado. Tente criá-lo manualmente ou verifique o motivo."),QMessageBox::Close);
      }
   } else
      dirErro = lineEditRootDir->text()+QString(" Já existe.")+"<br>";

  if( !exportDir.exists() ) {
      if(!exportDir.mkpath(lineEditExportDir->text())) {
        QMessageBox::warning(this , PROGRAM_NAME ,
          tr("Não foi possível criar o diretório informado. Tente criá-lo manualmente ou verifique o motivo."),QMessageBox::Close);
      }
  } else
    dirErro += lineEditExportDir->text()+tr(" Já existe.")+"\n";

  if( !tempDir.exists() ) {
      if( !tempDir.mkpath(lineEditTempDir->text()) ) {
        QMessageBox::warning(this , PROGRAM_NAME ,
          tr("Não foi possível criar o diretório informado. Tente criá-lo manualmente ou verifique o motivo."),QMessageBox::Close);
      }
  } else
    dirErro += lineEditRootDir->text()+tr(" Já existe.")+"\n";


  if(lineEditHostname->text().isEmpty()) {
     QMessageBox::critical(this ,  PROGRAM_NAME,
          tr("Nome da Host não pode ser deixado em branco."),QMessageBox::Close);
     return;
  } else {
     settings.setValue("hostname", lineEditHostname->text());
  }

  if(lineEditDBUser->text().isEmpty()) {
     QMessageBox::critical(this ,  PROGRAM_NAME,
          tr("Nome do Usuário não pode ser deixado em branco."),QMessageBox::Close);
     return;
  } else {
     settings.setValue("dbuser", lineEditDBUser->text());
  }

  if(lineEditDBPass->text().isEmpty()) {
     QMessageBox::critical(this ,  PROGRAM_NAME,
          tr("Senha não pode ser deixada em branco."),QMessageBox::Close);
     return;
  } else {
     settings.setValue("dbpass", lineEditDBPass->text());
  }


  if(lineEditRootDir->text().endsWith("/") || lineEditRootDir->text().endsWith("\\"))
     settings.setValue("rootdir", lineEditRootDir->text().left(lineEditRootDir->text().length()-1) );
  else
     settings.setValue("rootdir", lineEditRootDir->text());

  if(lineEditExportDir->text().endsWith("/") || lineEditExportDir->text().endsWith("\\"))
     settings.setValue("exportdir",
              lineEditExportDir->text().left(lineEditExportDir->text().length()-1) );
  else
     settings.setValue("exportdir", lineEditExportDir->text());

  if(lineEditTempDir->text().endsWith("/") || lineEditTempDir->text().endsWith("\\"))
     settings.setValue("tempdir",
              lineEditTempDir->text().left(lineEditTempDir->text().length()-1) );
  else
     settings.setValue("tempdir", lineEditTempDir->text());

  settings.setValue("livre", (radioButtonLivre->isChecked() ? 1 : 0) );
  settings.setValue("readonly", (radioButtonReadOnly->isChecked() ? 1 : 0) );
  settings.setValue("rosave", (radioButtonROSave->isChecked() ? 1 : 0) );

  // Seta checkbox se o padrao e FISCAL
  settings.setValue("fiscal", (checkBoxSetFiscal->isChecked() ? 1 : 0) );


   // Seta checkbox Verificacao de Autalizacao Automatica
  settings.setValue("updtAuto", (checkBoxUpdtAuto->isChecked() ? 1 : 0) );

  // Seta checkbox Usar Sempre Diretório de Imagens Padrão
  settings.setValue("setDirImgDflt", (checkBoxSetDirImg->isChecked() ? 1 : 0) );

  // Informa se a imagem deve ser deletada do disco quando o usario
  // gravar o registro
  settings.setValue("removeImageFromDir", (checkBoxRemoveImageFromDir->isChecked() ? 1 : 0) );


  // Se for marcado o label contendo o nome da entidade principal
  // nao sera apagado nas operacoes de gravacao e atualizacao
  // para que o usuario nao tenha que sempre escolher a mesma.
  settings.setValue("keepEntity", (checkBoxKeepEntity->isChecked() ? 1 : 0) );

  if(lineEditDfltDownload->text().endsWith("/")) {
     QMessageBox::information(this ,  PROGRAM_NAME,
          tr("Por favor remova todos os caracteres '/' ao final da linha: [%0]").arg(lineEditDfltDownload->text()),QMessageBox::Close);
     return;
  }


  if(lineEditDfltDownload->text().isEmpty()) {
     QString m;
#ifdef Q_OS_WIN32
     m = "C:\\temp";
#else
     m = "/tmp";
#endif
     QMessageBox::information(this ,  PROGRAM_NAME,
          tr("Diretório padrão para download colocado como <b>%0</b>").arg(m),QMessageBox::Close);
     settings.setValue("dirDownload", m);
     return;
  } else {
     settings.setValue("dirDownload", lineEditDfltDownload->text());
  }

  // Salva definicoes dos campos personalizados
  if(checkBox01Act->isChecked() ) {
     if(!lineEditFld01Name->text().isEmpty() && spinBox01Len->value() > 0) {
        settings.setValue("field01Name", lineEditFld01Name->text());
        settings.setValue("field01Len", spinBox01Len->value());
        settings.setValue("field01Act", (checkBox01Act->isChecked() ? 1 : 0) );
     } else {
        QMessageBox::warning(this , PROGRAM_NAME,
          tr("Você não pode definir um campo com tamanho zero."),QMessageBox::Close);
     }
  } else {
        settings.setValue("field01Name", "");
        settings.setValue("field01Len", "0");
        settings.setValue("field01Act", "0");
  }

  if(checkBox02Act->isChecked() ) {
     if(!lineEditFld02Name->text().isEmpty() && spinBox02Len->value() > 0) {
        settings.setValue("field02Name", lineEditFld02Name->text());
        settings.setValue("field02Len", spinBox02Len->value());
        settings.setValue("field02Act", (checkBox02Act->isChecked() ? 1 : 0) );
     } else {
        QMessageBox::warning(this , PROGRAM_NAME,
          tr("Você não pode definir um campo com tamanho zero."),QMessageBox::Close);
     }
  } else {
        settings.setValue("field02Name", "");
        settings.setValue("field02Len", "0");
        settings.setValue("field02Act", "0");
  }

  if(checkBox03Act->isChecked() ) {
     if(!lineEditFld03Name->text().isEmpty() && spinBox03Len->value() > 0) {
        settings.setValue("field03Name", lineEditFld03Name->text());
        settings.setValue("field03Len", spinBox03Len->value());
        settings.setValue("field03Act", (checkBox03Act->isChecked() ? 1 : 0) );
     } else {
        QMessageBox::warning(this , PROGRAM_NAME,
          tr("Você não pode definir um campo com tamanho zero."),QMessageBox::Close);
     }
  } else {
        settings.setValue("field03Name", "");
        settings.setValue("field03Len", "0");
        settings.setValue("field03Act", "0");
  }

  if(checkBox04Act->isChecked() ) {
     if(!lineEditFld04Name->text().isEmpty() && spinBox04Len->value() > 0) {
        settings.setValue("field04Name", lineEditFld04Name->text());
        settings.setValue("field04Len", spinBox04Len->value());
        settings.setValue("field04Act", (checkBox04Act->isChecked() ? 1 : 0) );
     } else {
        QMessageBox::warning(this , PROGRAM_NAME,
          tr("Você não pode definir um campo com tamanho zero."),QMessageBox::Close);
     }
  } else {
        settings.setValue("field04Name", "");
        settings.setValue("field04Len", "0");
        settings.setValue("field04Act", "0");
  }

  if(checkBox05Act->isChecked() ) {
     if(!lineEditFld05Name->text().isEmpty() && spinBox05Len->value() > 0) {
        settings.setValue("field05Name", lineEditFld05Name->text());
        settings.setValue("field05Len", spinBox05Len->value());
        settings.setValue("field05Act", (checkBox05Act->isChecked() ? 1 : 0) );
     } else {
        QMessageBox::warning(this , PROGRAM_NAME,
          tr("Você não pode definir um campo com tamanho zero."),QMessageBox::Close);
     }
  } else {
        settings.setValue("field05Name", "");
        settings.setValue("field05Len", "0");
        settings.setValue("field05Act", "0");
  }


  QString msg=tr("Configurações salvas com sucesso.");
  if(!dirErro.isEmpty()) {
      msg+=tr("Porém a(s) pasta(s) abaixo já existem e não foram criadas.")+dirErro;
  }
  QMessageBox::information(this, PROGRAM_NAME,
          msg,QMessageBox::Close);
  SALVO=true;
}
