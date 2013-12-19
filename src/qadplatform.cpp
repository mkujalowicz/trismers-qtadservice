#include "qadplatform.h"

QAdPlatform::QAdPlatform(QObject *parent) :
    QObject(parent)
{
}

QByteArray QAdPlatform::processResponseData(const QByteArray &bodyData)
{
    return QByteArray(bodyData);
}
