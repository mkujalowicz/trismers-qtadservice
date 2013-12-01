#include "qad.h"

QAd::QAd(QObject *parent) :
    QObject(parent), m_format(Null)
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

void QAd::setFormat(QAd::Format arg)
{
    if (m_format != arg) {
        m_format = arg;
        emit formatChanged(arg);
    }
}
