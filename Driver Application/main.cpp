/*VehicleSecure Driver Application main file*/

#include <QApplication>
#include "MainMenu.h"


int main(int argc, char *argv[]) {
   
    QApplication app(argc, argv); //create a QT Application
    
    //loads the QT form
    MainMenu form;
    form.show();
   

    return app.exec();
}
