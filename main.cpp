#include "Panel.h"
#include "form.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Panel w;
    Form w;
    w.show();

    return a.exec();
}
