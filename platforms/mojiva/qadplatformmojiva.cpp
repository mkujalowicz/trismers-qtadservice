#include "qadplatformmojiva.h"
#include <QUrlQuery>
#include <QHostInfo>
#include "qad.h"
#include "qadservice.h"

const char * kMojivaAdUrl = "http://ads.mojiva.com/ad";

QAdPlatformMojiva::QAdPlatformMojiva()
{
}

bool QAdPlatformMojiva::prepareRequest(const QAdService &adService, QUrl &url, QByteArray &/*bodyData*/)
{
    if (adService.slotId().length() == 0)
        return false;

    url = QString(kMojivaAdUrl);
    QUrlQuery query;

    query.addQueryItem("zone", adService.slotId());
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(adService.uniqueId().toUtf8());
    query.addQueryItem("udid", hash.result().toHex());
    /*QList<QHostAddress> ipAddresses = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
    if (ipAddresses.count()) {
        query.addQueryItem("ip", ipAddresses[0].toString());
    }
    if (adService.userAgent().length()) {
        query.addQueryItem("ua", adService.userAgent());
    }*/
    int adType = 0;
    if (adService.adTypeHint() == QAdService::AdTypeHintText) {
        adType = 1;
    } else {
        if (adService.adTypeHint() == QAdService::AdTypeHintBanner)
            adType = 3;
    }
    query.addQueryItem("key", "5"); //json
    if (adType) {
        query.addQueryItem("type", QString::number(adType));
    }
    query.addQueryItem("test", adService.testMode() ? "1": "0");
    query.addQueryItem("size_required", "1");
    url.setQuery(query);
    return true;
}

QAd *QAdPlatformMojiva::createAdFromResponse(const QVariant &response)
{
    QUrl url;
    QUrl imageUrl;
    QUrl trackingUrl;

    if (response.type() != QVariant::Map)
        return NULL;
    QVariantMap map = response.toMap();

    if (map.value("error").type() != QVariant::Invalid)
        return NULL;

    url = map.value("url").toUrl();

    if (map.value("img").type() != QVariant::List)
        return NULL;

    QVariantList imgList = map.value("img").toList();
    if (imgList.count() == 0)
        return NULL;

    imageUrl = imgList[0].toUrl();

    QVariantList trackingList = map.value("track").toList();
    if (trackingList.count() == 0)
        return NULL;

    trackingUrl = trackingList[0].toUrl();

    QAd *newAd = new QAd();

    newAd->setUrl(url);
    newAd->setImageUrl(imageUrl);
    newAd->setTrackingUrl(trackingUrl);
    return newAd;
}

QByteArray QAdPlatformMojiva::processResponseData(const QByteArray &bodyData)
{
    QByteArray data = bodyData;

    if (data.startsWith('(')) {
        data.remove(0, 1);
    }
    if (data.endsWith(')')) {
        data.remove(data.length() - 1, 1);
    }
    return data;
}
