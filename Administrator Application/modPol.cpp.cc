/*Administrator Application Modify Policy Form*/

/*Project file headers*/
#include "dispPol.h"
#include "modPol.h"
#include "MainMenu.h"
#include "loaGetSet.h"
#include "Login.h"
#include "modDri.h"

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

int modPolDri;
int modPolVec;
string modPolAlias;
string modPolType;
string modIssue;
string modExpiry;
string ogIssue;
string ogExpiry;


modPol::modPol() {
    widget.setupUi(this);
    
    /*Adding selection options for Combo boxes*/
    widget.dayCB->addItem("Day");
    widget.expDayCB->addItem("Day");
    for (int i = 1; i < 32; i++) {
        widget.dayCB->addItem(QString::number(i));
        widget.expDayCB->addItem(QString::number(i));
    }
    widget.yearCB->addItem("Year");
    for (int i = 2024; i > 2010; i--) {
        widget.yearCB->addItem(QString::number(i));
    }
    widget.expYearCB->addItem("Year");
    for (int i = 2016; i < 2030; i++) {
        widget.expYearCB->addItem(QString::number(i));
    }
    widget.monCB->addItem("Month");
    widget.expMonCB->addItem("Month");
    for (int i = 1; i < 13; i++) {
        widget.monCB->addItem(QString::number(i));
        widget.expMonCB->addItem(QString::number(i));
    }
}

modPol::~modPol() {
}

void modPol::pol_tbe(char driver[], int vecID, char polt[], char idate[], char edate[]) {
    modPolType = polt;
    modIssue = idate;
    modExpiry = edate;
    modPolAlias = driver;
    modPolVec = vecID;
    
    widget.aliasEdit->setText(QString("%1").arg(modPolAlias.c_str()));
    widget.vidEdit->setText(QString("%1").arg(vecID));
    widget.typeEdit->setText(QString("%1").arg(modPolType.c_str()));
    
    string issdates[3];
    string expdates[3];
    int sep;
    int i = 0;
    char *str_tbt;
    
    /*Converting licence issue date string to year/month/day*/
    ogIssue = modIssue;
    ogExpiry = modExpiry;
    modIssue.append("-");
    while (modIssue != "\0") {
        sep = modIssue.find("-");
        for (int j = 0; j < sep; j++) {
            issdates[i] += modIssue[j];
        }
        modIssue.erase(0, sep+1);
        i++;
    }
    
    /*Removing the 0 at the start of 1-9 numbers i.e. 05 -> 5*/
    str_tbt = new char[3];
    strcpy(str_tbt, issdates[1].c_str());
    if (strncmp("0",str_tbt, 1) == 0) {
        issdates[1] = str_tbt[1];
    }
    str_tbt = new char[3];
    strcpy(str_tbt, issdates[2].c_str());
    if (strncmp("0", str_tbt, 1) == 0) {
        issdates[2] = str_tbt[1];
    }
    
    widget.yearCB->setCurrentIndex(widget.yearCB->findText(QString("%1").arg(issdates[0].c_str())));
    widget.monCB->setCurrentIndex(widget.monCB->findText(QString("%1").arg(issdates[1].c_str())));
    widget.dayCB->setCurrentIndex(widget.dayCB->findText(QString("%1").arg(issdates[2].c_str())));
    
    i = 0;
    /*Converting licence expiry date string to year/month/day*/
    modExpiry.append("-");
    while (modExpiry != "\0") {
        sep = modExpiry.find("-");
        for (int j = 0; j < sep; j++) {
            expdates[i] += modExpiry[j];
        }
        modExpiry.erase(0, sep+1);
        i++;
    }
    
    /*Removing the 0 at the start of 1-9 numbers i.e. 05 -> 5*/
    str_tbt = new char[3];
    strcpy(str_tbt, expdates[1].c_str());
    if (strncmp("0",str_tbt, 1) == 0) {
        expdates[1] = str_tbt[1];
    }
    str_tbt = new char[3];
    strcpy(str_tbt, expdates[2].c_str());
    if (strncmp("0", str_tbt, 1) == 0) {
        expdates[2] = str_tbt[1];
    }
    
    printf("%s", modExpiry.c_str());
    widget.expYearCB->setCurrentIndex(widget.expYearCB->findText(QString("%1").arg(expdates[0].c_str())));
    widget.expMonCB->setCurrentIndex(widget.expMonCB->findText(QString("%1").arg(expdates[1].c_str())));
    widget.expDayCB->setCurrentIndex(widget.expDayCB->findText(QString("%1").arg(expdates[2].c_str())));
    
}

