//
// Created by lee on 17/6/30.
//

#include "FixedAllocator.h"
#include <stdexcept>
#include <climits>
#include <cassert>

// FixedAllocator::Chunk

void FixedAllocator::Chunk::Init(std::size_t blockSize, unsigned char blocks)
{
    assert(blockSize > 0);
    assert(blocks > 0);
    // overflow check
    assert((blockSize * blocks) / blockSize == blocks);
    
    m_pData = new unsigned char[blockSize * blocks];
    Reset(blockSize, blocks);
}

void FixedAllocator::Chunk::Reset(std::size_t blockSize, unsigned char blocks)
{
    assert(blockSize > 0);
    assert(blocks > 0);
    // overflow check
    assert((blockSize * blocks) / blockSize == blocks);
    
    m_firstAvailableBlock = 0;
    m_blocksAvailable = blocks;
    
    unsigned char i = 0, *p = m_pData;
    for (; i != blocks; p += blockSize)
    {
        *p = ++i;
    }
}

void FixedAllocator::Chunk::Release()
{
    delete[] m_pData;
}

void* FixedAllocator::Chunk::Allocate(std::size_t blockSize)
{
    if (!m_blocksAvailable) return nullptr;
    
    assert((m_firstAvailableBlock * blockSize) / blockSize == m_firstAvailableBlock);
    
    unsigned char* pResult = m_pData + (m_firstAvailableBlock * blockSize);
    m_firstAvailableBlock = *pResult;
    --m_blocksAvailable;
    
    return pResult;
}

void FixedAllocator::Chunk::Deallocate(void* p, std::size_t blockSize)
{
    assert(p >= m_pData);
    
    unsigned char* toRelease = static_cast<unsigned char*>(p);
    // Alignment check
    assert((toRelease - m_pData) % blockSize == 0);
    
    *toRelease = m_firstAvailableBlock;
    m_firstAvailableBlock = static_cast<unsigned char>((toRelease - m_pData) / blockSize);
    // Truncation check
    assert(m_firstAvailableBlock == (toRelease - m_pData) / blockSize);
    
    ++m_blocksAvailable;
}

FixedAllocator::FixedAllocator(size_t blockSize)
: m_blockSize(blockSize), m_allocChunk(nullptr), m_deallocChunk(nullptr)
{
    assert(m_blockSize > 0);
    
    m_prev = m_next = this;
    
    std::size_t numBlocks = DEFAULT_CHUNK_SIZE / blockSize;
    if (numBlocks > UCHAR_MAX) numBlocks = UCHAR_MAX;
    else if (numBlocks == 0) numBlocks = 8 * blockSize;
    
    m_numBlocks = static_cast<unsigned char>(numBlocks);
    assert(m_numBlocks == numBlocks);
}

FixedAllocator::FixedAllocator(const FixedAllocator& rhs)
: m_blockSize(rhs.m_blockSize),
  m_numBlocks(rhs.m_numBlocks),
  m_chunks(rhs.m_chunks)
{
    m_prev = &rhs;
    m_next = rhs.m_next;
    rhs.m_next->m_prev = this;
    rhs.m_next = this;
    
    m_allocChunk = rhs.m_allocChunk ? &m_chunks.front() + (rhs.m_allocChunk - &rhs.m_chunks.front()) : nullptr;
    
    m_deallocChunk = rhs.m_deallocChunk ? &m_chunks.front() + (rhs.m_deallocChunk - &rhs.m_chunks.front()) : nullptr;
}

FixedAllocator& FixedAllocator::operator=(const FixedAllocator& rhs)
{
    FixedAllocator copy(rhs);
    copy.swap(*this);
    return *this;
}

FixedAllocator::~FixedAllocator()
{
    if (m_prev != this)
    {
        m_prev->m_next = m_next;
        m_next->m_prev = m_prev;
        return;
    }
    
    assert(m_prev == m_next);
    Chunks::iterator i;
    for (i = m_chunks.begin(); i != m_chunks.end(); i++)
    {
        //assert(i->m_blocksAvailable == m_numBlocks);
        i->Release();
    }
}

