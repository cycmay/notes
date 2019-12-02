#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <string>
#include <map>

#define LOCAL

using namespace std;

int main(){
#ifdef LOCAL
    freopen("data.in", "r", stdin);
    freopen("data.out", "w", stdout);
#endif

    int m,n;    // m 是行 n替换数据
    // 模板{{}} 正则表达式
    // 匹配结果 result[0] {{ name }}  result[1] name
    vector <regex> stand;

    while (scanf("%d %d", &m,&n)==2){
        getchar();  // 回车符
        string line;
        string key, value, temp;

        vector <string> html;
        map <string, string> repalce;
        for (int i = 0; i < m; ++i) {
            getline(cin, line);
            html.push_back(line);
        }
        // 输入键值对
        for (int j = 0; j < n; ++j) {
            temp = "";
            value = "";
            getline(cin, line);
            stringstream ss(line);
            ss>>key;
            // 更新模板正则
            string temp = "\\{\\{ ("+ key +") \\}\\}";
//            cout<<temp<<endl;
            stand.push_back(regex(temp));
            // 输入后面所有内容
            while(ss>>temp)
                value += " "+temp;
            // 去掉首和尾"符号
            value = value.substr(2, value.length()-3);
            repalce[key]=value;
        }
        smatch sm;
        for(string &k:html){
            for(regex &j:stand) {
                while (regex_search(k.cbegin(), k.cend(), sm, j)) {
//                    for (int i = 0; i < sm.size(); i++)
//                        cout << sm[i] << endl;
                    // 替换匹配到的模板变量
                    k = regex_replace(k, j, repalce[(string) (sm[1])]);
                }
            }
            // 未知属性 清空
            k = regex_replace(k, regex("\\{\\{ ([^\\}]+) \\}\\}"), "");
        }
        for (string &k:html){
            cout<<k<<endl;
        }

    }
//    cout<<regex_search("<p>Email: <a href=\"mailto:{{ email }}\">{{ name }}</a></p>", stand.front())<<endl;
    return 0;
}


