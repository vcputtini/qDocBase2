#ifndef VCPDOUBLEVALIDATOR_H
#define VCPDOUBLEVALIDATOR_H

#include <QObject>
#include <QDoubleValidator>
#include <QString>

#include <limits.h>
#include <math.h>

#include <vcplib/vcplib2.h>


/**
  * @brief Para uso na classe vcpDoubleValidator
  *
  */
#ifndef LLONG_MAX
#   define LLONG_MAX Q_INT64_C(0x7fffffffffffffff)
#endif

using namespace std;


/**
  * @class vcpDoubleValidator
  * @author Volnei Cervi Puttini
  * @date 2011-02-17
  * @update 2013-05-23
  * @brief Valida corretamente a digitacao de numero com ponto decimal
  *
  * Faz com que os parametros top, bottol e decimals sejam rigorosamente
  * respeitados durante a digitacao do numero decimal.
  * Ou seja, se um numero for definido como por exemplo: bottom=0.0 top=9999.99 decimals=2
  * ira permitir a digitacao do numero apenas dentro dos limites estabelecidos.
  * no formato 9999.99 como dado neste exemplo.
  *
  * %ChangeLog%
  * 2013-05-23 Atualizacao para versao Qt5
  *
  */

class vcpDoubleValidator : public QDoubleValidator
{
    Q_OBJECT
public:
    vcpDoubleValidator( double bottom, double top, int decimals, QObject* parent = 0 );

    virtual QValidator::State validate(QString &input, int &pos) const;

private:

  /* Funcoes auxiliares */
  enum NumberMode { IntegerMode, DoubleStandardMode, DoubleScientificMode };
  bool validateChars(const QString &str, NumberMode numMode, QByteArray *buff,
                                      int decDigits) const;

  inline int numDigits(qlonglong n) const;
  inline char digitToCLocale(const QChar &in) const;

  qlonglong pow10(int n) const;

signals:

public slots:

};


#endif // VCPDOUBLEVALIDATOR_H
