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

#include "department.h"

Department::Department(QObject *parent) :
    QObject(parent),
    _oldName(""),
    _newName(""),
    _id(0)
{
}

void Department::insertName(const QString name)
{
   _newName = name.simplified();
   _save();
}

void Department::updateName(const QString oldName, const QString newName)
{
   _oldName = oldName.simplified();
   _newName = newName.simplified();
   _update();
}

void Department::deleteName(const QString name)
{
   _oldName = name.simplified();
   _delete();
}

QStringList Department::fillList()
{
    QStringList list;

    QString sql = "SELECT DP_NOME FROM DOCDEPARTAMENTOS ORDER BY DP_NOME";

    list << ""; // primeiro item e vazio

    QSqlDatabase::database().transaction();
    query.prepare(sql);
    query.exec();
    if( query.lastError().isValid()) {
       emit lastError( query.lastError().text());
       QSqlDatabase::database().rollback();
       return QStringList();
    }
    while(query.next()) {
        list << query.value(0).toString();
    }
    QSqlDatabase::database().commit();
    return list;
}


// Membros protegidos
void Department::_save()
{
    QString sql = "INSERT INTO DOCDEPARTAMENTOS (DP_NOME) VALUES (:DP_NOME)";

    QSqlDatabase::database().transaction();
    query.prepare(sql);
    query.bindValue(":DP_NOME", _newName); // nome novo
    query.exec();
    if(query.lastError().isValid()) {
        if( query.lastError().nativeErrorCode() == 1062 ) // registro duplicado
            emit lastError( tr("[<b>%0</b>] Este departamento já está cadastrado.").arg(_newName));
        else
            emit lastError(query.lastError().text());
        QSqlDatabase::database().rollback();
    } else
        QSqlDatabase::database().commit();
}

void Department::_update()
{
    _delete();
    _save();
}

void Department::_delete()
{
    QString sql = "DELETE FROM DOCDEPARTAMENTOS WHERE DP_NOME=:DP_NOME";

    QSqlDatabase::database().transaction();
    query.prepare(sql);
    query.bindValue(":DP_NOME", _oldName); // nome velho
    query.exec();
    if(query.lastError().isValid()) {
        emit lastError(query.lastError().text());
        QSqlDatabase::database().rollback();
    } else
        QSqlDatabase::database().commit();
}

void Department::_findByName()
{
}

void Department::_findById()
{
}

