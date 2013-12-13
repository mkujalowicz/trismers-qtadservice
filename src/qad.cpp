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

void QAd::setText(QString arg)
{
    if (m_text != arg) {
        m_text = arg;
        emit textChanged(arg);
    }
}

void QAd::setTrackingId(QString arg)
{
    if (m_trackingId != arg) {
        m_trackingId = arg;
        emit trackingIdChanged(arg);
    }
}
