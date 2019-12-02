
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <math.h>
#include <vector>

#define LOCAL

using namespace std;

map<string, string> Minedict;

//struct Time{
//    int mins;   // 分钟
//    int hours;  // 小时
//    int date;   // day of month
//    int month;  // 月份
//    int dweek;  // 当天星期几
////    Time(int mins, int hours, int date, int month, int dweeks): mins(mins), hours(hours),  date(date), month()
//};

map<int,int> daysOfMonth;

int str2int(string str){
    int ans=0;
    int length = str.length();
    for (int i = length-1, count=0; i >=0 ; i--,count++) {
        ans += (str[i]-'0')*(pow(10,count));
    }
    return ans;
}

class Time{
public:
    int mins;   // 分钟
    int hours;  // 小时
    int date;   // day of month
    int month;  // 月份
    int year;   // 年份

    int dweek;  //星期几

public:
    Time(int mins, int hours, int date, int month, int year): mins(mins), hours(hours), date(date), month(month),
    year(year){}
    Time(string str1){
        this->mins = str2int(str1.substr(10,2));
        this->hours = str2int(str1.substr(8,2));
        this->date = str2int(str1.substr(6,2));
        this->month = str2int(str1.substr(4,2));
        this->year =  str2int(str1.substr(0,4));
    }

    bool operator < (Time B){
        if(this->year>B.year)return false;
        else if(this->year<B.year) return true;
        else{
            if(this->month>B.month)return false;
            else if(this->month<B.month) return true;
            else{
                if(this->date>B.date)return false;
                else if(this->date<B.date) return true;
                else{
                    if(this->hours>B.hours)return false;
                    else if(this->hours<B.hours) return true;
                    else{
                        if(this->mins>B.mins)return false;
                        else if(this->mins<B.mins) return true;
                        else return false;
                    }
                };
            }
        }
    }

    Time operator + (int x){
        this->mins += 1;
        if(this->mins==60) {this->mins=0;this->hours+=1;}
        if(this->hours==24) {this->hours=0;this->date+=1;if(this->dweek){this->dweek = (this->dweek+1)%7;}}
        if(this->month!=2&&(this->date>daysOfMonth[this->month])) {this->date=1;this->month+=1;}
        else if(this->month==2&&(this->date>(daysOfMonth[this->month])+ isleapyear())) {this->date=1;this->month+=1;}
        if(this->month ==13) {this->month=1;this->year +=1;}
        return *this;
    }

    void addoneday(){
        this->date+=1;
        if(this->month!=2&&(this->date>daysOfMonth[this->month])) {this->date=1;this->month+=1;}
        else if(this->month==2&&(this->date>(daysOfMonth[this->month])+ isleapyear())) {this->date=1;this->month+=1;}
        if(this->month ==13) {this->month=1;this->year +=1;}
    }

    void print_time(){
        printf("%04d%02d%02d%02d%02d", this->year,this->month,this->date,this->hours,this->mins);
    }

    int isleapyear(){
        return ((this->year % 4 == 0 && this->year % 100 !=0)||(this->year % 400 ==0 ));
    }
};


//// 重载运算符 <<
//ostream & operator << (ostream &out, const Time &T) {
//    out << T.year << T.month << T.date << T.hours << T.mins;
//    return out;
//}

class MineTime{
private:
    vector <int> mins;   // 分钟
    vector <int> hours;  // 小时
    vector <int> date;   // day of month
    vector <int> month;  // 月份
    vector <int> dweek;  // 当天星期几
    string cronstr; //命令行

    vector <Time> cronb;    // 任务

    void strPushInt(vector <int> &ans, string src){
        ans.clear();
        // 如果输入为* 则存入不可能输入数字999
        if (src == "*"){
            ans.push_back(999);
        }
        string temp;
        string endstr;
        int leng = src.length();

        for (int i = 0; i < leng; ++i) {
            int flag=0;
            temp = "";
            endstr = "";
            while(src[i] != ','  && src[i] != '\0'){
                if(src[i] == '-'){
                    flag = 1;
                }
                if (flag) {
                    while (src[i] != ',' && src[i] != '\0') {
                        endstr = endstr + src[i];
                        i++;
                    };
                    if (isalpha(temp[0])) temp = Minedict[temp];
                    if (isalpha(endstr[0])) endstr = Minedict[endstr];
                    for (int j = str2int(temp); j < str2int(endstr); ++j) {
                        ans.push_back(j);
                    }
                }
                temp = temp + src[i];
                i++;
            }
            if (isalpha(temp[0])) temp = Minedict[temp];
            ans.push_back(str2int(temp));
        }
    }


public:

