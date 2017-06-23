/*START VEHICLE FORM*/

/*Project file headers*/
#include "Ve_sim.h"
#include "backPassForm.h"
#include "RegForm.h"
#include "MainMenu.h"
#include "gen_coords.h"

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

int vid;
int driverid;
int firstdriverid;
int numDoors;
int doors[4] = {0,0,0,0};
int seatbelts[5] = {0,0,0,0,0};
string offence;
string lictype;
string sqlquery;
ostringstream sqlstream;



Ve_sim::Ve_sim() {
    widget.setupUi(this);
    
}

Ve_sim::~Ve_sim() {
}

void Ve_sim::setDriverID(int id) {
    driverid = id;
}

void Ve_sim::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}

void Ve_sim::on_scanButton_clicked() {
    vid = widget.lineEdit->text().toInt();
    int year;
    int month;
    int day;
    backPassForm bpf;
    RegForm regf;
    
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
        
        res = stmt->executeQuery("SELECT VehicleID,ValidMOT, NumDoors FROM Vehicle");
        bool chk_vid = false;
        bool exMOT = false;
        while (res->next()) {
            if (res->getInt("VehicleID") == vid) {
                chk_vid = true;
                numDoors = res->getInt("NumDoors");
                if (res->getInt("ValidMOT") == 1)
                    exMOT = true;
            }
        }
            
        if (!(chk_vid)) {
            QMessageBox::warning(this, "Invalid Vehicle", "The VehicleID you have entered does not belong to a registered vehicle.");
            on_homeButton_clicked();
            return;
        }
        
        if (!(exMOT)) {
            QMessageBox::warning(this, "No MOT", "This vehicle does not have a valid MOT, so is not roadworthy.");
            on_homeButton_clicked();
            return;
        }
        
        bpf.setModal(true);
        bpf.exec();
        
        if (!(bpf.validbp)) {
            QMessageBox::critical(this, "Identity not verified", "Your identity could not be verified. An alert has been sent to the authorities.");
            offence = "Attempt Failed";
            create_alert();
            exit(0);
        }
        
    sqlstream.str("");
    sqlstream << "SELECT LicenceType, Year(LicenceExpDate), Month(LicenceExpDate), Day(LicenceExpDate) FROM Driver WHERE DriverID = '" << driverid << "'";
    sqlquery = sqlstream.str();
    res = stmt->executeQuery(sqlquery);
    bool can_drive = false;
    while (res->next()) {
        if (res->getString("LicenceType") != "None") {
            can_drive = true;
            lictype = res->getString("LicenceType");
            year = res->getInt("Year(LicenceExpDate)");
            month = res->getInt("Month(LicenceExpDate)");
            day = res->getInt("Day(LicenceExpDate)");
        }
    }
    
    if (!(can_drive)) {
        QMessageBox::critical(this, "No Licence", "You do not have a driving licence so you cannot drive this vehicle. An alert has been sent to the authorities.");
        offence = "No Licence";
        create_alert();
        return;
    } 
    if (!(regf.valid_exp_date(day, month, year))) {
        QMessageBox::critical(this, "Expired Licence", "Your driving licence is expired so you cannot drive this vehicle. An alert has been sent to the authorities.");
        offence = "Expired Licence";
        create_alert();
        return;
    }
    
    firstdriverid = driverid;
    
    if (lictype == "Provisional")
        sd_check();
    
    sqlstream.str("");
    sqlstream << "SELECT DriverID, Year(ExpiryDate), Month(ExpiryDate), Day(ExpiryDate) FROM InsurancePolicy WHERE VehicleID = '" << vid << "'";
    sqlquery = sqlstream.str();
    
    res = stmt->executeQuery(sqlquery);
    bool ip_exists = false;
    while (res->next()) {
        if (res->getInt("DriverID") == firstdriverid) {
            ip_exists = true;    
            year = res->getInt("Year(ExpiryDate)");
            month = res->getInt("Month(ExpiryDate)");
            day = res->getInt("Day(ExpiryDate)");
        }
    }
    
    if (!(ip_exists) || !(regf.valid_exp_date(day, month, year))) {
       QMessageBox::critical(this, "No Insurance", "You are not insured on this vehicle so you cannot drive this vehicle. An alert has been sent to the authorities.");
        offence = "No Insurance";
        create_alert(); 
        return;
    }
    
    if (chk_safety()){
        QMessageBox::information(this, "Engine Started", "Success! Engine Started!");
        on_homeButton_clicked();
    }
    
        
       /* output any SQL error*/
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void Ve_sim::create_alert() {
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the MySQL database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        if (offence == "Attempt Failed") {
            sqlstream.str("");
            sqlstream << "INSERT INTO Alert (VehicleID, NCoordinate, ECoordinate, OffenceClassification) VALUES ('" << vid << "','" << get_long() <<
                        "','" << get_lat() << "','" << offence << "')";
          
        } else if (offence == "Solo Provisional Driver"){
            sqlstream.str("");
            sqlstream << "INSERT INTO Alert (VehicleID, DriverID, NCoordinate, ECoordinate, OffenceClassification) VALUES ('" << vid << "','" << firstdriverid <<
                    "','" << get_long() << "','" << get_lat() << "','" << offence << "')";
        } else {
            sqlstream.str("");
            sqlstream << "INSERT INTO Alert (VehicleID, DriverID, NCoordinate, ECoordinate, OffenceClassification) VALUES ('" << vid << "','" << driverid <<
                    "','" << get_long() << "','" << get_lat() << "','" << offence << "')";
        }
        
        sqlquery = sqlstream.str();
        stmt->execute(sqlquery);
        
           /* output any SQL error*/
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void Ve_sim::sd_check() {
    int year;
    int month;
    int day;
    int ffl;
    int dobday;
    int dobmon;
    int dobyear;
    backPassForm bpf;
    RegForm regf;
    
    bpf.setModal(true);
    bpf.exec();

    if (!(bpf.validbp)) {
        QMessageBox::critical(this, "Solo Provisional Driver", "A Full licence holder insured on this vehicle could not be verified. It is illegal for provisional licence holders to drive on their own.\nAn alert has been sent to the authorities.");
        offence = "Solo Provisional Driver";
        create_alert();
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

        /* Connect to the MySQL database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");    
    
        sqlstream.str("");
        sqlstream << "SELECT LicenceType, FirstFullLicence, Year(DateofBirth), Month(DateofBirth), Day(DateofBirth), Year(LicenceExpDate), Month(LicenceExpDate), Day(LicenceExpDate) FROM Driver WHERE DriverID = '" << driverid << "'";
        sqlquery = sqlstream.str();
        res = stmt->executeQuery(sqlquery);
        bool can_drive = false;
        while (res->next()) {
            if (res->getString("LicenceType") == "Full") {
                can_drive = true;
                lictype = res->getString("LicenceType");
                year = res->getInt("Year(LicenceExpDate)");
                month = res->getInt("Month(LicenceExpDate)");
                day = res->getInt("Day(LicenceExpDate)");
                ffl = res->getInt("FirstFullLicence");
                dobyear = res->getInt("Year(DateofBirth)");
                dobmon = res->getInt("Month(DateofBirth)");
                dobday = res->getInt("Day(DateofBirth)");
            }
        }
        if (!(can_drive)) {
            QMessageBox::critical(this, "Solo Provisional Driver", "A Full licence holder insured on this vehicle could not be verified. It is illegal for provisional licence holders to drive on their own.\nAn alert has been sent to the authorities.");
            offence = "Solo Provisional Driver";
            create_alert();
            return;
        }
        
        if(!(regf.test_age(dobday, dobmon, dobyear, 21))) {
            QMessageBox::critical(this, "Solo Provisional Driver", "A Full licence holder insured on this vehicle could not be verified. It is illegal for provisional licence holders to drive on their own.\nAn alert has been sent to the authorities.");
            offence = "Solo Provisional Driver";
            create_alert();
            return;
        }


        if ((!(calculate_length(day, month, year))) && (ffl == 1)) {
            QMessageBox::critical(this, "Solo Provisional Driver", "A Full licence holder insured on this vehicle could not be verified. It is illegal for provisional licence holders to drive on their own.\nAn alert has been sent to the authorities.");
            offence = "Solo Provisional Driver";
            create_alert();
            return;
        }
        
        sqlstream.str("");
        sqlstream << "SELECT DriverID, Year(ExpiryDate), Month(ExpiryDate), Day(ExpiryDate) FROM InsurancePolicy WHERE VehicleID = '" << vid << "'";
        sqlquery = sqlstream.str();

        res = stmt->executeQuery(sqlquery);
        bool ip_exists = false;
        while (res->next()) {
            if (res->getInt("DriverID") == driverid) {
                ip_exists = true;    
                year = res->getInt("Year(ExpiryDate)");
                month = res->getInt("Month(ExpiryDate)");
                day = res->getInt("Day(ExpiryDate)");
            }
        }
    
        if (!(ip_exists) || !(regf.valid_exp_date(day, month, year))) {
            QMessageBox::critical(this, "Solo Provisional Driver", "A Full licence holder insured on this vehicle could not be verified. It is illegal for provisional licence holders to drive on their own.\nAn alert has been sent to the authorities.");
            offence = "Solo Provisional Driver";
            create_alert();
            return;
        }

            
       /* output any SQL error*/
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
}

bool Ve_sim::calculate_length(int day, int mon, int year) {
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

bool Ve_sim::chk_safety() {
    if (!all_belts(seatbelts)) {
        QMessageBox::warning(this, "Not enough seatbelts", "There are not enough seatbelts being worn. Please ensure all passengers are wearing a seatbelt.");
        return false;
    }
    
    if (!all_doors(doors)) {
        QMessageBox::warning(this, "Doors open", "There are doors open. Please ensure all doors are closed.");
        return false;
    }
    return true;
    
}

void Ve_sim::on_frontLeftDoor_clicked() {
    if (doors[0] == 0)
        doors[0] = 1;
    else
        doors[0] = 0;
    
}

void Ve_sim::on_frontRightDoor_clicked() {
    if (doors[1] == 0)
        doors[1] = 1;
    else
        doors[1] = 0;
}

void Ve_sim::on_backLeftDoor_clicked() {
    if (doors[2] == 0)
        doors[2] = 1;
    else
        doors[2] = 0;
}

void Ve_sim::on_backRightDoor_clicked() {
    if (doors[3] == 0)
        doors[3] = 1;
    else
        doors[3] = 0;
}

void Ve_sim::on_passenger1_clicked() {
   if (seatbelts[0] == 0)
       seatbelts[0] = 1;
   else
       seatbelts[0] = 0;
}

void Ve_sim::on_passenger2_clicked() {
   if (seatbelts[1] == 0)
       seatbelts[1] = 1;
   else
       seatbelts[1] = 0;
}

void Ve_sim::on_passenger3_clicked() {
   if (seatbelts[2] == 0)
       seatbelts[2] = 1;
   else
       seatbelts[2] = 0;
}

void Ve_sim::on_passenger4_clicked() {
   if (seatbelts[3] == 0)
       seatbelts[3] = 1;
   else
       seatbelts[3] = 0;
}

void Ve_sim::on_passenger5_clicked() {
   if (seatbelts[4] == 0)
       seatbelts[4] = 1;
   else
       seatbelts[4] = 0;
}

bool Ve_sim::all_belts(int in_belts[]) {
    int count = 0;
    for (int i=0; i < 5; i++){
        if (in_belts[i] == 1)
            count++;
    }
    if (count >= widget.spinBox->value())
        return true;
    else
        return false;
}

bool Ve_sim::all_doors(int in_doors[]) {
    int count = 0;
    for (int i=0; i < 4; i++){
        if (in_doors[i] == 1)
            count++;
    }
    if (count >= numDoors)
        return true;
    else
        return false;
}