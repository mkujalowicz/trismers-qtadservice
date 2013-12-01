#ifndef QADSERVICEPLATFORM_H
#define QADSERVICEPLATFORM_H

#include <QObject>

class QAdMob;
class QAdMobAd;

class QAdServicePlatform : public QObject
{
    Q_OBJECT
public:
    explicit QAdServicePlatform(QObject *parent = 0);
    virtual QUrl prepareUrlForRequest(const QAdMob& adService) = 0;
    virtual QByteArray preparePostDataForRequest(const QAdMob& adService) = 0;
    virtual QAdMobAd *createAdFromResponse(const QVariant& response) = 0;

signals:

public slots:

};

#endif // QADSERVICEPLATFORM_H
