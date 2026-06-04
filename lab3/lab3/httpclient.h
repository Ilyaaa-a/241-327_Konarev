#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include "camera.h"

// Синглтон + адаптер C++ -> HTTP/REST для /api/cameras
//
// Паттерн "Singleton": единственный экземпляр на всё приложение,
//   доступ через HttpClient::instance().
//
// Паттерн "Adapter": методы C++ (requestList/Get/Create/Update/Delete)
//   преобразуются в HTTP-запросы (GET/POST/PUT/DELETE) к серверу из ЛР1/2.
//
// Асинхронность: методы только отправляют запрос и возвращают управление.
//   Когда придёт ответ - сработает один из сигналов.

class HttpClient : public QObject
{
    Q_OBJECT

public:
    static HttpClient &instance();

    // 5 методов REST (адаптер)
    void requestList(); // GET    /api/cameras
    void requestGet(int id);  // GET    /api/cameras/{id}
    void requestCreate(const Camera &camera); // POST   /api/cameras
    void requestUpdate(int id, const Camera &camera);  // PUT    /api/cameras/{id}
    void requestDelete(int id); // DELETE /api/cameras/{id}

signals:
    // Пришёл список камер (после requestList)
    void listReceived(const QList<Camera> &cameras, const QString &rawJson);

    // Пришёл один объект (после requestGet / requestCreate / requestUpdate)
    void cameraReceived(const Camera &camera, const QString &rawJson);

    // Операция без тела ответа (после requestDelete) или общий статус
    void operationFinished(const QString &message);

    // Любая ошибка сети/HTTP
    void errorOccurred(const QString &message);

private:
    explicit HttpClient(QObject *parent = nullptr);
    HttpClient(const HttpClient &) = delete;
    HttpClient &operator=(const HttpClient &) = delete;

    // тип последней операции
    enum class Op { List, Get, Create, Update, Delete };

    QNetworkRequest makeRequest(const QString &path) const;
    void handleReply(QNetworkReply *reply, Op op);

    QNetworkAccessManager m_manager;
    QString m_baseUrl = "https://localhost:443/api/cameras";
};

#endif // HTTPCLIENT_H
