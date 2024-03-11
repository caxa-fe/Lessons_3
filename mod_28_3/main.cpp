/*
    Ресторан с доставкой

    Требуется реализовать упрощённую модель работы кухни ресторана, 
    работающего в режиме онлайн-доставки.

    Онлайн-заказ поступает каждые 5–10 секунд. Это случайное блюдо из пяти: 
    пицца, суп, стейк, салат, суши. 
    Официант, принявший заказ онлайн, оставляет его у кухни. 
    Если кухня свободна, она принимает заказ и спустя 5–15 секунд 
    возвращает уже готовое блюдо на выдачу. Время определяется случайным образом 
    с помощью функции std::rand() в указанных диапазонах.

    Курьер приезжает каждые 30 секунд, забирает готовые блюда на выдаче 
    и развозит их по заказчикам.

    Программа завершается после 10 успешных доставок. 
    На каждом из этапов, будь то заказ, или готовка, или доставка заказа, 
    выводятся соответствующие сообщения в консоль.

    Для симуляции готовки, используйте отдельный мьютекс 
    и запускайте отдельную нить ожидания с вызовом detach для неё.

    Замечание программиста.
    Сделан вывод отчетов в файлы для проверок.
    Вывод в файлы можно было поизящней, конечно...
    В задаче не требуется, поэтому для отладки, на скорую руку.
*/
#include<iostream>
#include<fstream>
#include<mutex>
#include<string>
#include<thread>
#include<vector>
#include<chrono>
#include<ctime>
using namespace std;

mutex mtxOQ, mtxRO, mtxCout;
const string MENU[6] {"Empty","pizza", "soup", "steak", "salad", "sushi"};
vector<int> OrderQueue  {};
vector<int> readyOrders {};
int finishedOrders = 0;
int startTime;

class Timer
{
    void start()
    {
        startTime = (int)time(nullptr);
    }
public:
    string getTime()
    {
        int countSeconds = (int)time(nullptr) - startTime;
        int minutes = countSeconds / 60;
        int seconds = countSeconds % 60;
        return to_string(minutes) + ":" + to_string(seconds);
    }
    string getDateTime()
    {
        time_t nowTime = time(nullptr);
        struct tm *now = localtime(&nowTime);
        string str = to_string(now->tm_year  + 1900) + "-" 
                   + to_string(now->tm_mon   + 1) + "-"
                   + to_string(now->tm_mday  + 1) + "_"
                   + to_string(now->tm_hour) + "."
                   + to_string(now->tm_min);
        return str;
    }
    Timer()
    {
        start();
    } 
};

class OrderTaker : public Timer
{
    string reportFileName {};
public:
    void createReportFile() // Засунуть бы эту штуку в конструктор, но в названии файла есть дата-время.
    {                       // Поэтому надо этот метод запускать после старта процесса
        reportFileName = "Report_OrderTaker_" + getDateTime() + ".txt";
        ifstream inFile(reportFileName.c_str() );
        if (inFile.is_open())
        {
            inFile.close();
            remove(reportFileName.c_str());
        }
        ofstream oFile(reportFileName.c_str());
        if ( !oFile.is_open())
        {
            cerr << "*** Order Taker: Report File Error ***\n";
            cout << "  Cannot create a file " << reportFileName.c_str() << endl;
        }
        else 
        {
            oFile.close();
        }
    }
    void getOrder(int ord)
    {
        OrderQueue.push_back(ord);
    }
    void saveReport(string &s)
    {
        ofstream oFile(reportFileName.c_str(), ios::app);
        if ( !oFile.is_open())
        {
            cerr << "*** Order Taker: Report File Error ***\n";
            cout << "  Cannot open a file " << reportFileName.c_str() << endl;
        }
        else 
        {
            oFile << s;
            oFile.close();
        }
    }
    void clearQueue()
    {
         OrderQueue.clear();
    }

    void orderTakerWork()
    {
        do {
            srand((unsigned)time(nullptr));
            int order = 1 + rand() % 5;
            getOrder(order);
            mtxCout.lock();
            cout << getTime() << " Order Taker got order \"" << MENU[order] << "\"" << endl;
            mtxCout.unlock();
            string str = getTime() + " Order Taker got order \"" + MENU[order] + "\"" + "\n";
            saveReport(str);
            int sec = 5 + rand() % 6;
            this_thread::sleep_for(chrono::seconds(sec));
        } while (finishedOrders < 10);
    }
};

class Kitchen : public Timer
{
    int currentOrder;
    string reportFileName {};
public:
    void createReportFile() // Засунуть бы эту штуку в конструктор, но в названии файла есть дата-время.
    {                       // Поэтому надо этот метод запускать после старта процесса
        reportFileName = "Report_Kitchen_" + getDateTime() + ".txt";
        ifstream inFile(reportFileName.c_str() );
        if (inFile.is_open())
        {
            inFile.close();
            remove(reportFileName.c_str());
        }
        ofstream oFile(reportFileName.c_str());
        if ( !oFile.is_open())
        {
            cerr << "*** Order Taker: Report File Error ***\n";
            cout << "  Cannot create a file " << reportFileName.c_str() << endl;
        }
        else 
        {
            oFile.close();
        }
    }
    void saveReport(string &s)
    {
        ofstream oFile(reportFileName.c_str(), ios::app);
        if ( !oFile.is_open())
        {
            cerr << "*** Order Taker: Report File Error ***\n";
            cout << "  Cannot open a file " << reportFileName.c_str() << endl;
        }
        else 
        {
            oFile << s;
            oFile.close();
        }
    }
    int getOrderFromQueue()
    {
        mtxOQ.lock();
            if (OrderQueue.empty()) 
            {
                mtxOQ.unlock();
                return 0;
            }
            currentOrder = OrderQueue[0];
            OrderQueue.erase(OrderQueue.begin());
        mtxOQ.unlock();
        mtxCout.lock();
            cout << getTime();
            cout << " Kitchen get order \"" << MENU[currentOrder] << "\". Preparing..." << endl;
        mtxCout.unlock();
            string str = getTime() + " Kitchen get order \"" + MENU[currentOrder] + "\". Preparing..." + "\n";
            saveReport(str);
        return currentOrder;
    }
    
