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
        std::cout << "Ôàéë óñïåøíî îòêðûò!" << std::endl;
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
        std::cout << "Îøèáêà: Íå óäàëîñü îòêðûòü ôàéë!" <<std::endl;
        std::cout << "Ïðîâåðüòå ïóòü: " <<fname <<std::endl;
    }
}
void namefile(std::string &line)
{
    std::cout << "Ââåäèòå íàçâàíèå ôàéëà â ïàïêå C:\\Users\\Admin\\Desktop\\mai2\\project_c" << std::endl;
    std::cin >> line;
    line+="C:\\Users\\Admin\\Desktop\\mai2\\project_c\\";
    std::cout << "Ïîëíûé ïóòü ê ôàéëó: " << line << std::endl;
}
void menu(char fname)
{
    char vibor;
    std::cout << "=====================================\n";
    std::cout << "      ÃËÀÂÍÎÅ ÌÅÍÞ ÏÐÎÃÐÀÌÌÛ        \n";
    std::cout << "=====================================\n\n";
    std::cout << "Âûáåðèòå òèï ôàéëîâ äëÿ ñòàðòà ðàáîòû:\n";
    std::cout << "1 - Ãîðîäà\n";
    std::cout << "2 - Ñòðàíû\n";
    std::cout << "3 - Äðóãîå\n";
    std::cout<< "4 - Âûõîä\n";
    std::cout << "\nÂàø âûáîð: ";
    std::cin >> vibor;
    std::cout << "Âû âûáðàëè: " << vibor << std::endl;
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
    std::cout << "\nÍàæìèòå Enter äëÿ ïðîäîëæåíèÿ..." <<std::endl;

    std::cin.ignore();
    std::cin.get();
    return 0;
}
