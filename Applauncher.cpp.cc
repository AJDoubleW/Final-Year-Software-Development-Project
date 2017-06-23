/*VehicleSecure Application Launcher*/

#include "Applauncher.h"
#include <cstdlib> //allows the std::system command
#include <unistd.h> //alows chdir

using namespace std;

Applauncher::Applauncher() {
    widget.setupUi(this); //sets up the applauncher
    
}

Applauncher::~Applauncher() {
}

void Applauncher::on_driLaunch_clicked() {
    this->close();
    chdir("../../../../vs_dri_app/dist/Debug/GNU-Linux"); //opens the driver launch app
    system("./vs_dri_app");
}

void Applauncher::on_insLaunch_clicked() {
    this->close();
    chdir("../../../../vs_ins_app/dist/Debug/GNU-Linux"); //opens the insurance launcher app
    system("./vs_ins_app");
}

void Applauncher::on_adminLaunch_clicked() {
    this->close();
    chdir("../../../../vs_admin_app/dist/Debug/GNU-Linux"); //opens the admin launcher app
    system("./vs_admin_app");
}