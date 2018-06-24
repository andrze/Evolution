#ifndef SIMWINDOW_H
#define SIMWINDOW_H

#include <QMainWindow>
#include "population.h"
#include "data.h"

namespace Ui {
class SimWindow;
}

class SimWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimWindow(QWidget *parent, Data data);
    ~SimWindow();

    Data data;

private slots:
    void on_endButton_clicked();

    void on_pauseButton_clicked();
    void on_restartButton_clicked();

    void next_turn(int turn);
    void end(Population winner);

private:
    Ui::SimWindow *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // SIMWINDOW_H
