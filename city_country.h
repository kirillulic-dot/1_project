#ifndef CITY_COUNTRY_H
#define CITY_COUNTRY_H

//static const int STO = 100;

enum { STO = 100 };

void clear_buffer(void);
void newstr(char *str);
void newgets(char *str,int g);
int exist_all(const char *str);
int is_special(unsigned char c);
int is_rus_alpha(unsigned char c);
int pr_space(const char *str);
void newfile(char *NAME, int *k);
void pr_str(char *str,int *k);
void pr_dig(char *chel,char *str,int *k);
void pr_txt(char *NAME);
int pr_name(char *NAME,int *k);
void city(char *NAME);
void Country(char *NAME);

#endif
