#ifndef QADPLATFORMMOJIVA_H
#define QADPLATFORMMOJIVA_H

#include "qadplatform.h"

class QAdPlatformMojiva : public QAdPlatform
{
public:
    QAdPlatformMojiva();

    virtual bool prepareRequest(const QAdService &adService, QUrl &url, QByteArray &bodyData) Q_DECL_OVERRIDE;
    virtual QAd *createAdFromResponse(const QVariant& response) Q_DECL_OVERRIDE;
    virtual QByteArray processResponseData(const QByteArray &bodyData) Q_DECL_OVERRIDE;
};

#endif // QADPLATFORMMOJIVA_H
