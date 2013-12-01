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

#include <QStringList>
#include <QNetworkRequest>
#include <QDebug>
#include <QSize>
#include <QQmlEngine>

#include "qadmob.h"
#ifdef QADMOB_QT4
#include "parser.h"
#else
#include <QJsonDocument>
#endif

#define QADMOB_MAXIMUM_REDIRECT_RECURSION 4

// CONSTANTS
static const QString kUserAgent ="Mozilla/5.0 (SymbianOS/9.4; Series60/5.0 NokiaN97-1/12.0.024; Profile/MIDP-2.1 Configuration/CLDC-1.1; en-us) AppleWebKit/525 (KHTML, like Gecko) BrowserNG/7.1.12344";

QAdService::QAdService(QObject *parent) :
    QObject(parent), m_status(Null), m_reply(NULL), m_platform(NULL), m_adTypeHint(AdTypeHintText),
    m_testMode ( false ), m_ad(new QAd(this))
{
}

QAdService::~QAdService()
{
    if (m_reply) {
        m_reply->abort();
        delete m_reply;
        m_reply = NULL;
    }
}

QAdService::Status QAdService::status() const
{
    return m_status;
}


void QAdService::setPublisherId(const QString& arg)
{
    if (m_publisherId != arg) {
        m_publisherId = arg;
        emit publisherIdChanged(arg);
    }
}

QString QAdService::publisherId() const
{
    return m_publisherId;
}

void QAdService::setKeywords(const QString& arg)
{
    if (m_keywords != arg) {
        m_keywords = arg;
        emit keywordsChanged(arg);
    }
}

QString QAdService::keywords() const
{
    return m_keywords;
}

void QAdService::resetKeywords()
{
    if (m_keywords.length()) {
        m_keywords.clear();
        emit keywordsChanged(m_keywords);
    }
}

void QAdService::setAdTypeHint( QAdService::AdTypeHint arg)
{
    if (m_adTypeHint != arg) {
        m_adTypeHint = arg;
        emit adTypeHintChanged(arg);
    }
}

QAdService::AdTypeHint QAdService::adTypeHint() const
{
    return m_adTypeHint;
}

void QAdService::setTestMode(bool arg)
{
    if (m_testMode != arg) {
        m_testMode = arg;
        emit testModeChanged(arg);
    }
}

bool QAdService::testMode() const
{
    return m_testMode;
}


QString QAdService::adLanguage() const
{
    return m_adLanguage;
}

void QAdService::setAdLanguage(const QString& arg)
{
    if (m_adLanguage != arg) {
        m_adLanguage = arg;
        emit adLanguageChanged(arg);
    }
}

void QAdService::fetchAd()
{
    if (m_reply) {
        m_reply->abort();
        delete m_reply;
        m_reply = NULL;
    }
    if (m_platform == NULL) {
        setStatus(Error);
    } else {
        fetchAdFromUrl(m_platform->prepareUrlForRequest(*this), m_platform->preparePostDataForRequest(*this));
    }
}

void QAdService::networkReplyFinished()
{
    int m_redirectCount = 0; //TODO: make an attribute for it
    if (m_reply) {
        m_redirectCount++;
        if (m_redirectCount < QADMOB_MAXIMUM_REDIRECT_RECURSION) {
            QVariant redirect = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if (redirect.isValid()) {
                QUrl url = m_reply->url().resolved(redirect.toUrl());
                m_reply->deleteLater();
                m_reply = 0;
                fetchAdFromUrl(url, QByteArray());
                return;
            }
        }
        m_redirectCount = 0;

        if (m_reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = m_reply->readAll();
            m_reply->deleteLater();
            m_reply = 0;
            bool ok = handleResponseData(responseData);
            if (ok) {
                setStatus(Ready);
            } else {
                setStatus(Error);
            }
        } else {
            m_reply->deleteLater();
            m_reply = 0;
            setStatus(Error);
        }
    }
}

bool QAdService::handleResponseData( const QByteArray& aResponseData )
{
    QVariant parsedResult = parseJsonResponseData(aResponseData);
    if (!parsedResult.isValid() || parsedResult.isNull()) {
        return false;
    }
    if (m_platform == NULL)
        return false;
    QAd *newAd = m_platform->createAdFromResponse(parsedResult);
    if (newAd == NULL) {
        return false;
    } else {
        newAd->setParent(this);
        setAd(newAd);
        return true;
    }
}

QVariant QAdService::parseJsonResponseData( const QByteArray& aResponseData )
{
#ifdef QADMOB_QT4
    QJson::Parser parser;
    bool ok;

    QVariant result = parser.parse(aResponseData, &ok);

    if (!ok)
    {
        return QVariant();
    }
#else
    QJsonParseError error;
    QJsonDocument parser =  QJsonDocument::fromJson(aResponseData, &error);

    if (error.error != QJsonParseError::NoError)
    {
        return QVariant();
    }
    return parser.toVariant();
#endif
}

void QAdService::setStatus(QAdService::Status arg)
{
    if (m_status != arg) {
        m_status = arg;
        emit statusChanged(arg);
    }
}

void QAdService::setAd(QAd *arg)
{
    if (m_ad != arg) {
        delete m_ad;
        m_ad = arg;
        emit adChanged(arg);
    }
}

QAd *QAdService::ad()
{
    return m_ad;
}

QAdServicePlatform *QAdService::platform() const
{
    return m_platform;
}

void QAdService::setPlatform(QAdServicePlatform *arg)
{
    if (m_platform != arg) {
        m_platform = arg;
        emit platformChanged(arg);
    }
}

void QAdService::fetchAdFromUrl(const QUrl &url, const QByteArray &data)
{
    if (m_reply) {
        m_reply->abort();
        delete m_reply;
        m_reply = NULL;
    }
    if (url.isValid() == false) {
        setStatus(Error);
        return;
    }
    setStatus(Loading);
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("User-Agent", kUserAgent.toUtf8());

    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);

    QNetworkAccessManager *nam = qmlEngine(this)->networkAccessManager();

    if (data.length()) {
        m_reply = nam->post(request, data);
    } else {
        m_reply = nam->get(request);
    }

    QObject::connect(m_reply, SIGNAL(finished()),  this, SLOT(networkReplyFinished()));
}
