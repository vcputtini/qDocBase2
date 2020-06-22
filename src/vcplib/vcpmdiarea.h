#ifndef VCPMDIAREA_H
#define VCPMDIAREA_H

#include <vcplib/vcplib2.h>

#include <QMdiArea>
#include <QPainter>
#include <QString>
#include <QImage>
#include <QFont>
#include <QFontMetricsF>



/**
 * @brief The vcpMdiArea class
 *        Subclasse destinada a criar a interface MDI de uma
 *        aplicacao qualquer.
 *        Padroniza a apresentacao das interfaces MDI dos
 *        aplicativos usando logotipos centralizados
 */
class vcpMdiArea : public QMdiArea
{
  Q_OBJECT
public:
    vcpMdiArea(QWidget *parent = 0, QString nomeprog=QString());
    virtual ~vcpMdiArea() { }

private:
    QString NomeProg;

protected slots:
    void paintEvent(QPaintEvent *);
};


#endif // VCPMDIAREA_H
