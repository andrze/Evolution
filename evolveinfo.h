#ifndef EVOLVEINFO_H
#define EVOLVEINFO_H

#include <QMainWindow>

namespace Ui {
class EvolveInfo;
}

class EvolveInfo : public QMainWindow
{
    Q_OBJECT

public:
    explicit EvolveInfo(QWidget *parent = 0);
    ~EvolveInfo();

signals:
    void stop_evolving();

private:
    Ui::EvolveInfo *ui;

private slots:
    void update_status(int,int);
    void on_stopButton_clicked();
};

#endif // EVOLVEINFO_H
