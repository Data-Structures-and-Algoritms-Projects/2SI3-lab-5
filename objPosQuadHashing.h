#ifndef OBJPOS_QUADRATIC_HASHING_H
#define OBJPOS_QUADRATIC_HASHING_H

#include "objPosHashTable.h"
#include "objPos.h"

class objPos;

class objPosQuadHashing : public objPosHashTable
{
    private:
        // do you need anything here?

    public:
        objPosQuadHashing();
        objPosQuadHashing(int);
        ~objPosQuadHashing();

        bool insert(const objPos &thisPos);
        bool remove(const objPos &thisPos);  // lazy delete 
        bool isInTable(const objPos &thisPos) const;
};

#endif