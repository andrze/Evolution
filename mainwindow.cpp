#include <stdexcept>
#include <fstream>
#include <QObject>
#include <QFileDialog>
#include <QCloseEvent>
#include <memory>
#include <QDirIterator>
#include <QCheckBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "population.h"
#include "simwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    get_files(QDir::currentPath()+"/Populations");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get_files(QString directory){
    pop_boxes.clear();
    ui->filenameEdit->setText(directory);
    ui->errorLabel->clear();
    data = Data();
    int i=0;
    for(QDirIterator it(directory); it.hasNext(); it.next()){
        std::string path = it.filePath().toStdString();
        if(path.size()>4 && path.substr(path.size()-4,4) == ".dat"){
            std::ifstream stream;
            stream.open(path);
            if(stream.fail()){
                continue;
            }
            Population pop(stream);
            pop.index = i;
            i++;
            data.populations.push_back(pop);
            pop_boxes.push_back(std::make_unique<QCheckBox>(QString::fromStdString(pop.name)));
            ui->popsLayout->addWidget(pop_boxes.back().get());
            connect(pop_boxes.back().get(), SIGNAL(toggled(bool)),this, SLOT(checkbox_changed()));
        }
    }
    ui->reloadButton->setDisabled(false);
    ui->labButton->setDisabled(true);
    ui->playButton->setDisabled(true);
}

void MainWindow::on_filenameButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory();
    if( !directory.isEmpty() ){
        get_files(directory);
    }
}

void MainWindow::on_reloadButton_clicked()
{
    QString directory = ui->filenameEdit->text();
    get_files(directory);
}

void MainWindow::on_playButton_clicked(){
    Data data = this->data;
    data.field_size = ui->sizeBox->value();
    data.num_turns = ui->turnBox->value();
    for(auto&& box: pop_boxes){
        if(!box->isChecked()){
            for(size_t i=0; i<data.populations.size(); i++){
                if(data.populations[i].name == box->text().toStdString()){
                    data.populations.erase(data.populations.begin()+i);
                }
            }
        }
    }
    simwindow = std::make_unique<SimWindow>(this, data);
    simwindow->show();
}

void MainWindow::closeEvent (QCloseEvent *event){
    simwindow = nullptr;
    event->accept();
}

void MainWindow::on_labButton_clicked(){
    Population pop;
    for(auto&& box: pop_boxes){
        if(box->isChecked()){
            for(auto&& p: data.populations){
                if(box->text().toStdString() == p.name){
                    pop = p;
                    break;
                }
            }
        }
    }
    labwindow = std::make_unique<LabWindow>(this,pop);
    labwindow->show();
}

void MainWindow::checkbox_changed(){
    chosen_pops=0;
    for(size_t i=0;i<pop_boxes.size(); i++){
        if(pop_boxes[i]->isChecked()){
            chosen_pops++;
        }
    }
    ui->labButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    if(chosen_pops==1){
        ui->labButton->setEnabled(true);
    }
    if(chosen_pops==2 || chosen_pops==3){
        ui->playButton->setEnabled(true);
    }
}
