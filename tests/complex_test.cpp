#include "../catch/catch.hpp"
#include "../delegator/delegator.hpp"

using namespace std;
using namespace delegator;

TEST_CASE("Item is available from callbacl", "[callback]")
{
    Subject<string> sub;

    string str;

    sub += make_delegate<string>([&str](string s) {
        str = s;
    });

    sub.publish("foo");

    REQUIRE("foo" == str);

    string a;
    int b;
    double c;
    bool d;

    Subject<string, int, double, bool> multisub;

    multisub += make_delegate<string, int, double, bool>([&](string s, int i, double q, bool p) {
        a = s;
        b = i;
        c = q;
        d = p;
    });

    multisub.publish("foo", 1488, 2.5, true);

    REQUIRE(a == "foo");
    REQUIRE(b == 1488);
    REQUIRE(c == 2.5);
    REQUIRE(d);
}

TEST_CASE("Subscribe and unsubscribe is working properly", "[sub-unsub]")
{
    Subject<void> sub;

    int incrementor = 0;

    auto delegate = make_delegate([&incrementor] {
        ++incrementor;
    });

    sub.subscribe(delegate);

    sub.publish();

    REQUIRE(incrementor == 1);

    sub.unsubscribe(delegate);
    sub.publish();

    REQUIRE(incrementor == 1);

    sub += delegate;
    sub.publish();

    REQUIRE(incrementor == 2);

    sub -= delegate;
    sub.publish();

    REQUIRE(incrementor == 2);
}

TEST_CASE("Once is working", "[once]")
{
    Subject<void> sub;

    int incrementor = 0;

    auto delegate = make_delegate([&incrementor] {
        ++incrementor;
    });

    sub.once(delegate);
    sub.publish();

    REQUIRE(incrementor == 1);

    sub.publish();

    REQUIRE(incrementor == 1);

    sub &= delegate;
    sub.publish();

    REQUIRE(incrementor == 2);

    sub.publish();

    REQUIRE(incrementor == 2);
}