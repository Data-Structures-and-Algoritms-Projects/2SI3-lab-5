#include "objPosLinearHashing.h"

#include <iostream>
using namespace std;

objPosLinearHashing::objPosLinearHashing()
{
    // do you need anything here? 
    myHashTable = new objPos[TABLE_SIZE];
    tableSize = TABLE_SIZE;
    for(int i = 0; i < tableSize; i++)
    {
        myHashTable[i].setX(0);
        myHashTable[i].setY(0);
        myHashTable[i].setPF('\0');
        myHashTable[i].setNum(0);
        myHashTable[i].setSym('\0');
    }
}

objPosLinearHashing::objPosLinearHashing(int size) : objPosHashTable(size)
{
    // do you need to modify anything here?
    myHashTable = new objPos[size];
    tableSize = size;
    for(int i = 0; i < tableSize; i++)
    {
        myHashTable[i].setX(0);
        myHashTable[i].setY(0);
        myHashTable[i].setPF('\0');
        myHashTable[i].setNum(0);
        myHashTable[i].setSym('\0');
    }
}

objPosLinearHashing::~objPosLinearHashing()
{
    // do you need anything here? 
    delete[] myHashTable;
    myHashTable = nullptr; // set the pointer to null to avoid dangling pointer    
}

bool objPosLinearHashing::insert(const objPos &thisPos)
{
    // Insert thisPos into the Hash Table (if not already inserted) using Linear Probing

    // When inserting, using Symbol as a way to check if the table entry is occupied or free
    //    Symbol = 0 (FREE),   Symbol = 'v' (Occupied)
    if(isInTable(thisPos)) return false;
    for(int i = 0; i < tableSize; i++)
    {
        int index = (calculateHashing((int)thisPos.getPF(), thisPos.getNum()) + i) % tableSize; // Linear Probing
        if(myHashTable[index].getSym() == '\0') // Check if the slot is free
        {
            myHashTable[index].setNum(thisPos.getNum()); // Insert the object
            myHashTable[index].setPF(thisPos.getPF()); // Insert the object
            myHashTable[index].setSym('v'); // Mark as occupied
            return true;
        }
    }
    // If the table is full, return false
    return false;
}

bool objPosLinearHashing::remove(const objPos &thisPos)
{
    // Remove thisPos from the Hash Table using Linear Probing

    // If found in the table, apply lazy deletion by setting Symbol = 0 (free), and return true
    // If not found in the table, return false

    // You need to think carefully about what conditions will end the probing
    // During debugging, you may use MAX_PROBING_COUNT defined in objPosHash.h as a hard limit to the number of probes 
    // to be carried out.  This hard limit is not to fix the bug, but only to help you debugger more easily.  
    if(!isInTable(thisPos)) return false;
    for(int i = 0; i < MAX_PROBING_COUNT; i++)
    {
        int index = (calculateHashing(thisPos.getPF(), thisPos.getNum()) + i) % tableSize; // Linear Probing
        if(myHashTable[index].getSym() == 'v' && myHashTable[index].getNum() == thisPos.getNum() && myHashTable[index].getPF() == thisPos.getPF()) // Check if the object is found
        {
            myHashTable[index].setSym('\0'); // Mark as free
            return true;
        }
    }
    // If the object is not found, return false
    return false;
}

bool objPosLinearHashing::isInTable(const objPos &thisPos) const
{
    // Check if thisPos is in the Hash Table using Linear Probing

    // If found in the table, return true
        // Remember, finding the match is not enough.  The entry has to be VALID (symbol = 'v').
    // If not found in the table, return false

    // You need to think carefully about what conditions will end the probing
    // During debugging, you may use MAX_PROBING_COUNT defined in objPosHash.h as a hard limit to the number of probes 
    // to be carried out.  This hard limit is not to fix the bug, but only to help you debugger more easily.    
    for(int i = 0; i < MAX_PROBING_COUNT; i++)
    {
        int index = (calculateHashing(thisPos.getPF(), thisPos.getNum()) + i) % tableSize; // Linear Probing
        if(myHashTable[index].getSym() == 'v' && myHashTable[index].getNum() == thisPos.getNum() && myHashTable[index].getPF() == thisPos.getPF()) // Check if the object is found and valid
        {
            return true; // Found
        }
    }
    // If the object is not found, return false
    return false;
}