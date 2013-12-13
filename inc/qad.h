#ifndef QAD_H
#define QAD_H

#include <QObject>
#include <QUrl>

/**
 * @brief Represents an Ad
 */
class QAd : public QObject
{
    Q_OBJECT

    Q_ENUMS(Format)

    Q_PROPERTY(Format format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString trackingId READ trackingId WRITE setTrackingId NOTIFY trackingIdChanged)

public:
    enum Format {Null, Banner, Text, Image};

signals:
    void urlChanged(const QUrl &arg);

    void imageUrlChanged(QUrl arg);

    void formatChanged(Format arg);

    void textChanged(QString arg);

    void trackingIdChanged(QString arg);

public:

    explicit QAd(QObject *parent = 0);

    const QUrl &url() const
    {
        return m_url;
    }

    QUrl imageUrl() const
    {
        return m_imageUrl;
    }

    Format format() const
    {
        return m_format;
    }

    QString text() const
    {
        return m_text;
    }

    QString trackingId() const
    {
        return m_trackingId;
    }

    void setUrl(const QUrl &);
    void setImageUrl(QUrl arg);
    void setFormat(Format arg);
    void setText(QString arg);
    void setTrackingId(QString arg);

private:
    QUrl m_url;
    QUrl m_imageUrl;
    Format m_format;
    QString m_text;
    QString m_trackingId;
};

#endif // QAD_H
