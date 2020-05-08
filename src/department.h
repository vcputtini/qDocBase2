 /***************************************************************************
 * Copyright (c) 2007-2019                                                 *
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

#ifndef DEPARTAMENTS_H
#define DEPARTAMENTS_H

#include <QObject>

#include <QtSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QString>
#include <QStringList>

#include <QDebug>

/**
 * @brief The Department class
 *        Esta interface foi criada para manipular de forma facil
 *        as operacoes de gravar/atualiar/apagar nomes de departamentos
 *        da tabela DOCDEPARTAMENTOS.
 *        Que e geralmente usada em comboBoxes
 *
 * @author Volnei Cervi Puttini <vcputtini@gmail.com>
 */
class Department : public QObject
{
    Q_OBJECT
public:
    explicit Department(QObject *parent = nullptr);
    enum DepartmentFind { Id, Name };

    void insertName(const QString name);
    void updateName(const QString oldName, const QString newName);
    void deleteName(const QString name);
    QStringList fillList();

protected:
    QSqlQuery query;

    QString _newName;
    QString _oldName;
    int _id;

    void _save();
    void _update();
    void _delete();
    void _findByName();
    void _findById();

signals:
    void lastError(const QString &e);
    
public slots:
    
};

#endif // DEPARTAMENTOS_H
