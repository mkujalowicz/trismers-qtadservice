#ifndef QADPLATFORMINNERACTIVE_H
#define QADPLATFORMINNERACTIVE_H

#include "qadplatform.h"

class QAdPlatformInneractive : public QAdPlatform
{
    Q_OBJECT
    Q_PROPERTY(int channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)

signals:
    void channelIdChanged(int arg);

public:
    explicit QAdPlatformInneractive(QObject *parent = 0);

    virtual bool prepareRequest(const QAdService &adService, QUrl &url, QByteArray &bodyData) Q_DECL_OVERRIDE;
    virtual QAd *createAdFromResponse(const QVariant& response) Q_DECL_OVERRIDE;
    int channelId() const;
    void setChannelId(int);

private:
    int m_channelId;
};

#endif // QADPLATFORMINNERACTIVE_H
