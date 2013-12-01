#ifndef QADPLATFORM_H
#define QADPLATFORM_H

#include <QObject>

class QAdService;
class QAd;

class QAdPlatform : public QObject
{
    Q_OBJECT
public:
    explicit QAdPlatform(QObject *parent = 0);
    virtual QUrl prepareUrlForRequest(const QAdService& adService) = 0;
    virtual QByteArray preparePostDataForRequest(const QAdService& adService) = 0;
    virtual QAd *createAdFromResponse(const QVariant& response) = 0;

signals:

public slots:

};

#endif // QADPLATFORM_H
