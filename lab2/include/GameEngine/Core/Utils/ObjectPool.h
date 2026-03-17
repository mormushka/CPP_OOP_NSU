#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <iostream>

template <typename T>
class ObjectPool
{
private:
    std::vector<std::unique_ptr<T>> pool;
    std::vector<T *> active;

    size_t maxSize;

public:
    ObjectPool(size_t initialSize = 32, size_t maxSize = 128)
        : maxSize(maxSize)
    {
        pool.reserve(maxSize);
        for (size_t i = 0; i < initialSize; ++i)
            pool.push_back(std::make_unique<T>());
    }

    template <typename InitFunc>
    T *Acquire(InitFunc initializer)
    {
        for (auto &obj : pool)
        {
            if (!IsActive(obj.get()))
            {
                initializer(*obj);
                active.push_back(obj.get());
                return obj.get();
            }
        }

        if (pool.size() < maxSize)
        {
            pool.push_back(std::make_unique<T>());
            initializer(*pool.back());
            active.push_back(pool.back().get());
            std::cout << "Pool expanded, size = " << pool.size() << "\n";
            return pool.back().get();
        }

        std::cout << "Pool limit reached!\n";
        return nullptr;
    }

    void Release(T *obj)
    {
        active.erase(std::remove(active.begin(), active.end(), obj), active.end());
    }

    template <typename Predicate>
    void Update(Predicate isInactive)
    {
        active.erase(
            std::remove_if(active.begin(), active.end(),
                           [&](T *obj)
                           { return isInactive(obj); }),
            active.end());
    }

    const std::vector<T *> &GetActive() const { return active; }
    size_t Size() const { return pool.size(); }

private:
    bool IsActive(T *obj) const
    {
        return std::find(active.begin(), active.end(), obj) != active.end();
    }
};
