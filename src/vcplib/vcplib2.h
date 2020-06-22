#ifndef VCPLIB2_H
#define VCPLIB2_H

#include <QApplication>
#include <QWidget>

#include <QString>
#include <QDate>

#include <QDebug>

#include "vcplowervalidator.h"
#include "vcpuppervalidator.h"
#include "vcpmdiarea.h"
#include "vcpdoublevalidator.h"
#include "cnpj.h"
#include "cpf.h"


/**
 * @brief The VCPLib2 class
 */
class VCPLib2
{
    
public:
    VCPLib2();

    enum DateFormats { Eur, Iso, Internal, SqlTimeStamp };
    QString today(DateFormats df=Eur);
    QString toISO(const QString);
    QString toEUR(const QString);

    QString organization() const;
};


#endif // VCPLIB2_H
