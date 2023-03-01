#include "scanscreen.h"
#include "ui_scanscreen.h"

ScanScreen::ScanScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanScreen)
{
    ui->setupUi(this);
}

ScanScreen::~ScanScreen()
{
    delete ui;
}
