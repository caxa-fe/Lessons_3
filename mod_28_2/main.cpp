#include<iostream>
#include<thread>
#include<mutex>
#include<string>
#include<chrono>

using namespace std;
/*
    Очередь из поездов на вокзал
    С помощью многопоточности реализуйте простую модель железнодорожного вокзала.
    С трёх точек отправления в разное время отбывают три разных поезда 
    с условными обозначениями A, B, C. Все они идут на один и тот же вокзал 
    и могут дойти до него в разное время.

    На этом целевом вокзале единовременно может находиться только один поезд. 
    Остальные поезда должны ждать, пока уже занятое место освободится.

    В начале программы пользователь последовательно вводит для каждого поезда 
    время в пути до вокзала (в секундах). После этого поезда начинают своё движение.
    Как только поезд за заданное количество времени прибыл на вокзал, он ожидает 
    от пользователя команды GO, которая сигнализирует о его отбытии с вокзала.

    По всем событиям (прибытие на вокзал, ожидание свободного места, отбытие с вокзала) 
    вместе с условным обозначением поезда выводятся сообщения в консоль.

    Программа завершается, когда на вокзале побывают все три поезда.
*/
mutex mtx1, mtx2;
const char station = 'S';
class Train
{
    string name;
    const string status[4] { "is travelling to train station.", 
                             "is awaiting arrival to the train station.",
                             "arrived at the station, awaiting shipment.",
                             "left the station."
                           };
    int travelTime = 0;
    int currentStatus = 0;

public:
    string getStatus()
    {
        return status[currentStatus];
    }
    void setStatus(int n)
    {
        currentStatus = n;
        mtx1.lock();
            cout << "Train " << name << " " << getStatus() << endl;
        mtx1.unlock();

    }
    void setTravelTime()
    {
        while(true)
        {
            cout << " Enter a travel time (seconds) to station for train " << name << ": ";
            int t;
            cin >> t;
            if (cin.fail() || cin.peek() != '\n' || t < 1)
            {
                cerr << "*** Input Error ***\n";
                cout << " Prohibited value " << t << "\n";
                cout << " Positive integer, please." << endl;
                cin.clear();
                cin.ignore(323232, '\n');
                continue;
            } 
            travelTime = t;
            break;
        }
    }
    void setTrainName()
    {
        cout << " Enter a train name : ";
        string str;
        getline(cin >> ws, str);
        if (str.length() > 8) str.resize(8);
        name = str;
    }
    string getName()
    {
        return name;
    }
    void trainGo()
    {
        setStatus(0);
        this_thread::sleep_for(chrono::seconds(travelTime));
        setStatus(1);
        mtx2.lock();
        setStatus(2);
        while (true)
        {
            cout << "--Enter GO for dispatch.\n";
            string str;
            getline(cin >> ws, str);
            if (str == "GO" || str == "Go" || str == "go") break;
        }
        
        setStatus(3);
        mtx2.unlock();
    }
    
    Train()
    {
        setTrainName();
        setTravelTime();
    }
};

int main()
{
    cout << "------Trains Initiation---------" << endl;
    cout << "------First Train Setup---------" << endl;
    Train* A = new Train();
    cout << "-----Second Train Setup---------" << endl;
    Train* B = new Train();
    cout << "------Third Train Setup---------" << endl;
    Train* C = new Train();

    cout << "----All Trains Are Ready---------" << endl;

    cout << "--Press Enter to start." << endl;
    cin.clear();
    cin.ignore(323232, '\n');
    cin.get();
    cout << "---------- START! -----------" << endl;
    thread tA(&Train::trainGo, A);
    thread tB(&Train::trainGo, B);
    thread tC(&Train::trainGo, C);
    tA.join();
    tB.join();
    tC.join();

}