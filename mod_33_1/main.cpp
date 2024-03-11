#include <iostream>
#include <string>
#include <map>
/*
Корзина магазина

Реализуйте простую и безопасную модель работы корзины онлайн-магазина.

    В начале программы вы заполняете базу данных самого магазина через стандартную консоль. 
    Каждый элемент этой базы данных состоит из двух компонентов — артикула и количества штук.
    Корзина должна поддерживать следующие операции: 
    добавление товара с указанным его количеством — add, 
    удаление товара с указанным количеством — remove. 
    Обе операции принимают артикул товара в виде строки и количество в виде целого числа.
    При вводе аргументов к операциям должна осуществляться их валидация. 
    Артикул должен быть в базе данных магазина, 
    количество не должно превышать количество доступного продукта 
    на складе (при удалении — в корзине).
*/

std::map<std::string, int> goods 
{
    {"apples", 130},
    {"tomatoes", 250},
    {"potatoes", 45},
    {"carrots", 30},
    {"tangerines", 250}
};

std::map<std::string, int> cart {};

void addGoods( std::string, int);
void removeGoods( std::string, int);
void pause()
{
    std::cin.clear();
    std::cin.ignore(323232, '\n');
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();    
}

class cartException
{
public:
  
    const std::string event() const noexcept 
    {
        if(status == 1) // Нет такого товара на полке
        {
            return "** There is no " + msg + " on the shelf. **";
        }
        else if (status == 2)
        {
            return "** Insufficient quantity of " + msg + " on the shelf. **";
        }
        else if(status == 3) // Нет такого товара на полке
        {
            return "** There is no " + msg + " in the cart. **";
        }
        else if (status == 4)
        {
            return "** Insufficient quantity of " + msg + " in the cart. **";
        }
        return "** Something went wrong. **";
    }
    cartException(int x, std::string s) : status(x), msg(s) {}
private:
    int status;
    std::string msg; 
};

int main()
{
    std::string sample {};  // Пример ввода команды

    while(true)
    {
        std::cout << "======== Shelf With Goods ========\n";
        bool empty = true;
        for ( auto& [key,val] : goods)
        {
            if (val > 0)
            {
                std::cout << "  " << key << "\t" << val << "\n";
                empty = false;
            }
        }
        if (empty)
        {
            std::cout << "  The shelf is empty\n";
        }

        std::cout << "============== Cart ==============\n";
        if (cart.empty())
        {
            std::cout << "  The cart is empty\n";
        }
        else
        {
            for ( auto& [key,val] : cart)
            {
                std::cout << "  " << key << "\t" << val << "\n";
            }
        }
        std::cout << "------------ Commands -------------\n";
        std::cout << " A - add; R - remove; Q - Exit\n";
        std::cout << "-----------------------------------\n";
        std::cout << " Command-> ";
        char cmd;
        std::cin >> cmd;
        std::cin.clear();
        std::cin.ignore(323232, '\n');
        if ( cmd == 'Q' || cmd == 'q')
        {
            std::cout << "Exiting..." << std::endl;
            break;            
        }
        if ( cmd != 'A' && cmd != 'a' && cmd != 'R' && cmd != 'r' )
        {
            std::cout << "*** Unknown command: " << cmd << " ***" << std::endl;
            continue;
        }
        if ( (cmd == 'R' || cmd == 'r' ) && cart.empty())
        {
            std::cout << "** The cart is empty. Nothing to remove. **" << std::endl;
            pause();
            continue;
        }

        std::cout << "------------ Commands -------------\n";
        std::cout << " Enter title of good and quantity.\n";
        std::cout << sample;
        std::cout << "-----------------------------------\n";
        std::cout << " Command-> ";
        
        sample.clear();
        std::string good;
        int quantity;
        std::cin >> std::ws >> good >> quantity;
        
        if (std::cin.fail() || std::cin.peek() != '\n')
        {
            std::cerr << "******* Invalid data input *******\n";
            sample = " Example: potatoes 15\n";
            pause();
            continue;
        }

        if (cmd == 'A' || cmd == 'a')
        {
            try 
            {
                addGoods( good, quantity);
            }
            catch (const cartException& x)
            {
                std::cerr << x.event() << std::endl;
                pause();
            }
        }
        else
        {
            try 
            {
                removeGoods( good, quantity);
            }
            catch (const cartException& x)
            {
                std::cerr << x.event() << std::endl;
                pause();
            }
        }
        
    }
  
    return 0;
}

void addGoods( std::string good, int quantity)
{
    if (!goods.count(good))
    {
        throw  cartException(1, good);      // Нет такого товара
    }
    else if ( goods[good] - quantity < 0)
    {
        throw  cartException(2, good);      // Неправильное количество товара
    }
    goods[good] -= quantity;
    if (cart.count(good)) cart[good] += quantity;
    else cart[good] = quantity;
}
void removeGoods( std::string good, int quantity)
{
    if (!cart.count(good))
    {
        throw  cartException(3, good);      // Нет такого товара
    }
    else if ( cart[good] - quantity < 0)
    {
        throw  cartException(4, good);      // Неправильное количество товара
    }
    
    cart[good]  -= quantity;
    if (cart[good] == 0)
        cart.erase(good);

    goods[good] += quantity;
}