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
    Person() {}
    Person(std::string name, std::string s, std::string b, int i)
        : name(name)
        , sex(s)
        , birth(b)
        , income(i) { std::cout << "constructed\n"; }
    Person(Person&& p)
        : name(std::move(p.name))
        , sex(std::move(p.sex))
        , birth(std::move(p.birth))
        , income(std::move(p.income)) { std::cout << "move constructed\n"; }
    Person& operator=(const Person& p) = default;
    Person& operator=(Person&& p) {
        name = std::move(p.name);
        sex = std::move(p.sex);
        birth = std::move(p.birth);
        income = std::move(p.income);
        std::cout << "move constructed\n";
        return *this;
    }
    ~Person() {}
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        std::cout << p.name << ", " << p.sex << ", " << p.birth << ", "
                  << p.income << "  ";
        return os;
    }
};

int main(int argc, char *argv[])
{
    iArray arr  {1, 2};
    iArray arr2 {2, 3, 4, 5, 6, 7};
    dArray darr {1., 2., 4., 8., 16., 32.};
    iArray arr3 = std::move(darr);
    arr = arr2;
    std::cout << darr << '\n';

    // test range based for loop
    for (const auto& elem : arr3 + arr2) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';

    // test std algorithm
    arr3[2] = 81;
    arr3.emplace(1, 25);
    std::for_each(arr3.begin(), arr3.end(), [](auto& elem) {
        std::cout << std::sqrt(elem) << ' ';
    });
    std::cout << '\n';
    std::cout << arr2 << '\n';

    ////////////////////////////////////////////////////////////////////
    // custom class array
    ////////////////////////////////////////////////////////////////////
    TArray<Person> parray {
        {"Alice", "female", "1992/2/12", 75000},
        {"Bob", "male", "1994/12/2", 73000},
        {}
    };
    std::string tmpBirth = "1996/5/12";
    parray.emplace(2, "Celina", "female", tmpBirth, 5000);
    std::cout << parray << '\n';
    std::cout << "tmpBirth = " << tmpBirth << '\n';
    std::cout << parray[0].birth << '\n';

    ////////////////////////////////////////////////////////////////////
    // Copy on write array
    ////////////////////////////////////////////////////////////////////
    std::cout << "\n";
    std::cout << "COMArray:\n";

    COWArray<int> cowarr {1, 2, 4, 8, 16};
    std::cout << cowarr << '\n';
    
    COWArray<int> cowarr2 = cowarr;
    const COWArray<int>& cowarrRef = cowarr; 
    
    std::cout << cowarr.own() << '\n';    
    cowarrRef[2];
    std::cout << cowarr.own() << '\n';    
    
    // std::cout << cowarr.getaddr() << " | " << cowarr2.getaddr() << '\n';
    // std::cout << cowarr2.at(2) << ' ';
    // std::cout << cowarr.own() << '\n';

    // cowarr2[2] = 200;
    // std::cout << cowarr.getaddr() << " | " << cowarr2.getaddr() << '\n';
    // std::cout << cowarr.own() << '\n';
    // std::cout << cowarr2 << '\n';

    // COWArray<int> cowarr3 = std::move(cowarr2);
    // std::cout << cowarr2.getaddr() << " | " << cowarr3.getaddr() << '\n';
    // std::cout << cowarr2 << " | " << cowarr2.own() << '\n';
    // std::cout << cowarr3 << '\n';

    return 0;
}
