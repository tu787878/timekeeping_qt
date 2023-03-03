#include "scanscreen.h"
#include "ui_scanscreen.h"

ScanScreen::ScanScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanScreen)
{
    ui->setupUi(this);
    ui->calendarList->hide();
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

void ScanScreen::on_backButton_clicked()
{
    emit backToStandBy();
}

void ScanScreen::on_pushButton_clicked()
{
    emit getCalendar();
}
