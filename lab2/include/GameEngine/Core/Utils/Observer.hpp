#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Logger.hpp"

struct Event
{
    const int tag;
    Event(const int &tag = 0) : tag(tag) {}
    virtual ~Event() = default;
};

template <typename T>
class Observer : public std::enable_shared_from_this<Observer<T>>
{
public:
    virtual ~Observer() = default;
    virtual void OnNotify(const T &event) = 0;
    
    // Удобный метод для получения weak_ptr
    std::weak_ptr<Observer<T>> getWeakPtr()
    {
        return this->shared_from_this();
    }
};

// Наблюдаемый объект
template <typename T>
class Subject
{
private:
    std::vector<std::weak_ptr<Observer<T>>> observers;

public:
    virtual ~Subject() = default;

    void AddObserver(std::weak_ptr<Observer<T>> observer)
    {
        observers.push_back(observer);
    }
    
    void AddObserver(std::shared_ptr<Observer<T>> observer)
    {
        observers.push_back(observer);
    }

    void RemoveObserver(std::weak_ptr<Observer<T>> observer)
    {
        auto target = observer.lock();
        if (!target) return; // Указатель уже невалиден
        
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [&target](const std::weak_ptr<Observer<T>>& weakObs) {
                    auto obs = weakObs.lock();
                    return !obs || obs == target;
                }),
            observers.end()
        );
    }
    
    void RemoveObserver(std::shared_ptr<Observer<T>> observer)
    {
        RemoveObserver(std::weak_ptr<Observer<T>>(observer));
    }

    void RefreshObservers()
    {
        // Удаляем всех несуществующих наблюдателей
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [](const std::weak_ptr<Observer<T>>& weakObs) {
                    return weakObs.expired(); // Проверяем истек ли weak_ptr
                }),
            observers.end()
        );
    }

protected:
    void Notify(const T &event)
    {
        // Обновляем список перед уведомлением
        //RefreshObservers();
        
        // Создаем копию weak_ptr для безопасности
        auto observersCopy = observers;
        
        for (auto& weakObserver : observersCopy)
        {
            auto observer = weakObserver.lock();
            if (observer)
            {
                observer->OnNotify(event);
            }
        }
        
        // После уведомления можно снова очистить невалидные
        //RefreshObservers();
    }
};