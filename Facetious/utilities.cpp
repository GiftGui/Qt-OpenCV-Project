#include <QObject>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostInfo>
#include <QDebug>

#include "utilities.h"

QString Utilities::getDataPath()
{
    QDir pictures_dir(qApp->applicationDirPath());
    pictures_dir.mkpath("pictures");
    return pictures_dir.absoluteFilePath("pictures");
}

QString Utilities::newPhotoName()
{
    QDateTime time = QDateTime::currentDateTime();
    return time.toString("yyyy-MM-dd+HH:mm:ss");
}

QString Utilities::getPhotoPath(QString name, QString postfix)
{
    return QString("%1/%2.%3").arg(Utilities::getDataPath(), name, postfix);
}

