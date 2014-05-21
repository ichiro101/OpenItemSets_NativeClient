#ifndef APICALL_H
#define APICALL_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QEventLoop>

QString getApiUrl();
QString getUserId(QString username);


#endif // APICALL_H
