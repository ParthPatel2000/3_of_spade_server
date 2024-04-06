#include "Game.h"
#include "user_account.h"
#include "mysql_connector.h"

using namespace std;

//////////////////// Functioning  /////////////////////
/*
We are first generating a deck of cards that are according to the lobby size
then we shuffle this cards deck n number of times where n is passed from the main as of now
we then split this shuffled deck into equal parts for the players in the lobby
and SORT these cards before assigning them to the player objects

after this inital phase we take the bids from the player and let the winner choose the partner
from a deck which has the bidder's cards excluded and this is how the partners are assigned.

*/

////////////////////////////////////////////////////////////////

int main()
{

    UserAccount *connector = new UserAccount();

    // connector->createUsersTable();
    // connector->fillUsersTable();
    connector->updateAuthToken("");
    connector->authenticateUser("1", "2");
    // Game *game = new Game(8);

    // // Printing the players cards
    // game->print_player_objects(0);
    // game->print_player_objects(1);
    // game->print_player_objects(2);
    // game->print_player_objects(3);
    // game->print_player_objects(4);
    // game->print_player_objects(5);
    // game->print_player_objects(6);
    // game->print_player_objects(7);

    // game->print_deck();

    // print_inputs();
    // game->game_main_routine();
    return 0;
}
