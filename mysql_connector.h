#ifndef MYSQL_CONNECTOR_H
#define MYSQL_CONNECTOR_H

#include <mysql/mysql.h> // Include the mysql.h header file from AWS
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <openssl/sha.h>
#include <chrono>

class MySQLConnector
{
private:
    MYSQL *connection;
    // These are the default values for the local mysql server, change these to the aws mysql server values.
    std::string host = "localhost";         // Change this when on aws
    std::string user = "root";              // Change this to aws mysql username
    int port = 3306;                        // Change this to aws mysql port
    std::string password = "8238";          // Change this to aws mysql password
    std::string database = "user_accounts"; // Change this to aws mysql database name


public:
    MySQLConnector();                                                                               // default constructor
    MySQLConnector(const char *host, const char *user, const char *password, const char *database); // parameterized constructor
    ~MySQLConnector();                                                                              // destructor
    // basic utility functions
    std::string inputSanitizer(const std::string &input);                                         // sanitize the input(escape special characters, no sql injection)
    bool connect(const char *host, const char *user, const char *password, const char *database); // connect to the mysql server
    void disconnect();                                                                            // disconnect from the mysql server
    bool executeQuery(const char *query);                                                         // execute a query on the mysql server
    MYSQL_RES *result();                                                                    // return the result of a query
    void freeResult(MYSQL_RES *result);                                                           // free the result of a query

    // database setup functions
    bool createTable(const char *table_name, std::vector<std::pair<std::string, std::string>> columns); // create all the tables
    bool checkTable(const char *table);                                                                 // check if a table exists
    bool createUsersTable();                                                                            // create the users table

};

#endif // MYSQL_CONNECTOR_H
