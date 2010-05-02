#ifndef QADMOB_H
#define QADMOB_H

#include <QObject>
#include <QString>
#include <QSize>
#include <QBool>
#include <QDate>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>


#if defined(QADMOB_LIBRARY)
#  define QADMOBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QADMOBSHARED_EXPORT Q_DECL_IMPORT
#endif


class QAdMob;
class QADMOBSHARED_EXPORT QAdMobAd : QObject
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(QSize size READ size)
    Q_PROPERTY(QImage adTitleImage READ adTitleImage)
    Q_PROPERTY(QImage adUrlTypeImage READ adUrlTypeImage)
    Q_PROPERTY(QString adMobStampText READ adMobStampText)

public:

    QString text() const
    {
        return iText;
    }

    QString url() const
    {
        return iUrl;
    }

    QSize size() const
    {
        return iSize;
    }

    QImage adUrlTypeImage() const
    {
        return iAdUrlTypeImage;
    }

    QImage adTitleImage() const
    {
        return iAdTitleImage;
    }

    QString adMobStampText() const
    {
        return iAdMobStampText;
    }

private:
    QString iText;
    QString iUrl;
    QSize   iSize;
    QImage iAdTitleImage;
    QImage iAdUrlTypeImage;
    QString iAdMobStampText;

    friend class QAdMob;
};

class QADMOBSHARED_EXPORT QAdMob : public QObject
{
    Q_OBJECT
    Q_ENUMS(Gender)
    Q_ENUMS(AdTypeHint)

    Q_PROPERTY(QString publisherId READ publisherId WRITE setPublisherId)
    Q_PROPERTY(QString keywords READ keywords WRITE setKeywords RESET resetKeywords)
    Q_PROPERTY(AdTypeHint adTypeHint READ adTypeHint WRITE setAdTypeHint)
    Q_PROPERTY(QBool testMode READ testMode WRITE setTestMode)
    Q_PROPERTY(QString adLanguage READ adLanguage WRITE setAdLanguage)
    Q_PROPERTY(QBool adReady READ adReady)

    /* Visitor propertyes */
    /* Read http://developer.admob.com/wiki/Requests for more info */
    Q_PROPERTY(QString visitorPostalCode READ visitorPostalCode WRITE setVisitorPostalCode RESET resetVisitorPostalCode)
    Q_PROPERTY(QString visitorAreaCode READ visitorAreaCode WRITE setVisitorAreaCode RESET resetVisitorAreaCode)
    Q_PROPERTY(QString visitorLocationCoords READ visitorLocationCoords WRITE setVisitorLocationCoords RESET resetVisitorLocationCoors)
    Q_PROPERTY(QDate visitorDOB READ visitorDOB WRITE setVisitorDOB RESET resetVisitorDOB)
    Q_PROPERTY(Gender visitorGender READ visitorGender WRITE setVisitorGender RESET resetVisitorGender)

signals:
    void adReceived(bool);

public slots:
    void fetchAd();

private slots:
    void networkReplyError ( QNetworkReply::NetworkError aCode );
    void networkReplyFinished ();

    void handleAdTitleImageDownload();
    void handleAdTargetImageDownload();

public:
    explicit QAdMob(QObject *parent = 0);

    enum Gender { GenderUnknown = 0, GenderMale , GenderFemale };
    enum AdTypeHint { AdTypeHintText = 0, AdTypeHintBanner };

    void setPublisherId(const QString&);
    QString publisherId() const;

    void setKeywords(const QString&); /* Space separated keywords */
    QString keywords() const;
    void resetKeywords();

    void setAdTypeHint( QAdMob::AdTypeHint );
    QAdMob::AdTypeHint adTypeHint() const;

    void setTestMode(const QBool&);
    QBool testMode() const;

    QString adLanguage() const;
    void setAdLanguage(const QString& );

    void setVisitorAreaCode( const QString& );
    QString visitorAreaCode() const;
    void resetVisitorAreaCode();

    void setVisitorPostalCode( const QString& );
    QString visitorPostalCode() const;
    void resetVisitorPostalCode();

    void setVisitorLocationCoords( const QString& );
    QString visitorLocationCoords() const;
    void resetVisitorLocationCoors();

    void setVisitorDOB( const QDate& );
    QDate visitorDOB() const;
    void resetVisitorDOB();

    void setVisitorGender( QAdMob::Gender );
    QAdMob::Gender visitorGender() const;
    void resetVisitorGender();

    QBool adReady() const;

    const QAdMobAd& ad() const;

private:
    QString genDataString() const;
    void handleResponseData( const QByteArray& aResponseData );


    void handleAdReady();

    QString iPublisherId;
    QString iKeywords;
    AdTypeHint iAdTypeHint;
    QBool   iTestMode;
    QString iAdLanguage;

    QString iVisitorAreaCode;
    QString iVisitorPostalCode;
    QString iVisitorLocationCoords;
    QDate   iVisitorDob;
    Gender  iVisitorGender;


    QNetworkAccessManager iNam;

    QAdMobAd iAd;
    QBool   iAdReady;

};

#endif // QADMOB_H
