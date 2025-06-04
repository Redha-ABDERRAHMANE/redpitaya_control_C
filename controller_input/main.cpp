#include "view.h"
#include <QtWidgets/QApplication>
#include "Observable.hpp"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Observable<std::pair<const int&, bool>> observable;
    View window;


    
    observable.setObserver(window);


    window.show();
    return app.exec();
}
