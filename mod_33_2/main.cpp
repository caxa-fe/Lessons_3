#include <iostream>
#include <string>
#include <ctime>

/*
    Рыбалка

Реализуйте простую игру-симуляцию ловли рыбы. 
Игровое поле представляет собой девять секторов условного пруда. 
В начале игры в одном из этих секторов с помощью 
генератора случайных чисел оказывается рыба. 
Точно так же, случайным образом, 
по секторам пруда распределяются и три сапога. 
Сапог и рыба при этом не могут находиться в одном 
и том же секторе одновременно, 
как и несколько сапог сразу в одном из секторов.

Игрок закидывает удочку с приманкой в один из секторов, 
номер которого вводится стандартным способом через консоль. 
Задача игрока — угадать сектор, в котором находится рыба и, таким образом, 
поймать её. Если игрок попал на пустой сектор, то удочку надо забросить повторно. 
Если игрок поймал сапог, то игра завершается с отрицательным результатом.

Реализовать задачу нужно с помощью исключений: 
когда успешный заброс удочки генерирует специальное пользовательское исключение, 
после чего программа должна завершиться и оповестить пользователя об успешной рыбалке 
и количестве попыток, которое ему для этого потребовалось. 
Если же был пойман сапог, то должно выводиться сообщение о неудачной ловле.

*/

void shuffle(char box[]);
void waiting (int t);
void fishing(int num, char pound[]);
void pause();

class Fish {};
class Boot {};

int main()
{
    char pound[9] {'f','b','b','b','e','e','e','e','e'};
    // В пруду pound 9 секторов, в которых: f - fish, b - boot, e - empty
    // Тусуем массив
    shuffle(pound);

/*     for( char ch : pound )
    {
        std::cout << ch << ", ";
    }
    std::cout << std::endl; */
    
    // Начинаем рыбачить
    while (true) 
    {
        std::cout << "--- Let's Catch Fish Big and Small ---\n";
        std::cout << "  Enter a your lucky number: ";
        int luckyNum;
        std::cin >> luckyNum;
        if (std::cin.fail() || std::cin.peek() != '\n')
        {
            std::cerr << "******* Invalid data input *******\n";
            pause();
            continue;
        }
        std::cout << "  Casting the rod..." << std::endl;
        waiting(3);
        try
        {
            fishing(luckyNum, pound);
        }
        catch(const Fish& e)
        {
            std::cout << "  Oh, yes! You catch a big fish! Let's go home.\n" << std::endl;
            break;
        }
        catch(const Boot& e)
        {
            std::cout << "  WHAT? A boot? Ahahahaha! Lot Of Laughs!" << std::endl;
            pause();
        }
        catch( ... )
        {
            std::cout << "  You catch noting. Fortune give you a smile a next time!" << std::endl;
            pause();
        }
    }

    return 0;
}

void fishing(int num, char pound[])
{
    if(num < 0)
        num *= -1;
    if (num < 10)
        num *= 10;
    num = num % 9;

    if( pound[num] == 'f')
    {
        throw Fish();
    }
    if( pound[num] == 'b')
    {
        pound[num] = 'e';
        throw Boot();
    }

    throw int();
}

void shuffle(char box[])
{
    std::srand((unsigned)std::time(nullptr));
    for (int i = 0; i < 9; i++)
    {
        int j = rand() * 100 % 9;
        std::swap(box[i], box[j]);
    }
}
void waiting (int t)    // t - минимальное время ожидания
{
    std::cout << "  Waiting...";
    int now = (int)std::time(nullptr);
    srand( (unsigned)now) ;
    t += rand() * 10 % 6; // Рандомизируем время ожидания
    int timeTo = now + t;
    while (std::time(nullptr) < timeTo)
    {
        if(std::time(nullptr) > now)
        {
            std::cout << ".";
            ++now;
        }
    }
    std::cout << std::endl;
}
void pause()
{
    std::cin.clear();
    std::cin.ignore(323232, '\n');
    std::cout << "  Press Enter to continue..." << std::endl;
    std::cin.get();    
}