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

public:
    enum Format {Null, Banner, Text, Image};

signals:
    void urlChanged(const QUrl &arg);

    void imageUrlChanged(QUrl arg);

    void formatChanged(Format arg);

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

    void setUrl(const QUrl &);
    void setImageUrl(QUrl arg);
    void setFormat(Format arg);

private:
    QUrl m_url;
    QUrl m_imageUrl;
    Format m_format;
};

#endif // QAD_H
