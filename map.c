// Author: Elijah Vaughnn
// Date:   10/28/19
// Class:  CMPE 311

#include "map.h"


// Name:   insertData
// Input:  Raw character data, new dynamic node
// Output: None, updates node
// Desc:   Alike constructor, creates necessary variables for node
void insertData(char data, NODE_t* refNode) {

    // Changing disease specific variables
    if (data == DYSENTERY || data == FLU || data == COLD) {
        refNode->m_threatType = DISEASE;
        refNode->m_threat.m_disease = data;
    }

    // Changing animal specific variables
    else if (data == GRIZZLY || data == BOAR || data == ELK || data == HARE) {
        refNode->m_threatType = ANIMAL;
        refNode->m_threat.m_animal = data;
    }

    // Make sure threattype is not set to a threat
    else {
        refNode->m_threatType = NONE;
    }

    // Store character data setup bools and pointers
    refNode->m_charData = data;
    refNode->m_discovered = false;
    refNode->m_isNothing = false;
    refNode->m_left = NULL;
    refNode->m_right = NULL;
    refNode->m_up = NULL;
    refNode->m_down = NULL;

    // Rivers, START, and END are discovered already
    if (data == RIVER || data == START || data == END) { 
        refNode->m_discovered = true; 
    }
}


// Name:   insert
// Input:  Raw character data, node left, and node above
// Output: Returns the new dynamically created node
// Desc:   Updates 4 ptrs (max), changing the top and left node pointers
NODE_t* insert(char data, NODE_t* leftNode, NODE_t* topNode) {

    // Create new node w/dynamic memory
    NODE_t* newNode = (NODE_t*)malloc(sizeof(NODE_t));
    insertData(data, newNode);

    // First Node to insert, update 0 ptrs
    if (!leftNode && !topNode) {
        newNode->m_x = 0;
        newNode->m_y = 0;
    }

    // In top Row, update 2 ptrs
    else if (leftNode && !topNode) {
        newNode->m_x = leftNode->m_x + 1;
        newNode->m_y = 0;
        leftNode->m_right = newNode;
        newNode->m_left = leftNode;
    }

    // First in row (not in top row), update 2 ptrs
    else if (!leftNode && topNode) {
        newNode->m_x = 0;
        newNode->m_y = topNode->m_y - 1;
        topNode->m_down = newNode;
        newNode->m_up = topNode;
    }

    // Normal insertion, update 4 ptrs
    else {
        newNode->m_x = leftNode->m_x + 1;
        newNode->m_y = topNode->m_y - 1;
        leftNode->m_right = newNode;
        newNode->m_left = leftNode;
        topNode->m_down = newNode;
        newNode->m_up = topNode;
    }
    
    // Don't want to go out of scope
    return newNode;
}


// Name:   printChar
// Input:  Node to print, whether or not it's the node the player is on
// Output: None, prints out character
// Desc:   Prints the character associated with the node (using game logic)
void printChar(NODE_t* trackNode, bool isPlayer) {
    
    // Just print 'X' if it's the player
    if (isPlayer) {
        printf("%c", START);
    }
    else {

        // Don't print if undiscovered
        if (!trackNode->m_discovered) { 
            printf("%c", UNKNOWN); 
        }

        // Print if already looted/nothing
        else if (trackNode->m_isNothing) { 
            printf("%c", NOTHING);
        }

        // Regular print
        else { 
            printf("%c", trackNode->m_charData); 
        }
    }
}


