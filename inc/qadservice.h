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

#ifndef QADSERVICE_H
#define QADSERVICE_H

#include <QtQml/qqml.h>
#include <QObject>
#include <QString>
#include <QSize>
#include <QDate>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "qad.h"
#include "qadplatform.h"

#define Q_ADSERVICE_EXPORT


/**
  * @brief Used to fetch ads from the ad platform
  */
class Q_ADSERVICE_EXPORT QAdService : public QObject
{
    Q_OBJECT

    Q_ENUMS(Status)
    Q_ENUMS(Gender)
    Q_ENUMS(AdTypeHint)

    Q_PROPERTY(QAd *ad READ ad NOTIFY adChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QAdPlatform *platform READ platform WRITE setPlatform NOTIFY platformChanged)
    Q_PROPERTY(QString uniqueId READ uniqueId WRITE setUniqueId NOTIFY uniqueIdChanged)

    Q_PROPERTY(QString slotId READ slotId WRITE setSlotId NOTIFY slotIdChanged)
    Q_PROPERTY(QString keywords READ keywords WRITE setKeywords RESET resetKeywords NOTIFY keywordsChanged)
    Q_PROPERTY(AdTypeHint adTypeHint READ adTypeHint WRITE setAdTypeHint NOTIFY adTypeHintChanged)
    Q_PROPERTY(bool testMode READ testMode WRITE setTestMode NOTIFY testModeChanged)
    Q_PROPERTY(QString adLanguage READ adLanguage WRITE setAdLanguage NOTIFY adLanguageChanged)

public:

    enum Status { Null = 0, Ready, Loading, Error };

    /**
      * @brief Used to specify the gender of th use. Used to for better ad targetting by AdService
      */
    enum Gender { GenderUnknown = 0, GenderMale , GenderFemale };

    /**
      * @brief Used to specify the type of ad you want, a banner or a text ad
      */
    enum AdTypeHint { AdTypeHintText = 0, AdTypeHintBanner };

signals:
    void statusChanged(Status);

    void slotIdChanged(const QString &arg);

    void keywordsChanged(const QString &arg);

    void adTypeHintChanged(AdTypeHint arg);

    void testModeChanged(bool arg);

    void adLanguageChanged(const QString &arg);

    void adChanged(QAd * arg);

    void platformChanged(QAdPlatform * arg);

    void uniqueIdChanged(const QString &arg) const;

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
      * @brief Set the slot Id
      */
    void setSlotId(const QString&);

    /**
      * @returns QString, your publisher id
      */
    QString slotId() const;

    /**
      * @brief Sets the keywords, AdService uses show targeted ads, keywords are seperated by spaces
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
      * @returns QAdService::AdTypeHint
      */
    QAdService::AdTypeHint adTypeHint() const;

    /**
      * @brief Used to enable test mode, if test mode is enabled, the ad service will always send you a test ad.
      */
    void setTestMode(bool aMode);
    bool testMode() const;

    QString adLanguage() const;
    void setAdLanguage(const QString&);

    QAd *ad();

    QAdPlatform * platform() const;
    void setPlatform(QAdPlatform * arg);

    QString uniqueIdForRequest() const;

private:
    void fetchAdFromUrl(const QUrl &, const QByteArray&);
    bool handleResponseData( const QByteArray& aResponseData );
    QVariant parseJsonResponseData( const QByteArray& aResponseData );
    void setStatus(Status);
    void setAd(QAd *);
    QString uniqueId() const;
    void setUniqueId(const QString &);
private:
    Status m_status;
    QNetworkReply   *m_reply;
    QAdPlatform     *m_platform;

    QString         m_slotId;
    QString         m_keywords;
    AdTypeHint      m_adTypeHint;
    bool            m_testMode;
    QString         m_adLanguage;
    QAd             *m_ad;
    QByteArray      m_bodyData;
    int             m_redirectCount;
    mutable QString         m_uniqueId;
};

#endif // QADSERVICE_H
