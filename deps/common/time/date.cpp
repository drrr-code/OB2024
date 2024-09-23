#include"date.h"

bool check_date(int year,int month,int day){
    int mon[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    bool is_leap=(year%400==0)||(year%100!=0&&year%4==0);
    if(year>0&&(month>0)&&(month<=12))//年月合格
    {
        if(is_leap)//是闰年
        {
            if((day>0)&&(month==2)&&(day<=29)){
                return true;
            }else if((day>0)&&(day<=mon[month])){
                return true;
            }else{
                return false;
            }
        }
        else
        {
            if((day>0)&&(day<=mon[month])){
                return true;
            }else{
                return false;
            }
        }
    }else{
        return false;
    }
}

int string_to_date(std::string date_str){
    int year,mon,day;
    sscanf(date_str.c_str(), "%d-%d-%d", &year, &mon, &day);
    if(check_date(year,mon,day)){
        return year*10000+mon*100+day;
    }else{
        return -1;
    }
}