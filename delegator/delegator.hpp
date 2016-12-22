#ifndef __DELEGATOR__SUBJECT__DEFINED
#define __DELEGATOR__SUBJECT__DEFINED

#include <list>
#include <memory>

namespace delegator {

template<typename ...T>
using Delegate = std::function<void(T...)>;

template<class ...T, class Callable>
std::shared_ptr<Delegate<T...>> make_delegate(Callable callable)
{
    return std::make_shared<Delegate<T...>>(callable);
}

template<class ...T>
class Subject {
  private:
    std::list <std::shared_ptr<Delegate<T...>>> _listeners;
  public:
    inline void subscribe(std::shared_ptr<Delegate<T...>> callable)
    {
        _listeners.push_back(callable);
    }

    inline void unsubscribe(std::shared_ptr<Delegate<T...>> callable)
    {
        _listeners.remove(callable);
    }

    inline void terminate_listeners()
    {
        _listeners.clear();
    }

    inline void publish(T... args)
    {
        for (auto &&closure : _listeners) {
            closure->operator()(args...);
        }
    }

    inline void operator+=(std::shared_ptr<Delegate<T...>> callable)
    {
        subscribe(callable);
    }

    inline void operator-=(std::shared_ptr<Delegate<T...>> callable)
    {
        unsubscribe(callable);
    }
};
}

#endif