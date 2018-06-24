#include "evolveinfo.h"
#include "ui_evolveinfo.h"
#include <iostream>

EvolveInfo::EvolveInfo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EvolveInfo)
{
    ui->setupUi(this);
}

EvolveInfo::~EvolveInfo()
{
    delete ui;
}

void EvolveInfo::on_stopButton_clicked()
{
    emit stop_evolving();
}

void EvolveInfo::update_status(int i,int max){
    ui->statusLabel->setText(QString::number(i)+"/"+QString::number(max));
}
