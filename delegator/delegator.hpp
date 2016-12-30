#ifndef __DELEGATOR__DEFINED
#define __DELEGATOR__DEFINED

#include <list>
#include <memory>

namespace delegator {

template<typename ...T>
using Delegate = std::function<void(T...)>;

using DelegateVoid = std::function<void()>;

template<class ...T, class Func>
std::shared_ptr<Delegate<T...>> make_delegate(Func callable)
{
    return std::make_shared<Delegate<T...>>(callable);
}

template<class ...T>
class Subject {
  private:
    std::unique_ptr<std::list<std::shared_ptr<Delegate<T...>>>> _listeners;
    std::unique_ptr<std::list<std::shared_ptr<Delegate<T...>>>> _onceListeners;
  public:

    Subject() : _listeners(new std::list<std::shared_ptr<Delegate<T...>>>),
                _onceListeners(new std::list<std::shared_ptr<Delegate<T...>>>)
    { }

    inline void once(std::shared_ptr<Delegate<T...>> callable)
    {
        _onceListeners->push_back(callable);
    }

    inline void subscribe(std::shared_ptr<Delegate<T...>> callable)
    {
        _listeners->push_back(callable);
    }

    inline void unsubscribe(std::shared_ptr<Delegate<T...>> callable)
    {
        _listeners->remove(callable);
    }

    inline void terminate_listeners()
    {
        _listeners->clear();
    }

    inline void publish(T... args)
    {
        for (auto &&closure : *_listeners) {
            closure->operator()(args...);
        }

        if (!_onceListeners->empty()) {
            for (auto &&closure : *_onceListeners) {
                closure->operator()(args...);
            }

            _onceListeners->clear();
        }
    }

    inline void operator&=(std::shared_ptr<Delegate<T...>> callable)
    {
        once(std::move(callable));
    }

    inline void operator+=(std::shared_ptr<Delegate<T...>> callable)
    {
        subscribe(std::move(callable));
    }

    inline void operator-=(std::shared_ptr<Delegate<T...>> callable)
    {
        unsubscribe(std::move(callable));
    }
};

template<>
class Subject<void> {
  private:
    std::unique_ptr<std::list<std::shared_ptr<DelegateVoid>>> _listeners;
    std::unique_ptr<std::list<std::shared_ptr<DelegateVoid>>> _onceListeners;
  public:

    Subject() : _listeners(new std::list<std::shared_ptr<DelegateVoid>>),
                _onceListeners(new std::list<std::shared_ptr<DelegateVoid>>)
    { }

    inline void once(std::shared_ptr<DelegateVoid> callable)
    {
        _onceListeners->push_back(callable);
    }

    inline void subscribe(std::shared_ptr<DelegateVoid> callable)
    {
        _listeners->push_back(callable);
    }

    inline void unsubscribe(std::shared_ptr<DelegateVoid> callable)
    {
        _listeners->remove(callable);
    }

    inline void terminate_listeners()
    {
        _listeners->clear();
    }

    inline void publish()
    {
        for (auto &&closure : *_listeners) {
            closure->operator()();
        }

        if (!_onceListeners->empty()) {
            for (auto &&closure : *_onceListeners) {
                closure->operator()();
            }

            _onceListeners->clear();
        }
    }

    inline void operator&=(std::shared_ptr<DelegateVoid> callable)
    {
        once(std::move(callable));
    }

    inline void operator+=(std::shared_ptr<DelegateVoid> callable)
    {
        subscribe(std::move(callable));
    }

    inline void operator-=(std::shared_ptr<DelegateVoid> callable)
    {
        unsubscribe(std::move(callable));
    }
};

}

#endif