/*Administrator Application Alert Log*/

/*Project file headers*/
#include "alertLog.h"
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

int alertid;
int responded;
ostringstream alsqlstream;
string alsqlquery;

alertLog::alertLog() {
    widget.setupUi(this);
    widget.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    widget.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    widget.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fillTable();
}

alertLog::~alertLog() {
}

void alertLog::fillTable() {
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
        res = stmt->executeQuery("SELECT * FROM Alert");
        rows = static_cast<int>(res->rowsCount());
        if (rows == 0) {
          QMessageBox::information(this, "No Alerts", "There are currently no alerts.");
          //on_homeButton_clicked();
        }
        widget.tableWidget->setRowCount(rows);
        
        /*Filling the table*/
        int i = 0;
        int driID [rows];
        QTableWidgetItem *item;
        res = stmt->executeQuery("SELECT * FROM Alert WHERE Responded = 0 ORDER BY TimeSubmitted DESC");
        while (res->next()) {
            driID[i] = res->getInt("DriverID");
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("AlertID")));
            widget.tableWidget->setItem(i, 0, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("VehicleID")));
            widget.tableWidget->setItem(i, 1, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("NCoordinate").c_str()));
            widget.tableWidget->setItem(i, 3, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("ECoordinate").c_str()));
            widget.tableWidget->setItem(i, 4, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("TimeSubmitted").c_str()));
            widget.tableWidget->setItem(i, 5, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("OffenceClassification").c_str()));
            widget.tableWidget->setItem(i, 6, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("Responded")));
            widget.tableWidget->setItem(i, 7, item);
            i++;
        }
        
        res = stmt->executeQuery("SELECT * FROM Alert WHERE Responded = 1 ORDER BY TimeSubmitted DESC");
        while (res->next()) {
            driID[i] = res->getInt("DriverID");
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("AlertID")));
            widget.tableWidget->setItem(i, 0, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("VehicleID")));
            widget.tableWidget->setItem(i, 1, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("NCoordinate").c_str()));
            widget.tableWidget->setItem(i, 3, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("ECoordinate").c_str()));
            widget.tableWidget->setItem(i, 4, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("TimeSubmitted").c_str()));
            widget.tableWidget->setItem(i, 5, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("OffenceClassification").c_str()));
            widget.tableWidget->setItem(i, 6, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("Responded")));
            widget.tableWidget->setItem(i, 7, item);
            i++;
        }
        
        for (i = 0; i < rows+1; i++) {
            alsqlstream.str("");
            alsqlstream << "SELECT Alias FROM Driver WHERE DriverID = '" << driID[i] << "'";
            alsqlquery = alsqlstream.str();
            res = stmt->executeQuery(alsqlquery);
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

void alertLog::on_tableWidget_cellClicked() {
  alertid = widget.tableWidget->item(widget.tableWidget->currentRow(), 0)->text().toInt();
  responded = widget.tableWidget->item(widget.tableWidget->currentRow(), 7)->text().toInt();
}

void alertLog::on_modButton_clicked() {
    if (getLoa() == 0) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be at least a level 1 administrator to respond to an alert.");
        return;
    }
    
    if (alertid == 0) {
        QMessageBox::warning(this, "No Alert Selected", "You have not selected an alert to respond to.");
        return;
    }
    
    if (responded == 1) {
        QMessageBox::warning(this, "Already responded", "This alert has already been responded to.");
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
        
        alsqlstream.str("");
        alsqlstream << "UPDATE Alert SET Responded = '1' WHERE AlertID = '" << alertid << "'";
        alsqlquery = alsqlstream.str();
        stmt->execute(alsqlquery);
        
        QMessageBox::information(this, "Alert Responded To", "You have responded to this alert.");
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

void alertLog::on_delButton_clicked() {
    if (getLoa() != 2) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be a level 2 administrator to delete an alert.");
        return;
    }
    
    if (alertid == 0) {
        QMessageBox::warning(this, "No Alert Selected", "You have not selected an alert to delete.");
        return;
    }
    
    if (responded == 0) {
        QMessageBox::warning(this, "Not responded", "This alert has not been responded to. Until a response is sent, the alert cannot be deleted.");
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
        
        alsqlstream.str("");
        alsqlstream << "DELETE FROM Alert WHERE AlertID = '" << alertid << "'";
        alsqlquery = alsqlstream.str();
        stmt->execute(alsqlquery);
        
        QMessageBox::information(this, "Alert Deleted", "You have deleted this alert from the system.");
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

void alertLog::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}