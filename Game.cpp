#include "Game.h"

////////////////// The debugging functions ////////////////////

void Game::print_player_cards(int no)
{
    cout << "\nPlayer " << no << "'s cards\n";
    for (int i = 0; i < players[no].cards.size(); i++)
    {
        cout << i << "."
             << " color " << players[no].cards[i]->color << "  value " << players[no].cards[i]->value << "\n";
    }
}

void Game::print_deck() // For Troubleshooting and backend only remove it in production
{
    cout << "\nThe deck size " << deck.size() << "\n";
    // Print the deck
    for (int i = 0; i < deck.size(); i++)
    {
        cout << " color " << deck[i]->color << "  value " << deck[i]->value << "\n";
    }
}

void Game::print_inputs()
{
    for (int i = 0; i < lobby_size; i++)
    {
        cout << "\nPlayer " << i << "'s Input \t ";
        cout << " color " << players[i].cards[players[i].current_input]->color << "  value " << players[i].cards[players[i].current_input]->value << "\n";
    }
}

void Game::print_player_objects(int player_number)
{
    cout << "\n//////////////Player " << player_number << " 's data //////////////////" << endl;
    cout << "Player id: " << players[player_number].player_id << endl;
    cout << "Team: " << players[player_number].team << endl;
    cout << "Scored: " << players[player_number].scored << endl;
    cout << "Current Input: " << players[player_number].current_input << endl;
    print_player_cards(player_number);
    cout << "///////////////////////////////////////////////////////////////////" << endl;
}

///////////////////////////////////////////////////////////////

// Important Game Functions //

bool card_compare(const Game::card *c1, const Game::card *c2)
{
    if (c1->color != c2->color)
    {
        return c1->color < c2->color;
    }
    return c1->value < c2->value;
}

Game::Game(int lobby_size, string gameID)
{
    this->gameID = gameID; // set the game id
    this->lobby_size = lobby_size;
    deck_size = 0; // 52 for 4 player, 102 for 6 and 152 for 8 respectively
    players = new player[lobby_size];
    // minimum target for 4 players is 150, for 6 players( 2 decks) is 300 and for 8 players(3 decks) is 450
    switch (lobby_size)
    {
    case 4:
        deck_size = 52;
        no_of_player_cards = 13;
        min_target = 150;
        max_target = 250;
        break;
    case 6:
        deck_size = 104;
        no_of_player_cards = 17;
        min_target = 300;
        max_target = 500;
        break;
    case 8:
        deck_size = 156;
        no_of_player_cards = 19;
        min_target = 450;
        max_target = 750;
        break;
    }

    // generate the deck and it will call the deck_adjuster_for_lobby function to adjust the deck according to the lobby size
    deck_gen();

    // the no of cards the player will get it will be different for 6 and 8 players.
    no_of_player_cards = deck_size / lobby_size;

    shufflecards();
    distribute_cards();
}

/**
 * @brief bind the player to the struct and set the auth token and username,
 *        this is useful when the player is reconnected to the game, or we need to validate their inputs.
 *
 * @param username
 * @param auth_token
 * @param player_id
 */
void Game::bindPlayerToStruct(string username, string auth_token, int player_id)
{
    players[player_id].username = username;
    players[player_id].auth_token = auth_token;
}

/**
 * @brief add a hashmap entry with the username as the key and the player id as the value,
 *          so we can easily get the player id from the username.
 *
 * @param username
 * @param player_id
 */
void Game::set_player_map(string username, int player_id)
{
    username_playerID_map.insert(make_pair(username, player_id));
}

/**
 * @brief This function shuffles the deck n number of times
 *        where n is the shuffle_count
 *
 */
void Game::shufflecards()
{
    for (int i = 0; i < shuffle_count; i++)
    {
        time_t now = time(0);
        unsigned seed = now;
        shuffle(deck.begin(), deck.end(), default_random_engine(seed));
    }
}

/**
 * @brief starts from 0, Range can be 0-51: Single deck, 0-103: Double Deck, 0-155: Triple deck.
 *        range 1-13  1- Ace, 2-10, 11- Jack, 12- Queen, 13- King.
 *        Color of the card 0- Sapdes, 1- Clubs, 2- Hearts, 3- Diamonds.
 * @param size
 */
void Game::deck_gen()
{
    // int number;
    int color = 1;
    int value = 1;
    // int id = 0;
    for (int i = 1; i <= deck_size; i++)
    {
        // creating a card and pushing it into the deck
        card *temp = new card(value, color);
        deck.push_back(temp);
        value++;
        // id++;

        // changing the color after 13 cards
        if (i % 13 == 0)
        {
            // reset the value as well
            value = 1;
            // change the color after size/4 cards
            // hence making the n decks together with n*13 cards
            if (i % (deck_size / 4) == 0)
            {
                color++;
            }
        }
    }
    cout << "Deck Generated\n";
    // adjust the deck according to the lobby size, so each player gets the same number of
    // cards 13 for 4 players and 17 for 6 players and 19 for 8 players
    deck_adjuster_for_lobby();
}

