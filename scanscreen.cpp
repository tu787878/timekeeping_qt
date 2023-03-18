#include "scanscreen.h"
#include "ui_scanscreen.h"
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSslConfiguration>

ScanScreen::ScanScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanScreen)
{
    ui->setupUi(this);
    ui->calendarList->hide();
    QPixmap pic(m_logi_url);
    pic.scaled(12,22,Qt::KeepAspectRatio);
    ui->logo->setPixmap(pic);


}

ScanScreen::~ScanScreen()
{
    delete ui;
}

void ScanScreen::setNameLabel(QString name)
{
    ui->nameLabel->setText(name);
}

void ScanScreen::setTimeLabel(QString time)
{
    ui->timeLabel->setText(time);
}

void ScanScreen::showCalendar()
{
    ui->calendarList->show();

}

void ScanScreen::hideCalendar()
{
    ui->calendarList->hide();
}

void ScanScreen::setItems(QJsonArray jsonArray)
{
    ui->calendarList->clear();
    ui->calendarList->setColumnCount(jsonArray.size());
    ui->calendarList->setRowCount(2);

    int i=0;
    foreach (const QJsonValue & value, jsonArray)
    {
        QJsonObject jobj = value.toObject();
        qDebug() << jobj << "\n";
        QString displayText = jobj["date"].toString();
        QDate date = QDate::fromString(displayText,"dd/MM/yyyy");
        displayText = date.toString("dd/MM");
        QTableWidgetItem* header = new QTableWidgetItem(displayText);
        ui->calendarList->setHorizontalHeaderItem(i, header);
        QJsonArray timeLogs = jobj["timeLogs"].toArray();

        QString row = "";
        foreach (const QJsonValue & timeLog, timeLogs)
        {
            row = row + timeLog["timeFrom"].toString() + "-" + timeLog["timeTo"].toString()+ "\n";
        }
        QTableWidgetItem* log = new QTableWidgetItem(row);
        log->setTextAlignment(Qt::AlignTop);
        if(jobj["status"].toString() != "VALID")
        {
            log->setTextColor(QColor::fromRgb(255,0,0));
        }
        ui->calendarList->setItem(0, i, log);
        i++;
    }
    ui->calendarList->resizeRowToContents(0);
    ui->calendarList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->calendarList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->calendarList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);


    QString c;
    if(m_isCheckin){
        c = m_checkin_color;
    }else{
        c = m_checkout_color;
    }

    ui->calendarList->setStyleSheet("QScrollBar:vertical { width: 30px;  background-color:'white'}QScrollBar:horizontal { height: 30px;  background-color:'white'}QScrollBar::handle:horizonal{background-color:"+c+";color:'grey'}QScrollBar::handle:vertical{background-color:"+c+";color:'grey'}QScrollBar::sub-page:horizontal{background-color:"+c+";color:black}QScrollBar::add-page:horizontall {background-color:"+c+";color:black}QScrollBar::add-line:horizontal {background-color:"+c+";color:black}QScrollBar::sub-line:horizontal{background-color:"+c+";color:black}");

}

void ScanScreen::on_backButton_clicked()
{
    emit backToStandBy();
}

void ScanScreen::on_pushButton_clicked()
{
    emit getCalendar();
}

void ScanScreen::setSettings(QString logo, QString checkInColor, QString checkOutColor)
{
    m_logi_url = logo;
    m_checkin_color = checkInColor;
    m_checkout_color = checkOutColor;

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    const QUrl qurl(m_logi_url);
    QNetworkRequest request(qurl);
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    QNetworkReply *reply = mgr->get(request);


    QObject::connect(reply, &QNetworkReply::finished, [=](){
        if(reply->error() == QNetworkReply::NoError){
            QPixmap pic;
            pic.loadFromData(reply->readAll());
            pic.scaled(12,22,Qt::KeepAspectRatio);
            ui->logo->setPixmap(pic);
        }
        else{
            QString err = reply->errorString();
            qDebug() << err;
        }
        reply->deleteLater();
    });

}

void ScanScreen::setIsCheckin(bool isCheckIn)
{
    m_isCheckin = isCheckIn;
}
