#include "qadmobad.h"

QAdMobAd::QAdMobAd(QObject *parent) :
    QObject(parent)
{
}

void QAdMobAd::setUrl(const QUrl &arg)
{
    if (m_url != arg) {
        m_url = arg;
        emit urlChanged(arg);
    }
}

void QAdMobAd::setImageUrl(QUrl arg)
{
    if (m_imageUrl != arg) {
        m_imageUrl = arg;
        emit imageUrlChanged(arg);
    }
}
