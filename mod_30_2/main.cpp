#include <iostream>
#include <string>
#include <cpr/cpr.h>

/*
Захват заголовка веб-страницы

Запросите с сервера HTML-страницу. 
Для этого изучите документацию о запросах, поддерживаемых сервисом httpbin.org/html. 
Найдите там запрос, который выдаёт именно HTML-страницу. Затем сделайте запрос, 
явным образом используя заголовок accept: text/html. 
В полученном ответе найдите заголовок статьи и выведите его в консоль.

Заголовок статьи размещается между HTML-тегами <h1></h1>.
*/

cpr::Response methodGet(std::string);
std::string get_h1(std::string &);

int main(int, char**)
{
    std::string invite("");
    do
    {
        std::cout << invite;
        std::cout << "Enter an URL: ";
        std::string domain;
        std::cin >> domain;
        if(domain == "Q" || domain == "q" )
        {
            std::cout << "Exiting...";
            exit(0);
        }
        if (domain.find("https") < 128)
        {
            std::cout << "Find https: " << domain.find("https") << std::endl;
            domain.replace( domain.find("https"), 5, "http");
        }
        if (( domain.find("http://") > 128))
        {
            domain.insert(0, "http://");
        }

        cpr::Response r = methodGet( domain );
        // std::cout << r.text << std::endl;
        // std::cout << r.raw_header << std::endl;
        if (r.text.length())
        {
            std::cout << "Response OK" << std::endl;
            std::cout << get_h1(r.text) << std::endl;;

        }
        else
        {
            std::cout << "No any response from server " << domain << std::endl;
        }
        invite = "---------------------------\n";
        invite += "Another URL or Q for exit\n";
    } while (true);
    return 0;
}

cpr::Response methodGet(std::string url)
{
    return cpr::Get(cpr::Url(url),
                    cpr::Header({{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:122.0) Gecko/20100101 Firefox/122.0"},
                                 {"Accept","text/html"}
                                })
                    ); 
}

std::string get_h1(std::string &str)
{
    int pos1 = str.find("<h1");
    int pos2 = str.find("</h1>");
    int len = pos2 - pos1;

    if (len > 0)
    {
        if (len > 1280 )
        {
            return "Header <h1> is too long. More than 90 chars. Ignore.";
        }
        std::string str2 = str.substr(pos1, len);
        
        pos1 = str2.find("<h1");
        pos2 = str2.find(">", pos1);
        len = pos2 - pos1 + 1;
        str2.erase(pos1,len);
        return str2;
    }
    return "Cannot find header <h1>.";

}