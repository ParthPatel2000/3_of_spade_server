#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <unordered_map>

using namespace std;

// This is the main class that will be used to play the game.
class Game
{
    // structs
public:
    struct card
    {
        int value; // range 1-13  1- Ace, 2-10, 11- Jack, 12- Queen, 13- King.
        int color; // Color of the card 1- Sapdes, 2- Clubs, 3- Hearts, 4- Diamonds.
        card(int value, int color) : value(value), color(color) {}
    };

    struct player
    {
        int player_id;        // this will start with 0
        string auth_token;    // this will come from the main function, based on the player who is playing.
        string username;      // this will come from the main function, based on the player who is playing.
        vector<card *> cards; // holds a vector of cards that will be presorted.
        int team;             // the team of the player 1 or 2, 2 is default, so we can just assign 1 to the players who are in team 1.
        int scored;           // this can be used in future to keep track of the score of individual players and declare mvp.
        int current_input;    // index of the selection from player cards, for easier popping and use.
        player() : player_id(0), team(2), scored(0), current_input(0) {}
    };

    // Variables
private:
    unordered_map<string, int>
        player_map[8]; // This will be helpful to know which player is playing with which id
                       // and in case of reconnection we can use this to get the player id.

    player *players;             // refernece to the players array that will be initialized by the constructor.
    vector<card *> deck;         // reference to the deck that will be initialized by the constructor.
    int lobby_size = 4;          // here lobby size is 4 but it is to make the code scalable i've left this one as a global constant
    int deck_size = 52;          // 52 for 4 player, 102 for 6 and 152 for 8 respectively
    int no_of_player_cards = 13; // the no of cards the player will get it will be different for 6 and 8 players.
    int max_target = 0;          // the max target will be set by the constructor based on the lobby size
    int min_target = 0;          // the min target will be set by the constructor based on the lobby size
    int team1_target = 0;        // the target that will be set by the highest bidder
    int team2_target = 0;        // the target that will be the max_target - team1_target
    int team1_scored = 0;        // the total points scored by team 1
    int team2_scored = 0;        // the total points scored by team 2
    int trump_color = 0;         // the color that will be set by the highest bidder
    int shuffle_count = 10;      // the no of times the deck will be shuffled
    int start_hand_from = 0;     // This is to know from which player to start taking inputs from in the next round, Initial value Highest bidder id.
    int highest_bidder_id = 0;   // Id of the highest bidder
    card *partner_card;          // the card that will be selected by the highest bidder to select the partner

    // Functions
public:
    // important game functions
    Game(int lobby_size);
    ~Game();
    void fill_lobby(int lobby_size); // fill the player_username_auth_map with the players in the lobby

    void bind_players_to_game(); // this will bind the player to the game and set the auth token and username
    void bindPlayerToStruct(string username, string auth_token, int player_id);
    void set_player_map(string username, int player_id);

    void sorting_the_cards(vector<card *>);
    void shufflecards();
    void deck_gen();
    void deck_adjuster_for_lobby();
    void distribute_cards();
    void player_inputs(int player_id);
    void bidding();
    void partner_select();
    void hand_logic();
    void team_builder();
    void game_main_routine();
    int search_cards_in_player(int);
    int card_points(card *);
    int player_has_color(int, int);
    void game_reset();

    // api functions
    void get_player_cards(int player_id);

    // debugging functions
    void print_inputs();
    void print_player_objects(int);
    void print_player_cards(int);
    void print_deck();
};
bool operator==(const Game::card &c1, const Game::card &c2);
bool operator<(const Game::card &c1, const Game::card &c2);
bool operator>(const Game::card &c1, const Game::card &c2);
#endif