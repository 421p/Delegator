//    BSD 3-Clause License
//
//    Copyright (c) 2016 Andrii Bakal
//    All rights reserved.
//
//    Redistribution and use in source and binary forms, with or without
//        modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
//    * Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//        IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//        FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//        DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//        SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//        CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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