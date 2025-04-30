#ifndef OBJPOS_LINEAR_HASHING_H
#define OBJPOS_LINEAR_HASHING_H

#include "objPosHashTable.h"
#include "objPos.h"

class objPos;

class objPosLinearHashing : public objPosHashTable
{
    private:
        // do you need anything here?

    public:
        objPosLinearHashing();
        objPosLinearHashing(int);
        ~objPosLinearHashing();

        bool insert(const objPos &thisPos);
        bool remove(const objPos &thisPos);  // lazy delete 
        bool isInTable(const objPos &thisPos) const;
};

#endif