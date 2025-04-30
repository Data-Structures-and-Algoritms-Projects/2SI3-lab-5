// This is an Abstract Class!!
// This is an Abstract Class!!
// This is an Abstract Class!!

#ifndef OBJPOS_HASHING_TABLE_H
#define OBJPOS_HASHING_TABLE_H

#include "objPos.h"

#define TABLE_SIZE 37
#define MAX_PROBING_COUNT 1000  // arbitrary to avoid integer overflow

class objPos;  // forward declaration

class objPosHashTable  // this is NOT an interface but an abstract class!!
{
    protected:
        objPos* myHashTable;
        int tableSize;

        // primary hashing function: common among all close hashing table
        int calculateHashing(int prefix, int number) const;

    public:        
        objPosHashTable();
        objPosHashTable(int size);
        ~objPosHashTable();

        double getLambda() const;
        void printMe() const;

        objPos* getTableRef() const { return myHashTable; }

        // Pure Virtual Functions - Making this class Abstract!
        virtual bool insert(const objPos &thisPos) = 0;
        virtual bool remove(const objPos &thisPos) = 0;  // lazy delete 
        virtual bool isInTable(const objPos &thisPos) const = 0;
        
};

#endif
