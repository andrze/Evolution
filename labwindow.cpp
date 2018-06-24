#include "labwindow.h"
#include "ui_labwindow.h"
#include "game.h"
#include "evolveinfo.h"
#include <sstream>
#include <QFileDialog>

LabWindow::LabWindow(QWidget *parent, Population pop) :
    QMainWindow(parent),
    ui(new Ui::LabWindow)
{
    ui->setupUi(this);
    this->pop = pop;
    refresh();
    connect(this,SIGNAL(end_game(Population)),this,SLOT(next_evo_it(Population)));
    connect(this,SIGNAL(evo_completed(Population)),this,SLOT(evo_complete(Population)));
    timer = std::make_unique<QTimer>(this);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(turn()));
    help_window = std::make_unique<EvolveInfo>(this);
    connect(this,SIGNAL(mutation(int,int)),help_window.get(),SLOT(update_status(int,int)));
    connect(help_window.get(),SIGNAL(stop_evolving()),this,SLOT(abandon_evolving()));
}

LabWindow::~LabWindow()
{
    delete ui;
}

void LabWindow::on_mutateButton_clicked()
{
    double range = ui->rangeBox->value();
    bool absolute = ui->absoluteCheck->isChecked();
    pop.mutate(range, absolute);
    refresh();
}

void LabWindow::refresh(){
    std::ostringstream sstream("", std::ios_base::ate);
    sstream<<"Populacja: "<<pop;
    QString text = QString::fromStdString(sstream.str());
    ui->popLabel->setText(text);
    ui->nameEdit->setText(QString::fromStdString(pop.name));
    ui->RBox->setValue(pop.color.red());
    ui->GBox->setValue(pop.color.green());
    ui->BBox->setValue(pop.color.blue());
}

void LabWindow::on_saveButton_clicked(){
    std::string filename = QFileDialog::getSaveFileName(this, "Zapisz dane", "Populations/"+ui->nameEdit->text()+".dat", "*.dat").toStdString();
    std::ofstream file(filename);
    pop.name = ui->nameEdit->text().toStdString();
    pop.color = QColor(ui->RBox->value(),ui->GBox->value(),ui->BBox->value());
    file << pop;
    file.close();
    ui->infoLine->setText(QString::fromStdString("Zapisano dane do pliku '"+filename+"'."));
    refresh();
}

void LabWindow::on_evolveButton_clicked()
{
    stop_evolving=false;
    max_evo_it=ui->iterationBox->value();
    evo_iteration=0;
    emit mutation(0,max_evo_it);
    help_window->show();
    next_evo_it(pop);
}

void LabWindow::play(){
    old.index=1;
    mutant.index=2;
    std::vector<Population> pops;
    pops.push_back(old);
    pops.push_back(mutant);
    Data data(pops);
    data.field_size = 6;
    game = Game(data);
    timer->start(5);
}

void LabWindow::next_evo_it(Population win){
    emit mutation(evo_iteration,max_evo_it);
    if(evo_iteration==ui->iterationBox->value()){
        emit evo_completed(win);
        return;
    }
    evo_iteration++;
    old = win;
    win.mutate(ui->rangeBox->value(),ui->absoluteCheck->isChecked());
    mutant = win;
    play();
}

void LabWindow::abandon_evolving(){
    help_window->hide();
    timer->stop();
}

void LabWindow::evo_complete(Population win){
    pop=win;
    help_window->hide();
    refresh();
}

void LabWindow::turn(){
    game.move();
    if(game.turn==75){
        timer->stop();
        Population win = winner(game.get_results());
        emit end_game(win);
    }
}

void LabWindow::on_pushButton_clicked()
{
    close();
}
