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

    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
signals:
    void urlChanged(const QUrl &arg);

    void imageUrlChanged(QUrl arg);

public:
    enum Format {Banner, Text, Image};

    explicit QAd(QObject *parent = 0);
    const QUrl &url() const
    {
        return m_url;
    }

    QUrl imageUrl() const
    {
        return m_imageUrl;
    }

    void setUrl(const QUrl &);
    void setImageUrl(QUrl arg);

private:
    QUrl m_url;
    QUrl m_imageUrl;
};

#endif // QAD_H
