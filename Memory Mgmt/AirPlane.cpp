#include <iostream>
#include <new>
#include <cstddef>

class AirPlane
{
private:
    struct AirPlaneRep          // Rep = Representation
    {
        unsigned long miles;
        char type;
    };
    union                       // anonymous union
    {
        AirPlaneRep rep;
        AirPlane *next;
    };

public:
    void set(unsigned long m, char c) { rep.miles = m; rep.type = c; }
    unsigned long getMiles() { return rep.miles; }
    char getType() { return rep.type; }

private:
    static const int BLOCK_SIZE;
    static AirPlane *headOfFreeList;

public:
    static void *operator new(size_t size)
    {
        if(size != sizeof(AirPlane)) return ::operator new(size);   // 继承

        AirPlane *p = nullptr;
        if(headOfFreeList == nullptr)
        {
            headOfFreeList = static_cast<AirPlane*>(::operator new(size * BLOCK_SIZE));
            for(int i = 0; i < BLOCK_SIZE - 1; i++)
                headOfFreeList[i].next = headOfFreeList + i + 1;
            headOfFreeList[BLOCK_SIZE - 1].next = nullptr; 
        }
        p = headOfFreeList;
        headOfFreeList = headOfFreeList->next;
        return p;
    }
    static void operator delete(void *block, size_t size)
    {
        if(block == nullptr) return;
        if(size != sizeof(AirPlane))
        {
            ::operator delete(block);
            return;
        }
        AirPlane *deadObj = static_cast<AirPlane*>(block);
        deadObj->~AirPlane();
        deadObj->next = headOfFreeList;
        headOfFreeList = deadObj;
    }
};

// initialize static member data
const int AirPlane::BLOCK_SIZE = 512;
AirPlane *AirPlane::headOfFreeList = nullptr;