void FixedAllocator::swap(FixedAllocator& rhs)
{
    using std::swap;
    
    swap(m_blockSize, rhs.m_blockSize);
    swap(m_numBlocks, rhs.m_numBlocks);
    m_chunks.swap(rhs.m_chunks);
    swap(m_allocChunk, rhs.m_allocChunk);
    swap(m_deallocChunk, rhs.m_deallocChunk);
}

void* FixedAllocator::Allocate()
{
    if (m_allocChunk == nullptr || m_allocChunk->m_blocksAvailable == 0)
    {
        Chunks::iterator i;
        for (i = m_chunks.begin(); ; i++)
        {
            if (i == m_chunks.end())
            {
                // Initialize
                m_chunks.reserve(m_chunks.size() + 1);
                Chunk newChunk;
                newChunk.Init(m_blockSize, m_numBlocks);
                m_chunks.push_back(newChunk);
                m_allocChunk = &m_chunks.back();
                m_deallocChunk = &m_chunks.front();
                break;
            }
            if (i->m_blocksAvailable > 0)
            {
                m_allocChunk = &*i;
                break;
            }
        }
    }
    assert(m_allocChunk != nullptr);
    assert(m_allocChunk->m_blocksAvailable > 0);
    
    return m_allocChunk->Allocate(m_blockSize);
}

void FixedAllocator::Deallocate(void* p)
{
    assert(!m_chunks.empty());
    assert(&m_chunks.front() <= m_deallocChunk);
    assert(&m_chunks.back() >= m_deallocChunk);
    
    m_deallocChunk = VicinityFind(p);
    assert(m_deallocChunk);
    
    DoDeallocate(p);
}

FixedAllocator::Chunk* FixedAllocator::VicinityFind(void* p)
{
    assert(!m_chunks.empty());
    assert(m_deallocChunk);
    
    const std::size_t chunkLength = m_numBlocks * m_blockSize;
    
    Chunk* lo = m_deallocChunk;
    Chunk* hi = m_deallocChunk + 1;
    Chunk* loBound = &m_chunks.front();
    Chunk* hiBound = &m_chunks.back() + 1;
    
    // Special case: m_deallocChunk id the last in the array
    if (hi == hiBound) hi = nullptr;
    
    while (1)
    {
        if (lo)
        {
            if (p >= lo->m_pData && p < lo->m_pData + chunkLength)
            {
                return lo;
            }
            if (lo == loBound) lo = nullptr;
            else --lo;
        }
    
        if (hi)
        {
            if (p >= hi->m_pData && p < hi->m_pData + chunkLength)
            {
                return hi;
            }
            if (++hi == hiBound) hi = nullptr;
        }
    }
    assert(false);
    return nullptr;
}

void FixedAllocator::DoDeallocate(void* p)
{
    assert(m_deallocChunk->m_pData <= p);
    assert(m_deallocChunk->m_pData + m_numBlocks * m_blockSize > p);
    
    // call into the chunk, will adjust the inner list but won't release memory
    m_deallocChunk->Deallocate(p, m_blockSize);
    
    if (m_deallocChunk->m_blocksAvailable == m_numBlocks)
    {
        // m_deallocChunk is completely free, should we release it?
        
        Chunk& lastChunk = m_chunks.back();
        
        if (&lastChunk == m_deallocChunk)
        {
            // check if we have two last chunks empty
            
            if (m_chunks.size() > 1 && m_deallocChunk[-1].m_blocksAvailable == m_numBlocks)
            {
                // Two free chunks, discard the last one
                lastChunk.Release();
                m_chunks.pop_back();
                m_allocChunk = m_deallocChunk = &m_chunks.front();
            }
            return;
        }
        
        if (lastChunk.m_blocksAvailable == m_numBlocks)
        {
            // Two free blocks, discard one
            lastChunk.Release();
            m_chunks.pop_back();
            m_allocChunk = m_deallocChunk;
        }
        else
        {
            // move the empty chunk to the end
            using std::swap;
            swap(*m_deallocChunk, lastChunk);
            m_allocChunk = &m_chunks.back();
        }
    }
}