    void putReadyOrder()
    {
        mtxRO.lock();
            if( currentOrder > 0 && currentOrder < 6)
                readyOrders.push_back(currentOrder);
        mtxRO.unlock();
        mtxCout.lock();
            cout << getTime();
            cout << " Kitchen: order \"" << MENU[currentOrder] << "\" is ready and wait for delivery." << endl;
        mtxCout.unlock();
        string str = getTime() + " Kitchen: order \"" + MENU[currentOrder] + "\" is ready and wait for delivery." + "\n";
        saveReport(str);
    }
    void clearReadyOrders()
    {
        readyOrders.clear();
    }
    void kitchenWork()
    {
        do {
            if(!getOrderFromQueue()) // Если очередь заказов пуста
            {
                this_thread::sleep_for(chrono::seconds(1));
                continue;
            }
            // Готовка
            srand((unsigned)time(nullptr));
            int sec = 5 + rand() % 11;
            this_thread::sleep_for(chrono::seconds(sec));
            // Отдаём готовый заказ на выдачу
            putReadyOrder();
        } while (finishedOrders  < 10);
    }
};

class Courier : public Timer
{
    vector<string> basket {};
    string reportFileName {};
public:
    void createReportFile() // Засунуть бы эту штуку в конструктор, но в названии файла есть дата-время.
    {                       // Поэтому надо этот метод запускать после старта процесса
        reportFileName = "Report_Courier_" + getDateTime() + ".txt";
        ifstream inFile(reportFileName.c_str() );
        if (inFile.is_open())
        {
            inFile.close();
            remove(reportFileName.c_str());
        }
        ofstream oFile(reportFileName.c_str());
        if ( !oFile.is_open())
        {
            cerr << "*** Order Taker: Report File Error ***\n";
            cout << "  Cannot create a file " << reportFileName.c_str() << endl;
        }
        else 
        {
            oFile.close();
        }
    } 
    void saveReport(string &s)
    {
        ofstream oFile(reportFileName.c_str(), ios::app);
        if ( !oFile.is_open())
        {
            cerr << "*** Order Taker: Report File Error ***\n";
            cout << "  Cannot open a file " << reportFileName.c_str() << endl;
        }
        else 
        {
            oFile << s;
            oFile.close();
        }
    }   
    void takeReadyOrders()
    {
        mtxRO.lock();
        while (readyOrders.size())
        {
            int dishIndex = readyOrders[readyOrders.size() - 1];
            string str = MENU[dishIndex];
            basket.push_back(str);
            readyOrders.pop_back();
            ++finishedOrders;
        }
        mtxRO.unlock();
    }
    void report()
    {
        mtxCout.lock();
        cout << getTime() << " Courier: next orders was delivered:\n";
        string report = getTime() + " Courier: next orders was delivered:\n";
        for ( int i = (int)basket.size() - 1; i >= 0; --i)
        {
            cout << "  " << basket[i] << endl;
            report += "  " + basket[i] + "\n";
        }
        cout << "  Finished orders: " << finishedOrders << endl;
        report += "  Finished orders: " + to_string(finishedOrders) + "\n";
        mtxCout.unlock();
        saveReport(report);
    }
    void clearBasket()
    {
        basket.clear();
    }
    int getFinished()
    {
        return finishedOrders;
    }
    void courierWork()
    {
        do {
            this_thread::sleep_for(chrono::seconds(30));
            takeReadyOrders();
            report();
            clearBasket();
        } while (finishedOrders < 10);
    }
};



int main()
{
    cout << "----------- Initializing ------------\n";
    cout << "  Making Order Taker... ";
    OrderTaker* ot = new OrderTaker();
    ot->clearQueue();
    cout << "  Done." << endl;
    cout << "  Setup Kitchen... ";
    Kitchen* kitchen = new Kitchen();
    kitchen->clearReadyOrders();
    cout << "  Done." << endl;
    cout << "  Making Courier... ";
    Courier* courier = new Courier();
    courier->clearBasket();
    cout << "  Done." << endl;
    cout << "----- Restaurant Ready to Work ------\n";
    cout << "-- Press Enter to Start" << endl;
    cin.get();
    
    Timer* timer = new Timer();
         ot->createReportFile();
    kitchen->createReportFile();
    courier->createReportFile();

    thread tOrderTaker(&OrderTaker::orderTakerWork, ot);
    thread    tKitchen(&Kitchen::kitchenWork, kitchen);
    thread    tCourier(&Courier::courierWork, courier);
    
    tOrderTaker.detach();
    tKitchen.detach();
    tCourier.detach();
    /*
    tOrderTaker.join();
       tKitchen.join();
       tCourier.join();
    */
    while (finishedOrders < 10)
    {
        this_thread::sleep_for(chrono::seconds(2));
    }
    return 0;
}