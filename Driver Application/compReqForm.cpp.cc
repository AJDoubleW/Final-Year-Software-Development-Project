/*Comprehensive Permission Form*/

/*Project file headers*/
#include "compReqForm.h"
#include "RegForm.h"
#include "MainMenu.h"
#include "encrypt.hpp"

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

ostringstream crsqlstream;
string crsqlquery;
string ownerAlias;
string reqPass;
string driverAlias;
string coverFrom;
string coverUntil;
string licencetype;
int reqvid;
int reqDriID;
int creDriID;

compReqForm::compReqForm() {
    widget.setupUi(this);
    
    /*Adding combo box options*/
    widget.fromDayCB->addItem("Day");
    widget.untilDayCB->addItem("Day");
    for (int i = 1; i < 32; i++) {
        widget.fromDayCB->addItem(QString::number(i));
        widget.untilDayCB->addItem(QString::number(i));
    }
    widget.fromYearCB->addItem("Year");
    for (int i = 2022; i > 2016; i--) {
        widget.fromYearCB->addItem(QString::number(i));
    }
    widget.untilYearCB->addItem("Year");
    for (int i = 2025; i > 2017; i--) {
        widget.untilYearCB->addItem(QString::number(i));
    }
    widget.fromMonCB->addItem("Month");
    widget.untilMonCB->addItem("Month");
    for (int i = 1; i < 13; i++) {
        widget.fromMonCB->addItem(QString::number(i));
        widget.untilMonCB->addItem(QString::number(i));
    }
    
}

compReqForm::~compReqForm() {
}

void compReqForm::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}

