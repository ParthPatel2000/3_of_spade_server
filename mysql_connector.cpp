#include "mysql_connector.h"

MySQLConnector::MySQLConnector()
{
    connection = mysql_init(NULL);
    connect(host.c_str(), user.c_str(), password.c_str(), database.c_str());
}

MySQLConnector::MySQLConnector(const char *host, const char *user, const char *password, const char *database)
{
    connection = mysql_init(NULL);
    connect(host, user, password, database);
}

MySQLConnector::~MySQLConnector()
{
    mysql_close(connection);
}

// basic utility functions
bool MySQLConnector::connect(const char *host, const char *user, const char *password, const char *database)
{
    if (mysql_real_connect(connection, host, user, password, database, 0, NULL, 0) == NULL)
    {
        std::cerr << "Error: " << mysql_error(connection) << std::endl;
        return false;
    }
    return true;
}

void MySQLConnector::disconnect()
{
    mysql_close(connection);
}

std::string MySQLConnector::inputSanitizer(const std::string &input)
{
    std::string sanitized_input = input;
    // Sanitize the input here...
    return sanitized_input;
}

bool MySQLConnector::executeQuery(const char *query)
{
    if (mysql_query(connection, query) != 0)
    {
        std::cerr << "Error: " << mysql_error(connection) << std::endl;
        return false;
    }
    return true;
}

// user account functions
bool MySQLConnector::registerUser(const std::string &username, const std::string &password, const std::string &email)
{
    std::string query = "INSERT INTO users (username, password, email) VALUES ('" + inputSanitizer(username) + "', '" + inputSanitizer(password) + "', '" + inputSanitizer(email) + "')";
    return executeQuery(query.c_str());
}

bool MySQLConnector::checkUsername(const std::string &username)
{
    std::string query = "SELECT * FROM users WHERE username = '" + inputSanitizer(username) + "'";
    return executeQuery(query.c_str());
}

bool MySQLConnector::authenticateUser(const std::string &username, const std::string &password)
{
    std::string query = "SELECT * FROM users WHERE username = '" + inputSanitizer(username) + "' AND password = '" + inputSanitizer(password) + "'";
    return executeQuery(query.c_str());
}

std::string MySQLConnector::generateSessionToken(const std::string &username)
{
    std::string query = "SELECT * FROM users WHERE username = '" + inputSanitizer(username) + "'";
    if (executeQuery(query.c_str()))
    {
        MYSQL_RES *result = mysql_store_result(connection);
        MYSQL_ROW row = mysql_fetch_row(result);
        std::string session_token = row[0];
        mysql_free_result(result);
        return session_token;
    }
    return "";
}

// database setup functions
bool MySQLConnector::createDatabase(const char *database)
{
    if (database == NULL)
    {
        std::cerr << "Error: "
                  << "Database name cannot be NULL" << std::endl;
        return false;
    }

    if (database == "")
    {
        std::cerr << "Error: "
                  << "Database name cannot be empty" << std::endl;
        return false;
    }

    if (database == "mysql" || database == "information_schema" || database == "performance_schema" || database == "sys")
    {
        std::cerr << "Error: "
                  << "Database name cannot be a system database" << std::endl;
        return false;
    }

    // The query to create a new database, with the given name
    std::string query = "CREATE DATABASE " + inputSanitizer(database) + ";";
    if (executeQuery(query.c_str()) == false)
    {
        std::cerr << "Error: "
                  << "Database creation failed" << std::endl;
        return false;
    }
    return true;
}

bool MySQLConnector::checkDatabase(const char *database)
{
    if (database == NULL)
    {
        std::cerr << "Error: "
                  << "Database name cannot be NULL" << std::endl;
        return false;
    }

    if (database == "")
    {
        std::cerr << "Error: "
                  << "Database name cannot be empty" << std::endl;
        return false;
    }

    if (database == "mysql" || database == "information_schema" || database == "performance_schema" || database == "sys")
    {
        std::cerr << "Error: "
                  << "Database name cannot be a system database" << std::endl;
        return false;
    }

    // The query to check if a database exists
    std::string query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '" + inputSanitizer(database) + "';";
    if (executeQuery(query.c_str()) == false)
    {
        std::cerr << "Error: "
                  << "Database Doesn't exist, Creating Database..." << std::endl;
        return false;
    }
    return true;
}