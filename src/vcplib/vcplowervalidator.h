#ifndef VCPLOWERVALIDATOR_H
#define VCPLOWERVALIDATOR_H

#include <vcplib/vcplib2.h>

#include <QValidator>

class vcpLowerValidator : public QValidator
{
    Q_OBJECT
public:
    explicit vcpLowerValidator(QObject *parent = 0);

    State validate(QString &input, int &pos) const;

signals:
    
public slots:
    
};

#endif // VCPLOWERVALIDATOR_H
