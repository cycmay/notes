#include <iostream>
#include <cstdio>
#include <deque>
#define LOCAL
using namespace std;

int main(){
#ifdef LOCAL
    freopen("data.in", "r", stdin);
    freopen("data.out", "w", stdout);
#endif
    int n=0;
    while(scanf("%d",&n)==1){
        getchar();
        string line;
        char temp;
        int m, q, ans;
        for(int i=0;i<n;i++){
            deque <int> A;
            deque <char> B;
            getline(cin, line);
            for(int i=0;i<line.length();i++){
                temp = line[i];
                if(temp-'0'>0&&temp-'0'<=9)
                    A.push_back(temp-'0');
                else if(temp=='x'||temp=='/') {
                    // 遇到高优先级 A队尾出队
                    m = A.back();
                    A.pop_back();
                    // 为计算结果 再次读入一个数据
                    q = line[++i]-'0';
                    if(temp=='x'){
                        ans = m*q;
                    }else{
                        ans = m/q;
                    }
                    // 将高优先级结果入A
                    A.push_back(ans);
                }else
                    // 低优先级运算符号入队
                    B.push_back(temp);
            }
            // 所有低优先级符号出
            while(!B.empty()){
                m = A.front();
                A.pop_front();
                q = A.front();
                A.pop_front();
                if(B.front()=='+')
                    m += q;
                else
                    m -= q;
                // 结果入A
                A.push_front(m);
                B.pop_front();
            }
            // A最后一个元素即为结果
            //cout<<A.front();
            puts(A.front()==24?"Yes":"No");
        }
    }
    return 0;
}