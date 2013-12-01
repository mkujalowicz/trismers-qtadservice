#ifndef QADPLATFORMADFONIC_H
#define QADPLATFORMADFONIC_H

#include "qadplatform.h"

class QAdPlatformAdfonic : public QAdPlatform
{
public:
    QAdPlatformAdfonic();

    virtual bool prepareRequest(const QAdService &adService, QUrl &url, QByteArray &bodyData) Q_DECL_OVERRIDE;
    virtual QAd *createAdFromResponse(const QVariant& response) Q_DECL_OVERRIDE;
};

#endif // QADPLATFORMADFONIC_H
