#ifndef CNPJ_H
#define CNPJ_H

#include <vcplib/vcplib2.h>

#include <QString>
#include <QStringList>
#include <QDebug>

class CNPJ
{
public:
    explicit CNPJ() { }
    explicit CNPJ(QString cnpj);

protected:
    QString _cnpj;
    int _dv;

    void normalize_P();
    int calcDV_P();

    bool _valid;
    struct stDelimiter {
        int pos;
        QString delemiter;
    };

    struct Section {
        QString first;
        QString second;
        QString dv;
    };

public: // utilitarios

    CNPJ operator=(CNPJ cnpj2);
    bool operator==(const CNPJ &cnpj2) const;
    bool operator!=(const CNPJ &cnpj2) const;

    enum CNPJFormats { NumericOnly, Delimited };

    bool isValid();
    QString data(CNPJFormats format=NumericOnly);
    int DV();
    Section section();
};

#endif // CNPJ_H

