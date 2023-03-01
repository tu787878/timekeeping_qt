#include "standbyscreen.h"
#include "ui_standbyscreen.h"

StandByScreen::StandByScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StandByScreen)
{
    ui->setupUi(this);
}

StandByScreen::~StandByScreen()
{
    delete ui;
}

void StandByScreen::on_closeWindow_clicked()
{
    emit exit();
}

void StandByScreen::on_pushButton_clicked()
{
    emit rescan();
}
