#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "city_country.h"
#include <conio.h>

// Безопасный ввод строки с клавиатуры
// g — размер буфера
void newgets(char *str,int g){
    printf("(%d - максимальная длина строки) ",g-2);
    int cc=0;
    int lenstrd=16;
    int len =0;
    char *dstr = (char*)malloc(lenstrd*sizeof(char));
    if (!dstr)
    {
        return;
    }
    while ((cc==13 && len==0) || cc!=13)
    {
        cc=_getch();
        if (cc==27)
        {
            str[0]=27;
            str[1]='\0';
            free(dstr);
            printf("\n");
            return;
        }
        else if (cc==13)
        {
            if (len != 0)
            {
                dstr[len]='\0';
                // str[i+1]='\0';
                printf("\n");
                break;
            }
        }
        else if (cc==8)
        {
            if (len>0)
            {
                len--;
                printf("\b \b");
            }
        }
        else if (isprint(cc) || (unsigned char)cc >= 0x80) { 
            if (len +1>=lenstrd )
            {
                lenstrd *=2;
                char *newdstr = (char *)realloc(dstr,lenstrd*sizeof(char));
                if (!newdstr)
                {
                    free(dstr);
                    return;
                }
                dstr = newdstr;

            }
            // Печатаем только печатные символы и расширенную кириллицу
            dstr[len++] = (char)cc;
            printf("%c", cc); // Отображаем ввод пользователю
        }
    }
    if (len >= g-1)
    {
        printf("ошибка размера строки!!!\n");
        strncpy(str,"error",g-1);
    }
    else
    {
        strcpy(str, dstr);
    }
    free(dstr);
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
    if (str[0]==27)
    {
        char buf[10];
        printf("Вы нажали Esc. Выйти в главное меню? (1-да, 2-нет): ");
        while (1)
        {
            newgets(buf,10);
            if (buf[0] == 27) return 2;
            if (strcmp(buf, "1") == 0)
                return 9;      // да, выходим
            if (strcmp(buf, "2") == 0)
                return 2;      // нет, продолжаем
            printf("Ошибка, некорректный выбор :%s\nВыйти в главное меню (1-да, 2-нет): ",buf);
        }
    }
    return 2;              
}
// Запрещённые спецсимволы для любых текстов (и для файла, и для данных)
int is_special(unsigned char c){
    if (ispunct(c) || c==0xB9)
        return 1;
    return 0;
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
        newgets(valuestr, 11);
        if (strcmp(valuestr,"error")==0)
            continue;
        v=exist_all(valuestr);
        if (v==9){
            *k=4;
            return;
        }
        if (valuestr[0] != 27){
            if (strcmp(valuestr,"4") !=0 && strcmp(valuestr,"3") !=0 && strcmp(valuestr,"2") !=0 && strcmp(valuestr,"1") !=0)
                printf("Ошибка, некорректный выбор: %s\n",valuestr);
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
        } while (NAME[0]==27  || strcmp(NAME,"error")==0);
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
        } while (NAME[0]==27 || strcmp(NAME,"error")==0);
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

    while (!K)
    {
        K=pr_space(str);
        if (K==1)
        {
            int found=0;
            int H=0;
            int found1=0;
            char str1[STO];
            memset(str1,0,sizeof(str1));
            for (int i=0;i<strlen(str);i++)
            {
                unsigned char c = (unsigned char)str[i];
                if (is_special(c) == 1 && !(c==' ' || c=='-'))
                {
                    H=0;
                    for (int g=0;g<found;g++)
                    {
                        if (str1[g]==c)
                            H=1;
                    }
                    if (H==0){
                        str1[found] = c;
                        found++;
                    }
                    K=0;
                }
                else if (isdigit((unsigned char)c)) {
                    found1++;
                    K=0;
                }
            }
            if (found > 0)
            {
                printf("Ошибка, спецсимволы в названии: %s\n",str1);
                memset(str1,0,sizeof(str1));
            }
            if (found1==1)
                printf("Ошибка: цифра в названии\n");
            else if (found1>1)
                printf("Ошибка: цифры в названии\n");
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

            } while (str[0]==27  || strcmp(str,"error")==0);
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
        char str1[STO];
        char str2[STO];
        memset(str1,0,sizeof(str1));
        memset(str2,0,sizeof(str2));
        int found=0;
        int found1=0;
        int H=0;
        int G=0;
        for (int j=0;j<strlen(chel);j++){
            char u=chel[j];
            unsigned char c = (unsigned char)chel[j];
            if (!isdigit(c)){
                if (is_special(c))
                {
                    H=0;
                    for (int g=0;g<found;g++)
                    {
                        if (str1[g]==c)
                            H=1;
                    }
                    if (H==0){
                        str1[found] = c;
                        found++;
                    }
                }
                if (is_rus_alpha(u) || isalpha(u))
                {
                    G=0;
                    for (int g=0;g<found1;g++)
                    {
                        if (str2[g]==u)
                            G=1;
                    }
                    if (G==0){
                        str2[found1] = u;
                        found1++;
                    }
                }
                K=0;
            }

        }
        if (found > 0 )
            printf("Ошибка, в числе присутствуют спецсимволы:%s\n",str1);
        if (found1>0)
            printf("Ошибка, в числе присутствуют буквы:%s\n",str2);
        if (found >0 || found1 >0)
            printf("Для ввода числа используйте только цифры!!!\n");
        memset(str1, 0, sizeof(str1));
        memset(str2, 0, sizeof(str2));
        if (!K){
            do
            {
                printf("%s",str);
                newgets(chel,15);
                if (strcmp(chel,"error")==0) continue;              
                *k=exist_all(chel);
                if (*k==9)
                    return;
            } while (chel[0]==27  || strcmp(chel,"error")==0);
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
    int found=0;
    int H;
    char str1[STO];
    for (size_t i = 0; i < len; i++){
        unsigned c = (unsigned char)core[i];
        if (is_special(c) && !(c==' ' || c=='-')){
            H=0;
            for (int g=0;g<found;g++)
            {
                if (str1[g]==c)
                    H=1;
            }
            if (H==0){
                str1[found] = c;
                found++;
            }
        }
    }
    if (found > 0)
        {
            printf("Ошибка, спецсимволы в названии: %s\n",str1);
            return 0;
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
            printf("Ошибка, некорректный выбор: %s\n",buf);
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
        } while (str=="\0" || str =="\n" || str[0]== 27 || strcmp(str,"error")==0);
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

            else if (Err == 2) continue;

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
            newgets(chel,15);
            if (strcmp(chel,"error")==0) continue;
            if (exist_all(chel)==9)
                return;
            pr_dig(chel,"Введите кол-во жителей числом:",&k);// проверяем, что в строке только цифры
            if (k==9)
                return;
            people = (int)strtol(chel,NULL,10);
        } while (chel[0]==27  || strcmp(chel,"error")==0);// не допускаем пустую строку

        newobject(country,&k,"страны");
        if (k==9) return;
        // записываем строку в файл в виде таблицы
        fprintf(f, "%-20s |%-15d |%-20s\n", name, people, country);
        printf("Данные успешно добавлены в файл %s.\n",NAME);
        // выводим подтверждение и печать строки на экран
        printf("%-20s |%-15s |%-20s\n", "город", "кол-во жителей", "страна");
        printf("%-20s |%-15d |%-20s\n", name, people, country);
        // printf("\nХотите продолжить вводить города нажмите enter или введите 2 для выхода в меню.");
        // printf("\nВаш выбор: ");
        // newenter(&i);
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
        // printf("\nХотите продолжить вводить страны нажмите enter или введите 2 для выхода в меню.");
        // printf("\nВаш выбор: ");
        // newenter(&i);
    }
    fclose(f);
}
