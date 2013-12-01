#ifndef QADSERVICEPLATFORMADFONIC_H
#define QADSERVICEPLATFORMADFONIC_H

#include "qadserviceplatform.h"

class QAdServicePlatformAdfonic : public QAdServicePlatform
{
public:
    QAdServicePlatformAdfonic();

    virtual QUrl prepareUrlForRequest(const QAdService& adService) Q_DECL_OVERRIDE;
    virtual QByteArray preparePostDataForRequest(const QAdService& adService) Q_DECL_OVERRIDE;
    virtual QAd *createAdFromResponse(const QVariant& response) Q_DECL_OVERRIDE;
};

#endif // QADSERVICEPLATFORMADFONIC_H
