#ifndef VCPUPPERVALIDATOR_H
#define VCPUPPERVALIDATOR_H

#include <vcplib/vcplib2.h>

#include <QValidator>

class  vcpUpperValidator : public QValidator
{
    Q_OBJECT
public:
    explicit vcpUpperValidator(QObject *parent = 0);

    State validate(QString &input, int &pos) const;

signals:
    
public slots:
    
};

#endif // VCPUPPERVALIDATOR_H
