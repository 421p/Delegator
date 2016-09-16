#ifndef __DELEGATOR__SUBJECT__DEFINED
#define __DELEGATOR__SUBJECT__DEFINED

#include "delegate.hpp"

namespace delegator {

    using namespace std;

    template<class ...T>
    class Subject {
    private:
        vector<delegate<T...> *> _listeners;
    public:
        inline void subscribe(delegate<T...> &callable) {
            _listeners.push_back(&callable);
        }

        inline void unsubscribe(delegate<T...> &callable) {
            for (auto i = _listeners.begin(); i != _listeners.end(); ++i) {

                if ((unsigned long long) (&callable) == (unsigned long long) (*i)) {
                    _listeners.erase(i);
                    return;
                }
            }
        }

        inline void terminate_listeners() {
            _listeners.clear();
        }

        inline void publish(T... args) {
            for (auto closure : _listeners) {
                closure->invoke(args...);
            }
        }

        inline void operator+=(delegate<T...> &callable) {
            subscribe(callable);
        }

        inline void operator-=(delegate<T...> &callable) {
            unsubscribe(callable);
        }
    };
}

#endif