#ifndef QADSERVICEPLATFORM_H
#define QADSERVICEPLATFORM_H

#include <QObject>

class QAdService;
class QAd;

class QAdServicePlatform : public QObject
{
    Q_OBJECT
public:
    explicit QAdServicePlatform(QObject *parent = 0);
    virtual QUrl prepareUrlForRequest(const QAdService& adService) = 0;
    virtual QByteArray preparePostDataForRequest(const QAdService& adService) = 0;
    virtual QAd *createAdFromResponse(const QVariant& response) = 0;

signals:

public slots:

};

#endif // QADSERVICEPLATFORM_H
