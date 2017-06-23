/*Insurer Application Main Qt file*/

#include <QApplication>
#include "RegLog.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    
    RegLog form;
    form.show();

    // create and show your widgets here

    return app.exec();
}
