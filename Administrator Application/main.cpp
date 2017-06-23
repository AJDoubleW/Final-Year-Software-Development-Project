/*Administrator application main file*/

#include <QApplication>
#include "RegLog.h"
#include "MainMenu.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    
    RegLog regl;
    regl.show();
    
    /*MainMenu mainm;
    mainm.show();*/

    // create and show your widgets here

    return app.exec();
}
