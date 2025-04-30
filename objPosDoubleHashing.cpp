#include "objPosDoubleHashing.h"
//#include "MacUILib.h"

#include <iostream>
using namespace std;

objPosDoubleHashing::objPosDoubleHashing()
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

objPosDoubleHashing::objPosDoubleHashing(int size) : objPosHashTable(size)
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

objPosDoubleHashing::~objPosDoubleHashing()
{
    // do you need anything here? 
    delete[] myHashTable;
    myHashTable = nullptr; // set the pointer to null to avoid dangling pointer
}

int objPosDoubleHashing::calculateSecondaryHashing(int input) const
{
    // Secondary Hashing Function

    // Formula : sum of all digits of the first hashing index mod 5.

    // Ex. Hashing Index = 36, then Secondary Hashing Index = (3 + 6) % 5 = 4
    int sum = 0;
    int tempInput = input;
    while(tempInput > 0)
    {
        sum += tempInput % 10;
        tempInput /= 10;
    }
    return (5 - sum % 5); // Return the secondary hashing index
}

bool objPosDoubleHashing::insert(const objPos &thisPos)
{
    // Insert thisPos into the Hash Table (if not already inserted) using Double Hashing

    // When inserting, using Symbol as a way to check if the table entry is occupied or free
    //    Symbol = 0 (FREE),   Symbol = 'v' (Occupied)
    if(isInTable(thisPos)) return false;
    for(int i = 0; i < MAX_PROBING_COUNT; i++)
    {
        int index = (calculateHashing((int)thisPos.getPF(), thisPos.getNum()) + i * calculateSecondaryHashing(calculateHashing((int)thisPos.getPF(), thisPos.getNum()))) % tableSize; // Double Hashing
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

bool objPosDoubleHashing::remove(const objPos &thisPos)
{
    // Remove thisPos from the Hash Table using Double Hashing

    // If found in the table, apply lazy deletion by setting Symbol = 0 (free), and return true
    // If not found in the table, return false

    // You need to think carefully about what conditions will end the probing
    // During debugging, you may use MAX_PROBING_COUNT defined in objPosHash.h as a hard limit to the number of probes 
    // to be carried out.  This hard limit is not to fix the bug, but only to help you debugger more easily.    
    if(!isInTable(thisPos)) return false;
    for(int i = 0; i < MAX_PROBING_COUNT; i++)
    {
        int index = (calculateHashing(thisPos.getPF(), thisPos.getNum()) + i * calculateSecondaryHashing(thisPos.getPF())) % tableSize; // Double Hashing
        if(myHashTable[index].getSym() == 'v' && myHashTable[index].getNum() == thisPos.getNum() && myHashTable[index].getPF() == thisPos.getPF()) // Check if the object is found
        {
            myHashTable[index].setSym('\0'); // Mark as free (lazy delete)
            return true;
        }
    }
    // If the object is not found, return false
    return false;
}

bool objPosDoubleHashing::isInTable(const objPos &thisPos) const
{
    // Check if thisPos is in the Hash Table using Double Hashing

    // If found in the table, return true
        // Remember, finding the match is not enough.  The entry has to be VALID (symbol = 'v').
    // If not found in the table, return false

    // You need to think carefully about what conditions will end the probing
    // During debugging, you may use MAX_PROBING_COUNT defined in objPosHash.h as a hard limit to the number of probes 
    // to be carried out.  This hard limit is not to fix the bug, but only to help you debugger more easily.    
    for(int i = 0; i < tableSize; i++)
    {
        int index = (calculateHashing(thisPos.getPF(), thisPos.getNum()) + i * calculateSecondaryHashing(thisPos.getPF())) % tableSize; // Double Hashing
        if(myHashTable[index].getSym() == 'v' && myHashTable[index].getNum() == thisPos.getNum() && myHashTable[index].getPF() == thisPos.getPF()) // Check if the object is found and valid
        {
            return true; // Object found
        }
    }
    // If the object is not found, return false
    return false;
}