// Name:   displayView
// Input:  Takes in the node the player is on, viewSize, and mapsize
// Output: None, prints out view around the player
// Desc:   Prints the view around the player in accordance with viewSize
void displayView(NODE_t* curNode, int viewSize, int mapSize) {

    // Just print single square
    if (viewSize <= 1) {
        printChar(curNode, true);
        return;
    }

    // Amount of nodes in each direction to print (finding min)
    int reach = (viewSize - 1) / 2;
    int left = (curNode->m_x < reach) ? curNode->m_x : reach;
    int right = (mapSize - 1 - curNode->m_x < reach) ? 
                 mapSize - 1 - curNode->m_x : reach;
    int up = (-curNode->m_y < reach) ? -curNode->m_y : reach;
    int down = (mapSize - 1 + curNode->m_y < reach) ?
                mapSize - 1 + curNode->m_y : reach;

    // Starting at upper left node
    NODE_t* trackNode = curNode;
    for (int i = 0; i < up; i++) { trackNode = trackNode->m_up; }
    for (int i = 0; i < left; i++) { trackNode = trackNode->m_left; }

    // Printing empty rows before
    int counter = up;
    while (counter < reach) {
        for (int i = 0; i < viewSize; i++) { printf("%c", OCEAN); }
        printf("\n");
        counter++;
    }

    // Traversing columns
    int colHeight = 1 + up + down;
    int rowWidth = 1 + left + right;
    NODE_t* firstInRow = trackNode;
    for (int colIndex = 0; colIndex < colHeight; colIndex++) {

        // Printing empty nodes before
        for (int i = left; i < reach; i++) { printf("%c", OCEAN); }

        // Traversing row
        int rowIndex = 0;
        while (trackNode != NULL && rowIndex < rowWidth) {

            // Print X if it's the player
            if (trackNode == curNode) {
                printChar(trackNode, true);
            }

            // Normal print
            else {
                printChar(trackNode, false);
            }
            trackNode = trackNode->m_right;
            rowIndex++;
        }
        trackNode = firstInRow->m_down;
        firstInRow = trackNode;

        // Printing empty nodes after
        for (int i = right; i < reach; i++) { printf("%c", OCEAN); }
        printf("\n");
    }

    // Printing empty rows after
    counter = down;
    while (counter < reach) {
        for (int i = 0; i < viewSize; i++) { printf("%c", OCEAN); }
        printf("\n");
        counter++;
    }
}


// Name:   gameLogic
// Input:  Takes in the node the player is on, and their food reservoir
// Output: Whether or not the player moved
// Desc:   Using game logic and player input, calculates change in food/pos.
bool gameLogic(NODE_t* playerNode, int *food) {

    // Going to nothing
    if (playerNode->m_isNothing) { 
        return true;
    }

    // Going to animal
    if (playerNode->m_threatType == ANIMAL) {

        // Animal variables
        char animalName[8];
        int foodChange;
        if (playerNode->m_charData == GRIZZLY) {
            strcpy(animalName,"Grizzly");
            foodChange = GRIZZLY_F;
        }
        else if (playerNode->m_charData == BOAR) {
            strcpy(animalName, "Boar");
            foodChange = BOAR_F;
        }
        else if (playerNode->m_charData == ELK) {
            strcpy(animalName, "Elk");
            foodChange = ELK_F;
        }
        else {
            strcpy(animalName, "Hare");
            foodChange = HARE_F;
        }

        // User unput
        char input;
        do {
            printf("You have encountered a(n) %s, would you choose to hunt (h) "
                "or retreat (r)?\n", animalName);
            scanf(" %c", &input);
        } while (input != HUNT && input != RETREAT);

        // Gain/Lose food
        if (input == HUNT) {
            *food = *food + foodChange;
            playerNode->m_isNothing = true;
            return true;
        }

        // Retreating
        else {
            return false;
        }
    }

    // Going to Disease
    else if (playerNode->m_threatType == DISEASE) {

        // Disease variables
        char diseaseName[10];
        int foodChange;
        if (playerNode->m_charData == DYSENTERY) {
            strcpy(diseaseName, "Dysentery");
            foodChange = DYSENTERY_F;
        }
        else if (playerNode->m_charData == FLU) {
            strcpy(diseaseName, "Flu");
            foodChange = FLU_F;
        }
        else {
            strcpy(diseaseName, "Cold");
            foodChange = COLD_F;
        }

        // Change in food
        printf("You got sick with %s, your food storage drops by %d\n", 
               diseaseName, foodChange);
        *food = *food + foodChange;
        playerNode->m_isNothing = true;
        return true;
    }

    // Going to river
    else if (playerNode->m_charData == RIVER) {

        // User input
        char input;
        do {
            printf("You are attempting to ford a river; you may either choose to "
                   "ford (f) or retreat (r): ");
            scanf(" %c", &input);
        } while (input != FORD && input != RETREAT);

        // Lose food
        if (input == FORD) {
            *food = *food + FORD_RIVER;
            return true;
        }

        // retreating
        else {
            return false;
        }
    }

    // Going to END
    else {
        return true;
    }
}



