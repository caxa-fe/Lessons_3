#include<iostream>
#include<iomanip>
#include<thread>
#include<chrono>
#include<string>
#include<vector>
#include<mutex>
using namespace std;

/*
  Реализуйте симуляцию состязаний по заплыву на 100 метров.
    Всего шесть дорожек и шесть пловцов. 
    Имена пловцов указываются вначале из стандартного ввода. 
    Каждый из них плывёт с разной скоростью, 
    которая также задаётся пользователем при старте, 
    через стандартный ввод в метрах в секунду.

    После старта каждую секунду в стандартный вывод 
    выдаётся информация о том, сколько проплыл тот или иной пловец.

    Как только все пловцы коснулись 100-метровой отметки, 
    заплыв заканчивается и выводится таблица с итоговыми результатами, 
    отсортированная по возрастанию итогового времени заплыва.
*/

//mutex messages_access;
vector<string> finResult {};
vector<string> names {};
double speeds[6] {0};
const double DISTANCE = 100;
const     int DELTA_T = 10; // Milliseconds

mutex mtx;
void swim( unsigned n)
{
    double path = 0;
    double time = 0;
    int interval = 0;
    while (path < DISTANCE)
    {
        this_thread::sleep_for( chrono::milliseconds(DELTA_T) );
        time += (double)DELTA_T * 0.001;
        path = speeds[n] * time;
        ++interval; 
        mtx.lock();
        if ( interval == 100 || path >= DISTANCE)
        {   
            cout <<  time << " seconds: " << names[n] << " swam " 
                 <<  path << " meters." <<  endl;
            interval = 0;
        }
        mtx.unlock();
    }
    finResult.push_back(names[n] + "\t" + to_string(time) + " sec.");
}

int main()
{
    cout << "------- On Your Mark! --------\n";
    for ( int i = 0; i < 6; ++i)
    {
        cout << "Swimmer#" << i + 1 << " name: ";
        string str;
        getline(cin >> ws, str);
        if (str.length() > 8) str.resize(8);
        names.push_back(str);
        double d = 0;
        while (true)
        {
            cout << "Enter a speed for " << str << " m/sec: ";
            cin >> d;
            if ( cin.fail() || cin.peek() != '\n' || d < 0)
            {
                cerr << "*** Input Error ***\n";
                cout << "Prohibited speed value " << d << endl;
                cin.clear();
                cin.ignore(323232, '\n');
                continue;
            }
            cin.clear();
            cin.ignore(323232, '\n');
            break;
        }
        speeds[i] = d;

    }
    cout << "--------- Attention! ---------\n";
    cout << "-- These swimmers are on the start line --\n";
    cout << "  " << setw(6) << "Name" << " Speed\n";
    for ( unsigned i = 0 ; i < 6; ++i)
    {
        cout << "  " << setw(6) << names[i] << " " << speeds[i] << "\n";
    }
    cout << "--Press Enter for start--" << endl;
    cin.get();
    cout << "----------- START! -----------" << endl;

    thread s0(swim, 0);
    thread s1(swim, 1);
    thread s2(swim, 2);
    thread s3(swim, 3);
    thread s4(swim, 4);
    thread s5(swim, 5);
    s0.join();
    s1.join();
    s2.join();
    s3.join();
    s4.join();
    s5.join();
    cout << "----------- FINISH! -----------" << endl;
    for (unsigned i = 0; i < finResult.size(); ++i )
    {
        cout << "  " << finResult[i] << endl;
    }
    return 0;
}