    MineTime(){
        string temp;
        cin>>temp;
        strPushInt(this->mins, temp);
        cin>>temp;
        strPushInt(this->hours, temp);
        cin>>temp;
        strPushInt(this->date, temp);
        cin>>temp;
        strPushInt(this->month, temp);
        cin>>temp;
        strPushInt(this->dweek, temp);
        cin>>temp;  // 任务命令行
        this->cronstr=temp;
    }

    bool judgefunc(int now, vector<int> time){
        bool flag = false;

        for(vector<int>::iterator it=time.begin();it!=time.end();it++){
            if((*it)==999) flag=true;
            else if(now==(*it)) flag=true;
        }
        return flag;
    }

    void judge(Time now){
        bool flag = false;

        flag = judgefunc(now.mins, this->mins)&&judgefunc(now.hours,this->hours)&&judgefunc(now.date,this->date)
                && judgefunc(now.month, this->month) && judgefunc(now.dweek, this->dweek);

        if(flag){
            now.print_time();
            cout<<" "<<this->cronstr<<endl;
        }
        return;
    }

};

class solution{
private:
    // 1970年1月1日 星期4
    int yuanDay = 4;

    vector <MineTime> halo;

public:
    // 判断星期几
    int dayofweek(int date,int month,int year){
        int sum=0;
        Time A = Time(0,0,1,1,1970);
        Time B = Time(0,0,date,month,year);
        while(A<B){
            sum+=1;
            A.addoneday();
            //A.print_time();
            //cout<<endl;
        }
        return (sum%7 + yuanDay)%7;
    }

    void run(){
        int n;
        string str1,str2;
        while(cin>>n>>str1>>str2&&n){
            Time start = Time(str1);
            Time end = Time(str2);

            // 开始是星期几
            start.dweek = dayofweek(start.date, start.month, start.year);

            for (int i = 0; i < n; ++i) {
                halo.push_back(MineTime());
            }

            while(start<end){
                for(vector<MineTime>::iterator it=halo.begin(); it!=halo.end();it++){
                    (*it).judge(start);
                }
                start = start + 1;
            }
        }

    }

};




int main(){

#ifdef LOCAL
    freopen("data.in", "r", stdin);
    freopen("data.out", "w", stdout);
#endif
    Minedict["Jan"] = '1';
    Minedict["Feb"] = '2';
    Minedict["Mar"] = '3';
    Minedict["Apr"] = '4';
    Minedict["May"] = '5';
    Minedict["Jun"] = '6';
    Minedict["Jul"] = '7';
    Minedict["Aug"] = '8';
    Minedict["Sep"] = '9';
    Minedict["Oct"] = "10";
    Minedict["Nov"] = "11";
    Minedict["Dec"] = "12";
    Minedict["Sun"] = "0";
    Minedict["Mon"] = "1";
    Minedict["Tue"] = "2";
    Minedict["Wed"] = "3";
    Minedict["Thu"] = "4";
    Minedict["Fri"] = "5";
    Minedict["Sat"] = "6";

    daysOfMonth[1] = 31;
    daysOfMonth[2] = 28;
    daysOfMonth[3] = 31;
    daysOfMonth[4] = 30;
    daysOfMonth[5] = 31;
    daysOfMonth[6] = 30;
    daysOfMonth[7] = 31;
    daysOfMonth[8] = 31;
    daysOfMonth[9] = 30;
    daysOfMonth[10] = 31;
    daysOfMonth[11] = 30;
    daysOfMonth[12] = 31;

// 测试时间流逝
//    Time a = Time(56,23,28,2,1997);
//    while(1){
//        a.print_time();
//        a = a+1;
//        cout<<endl;
//    }
// 测试今天星期几
//    solution test = solution();
//    cout<<test.dayofweek(20,8,2019)<<endl;
//    return 0;

    solution ans = solution();
    ans.run();
}
