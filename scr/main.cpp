#include <iostream>
#include "forward_list.h"
#include <forward_list>
int main() {
    std::cout << "Hello, world!\n";


    htp::forward_list<int> lst;
    //в этом случае сделал исключение
    //lst.insert_after(lst.begin(), 4);
    // 
    //lst.pop_front();
    lst.push_front(1);
    lst.insert_after(lst.begin(), 4);

    lst.push_front(2);
    lst.push_front(3);
    //в этом случае сделал исключение
    //lst.insert_after(lst.end(), 4);
   
    for (const auto& c : lst) {
        std::cout << c << "\t";
    }
    std::cout << "\n";
    lst.erase_after(lst.begin());
    lst.pop_front();
    for (const auto& c : lst) {
        std::cout << c << "\t";
    }
    std::cout << "\n";
    std::cout << *lst.find(4) << "\n";


    // нет такого - исключение
    //std::cout << *lst.find(5) << "\n";
    


}

   
