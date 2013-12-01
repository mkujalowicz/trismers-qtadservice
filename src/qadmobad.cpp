#include "qadmobad.h"

QAd::QAd(QObject *parent) :
    QObject(parent)
{
}

void QAd::setUrl(const QUrl &arg)
{
    if (m_url != arg) {
        m_url = arg;
        emit urlChanged(arg);
    }
}

void QAd::setImageUrl(QUrl arg)
{
    if (m_imageUrl != arg) {
        m_imageUrl = arg;
        emit imageUrlChanged(arg);
    }
}
