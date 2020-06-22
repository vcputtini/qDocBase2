#include "vcpuppervalidator.h"

vcpUpperValidator::vcpUpperValidator(QObject *parent) :
    QValidator(parent)
{
}

QValidator::State vcpUpperValidator::validate(QString &input, int &pos) const
{
      input = input.toUpper();
      return QValidator::Acceptable;
}
