#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm> // for count()
#include <memory>    // for unique_ptr

/* 
    Задача 2.
    Создайте лямбда-выражение, принимающее на вход std::vector<int>, 
    в котором содержатся числа, которые могут повторяться. 
    Внутри лямбды создайте unordered_set и при помощи него определяйте, 
    сколько раз текущее число вам встречалось раньше. 
    В результате верните std::unique_ptr<std::vector<int>>, содержащий числа без повторов. 
    При проходе по контейнеру используйте новый формат цикла for и ключевое слово auto. 
*/

int main()
{
    auto printVector 
    { [](std::vector<int> v) 
        {
            for (auto &el : v )
            {
                std::cout << el << ", ";
            }
            std::cout << std::endl;
        }
    };
    
    auto makeUnicVector 
    { [](std::vector<int> vecIn)
        { 
            std::unordered_set<int> unicSet {};
            std::vector<int> unicVec {};

            for (auto &el : vecIn )
            {
                unicSet.insert(el);
            }
            
            for (auto &el : unicSet )
            {
                std::cout << "Number " << el 
                          << " occurs in the input vector " 
                          << std::count(std::begin( vecIn ), std::end( vecIn ), el)
                          << " times.\n";
                unicVec.push_back(el);
            }
            
            return  std::unique_ptr<std::vector<int>> {std::make_unique<std::vector<int>>(unicVec)};
        }
    };

    std::vector<int>inputVector {33,22,11,33,44,55,66,33,77,44};
    std::cout << "Input values:\n";
    printVector(inputVector);

    auto p = makeUnicVector(inputVector);
    
    std::cout << "Unique values:\n";
    printVector(*p);

    return 0;
}