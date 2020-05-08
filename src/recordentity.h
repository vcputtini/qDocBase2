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

#ifndef FORMRECORDENTITY_H
#define FORMRECORDENTITY_H

#include "ui_formRecordEntity.h"


#include <QObject>
#include <QApplication>
#include <QCloseEvent>

#include <QUuid>

#include <QtSql>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>

#include <QPrinter>

#include <QString>
#include <QModelIndex>

#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QIcon>
#include <QLineEdit>
#include <QComboBox>

#include "../vcplib2/vcplib2.h"
#include "utilities.h"
#include "configuracao.h"
#include "sysactivity.h"

#include "printentity.h"

extern QSqlDatabase	db;
extern QSqlError	sqlError;


using namespace NS_UTILITIES;

/**
 * @brief The RecordEntity class
 *@author Volnei Cervi Puttini <vcputtini@gmail.com>
 */
class RecordEntity : public QWidget, private Ui::RecordEntity
{
    Q_OBJECT
    
public:
    explicit RecordEntity(QWidget *parent = nullptr);

private:
    VCPLib2 lib2;
    Configuracao CFG;

    QString sNameEntity;
    QString sTypeEntity;
    int iTypeEntity;
    bool __IDENTIFY__;
    int iClasse;
    QLineEdit *lineEditObraCliente;
    QLineEdit *lineEditObraDepe;

    quint64 lastPgID();
    QString lastObraID();

    void controlTabModelos(int, bool);
    void controlObjects(bool);
    int classEntity(void);

public slots:
    // usado na listview dockable
    void nameEntity(QModelIndex); // recebe o dados sobre a entidade escolhida pelo usuario

    void saveReg();
    void updateReg();
    void deleteReg();
    void clearReg();
    void printActiveReg();
    void printInactiveReg();
    void comboBoxEntities_currentIndexChanged(int);

    virtual void checkBoxStatus_toggled(bool);

    virtual void radioButtonEmpresa_toggled(bool);
    virtual void radioButtonCliente_toggled(bool);
    virtual void radioButtonFornecedor_toggled(bool);
    virtual void radioButtonProjetos_toggled(bool);
    virtual void radioButtonOutros_toggled(bool);
    virtual void radioButtonSimples_toggled(bool);

    void dynamic_editFinished();

signals:
    void open(bool);
    void closed(bool);
    void hide(DockType);
    void deleteMenu(ModulesOfApp);

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // FORMRECORDENTITY_H