/**
 * @brief This function adjusts the deck according to the lobby size
 *        It removes the cards values 2 and one from a black color and one from a red color
 *        so each player gets the same number of cards 17 for 6 players and 19 for 8 players
 */
void Game::deck_adjuster_for_lobby()
{
    int removed = 0;
    if (lobby_size == 6)
    {
        // remove the card with value 2 and one from a black color and one from a red color
        for (int i = 0; i < deck.size(); i++)
        {
            if (removed < 2 && deck[i]->value == 2 && deck[i]->color == 1)
            {
                deck.erase(deck.begin() + i);
            }
        }
        deck_size = 102;
        cout << "Deck Adjusted for 6 players\n";
    }
    // for 8 players and 3 decks the total cards are 156 but we need only 152
    else if (lobby_size == 8)
    {
        // remove the card with value 2 and one from a black color and one from a red color
        for (int i = 0; i < deck.size(); i++)
        {
            if (deck[i]->value == 2)
            {
                deck.erase(deck.begin() + i);
                if (removed == 4)
                {
                    break;
                }
            }
        }
        deck_size = 152;
        cout << "Deck Adjusted for 8 players\n";
    }
    return;
}

/**
 * @brief This function distributes the cards to the players
 *        It also sorts the cards of the players,
 *        and assigns the player id to the players
 *        then sorts the deck for later use.
 *
 */
void Game::distribute_cards()
{
    int player_number = 0;

    for (int i = 1; i <= deck_size; i++)
    {
        players[player_number].cards.push_back(deck[i - 1]);
        if (i % no_of_player_cards == 0)
        {
            // sorting_the_cards(players[player_number].cards);
            sort(players[player_number].cards.begin(), players[player_number].cards.end(), card_compare);
            players[player_number].player_id = player_number;
            player_number++;
            // cout << "Player " << player_number << " Cards Distributed\n";
        }
    }
    // sorting the cards of the deck to use it later.
    cout << "Cards Distributed\n";
    sort(deck.begin(), deck.end(), card_compare);
    // sorting_the_cards(deck);
}

/**
 * @brief This function checks if the player has the color of the hand
 *
 * @return int 0 if the player doesn't have the color of the hand, 1 if the player has the color of the hand
 *
 */
int Game::player_has_color(int player_id, int hand_color)
{
    int status = 0;
    for (int i = 0; i < players[player_id].cards.size(); i++)
    {
        if (players[player_id].cards[i]->color == hand_color)
        {
            status = 1;
        }
    }
    return status;
}

void Game::player_inputs(int player_id)
{
    int hand_color = -1;
    int selection = -1;
    if (player_id == start_hand_from) // As being the hand starter this player can select any color they wish.
    {
        print_player_cards(player_id);
        cin >> selection;
        while (selection > players[player_id].cards.size() - 1 || selection < 0)
        {
            cout << "Please enter a valid input: ";
            cin >> selection;
        }
        hand_color = players[player_id].cards[selection]->color;
    }
    else // Others have to adhere to the hand color.
    {
        cout << "Player " << player_id << " 's turn please select a card from the following\t\n";
        if (player_has_color(player_id, hand_color))
        {
            for (int i = 0; i < players[player_id].cards.size(); i++)
            {
                if (players[player_id].cards.at(i)->color == hand_color)
                {
                    cout << "Press " << i << "  color " << players[player_id].cards.at(i)->color << "  value " << players[player_id].cards.at(i)->value << "\n";
                }
            }
            cin >> selection;
        }
        else
        {
            for (int i = 0; i < players[player_id].cards.size(); i++)
            {
                if (players[player_id].cards.at(i)->color != hand_color)
                {
                    cout << "Press " << i << " id "
                         << "  color " << players[player_id].cards.at(i)->color << "  value " << players[player_id].cards.at(i)->value << "\n";
                }
            }
            cin >> selection;
        }
    }

    // assign the input to the player object
    players[player_id].current_input = selection;

    if (players[player_id].cards[selection] == partner_card)
    {
        cout << "Partner Revealed\n";
        cout << highest_bidder_id << "and" << player_id << " are partners\n";
    }
    return;
}

