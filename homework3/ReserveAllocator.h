#pragma once

#include <list>
#include <memory>

template<typename T, std::size_t ChunkSize>
class ReserveAllocator
{
public:
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind
    {
        using other = ReserveAllocator<U, ChunkSize>;
    };

    ReserveAllocator() = default;
    ~ReserveAllocator() = default;

    T *allocate(std::size_t n)
    {
        if (m_chunks.empty())
            AllocateNewChunk();

        if (!m_chunks.back().canAllocate(n))
            AllocateNewChunk();

        return m_chunks.back().allocate(n);
    }

    void deallocate(T *p, std::size_t n)
    {
        for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
        {
            if (it->tryDeallocate(p, n))
            {
                if (it->isEmpty() && m_chunks.size() > 1)
                    m_chunks.erase(it);
                break;
            }
        }
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args)
    {
        new(p) U(std::forward<Args>(args)...);
    };

    void destroy(T *p)
    {
        p->~T();
    }

    static ReserveAllocator select_on_container_copy_construction()
    {
        return ReserveAllocator<T, ChunkSize>();
    }

private:
    class Chunk
    {
    public:
        ~Chunk()
        {
            std::free(mem);
        }

        void init()
        {
            mem = reinterpret_cast<T *>(std::calloc(ChunkSize, sizeof(T)));
            if (!mem)
                throw std::bad_alloc();
        }

        bool canAllocate(std::size_t n)
        {
            return ChunkSize - allocated >= n;
        }

        bool isEmpty()
        {
            return allocated == 0;
        }

        T *allocate(std::size_t n)
        {
            T *ptr = mem + pos;
            pos += n;
            allocated += n;
            return ptr;
        }

        bool tryDeallocate(T *p, std::size_t n)
        {
            if (p >= mem && p <= mem + ChunkSize)
            {
                allocated -= n;
                return true;
            }

            return false;
        }

    private:
        T *mem = nullptr;
        std::size_t pos = 0;
        std::size_t allocated = 0;
    };

    std::list<Chunk> m_chunks;

    void AllocateNewChunk()
    {
        Chunk c;
        c.init();
        m_chunks.push_back(c);
    }
};
