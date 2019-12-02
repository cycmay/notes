/*
#include <iostream>
#include <string>
#include <queue>
#define LOCAL
using namespace std;

class Solution{
private:
    void earseFrontSpace(string &src){
        int leg = src.length();
        int i=0;
        for (; i < leg; ++i) {
            if(src[i]!=' ') break;
        }
        src = src.substr(i);
    }

public:

    void deal_title(string &line){
        int level=0;
        while(line[level]=='#'&&level<6) level++;
        line = line.substr(level);
        this->earseFrontSpace(line);
        cout<<"<h"<<level<<">"<<line<<"</h"<<level<<">"<<endl;
    }

    void deal_table(string &line){
        line = line.substr(1);
        this->earseFrontSpace(line);
        cout<<"<li>"<<line<<"</li>"<<endl;

    }
    void href(string &line){
        int cur = 0, leg = line.length();
        while(cur<leg&&line[cur]!='[') cur++;
        // 未检测到[标志 退出
        if(cur==leg)
            return;
        string text,link;
        // 保存符号内值
        int start = cur;
        cur++;
        while(cur<leg&&line[cur]!=']') {
            text += line[cur];
            cur++;
        }
        while(line[cur]!='(') cur++;
        cur++;
        while(line[cur]!=')'){
            link += line[cur];
            cur++;
        }
        string next = line.substr(cur+1);
        href(next);
        line = line.substr(0, start) + "<a href=\"" + link + "\">" + text + "</a>"+next;

    }

    void mark(string &line){
        string temp;
        int cur = 0, leg = line.length();
        while(cur<leg&&line[cur]!='_') cur++;
        // 未检测到_标志 退出
        if(cur==leg)
            return;
        // 保存符号内值
        int start = cur;
        cur++;
        while(cur<leg&&line[cur]!='_') {
            temp += line[cur];
            cur++;
        }
        string next = line.substr(cur+1);
        mark(next);
        line = line.substr(0,start)+ "<em>" + temp + "</em>" + next;
    }

    void deal_line(string &line){
        // 处理line 解决递归问题
        int pos_mark = line.find('_');
        int pos_href = line.find('[');
        if(pos_mark!=string::npos&&pos_href!=string::npos){
            if(pos_mark<pos_href){
                href(line);
                mark(line);
            } else{
                mark(line);
                href(line);
            }
        }else if(pos_mark!=string::npos){
            mark(line);
        }else if(pos_href!=string::npos){
            href(line);
        }
        return;
    }

    void run(){
        string line;
        while(getline(cin, line)){
            if(line.length()==0) continue;
            // 出去行内标记
            deal_line(line);
            if(line[0] == '#'){
                deal_title(line);
                continue;
            }else if(line[0]=='*'){
                cout<<"<ul>"<<endl;
                deal_table(line);
                while(getline(cin, line)&&line[0]=='*')
                    deal_table(line);
                cout<<"<\\ul>"<<endl;
                continue;
            }
            // 输入的是段落
            cout<<"<p>"<<line;
            queue <string> strque;
            while(getline(cin, line)&&line[0]!='*'&&line[0]!='#'&&line.length()!=0) {
                // 出去行内标记
                deal_line(line);
                strque.push(line);
            }
            if(!strque.empty()){
                // 队列有内容
                cout<<endl;
                while(!(strque.size()<=1)){
                    cout<<strque.front()<<endl;
                    strque.pop();
                }

                cout<<strque.front();
            }
            cout<<"</p>"<<endl;
            if(line.length()==0) continue;
            if(line[0] == '#'){
                deal_title(line);
                continue;
            }else if(line[0]=='*'){
                cout<<"<ul>"<<endl;
                deal_table(line);
                while(getline(cin, line)&&line[0]=='*')
                    deal_table(line);
                continue;
            }
        }

    }
    Solution(){

    }
    ~Solution(){

    }
};

int main(){

#ifdef LOCAL
freopen("data.in", "r", stdin);
freopen("data.out", "w", stdout);
#endif
    Solution test = Solution();
//    string line;
//    getline(cin, line);
    // 测试href mark 功能
//    test.href(line);
//    cout<<line<<endl;
//    getline(cin, line);
//    test.mark(line);
//    cout<<line<<endl;
    // 测试嵌套输出
//    test.deal_line(line);
//    cout<<line<<endl;
    // 测试最终功能
    test.run();
    return 0;
}
*/


//#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#define LOCAL
using namespace std;

int main(){

#ifdef LOCAL
    freopen("/Users/bicycle/CLionProjects/untitled/data.in", "r", stdin);
    freopen("/Users/bicycle/CLionProjects/untitled/data.out", "w", stdout);
#endif
    // trans 容器保存涉及到的所有正则表达式
    vector <pair <regex, string> > trans = {
            {regex("^\\* +(.*)"),"<li>$1</li>"},
            {regex("_([^_]+)_"), "<em>$1</em>"},
            {regex("\\[([^\\]]+)\\]\\(([^\\)]+)\\)"),"<a href=\"$2\">$1</a>"},
    };
    for(int i=1;i<=6;i++){
        trans.push_back({regex("^"+ string(i,'#') + " +(.*)"),"<h" + to_string(i) + ">$1</h" + to_string(i) +">"});
    }
    // 保存输入区块
    vector <vector <string> > markdown(1);
    string s="";
    while (getline(cin, s)){
        if(s==""){  // 遇到空行
            markdown.push_back({});
        }else{
            markdown.back().push_back(s);   // 保存本行s入区块
        }
    }
    for (auto &i:markdown){
        if (i.empty()) continue;    // 区块为空
        int type = (i[0][0]=='*')?0:(i[0][0]=='#')?1:2;
        // type 0,1,2 分别代表无序 标题 段落
        printf("%s", (type==0)?"<ul>\n":(type==2)?"<p>":"");
        for(int j=0;j<i.size();j++){
            // 遍历已有的正则表达式
            for(auto &k:trans){
                // regex_replace(容器, 正则表达式匹配对象, 匹配结果替换字符串格式)
                i[j] = regex_replace(i[j], k.first, k.second);
            }
            // 区块i的输出 到区块最后一行输出一个换行符
            printf("%s%s", i[j].c_str(), j==(i.size()-1)?"":"\n");
        }
        // 输出区块结尾标志符
        puts(type==0?"\n</ul>":type==2?"</p>":"");
    }
    return 0;
}