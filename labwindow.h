#ifndef LABWINDOW_H
#define LABWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "population.h"
#include "evolveinfo.h"
#include "simwidget.h"

namespace Ui {
class LabWindow;
}

class LabWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LabWindow(QWidget *parent, Population pop);
    ~LabWindow();

    Population pop;
    bool stop_evolving=false;
    std::unique_ptr<QTimer> timer;
    Population old, mutant;
    Game game;
    int evo_iteration, max_evo_it=50;
    std::unique_ptr<EvolveInfo> help_window;

signals:
    void mutation(int,int);
    void end_evolving();
    void evo_completed(Population);
    void end_game(Population);

private slots:
    void on_mutateButton_clicked();

    void on_saveButton_clicked();

    void on_evolveButton_clicked();
    void abandon_evolving();
    void next_evo_it(Population);
    void evo_complete(Population);
    void turn();

    void on_pushButton_clicked();

private:
    Ui::LabWindow *ui;

    void play();
    void refresh();
};

#endif // LABWINDOW_H
