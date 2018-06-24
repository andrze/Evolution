#include <memory>
#include <string>
#include <sstream>
#include "simwindow.h"
#include "ui_simwindow.h"
#include "simwidget.h"

SimWindow::SimWindow(QWidget *parent, Data data) :
    QMainWindow(parent),
    ui(new Ui::SimWindow)
{
    ui->setupUi(this);
    this->data = data;
    ui->simwidget->data=data;
    int wid_h = 30*(3*data.field_size-1),
        wid_w = 30*1.732*(2*data.field_size-1);
    ui->simwidget->setGeometry(10,80,1+wid_w,1+wid_h);
    this->window()->setGeometry(0,0,std::max(21+wid_w,400), 111+wid_w);
    ui->simwidget->init();
    connect(ui->simwidget,SIGNAL(next_turn(int)),this,SLOT(next_turn(int)));
    connect(ui->simwidget,SIGNAL(end_game(Population)),this,SLOT(end(Population)));
}

SimWindow::~SimWindow()
{
    delete ui;
}

void SimWindow::next_turn(int turn){
    ui->turnLabel->setText(QString::number(turn));
}

void SimWindow::on_endButton_clicked(){
    ui->simwidget->stop();
    this->close();
}

void SimWindow::on_pauseButton_clicked(){
    if(ui->simwidget->isActive()){
        ui->pauseButton->setText(tr("Wznów"));
        ui->simwidget->stop();
    }
    else{
        ui->pauseButton->setText(tr("Pauza"));
        ui->simwidget->start();
    }
}

void SimWindow::closeEvent(QCloseEvent *){
    ui->simwidget->stop();
    this->close();
}

void SimWindow::end(Population winner){
    ui->simwidget->stop();
    ui->simwidget->hide();
    std::ostringstream stream("", std::ios_base::ate);
    stream<<"Populacja: "<<winner;
    ui->winnerLabel->setText(QString::fromStdString(stream.str()));
    ui->pauseButton->setText("Start");
    ui->pauseButton->setDisabled(true);
}


void SimWindow::on_restartButton_clicked(){
    ui->simwidget->stop();
    ui->simwidget->data = data;
    ui->simwidget->init();
    ui->pauseButton->setText(tr("Start"));
    ui->turnLabel->setText("0");
    ui->winnerLabel->setText("");
    ui->pauseButton->setEnabled(true);
}
