#include "apicall.h"
#include <QDebug>

static QString sendRequest(QString relativeUrl);

QString getApiUrl() {
  // may change this to read from a config file later on
  return "http://api.openitemsets.com/api";
}

QString getUserId(QString username) {
  return sendRequest("/get_user?query=" + username);
}

static QString sendRequest(QString relativeUrl) {
  // temporary event loop
  QEventLoop eventLoop;

  QNetworkAccessManager mgr;
  QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

  QNetworkRequest req(QUrl(QString(getApiUrl() + relativeUrl)));
  QNetworkReply* reply = mgr.get(req);

  // this will be blocked until the HTTP request is completed
  eventLoop.exec();

  if (reply->error() != QNetworkReply::NoError) {
    return "error: Error while making network request, please check your Internet connection"
        ", alternatively Open Item Sets may also be offline";
  }

  return QString(reply->readAll());
}
