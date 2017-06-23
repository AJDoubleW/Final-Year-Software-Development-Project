/*Add New Vehicle Form*/

/*Project file headers*/
#include "addVec.h"
#include "Login.h"
#include "MainMenu.h"

/*C standard headers*/
#include <stdio.h> //allows commands such as printf to be used
#include <iostream> //allows cin
#include <string> //allows strings
#include <ctype.h> //checks datatype of characters - whitelist
#include <cstring> //allows convert to cstring
#include <cstdlib> //allkinds - system/exit etc...
#include <sstream> //for creation of sql query string

/*MySQL headers*/
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "examples/examples.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

/*QT headers*/
#include <qt4/QtGui/qmessagebox.h>
#include <QInputDialog>

using namespace std;

string make;
string model;
int YearOfReg;
int numDoors;
int validMOT;
string owner;

addVec::addVec() {
    widget.setupUi(this);
    widget.yorCB->addItem("Year");
    for (int i = 2017; i > 1999; i--) {
        widget.yorCB->addItem(QString::number(i));
    }
}

addVec::~addVec() {
}

void addVec::on_addButton_clicked() {
    char *str_tbt;
    Login log;
    bool validVec = true;
    /*Setting values*/
    make = widget.makeEdit->text().toStdString();
    model = widget.modelEdit->text().toStdString();
    YearOfReg = widget.yorCB->currentText().toInt();
    numDoors = widget.numdSB->value();
    owner = widget.ownerEdit->text().toStdString();
    if ((widget.motyRB->isChecked()) || mot_not_needed()) {
        validMOT = 1;
    } else {
        validMOT = 0;
    }
    
    /*Testing entry fields for data*/
    if (make.length() == 0) {
        validVec = false;
        QMessageBox::warning(this, "No Make Entered", "You have not entered a make of vehicle. Please complete all fields.");
    }
    
    if (model.length() == 0) {
        validVec = false;
        QMessageBox::warning(this, "No Model Entered", "You have not entered a model of vehicle. Please complete all fields.");
    }
    
    if (YearOfReg == 0) {
        validVec = false;
        QMessageBox::warning(this, "No Year of Registration Entered", "You have not entered a year of registration for the vehicle. Please complete all fields.");
    }
    
    if (owner.length() == 0) {
        validVec = false;
        QMessageBox::warning(this, "No Owner Entered", "You have not entered an owner for the vehicle. Please complete all fields.");
    }
    
    
    /*Whitelisting strings*/
    
    str_tbt = new char[make.length() + 1];
    strcpy(str_tbt, make.c_str());
    
    if(!(log.whiteString(str_tbt))) {
        validVec = false;
        QMessageBox::warning(this, "Make Invalid", "That make of vehicle is invalid. Vehicle makes are alphanumeric only");
    }
    
    str_tbt = new char[model.length() + 1];
    strcpy(str_tbt, model.c_str());
    
    if(!(log.whiteString(str_tbt))) {
        validVec = false;
        QMessageBox::warning(this, "Model Invalid", "That model of vehicle is invalid. Vehicle models are alphanumeric only");
    }
    
    str_tbt = new char[owner.length() + 1];
    strcpy(str_tbt, owner.c_str());
    
    if(!(log.whiteString(str_tbt))) {
        validVec = false;
        QMessageBox::warning(this, "Owner Invalid", "That owner alias is invalid. Aliases are alphanumeric only");
    }
    
    if (validVec)
        chk_addvec();
}

void addVec::chk_addvec() {
    int driverid;
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the FinalYearProject database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        /*finding the driver with entered alias*/
        bool exDriver = false;
        res = stmt->executeQuery("SELECT DriverID, Alias FROM Driver");
        while (res->next()){
            if (res->getString("Alias") == owner){
                exDriver = true;
                driverid = res->getInt("DriverID");
            }
        }
        delete res;
        
        /*If there is no driver with that alias, display error*/
        if(!(exDriver)){
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "No such driver", "There is no driver with that alias.\nTry again?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
            if (cont == QMessageBox::No)
                exit(0);
            
            return;
        }
        
        ostringstream sqlstream;
        string sqlquery;
        sqlstream << "INSERT INTO Vehicle(Make,Model,YearOfReg,NumDoors,ValidMOT,Owner) VALUES ('" << make << "','" << model <<
                    "','" << YearOfReg << "','" << numDoors << "','" << validMOT << "','" << driverid << "')";
        sqlquery = sqlstream.str();
        stmt->execute(sqlquery);
        
        QMessageBox::StandardButton cont;
        cont = QMessageBox::information(this, "Vehicle Added", "Vehicle added to the system!\nWould you like to add another?",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        
        if (cont == QMessageBox::No) {
            MainMenu mainmenu;
            mainmenu.setModal(true);
            this->hide();
            mainmenu.exec();
        }
        
        /*If the user wishes to add another vehicle, clear all fields*/
        widget.makeEdit->clear();
        widget.modelEdit->clear();
        widget.yorCB->setCurrentIndex(0);
        widget.motnRB->isChecked();
        widget.numdSB->setValue(2);
        widget.ownerEdit->clear();
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

bool addVec::mot_not_needed() {
    //establishing the current date
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;
    
    if ((cYear - YearOfReg) < 3)
        return true;
    else
        return false;
}

void addVec::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}
