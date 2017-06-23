/*Insurer Registration Form*/

/*Project file headers*/
#include "newPol.h"
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

string alias;
string poltype;
string lyearst;
string lmonst;
int vid;
int sday;
int smon;
int syear;
int lmon;
int lyear;
int expmon;
int expyear;

newPol::newPol() {
    widget.setupUi(this);
    
    /*Adding numeric options for date Combo boxes*/
    widget.dayCB->addItem("Day");
    for (int i = 1; i < 32; i++) {
        widget.dayCB->addItem(QString::number(i));
    }

    widget.yearCB->addItem("Year");
    for (int i = 2017; i < 2023; i++) {
        widget.yearCB->addItem(QString::number(i));
    }
    widget.monCB->addItem("Month");
    widget.monCB_2->addItem("Months");
    for (int i = 0; i < 12; i++) {
        widget.monCB->addItem(QString::number(i));
        widget.monCB_2->addItem(QString::number(i));
    }
    widget.yearCB_2->addItem("Years");
    for (int i = 0; i < 6; i++) {
        widget.yearCB_2->addItem(QString::number(i));
    }
    
    /*Adding policy types to combo box*/
    widget.polTypeCB->addItem("Policy Type");
    widget.polTypeCB->addItem("Fully Comprehensive");
    widget.polTypeCB->addItem("Third Party");
    widget.polTypeCB->addItem("Third Party Fire and Theft");
    
    
}

newPol::~newPol() {
}

void newPol::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}

void newPol::on_submitButton_clicked() {
    char *str_tbt;
    Login log;
    
    /*setting variables*/
    alias = widget.aliasEdit->text().toStdString();
    vid = widget.vidEdit->text().toInt();
    poltype = widget.polTypeCB->currentText().toStdString();
    sday = widget.dayCB->currentText().toInt();
    smon = widget.monCB->currentText().toInt();
    syear = widget.yearCB->currentText().toInt();
    lmonst = widget.monCB_2->currentText().toStdString();
    lyearst = widget.yearCB_2->currentText().toStdString();
    str_tbt = new char[alias.length() + 1];
    strcpy(str_tbt, alias.c_str());
    
    /*Testing numerics*/
    if(vid == 0) {
        QMessageBox::warning(this, "Invalid VehicleID", "The VehicleID you entered is invalid. Vehicle IDs are positive integers.");
        return;
    }
    
    if (sday == 0) {
        QMessageBox::warning(this, "No Starting Day Selected", "Please ensure you have selected a complete starting date.");
        return;
    }
    
    if (smon == 0) {
        QMessageBox::warning(this, "No Starting Month Selected", "Please ensure you have selected a complete starting date.");
        return;
    }
    
    if (syear == 0) {
        QMessageBox::warning(this, "No Starting Year Selected", "Please ensure you have selected a complete starting date.");
        return;
    }
    
    if (lmonst == "Month") {
        QMessageBox::warning(this, "No Policy Length Month Selected", "Please ensure you have selected a complete policy length.");
        return;
    } else {
        lmon = atoi(lmonst.c_str());
    }
    
    if (lyearst == "Year") {
        QMessageBox::warning(this, "No Policy Length Year Selected", "Please ensure you have selected a complete policy length.");
        return;
    } else {
        lyear = atoi(lyearst.c_str());
    }
    
    /*Testing Policy Type and Alias for entry*/
    if (poltype == "Policy Type") {
        QMessageBox::warning(this, "No Policy Type Selected", "Please ensure you have selected a policy type.");
        return;
    }
   
    /*Checking alias adheres to whitestring - the cast to int for vehicle ID does this already*/

    if (!(log.whiteString(str_tbt))) {
        QMessageBox::warning(this, "Invalid Alias", "The alias you have entered is invalid. Aliases are alphanumeric only.");
        return;
    }
       
    
    /*Validating Starting Date and Policy Length*/
    if (!(validDate(sday, smon, syear))) {
        QMessageBox::warning(this, "Invalid Starting Date", "The Starting Date you have entered is invalid. Ensure you select a valid date that is not in the past.");
        return;
    }
    
    if (!(validLength(lmon, lyear))) {
        QMessageBox::warning(this, "Invalid Policy Length", "The policy length you have entered is invalid. Ensure you enter a policy length between 6 months and 5 years.");
        return;
    }
    
    /*If all data is valid - query*/
    create_new_pol();
}

/*Testing the starting date validity*/
bool newPol::validDate(int day, int mon, int year) {
    //establishing the current date
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;
    
    if ((year < cYear) || ((year == cYear) && (mon < cMon)) || ((year == cYear) && (mon == cMon) && (day < cDay))) {
        return false;
    } else if (((mon == 2) && (day > 29)) || (((mon == 4) || (mon == 6) || (mon == 9) || (mon == 11)) && (day == 31)) || ((mon == 2) && (year%4 != 0) && (day > 28))) {
        return false;
    } 
    
    return true;
}