void modPol::on_modButton_clicked() {
    char *str_tbt;
    Login log;
    ostringstream mpsqlstream;
    string mpsqlquery;
    string newmpAlias;
    string newmpType;
    string newmpIssue;
    string newmpExpiry;
    string issueMon;
    string expiryMon;
    int newmpVec;
    int newmpDri;
    
    newmpAlias = widget.aliasEdit->text().toStdString();
    newmpVec = widget.vidEdit->text().toInt();
    newmpType = widget.typeEdit->text().toStdString();
    
    mpsqlstream.str("");
    mpsqlstream << widget.yearCB->currentText().toStdString() << "-" << widget.monCB->currentText().toStdString() << "-" << widget.dayCB->currentText().toStdString();
    
    str_tbt = new char[3];
    strcpy(str_tbt, widget.expMonCB->currentText().toStdString().c_str());
    if (strncmp("0", str_tbt, 1) == 0) {
        expiryMon = str_tbt[1];
    } else {
        expiryMon = widget.expMonCB->currentText().toStdString();
    }
    newmpIssue = mpsqlstream.str();
    mpsqlstream.str("");
    mpsqlstream << widget.expYearCB->currentText().toStdString() << "-" << expiryMon << "-"
                << widget.expDayCB->currentText().toStdString();
    newmpExpiry = mpsqlstream.str();
    
    /*Testing entry fields for data*/
    if (newmpAlias.length() == 0) {
        QMessageBox::warning(this, "No Alias Entered", "You have not entered a driver alias. Please complete all fields.");
        return;
    }
    
    if (newmpVec == 0) {
        QMessageBox::warning(this, "No Valid Vehicle Entered", "You have not entered a valid Vehicle ID. Please complete all fields.");
        return;
    }
    
    if (newmpType.length() == 0) {
        QMessageBox::warning(this, "No Policy Type Entered", "You have not entered a Policy Type. Please complete all fields.");
        return;
    }
    
    if ((widget.yearCB->currentText().toInt() == 0) || (widget.monCB->currentText().toInt() == 0) || (widget.dayCB->currentText().toInt() == 0)) {
        QMessageBox::warning(this, "No Issue Date Entered", "You have not entered a complete issue date for the policy. Please complete all fields.");
        return;
    }
    
    if ((widget.expYearCB->currentText().toInt() == 0) || (widget.expMonCB->currentText().toInt() == 0) || (widget.expDayCB->currentText().toInt() == 0)) {
        QMessageBox::warning(this, "No Policy Expiry Date Entered", "You have not entered a complete expiry date for the policy. Please complete all fields.");
        return;
    }
    
    /*Testing Validity of the Dates*/
    if (!validDates(widget.dayCB->currentText().toInt(), widget.monCB->currentText().toInt(), widget.yearCB->currentText().toInt(),
                widget.expDayCB->currentText().toInt(), widget.expMonCB->currentText().toInt(), widget.expYearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Issue/Expiry Date", "There is an issue with either the issue or expiry date. Please ensure both dates are valid.");
        return;
    }
    
    /*Whitelisting strings*/
    str_tbt = new char[newmpAlias.length() + 1];
    strcpy(str_tbt, newmpAlias.c_str());
    if (!log.whiteString(str_tbt)) {
        QMessageBox::warning(this, "Invalid Alias", "The alias you have entered is invalid. Aliases are alphanumeric only.");
        return;
    }
    
    /*str_tbt = new char[modPolType.length() + 1];
    strcpy(str_tbt, modPolType.c_str());
    if (!log.whiteString(str_tbt)) {
        QMessageBox::warning(this, "Invalid Licence Type", "The licence type you have entered is invalid. Licence Types are alphanumeric only.");
        return;
    }  CHANGE THIS TO A COMBO BOX SO NO WHITELIST AND JUST LENGTH IT */
    
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
        
        bool chk_usr = false;
        res = stmt->executeQuery("SELECT DriverID, Alias FROM Driver");
        while (res->next()) {
            if (res->getString("Alias") == modPolAlias) {
                modPolDri = res->getInt("DriverID");
            }
            if (res->getString("Alias") == newmpAlias) {
                chk_usr = true;
                newmpDri = res->getInt("DriverID");
            }
        }
        
        /*If there is no driver with that alias, display error*/
        if(!(chk_usr)){
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
        string licencetype;
        sqlstream << "SELECT LicenceType FROM Driver WHERE Alias = '" << newmpAlias << "'";
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
            sqlstream << "SELECT FirstFullLicence,Year(LicenceExpDate),Month(LicenceExpDate),Day(LicenceExpDate) FROM Driver WHERE DriverID = '" << newmpDri << "'";
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
            
            modDri mdri;
            /* If their driving licence has expired, display error */
            if (!(mdri.validExp(eday, emon, eyear))){
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
            if (res->getInt("VehicleID") == newmpVec)
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
        sqlstream << "SELECT ValidMOT FROM Vehicle WHERE VehicleID = '" << newmpVec << "'";
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
        sqlstream << "SELECT DriverID, Day(IssueDate), Month(IssueDate), Year(IssueDate), Day(ExpiryDate), Month(ExpiryDate), Year(ExpiryDate) FROM InsurancePolicy WHERE VehicleID = '" << newmpVec <<"'";
        sqlquery = sqlstream.str();
        res = stmt->executeQuery(sqlquery);
        while (res->next()){
            if (res->getInt("DriverID") == newmpDri) {
                peday = res->getInt("Day(ExpiryDate)");
                pemon = res->getInt("Month(ExpiryDate)");
                peyear = res->getInt("Year(ExpiryDate)");
                piday = res->getInt("Day(IssueDate)");
                pimon = res->getInt("Month(IssueDate)");
                piyear = res->getInt("Year(IssueDate)");
                if((mdri.validDate(peday, pemon, peyear)) && (!(mdri.validDate(piday, pimon, piyear))))
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
        int oth_driID;
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
            if((!(otherDriver)) || (!(mdri.validDate(peday, pemon, peyear)))) {
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
            if ((!mdri.validDate(oth_eday, oth_emon, oth_eyear)) || ((!(calculate_length(oth_eday, oth_emon, oth_eyear))) && (ffl == 1))){
                QMessageBox::StandardButton cont;
                cont = QMessageBox::warning(this, "Licence Expired", "The full licence holder's licence has expired. A provisional licence holder must be accompanied at all times.\nTry again?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
                if (cont == QMessageBox::No)
                    exit(0);
                return;
            }
     
        }
        
        mpsqlstream.str("");
        mpsqlstream << "UPDATE InsurancePolicy SET DriverID = '" << newmpDri << "', VehicleID = '" << newmpVec << "', PolicyType = '"
                    << newmpType << "', IssueDate = '" << newmpIssue << "', ExpiryDate = '" << newmpExpiry << "' WHERE DriverID = '"
                    << modPolDri << "'AND VehicleID = '" << modPolVec << "'AND PolicyType = '" << modPolType << "'AND IssueDate = '" << 
                    ogIssue << "'AND ExpiryDate = '" << ogExpiry << "'";
        mpsqlquery = mpsqlstream.str();
        
        
        stmt->execute(mpsqlquery);
        
        
        QMessageBox::information(this, "Policy Updated", "This policy has been updated!");
        
        dispPol disp;
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

/*Testing the starting date validity*/
bool modPol::validDates(int day, int mon, int year, int expday, int expmon, int expyear) {
      
    if (((mon == 2) && (day > 29)) || (((mon == 4) || (mon == 6) || (mon == 9) || (mon == 11)) && (day == 31)) || ((mon == 2) && (year%4 != 0) && (day > 28))) {
        return false;
    } else if (((expmon == 2) && (expday > 29)) || (((expmon == 4) || (expmon == 6) || (expmon == 9) || (expmon == 11)) && (expday == 31)) || ((expmon == 2) && (expyear%4 != 0) && (expday > 28))) {
        return false;
    } else if ((expyear < year) || ((expyear == year) && (expmon < mon)) || ((expyear == year) && (expmon == mon) && (expday < day))) {
        return false;
    } else if (((expyear - year) > 5) || (((expyear - year) == 5) && (((expmon - mon) > 0) || (((expmon - mon) == 0) && ((expday - day) > 0))))) {
        return false;
    }
    return true;
}

void modPol::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}

bool modPol::calculate_length(int day, int mon, int year) {
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

bool modPol::display_ex_pol() {
    QMessageBox::StandardButton cont;
    cont = QMessageBox::question(this, "Already insured", "That driver is already insured on that vehicle. Do you wish to extend their policy?",
                        QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
    
    if (cont == QMessageBox::Yes)
        return true;
    return false;
}
