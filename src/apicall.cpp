#include "apicall.h"
#include <QDebug>
#include <cassert>

static QString sendRequest(QString relativeUrl);

QString getApiUrl() {
  // may change this to read from a config file later on
  return "http://api.openitemsets.com/api";
}

QString getUserId(QString username) {
  return sendRequest("/get_user?query=" + username);
}

QString getStateHash() {
  auto userId = Settings::getInstance().getUserId();
  return sendRequest("/get_user_hash?query=" + userId);
}

std::vector<QString> getUserSubscription() {
  auto userId = Settings::getInstance().getUserId();
  auto rawData = sendRequest("/get_user_subscription?query=" + userId);

  QJsonDocument loadDoc = QJsonDocument::fromJson(rawData.toUtf8());
  QJsonArray subJsonArray = loadDoc.array();
  std::vector<QString> subArray;
  for(int i = 0; i < subJsonArray.count(); i++) {
    int setIdInt = subJsonArray[i].toInt();
    QString setIdStr = QString::number(setIdInt);
    assert(setIdStr.length() != 0);
    subArray.push_back(setIdStr);
  }

  return subArray;
}

QString getItemSet(QString itemSetId) {
  assert(itemSetId.length() != 0);
  return sendRequest("/get_item_set?query=" + itemSetId);
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
    throw "error: Error while making network request, please check your Internet connection"
        ", alternatively Open Item Sets may also be offline";
  }

  return QString(reply->readAll());
}
