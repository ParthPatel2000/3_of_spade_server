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
    // Execute the query, and check if it executed successfully
    // if error occurs, print the error message
    if (mysql_query(connection, query) != 0)
    {
        std::cerr << "Error: " << mysql_error(connection) << std::endl;
        return false;
    }
    // return true if the query executed successfully
    return true;
}

MYSQL_RES *MySQLConnector::result()
{
    MYSQL_RES *result = mysql_store_result(connection);
    if (result == NULL)
    {
        std::cerr << "Error: " << mysql_error(connection) << std::endl;
    }
    return result;
}

void MySQLConnector::freeResult(MYSQL_RES *result)
{
    mysql_free_result(result);
}


// database setup functions
bool MySQLConnector::checkTable(const char *table)
{
    bool ifTableExists = false;
    std::string query = "SHOW TABLES LIKE '" + inputSanitizer(table) + "'";
    if (executeQuery(query.c_str()))
    {
        MYSQL_RES *res = mysql_store_result(connection);
        if (res == NULL)
        {
            std::cerr << "Error: "
                      << "Table check failed" << std::endl;
            return false;
        }
        if (mysql_num_rows(res) == 0)
        {
            std::cout << "Table does not exist" << std::endl;
            mysql_free_result(res);
            return false;
        }
        std::cout << "Table exists" << std::endl;
        ifTableExists = true;
        mysql_free_result(res);
    }

    return ifTableExists;
}
/**
 * @brief Creates a table in the database with the given name and columns.
 *        The columns are passed as a vector of pairs, where each pair contains
 *        the column name (first element) and the column type with constraints (second element).
 *
 * @param table_name The name of the table to be created.
 * @param columns A vector of pairs representing the columns of the table.
 *                Each pair contains the column name and its type with constraints.
 * @return true if the table creation was successful, false otherwise.
 */
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

