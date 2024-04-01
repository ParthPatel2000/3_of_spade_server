#include "server.h"

Server::Server()
{
    player_game_map = new unordered_map<string, Game *>();
}

Server::~Server()
{
    delete player_game_map;
}

void Server::createLobby(int lobby_size)
{
    Game *game = new Game(lobby_size, "game1");        // create a new game, for now we are using only one game, later make it random.
    player_game_map->insert(make_pair("game1", game)); // for now we are using only one game
// add the players to the game
#pragma omp parallel for
    for (int i = 0; i < lobby_size; i++)
    {
        string player_id = "player" + to_string(i);
    }
}

void *Server::login_player(void *args)
{
    login_args *login = (login_args *)args;
    // add the player to the game
    
    login->game_instance->bindPlayerToStruct(login->username, login->auth_token, login->player_id);
}