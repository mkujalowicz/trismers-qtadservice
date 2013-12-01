#include "qadserviceplatformadfonic.h"
#include <QUrlQuery>
#include "qadmobad.h"
#include "qadmob.h"

const QString kAdfonicUrlFormat = "http://adfonic.net/ad/%1";

QAdServicePlatformAdfonic::QAdServicePlatformAdfonic()
{
}

QUrl QAdServicePlatformAdfonic::prepareUrlForRequest(const QAdMob &adService)
{
    if (adService.publisherId().length() == 0)
        return QUrl();

    QUrl url(QString(kAdfonicUrlFormat).arg(adService.publisherId()));
    QUrlQuery query;

    query.addQueryItem("r.id", "58989235844-232");
    query.addQueryItem("s.test", adService.testMode() ? "1": "0");
    query.addQueryItem("t.markup", "0");
    query.addQueryItem("t.format", "json");
    url.setQuery(query);
    return url;
}

QByteArray QAdServicePlatformAdfonic::preparePostDataForRequest(const QAdMob &)
{
    return QByteArray();
}

QAdMobAd *QAdServicePlatformAdfonic::createAdFromResponse(const QVariant &response)
{
    QUrl url;
    QUrl imageUrl;

    if (response.type() != QVariant::Map)
        return NULL;
    QVariantMap map = response.toMap();
    if (map.value("status") != QString("success"))
        return NULL;

    QVariant destination = map.value("destination");
    if (destination.type() == QVariant::Map) {
        QVariantMap destinationMap = destination.toMap();

        if (destinationMap.value("type") == QString("URL")) {
            url = QUrl(destinationMap.value("url").toUrl());
        }
    }
    QVariant components = map.value("components");
    if (components.type() == QVariant::Map) {
        QVariantMap componentsMap = components.toMap();
        QVariant imageComponent = componentsMap.value("image");

        if (imageComponent.type() == QVariant::Map) {
            QVariantMap imageComponentMap = imageComponent.toMap();

            imageUrl = QUrl(imageComponentMap.value("url").toUrl());
        }
    }
    QAdMobAd *newAd = new QAdMobAd();

    newAd->setUrl(url);
    newAd->setImageUrl(imageUrl);
    return newAd;
}
