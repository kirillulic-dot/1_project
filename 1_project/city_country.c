#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "city_country.h"

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
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
    if (strlen(str)==g-1 && str[g-2]!='\n')
    {
        printf("ошибка размера строки!!!\n");
        clear_buffer();
        strncpy(str,"error",g-1);
    }
    else newstr(str);
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
        if (strcmp(buf,"error")==0) 
        {
            printf("Вы нажали Enter. Выйти в главное меню? (1-да, 2-нет): ");
            continue; 
        }
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
    char valuestr[11];
    int v;//за выход из любой точки этой функции
    do
        {      
        printf("======================================================================\n");
        printf("\nВведите 1 для работы в файле по умолчанию.\n");
        printf("Введите 2 для продожнения работы в уже созданном файле.\n");
        printf("Введите 3 для работы в новом файле.\n");
        printf("Введите 4 для выхода.\n");
        printf("======================================================================\n");
        printf("\n");
        printf("Ваш выбор: ");
        valuestr[0]='\0';
        newgets(valuestr, sizeof valuestr);
        if (strcmp(valuestr,"error")==0)
            continue;
        v=exist_all(valuestr);
        if (v==9){
            *k=4;
            return;
        }
        if (valuestr[0] != '\0'){
            if (strcmp(valuestr,"4") !=0 && strcmp(valuestr,"3") !=0 && strcmp(valuestr,"2") !=0 && strcmp(valuestr,"1") !=0)
                printf("Ошибка: неизвестный выбор: %s\n",valuestr);
        }
    } while (strcmp(valuestr,"4") !=0 && strcmp(valuestr,"3") !=0 && strcmp(valuestr,"2") !=0 && strcmp(valuestr,"1") !=0);
    if (strcmp(valuestr,"1") ==0)
        return;
    else if (strcmp(valuestr,"2") ==0){
        do
        {
            printf("\nВведите названия файла, находящийся в папке 'MAI_1':\n");
            newgets(NAME,STO);
            if (strcmp(NAME,"error")==0) continue;
            v= exist_all(NAME);
            if (v==9){
                *k=4;
                return;
            }
            *k=2;
        } while (!strlen(NAME) || strcmp(NAME,"error")==0);
    }
    else if (strcmp(valuestr,"3") ==0){
        do
        {
            printf("Введите названия для нового файла:\n");
            newgets(NAME,STO);
            if (strcmp(NAME,"error")==0) continue;
            v=exist_all(NAME);
            if (v==9){
                *k=4;
                return;
            }
            *k=3;
        } while (!strlen(NAME) || strcmp(NAME,"error")==0);
    }
    else 
    {
        *k=4;
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
                if (strcmp(str,"error")==0)
                    continue;
                *k=exist_all(str);
                if (*k==9){
                    return;
                }

            } while (!strlen(str) || strcmp(str,"error")==0);
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
                newgets(chel,10);
                if (strcmp(chel,"error")==0) continue;              
                *k=exist_all(chel);
                if (*k==9)
                    return;
            } while (!strlen(chel) || strcmp(chel,"error")==0);
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
            if (strcmp(buf,"error")==0) continue;
            *k=exist_all(buf);
            if (*k==9)
                return 9;
            if (strcmp(buf,"1")==0) return 1;
            if (strcmp(buf,"2")==0){
                printf("Отмена — выберите другое имя файла.\n");
                return 0;
            }
            printf("Ошибка: неизвестный выбор: %s\n",buf);
        } while (strcmp(buf,"1") != 0 && strcmp(buf,"2") != 0);
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
            if (strcmp(str,"error")==0) continue;         
            *k=exist_all(str);
            if (*k==9)
                return;
            pr_str(str,k);
            if (*k==9)
                return; 
        } while (!strlen(str) || strcmp(str,"error")==0);
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
            if (strcmp(string,"error")!=0)
            {
                printf("\nОшибка: неизвестный выбор:%s",string);
            }
            printf("\nнажмите enter чтобы продолжить или введите 2 для выхода в меню.");
            printf("\nВаш выбор: ");
            newgets(string, sizeof string);
            if (strcmp(string,"error")==0)
                continue;            
            if (strlen(string)==0){
                *i=0;
                return;
            }
            else if (strlen(string) == 1 && string[0] == '2')
                *i = 2;        
        }
}
void city(char *NAME) {
    FILE *f;                 // указатель на файл (через него читаем/пишем)
    char name[STO];          // строка: название города (ввод пользователя)
    char country[STO];       // строка: страна (ввод пользователя)
    char chel[10];          // строка: число жителей, но в виде текста (потом проверяем, что там только цифры)
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
            if (strcmp(NAME,"error")==0)
                continue;            
            int Err = exist_all(NAME);

            if (Err==9) return;

            else if (Err == 2 && strlen(NAME)==0) continue;

            pr_txt(NAME);
            pr=pr_name(NAME,&k);
            if (k==9)
                return;
        }
        f = fopen(NAME,"w"); 
        if (!pr_file(f,NAME)) return;
        fprintf(f,"%-20s |%-15s |%-20s\n", "город", "кол-во жителей", "страна");
        fclose(f);
        f = fopen(NAME,"a");
    }
    // режим: "продолжить работу в уже созданном файле"
    else if (k==2){
        pr_txt(NAME);
        f = fopen(NAME,"r");
        if (!pr_file(f,NAME)) return;
        int far;
        far = fgetc(f);
    // идея проверки: если в файле в начале стоит 'c',
    // значит это файл "для стран" (Country), а не файл "для городов"
        if (far == 241)
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
        if (f==NULL){
            f=fopen(NAME,"w");
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
            newgets(chel,10);
            if (strcmp(chel,"error")==0) continue;
            if (exist_all(chel)==9)
                return;
            pr_dig(chel,"Введите кол-во жителей числом:",&k);// проверяем, что в строке только цифры
            if (k==9)
                return;
            people = (int)strtol(chel,NULL,10);
        } while (!strlen(chel) || strcmp(chel,"error")==0);// не допускаем пустую строку

        newobject(country,&k,"страны");
        if (k==9) return;
        printf("1");
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
            if (strcmp(NAME,"error")==0) continue;    
            int Err = exist_all(NAME);
            if (Err==9)
                return;
            else if (Err == 2 && strlen(NAME)==0)
                continue;
            pr_txt(NAME);
            pr=pr_name(NAME,&k);
            if (pr==9)
                return;
        }
        f = fopen(NAME,"w"); 
        if (!pr_file(f,NAME)) return;
        fprintf(f,"%-20s |%-20s |%-20s\n", "страна", "столица", "континент");
        fclose(f);
        f = fopen(NAME,"a");
    }
    else if (k==2){
        pr_txt(NAME);
        f = fopen(NAME,"r");
        if (!pr_file(f,NAME)) return;
        int far;
        far = fgetc(f);
        // идея проверки: если в файле в начале стоит 'г',
        // значит это файл "для городов" (City), а не файл "для Стран"
        if (far == 227)
        {
            printf("Вы указали файл для стран, возрат в главное меню");
            return;
        }
        fclose(f);
        f = fopen(NAME,"a");     
    }
    else{
        f = fopen(NAME,"r"); 
        if (f==NULL){
            f=fopen(NAME,"w");
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
