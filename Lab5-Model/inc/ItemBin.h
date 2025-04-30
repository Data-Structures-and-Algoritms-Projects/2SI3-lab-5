#ifndef ITEMBIN_H
#define ITEMBIN_H

#include "objPos.h"
#include "objPosList.h"
#include "GameMechs.h"
#include "Player.h"
#include "objPosSHLinkedList.h"

#define LIST_SIZE 25

class GameMechs;
class Player;
class objPosSHLinkedList;

class ItemBin
{
    private:
        objPos myDrawItem;
        objPosList *myList;
                            
        GameMechs* gmRef;
        char** boardRef;

        void undrawItem();
        void DigitToChar(char result[]); // pass by ref

        void populateRandomItems();  // new method to populate random item list

    public:
        ItemBin(GameMechs* gm);
        ~ItemBin();

        void drawItem();
        void generateItem();
        objPos getItem();
        
        // pass by array!
        void pickItems(objPos list[], int size);
};

#endif