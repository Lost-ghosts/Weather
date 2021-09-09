#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //���ô�������Ϊû�б߿�
    setWindowFlag(Qt::FramelessWindowHint);
    //���ô��ڹ̶���С
    setFixedSize(width(),height());
    //�Ҽ��˳�
    exitMenu = new QMenu(this);
    exitAct = new QAction;
    exitAct->setText(u8"�˳�");
    exitAct->setIcon(QIcon(":/weatherIco/close.ico"));
    exitMenu->addAction(exitAct);
    connect(exitAct,SIGNAL(triggered(bool)),this,SLOT(slot_exitApp()));
    //ui��ʼ��
    forecast_week_list << ui->week0Lb << ui->week1Lb << ui->week2Lb << ui->week3Lb << ui->week4Lb << ui->week5Lb;
    forecast_date_list << ui->date0Lb << ui->date1Lb << ui->date2Lb << ui->date3Lb << ui->date4Lb << ui->date5Lb;
    forecast_aqi_list << ui->quality0Lb << ui->quality1Lb << ui->quality2Lb << ui->quality3Lb << ui->quality4Lb << ui->quality5Lb;
    forecast_type_list << ui->type0Lb << ui->type1Lb << ui->type2Lb << ui->type3Lb << ui->type4Lb << ui->type5Lb;
    forecast_typeIco_list << ui->typeIco0Lb << ui->typeIco1Lb << ui->typeIco2Lb << ui->typeIco3Lb << ui->typeIco4Lb << ui->typeIco5Lb;
    forecast_high_list << ui->high0Lb << ui->high1Lb << ui->high2Lb << ui->high3Lb << ui->high4Lb << ui->high5Lb;
    forecast_low_list << ui->low0Lb << ui->low1Lb << ui->low2Lb << ui->low3Lb << ui->low4Lb << ui->low5Lb;
    //dateLb��WeekLb��ʽ������
    for(int i = 0;i<6;i++)
    {
        forecast_date_list[i]->setStyleSheet("background-color:rgba(0,255,255,100);");
        forecast_week_list[i]->setStyleSheet("background-color:rgba(0,255,255,100);");
    }
    //��������ΪԲ�ǿ�
    ui->cityLineEdit->setStyleSheet("QLineEdit{border: 1px solid gray; border-radius: 4px; background:argb(47, 47, 47, 130); color:rgb(255, 255, 255);} QLineEdit:hover{border-color:rgb(101, 255, 106); }");

    //��������ĳ�ʼ��
    // ��������API��Ϣ
        url = "http://t.weather.itboy.net/api/weather/city/";
        city = u8"�Ϻ�";
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
//��������
void MainWindow::getWeatherInfo(QNetworkAccessManager *manager)
{
    QString citycode = tool[city];
        if(citycode=="000000000"){
            QMessageBox::warning(this, u8"����", u8"������ָ�����в����ڣ�", QMessageBox::Ok);
            return;
        }
        QUrl jsonUrl(url + citycode);
        manager->get( QNetworkRequest(jsonUrl) );

}

void MainWindow::parseJson(QByteArray &bytes)
{
    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes,&err);    //���json��ʽ
    if(err.error != QJsonParseError::NoError)       //Json��ʽ����
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
        QMessageBox::information(this,tr("The information of Json_desc"),u8"���������д���",QMessageBox::Ok);
        city = cityTmp;
        return;
    }
    today = jsObj;

    //����data�е�yesterday
    QJsonObject dataObj = jsObj.value("data").toObject();
    //��������
    forecast[0] = dataObj.value("yesterday").toObject();
    //����data�е�forecast
    QJsonArray forecastArr = dataObj.value("forecast").toArray();
    int j = 0;      //Ԥ�����ݴӽ��쿪ʼ
    for(int i = 1; i < 6;i++)
    {
        forecast[i] = forecastArr.at(j).toObject();
        j++;
    }
    setLabelContent();
}

void MainWindow::setLabelContent()
{
    //����UI����
    // ��������
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

    // ��������
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
                forecast_aqi_list[i]->setText(u8"����");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(0, 255, 0);");
            }
            else if (forecast[i].aqi.toInt() > 50 && forecast[i].aqi.toInt() <= 100)
            {
                forecast_aqi_list[i]->setText(u8"����");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 255, 0);");
            }
            else if (forecast[i].aqi.toInt() > 100 && forecast[i].aqi.toInt() <= 150)
            {
                forecast_aqi_list[i]->setText(u8"�����Ⱦ");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 170, 0);");
            }
            else if (forecast[i].aqi.toInt() > 150 && forecast[i].aqi.toInt() <= 200)
            {
                forecast_aqi_list[i]->setText(u8"�ض���Ⱦ");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(255, 0, 0);");
            }
            else
            {
                forecast_aqi_list[i]->setText(u8"������Ⱦ");
                forecast_aqi_list[i]->setStyleSheet("color: rgb(170, 0, 0);");
            }
        }
        //������������������һ����
        ui->week0Lb->setText( u8"����" );
        ui->week1Lb->setText( u8"����" );
        //TODO:�¶����ߵĻ��ƺ��ճ�����Ļ���
}

void MainWindow::slot_exitApp()
{
    qApp->exit(0);
}

void MainWindow::replayFinished(QNetworkReply *reply)
{
    /* ��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ���� */
        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if(reply->error() != QNetworkReply::NoError || status_code != 200)
        {
            QMessageBox::warning(this, u8"����", u8"�������������ݴ��󣬼���������ӣ�", QMessageBox::Ok);
            return;
        }

        QByteArray bytes = reply->readAll();
       // qDebug()<<bytes;
        parseJson(bytes);

}

