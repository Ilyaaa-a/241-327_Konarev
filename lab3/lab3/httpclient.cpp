#include "httpclient.h"

#include <QNetworkReply>
#include <QSslError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

namespace {
// для локального сервера с самоподписанным сертификатом - игнорируем ошибки SSL

void ignoreLocalSslErrors(QNetworkReply *reply)
{
    QObject::connect(reply, &QNetworkReply::sslErrors,
                     reply, [reply](const QList<QSslError> &) {
        reply->ignoreSslErrors();
    });
}
}

HttpClient &HttpClient::instance()
{
    // синглтон Майерса. Один экземпляр на всё приложение
    static HttpClient s_instance;
    return s_instance;
}

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
{
}

QNetworkRequest HttpClient::makeRequest(const QString &path) const
{
    QNetworkRequest req{QUrl(path)};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return req;
}

// 5 методов адаптера C++ -> HTTP

void HttpClient::requestList()
{
    QNetworkReply *reply = m_manager.get(makeRequest(m_baseUrl));
    ignoreLocalSslErrors(reply);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply, Op::List);
    });
}

void HttpClient::requestGet(int id)
{
    const QString url = m_baseUrl + "/" + QString::number(id);
    QNetworkReply *reply = m_manager.get(makeRequest(url));
    ignoreLocalSslErrors(reply);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply, Op::Get);
    });
}

void HttpClient::requestCreate(const Camera &camera)
{
    const QByteArray body = QJsonDocument(camera.toJson(false)).toJson();
    QNetworkReply *reply = m_manager.post(makeRequest(m_baseUrl), body);
    ignoreLocalSslErrors(reply);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply, Op::Create);
    });
}

void HttpClient::requestUpdate(int id, const Camera &camera)
{
    const QString url = m_baseUrl + "/" + QString::number(id);
    const QByteArray body = QJsonDocument(camera.toJson(false)).toJson();
    QNetworkReply *reply = m_manager.put(makeRequest(url), body);
    ignoreLocalSslErrors(reply);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply, Op::Update);
    });
}

void HttpClient::requestDelete(int id)
{
    const QString url = m_baseUrl + "/" + QString::number(id);
    QNetworkReply *reply = m_manager.deleteResource(makeRequest(url));
    ignoreLocalSslErrors(reply);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply, Op::Delete);
    });
}

// Обработка ответа

void HttpClient::handleReply(QNetworkReply *reply, Op op)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        const QString msg = QString("Ошибка сети: %1").arg(reply->errorString());
        qDebug() << msg;
        emit errorOccurred(msg);
        return;
    }

    const QByteArray data = reply->readAll();
    const QString rawJson = QString::fromUtf8(data);
    qDebug() << "Ответ сервера:" << rawJson;

    // Парсинг JSON через QJsonDocument/QJsonObject/QJsonArray/QJsonValue
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    switch (op) {
    case Op::List: {
        if (!doc.isArray()) {
            emit operationFinished(rawJson);
            return;
        }
        QList<Camera> cameras;
        const QJsonArray arr = doc.array();
        for (const QJsonValue &v : arr) {
            cameras.append(Camera::fromJson(v.toObject()));
        }
        emit listReceived(cameras, rawJson);
        break;
    }
    case Op::Get:
    case Op::Create:
    case Op::Update: {
        if (!doc.isObject()) {
            emit operationFinished(rawJson);
            return;
        }
        Camera c = Camera::fromJson(doc.object());
        emit cameraReceived(c, rawJson);
        break;
    }
    case Op::Delete: {
        // DELETE обычно возвращает 204 без тела
        emit operationFinished(QStringLiteral("Удалено").arg(rawJson));
        break;
    }
    }
}
