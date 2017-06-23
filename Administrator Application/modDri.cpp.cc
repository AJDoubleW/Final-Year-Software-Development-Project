/*Administrator Application Modify Driver Form*/

/*Project file headers*/
#include "modDri.h"
#include "dispDri.h"
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

int modDriID;
int Age;
string modAlias;
string modDOB;
string modLicType;
string modLicExp;
string mdsqlquery;
ostringstream mdsqlstream;

modDri::modDri() {
    widget.setupUi(this);
    
    /*Adding selection options for Combo boxes*/
    widget.dayCB->addItem("Day");
    widget.expDayCB->addItem("Day");
    for (int i = 1; i < 32; i++) {
        widget.dayCB->addItem(QString::number(i));
        widget.expDayCB->addItem(QString::number(i));
    }
    widget.yearCB->addItem("Year");
    for (int i = 2016; i > 1899; i--) {
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

modDri::~modDri() {
}

void modDri::dri_tbe(int driID) {
    modDriID = driID;
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
        
        mdsqlstream << "SELECT * FROM Driver WHERE DriverID = '" << driID << "'";
        mdsqlquery = mdsqlstream.str();
        
        res = stmt->executeQuery(mdsqlquery);
        while (res->next()) {
            modAlias = res->getString("Alias");
            modDOB = res->getString("DateOfBirth");
            modLicType = res->getString("LicenceType");
            modLicExp = res->getString("LicenceExpDate");
        }
        
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
    
    /*Setting the text*/
    
    string dobdates[3];
    string expdates[3];
    int sep;
    int i = 0;
    char *str_tbt;
    
    widget.aliasEdit->setText(QString("%1").arg(modAlias.c_str()));
    
    /*Converting date of birth string to year/month/day*/
    modDOB.append("-");
    while (modDOB != "\0") {
        sep = modDOB.find("-");
        for (int j = 0; j < sep; j++) {
            dobdates[i] += modDOB[j];
        }
        modDOB.erase(0, sep+1);
        i++;
    }
    
    /*Removing the 0 at the start of 1-9 numbers i.e. 05 -> 5*/
    str_tbt = new char[3];
    strcpy(str_tbt, dobdates[1].c_str());
    if (strncmp("0",str_tbt, 1) == 0) {
        dobdates[1] = str_tbt[1];
    }
    str_tbt = new char[3];
    strcpy(str_tbt, dobdates[2].c_str());
    if (strncmp("0", str_tbt, 1) == 0) {
        dobdates[2] = str_tbt[1];
    }
    
    widget.yearCB->setCurrentIndex(widget.yearCB->findText(QString("%1").arg(dobdates[0].c_str())));
    widget.monCB->setCurrentIndex(widget.monCB->findText(QString("%1").arg(dobdates[1].c_str())));
    widget.dayCB->setCurrentIndex(widget.dayCB->findText(QString("%1").arg(dobdates[2].c_str())));
    
    if (modLicType == "Full")
        widget.ltfullRB->setChecked(true);
    else if (modLicType == "Provisional")
        widget.ltprovRB->setChecked(true);
    else
        widget.ltnoneRB->setChecked(true);
    
    
    i = 0;
    
    /*Converting the licence expiry date to year/month/day*/
    modLicExp.append("-");
    while (modLicExp != "\0") {
        sep = modLicExp.find("-");
        for (int j = 0; j < sep; j++) {
            expdates[i] += modLicExp[j];
        }
        modLicExp.erase(0, sep+1);
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
    
    widget.expYearCB->setCurrentIndex(widget.expYearCB->findText(QString("%1").arg(expdates[0].c_str())));
    widget.expMonCB->setCurrentIndex(widget.expMonCB->findText(QString("%1").arg(expdates[1].c_str())));
    widget.expDayCB->setCurrentIndex(widget.expDayCB->findText(QString("%1").arg(expdates[2].c_str())));
}

void modDri::on_modButton_clicked() {
    char *str_tbt;
    Login log;
    modAlias = widget.aliasEdit->text().toStdString();
    mdsqlstream.str("");
    mdsqlstream << widget.yearCB->currentText().toStdString() << "-" << widget.monCB->currentText().toStdString() << "-"
                << widget.dayCB->currentText().toStdString();
    modDOB = mdsqlstream.str();
    if (widget.ltfullRB->isChecked())
        modLicType = "Full";
    else if (widget.ltprovRB->isChecked())
        modLicType = "Provisional";
    else
        modLicType = "None";
    mdsqlstream.str("");
    mdsqlstream << widget.expYearCB->currentText().toStdString() << "-" << widget.expMonCB->currentText().toStdString() << "-"
                << widget.expDayCB->currentText().toStdString();
    modLicExp = mdsqlstream.str();
    
    /*Testing entry fields for data*/
    if (modAlias.length() == 0) {
        QMessageBox::warning(this, "No Alias Entered", "You have not entered a driver alias. Please complete all fields.");
        return;
    }
    
    if ((widget.yearCB->currentText().toInt() == 0) || (widget.monCB->currentText().toInt() == 0) || (widget.dayCB->currentText().toInt() == 0)) {
        QMessageBox::warning(this, "No Date of Birth Entered", "You have not entered a complete date of birth. Please complete all fields.");
        return;
    }
    
    if (modLicType.length() == 0) {
        QMessageBox::warning(this, "No Licence Type Entered", "You have not entered a driver licence type. Please complete all fields.");
        return;
    }
    
    if (modLicType != "None") {
        if ((widget.expYearCB->currentText().toInt() == 0) || (widget.expMonCB->currentText().toInt() == 0) || (widget.expDayCB->currentText().toInt() == 0)) {
            QMessageBox::warning(this, "No Licence Expiry Date Entered", "You have not entered a complete licence expiry date. Please complete all fields.");
            return;
        }
    }
    
    /*Checking all dates are valid*/
    if (!validDate(widget.dayCB->currentText().toInt(), widget.monCB->currentText().toInt(), widget.yearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Date of Birth", "This date of birth is invalid as it is not a real date.");
        return;
    }   
    
    if (!validDate(widget.expDayCB->currentText().toInt(), widget.expMonCB->currentText().toInt(), widget.expYearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Licence Expiry Date", "This licence expiry date is invalid. as it is not a real date.");
        return;
    }
    
    if (!test_age(widget.dayCB->currentText().toInt(), widget.monCB->currentText().toInt(), widget.yearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Date of Birth", "This date of birth is invalid. The driver would be under 17.");
        return;
    }   
    
    if (!validExp(widget.expDayCB->currentText().toInt(), widget.expMonCB->currentText().toInt(), widget.expYearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Licence Expiry Date", "This licence expiry date is invalid. The expiry date should be within 10 years.\n(Unless they are over 70, then within the next 3 years)");
        return;
    }
    
    /*whitestring alias*/
    str_tbt = new char[modAlias.length() + 1];
    strcpy(str_tbt, modAlias.c_str());
    
    if (!log.whiteString(str_tbt)) {
        QMessageBox::warning(this, "Invalid Alias", "The alias you have entered is invalid. Aliases are alphanumeric only.");
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
        
        bool chk_usr = false;
        res = stmt->executeQuery("SELECT DriverID, Alias FROM Driver");
        while (res->next()) {
            if (res->getString("Alias") == modAlias) {
                if (res->getInt("DriverID") != modDriID)
                    chk_usr = true;
            }
        }
        
        if (chk_usr) {
            QMessageBox::warning(this, "Invalid Alias", "The alias you have entered already belongs to a different driver.");
            return; 
        }
        
        mdsqlstream.str("");
        if (modLicType == "None") {
            mdsqlstream << "UPDATE Driver SET Alias = '" << modAlias << "', DateOfBirth = '" << modDOB << 
                            "', LicenceType = 'None', LicenceExpDate = NULL WHERE DriverID = '" << modDriID << "'";
        } else {
             mdsqlstream << "UPDATE Driver SET Alias = '" << modAlias << "', DateOfBirth = '" << modDOB << "', LicenceType = '" << modLicType <<
                            "', LicenceExpDate = '" << modLicExp << "' WHERE DriverID = '" << modDriID << "'";
        }
        mdsqlquery = mdsqlstream.str();
        stmt->execute(mdsqlquery);
       
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
     
    QMessageBox::information(this, "Driver Updated", "This driver has been updated!");
    dispDri disp;
    disp.setModal(true);
    this->hide();
    disp.exec();    
}

bool modDri::test_age(int day, int mon, int year) {
    //establishing the current Date	
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;

    //calculating the user age
    Age = cYear - year;

    if ((cMon < mon) || ((cMon == mon) && (cDay < day))){
        Age -= 1; //if their birthday is later in the year
    }

    if (Age >= 17) {
        return true;
    }else {
        return false;
    }
}

/* Testing the validity of the licence expiry date*/
bool modDri::validExp(int day, int mon, int year) {
    //establishing the current Date	
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;


    if ((year < cYear) || ((year == cYear) && ((mon < cMon) || ((mon == cMon) && (day < cDay)))))
        return false;
    else if (((year - cYear) > 10) || (((year - cYear) == 10) && ((mon > cMon) || ((mon == cMon) && (day > cDay))))) //driving licence photocards are only valid for 10 years
        return false;
    else if ((Age > 70) && (((year - cYear) > 3) || (((year - cYear) == 3) && ((mon > cMon) || ((mon == cMon) && (day > cDay)))))) //if over the age of 70, they are only valid for 3 years
        return false;
    else if ((Age > 60) && (((year - cYear) > (70 - Age)) || (((year - cYear) == (70-Age)) && ((mon > cMon) || ((mon == cMon) && (day > cDay))))))
        return false;
    return true;
}

bool modDri::validDate(int day, int mon, int year) {
    //establishing the current date
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;
    
    if (((mon == 2) && (day > 29)) || (((mon == 4) || (mon == 6) || (mon == 9) || (mon == 11)) && (day == 31)) 
            || ((mon == 2) && (year%4 != 0) && (day > 28)))
        return false;
    else 
        return true;
}

void modDri::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}