#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <memory>
#include "simwindow.h"
#include "labwindow.h"
#include "data.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Data data;
    std::unique_ptr<SimWindow> simwindow;
    std::unique_ptr<LabWindow> labwindow;
    std::vector<std::unique_ptr<QCheckBox> > pop_boxes;
    int chosen_pops=0;

private slots:
    void on_filenameButton_clicked();
    void on_reloadButton_clicked();
    void on_playButton_clicked();
    void on_labButton_clicked();
    void checkbox_changed();

private:
    Ui::MainWindow *ui;
    void get_files(QString directory);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
