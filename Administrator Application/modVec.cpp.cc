/*Administrator Application Modify Vehicle Form*/

/*Project file headers*/
#include "modVec.h"
#include "dispVec.h"
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

using namespace std;

ostringstream mvsqlstream;
string mvsqlquery;
string modmake;
string modmodel;
string modown_st;
int modvid;
int modyor;
int modmot;
int modown;

modVec::modVec() {
    widget.setupUi(this);
    widget.yorCB->addItem("Year");
    for (int i = 2017; i > 1999; i--) {
        widget.yorCB->addItem(QString::number(i));
    }
}

modVec::~modVec() {
}

void modVec::vec_tbe(int vecid) {
    modvid = vecid;
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        
        mvsqlstream << "SELECT * FROM Vehicle WHERE VehicleID = '" << vecid << "'";
        mvsqlquery = mvsqlstream.str();
        
        res = stmt->executeQuery(mvsqlquery);
        while (res->next()) {
            modmake = res->getString("Make");
            modmodel = res->getString("Model");
            modyor = res->getInt("YearOfReg");
            modmot = res->getInt("ValidMOT");
            modown = res->getInt("Owner");
        }
        
        mvsqlstream.str("");
        mvsqlstream << "SELECT Alias FROM Driver WHERE DriverID = '" << modown << "'";
        mvsqlquery = mvsqlstream.str();
        
        res = stmt->executeQuery(mvsqlquery);
        while (res->next())
            modown_st = res->getString("Alias");
        
        delete res;
        delete stmt;
        delete con;
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
    widget.makeEdit->setText(QString("%1").arg(modmake.c_str()));
    widget.modelEdit->setText(QString("%1").arg(modmodel.c_str()));
    widget.yorCB->setCurrentIndex(widget.yorCB->findText(QString("%1").arg(modyor)));
    if (modmot == 1)
        widget.motyRB->setChecked(true);
    else
        widget.motnRB->setChecked(true);
    widget.ownerEdit->setText(QString("%1").arg(modown_st.c_str()));
}

void modVec::on_modButton_clicked() {
    char *str_tbt;
    Login log;
    modmake = widget.makeEdit->text().toStdString();
    modmodel = widget.modelEdit->text().toStdString();
    modyor = widget.yorCB->currentText().toInt();
    if (widget.motyRB->isChecked())
        modmot = 1;
    else
        modmot = 0;
    modown_st = widget.ownerEdit->text().toStdString();

    /*Testing entry fields for data*/
    if (modmake.length() == 0) {
        QMessageBox::warning(this, "No Make Entered", "You have not entered a make of vehicle. Please complete all fields.");
        return;
    }
    
    if (modmodel.length() == 0) {
        QMessageBox::warning(this, "No Model Entered", "You have not entered a model of vehicle. Please complete all fields.");
        return;
    }
    
    if (modyor == 0) {
        QMessageBox::warning(this, "No Year of Registration Entered", "You have not entered a year of registration for the vehicle. Please complete all fields.");
        return;
    }
    
    if (modown_st.length() == 0) {
        QMessageBox::warning(this, "No Owner Entered", "You have not entered an owner for the vehicle. Please complete all fields.");
        return;
    }
    
    /*Whitelisting strings*/
    
    str_tbt = new char[modmake.length() + 1];
    strcpy(str_tbt, modmake.c_str());
    
    if(!(log.whiteString(str_tbt))) {
        QMessageBox::warning(this, "Make Invalid", "That make of vehicle is invalid. Vehicle makes are alphanumeric only");
        return;
    }
    
    str_tbt = new char[modmodel.length() + 1];
    strcpy(str_tbt, modmodel.c_str());
    
    if(!(log.whiteString(str_tbt))) {
        QMessageBox::warning(this, "Model Invalid", "That model of vehicle is invalid. Vehicle models are alphanumeric only");
        return;
    }
    
    str_tbt = new char[modown_st.length() + 1];
    strcpy(str_tbt, modown_st.c_str());
    
    if(!(log.whiteString(str_tbt))) {
        QMessageBox::warning(this, "Owner Invalid", "That owner alias is invalid. Aliases are alphanumeric only");
        return;
    }
    
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        res = stmt->executeQuery("SELECT Alias FROM Driver");
        bool chk_usr = false;
        while (res->next()) {
            if (res->getString("Alias") == modown_st)
                chk_usr = true;
        }
        
        if (!chk_usr) {
            QMessageBox::warning(this, "No Such Driver", "There is no driver with that alias. The vehicle could not be modified.");
            return;
        }
        
        mvsqlstream.str("");
        mvsqlstream << "SELECT DriverID FROM Driver WHERE Alias = '" << modown_st << "'";
        mvsqlquery = mvsqlstream.str();
        
        res = stmt->executeQuery(mvsqlquery);
        while (res->next())
            modown = res->getInt("DriverID");
        
        mvsqlstream.str("");
        mvsqlstream << "UPDATE Vehicle SET Make = '" << modmake << "', Model = '" << modmodel << "', YearOfReg = '"
                    << modyor << "', ValidMOT = '" << modmot << "', Owner = '" << modown << "' WHERE VehicleID = '" << modvid << "'";
        mvsqlquery = mvsqlstream.str();
        
        stmt->execute(mvsqlquery);
        
        QMessageBox::information(this, "Vehicle Updated", "This vehicle has been updated!");
        
        dispVec disp;
        disp.setModal(true);
        this->hide();
        disp.exec();
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void modVec::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}