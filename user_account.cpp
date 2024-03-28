#include "user_account.h"
#include "mysql_connector.h"

UserAccount::UserAccount()
{
    database_connector = new MySQLConnector(host.c_str(), user.c_str(), password.c_str(), database.c_str());

    //for debugging purposes only
    if(database_connector->checkTable("users") == false)
    {
        createUsersTable();
        fillUsersTable();
    }
}

UserAccount::~UserAccount()
{
    database_connector->disconnect();
    delete database_connector;
    // Destructor implementation...
}

// user account functions
bool UserAccount::registerUser(const std::string &username, const std::string &password, const std::string &email)
{
    std::string query = "INSERT INTO users (username, password_hash, email, auth_token) VALUES ('" + database_connector->inputSanitizer(username) + "', '" +
                        database_connector->inputSanitizer(password) + "', '" +
                        database_connector->inputSanitizer(email) + "'" + ", '" + generateRandomToken(username, auth_token_length) + "')";
    return database_connector->executeQuery(query.c_str());
}

bool UserAccount::checkUsername(const std::string &username)
{
    bool isUsernameAvailable = false;
    std::string query = "SELECT * FROM users WHERE username = '" + database_connector->inputSanitizer(username) + "'";
    if (database_connector->executeQuery(query.c_str()))
    {
        MYSQL_RES *res = database_connector->result();

        // if no rows are returned, username is available
        if (mysql_num_rows(res) == 0)
        {
            std::cout << "Username is available" << std::endl;
            isUsernameAvailable = true;
        }
        // if rows are returned, username is not available
        else
        {
            std::cerr << "Error: " << "Username is not available" << std::endl;
        }
        // free the result
        mysql_free_result(res);
    }
    return isUsernameAvailable;
}

bool UserAccount::authenticateUser(const std::string &username, const std::string &password)
{
    bool result = false;
    std::string query = "SELECT * FROM users WHERE username = '" + database_connector->inputSanitizer(username) + "' AND password_hash = '" + database_connector->inputSanitizer(password) + "'";
    if (database_connector->executeQuery(query.c_str()))
    {
        MYSQL_RES *res = database_connector->result();
        if (mysql_num_rows(res) == 0)
        {
            std::cerr << "Error: " << "Authentication failed" << std::endl;
        }
        else
        {
            std::cout << "Authenticated" << std::endl;
            result = true;
        }
        // free the result
        mysql_free_result(res);
    }
    return result;
}

std::string UserAccount::generateGameID(const std::string &username)
{
    std::string gameID = generateRandomToken(username, game_id_length);
    std::string query = "UPDATE users SET game_id = '" + database_connector->inputSanitizer(gameID) + "' WHERE username = '" + database_connector->inputSanitizer(username) + "'";
    if (database_connector->executeQuery(query.c_str()))
    {
        return gameID;
    }
    else
    {
        return "";
    }
}

std::string UserAccount::generateAuthToken(const std::string &username)
{
    std::string authToken = generateRandomToken(username, auth_token_length);
    std::string query = "UPDATE users SET auth_token = '" + database_connector->inputSanitizer(authToken) + "' WHERE username = '" + database_connector->inputSanitizer(username) + "'";
    if (database_connector->executeQuery(query.c_str()))
    {
        return authToken;
    }
    else
    {
        return "";
    }
}

std::string UserAccount::generateRandomToken(const std::string &input, int length)
{
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string token;
    for (int i = 0; i < length; ++i)
    {
        token += characters[dis(gen)];
    }
    // debug print
    // std::cout << token << std::endl;
    return token;
}



std::string UserAccount::getGameIDfromUsername(const std::string &username)
{
    std::string query = "SELECT Game_id FROM users WHERE username = '" + database_connector->inputSanitizer(username) + "'";
    if (database_connector->executeQuery(query.c_str()))
    {
        MYSQL_RES *res = database_connector->result();
        MYSQL_ROW row = mysql_fetch_row(res);
        std::string gameID = row[0];
        mysql_free_result(res);
        return gameID;
    }
    else
    {
        return "";
    }
}
//
void UserAccount::createUsersTable()
{
    std::vector<std::pair<std::string, std::string>> columns = {
        {"id", "INT AUTO_INCREMENT PRIMARY KEY"},
        {"username", "VARCHAR(100) NOT NULL UNIQUE"},
        {"password_hash", "VARCHAR(100) NOT NULL UNIQUE"},
        {"email", "VARCHAR(100) NOT NULL UNIQUE"},
        {"auth_token", "VARCHAR(100) NOT NULL UNIQUE"},
        {"role", "ENUM('admin', 'user') DEFAULT 'user'"},
        {"status", "ENUM('active', 'inactive') DEFAULT 'active'"},
        {"created_at", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP"},
        {"updated_at", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"},
        {"deleted_at", "TIMESTAMP"}};
    database_connector->createTable("users", columns);
    return;
}

void UserAccount::createGamesTable()
{
    std::vector<std::pair<std::string, std::string>> columns = {
        {"id", "INT AUTO_INCREMENT PRIMARY KEY"},
        {"user_id", "INT NOT NULL"},
        {"Game_id", "VARCHAR(100)"},
        {"created_at", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP"},
        {"updated_at", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"},
        {"deleted_at", "TIMESTAMP"}};
    database_connector->createTable("Games", columns);
    return;
}


void UserAccount::fillUsersTable()
{
    std::string csv_file = "MOCK_DATA.csv";
    std::ifstream file(csv_file);
    if (!file.is_open())
    {
        std::cerr << "Error: "
                  << "File open failed" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip the first line (header)
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string username, password, email;
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, email, ',');
        registerUser(username, password, email);
    }
}

void UserAccount::fillGamesTable()
{
    std::string csv_file = "game_id.csv";
    std::ifstream file(csv_file);
    if (!file.is_open())
    {
        std::cerr << "Error: "
                  << "File open failed" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip the first line (header)
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string username, password, email;
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, email, ',');
        registerUser(username, password, email);
    }
}