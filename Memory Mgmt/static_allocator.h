#include <iostream>

class allocator
{
private:
    struct linker { linker *next; };  // embedded pointer
    linker *freeStore = nullptr;      // head of freelist
    int chunk = 5;                    // default block size
public:
    void setChunk(int x) { chunk = x; }
    void *allocate(size_t size)
    {
        linker *p = nullptr;
        if(freeStore == nullptr)
        {
            p = freeStore = (linker *)malloc(size * chunk);
            for(int i = 0; i < chunk - 1; i++)
            {
                p->next = (linker *)((char *)p + size);
                p = p->next;
            }
            p->next = nullptr; 
        }
        p = freeStore;
        freeStore = freeStore->next;
        return p;
    }
    void deallocate(void *block, size_t size)
    {
        if(block == nullptr) return;
        linker *p = (linker *)block;
        p->next = freeStore;
        freeStore = p;
    }
};

// macro for static allocator
// DECLARE_POOL_ALLOC, used in calss definition
#define DECLARE_POOL_ALLOC \
public:	\
  static void *operator new(size_t size) { return myAlloc.allocate(size); } 	\
  static void operator delete(void *block, size_t size) { return myAlloc.deallocate(block, size); } \
protected: \
  static allocator myAlloc;

// IMPLEMENT_POOL_ALLOC, used in class implementation file
#define IMPLEMENT_POOL_ALLOC(class_name) \
allocator class_name::myAlloc