// Name:   game
// Input:  Takes mapSize, viewSize, food, and starting node 'X'
// Output: Whether or not the player wants to play again
// Desc:   Handles menuing and some game logic w/input checking
bool game(int mapSize, int viewSize, int food, NODE_t* startNode) {

    // Displaying menu/instructions
    int menuOpt;
    printf("Welcome to Oregon Trail\n");
    do {
        printf("1. Start Game\n2. Display Instructions\n3. Quit\n");
        scanf("%d", &menuOpt);
        if (menuOpt == INSTRUCTIONS) {
            printf("You must get to the end 'X' without losing too much food " 
                   "supply, here are the obstacles:\nAnimals: Grizzly (G) [-10]"
                   ", Boar (B) [-5], Elk (E) [0], Hare (H) [5]\nDiseases: "
                   "Cold (C) [-5], Flu (F) [-10], Dysentary (D) [-15]\n"
                   "Other: River (R) [20 to Ford]\n\n");
        }
    } while (menuOpt != QUIT && menuOpt != START_GAME);

    // Exit if quit
    if (menuOpt == QUIT) { return false; }

    // Starting Game
    NODE_t* playerNode = startNode;
    while (food > 0 && playerNode->m_charData != END) {

        // Display Food/ViewPort
        printf("Food: %d\n", food);
        displayView(playerNode, viewSize, mapSize);

        // Direction to move
        char userDir;
        do {
            printf("Enter a direction to move (l/r/u/d):");
            scanf(" %c", &userDir);
        } while (userDir != LEFT_DIR && userDir != RIGHT_DIR && userDir 
                 != UP_DIR && userDir != DOWN_DIR);

        // Game Logic
        bool userMoved;
        if (userDir == LEFT_DIR) {
            if (playerNode->m_left) {
                
                // Did user move?
                playerNode->m_left->m_discovered = true;
                userMoved = gameLogic(playerNode->m_left, &food);
                if (userMoved) { playerNode = playerNode->m_left; }
            }
            else {
                printf("Ocean is impassable\n");
            }
        }
        else if (userDir == RIGHT_DIR) {
            if (playerNode->m_right) {

                // Did user move?
                playerNode->m_right->m_discovered = true;
                userMoved = gameLogic(playerNode->m_right, &food);
                if (userMoved) { playerNode = playerNode->m_right; }
            }
            else {
                printf("Ocean is impassable\n");
            }
        }
        else if (userDir == UP_DIR) {
            if (playerNode->m_up) {

                // Did user move?
                playerNode->m_up->m_discovered = true;
                userMoved = gameLogic(playerNode->m_up, &food);
                if (userMoved) { playerNode = playerNode->m_up; }
            }
            else {
                printf("Ocean is impassable\n");
            }
        }
        else {
            if (playerNode->m_down) {

                // Did user move?
                playerNode->m_down->m_discovered = true;
                userMoved = gameLogic(playerNode->m_down, &food);
                if (userMoved) { playerNode = playerNode->m_down; }
            }
            else {
                printf("Ocean is impassable\n");
            }
        }
    }

    if (food <= 0) {
        printf("The trail proved too difficult, better luck next time\n"
               "Food: %d\n", food);
    }
    else {
        printf("Congratulations, you traversed the Oregon Trail!\n"
               "Food: %d\n", food);
    }

    // User Input
    char again;
    do {
        printf("Start Again (y/n)? ");
        scanf(" %c", &again);
    } while (again != YES && again != NO);

    if (again == YES) { return true; }
    return false;
}


// Name:   clean
// Input:  Takes in bottom left node (start is not always in corner)
// Output: None, changes node values
// Desc:   Iterates through each node, clearing discovered and isNothing
void clean(NODE_t* botLeftNode) {

    // Iterating though each row (going up)
    NODE_t* colNode = botLeftNode;
    NODE_t* rowNode = botLeftNode;
    while (rowNode != NULL) {

        // Iterating though each column in row (going right)
        colNode = rowNode;
        rowNode = rowNode->m_up;
        while (colNode != NULL)
        {
            if (colNode->m_charData != RIVER && colNode->m_charData != START &&
                colNode->m_charData != END) {
                colNode->m_discovered = false;
                colNode->m_isNothing = false;
            }
            colNode = colNode->m_right;
        }
    }
}


// Name:   removeDynamic
// Input:  Takes in bottom left node (start is not always in corner)
// Output: None, deletes dynamic nodes
// Desc:   Iterates through each node, deleting them w/o segfault
void removeDynamic(NODE_t* botLeftNode, int mapSize) {

    // Iterating though each row (going up)
    NODE_t* colNode = botLeftNode;
    NODE_t* rowNode = botLeftNode;
    NODE_t* prevNode;
    while (rowNode != NULL) {

        // Iterating though each column in row (going right)
        colNode = rowNode;
        rowNode = rowNode->m_up;
        while (colNode != NULL)
        {
            prevNode = colNode;
            colNode = colNode->m_right;
            free(prevNode); // Deleting dynamic memory
        }
    }
}