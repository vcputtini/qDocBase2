#ifndef CPF_H
#define CPF_H

#include <vcplib/vcplib2.h>
#include <QString>


class CPF
{
public:
    explicit CPF() { }
    explicit CPF(const QString cpf);

protected:
    QString _cpf;
    int _dv;
    bool _valid;

    struct stDelimiter {
        int pos;
        QString delemiter;
    };
    struct Section {
        QString first;
        QString dv;
    };


    void normalize_P();
    int calcDV();

public:
    CPF operator=(CPF cpf2);
    bool operator==(const CPF &cpf2) const;
    bool operator!=(const CPF &cpf2) const;

    enum CPFFormats { NumericOnly, Delimited };
    QString data(CPFFormats format=NumericOnly);
    bool isValid();
    Section section();
    int DV();
};

#endif // CPF_H
