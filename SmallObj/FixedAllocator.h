//
// Created by lee on 17/6/30.
//

#ifndef SMALLOBJ_FIXEDALLOCATOR_H
#define SMALLOBJ_FIXEDALLOCATOR_H

#include <cstddef>
#include <vector>

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif // DEFAULT_CHUNK_SIZE

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

// FixedAllocator
// allocate fixed-size objects
class FixedAllocator
{
private:
    // Chunk
    // raw memory
    // too dangerous, not allowed to use
    struct Chunk
    {
        // functions
        void Init(std::size_t, unsigned char);
        void* Allocate(std::size_t);
        void Deallocate(void*, std::size_t);
        void Reset(std::size_t, unsigned char);
        void Release();
        
        // members
        unsigned char* m_pData;
        unsigned char m_firstAvailableBlock, m_blocksAvailable;
    };
    
    // internal functions
    void DoDeallocate(void*);
    Chunk* VicinityFind(void*);
    
    // members
    std::size_t m_blockSize;
    unsigned char m_numBlocks;
    typedef std::vector<Chunk> Chunks;
    Chunks m_chunks;
    Chunk* m_allocChunk;
    Chunk* m_deallocChunk;
    // For ensuring proper copy semantics
    mutable const FixedAllocator* m_prev;
    mutable const FixedAllocator* m_next;

public:
    // ctors
    explicit FixedAllocator(std::size_t = 0);
    FixedAllocator(const FixedAllocator&);
    FixedAllocator& operator=(const FixedAllocator&);
    ~FixedAllocator();
    
    // swap
    void swap(FixedAllocator&);
    
    // functions
    void* Allocate();
    void Deallocate(void*);
    std::size_t BlockSize() const {return m_blockSize;}
};


#endif //SMALLOBJ_FIXEDALLOCATOR_H
