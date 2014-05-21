#ifndef APICALL_H
#define APICALL_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <vector>
#include "settings.h"

QString getApiUrl();
QString getUserId(QString username);
std::vector<QString> getUserSubscription();
QString getStateHash();
QString getItemSet(QString itemSetId);


#endif // APICALL_H
