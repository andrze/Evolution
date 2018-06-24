#ifndef SIMWIDGET_H
#define SIMWIDGET_H

#include <QWidget>
#include <QPainter>
#include <memory>
#include "game.h"
#include "data.h"
#include "population.h"

namespace Ui {
class SimWidget;
}

class SimWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimWidget(QWidget *parent=0);
    explicit SimWidget(QWidget *parent, std::size_t size, Data data);
    ~SimWidget();

    std::unique_ptr<Game> game=nullptr;
    Data data;

    void init();
    void start();
    void stop();
    bool isActive();

signals:
    void end_game(Population winner);
    void next_turn(int turns);

private:
    Ui::SimWidget *ui;

    std::unique_ptr<QTimer> timer;

    void paintEvent(QPaintEvent *);

private slots:
    void turn();

};

#endif // SIMWIDGET_H
