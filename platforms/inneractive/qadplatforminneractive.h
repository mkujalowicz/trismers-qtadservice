#ifndef QADPLATFORMINNERACTIVE_H
#define QADPLATFORMINNERACTIVE_H

#include "qadplatform.h"

class QAdPlatformInneractive : public QAdPlatform
{
public:
    QAdPlatformInneractive();

    virtual bool prepareRequest(const QAdService &adService, QUrl &url, QByteArray &bodyData) Q_DECL_OVERRIDE;
    virtual QAd *createAdFromResponse(const QVariant& response) Q_DECL_OVERRIDE;
};

#endif // QADPLATFORMINNERACTIVE_H
