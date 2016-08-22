#include "hello.h"
#include <iostream>


using namespace std;

Hello::Hello()
{
    this->name = "Yoppy Yunhasnawa";
}

void Hello::say_hello()
{
    string message = "Hello, " + this->name + ".. Have nice day!";

    cout << message << "\n";
}
