#include "mysql_connector.h"

MySQLConnector::MySQLConnector()
{
    connection = mysql_init(NULL);
    connect(host.c_str(), user.c_str(), password.c_str(), database.c_str());
    if (checkTable("users") == false)
    {
        createUsersTable();
    }
}

MySQLConnector::MySQLConnector(const char *host, const char *user, const char *password, const char *database)
{
    connection = mysql_init(NULL);
    connect(host, user, password, database);
    if (checkTable("users") == false)
    {
        createUsersTable();
    }
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

    MYSQL_RES *result = mysql_store_result(connection);
    if (result == NULL)
    {
        return false;
    }
    mysql_free_result(result);
    return true;
}

// user account functions
bool MySQLConnector::registerUser(const std::string &username, const std::string &password, const std::string &email)
{
    std::string query = "INSERT INTO users (username, password_hash, email) VALUES ('" + inputSanitizer(username) + "', '" +
                        inputSanitizer(password) + "', '" +
                        inputSanitizer(email) + "')";
    return executeQuery(query.c_str());
}

bool MySQLConnector::checkUsername(const std::string &username)
{
    std::string query = "SELECT * FROM users WHERE username = '" + inputSanitizer(username) + "'";
    return executeQuery(query.c_str());
}

bool MySQLConnector::authenticateUser(const std::string &username, const std::string &password)
{
    std::string query = "SELECT * FROM users WHERE username = '" + inputSanitizer(username) + "' AND password_hash = '" + inputSanitizer(password) + "'";
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
bool MySQLConnector::checkTable(const char *table)
{
    std::string query = "SHOW TABLES LIKE '" + inputSanitizer(table) + "'";
    
    return executeQuery(query.c_str());
}

bool MySQLConnector::createTable(const char *table_name, std::vector<std::pair<std::string, std::string>> columns)
{
    std::string query = "CREATE TABLE IF NOT EXISTS " + inputSanitizer(table_name) + " (";
    for (int i = 0; i < columns.size(); i++)
    {
        query += columns[i].first + " " + columns[i].second;
        if (i != columns.size() - 1)
        {
            query += ", ";
        }
    }
    query += ");";
    if (executeQuery(query.c_str()) == false)
    {
        std::cerr << "Error: "
                  << "Table creation failed" << std::endl;
        return false;
    }
    std::cout << "Table created successfully" << std::endl;
    return true;
}

bool MySQLConnector::createUsersTable()
{
    std::vector<std::pair<std::string, std::string>> columns = {
        {"id", "INT AUTO_INCREMENT PRIMARY KEY"},
        {"username", "VARCHAR(100) NOT NULL UNIQUE"},
        {"password_hash", "VARCHAR(100) NOT NULL UNIQUE"},
        {"email", "VARCHAR(100) NOT NULL UNIQUE"},
        {"session_token", "VARCHAR(100) NOT NULL UNIQUE"},
        {"role", "ENUM('admin', 'user') DEFAULT 'user'"},
        {"status", "ENUM('active', 'inactive') DEFAULT 'active'"},
        {"created_at", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP"},
        {"updated_at", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"},
        {"deleted_at", "TIMESTAMP"}};
    return createTable("users", columns);
}

// debug database functions
void MySQLConnector::printTable(const char *table)
{
    std::string query = "SELECT * FROM " + inputSanitizer(table);
    if (executeQuery(query.c_str()))
    {
        MYSQL_RES *result = mysql_store_result(connection);
        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)))
        {
            for (int i = 0; i < num_fields; i++)
            {
                std::cout << row[i] << " ";
            }
            std::cout << std::endl;
        }
        mysql_free_result(result);
    }
    else
    {
        std::cerr << "Error: "
                  << "Table print failed" << std::endl;
    }
}


void MySQLConnector::printUsersTable()
{
    printTable("users");
}

void MySQLConnector::fillUsersTable()
{
    std::string query = "LOAD DATA LOCAL INFILE '~/Downloads/MOCK_DATA.csv' INTO TABLE users FIELDS TERMINATED BY ',' TERMINATED BY '\n' IGNORE 1 ROWS";
    if (executeQuery(query.c_str()) == false)
    {
        std::cerr << "Error: "
                  << "Table fill failed" << std::endl;
    }
}