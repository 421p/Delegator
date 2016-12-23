#include <iostream>
#include "delegator/delegator.hpp"
#include <vector>

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
    auto closure = make_delegate<string>([](string item) {
        cout << "Item added: " << item << endl;
    });

    collection.itemAdded += closure;

    collection.push_back("cool item");
    collection.push_back("avetage item");

    collection.itemAdded -= closure;

    collection.push_back("no catches");

    collection.itemAdded.once(make_delegate<string>([](string item) {
        cout << "will work just once." << endl;
    }));

    collection.push_back("one");
    collection.push_back("two");

    return 0;
}