void Game::bidding() // implement the current highest bid option to not have 2 player bid the same amount!
{
    int highest_bid = 0;

    vector<player *> queue; // queue to keep the order of the players, so we can easily remove the player who retreats.
    for (int i = 0; i < lobby_size; i++)
    {
        queue.push_back(&players[i]);
    }

    int current_bidder = 0;
    while (queue.size() > 1 && highest_bid < max_target)
    {
        int bid_input = 0;
        cout << "Player " << queue.at(current_bidder)->player_id << "'s turn\n";
        cout << "enter your bid (should be greater than the current highest bid)" << highest_bid;
        cin >> bid_input;
        // if the bidder bids less than the current highest bid
        while (bid_input <= highest_bid && bid_input != -1)
        {
            cout << "Please enter a bid greater than the current highest bid\n";
            cout << "\n-1 for retreat \n";
            cin >> bid_input;
        }
        // if the bidder bids more than the max target
        while (bid_input > max_target && bid_input != -1)
        {
            cout << "Please enter a bid less than the max target\n";
            cout << "\n-1 for retreat \n";
            cin >> bid_input;
        }
        // if the bidder retreats
        if (bid_input == -1)
        {
            queue.erase(queue.begin() + current_bidder);
        }
        // if the bidder bids more than the current highest bid
        else
        {
            // replace the current highest bid with the new bid
            highest_bid = bid_input;
            // replace the current highest bidder with the new bidder
            highest_bidder_id = queue[current_bidder]->player_id;
            // goto to the next bidder
            current_bidder++;
        }
        // if the current bidder is the last one in the queue, go back to the first one
        current_bidder = current_bidder % queue.size();
    }

    if (highest_bid < min_target) // handling the no initial bid problem where if no one bids the last one is left with 0 as target.
    {                             // now they'll have 150 as target which is the bare minimum to start the game.
        highest_bidder_id = queue.at(lobby_size - 1)->player_id;
        highest_bid = min_target;
    }

    team1_target = highest_bid;

    cout << "Player " << highest_bidder_id << " placed the highest bid and their target is " << team1_target << "\n";
    partner_select();
    start_hand_from = highest_bidder_id;
}

void Game::partner_select()
{
    int selected_color;
    int selected_value;

    bool is_valid = true;

    // keep taking the input until the player selects a card they don't have
    do
    {
        is_valid = true;
        std::cout << "Enter color : \n0 - spades\n1 - clubs\n2 - hearts\n3 - diamonds\n";
        std::cin >> selected_color;
        std::cout << "Enter the id of the card you want to select as partner\n";
        std::cout << "0 - Ace\n 1-2-3-4-5-6-7-8-9-10\n11 - Jack\n12 - Queen\n13 - King\n";
        std::cin >> selected_value;
        // while making multiplayer we will take the two inputs at the same time, as json.

        // code to check if the player has the card as they cannot select a card they have
        for (int i = 0; i < players[highest_bidder_id].cards.size(); i++)
        {
            if (players[highest_bidder_id].cards[i]->value == selected_value && players[highest_bidder_id].cards[i]->color == selected_color)
            {
                cout << "You cannot select a card you have\n";
                is_valid = false;
                break;
            }
        }
    } while (is_valid == false);

    partner_card = new card(selected_value, selected_color);
    // call the team builder function to assign the teams based on the partner card
    team_builder();

    cout << "\n Select the master(sir) color" << endl; // As Being the highest bidder after choosing the partners they now can choose the master color or SIR.
    cin >> trump_color;

    // Debugging
    // team 1
    cout << "Team 1: ";
    for (int i = 0; i < lobby_size; i++)
    {
        if (players[i].team == 1)
        {
            cout << i << " ";
        }
    }
    cout << endl;

    // team 2
    cout << "Team 2: ";
    for (int i = 0; i < lobby_size; i++)
    {
        if (players[i].team == 2)
        {
            cout << i << " ";
        }
    }
    cout << endl;

    // debugging ends

    return;
}

/**
 * @brief This function searches for the cards in the player's hand,
 *        and then places players in team 1 and team 2 based on if they have the partner card.
 */
void Game::team_builder()
{
    // finding what players have the partner card and what players don't
    for (int i = 0; i < lobby_size; i++)
    {
        bool is_partner = false;
        if (!highest_bidder_id)
        {
            // check if the player has the partner card in their hand if yes team 1 else team 2(default)
            for (int j = 0; j < players[i].cards.size(); j++)
            {
                if (*players[i].cards[j] == *partner_card)
                {
                    players[i].team = 1;
                }
            }
        }
        // if the player is the highest bidder they are automatically in team 1
        else
        {
            players[highest_bidder_id].team = 1;
        }
    }
    return;
}

/**
 * @brief This function returns the points that card has, based on the card value and color of the given card object.
 *
 * @param card *player_card
 */

