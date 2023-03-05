#include "scanscreen.h"
#include "ui_scanscreen.h"

ScanScreen::ScanScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanScreen)
{
    ui->setupUi(this);
    ui->calendarList->hide();
    QPixmap pic("/home/pi/Downloads/SmallLogoBW.png");
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
}

void ScanScreen::on_backButton_clicked()
{
    emit backToStandBy();
}

void ScanScreen::on_pushButton_clicked()
{
    emit getCalendar();
}
