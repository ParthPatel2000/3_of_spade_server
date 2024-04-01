#ifndef SERVER_H
#define SERVER_H

#include "Game.h"
#include "user_account.h"
#include <pthread.h>

class Server
{
private:
unordered_map<string, Game*> *player_game_map; //hashmap to store the games the players will be inside the game object.

private:
struct login_args
{
    string username;
    string password;
    string auth_token;
    int player_id;
    Game *game_instance;
};

void *login_player(void *args); // this will add the player to the game and update the map.
public:
Server();
~Server();
void createLobby(int lobby_size); // this will create a lobby and add the player to the game usig addPlayer.
void addPlayer(string player_id, string game_id); // this will add the player to the game and update the map.
void removePlayer(string player_id, string game_id); // this will remove the player from the game and update the map.
void startGame(string game_id); // this will start the game and assign the player their cards.


};
#endif // SERVER_H