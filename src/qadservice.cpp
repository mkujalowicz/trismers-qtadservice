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
#include <QDomDocument>

#include "qadservice.h"
#ifdef QADSERVICE_QT4
#include "parser.h"
#else
#include <QJsonDocument>
#endif

#define QADSERVICE_MAXIMUM_REDIRECT_RECURSION 4

// CONSTANTS
static const QString kDefaultUserAgentFormat ="Mozilla/5.0 (X11; Linux x86_64; en-US; rv:2.0.1) Gecko/20100101 Firefox/4.0.1";

QAdService::QAdService(QObject *parent) :
    QObject(parent), m_status(Null), m_reply(NULL), m_platform(NULL), m_adTypeHint(AdTypeHintBanner),
    m_testMode ( false ), m_ad(new QAd(this)), m_redirectCount(0), m_visitorGender(GenderUnknown), m_visitorAge(-1),
    m_userAgent(kDefaultUserAgentFormat)
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
            QVariant mimeType = m_reply->header(QNetworkRequest::ContentTypeHeader);
            m_reply->deleteLater();
            m_reply = 0;
            bool ok = handleResponseData(responseData, mimeType.toByteArray());
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

bool QAdService::handleResponseData(const QByteArray &responseData, const QByteArray &mimeType)
{
    QVariant parsedResult = QVariant::Invalid;

    QByteArray data = m_platform->processResponseData(responseData);

    if (mimeType.contains("json")) {
        parsedResult = parseJsonResponseData(data);
    } else {
        if (mimeType.contains("xml")) {
            parsedResult = parseXMLResponseData(data);
        }
    }
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

QVariant QAdService::parseJsonResponseData(const QByteArray& aResponseData)
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

static QVariant domElementToVariant(const QDomElement &element)
{
    QVariantMap variantMap;

    QDomNamedNodeMap attributesMap = element.attributes();

    for (qint32 index = 0; index < attributesMap.count(); index++) {
        QDomAttr attribute = attributesMap.item(index).toAttr();
        QString key = attribute.nodeName();
        QVariant value = attribute.value();

        if (key.length())
            variantMap[key] = value;
    }
    for (QDomElement child = element.firstChildElement(); !child.isNull(); child = child.nextSiblingElement()) {
        QString key = child.nodeName();
        QVariant value;

        if (!child.firstChildElement().isNull() || child.attributes().count()) {
            value = domElementToVariant(child);
        } else {
            value = child.text();
        }
        if (key.length())
            variantMap[key] = value;
    }
    if (variantMap.keys().count()) {
        return variantMap;
    } else {
        return QVariant::Invalid;
    }
}

QVariant QAdService::parseXMLResponseData(const QByteArray& aResponseData)
{
    QDomDocument doc;
    if (!doc.setContent(aResponseData, true))
        return QVariant();

    QDomElement root = doc.documentElement();

    return domElementToVariant(root);
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

void QAdService::setTrackingId(const QString &arg)
{
    if (m_trackingId != arg) {
        m_trackingId = arg;
        emit trackingIdChanged(arg);
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

QString QAdService::uniqueId() const
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
        }
    }
    return m_uniqueId;
}

int QAdService::visitorAge() const
{
    return m_visitorAge;
}

void QAdService::setVisitorAge(int arg)
{
    if (m_visitorAge != arg) {
        m_visitorAge = arg;
        emit visitorAgeChanged(arg);
    }
}

QAdService::Gender QAdService::visitorGender() const
{
    return m_visitorGender;
}

void QAdService::setVisitorGender(QAdService::Gender arg)
{
    if (m_visitorGender != arg) {
        m_visitorGender = arg;
        emit visitorGenderChanged(arg);
    }
}

QString QAdService::userAgent() const
{
    return m_userAgent;
}

void QAdService::setUserAgent(QString arg)
{
    if (m_userAgent != arg) {
        m_userAgent = arg;
        emit userAgentChanged(arg);
    }
}

QString QAdService::trackingId() const
{
    return m_trackingId;
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
    request.setRawHeader("User-Agent", userAgent().toUtf8());
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
