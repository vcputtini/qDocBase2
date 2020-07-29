#include "vcpftp.h"

vcpFtp::vcpFtp(QObject *parent) :
   QObject(parent),
    __fname(QString())
{
    manager = new QNetworkAccessManager;
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

vcpFtp::~vcpFtp()
{
    if(manager)
        delete manager;
}

// $SLOTS
void vcpFtp::cancelDownload(void)
{
    ;
}

void vcpFtp::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit dataReceived(bytesReceived, bytesTotal);
}

void vcpFtp::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this,
            SLOT(downloadProgress(qint64,qint64)));
}

void vcpFtp::execute()
{
    QUrl url = QUrl::fromEncoded(__fname.toLocal8Bit());
    doDownload(url);

}

void vcpFtp::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        if(reply->error() == 203)
            emit error( tr("Não há atualizações dispníveis no momento.").arg(reply->error()) );
        else
            emit error( tr("Download falhou. Erro: %1").arg(reply->error()) );
    } else {
        QString filename = saveFileName(url);
        if (saveToDisk(filename, reply)) {
            emit error(tr("Download terminou com sucesso"));
        }
    }

    reply->deleteLater();
    return;
}

// METODOS
void vcpFtp::setFileName(const QString fname)
{
    __fname = fname;
}

QString vcpFtp::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

bool vcpFtp::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        emit error(tr("Não pode salvar o arquivo. Erro: %1").arg(file.errorString()));
        return false;
    }
    file.write(data->readAll());
    file.close();
    return true;
}
