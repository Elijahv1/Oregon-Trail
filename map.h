// Author: Elijah Vaughnn
// Date:   10/28/19
// Class:  CMPE 311

#ifndef MAP_h
#define MAP_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// Global Constants
static const char BOAR = 'B';
static const char COLD = 'C';
static const char DYSENTERY = 'D';
static const char ELK = 'E';
static const char FLU = 'F';
static const char GRIZZLY = 'G';
static const char HARE = 'H';
static const char NOTHING = 'N';
static const char OCEAN = 'O';
static const char RIVER = 'R';
static const char UNKNOWN = 'U';
static const char START = 'X';
static const char END = 'Z';
static const char LEFT_DIR = 'l';
static const char RIGHT_DIR = 'r';
static const char UP_DIR = 'u';
static const char DOWN_DIR = 'd';
static const char HUNT = 'h';
static const char RETREAT = 'r';
static const char FORD = 'f';
static const int YES = 'y';
static const int NO = 'n';
static const int FORD_RIVER = -20;


enum THREAT { DISEASE, ANIMAL, NONE };
enum DISEASES { DYSENTERY_F = -15, FLU_F = -10, COLD_F = -5 };
enum ANIMALS { GRIZZLY_F = -10, BOAR_F = -5, ELK_F = 0, HARE_F = 5};
enum MENU { START_GAME = 1, INSTRUCTIONS = 2, QUIT = 3 };

typedef union Threat {
    char m_disease;
    char m_animal;
}THREAT_t;

typedef struct Node {

    // The threat and threattype
    THREAT_t m_threat;
    enum THREAT m_threatType;

    // Integer X and Y locations for the Node
    int m_x, m_y;

    // Whether or not the position has been discovered/looted
    bool m_discovered, m_isNothing;

    // Each of the pointers to the positions on the map
    struct Node *m_up, *m_down, *m_left, *m_right;

    // Raw char data from input file
    char m_charData;

}NODE_t;

// Name:   insertData
// Input:  Raw character data, new dynamic node
// Output: None, updates node
// Desc:   Alike constructor, creates necessary variables for node
void insertData(char data, NODE_t* refNode);

// Name:   insert
// Input:  Raw character data, node left, and node above
// Output: Returns the new dynamically created node
// Desc:   Updates 4 ptrs (max), changing the top and left node pointers
NODE_t* insert(char data, NODE_t* leftNode, NODE_t* topNode);

// Name:   printChar
// Input:  Node to print, whether or not it's the node the player is on
// Output: None, prints out character
// Desc:   Prints the character associated with the node (using game logic)
void printChar(NODE_t* trackNode, bool isPlayer);

// Name:   displayView
// Input:  Takes in the node the player is on, viewSize, and mapsize
// Output: None, prints out view around the player
// Desc:   Prints the view around the player in accordance with viewSize
void displayView(NODE_t* curNode, int viewSize, int mapSize);

// Name:   gameLogic
// Input:  Takes in the node the player is on, and their food reservoir
// Output: Whether or not the player moved
// Desc:   Using game logic and player input, calculates change in food/pos.
bool gameLogic(NODE_t* playerNode, int *food);

// Name:   game
// Input:  Takes mapSize, viewSize, food, and starting node 'X'
// Output: Whether or not the player wants to play again
// Desc:   Handles menuing and some game logic w/input checking
bool game(int mapSize, int viewSize, int food, NODE_t* startNode);

// Name:   clean
// Input:  Takes in bottom left node (start is not always in corner)
// Output: None, changes node values
// Desc:   Iterates through each node, clearing discovered and isNothing
void clean(NODE_t* botLeftNode);

// Name:   removeDynamic
// Input:  Takes in bottom left node (start is not always in corner)
// Output: None, deletes dynamic nodes
// Desc:   Iterates through each node, deleting them w/o segfault
void removeDynamic(NODE_t* botLeftNode, int mapSize);


#endif