bool newPol::validLength(int mon, int year) {
    if ((year == 5) && (mon != 0)) {
        return false;
    } else if ((year == 0) && (mon < 6)) {
        return false;
    }
    
    expyear = syear + year;
    if ((smon + mon) > 12) {
        expmon = (smon + mon) - 12;
    } else {
        expmon = smon + mon;
    }
    return true;
}

void newPol::create_new_pol() {
    int driverid;
    int oth_driID;
    string licencetype;
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
            if (res->getString("Alias") == alias){
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
        
        /* Check to return the licencetype of the driver*/
        ostringstream sqlstream;
        string sqlquery;
        sqlstream << "SELECT LicenceType FROM Driver WHERE Alias = '" << alias << "'";
        sqlquery = sqlstream.str();
        res = stmt->executeQuery(sqlquery);
        while (res->next()) {
            licencetype = res->getString("LicenceType");               
        }
        delete res;
        
        /* If the driver has no licence, display error*/
        if (licencetype == "None") {
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "No licence", "The driver with that alias has no driver's licence.\nTry again?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
            if (cont == QMessageBox::No)
                exit(0);
            return;
        }
        
        /* Check the licence expiry date of the driver */
            sqlstream.str("");
            sqlstream << "SELECT FirstFullLicence,Year(LicenceExpDate),Month(LicenceExpDate),Day(LicenceExpDate) FROM Driver WHERE DriverID = '" << driverid << "'";
            sqlquery = sqlstream.str();
            res = stmt->executeQuery(sqlquery);
            int eday;
            int emon;
            int eyear;
            int ffl;
            while (res->next()) {
                eday = res->getInt("Day(LicenceExpDate)");
                emon = res->getInt("Month(LicenceExpDate)");
                eyear = res->getInt("Year(LicenceExpDate)");
                ffl = res->getInt("FirstFullLicence");
            }
            
            /* If their driving licence has expired, display error */
            if (!(validDate(eday, emon, eyear))){
                QMessageBox::StandardButton cont;
                cont = QMessageBox::warning(this, "Licence Expired", "Theat driver's licence has expired.\nTry again?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
                if (cont == QMessageBox::No)
                    exit(0);
                return;
            }        
        
        /*check to see if the vehicle exists*/
        bool exVehicle = false;
        res = stmt->executeQuery("SELECT VehicleID FROM Vehicle");
        while (res->next()){
            if (res->getInt("VehicleID") == vid)
                exVehicle = true;
        }
        delete res;
        
        /* If that vehicle does not exist, display error */
        if(!(exVehicle)){
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "No such vehicle", "There is no vehicle with that Vehicle ID.\nTry again?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
            if (cont == QMessageBox::No)
                exit(0);
            return;
        }
        
        /* Check if the vehicle has an MOT */
        bool exMOT = false;
        sqlstream.str(""); //clear string stream
        sqlstream << "SELECT ValidMOT FROM Vehicle WHERE VehicleID = '" << vid << "'";
        sqlquery = sqlstream.str();
        res = stmt->executeQuery(sqlquery);
        while (res->next()){
            if (res->getInt("ValidMOT") == 1)
                exMOT = true;
        }
        delete res;
        
        /* If that vehicle does not have a valid MOT, display error */
        if(!(exMOT)) {
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "No MOT", "That vehicle does not have a valid MOT.\nTry again?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
            if (cont == QMessageBox::No)
                exit(0);
            return;
        }
        /* Check if driver is already insured on this vehicle */
        bool exPolicy = false;
        int peday;
        int pemon;
        int peyear;
        int piday;
        int pimon;
        int piyear;
        sqlstream.str("");
        sqlstream << "SELECT DriverID, Day(IssueDate), Month(IssueDate), Year(IssueDate), Day(ExpiryDate), Month(ExpiryDate), Year(ExpiryDate) FROM InsurancePolicy WHERE VehicleID = '" << vid <<"'";
        sqlquery = sqlstream.str();
        res = stmt->executeQuery(sqlquery);
        while (res->next()){
            if (res->getInt("DriverID") == driverid) {
                peday = res->getInt("Day(ExpiryDate)");
                pemon = res->getInt("Month(ExpiryDate)");
                peyear = res->getInt("Year(ExpiryDate)");
                piday = res->getInt("Day(IssueDate)");
                pimon = res->getInt("Month(IssueDate)");
                piyear = res->getInt("Year(IssueDate)");
                if((validDate(peday, pemon, peyear)) && (!(validDate(piday, pimon, piyear))))
                    exPolicy = true;
            }                
        }
        delete res;
        
        
        
        /* If the driver is insured already on the vehicle, alert user */
        if(exPolicy){
            if (!display_ex_pol())
                on_homeButton_clicked();
        }
        
        /*If the driver holds a provisional licence, check if there is a full licence holder insured on the vehicle */
        bool otherDriver = false;
        if (licencetype == "Provisional"){
            res = stmt->executeQuery("SELECT Driver.DriverID, LicenceType, Year(ExpiryDate), Month(ExpiryDate), Day(ExpiryDate) FROM Driver INNER JOIN InsurancePolicy WHERE Driver.DriverID = InsurancePolicy.DriverID");
            while (res->next()){
                if (res->getString("LicenceType") == "Full") {
                    otherDriver = true;
                    oth_driID = res->getInt("DriverID");
                    peday = res->getInt("Day(ExpiryDate)");
                    pemon = res->getInt("Month(ExpiryDate)");
                    peyear = res->getInt("Year(ExpiryDate)");
                }
            }
        
            /* If there is no full licence holder insured on the vehicle, display error */
            if((!(otherDriver)) || (!(validDate(peday, pemon, peyear)))) {
                QMessageBox::StandardButton cont;
                cont = QMessageBox::warning(this, "No Full Licence Holder", "There is no full licence holder insured on that vehicle. A provisional licence holder must be accompanied at all times.\nTry again?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
                if (cont == QMessageBox::No)
                    exit(0);
                return;
            }
            
            /* Check the licence expiry date of the full licence holder */
            sqlstream.str("");
            sqlstream << "SELECT FirstFullLicence,Year(LicenceExpDate),Month(LicenceExpDate),Day(LicenceExpDate) FROM Driver WHERE DriverID = '" << oth_driID << "'";
            sqlquery = sqlstream.str();
            res = stmt->executeQuery(sqlquery);
            int oth_eday;
            int oth_emon;
            int oth_eyear;
            while (res->next()) {
                oth_eday = res->getInt("Day(LicenceExpDate)");
                oth_emon = res->getInt("Month(LicenceExpDate)");
                oth_eyear = res->getInt("Year(LicenceExpDate)");
                ffl = res->getInt("FirstFullLicence");
            }
            
            /* If they have not had a full licence for 3 years, display error */
            if ((!validDate(oth_eday, oth_emon, oth_eyear)) || ((!(calculate_length(oth_eday, oth_emon, oth_eyear))) && (ffl == 1))){
                QMessageBox::StandardButton cont;
                cont = QMessageBox::warning(this, "Licence Expired", "The full licence holder's licence has expired. A provisional licence holder must be accompanied at all times.\nTry again?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
                if (cont == QMessageBox::No)
                    exit(0);
                return;
            }
     
        }
        
        /*drop any existing policy*/
        sqlstream.str("");
        sqlstream << "DELETE FROM InsurancePolicy WHERE DriverID = '" << driverid << "' AND VehicleID = '" << vid << "'";
        /* Insert the policy into the database */
        sqlstream.str("");
        sqlstream << "INSERT INTO InsurancePolicy (DriverID,VehicleID,PolicyType,IssueDate,ExpiryDate) VALUES ('"
                << driverid << "','" << vid << "','" << poltype << "','" << syear << "-" << smon << "-" << sday << "','" << expyear << "-" << expmon << "-" << sday << "')";
        sqlquery = sqlstream.str();
        stmt->execute(sqlquery);
        QMessageBox::information(this, "Policy Created", QString("%1 has been insured on vehicle %2 until %3/%4/%5\nType: %6").arg(alias.c_str()).arg(vid).arg(sday).arg(expmon).arg(expyear).arg(poltype.c_str()));
        on_homeButton_clicked();
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

bool newPol::display_ex_pol() {
    QMessageBox::StandardButton cont;
    cont = QMessageBox::question(this, "Already insured", "That driver is already insured on that vehicle. Do you wish to extend their policy?",
                        QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
    
    if (cont == QMessageBox::Yes)
        return true;
    return false;
}

bool newPol::calculate_length(int day, int mon, int year) {
    //establishing the current Date
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;
    
    if ((year - cYear) > 7) { //if they have held a full licence for less than 3 years
        return false;
    } else if ((year - cYear) < 7) { //if they have held a full licence for more than 3 years
        return true;
    } else if ((year - cYear) == 7){ //if the number of years is exactly 3
        if (((mon - cMon) == 0) && ((day - cDay) == 0)) { //there should be no additional months or days
            return true;
        } else {
            return false;
        }
    }
}