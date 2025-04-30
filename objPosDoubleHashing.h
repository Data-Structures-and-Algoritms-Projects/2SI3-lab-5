#ifndef OBJPOS_DOUBLE_HASHING_H
#define OBJPOS_DOUBLE_HASHING_H

#include "objPosHashTable.h"
#include "objPos.h"

class objPos;

class objPosDoubleHashing : public objPosHashTable
{
    private:
        // secondary hashing function
        int calculateSecondaryHashing(int input) const;     

    public:
        objPosDoubleHashing();
        objPosDoubleHashing(int);
        ~objPosDoubleHashing();

        bool insert(const objPos &thisPos);
        bool remove(const objPos &thisPos);  // lazy delete 
        bool isInTable(const objPos &thisPos) const;
};

#endif