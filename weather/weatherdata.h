#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>
#include <QWidget>
#pragma execution_character_set("utf-8")
class Today//��������Ԥ��������
{
public:
    Today();
    Today& operator=(const QJsonObject& obj);
    QString date;//����
    QString wendu;//�¶�
    QString city;//����
    QString shidu;//ʪ��
    QString pm25;//pm2.5
    QString quality;//����
    QString ganmao;//��ðָ��
    QString fx;//����
    QString fl;//����
    QString type;//����
    QString sunrise;//�ճ�ʱ��
    QString sunset;//����ʱ��
    QString notice;//ע����Ϣ
};

class Forecast//Ԥ������
{
public:
    Forecast();
    //����=�Ų�����
    Forecast& operator=(const QJsonObject& obj);
    QString date;   //����
    QString week;   //����
    QString high;   //����
    QString low;    //����
    QString aqi;    //����
    QString type;   //����
};


#endif // WEATHERDATA_H
