# Delegator
Another c++ event dispatching library

now in c# style

quick example:

```c++
#include "delegator/subject.hpp"
#include "delegator/delegate.hpp"

using namespace std;
using namespace delegator;

template<class T>
class observableCollection : vector<T> {
public:
    Subject<T> itemAdded;

    void push_back(T item) {
        vector<T>::push_back(item);
        itemAdded.publish(item);
    }
};

int main() {

    observableCollection<string> collection;
    delegate<string> closure;
    closure = [](string item) {
        cout << "Item added: " << item << endl;
    };

    collection.itemAdded += closure;

    collection.push_back("cool item");
    collection.push_back("avetage item");

    return 0;
}

```
