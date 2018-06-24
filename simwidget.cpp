#include "simwidget.h"
#include "ui_simwidget.h"
#include <QTimer>
#include <QPalette>
#include <memory>
#include "game.h"

SimWidget::SimWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimWidget)
{
    ui->setupUi(this);
    timer = std::make_unique<QTimer>(this);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(turn()));
}

SimWidget::~SimWidget()
{
    delete ui;
}

bool SimWidget::isActive(){
    return timer->isActive();
}

void SimWidget::stop(){
    timer->stop();
}

void SimWidget::start(){
    timer->start(5);
}

void SimWidget::init(){
    game = std::make_unique<Game>(data);
    QPalette p(palette());
    QColor color(255,255,255,255);
    p.setColor(QPalette::Background, color);
    setPalette(p);
    setAutoFillBackground(true);
    this->show();
    this->update();
}

void SimWidget::turn(){

    game->move();
    this->update();
    if(game->turn==data.num_turns){
        Population win = winner(game->get_results());
        emit end_game(win);
    }
    emit next_turn(game->turn);
}


void SimWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    int max_y = data.field_size;

    for(auto && item: game->cells){

        auto& cell = item.second;
        QPainterPath path;
        int len = cell.size;
        int nx=cell.position.first, ny=cell.position.second;
        double x0=1.732*nx*len+(max_y-ny)*len*1.732/2, y0=1.5*ny*len;

        QVector<QPoint> points;
        points.push_back({static_cast<int>(x0),
                          static_cast<int>(y0)});
        points.push_back({static_cast<int>(x0+len*1.732/2),
                          static_cast<int>(y0+len/2)});
        points.push_back({static_cast<int>(x0+len*1.732/2),
                          static_cast<int>(y0+len*1.5)});
        points.push_back({static_cast<int>(x0),
                          static_cast<int>(y0+2*len)});
        points.push_back({static_cast<int>(x0-len*1.732/2),
                          static_cast<int>(y0+len*1.5)});
        points.push_back({static_cast<int>(x0-len*1.732/2),
                          static_cast<int>(y0+len/2)});
        QPolygon poly(points);
        path.addPolygon(poly);
        painter.fillPath(path, cell.getColor());
        painter.drawPolygon(poly);
    }
}

