#ifndef VCPFTP_H
#define VCPFTP_H

#include <vcplib/vcplib2.h>

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QString>
#include <QList>
#include <QProgressDialog>
#include <QIODevice>
#include <QUrl>

#include <QFile>
#include <QFileInfo>

class vcpFtp : public QObject
{
    Q_OBJECT
public:
    explicit vcpFtp(QObject *parent = 0);
    ~vcpFtp();

    void setFileName(const QString fname=QString());

private:
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);

private:
    QNetworkAccessManager *manager;
    QList<QNetworkReply *> currentDownloads;
    QProgressDialog *progress;

    QString __fname;

signals:
    void error(const QString &);
    void dataReceived(qint64, qint64);

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void cancelDownload(void);
};

#endif // VCPFTP_H

