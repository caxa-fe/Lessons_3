#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
/*
    Задача 3.
    Создайте лямбда-функцию, которая принимает на вход 
    путь в файловой системе и искомое расширение в виде строки, 
    а возвращает список имён файлов, которые имеют данное расширение.
*/
/* 
void trimBack(std::string &str)
{
    if( str.back() != ' ')
        return;
    str.pop_back();
    trimBack(str);
} 
*/
int main()
{
    auto recursiveGetFileNamesByExtension =
            [](std::filesystem::path path, const std::string ext)
    {
        for(auto& p: fs::recursive_directory_iterator(path))
        {
            if( fs::is_regular_file( p.path() ) && !p.path().extension().compare(ext) )
                std::cout << p.path() << '\n'; 
        }
    };
    // Рекурсивная лямбда для обрезки пробелов сзади. 
    auto trimBack = [](auto& self, std::string &str)
    {
        if( str.back() != ' ')
            return;
        str.pop_back();
        self(self, str);
    }; 

    while (true)
    {
        std::cout << "--- List of files  in the directory ---\n";
        std::cout << "  -- with a certain file extension --\n";
        std::cout << std::endl;
        std::cout << " Enter Q for exit or\n";
        std::cout << " Enter a path (Ex. D:/docs):\n";
        std::string path {};
        std::getline(std::cin >> std::ws, path);
        trimBack( trimBack, path ); // Обрежем возможные пробелы сзади. Спереди режет std::ws

        if ( path == "Q" || path == "q" )
        {
            std::cout << " Exiting..." << std::endl;
            break;
        }    

        if ( !fs::exists(path) )
        {
            std::cerr << "*** " << path << " does not exists! ***\n";
            std::cout << "  Press Enter to continue..." << std::endl;
            std::cin.get();
            continue;
        }
        std::cout << " Enter an extension (Ex. .png):\n";
        std::string ext {};
        std::getline(std::cin >> std::ws, ext);
        trimBack( trimBack, ext ); // Обрежем возможные пробелы сзади. Спереди режет std::ws
        if ( ext.front() != '.') ext = "." + ext;

        std::cout << path << "/*" << ext << std::endl;
        recursiveGetFileNamesByExtension(path, ext);
        std::cout << std::endl;
        std::cout << "  Press Enter to continue..." << std::endl;
        std::cin.get();
    }
    
    return 0;
}