int Game::card_points(card *player_card)
{
    int points = 0;
    // Ace will have id 0 and 10 points
    if (player_card->value == 1)
    {
        points = 10;
    }
    // 3 of spade value 3 and color 0
    else if (player_card->value == 3 && player_card->color == 0)
    {
        points = 30;
    }
    // The 5 card has 5 points
    else if (player_card->value == 5)
    {
        points = 5;
    }
    // The cards 10 to 13 have 10 points
    else if (player_card->value > 9)
    {
        points = 10;
    }
    return points;
}

void Game::game_main_routine()
{

    // take the bids from the players
    bidding();

    for (int i = 0; i < no_of_player_cards; i++)
    {
        hand_logic();
    }
    if (team1_scored >= team1_target)
    {
        cout << "Team 1 Wins";
    }
    else
    {
        cout << "Team 2 Wins";
    }
    game_reset();
}

/**
 * @brief This is the routine to asses the winner of the hand,
 *        and then assign the points to the winner and the team.
 *       It also sets the next hand to start from the winner of the current hand.
 */
void Game::hand_logic() // return the player id who won the hand so we can start the next hand from them
{
    cout << "Inside hand Logic" << endl;

    player_inputs(start_hand_from);
    int current_winner_id = start_hand_from;                                                            // the player who has the highest card
    int hand_color = players[start_hand_from].cards[players[start_hand_from].current_input]->color;     // the color of the hand
    int max_card_value = players[start_hand_from].cards[players[start_hand_from].current_input]->value; // the first card is the highest card for now
    bool is_trump_in_hand = 0;                                                                          // if there is a trump card in the hand, change this to 1.
    int trump_max_value = 0;                                                                            // the value of the highest trump card 0(because the value of the card is 1-13)
    int hand_points = 0;                                                                                // the total points of the hand

    int player_id = (start_hand_from + 1) % lobby_size; // the player who is currently playing;

    // i can simply take inputs and asses the winner directly, but keeping the cards on the server will make it impossible to cheat.
    // as the server is the one sending cards to the players. and players can only send the card they have, which i'll check against the cards they have.
    while (player_id != start_hand_from)
    {
        // register the points of the hand
        hand_points += card_points(players[player_id].cards[players[player_id].current_input]);

        // if the player has the color of the hand
        if (player_has_color(player_id, hand_color) && players[player_id].cards[players[player_id].current_input]->value > max_card_value)
        {
            // if the player has a card with a higher value than the current highest card
            current_winner_id = player_id;
            max_card_value = players[player_id].cards[players[player_id].current_input]->value;
        }
        else if (player_has_color(player_id, trump_color) && players[player_id].cards[players[player_id].current_input]->value > trump_max_value)
        {
            // if the player played a trump card with a higher value than the current highest trump card
            is_trump_in_hand = 1;
            current_winner_id = player_id;
            trump_max_value = players[player_id].cards[players[player_id].current_input]->value;
        }
        // if the player doesn't have the color of the hand and doesn't have a trump card
        // they can play any card they want but they can't win the hand

        // remove the card from the player's hand
        players[player_id].cards.erase(players[player_id].cards.begin() + players[player_id].current_input);
        player_id = (player_id + 1) % lobby_size;
    }
    start_hand_from = current_winner_id;
    players[current_winner_id].scored += hand_points; // add the points of the hand to the winner's score
    // add the points of the hand to the team's score as well.
    if (players[current_winner_id].team == 1)
    {
        team1_scored += hand_points;
    }
    else
    {
        team2_scored += hand_points;
    }
    return;
}

void Game::game_reset()
{

    for (int i = 0; i < lobby_size; i++)
    {
        players[i].team = 0;
        players[i].scored = 0;
        players[i].current_input = 0;
        players[i].cards.clear();
    }
    return;
}

Game::~Game()
{
    delete[] players;
    players = nullptr;
    for (auto card : deck)
    {
        delete card;
    }
    deck.clear();
}

bool operator==(const Game::card &c1, const Game::card &c2)
{
    return c1.color == c2.color && c1.value == c2.value;
}

/**
 * @brief Overloading the < operator to compare the cards
 *        based on the color then the value
 * @param c1 AND c2
 *
 * @return bool
 *
 */
bool operator<(const Game::card &c1, const Game::card &c2)
{
    if (c1.color == c2.color)
    {
        return c1.value < c2.value;
    }
    return c1.color < c2.color;
}

/**
 * @brief Overloading the > operator to compare the cards
 *        based on the color then the value
 * @param c1 AND c2
 *
 * @return bool
 *
 */
bool operator>(const Game::card &c1, const Game::card &c2)
{
    if (c1.color == c2.color)
    {
        return c1.value > c2.value;
    }
    return c1.color > c2.color;
}