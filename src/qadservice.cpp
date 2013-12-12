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
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QDebug>
#include <QSize>
#include <QQmlEngine>

#include "qadservice.h"
#ifdef QADSERVICE_QT4
#include "parser.h"
#else
#include <QJsonDocument>
#endif

#define QADSERVICE_MAXIMUM_REDIRECT_RECURSION 4

// CONSTANTS
static const QString kUserAgent ="QtAdService/1.0 (Qt/5.2; http://qt-project.org)";

QAdService::QAdService(QObject *parent) :
    QObject(parent), m_status(Null), m_reply(NULL), m_platform(NULL), m_adTypeHint(AdTypeHintText),
    m_testMode ( false ), m_ad(new QAd(this)), m_redirectCount(0)
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


void QAdService::setSlotId(const QString& arg)
{
    if (m_slotId != arg) {
        m_slotId = arg;
        emit slotIdChanged(arg);
    }
}

QString QAdService::slotId() const
{
    return m_slotId;
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
        m_redirectCount = 0;
        m_bodyData.clear();
        QUrl url;
        if (!m_platform->prepareRequest(*this, url, m_bodyData)) {
            setStatus(Error);
            return;
        }
        fetchAdFromUrl(url, m_bodyData);
    }
}

void QAdService::networkReplyFinished()
{
    if (m_reply) {
        m_redirectCount++;
        if (m_redirectCount <= QADSERVICE_MAXIMUM_REDIRECT_RECURSION) {
            QVariant redirect = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if (redirect.isValid()) {
                QUrl url = m_reply->url().resolved(redirect.toUrl());
                m_reply->deleteLater();
                m_reply = 0;
                fetchAdFromUrl(url, m_bodyData);
                return;
            }
        }
        m_redirectCount = 0;
        m_bodyData.clear();

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
#ifdef QADSERVICE_QT4
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

void QAdService::setUniqueId(const QString &arg)
{
    if (m_uniqueId != arg) {
        m_uniqueId = arg;
        emit uniqueIdChanged(arg);
    }
}

QAd *QAdService::ad()
{
    return m_ad;
}

QAdPlatform *QAdService::platform() const
{
    return m_platform;
}

void QAdService::setPlatform(QAdPlatform *arg)
{
    if (m_platform != arg) {
        m_platform = arg;
        emit platformChanged(arg);
    }
}

QString QAdService::uniqueIdForRequest() const
{
    if (m_uniqueId.length() == 0) {
        QCryptographicHash hash(QCryptographicHash::Sha1);

        foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces()) {
            // Combine all non-loopback MAC Addresses
            if (!(netInterface.flags() & QNetworkInterface::IsLoopBack)
                    && netInterface.hardwareAddress().length()) {
                hash.addData(netInterface.hardwareAddress().toLatin1());
            }
        }
        if (m_uniqueId != hash.result().toHex()) {
            m_uniqueId = hash.result().toHex();
            emit uniqueIdChanged(m_uniqueId);
        }
    }
    return m_uniqueId;
}

QString QAdService::uniqueId() const
{
    return m_uniqueId;
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
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

    QNetworkAccessManager *nam = qmlEngine(this)->networkAccessManager();

    if (data.length()) {
        m_reply = nam->post(request, data);
    } else {
        m_reply = nam->get(request);
    }

    QObject::connect(m_reply, SIGNAL(finished()),  this, SLOT(networkReplyFinished()));
}
