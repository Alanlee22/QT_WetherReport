#include "widget.h"
#include "ui_widget.h"
#include <QMouseEvent>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFont(QFont("Arial",15));
    setFixedSize(389,674);
    setWindowFlag(Qt::FramelessWindowHint);

    menuQuit = new QMenu(this);

    QAction *openAct = new QAction(QIcon(":/res/close.png"), tr("退出"),this);
    menuQuit->addAction(openAct);
    connect(menuQuit,&QMenu::triggered,this,[=]{
        this->close();
    });
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl TianQi("http://gfeljm.tianqiapi.com/api?unescape=1&version=v61&appid=62734772&appsecret=4s6jM9ub");
    QUrl ItBoy("http://t.weather.itboy.net/api/weather/city/101010100");
    QNetworkRequest res(TianQi);
    reply = manager->get(res);
    connect(manager,&QNetworkAccessManager::finished,this,&Widget::readHttpReply);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        qDebug() << "right Mouse clicked!";
        menuQuit->exec(QCursor::pos());
    }

    if(event->button() == Qt::LeftButton){
        //qDebug() << event->globalPos();
        mOffset = event->globalPos()-this->pos();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-mOffset);
}

void Widget::parseWeatherJsonData(QByteArray rawData)
{   //日期星期设置
    QJsonDocument jsonObj = QJsonDocument::fromJson(rawData);
    if(!jsonObj.isNull() && jsonObj.isObject()){
        QJsonObject objRoot = jsonObj.object();
        QString date = objRoot["date"].toString();
        QString xingqi = objRoot["week"].toString();
        ui->label_riqi->setText(date + " " + xingqi);
        //空气情况
        QString airtype = objRoot["air_tips"].toString();
        ui->kongqi->setText(airtype);
        //风向
        QString wind = objRoot["win"].toString();
        ui->labelfengxiang_2->setText(wind);
        QString wind_speed = objRoot["win_speed"].toString();
        ui->label->setText(wind_speed);
        //pm2.5
        QJsonObject aqiObject = objRoot["aqi"].toObject();
        QString pm25 = aqiObject["pm25"].toString();
        ui->label_2->setText(pm25);
        QString shidu = objRoot["humidity"].toString();
        ui->label_3->setText(shidu);
        //city
        QString city = objRoot["city"].toString();
        ui->label_diqu->setText(city);
        //wea
        QString wea = objRoot["wea"].toString();
        ui->label_tianqi->setText(wea);
        QString iconPath; // 图标的路径
        if (wea == "多云") {
            iconPath = ":/res/duoyun.png"; // 假设你将图标存储在Qt资源中
        } else if (wea == "晴") {
            iconPath = "::/res/qingtian.png";
        } else if (wea == "雨") {
            iconPath = ":/res/xiayu.png";
        } else if (wea == "雾霾") {
            iconPath = ":/res/wumai.png";
        } else if (wea == "大雨" && "中雨") {
            iconPath = ":/res/dayu.png";
        }
        // 设置 QPixmap 到 QLabel
        if (!iconPath.isEmpty()) {
            QPixmap pixmap(iconPath);
            ui->label_5->setPixmap(pixmap.scaled(ui->label_5->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        //air_level
        QString air_level = objRoot["air_level"].toString();
        ui->label_4->setText(air_level);
        //temple
        QString temp = objRoot["tem"].toString();
        QFont font = ui->label_wendu->font();
        font.setPointSize(36); // 设置字体大小为36
        ui->label_wendu->setFont(font); // 将修改后的字体应用到label
        ui->label_wendu->setText(temp);
        //temple range
        QString temple_range1 = objRoot["tem1"].toString();
        QString temple_range2 = objRoot["tem2"].toString();
         ui->label_wendufanwei->setText(temple_range1 + "-" + temple_range2);
    }
    //
}

void Widget::readHttpReply(QNetworkReply *reply)
{
    int rescode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if( reply->error() == QNetworkReply::NoError && rescode == 200){
        QByteArray data = reply->readAll();
        parseWeatherJsonData(data);
    }else{
        QMessageBox mes;
        mes.setWindowTitle("错误");
        mes.setText("网络请求错误");
        mes.setStyleSheet("QPushButton {color:red}");
        mes.setStandardButtons(QMessageBox::Ok);
        mes.exec();
    }
}

void Widget::on_pushButton_clicked()
{
    QString cityNameFromUser = ui->lineEdit->text();
}

