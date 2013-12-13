#include "qadplatformadfonic.h"
#include <QUrlQuery>
#include <QCoreApplication>
#include "qad.h"
#include "qadservice.h"

const char * kAdfonicUrlFormat = "http://adfonic.net/ad/%1";

QAdPlatformAdfonic::QAdPlatformAdfonic()
{
}

bool QAdPlatformAdfonic::prepareRequest(const QAdService &adService, QUrl &url, QByteArray &/*bodyData*/)
{
    if (adService.slotId().length() == 0)
        return false;

    url = QString(kAdfonicUrlFormat).arg(adService.slotId());
    QUrlQuery query;

    query.addQueryItem("r.id", adService.trackingId().length() ? adService.trackingId(): adService.uniqueId());
    query.addQueryItem("s.test", adService.testMode() ? "1": "0");
    query.addQueryItem("t.markup", "0");
    query.addQueryItem("t.format", "json");

    QCoreApplication *app = QCoreApplication::instance();
    QString clientId = QString("%1/%2/%3").arg(app->organizationName(), app->applicationName(), app->applicationVersion());
    query.addQueryItem("r.client", clientId);

    url.setQuery(query);
    return true;
}

QAd *QAdPlatformAdfonic::createAdFromResponse(const QVariant &response)
{
    QUrl url;
    QUrl imageUrl;
    QAd::Format adFormat = QAd::Null;

    if (response.type() != QVariant::Map)
        return NULL;
    QVariantMap map = response.toMap();
    if (map.value("status") != QString("success"))
        return NULL;

    QVariant format = map.value("format");
    if (format.type() == QVariant::String) {
        QString formatString = format.toString();

        if (formatString == "banner") {
            adFormat = QAd::Banner;
        } else {
            if (formatString == "text") {
                adFormat = QAd::Text;
            } else {
                if (formatString.indexOf("image") == 0) {
                    adFormat = QAd::Image;
                }
            }
        }
    }

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
    QAd *newAd = new QAd();

    newAd->setUrl(url);
    newAd->setImageUrl(imageUrl);
    return newAd;
}
