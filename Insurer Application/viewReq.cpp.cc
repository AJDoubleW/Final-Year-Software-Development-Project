/*Insurance app view requests form*/

/*Project file headers*/
#include "viewReq.h"
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

ostringstream sqlstream;
string sqlquery;
string cAlias;
string dAlias;
string cFrom;
string cUntil;
int dID;
int cID;
int vecID;

viewReq::viewReq() {
    widget.setupUi(this);
    widget.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    widget.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    widget.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fillTable();
}

viewReq::~viewReq() {
}

void viewReq::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}

void viewReq::fillTable() {
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
        res = stmt->executeQuery("SELECT * FROM CompPerms");
        rows = static_cast<int>(res->rowsCount());
        if (rows == 0) {
          QMessageBox::information(this, "No Pending Requests", "There are no pending requests.");
          on_homeButton_clicked();
        }
        widget.tableWidget->setRowCount(rows);
        
        /*Filling the table*/
        int i = 0;
        int driID [rows];
        int creID [rows];
        QTableWidgetItem *item;
        res = stmt->executeQuery("SELECT * FROM CompPerms ORDER BY CoverFrom ASC");
        while (res->next()) {
            driID[i] = res->getInt("DriverID");
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("VehicleID")));
            widget.tableWidget->setItem(i, 1, item);
            creID[i] = res->getInt("CreatorID");
            item = new QTableWidgetItem(QString("%1").arg(res->getString("CoverFrom").c_str()));
            widget.tableWidget->setItem(i, 3, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("CoverUntil").c_str()));
            widget.tableWidget->setItem(i, 4, item);
            i++;
        }
        
        for (i = 0; i < rows+1; i++) {
            sqlstream.str("");
            sqlstream << "SELECT Alias FROM Driver WHERE DriverID = '" << driID[i] << "'";
            sqlquery = sqlstream.str();
            res = stmt->executeQuery(sqlquery);
            while (res->next()) {
                item = new QTableWidgetItem(QString("%1").arg(res->getString("Alias").c_str()));
                widget.tableWidget->setItem(i, 0, item); 
            }
            sqlstream.str("");
            sqlstream << "SELECT Alias FROM Driver WHERE DriverID = '" << creID[i] << "'";
            sqlquery = sqlstream.str();
            res = stmt->executeQuery(sqlquery);
            while (res->next()) {
                item = new QTableWidgetItem(QString("%1").arg(res->getString("Alias").c_str()));
                widget.tableWidget->setItem(i, 2, item); 
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

void viewReq::on_tableWidget_cellClicked() {
    dAlias = widget.tableWidget->item(widget.tableWidget->currentRow(), 0)->text().toStdString();
    vecID = widget.tableWidget->item(widget.tableWidget->currentRow(), 1)->text().toInt();
    cAlias = widget.tableWidget->item(widget.tableWidget->currentRow(), 2)->text().toStdString();
    cFrom = widget.tableWidget->item(widget.tableWidget->currentRow(), 3)->text().toStdString();
    cUntil = widget.tableWidget->item(widget.tableWidget->currentRow(), 4)->text().toStdString();

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
        
        sqlstream.str("");
        sqlstream << "SELECT DriverID FROM Driver WHERE Alias = '" << dAlias << "'";
        sqlquery = sqlstream.str();
        res = stmt->executeQuery(sqlquery);
        while (res->next()) {
            dID = res->getInt("DriverID") ;
        }
        sqlstream.str("");
        sqlstream << "SELECT DriverID FROM Driver WHERE Alias = '" << cAlias << "'";
        sqlquery = sqlstream.str();
        res = stmt->executeQuery(sqlquery);
        while (res->next()) {
            cID = res->getInt("DriverID");
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

void viewReq::on_appButton_clicked() {
    if (cID == 0) {
        QMessageBox::warning(this, "No Request Selected", "You have not selected a request to approve.");
        return;
    }
    
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the MySQL test database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        sqlstream.str("");
        sqlstream << "DELETE FROM CompPerms WHERE CreatorID = '" << cID << "' AND VehicleID = '" << vecID <<
                    "' AND DriverID = '" << dID << "'AND CoverFrom = '" << cFrom << "'AND CoverUntil = '" << cUntil << "'";
        sqlquery = sqlstream.str();
        stmt->execute(sqlquery);
        
        sqlstream.str("");
        sqlstream << "INSERT INTO InsurancePolicy (DriverID,VehicleID,PolicyType,IssueDate,ExpiryDate) VALUES ('" << dID <<
                    "','" << vecID << "','Comprehensive','" << cFrom << "','" << cUntil << "')";
        sqlquery = sqlstream.str();
        stmt->execute(sqlquery);
        
        QMessageBox::information(this, "Request approved", QString("You have approved this request.\n%1 is now insured on this vehicle from %2 until %3").arg(dAlias.c_str()).arg(cFrom.c_str()).arg(cUntil.c_str()));
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

void viewReq::on_denyButton_clicked() {
     if (cID == 0) {
        QMessageBox::warning(this, "No Request Selected", "You have not selected a request to deny.");
        return;
    }
     
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the MySQL test database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        sqlstream.str("");
        sqlstream << "DELETE FROM CompPerms WHERE CreatorID = '" << cID << "' AND VehicleID = '" << vecID <<
                    "' AND DriverID = '" << dID << "'AND CoverFrom = '" << cFrom << "'AND CoverUntil = '" << cUntil << "'";
        sqlquery = sqlstream.str();
        stmt->execute(sqlquery);
        
        QMessageBox::information(this, "Request denied", "You have denied this request.");
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