#include <iostream>
#include <string>
#include <stack>
#include <vector>
#define LOCAL
using namespace std;

class Json{
    // Json 对象
private:

public:
    string key="";  // 键值对 的键
    string value="";    // 键值对 值
    bool is_string = false;  // 是否为字符串 否则就是值为新的json对象
    vector <Json> children;

    void print_json(){
        cout<<'{'<<endl;
        for(vector <Json>::iterator it=this->children.begin(); it!=this->children.end();it++){
            Json son = *it;
            cout<<"\""<<son.key<<"\": ";
            if(son.is_string) cout<<"\""<<son.value<<"\",\n";
            else {
                son.print_json();
            }
        }
        cout<<'}'<<endl;
    }
    // 获取子节点
    int get_attribute(Json &root, Json &ans, string key){
        for (vector <Json>::iterator it = root.children.begin(); it!=root.children.end(); it++)
        {
            if((*it).key == key){
                ans = *it;
                return 1;
            }
        }
        return 0;
    }
    void find_key(Json &Node,string key){

        string temp;
        if(key.find('.')!=key.npos){
            // 返现 .  截取直到后面
            temp = key.substr(0, key.find('.'));
            Json son;
            get_attribute(Node, son, temp);
            find_key(son, key.substr(key.find('.')+1));
        }else{
            bool flag = false;
            for (vector <Json>::iterator it = Node.children.begin(); it!=Node.children.end(); it++)
            {
                if((*it).key == key){
                    flag = true;
                    if((*it).is_string){
                        cout<<"STRING "<<(*it).value<<endl;
                    }else
                    {
                        cout<<"OBJECT"<<endl;
                    }
                    break;
                }
            }
            if(!flag)
                cout<<"NOTEXIST"<<endl;
            return;
        }
    }
    Json(){

    }
    ~Json(){

    }
};


class Solution{
private:
    // 格式化输入字符 去掉转义
    string formatstr(string &src){
        string ans;
        int temp_len = src.length();
        int i=0;
        while(src[i]!='\0'){
            if(src[i]=='\\'&&src[i+1]!='\\'){
                i++;
                continue;
            }else if(src[i]=='\\'&&src[i+1]=='\\')
            {
                ans += src[i];
                i+=2;
                continue;
            }
            ans += src[i];
            i++;
        }
        return ans;
    }

public:
    // 更新数据
    void build_json(Json &root){

        string temp;
        while(getline(cin, temp)){
            // 去掉前导空格
            replaceFrontSpace(temp);
            if(temp[0]=='}') break;
            Json child = Json();
            string c_key;   // 子对象key
            int temp_len = temp.length();
            int count=0;  // 用来统计“”符号
            // 找到temp的:符号 取开始到:符号之间的内容
            int i=temp.find(':');
            c_key = temp.substr(1, i-2);
            c_key = formatstr(c_key);
            child.key = c_key;
            // 判断子对象类型
            if(temp.find('{')==string::npos){
                child.is_string = true;
                // c_value 串
                string c_value = temp.substr(i+1, temp.rfind('\"')-i-1);
                // 去掉后面的字符串前导空格
                replaceFrontSpace(c_value);
                c_value = c_value.substr(1);
                child.value = formatstr(c_value);
            }else{
                child.is_string = false;
                build_json(child);
            }
            root.children.push_back(child);
        }
    }
    void replaceFrontSpace(string &src){
        // 去掉前导空格
        int leg = src.length();
        for(int i=0;i<leg;i++){
            if(src[i]!=' '){
                src = src.substr(i);
                break;
            }
        }
    }
    void run(){
        int n,m;
        string src;
        while(scanf("%d%d", &n,&m)==2){
            cin.get();
            getline(cin, src);
            // 去掉前导所有空格
            replaceFrontSpace(src);
            if(src[0]=='{'){
                Json Mine = Json();
                build_json(Mine);
                Mine.print_json();
                while(m--){
                    getline(cin, src);
                    Mine.find_key(Mine, src);
                }
            }
        }
    }
};

int main(){
// 测试json输入数据
#ifdef LOCAL
    freopen("data.in", "r", stdin);
    freopen("data.out", "w", stdout);
#endif
    Solution test;
    test.run();
    return 0;
}