void compReqForm::on_submitButton_clicked() {
    char *str_tbt;
    RegForm regf;
    ownerAlias = widget.ownerAliasEdit->text().toStdString();
    reqPass = widget.backPass->text().toStdString();
    driverAlias = widget.driverAliasEdit->text().toStdString();
    reqvid = widget.vidEdit->text().toInt();
    
    crsqlstream.str("");
    crsqlstream << widget.fromYearCB->currentText().toStdString() << "-" << widget.fromMonCB->currentText().toStdString() << "-"
                    << widget.fromDayCB->currentText().toStdString();
    coverFrom = crsqlstream.str();
    
    crsqlstream.str("");
    crsqlstream << widget.untilYearCB->currentText().toStdString() << "-" << widget.untilMonCB->currentText().toStdString() << "-"
                    << widget.untilDayCB->currentText().toStdString();
    coverUntil = crsqlstream.str();
    
    /*Testing entry strings for data*/
    if (ownerAlias.length() == 0) {
        QMessageBox::warning(this, "No Alias Entered", "You have not entered your driver alias. Please complete all fields.");
        return;
    }
    
    if (reqPass.length() == 0) {
        QMessageBox::warning(this, "No Password Entered", "You have not entered your backup password. Please complete all fields.");
        return;
    }
    
    if (driverAlias.length() == 0) {
        QMessageBox::warning(this, "No Alias Entered", "You have not entered the alias of the driver you wish to grant permission to. Please complete all fields.");
        return;
    }
    
    if(reqvid == 0) {
        QMessageBox::warning(this, "Invalid VehicleID", "The VehicleID you entered is invalid. Vehicle IDs are positive integers.");
        return;
    }
    
    if ((widget.fromYearCB->currentText().toInt() == 0) || (widget.fromMonCB->currentText().toInt() == 0) || (widget.fromDayCB->currentText().toInt() == 0)) {
        QMessageBox::warning(this, "No Starting Date Entered", "You have not entered a starting date for the cover. Please complete all fields.");
        return;
    }
    
    if ((widget.untilYearCB->currentText().toInt() == 0) || (widget.untilMonCB->currentText().toInt() == 0) || (widget.untilDayCB->currentText().toInt() == 0)) {
        QMessageBox::warning(this, "No Finishing Date Entered", "You have not entered a finishing date for the cover. Please complete all fields.");
        return;
    }
    
   /*Whitelisting strings*/ 
    str_tbt = new char[ownerAlias.length() + 1];
    strcpy(str_tbt, ownerAlias.c_str());
    
    if(!(regf.whiteString(str_tbt))) {
        QMessageBox::warning(this, "Invalid Alias", "The Alias you entered for yourself is invalid. Aliases are alphanumeric only.");
        return;
    }
    
    str_tbt = new char[driverAlias.length() + 1];
    strcpy(str_tbt, driverAlias.c_str());
    
    if(!(regf.whiteString(str_tbt))) {
        QMessageBox::warning(this, "Invalid Alias", "The Alias you entered for the other driver is invalid. Aliases are alphanumeric only.");
        return;
    }
    
    str_tbt = new char[reqPass.length() + 1];
    strcpy(str_tbt, reqPass.c_str());
    
    if(!(regf.whiteString(str_tbt))) {
        QMessageBox::warning(this, "Invalid Password", "The Password you entered is invalid. Passwords are alphanumeric only.");
        return;
    }
    
    /*Validating dates*/
    if(!(regf.validDate(widget.fromDayCB->currentText().toInt(), widget.fromMonCB->currentText().toInt(), widget.fromYearCB->currentText().toInt()))) {
        QMessageBox::warning(this, "Invalid Starting Date", "The starting date you entered is invalid as it is not a real date.Please double check your starting date.");
        return;
    }
    
    if(!(regf.validDate(widget.untilDayCB->currentText().toInt(), widget.untilMonCB->currentText().toInt(), widget.untilYearCB->currentText().toInt()))) {
        QMessageBox::warning(this, "Invalid Finishing Date", "The finishing date you entered is invalid as it is not a real date.Please double check your finishing date.");
        return;
    }
    
    if (!orderDate(widget.fromDayCB->currentText().toInt(), widget.fromMonCB->currentText().toInt(), widget.fromYearCB->currentText().toInt(),
                widget.untilDayCB->currentText().toInt(), widget.untilMonCB->currentText().toInt(), widget.untilYearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Cover Dates", "Either your finishing date is before your starting date or your finishing date is more than 5 years after your starting date.\nPlease double check both dates.");
        return;
    }
    
    create_req();
}

void compReqForm::create_req() {
    Encrypt enc;
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the MySQL database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
  
        /*finding the user with entered username*/
        res = stmt->executeQuery("SELECT DriverID,Alias FROM Driver");
        bool chk_usr = false;
        while (res->next()){
            if (res->getString("Alias") == ownerAlias) {
                chk_usr = true;
                creDriID = res->getInt("DriverID");
            }
        }
                
        /*If there is no such user, output error message*/
        if(!(chk_usr)){
            QMessageBox::warning(this, "No Such Driver", "The Alias you entered for yourself does not belong to to any registered driver.");
            return;
        }
        
        /*Ensure the driver is the owner of a vehicle*/
        res = stmt->executeQuery("SELECT VehicleID, Owner FROM Vehicle");
        bool chk_vec = false;
        bool chk_own = false;
        while (res->next()) {
            if (res->getInt("VehicleID") == reqvid) {
                chk_vec = true;
                if (res->getInt("Owner") == creDriID)
                    chk_own = true;
            }
        }
        
        /*If the vehicle does not exist, discontinue*/
        if (!(chk_vec)) {
            QMessageBox::warning(this, "No Such Vehicle", "The VehicleID you entered does not belong to to any registered vehicle.");
            return;
        }
        
        /*If the driver is not a vehicle owner, they cannot continue*/
        if (!(chk_own)) {
            QMessageBox::warning(this, "Not Vehicle Owner", "Our records show that you are not the owner of this vehicle, and as such you cannot grant another driver permission to drive it.");
            return;
        }
        
        /*finding the user with entered username*/
        res = stmt->executeQuery("SELECT DriverID,Alias,LicenceType FROM Driver");
        bool chk_dri = false;
        bool chk_lic = false;
        while (res->next()){
            if (res->getString("Alias") == driverAlias) {
                chk_dri = true;
                reqDriID = res->getInt("DriverID");
                if (res->getString("LicenceType") != "None")
                    chk_lic = true;
            }
        }
        
        /*If there is no such user, output error message*/
        if(!(chk_dri)){
            QMessageBox::warning(this, "No Such Driver", "The Alias you entered for the other driver does not belong to to any registered driver.");
            return;
        }
        
        if (!(chk_lic)) {
            QMessageBox::warning(this, "No Licence", "The driver you wish to grant permission to does not have a licence.");
            return;
        }
        
        char *alias;
        alias = new char[ownerAlias.length() + 1];
        strcpy(alias, ownerAlias.c_str());
        char *pass;
        pass = new char[reqPass.length() + 1];
        strcpy(pass, reqPass.c_str());
        
        string encpass = enc.enctext(pass, alias);
        
        crsqlstream.str("");
        crsqlstream << "SELECT Backup_Password FROM Driver WHERE Alias = '" << ownerAlias << "'";
        crsqlquery = crsqlstream.str();
        res = stmt->executeQuery(crsqlquery);
        bool chk_pass = false;
        while (res->next()) {
            if (res->getString("Backup_Password") == encpass)
                chk_pass = true;
        }
        
        /* If they enter the wrong password, take them to the main menu */
        if (!(chk_pass)) {
            QMessageBox::warning(this, "Incorrect Password", "The password you have entered is incorrect.");
            on_homeButton_clicked();
            return;
        }
        
        /*Check if the driver they are granting permission to has comprehensive cover */
        crsqlstream.str("");
        crsqlstream << "SELECT PolicyType, VehicleID FROM InsurancePolicy WHERE DriverID = '" << reqDriID << "'";
        crsqlquery = crsqlstream.str();
        res = stmt->executeQuery(crsqlquery);
        bool isComp = false;
        bool isInsured = false;
        while (res->next()) {
            if (res->getString("PolicyType") == "Fully Comprehensive") {
                isComp = true;
                if (res->getInt("VehicleID") == reqvid)
                    isInsured = true;
            }
        }
        
        if (!(isComp)) {
            QMessageBox::warning(this, "Not Comprehensive", "This driver does not have comprehensive cover on their own vehicle. If you wish to insure them, contact the insurer directly.");
            return;
        }
        
        if (isInsured) {
            QMessageBox::information(this, "Already Insured", "This driver is already insured on your vehicle. They have permission to drive it.");
            return;
        }
        
        crsqlstream.str("");
        crsqlstream << "INSERT INTO CompPerms (CreatorID, VehicleID, DriverID, CoverFrom, CoverUntil) VALUES ('" << creDriID << "','" << reqvid << "','" << reqDriID << "','"
                    << coverFrom << "','" << coverUntil << "')";
        crsqlquery = crsqlstream.str();
        
        stmt->execute(crsqlquery);
        
        QMessageBox::information(this, "Request Created", "You have successfully created this request. The Insurer should either approve or deny it in 3-5 days.");
        on_homeButton_clicked();
        
       /* output any SQL error*/
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}



bool compReqForm::orderDate(int day, int mon, int year, int expday, int expmon, int expyear) {
      
    if ((expyear < year) || ((expyear == year) && (expmon < mon)) || ((expyear == year) && (expmon == mon) && (expday < day))) {
        return false;
    } else if (((expyear - year) > 5) || (((expyear - year) == 5) && (((expmon - mon) > 0) || (((expmon - mon) == 0) && ((expday - day) > 0))))) {
        return false;
    }
    return true;
}
