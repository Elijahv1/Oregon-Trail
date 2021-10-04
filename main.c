#include "map.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Please use: main filename.txt\n");
        return 0;
    }

    printf("Importing file: %s\n", argv[1]);
    FILE* fptr = fopen(argv[1], "r"); // Input file descriptor
    if (fptr == NULL) {
        printf("Cannot open file\n");
        exit(1);
    }

    // Inporting file variables
    int mapSize, viewSize, food;
    char inputBuff[256]; // Buffer to read in lines
    fscanf(fptr, "MapSize:%d\nViewPort:%d\nFood:%d\n\nMap\n", &mapSize, &viewSize, &food);

    // Inputtng the map
    NODE_t* leftNode = NULL;
    NODE_t* aboveNode = NULL;
    NODE_t* startNode;
    NODE_t* firstInRow;
    while (fgets(inputBuff, 256, fptr)) {

        // Make sure we aren't in 1st or last row
        if (inputBuff[1] != OCEAN) {

            // Loop through
            for (int i = 1; i <= mapSize; i++) {
                leftNode = insert(inputBuff[i], leftNode, aboveNode);

                // set at first in row
                if (i == 1) { firstInRow = leftNode; }

                // increment above node
                if (aboveNode) { aboveNode = aboveNode->m_right; }

                // Set start ptr if it it's start
                if (inputBuff[i] == START) { startNode = leftNode; }
            }
            aboveNode = firstInRow;
            leftNode = NULL;
        }
    }
    fclose(fptr);

    // Cannot have even view size
    if (viewSize % 2 == 0) {
        printf("File Import Error: ViewPort is even.");
        return 0;
    }

    // Start Game
    bool replay;
    do {
        replay = game(mapSize, viewSize, food, startNode);
        if (replay) { clean(firstInRow); }
    } while (replay);

    // Remove dynamic memory
    removeDynamic(firstInRow, mapSize);

    return 0;
}