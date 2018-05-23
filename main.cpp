#include <algorithm>
#include <cmath>
#include "Array.h"
#include "COWArray.h"

template<typename T> struct TD;
// TD<decltype(var)> td;

struct Person {
    std::string name;
    std::string sex;
    std::string birth;
    int income;
    Person(std::string n, std::string s, std::string b, int i)
        : name(name)
        , sex(s)
        , birth(b)
        , income(i)
    {}
};

int main(int argc, char *argv[])
{
    iArray arr  {1, 2};
    iArray arr2 {2, 3, 4, 5, 6, 7};
    dArray darr {1., 2., 4., 8., 16., 32.};
    iArray darr3 = std::move(darr);
    const dArray arr3 = arr2;

    TArray<Person> parray {
        {"Alice", "female", "1992/2/12", 15000},
        {"Bob", "maLe", "1994/12/2", 3000},
    };
    parray.emplace(1, "GOOD", "Celina", "female", "1996/5/12", 5000);

    // test range based for loop
    for (const auto& elem : darr3 + arr2) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';

    // test std algorithm
    darr3[2] = 81;
    std::for_each(darr3.begin(), darr3.end(), [](auto& elem) {
        std::cout << std::sqrt(elem) << ' ';
    });
    std::cout << '\n';
    std::cout << arr2 << '\n';

    ////////////////////////////////////////////////////////////////////
    // Copy on write array
    ////////////////////////////////////////////////////////////////////
    std::cout << "\n";
    std::cout << "COMArray:\n";

    COWArray<int> cowarr {1, 2, 4, 8, 16};
    std::cout << cowarr << '\n';
    COWArray<int> cowarr2 = cowarr;

    cowarr2[2] = 200;
    std::cout << cowarr.own() << '\n';
    std::cout << cowarr2 << '\n';

    COWArray<int> cowarr3 = std::move(cowarr2);
    std::cout << cowarr2 << " | " << cowarr2.own() << '\n';
    std::cout << cowarr3 << '\n';

    return 0;
}
