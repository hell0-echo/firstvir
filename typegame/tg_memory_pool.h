/* ------------------------------------------------------------------
//  文件名    :   tg_memory_pool.h
//  创建者    :   dev@project.cn
//  创建时间  :   2024-05-20
//  功能描述  :   泛型对象内存池，零动态分配回收
// ----------------------------------------------------------------*/
#ifndef _TG_MEMORY_POOL_H_
#define _TG_MEMORY_POOL_H_

#include <vector>
#include <queue>
#include <memory>
#include "tg_poolable_base.h"

template<typename T>
class TgMemoryPool
{
    static_assert(std::is_base_of<TgPoolableBase, T>::value, "T must inherit TgPoolableBase");

public:
    explicit TgMemoryPool(int initialCapacity = 64) { m_objects.reserve(initialCapacity); }
    ~TgMemoryPool() = default;

    TgMemoryPool(const TgMemoryPool&) = delete;
    TgMemoryPool& operator=(const TgMemoryPool&) = delete;

    T* allocate()
    {
        if (m_freeList.empty()) {
            m_objects.emplace_back(std::make_unique<T>());
            m_freeList.push(m_objects.back().get());
        }
        T* obj = m_freeList.front();
        m_freeList.pop();
        obj->acquire();
        return obj;
    }

    void deallocate(T* obj)
    {
        if (obj != nullptr) {
            obj->release();
            m_freeList.push(obj);
        }
    }

    void clear()
    {
        while (!m_freeList.empty()) m_freeList.pop();
        for (auto& ptr : m_objects) {
            ptr->acquire();
            m_freeList.push(ptr.get());
        }
    }

private:
    std::vector<std::unique_ptr<T>> m_objects;
    std::queue<T*> m_freeList;
};

#endif // _TG_MEMORY_POOL_H_