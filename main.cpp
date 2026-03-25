#include <bits/stdc++.h>

class square
{
protected:
    int a = 0;

public:
    square(int a){ this->a = a; }

    virtual int P() {return a*a; }
    virtual int print(){ std::cout << a << ' ' << P() << '\n'; return 0;}
};

class rect: public square
{
protected:
    int b = 0;

public:
    rect(int a, int b): square(a) {this->b = b; }

    int P(){return a*b; }
    int print() { std::cout << a << ' ' << b << ' '<< P() << '\n'; return 0; }
};

int main()
{
    square sq(5);
    std::cout << sq.P() << '\n';
    sq.print();

    rect re(5, 8);
    std::cout << re.P() << '\n';
    re.print();
}
