#include "qadplatforminneractive.h"
#include <QUrlQuery>
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include "qad.h"
#include "qadservice.h"

// REFERENCE: https://inneractive.jira.com/wiki/plugins/viewsource/viewpagesrc.action?pageId=13631537

const char * kInneractiveUrlFormat = "http://m2m1.inner-active.mobi/simpleM2M/clientRequestAd";

QAdPlatformInneractive::QAdPlatformInneractive(QObject *parent): QAdPlatform(parent), m_channelId(0)
{
}

bool QAdPlatformInneractive::prepareRequest(const QAdService &adService, QUrl &url, QByteArray &/*bodyData*/)
{
    if (adService.slotId().length() == 0)
        return false;

    url = QString(kInneractiveUrlFormat);
    QUrlQuery query;

    query.addQueryItem("aid", adService.slotId());
    query.addQueryItem("v", "Sm2m-2.0.1");
    query.addQueryItem("po", QString::number(channelId()));
    query.addQueryItem("hid", adService.uniqueId());
    query.addQueryItem("cid", adService.trackingId());
    query.addQueryItem("test", adService.testMode() ? "true": "false");
    if (adService.adTypeHint() == QAdService::AdTypeHintInterstitial) {
        query.addQueryItem("fs", "true");
    }
    switch (adService.visitorGender()) {
    case QAdService::GenderMale:
        query.addQueryItem("g", "Male");
        break;
    case QAdService::GenderFemale:
        query.addQueryItem("g", "Female");
        break;
    default:
        break;
    }
    if (adService.visitorAge() >= 0)
        query.addQueryItem("a", QString::number(adService.visitorAge()));
    if (adService.keywords().length()) {
        QStringList keywordsList = adService.keywords().split(' ', QString::SkipEmptyParts);
        if (keywordsList.count())
            query.addQueryItem("k", keywordsList.join(','));
    }
    QGuiApplication *app = qobject_cast<QGuiApplication *>(QCoreApplication::instance());
    if (app) {
        QScreen *mainScreen = app->primaryScreen();
        if (mainScreen) {
            query.addQueryItem("w", QString::number(mainScreen->size().width()));
            query.addQueryItem("h", QString::number(mainScreen->size().height()));
        }
    }
    url.setQuery(query);
    qDebug() << "url is " << url;
    return true;
}

QAd *QAdPlatformInneractive::createAdFromResponse(const QVariant &response)
{
    QUrl url;
    QUrl imageUrl;
    QString text;
    QString trackingId;

    if (response.type() != QVariant::Map)
        return NULL;
    QVariantMap map = response.toMap();
        qDebug() << "Ress " << response;
    QString errorValue = map.value("Error").toString();
    if (errorValue != QString("OK") && errorValue != QString("House Ad"))
        return NULL;

    QVariant client = map.value("tns:Client");
    if (client.type() == QVariant::Map) {
        QVariantMap clientMap = client.toMap();

        trackingId = clientMap.value("Id").toString();
    }

    if (trackingId.length() == 0)
        return NULL;

    QVariant ad = map.value("tns:Ad");
    if (ad.type() == QVariant::Map) {
        QVariantMap adMap = ad.toMap();

        imageUrl = QUrl(adMap.value("tns:Image").toUrl());
        url = QUrl(adMap.value("tns:URL").toUrl());
        text = adMap.value("tns:Text").toString();
    }
    QAd *newAd = new QAd();

    newAd->setUrl(url);
    newAd->setImageUrl(imageUrl);
    newAd->setText(text);
    newAd->setTrackingId(trackingId);
    return newAd;
}

int QAdPlatformInneractive::channelId() const
{
    return m_channelId;
}

void QAdPlatformInneractive::setChannelId(int arg)
{
    if (m_channelId != arg) {
        m_channelId = arg;
        emit channelIdChanged(arg);
    }
}
