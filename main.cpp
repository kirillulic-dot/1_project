#include <iostream>
#include <string>
#include "work_WITH_files.h"
#include "Windows.h"
#include <fstream>

int prstr()
{

}
void openfile(std::ifstream& f,std::string &lineF,std::string &fname)
{   
    f.open(fname);
    if (f.is_open())
    {
        std::cout << "Файл успешно открыт!" << std::endl;
        std::string line;
        while (std::getline(f,line))
        {
            lineF+=line+'\n';
            std::cout << line <<std::endl;
        }
        f.close();
    }
    else
    {
        std::cout << "Ошибка: Не удалось открыть файл!" <<std::endl;
        std::cout << "Проверьте путь: " <<fname <<std::endl;
    }
}
void namefile(std::string &line)
{
    std::cout << "Введите название файла в папке C:\\Users\\Admin\\Desktop\\mai2\\project_c" << std::endl;
    std::cin >> line;
    line+="C:\\Users\\Admin\\Desktop\\mai2\\project_c\\";
    std::cout << "Полный путь к файлу: " << line << std::endl;
}
void menu(char fname)
{
    char vibor;
    std::cout << "=====================================\n";
    std::cout << "      ГЛАВНОЕ МЕНЮ ПРОГРАММЫ        \n";
    std::cout << "=====================================\n\n";
    std::cout << "Выберите тип файлов для старта работы:\n";
    std::cout << "1 - Города\n";
    std::cout << "2 - Страны\n";
    std::cout << "3 - Другое\n";
    std::cout<< "4 - Выход\n";
    std::cout << "\nВаш выбор: ";
    std::cin >> vibor;
    std::cout << "Вы выбрали: " << vibor << std::endl;
    if (vibor=="1")
    {
        fname = "C:\\Users\\Admin\\Desktop\\mai2\\project_c\\CITY.txt";
    }
    else if (vibor=="2")
    {
        fname = "C:\\Users\\Admin\\Desktop\\mai2\\project_c\\COUNTRY.txt";
    }
    else if (vibor=="3")
    {
        namefile(fname);
    }
    else if (vibor == "4")
    {
        fname = 27;
        return;
    }
}
int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    std::string fname;
    menu(fname);
    if (fname==(char*)27)

    std::ifstream file;
    
    std::string lineF;
    openfile(file,lineF,fname);
    std::cout << "\nНажмите Enter для продолжения..." <<std::endl;

    std::cin.ignore();
    std::cin.get();
    return 0;
}
