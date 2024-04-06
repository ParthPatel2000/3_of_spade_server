#ifndef USER_ACCOUNT_H
#define USER_ACCOUNT_H

#include "mysql_connector.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp>

class UserAccount
{
private:
    std::string host = "192.168.0.100";
    std::string user = "debian";
    std::string password = "8238";
    std::string database = "user_accounts";
    int port = 3306;
    int game_id_length = 32;    // Change this to the desired length of the Game token
    int auth_token_length = 64; // Change this to the desired length of the auth token
    MySQLConnector *database_connector;

public:
    UserAccount();
    ~UserAccount();

    // user account functions
    bool registerUser(const std::string &username, const std::string &password, const std::string &email); // register a new user
    bool checkUsername(const std::string &username);                                                       // check if the username already exists
    bool authenticateUser(const std::string &username, const std::string &password);                       // authenticate a user
    std::string updateAuthToken(const std::string &username);                                              // generate and update auth token for a user
    std::string updateGameID(const std::string &username, const std::string &gameID);                                                 // generate and update Game token for a user
    std::string generateRandomToken(const std::string &input, int length);                                 // generate a random token of a given length
    std::string getGameIDfromUsername(const std::string &username);                                        // get the Game token of a user

    // setup functions
    void createUsersTable(); // create the users table
    void createGamesTable(); // create the Games table

    //api functions
    bool user_login(); // login a user

    // debug functions
    void fillUsersTable(); // fill the users table with dummy data
};

#endif
