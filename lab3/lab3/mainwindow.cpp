#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "httpclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Кнопки - слоты
    connect(ui->btnList,   &QPushButton::clicked, this, &MainWindow::onListClicked);
    connect(ui->btnGet,    &QPushButton::clicked, this, &MainWindow::onGetClicked);
    connect(ui->btnCreate, &QPushButton::clicked, this, &MainWindow::onCreateClicked);
    connect(ui->btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdateClicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);

    // сколько бы раз ни запросили instance()
    // адрес объекта один и тот же - клиент в приложении ровно один
    HttpClient *c1 = &HttpClient::instance();
    HttpClient *c2 = &HttpClient::instance();
    HttpClient *c3 = &HttpClient::instance();
    qDebug() << "client 1" << c1;
    qDebug() << "client 2" << c2;
    qDebug() << "client 3" << c3;

    // сигналы синглтон-клиента - слоты (асинхронный приём ответа)
    HttpClient &client = HttpClient::instance();
    connect(&client, &HttpClient::listReceived,      this, &MainWindow::onListReceived);
    connect(&client, &HttpClient::cameraReceived,    this, &MainWindow::onCameraReceived);
    connect(&client, &HttpClient::operationFinished, this, &MainWindow::onOperationFinished);
    connect(&client, &HttpClient::errorOccurred,     this, &MainWindow::onErrorOccurred);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Кнопки

void MainWindow::onListClicked()
{
    HttpClient::instance().requestList();
}

void MainWindow::onGetClicked()
{
    bool ok = false;
    const int id = ui->idEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите числовой ID");
        return;
    }
    HttpClient::instance().requestGet(id);
}

void MainWindow::onCreateClicked()
{
    Camera c;
    if (!parseBodyFromTextEdit(c)) return;
    HttpClient::instance().requestCreate(c);
}

void MainWindow::onUpdateClicked()
{
    bool ok = false;
    const int id = ui->idEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите числовой ID");
        return;
    }
    Camera c;
    if (!parseBodyFromTextEdit(c)) return;
    HttpClient::instance().requestUpdate(id, c);
}

void MainWindow::onDeleteClicked()
{
    bool ok = false;
    const int id = ui->idEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите числовой ID");
        return;
    }
    HttpClient::instance().requestDelete(id);
}

// --- ответы от клиента ---

void MainWindow::onListReceived(const QList<Camera> &cameras, const QString &rawJson)
{
    qDebug() << "Получено камер:" << cameras.size();
    for (const Camera &c : cameras) {
        qDebug() << "  id=" << c.id
                 << "brand=" << c.brand
                 << "year=" << c.release_year
                 << "mp=" << c.megapixels
                 << "sensor=" << c.sensor_type
                 << "lens=" << c.interchangeable_lens
                 << "price=" << c.price;
    }
    ui->textEdit->setPlainText(rawJson);
}

void MainWindow::onCameraReceived(const Camera &c, const QString &rawJson)
{
    qDebug() << "Камера: id=" << c.id
             << "brand=" << c.brand
             << "year=" << c.release_year
             << "mp=" << c.megapixels
             << "sensor=" << c.sensor_type
             << "lens=" << c.interchangeable_lens
             << "price=" << c.price;
    ui->textEdit->setPlainText(rawJson);
}

void MainWindow::onOperationFinished(const QString &message)
{
    qDebug() << message;
    ui->textEdit->setPlainText(message);
}

void MainWindow::onErrorOccurred(const QString &message)
{
    qDebug() << message;
    ui->textEdit->setPlainText(message);
}

// --- Утилита ---

bool MainWindow::parseBodyFromTextEdit(Camera &outCamera)
{
    const QByteArray data = ui->textEdit->toPlainText().toUtf8();
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        QMessageBox::warning(this, "Ошибка",
                             "В textEdit должен быть корректный JSON-объект:\n" + err.errorString());
        return false;
    }
    outCamera = Camera::fromJson(doc.object());
    return true;
}
