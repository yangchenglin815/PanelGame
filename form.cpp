#include "form.h"
#include "ui_form.h"
#include <QGraphicsScene>
#include "view.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::gray);
    scene->setSceneRect(0, 0, 400, 300);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->setScene(scene);

    View *view = new View;
    view->setRect(20, 20, 60, 60);
    scene->addItem(view);

    View *view1 = new View;
    view1->setRect(100, 100, 60, 60);
    scene->addItem(view1);

}

Form::~Form()
{
    delete ui;
}
