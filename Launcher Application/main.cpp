/*VehicleSecure Application Launcher main file*/

#include <QApplication>
#include "Applauncher.h"


int main(int argc, char *argv[]) {
   
    QApplication app(argc, argv); //create a QT Application
    
    //loads the QT form
    Applauncher form;
    form.show();
   

    return app.exec();
}
