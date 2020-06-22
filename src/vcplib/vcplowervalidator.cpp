#include "vcplowervalidator.h"

vcpLowerValidator::vcpLowerValidator(QObject *parent) :
    QValidator(parent)
{
}

QValidator::State vcpLowerValidator::validate(QString &input, int &pos) const
{
      input = input.toLower();
      return QValidator::Acceptable;
}
