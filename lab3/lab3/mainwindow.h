#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Слоты на нажатия кнопок (отправляют запросы через HttpClient)
    void onListClicked();
    void onGetClicked();
    void onCreateClicked();
    void onUpdateClicked();
    void onDeleteClicked();

    // Слоты на ответы от HttpClient (асинхронно пишут результат в textEdit)
    void onListReceived(const QList<Camera> &cameras, const QString &rawJson);
    void onCameraReceived(const Camera &camera, const QString &rawJson);
    void onOperationFinished(const QString &message);
    void onErrorOccurred(const QString &message);

private:
    // Парсит тело из textEdit как JSON-камеру, возвращает false при ошибке
    bool parseBodyFromTextEdit(Camera &outCamera);

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
