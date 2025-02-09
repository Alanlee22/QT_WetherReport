#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QPoint mOffset;
    void mouseMoveEvent(QMouseEvent *event) override;

public slots:
    void readHttpReply(QNetworkReply *reply);
private:
    Ui::Widget *ui;
    void mousePressEvent(QMouseEvent *event);
    QMenu *menuQuit;
    QNetworkReply *reply;
    void parseWeatherJsonData(QByteArray rawData);
};
#endif // WIDGET_H
