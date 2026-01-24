#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "city_country.h"

int main() 
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    int i = 0;      
    char NAME[STO];
    while (i!=3) {
        printf("Формирование базы данных для городов и стран\n");
        printf("======================================================================\n");
        printf("Что умеет:\n");
        printf("- Создавать новый файл или работать с уже существующими;\n");
        printf("- Добавлять города (название, население, страна);\n");
        printf("- Добавлять страны (название, столица, континент);\n");
        printf("- Проверять корректность вводимых данных;\n");
        printf("- Сохранять записи в текстовые файлы CITY.txt и COUNTRY.txt.\n");
        printf("- Имеется функция выхода в главное меню из любой точки программы(нажмите enter, когда ваша строка пуста)\n");
        printf("======================================================================\n");
        printf("\n");
        printf("1 - Добавить данные о городах\n");
        printf("2 - Добавить данные о странах\n");
        printf("3 - Выйти\n");
        printf("\n");
        printf("Ваш выбор: ");
        char _buf_m[64];
        fgets(_buf_m, sizeof _buf_m, stdin);
        printf("======================================================================\n");
        i = (int)strtol(_buf_m, NULL, 10);


        if (i == 1)
        {
            strcpy(NAME, "CITY.txt");   
            char s[STO];
            strcpy(s,"\n");
            printf("тест %d",!strlen(s));    
            newstr(s);
            do
            {
                printf("%s",s);
                printf("тест %d",!s);
            } while (!s);
            
            city(NAME);

        }
        else if (i == 2)
        {
            strcpy(NAME, "COUNTRY.txt"); 
            Country(NAME);
        }
        else if (i == 3) {
            printf("Выход из программы.\n");
        } 
        else{
            printf("Неверный выбор, попробуйте еще раз.\n");
        }
    }

    return 0;
}