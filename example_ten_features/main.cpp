#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;


// ---------------------------------------------
template <typename T, typename E>
auto compose(T a, E b) -> decltype(a+b) // http://en.cppreference.com/w/cpp/language/decltype
{
    return a + b;
}
// ---------------------------------------------
class BaseClass
{

public:
    virtual void foo() const
    { cout << "BaseClass::foo()" << endl; }
    virtual void foo(int a) const
    { cout << "BaseClass::foo(int a) with a=" << a << endl; }

    //    void foo_compile_err(string a) const
    //    { cout << "BaseClass::foo_compile_err(int a) with a=" << a << endl; }

    virtual void foo_final(string a) const final
    { cout << "BaseClass::foo_final(string a) with a=" << a << endl; }
};
class DerivedClass : public BaseClass
{

public:
    virtual void foo() const override
    { cout << "DerivedClass::foo()" << endl; }
    virtual void foo(int a) const override
    { cout << "DerivedClass::foo(int a) with a=" << a << endl; }

    //    void foo_compile_err(string a) const override
    //    { cout << "DerivedClass::foo_compile_err(int a) with a=" << a << endl; }

    //    virtual void foo_final(string a) const
    //    { cout << "DerivedClass::foo_final_compile_error(string a) with a=" << a << endl; }
};
// ---------------------------------------------
struct Foo
{
    Foo(double bb=10.0) : b(bb) { std::cout << "Foo::Foo\n"; }
    ~Foo()     { std::cout << "Foo::~Foo\n"; b=-1.0; }
    void bar() { std::cout << "Foo::bar, b=" << b << "\n";  }
    double b;
};
void f(const Foo & f)
{
    std::cout << "f(const Foo&) : b= "<< f.b << "\n";
}
void old_style_foo(const Foo * f)
{
    std::cout << "old_style_foo(const Foo*) : b= "<< f->b << "\n";
}
void new_style_foo(const unique_ptr<Foo> &f)
{
    std::cout << "new_style_foo(const Foo*) : b= "<< f->b << "\n";
}
void new_style_foo(const shared_ptr<Foo> &f)
{
    std::cout << "new_style_foo(const Foo*) : b= "<< f->b << "\n";
}


// ---------------------------------------------



