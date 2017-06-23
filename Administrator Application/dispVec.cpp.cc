/*Administrator Application Display Vehicles Form*/

/*Project file headers*/
#include "modVec.h"
#include "dispVec.h"
#include "MainMenu.h"
#include "loaGetSet.h"

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

ostringstream vecsqlstream;
string vecsqlquery;
string mdmake;
string mdmodel;
string mdown_st;
int mdvid;
int mdyor;
int mdmot;
int mdown;

dispVec::dispVec() {
    widget.setupUi(this);
    widget.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    widget.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    widget.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fillTable();
}

dispVec::~dispVec() {
}

void dispVec::fillTable() {
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the MySQL test database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        /*Determining the number of rows*/
        int rows;
        res = stmt->executeQuery("SELECT * FROM Vehicle");
        rows = static_cast<int>(res->rowsCount());
        if (rows == 0)
          QMessageBox::information(this, "No Vehicles", "There are no vehicles currently registered on the system.");
    
        widget.tableWidget->setRowCount(rows);
        
        /*Filling the table*/
        int i = 0;
        int driID [rows];
        QTableWidgetItem *item;
        res = stmt->executeQuery("SELECT * FROM Vehicle");
        while (res->next()) {
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("VehicleID")));
            widget.tableWidget->setItem(i, 0, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("Make").c_str()));
            widget.tableWidget->setItem(i, 1, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("Model").c_str()));
            widget.tableWidget->setItem(i, 2, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("YearOfReg")));
            widget.tableWidget->setItem(i, 3, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("ValidMOT")));
            widget.tableWidget->setItem(i, 4, item);
            driID[i] = res->getInt("Owner");
            i++;
        }
        
        for (i = 0; i < rows+1; i++) {
            vecsqlstream.str("");
            vecsqlstream << "SELECT Alias FROM Driver WHERE DriverID = '" << driID[i] << "'";
            vecsqlquery = vecsqlstream.str();
            res = stmt->executeQuery(vecsqlquery);
            while (res->next()) {
                item = new QTableWidgetItem(QString("%1").arg(res->getString("Alias").c_str()));
                widget.tableWidget->setItem(i, 5, item); 
            }
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
}

void dispVec::on_tableWidget_cellClicked() {
    mdvid = widget.tableWidget->item(widget.tableWidget->currentRow(), 0)->text().toInt();
    mdmake = widget.tableWidget->item(widget.tableWidget->currentRow(), 1)->text().toStdString();
    mdmodel = widget.tableWidget->item(widget.tableWidget->currentRow(), 2)->text().toStdString();
    mdyor = widget.tableWidget->item(widget.tableWidget->currentRow(), 3)->text().toInt();
    mdmot = widget.tableWidget->item(widget.tableWidget->currentRow(), 4)->text().toInt();
    mdown_st = widget.tableWidget->item(widget.tableWidget->currentRow(), 5)->text().toStdString();

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the MySQL test database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        vecsqlstream.str("");
        vecsqlstream << "SELECT DriverID FROM Driver WHERE Alias = '" << mdown_st << "'";
        vecsqlquery = vecsqlstream.str();
        res = stmt->executeQuery(vecsqlquery);
        while (res->next()) {
            mdown = res->getInt("DriverID") ;
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
}



void dispVec::on_modButton_clicked() {
    if (getLoa() == 0) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be at least a level 1 administrator to modify a vehicle.");
        return;
    }
    
    if (mdvid == 0) {
        QMessageBox::warning(this, "No Vehicle Selected", "You have not selected a vehicle to modify.");
        return;
    }
    
    modVec modv;
    modv.vec_tbe(mdvid);
    modv.setModal(true);
    this->hide();
    modv.exec();
}

void dispVec::on_delButton_clicked() {
    if (getLoa() != 2) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be a level 2 administrator to delete a vehicle.");
        return;
    }
    
    if (mdvid == 0) {
        QMessageBox::warning(this, "No Vehicle Selected", "You have not selected a vehicle to delete.");
        return;
    }
    
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        vecsqlstream.str("");
        vecsqlstream << "DELETE FROM CompPerms WHERE VehicleID = '" << mdvid << "'";
        vecsqlquery = vecsqlstream.str();
        stmt->execute(vecsqlquery);
        
        vecsqlstream.str("");
        vecsqlstream << "DELETE FROM InsurancePolicy WHERE VehicleID = '" << mdvid << "'";
        vecsqlquery = vecsqlstream.str();
        stmt->execute(vecsqlquery);
        
        vecsqlstream.str("");
        vecsqlstream << "DELETE FROM Alert WHERE VehicleID = '" << mdvid << "'";
        vecsqlquery = vecsqlstream.str();
        stmt->execute(vecsqlquery);
        
        vecsqlstream.str("");
        vecsqlstream << "DELETE FROM Vehicle WHERE VehicleID = '" << mdvid << "'";
        vecsqlquery = vecsqlstream.str();
        stmt->execute(vecsqlquery);
        
        QMessageBox::information(this, "Vehicle Deleted", "You have deleted this vehicle from the system.");
        fillTable();
        
        delete stmt;
        delete con;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    } 
}

void dispVec::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}