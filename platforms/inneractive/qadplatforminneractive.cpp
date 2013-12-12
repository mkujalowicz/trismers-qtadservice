#include "qadplatforminneractive.h"
#include <QUrlQuery>
#include "qad.h"
#include "qadservice.h"

// REFERENCE: https://inneractive.jira.com/wiki/plugins/viewsource/viewpagesrc.action?pageId=13631537

const char * kInneractiveUrlFormat = "http://m2m1.inner-active.mobi/simpleM2M/clientRequestAd";

QAdPlatformInneractive::QAdPlatformInneractive()
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
    query.addQueryItem("po", "947");
    query.addQueryItem("hid", adService.uniqueId());
    url.setQuery(query);
    return true;
}

QAd *QAdPlatformInneractive::createAdFromResponse(const QVariant &response)
{
    return NULL;
}