int main()
{
    cout << "Hello C++11 World!" << endl;

    // ----------------
    cout << "keyword 'auto'" << endl;
    // ----------------
    map<char, vector<int>> map;
    map['a'].push_back(0);
    map['a'].push_back(1);
    map['a'].push_back(2);
    map['b'].push_back(12);
    map['b'].push_back(11);
    map['c'].push_back(22);
    map['c'].push_back(24);
    map['c'].push_back(25);
    map['c'].push_back(26);

    {
        auto l = 42LL;

        for (auto it=map.begin(); it != map.end(); ++it)
        {
            cout << "'"<< it->first <<"'=[";
            for (auto itt=it->second.begin(); itt!=it->second.end(); ++itt)
            {
                cout << *itt << " ";
            }
            cout << "]" <<endl;
        }

        auto res = compose(2.4, 1);
        cout << "res= " << res << endl;


    }
    // ----------------
    cout << "range-based loops" << endl;
    // ----------------
    {

        for (const auto &kvp: map)
        {
            cout << "'"<< kvp.first <<"'=[";
            for (auto v: kvp.second)
            {
                cout << v << " ";
            }
            cout << "]" <<endl;
        }

        int arr[] = {1,2,3,4,5,6};

        for (auto & item: arr)
        {
            item *= item;
        }

        cout << "array=[";
        for (const auto &item: arr)
        {
            cout << item << " ";
        }
        cout << "]" << endl;


    }


    // ----------------
    cout << "Override and final" << endl;
    // ----------------
    {

        BaseClass * b = new BaseClass();
        BaseClass * d = new DerivedClass();

        b->foo();
        b->foo(1);
        b->foo_final("abcd");

        d->foo();
        d->foo(1);
        d->foo_final("dbca");

    }



    // ----------------
    cout << "Smart pointers" << endl;
    // ----------------

    // ----------
    // unique_ptr
    // ----------

    { // Define the scope

        unique_ptr<Foo> p1(new Foo());

        if (p1) p1->bar();

        new_style_foo(p1);

        { // Define another scope
            // Move one to another
            unique_ptr<Foo> p2 = std::move(p1);
            f(*p2);
            old_style_foo(p2.get());

        }

        // Check that it is null
        cout << "p1 == null : " << (p1.get() == nullptr) << endl;
        // This wont be called
        if (p1) p1->bar();

    }

    // ----------
    // shared_ptr
    // ----------

    { // Define the scope

        auto p1 = make_shared<Foo>(12345.0);

        if (p1) p1->bar();

        new_style_foo(p1);

        { // Define another scope
            auto p2 = p1;
            f(*p2);
            old_style_foo(p2.get());
        }

        cout << "p1 = " << p1.get() << endl;
        // This will be called
        if (p1) p1->bar();

    }

    // ----------
    // weak_ptr
    // ----------


    {
        // To track object
        auto p = make_shared<Foo>(23456.0);
        weak_ptr<Foo> tracker = p;

        if (tracker.expired())
            cout << "Pointer is null" << endl;

        // delete object
        p.reset();

        if (tracker.expired())
            cout << "Pointer is null" << endl;

    }




    // ----------------
    cout << "Lamdba functions" << endl;
    // ----------------
    {

        // in for_each
        for_each(begin(map), end(map), [](auto item) {
            cout << item.first << " : ";
            for_each(begin(item.second), end(item.second), [](int v) {
                cout << v << " ";
            });
            cout << endl;
        });

        // With external variables:
        {
            const double pi2 = 6.28;
            Foo fobj(123.4);
            auto foo = [&pi2, &fobj] (double param) {
                return pi2 * param / fobj.b;
            };
            cout << foo(123.4) << endl;
        }

        // With find, find_if
        vector<int> fib{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55};

        auto res = find(begin(fib), end(fib), 8);
        if (res != end(fib))
        {
            cout << "Found value : " << *res << endl;
        }

        res = find_if(begin(fib), end(fib), [](int v) { return v > 8; });
        if (res != end(fib))
        {
            cout << "Found value > 8 : " << *res << endl;
        }

        // with std::function
        function<int(int)> fib_next = [&fib_next](int index){ return index < 2 ? 1 : fib_next(index-1) + fib_next(index-2); };
        cout << fib_next(20) << endl;


    }

    // ----------------
    cout << "static assert" << endl;
    // ----------------
    {

        auto add_func = [](auto a, auto b){
            static_assert(is_integral<decltype(a)>::value || is_floating_point<decltype(a)>::value, "First argument should be a number");
            static_assert(is_integral<decltype(b)>::value || is_floating_point<decltype(b)>::value, "Second argument should be a number");
            return a + b;
        };

        cout << add_func(1, 2) << endl;
        cout << add_func(1.2, 2.3) << endl;
        // compile error
        // cout << add_func("a", 3) << endl;


    }


    // ----------------
    cout << "rvalue & move" << endl;
    // ----------------
    {
        vector<string> v;
        auto create_string = []() -> string {
            return string("asjkdbaksjdbka");
        };

        v.push_back(move(create_string()));
        v.push_back(move(create_string()));
        v.push_back(move(create_string()));

    }




    // ----------------
    cout << "Multi-threading" << endl;
    // ----------------
    {
        auto sleep_foo = [](int delay) {
            cout << "Thread id : " << this_thread::get_id() << endl;
            this_thread::sleep_for(chrono::milliseconds(delay));
        };

        cout << "Start thread : ";
        auto t0 = chrono::high_resolution_clock::now();
        thread t(sleep_foo, 2000);
        t.join();
        auto elapsed = chrono::high_resolution_clock::now() - t0;
        cout << "END : " << chrono::duration<float>(elapsed).count() << endl;

    }

    return 0;
}

