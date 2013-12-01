#ifndef QADPLATFORMADFONIC_H
#define QADPLATFORMADFONIC_H

#include "qadplatform.h"

class QAdPlatformAdfonic : public QAdPlatform
{
public:
    QAdPlatformAdfonic();

    virtual QUrl prepareUrlForRequest(const QAdService& adService) Q_DECL_OVERRIDE;
    virtual QByteArray preparePostDataForRequest(const QAdService& adService) Q_DECL_OVERRIDE;
    virtual QAd *createAdFromResponse(const QVariant& response) Q_DECL_OVERRIDE;
};

#endif // QADPLATFORMADFONIC_H
