#ifndef __DELEGATOR__DELEGATE__DEFINED
#define __DELEGATOR__DELEGATE__DEFINED

#include <iostream>
#include <memory>
#include <vector>

namespace delegator {

    using namespace std;

    template<class ...T>
    class delegate {
    private:
        function<void(T...)> _closure;
    public:
        delegate() : _closure(nullptr) {}

        delegate(const function<void(T...)> &&f) : _closure(f) {}

        delegate<T...> &operator=(const function<void(T...)> &&f) {
            _closure = f;
            return *this;
        }

        inline void operator()(T... args) {
            invoke();
        }

        inline void invoke(T... args) {
            if (_closure == nullptr) {
                throw logic_error("no callback set");
            }
            _closure(args...);
        }
    };
}

#endif