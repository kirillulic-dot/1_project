#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "city_country.h"

// Удаляет символ '\n' в конце строки, если он есть

void newstr(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

// Безопасный ввод строки с клавиатуры
// g — размер буфера
void newgets(char *str,int g){
    fgets(str,g,stdin);
    newstr(str);
}

// Проверяет, удалось ли открыть файл
// f — указатель на файл
// NAME — имя файла (для вывода ошибки)
int pr_file(FILE *f,char *NAME){
    if (f==NULL){
        printf("Ошибка открытия файла %s\n",NAME);
        return 0;
    }
    return 1;
}
// Проверяет, пуста ли строка
// Если пользователь нажал Enter — предлагает выйти в меню
int exist_all(const char *str){
    // str уже после newstr()
    if (strlen(str) > 0)
        return 2;          // продолжаем
    char buf[10];
    printf("Вы нажали Enter. Выйти в главное меню? (1-да, 2-нет): ");
    while (1){
        newgets(buf,10);
        if (strcmp(buf, "1") == 0)
            return 9;      // да, выходим
        if (strcmp(buf, "2") == 0)
            return 2;      // нет, продолжаем
        printf("Ошибка: введите 1 или 2: ");
    }
}
// Запрещённые спецсимволы для любых текстов (и для файла, и для данных)
int is_special(unsigned char c){
    return (c == '\\' || c == '/' || c == ':' || c == '*' ||
            c == '?'  || c == '"' || c == '<' || c == '>' || c == '|');
}
// Проверяет, является ли символ русской буквой (кодировка Windows-1251)
int is_rus_alpha(unsigned char c){
    return (c >= 0xC0 && c <= 0xDF) ||    // А-Я
           (c >= 0xE0 && c <= 0xFF) ||    // а-я
           c == 0xA8 ||                   // Ё
           c == 0xB8;                     // ё
}

// Проверяет корректность пробелов и тире в строке
// Запрещено:
// - начало/конец строки с пробела или '-'
// - два пробела подряд
// - два '-' подряд
// - сочетания " -" и "- "
int pr_space(const char *str){
    size_t len = strlen(str);
    // 1) начало строки
    if (str[0] == ' ' || str[0] == '-'){
        printf("Ошибка: строка не может начинаться с пробела или '-'\n");
        return 0;
    }
    // 2) конец строки
    if (str[len - 1] == ' ' || str[len - 1] == '-'){
        printf("Ошибка: строка не может заканчиваться пробелом или '-'\n");
        return 0;
    }
    // 3) проверки символов подряд
    for (size_t i = 0; i < len - 1; i++){
        char c1 = str[i];
        char c2 = str[i + 1];
        // два пробела
        if (c1 == ' ' && c2 == ' '){
            printf("Ошибка: два пробела подряд запрещены\n");
            return 0;
        }
        // два тире подряд
        if (c1 == '-' && c2 == '-'){
            printf("Ошибка: два '-' подряд запрещены\n");
            return 0;
        }
        // пробел + тире
        if (c1 == ' ' && c2 == '-'){
            printf("Ошибка: сочетание ' -' запрещено\n");
            return 0;
        }
        // тире + пробел
        if (c1 == '-' && c2 == ' '){
            printf("Ошибка: сочетание '- ' запрещено\n");
            return 0;
        }
    }
    return 1;
}
//к отвечает за действия пользователя
//value отвечает за выбор нового, старого или файла по умолчанию
// в newfile мы выбираем между новыми и уже существующими файлами
void newfile(char *NAME,int *k){
    int value;//value отвечает за выбор нового, старого или файла по умолчанию
    int v;//за выход из любой точки этой функции
    do
        {      
        printf("\nВведите 1 для работы в файле по умолчанию.\n");
        printf("Введите 2 для продожнения работы в уже созданном файле.\n");
        printf("Введите 3 для работы в новом файле.\n");
        printf("Введите 4 для выхода.\n");
        printf("\n");
        printf("Ваш выбор: ");
        char valuestr[11];
        newgets(valuestr, sizeof valuestr);
        v=exist_all(valuestr);
        if (v==9){
            *k=4;
            return;
        }
        if (valuestr[0] != '\0'){
            printf("======================================================================\n");
            value = (int)strtol(valuestr, NULL, 10);
            if (value !=4 && value != 3 && value != 2 && value != 1)
                printf("Ошибка: неизвестный выбор: %s\n",valuestr);
        }
    } while (value !=4 && value != 3 && value != 2 && value != 1);
    if (value==1)
        return;
    else if (value == 2){
        do
        {
            printf("\nВведите названия файла, находящийся в папке 'MAI_1':\n");
            newgets(NAME,STO);
            v= exist_all(NAME);
            if (v==9){
                *k=4;
                return;
            }
            *k=2;
        } while (!strlen(NAME));
    }
    else if (value == 3){
        do
        {
            printf("Введите названия для нового файла:\n");
            newgets(NAME,STO);
            v=exist_all(NAME);
            if (v==9){
                *k=4;
                return;
            }
            *k=3;
        } while (!strlen(NAME));
    }
    else if (value == 4){
        *k = 4;  
        return;
    }
}

// Проверяет строку с названием:
// - отсутствие спецсимволов
// - отсутствие цифр
// - корректные пробелы и тире
void pr_str(char *str,int *k)
{
    int K=0;//показывает ошибки,если в конце =1 то их не было
    if (strlen(str)==0)
        return;
    while (!K)
    {
        K=pr_space(str);
        if (K==1){
            for (int i=0;i<strlen(str);i++){
                unsigned char c = (unsigned char)str[i];
                if (is_special(c) == 1){
                    printf("Ошибка: спецсимвол в названии: %c\n", c);
                    K=0;
                    break;
                }
                if (isdigit((unsigned char)c)) {
                    printf("Ошибка: цифра в названии\n");
                    K=0;
                    break;
                }

                else if (!(isalpha(c) || is_rus_alpha(c) || c==' ' || c=='-')){
                    printf("Ошибка: некорректный символ в названии :%c\n",c);
                    K=0;
                    break;
                }
            }
        }
        if (!K){
            do
            {
                printf("Введите название снова\nВаш выбор: ");
                newgets(str, STO);
                *k=exist_all(str);
                if (*k==9){
                    return;
                }

            } while (!strlen(str));
        }
    }

}
// Проверка, что строка chel содержит ТОЛЬКО цифры.
// Если есть любой другой символ — просим ввести заново.
// str — текст подсказки, k — служебный флаг выхода в меню (через exist_all).
void pr_dig(char *chel,char *str,int *k){
    int K=0;
    while (K==0){
        K=1;
        for (int j=0;j<strlen(chel);j++){
            unsigned char c = (unsigned char)chel[j];
            if (!isdigit(c)){
                printf("Ошибка: введён непонятный символ в числе:%c\n",c);
                K=0;
                break;
            }

        }
        if (!K){
            do
            {
                printf("%s",str);
                newgets(chel,STO);
                *k=exist_all(chel);
                if (*k==9)
                    return;
            } while (!strlen(chel));
        }
    }
}
// Добавляет расширение .txt к имени файла, если его нет.
// NAME — строка с именем файла (буфер размера STO).
void pr_txt(char *NAME){
    size_t len = strlen(NAME);
    if (len < 4 || strcmp(NAME + len - 4, ".txt") != 0)
        strncat(NAME,".txt",STO-strlen(NAME)-1);
}

/*
    Проверка имени файла:
    - отсутствие спецсимволов
    - корректные пробелы и тире
    - проверка существования файла
*/
int pr_name(char *NAME,int *k){
    size_t total = strlen(NAME);
    // Ядро без .txt
    size_t len = total - 4;
    char core[STO];
    strncpy(core, NAME, len);
    core[len] = '\0';
    // спецсимволы
    for (size_t i = 0; i < len; i++){
        char c = core[i];
        if (is_special(c)){
            printf("Ошибка: запрещённый символ в имени файла: %c\n", c);
            return 0;
        }
    }
    // пробелы + тире
    if (!pr_space(core))
        return 0;
    // файл существует -> предупредить
    FILE *f = fopen(NAME, "r");
    if (f != NULL){
        fclose(f);
        char buf[10];
        do
        {
            printf("Файл \"%s\" уже существует. Перезаписать? (1-да, 2-нет): ", NAME);
            newgets(buf, 10);
            *k=exist_all(buf);
            if (*k==9)
                return 9;
            if (strcmp(buf,"1")==0) return 1;
            if (strcmp(buf,"2")==0){
                printf("Отмена — выберите другое имя файла.\n");
                return 0;
            }
            if(*k==2)
                continue;
            else if (strcmp(buf,"1") != 0 && strcmp(buf,"2") != 0)
                printf("Ошибка: неизвестный выбор: %s\n",buf);
        } while (strcmp(buf,"1") != 0 && strcmp(buf,"0") != 0);
    }
    return 1;
}

/*
    Универсальный ввод текстового объекта (город, страна и т.п.).
*/
void newobject(char *str,int *k,char *name)
{
    do
        {
            printf("Введите название %s: ",name);
            newgets(str, STO);
            *k=exist_all(str);
            if (*k==9)
                return;
            pr_str(str,k);
            if (*k==9)
                return; 
        } while (!strlen(str));
}

/*
    Обработка ввода "продолжить / выйти".
*/
void newenter(int *i){
    char string[11];
        newgets(string, sizeof string);
        if (strlen(string)==0){
            *i=0;
            return;
        }
        else if (strlen(string) == 1 && string[0] == '2'){
            *i=2;
            return;
        }
        *i=1;
        while (*i != 0 && *i != 2){
            printf("\nОшибка: неизвестный выбор:%s",string);
            printf("\nнажмите enter чтобы продолжить или введите 2 для выхода в меню.");
            printf("\nВаш выбор: ");
            newgets(string, sizeof string);
            if (strlen(string)==0){
                *i=0;
                return;
            }
            else if (strlen(string) == 1 && string[0] == '2')
                *i = 2;        }
}
void city(char *NAME) {
    FILE *f;                 // указатель на файл (через него читаем/пишем)
    char name[STO];          // строка: название города (ввод пользователя)
    char country[STO];       // строка: страна (ввод пользователя)
    char chel[STO];          // строка: число жителей, но в виде текста (потом проверяем, что там только цифры)
    int people;              // число жителей уже как int (после strtol)
    int i=0;                 // управляющая переменная цикла ввода:
                             // i==0 -> продолжаем ввод, i==2 -> выйти в меню (задаётся функцией newenter)
    int k=1;                 // "код состояния/действия" (используется во многих функциях):
                             // k==4 -> выход из выбора файла (в newfile)
                             // k==3 -> выбран новый файл
                             // k==2 -> выбран существующий файл
                             // k==9 -> пользователь выбрал "выйти в меню" (через exist_all)
    int pr;                  // результат проверки имени файла (pr_name):
                             // 1 -> можно, 0 -> нужно ввести другое имя, 9 -> выход в меню

    newfile(NAME, &k);

    if (k==4) // режим: "выход в меню"
        return;
    else if (k==3)// режим: "работа в новом файле"
    {
        pr_txt(NAME);
        pr=pr_name(NAME,&k);// проверка имени файла и вопрос "перезаписать, если файл уже существует"
        if (pr==9)
            return;
        while(pr==0){
            printf("Введите название файла повторно:");
            newgets(NAME,STO);
            if (exist_all(NAME)==9)
                return;
            pr_txt(NAME);
            pr=pr_name(NAME,&k);
            if (k==9)
                return;
        }
        f = fopen(NAME,"w"); 
        if (!pr_file(f,NAME)) return;
        fprintf(f,"%-20s |%-15s |%-20s\n", "город", "кол-во жителей", "страна");
    }
    // режим: "продолжить работу в уже созданном файле"
    else if (k==2){
        f = fopen(NAME,"r");
        if (!pr_file(f,NAME)) return;
        char far[10];
        fgets(far,2,f);
        far[1]='\0';
    // идея проверки: если в файле в начале стоит 'c',
    // значит это файл "для стран" (Country), а не файл "для городов"
        if (strcmp(far,"c")==0)
        {
            printf("Вы указвли файл для стран, возрат в главное меню");
            return;
        }
        fclose(f);
        f = fopen(NAME,"a");    
    }
    else{
         // если открыть на чтение не получилось — вероятно файла нет.
        // тогда создаём/открываем на добавление и пишем шапку
        f = fopen(NAME,"r"); 
        if (!pr_file(f,NAME)){
            f=fopen(NAME,"a");
            if (!pr_file(f,NAME)) return;  
            fprintf(f,"%-20s |%-15s |%-20s\n", "город", "кол-во жителей", "страна"); 
        }
        // если файл есть — закрываем чтение и открываем на добавление
        else{
            fclose(f);
            f=fopen(NAME,"a");
            if (!pr_file(f,NAME)) return;  
        }
    }
    // основной цикл ввода данных. Пока i==0, пользователь продолжает вводить записи
    while(i==0){
        newobject(name,&k,"города"); // ввод названия города + проверки (пробелы/тире/символы)
        if (k==9) return;
        do
        {
            printf("Введите количество жителей: ");
            newgets(chel,STO);
            if (exist_all(chel)==9)
                return;
            pr_dig(chel,"Введите кол-во жителей числом:",&k);// проверяем, что в строке только цифры
            if (k==9)
                return;
            people = (int)strtol(chel,NULL,10);
        } while (!strlen(chel));// не допускаем пустую строку
        newobject(country,&k,"страны");
        if (k==9) return;
        // записываем строку в файл в виде таблицы
        fprintf(f, "%-20s |%-15d |%-20s\n", name, people, country);
        printf("Данные успешно добавлены в файл %s.\n",NAME);
        // выводим подтверждение и печать строки на экран
        printf("%-20s |%-15s |%-20s\n", "город", "кол-во жителей", "страна");
        printf("%-20s |%-15d |%-20s\n", name, people, country);
        printf("\nХотите продолжить вводить города нажмите enter или введите 2 для выхода в меню.");
        printf("\nВаш выбор: ");
        newenter(&i);
    }
    fclose(f);
}
///тоже самое только для страны столицы и континента
void Country(char *NAME) 
{
    FILE *f;
    char country[STO];
    char stolica[STO];
    char kontinent[STO];
    int i=0;
    int k=1;
    int pr;
    newfile(NAME, &k);
    if (k==4)
        return;
    else if (k==3){
        pr_txt(NAME);
        pr=pr_name(NAME,&k);
        if (pr==9)
            return;
        while(pr==0){
            printf("Введите название файла повторно:");
            newgets(NAME,STO);
            if (exist_all(NAME)==9)
                return;
            pr_txt(NAME);
            pr=pr_name(NAME,&k);
            if (k==9)
                return;
        }
        f = fopen(NAME,"w"); 
        if (!pr_file(f,NAME)) return;
        fprintf(f,"%-20s |%-20s |%-20s\n", "страна", "столица", "континент");
    }
    else if (k==2){
        f = fopen(NAME,"r");
        if (!pr_file(f,NAME)) return;
        char far[10];
        fgets(far,2,f);
        far[1]='\0';
        // идея проверки: если в файле в начале стоит 'г',
        // значит это файл "для городов" (City), а не файл "для Стран"
        if (strcmp(far,"г")==0)
        {
            printf("Вы указали файл для стран, возрат в главное меню");
            return;
        }
        fclose(f);
        f = fopen(NAME,"a");     
    }
    else{
       f = fopen(NAME,"r"); 
        if (!pr_file(f,NAME)){
            f=fopen(NAME,"a");
            if (!pr_file(f,NAME)) return;
            fprintf(f,"%-20s |%-20s |%-20s\n", "страна", "столица", "континент");  
        }
        else{
            fclose(f);
            f=fopen(NAME,"a");
            if (!pr_file(f,NAME)) return;   
        }
    }
    while(i==0){
        newobject(country,&k,"страны");
        if (k==9) return;
        newobject(stolica,&k,"столица");
        if (k==9) return;
        newobject(kontinent,&k,"континента");
        if (k==9) return;
        fprintf(f, "%-20s |%-20s |%-20s\n", country, stolica, kontinent);
        printf("Данные успешно добавлены в файл %s.\n",NAME);
        printf("%-20s |%-20s |%-20s\n", "страна", "столица", "континент");
        printf("%-20s |%-20s |%-20s\n", country, stolica, kontinent);
        printf("\nХотите продолжить вводить страны нажмите enter или введите 2 для выхода в меню.");
        printf("\nВаш выбор: ");
        newenter(&i);
    }
    fclose(f);
}
