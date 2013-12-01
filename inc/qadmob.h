/*
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef QADMOB_H
#define QADMOB_H

#include <QtQml/qqml.h>
#include <QObject>
#include <QString>
#include <QSize>
#include <QDate>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "qadmobad.h"
#include "qadserviceplatform.h"

#define Q_ADMOB_EXPORT


/**
  * @brief Used to fetch ads from the admob servers
  */
class Q_ADMOB_EXPORT QAdService : public QObject
{
    Q_OBJECT

    Q_ENUMS(Status)
    Q_ENUMS(Gender)
    Q_ENUMS(AdTypeHint)

    Q_PROPERTY(QAd *ad READ ad NOTIFY adChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QAdServicePlatform *platform READ platform WRITE setPlatform NOTIFY platformChanged)

    Q_PROPERTY(QString publisherId READ publisherId WRITE setPublisherId NOTIFY publisherIdChanged)
    Q_PROPERTY(QString keywords READ keywords WRITE setKeywords RESET resetKeywords NOTIFY keywordsChanged)
    Q_PROPERTY(AdTypeHint adTypeHint READ adTypeHint WRITE setAdTypeHint NOTIFY adTypeHintChanged)
    Q_PROPERTY(bool testMode READ testMode WRITE setTestMode NOTIFY testModeChanged)
    Q_PROPERTY(QString adLanguage READ adLanguage WRITE setAdLanguage NOTIFY adLanguageChanged)

public:

    enum Status { Null = 0, Ready, Loading, Error };

    /**
      * @brief Used to specify the gender of th use. Used to for better ad targetting by AdMob
      */
    enum Gender { GenderUnknown = 0, GenderMale , GenderFemale };

    /**
      * @brief Used to specify the type of ad you want, a banner or a text ad
      */
    enum AdTypeHint { AdTypeHintText = 0, AdTypeHintBanner };

signals:
    void statusChanged(Status);

    void publisherIdChanged(const QString &arg);

    void keywordsChanged(const QString &arg);

    void adTypeHintChanged(AdTypeHint arg);

    void testModeChanged(bool arg);

    void adLanguageChanged(const QString &arg);

    void adChanged(QAd * arg);

    void platformChanged(QAdServicePlatform * arg);

public slots:
    /**
      * @brief starts the ad fetching process, signal adReceived is triggered as a result.
      */
    Q_INVOKABLE void fetchAd();

private slots:
    void networkReplyFinished ();

public:
    explicit QAdService(QObject *parent = 0);

    ~QAdService();

    Status status() const;

    /**
      * @brief Set the publisher Id, see AdMob pages for your publisher id
      * @param aPublisherId your publisher id
      */
    void setPublisherId(const QString& aPublisherId);

    /**
      * @returns QString, your publisher id
      */
    QString publisherId() const;

    /**
      * @brief Sets the keywords, AdMob uses show targeted ads, keywords are seperated by spaces
      */
    void setKeywords(const QString&);

    /**
      * @returns QString, the keyrds you have set @see setKeywords
      */
    QString keywords() const;

    /**
      * @brief reset keywords
      */
    void resetKeywords();

    /**
      * @brief Used to specify what type of ad you want, a banner or a text ad
      * @param aType @see AdTypeHint
      */
    void setAdTypeHint( QAdService::AdTypeHint aType);

    /**
      * @brief returns the ad type hint you have set.
      * @returns QAdMob::AdTypeHint
      */
    QAdService::AdTypeHint adTypeHint() const;

    /**
      * @brief Used to enable test mode, if test mode is enable AdMob will always send you test ad.
      */
    void setTestMode(bool aMode);
    bool testMode() const;

    QString adLanguage() const;
    void setAdLanguage(const QString& );

    QAd *ad();

    QAdServicePlatform * platform() const;
    void setPlatform(QAdServicePlatform * arg);

private:
    void fetchAdFromUrl(const QUrl &, const QByteArray&);
    bool handleResponseData( const QByteArray& aResponseData );
    QVariant parseJsonResponseData( const QByteArray& aResponseData );
    void setStatus(Status);
    void setAd(QAd *);
private:
    Status m_status;
    QNetworkReply *m_reply;
    QAdServicePlatform * m_platform;

    QString     m_publisherId;
    QString     m_keywords;
    AdTypeHint  m_adTypeHint;
    bool        m_testMode;
    QString     m_adLanguage;
    QAd    *m_ad;
};

#endif // QADMOB_H
