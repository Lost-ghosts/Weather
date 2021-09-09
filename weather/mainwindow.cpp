#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口属性为没有边框
    setWindowFlag(Qt::FramelessWindowHint);
    //设置窗口固定大小
    setFixedSize(width(),height());
    //右键退出
    exitMenu = new QMenu(this);
    exitAct = new QAction;
    exitAct->setText(u8"退出");
    exitAct->setIcon(QIcon(":/weatherIco/close.ico"));
    exitMenu->addAction(exitAct);
    connect(exitAct,SIGNAL(triggered(bool)),this,SLOT(slot_exitApp()));
    //ui初始化
    forecast_week_list << ui->week0Lb << ui->week1Lb << ui->week2Lb << ui->week3Lb << ui->week4Lb << ui->week5Lb;
    forecast_date_list << ui->date0Lb << ui->date1Lb << ui->date2Lb << ui->date3Lb << ui->date4Lb << ui->date5Lb;
    forecast_aqi_list << ui->quality0Lb << ui->quality1Lb << ui->quality2Lb << ui->quality3Lb << ui->quality4Lb << ui->quality5Lb;
    forecast_type_list << ui->type0Lb << ui->type1Lb << ui->type2Lb << ui->type3Lb << ui->type4Lb << ui->type5Lb;
    forecast_typeIco_list << ui->typeIco0Lb << ui->typeIco1Lb << ui->typeIco2Lb << ui->typeIco3Lb << ui->typeIco4Lb << ui->typeIco5Lb;
    forecast_high_list << ui->high0Lb << ui->high1Lb << ui->high2Lb << ui->high3Lb << ui->high4Lb << ui->high5Lb;
    forecast_low_list << ui->low0Lb << ui->low1Lb << ui->low2Lb << ui->low3Lb << ui->low4Lb << ui->low5Lb;
    //dateLb和WeekLb样式表设置
    for(int i = 0;i<6;i++)
    {
        forecast_date_list[i]->setStyleSheet("background-color:rgba(0,255,255,100);");
        forecast_week_list[i]->setStyleSheet("background-color:rgba(0,255,255,100);");
    }
    //设置搜索为圆角框
    ui->cityLineEdit->setStyleSheet("QLineEdit{border: 1px solid gray; border-radius: 4px; background:argb(47, 47, 47, 130); color:rgb(255, 255, 255);} QLineEdit:hover{border-color:rgb(101, 255, 106); }");

    //网络请求的初始化
    // 请求天气API信息
        url = "http://t.weather.itboy.net/api/weather/city/";
        city = u8"上海";
        cityTmp = city;
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replayFinished(QNetworkReply*)));
        getWeatherInfo(manager);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos()-mPos);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mPos = event->globalPos()-this->pos();
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    exitMenu->exec(QCursor::pos());
    event->accept();

}
//请求数据
void MainWindow::getWeatherInfo(QNetworkAccessManager *manager)
{
    QString citycode = tool[city];
        if(citycode=="000000000"){
            QMessageBox::warning(this, u8"错误", u8"天气：指定城市不存在！", QMessageBox::Ok);
            return;
        }
        QUrl jsonUrl(url + citycode);
        manager->get( QNetworkRequest(jsonUrl) );

}

void MainWindow::parseJson(QByteArray &bytes)
{
    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes,&err);    //检测json格式
    if(err.error != QJsonParseError::NoError)       //Json格式错误
    {
        return;
    }

    QJsonObject jsObj = jsonDoc.object();
    qDebug()<<jsObj;
    qDebug()<<"=========================\r\n";
    qDebug()<<jsObj.value("message").toString();
    QString message = jsObj.value("message").toString();
    if(message.contains("success") == false)
    {
        QMessageBox::information(this,tr("The information of Json_desc"),u8"天气：城市错误",QMessageBox::Ok);
        city = cityTmp;
        return;
    }
    today = jsObj;

    //解析data中的yesterday
    QJsonObject dataObj = jsObj.value("data").toObject();
    //昨天数据
    forecast[0] = dataObj.value("yesterday").toObject();
    //解析data中的forecast
    QJsonArray forecastArr = dataObj.value("forecast").toArray();
    int j = 0;      //预报数据从今天开始
    for(int i = 1; i < 6;i++)
    {
        forecast[i] = forecastArr.at(j).toObject();
        j++;
    }
    setLabelContent();
}

void MainWindow::setLabelContent()
{
    //更新UI数据
    // 今日数据
    ui->dateLb->setText(today.date);
    ui->temLb->setText(today.wendu);
    ui->cityLb->setText(today.city);
    ui->typeLb->setText(today.type);
    ui->noticeLb->setText(today.notice);
    ui->shiduLb->setText(today.shidu);
    ui->pm25Lb->setText(today.pm25);
    ui->fxLb->setText(today.fx);
    ui->flLb->setText(today.fl);
    ui->ganmaoBrowser->setText(today.ganmao);

    // 六天数据
        for (int i = 0; i < 6; i++)
        {
            forecast_week_list[i]->setText(forecast[i].week.right(3));
            forecast_date_list[i]->setText(forecast[i].date.left(3));
            forecast_type_list[i]->setText(forecast[i].type);
            forecast_high_list[i]->setText(forecast[i].high.split(" ").at(1));
            forecast_low_list[i]->setText(forecast[i].low.split(" ").at(1));
            forecast_typeIco_list[i]->setStyleSheet( tr("image: url(:/day/day/%1.png);").arg(forecast[i].type) );

            if (forecast[i].aqi.toInt() >= 0 && forecast[i].aqi.toInt() <= 50)
            {
                forecast_aqi_list[i]->setText(u8"优质");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(0, 255, 0);");
            }
            else if (forecast[i].aqi.toInt() > 50 && forecast[i].aqi.toInt() <= 100)
            {
                forecast_aqi_list[i]->setText(u8"良好");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 255, 0);");
            }
            else if (forecast[i].aqi.toInt() > 100 && forecast[i].aqi.toInt() <= 150)
            {
                forecast_aqi_list[i]->setText(u8"轻度污染");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 170, 0);");
            }
            else if (forecast[i].aqi.toInt() > 150 && forecast[i].aqi.toInt() <= 200)
            {
                forecast_aqi_list[i]->setText(u8"重度污染");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 0, 0);");
            }
            else
            {
                forecast_aqi_list[i]->setText(u8"严重污染");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(170, 0, 0);");
            }
        }
        //设置昨天今天的星期在一周中
        ui->week0Lb->setText( u8"昨天" );
        ui->week1Lb->setText( u8"今天" );
        //TODO:温度曲线的绘制和日出日落的绘制
}

void MainWindow::slot_exitApp()
{
    qApp->exit(0);
}

void MainWindow::replayFinished(QNetworkReply *reply)
{
    /* 获取响应的信息，状态码为200表示正常 */
        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if(reply->error() != QNetworkReply::NoError || status_code != 200)
        {
            QMessageBox::warning(this, u8"错误", u8"天气：请求数据错误，检查网络连接！", QMessageBox::Ok);
            return;
        }

        QByteArray bytes = reply->readAll();
       // qDebug()<<bytes;
        parseJson(bytes);

}

