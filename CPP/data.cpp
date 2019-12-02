//#defin LOCAL
//#include <stdio.h>
//#define INF 1000000000
//int main() {
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//    int x, n = 0, min = INF, max = -INF, s = 0;
//    while(scanf("%d", &x) == 1) {
//        s += x;
//        if(x < min) min = x; if(x > max) max = x;
///*
// *   printf("x = %d, min = %d, max = %d\n", x, min, max);
//*/
//        n++;
//    }
//    printf("%d %d %.3f\n", min, max, (double)s/n);
//    printf("OK!");
//    return 0;
//}

//
//#define  LOCAL
//
//#include <stdio.h>
//#define INF 1000000000
//
//int main() {
//
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//    int s = 0;
//    for (int i = 1; i < 10; i++) {
//        for (int j = 0; j < 10; j++) {
//            for (int k = 0; k < 10; k++) {
//                s = i * 100 + j * 10 + k;
//                if (s == i * i * i + j * j * j + k * k * k) {
//                    printf("%d ", s);
//                }
//            }
//        }
//    }
//    return 0;
//}

//#define LOCAL
//
//#include <stdio.h>
//#define INF 1000000000
//
//int main(){
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//
//    int a,b,c,kase=0;
//    while(scanf("%d %d %d", &a,&b,&c) == 3){
//        int sum;
//        sum = 70*a + 21*b + 15*c;
//        if(sum%105 <= 100){
//            printf("Case %d: %d\n", ++kase, sum%105);
//        }
//        else{
//            printf("Case %d: No answer!\n", ++kase);
//        }
//    }
//    printf("OK!");
//    return 0;
//}


//#include<stdio.h>
//#include<math.h>
//#include<iostream>
//using namespace std;
//int main()
//{
////    freopen("data.in", "r", stdin);
////    freopen("data.out", "w", stdout);
//    int n, m, kase = 0;
//    while(scanf("%d%d", &n, &m) == 2 && !(n == 0 && m == 0))
//    {
//        if(n > m || n > 1e6 || m > 1e6)
//            break;
//        float sum = 0.0;
//        for(long long i=n; i<= m; i++)
//        {
//            sum += 1.0 / (i * i);
//        }
//        printf("case %d:%.5f", ++kase, sum);
//    }
//}
//
//#define LOCAL
//
//#include <iostream>
//using namespace std;
//
//int main(){
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//
//    int a,b,c,kase=0;
//    while((scanf("%d%d%d", &a,&b,&c) == 3)&&!(a==0&&b==0&c==0)&&c<100){
//        printf("Case: %d: %.*lf", ++kase, c, (double)a/b);
//    }
//
//}

//#include <stdio.h>
//#define maxn 105
//int a[maxn];
//
//int main(){
//    int x,n=0;
//    while(scanf("%d", &x) == 1){
//        a[n++] = x;
//    }
//    // 逆序输出
//    for(int i=n-1;i>=0;i--){
//        printf("%d ", a[i]);
//    }
//    return 0;
//}

//#include <stdio.h>
//#include <string.h>
//#define maxn 20
//int a[maxn][maxn];
//
//int main(){
//    int n,x,y,tot=0;
//    scanf("%d", &n);
//    memset(a,0, sizeof(a));
//
//    tot=a[x=0][y=n-1] = 1;
//    while (tot<n*n){
//        while((x+1<n) && (!a[x+1][y]))
//            a[++x][y] = ++tot;
//        while((y-1>=0) && (!a[x][y-1]))
//            a[x][--y] = ++tot;
//        while ((x-1>=0) && (!a[x-1][y]))
//            a[--x][y] = ++tot;
//        while ((y+1<n) && (!a[x][y+1]))
//            a[x][++y] = ++tot;
//    }
//    for(x=0;x<n;x++){
//        for(y=0;y<n;y++){
//            printf("%3d", a[x][y]);
//        }
//        printf("\n");
//    }
//
//    return 0;
//}
//

//#include<stdio.h>
//#include<string.h>
//int main() {
//    int count = 0;
//    char s[20], buf[99];
//    scanf("%s", s);
//    for(int abc = 111; abc <= 999; abc++)
//        for(int de = 11; de <= 99; de++){
//            int x = abc*(de%10), y = abc*(de/10), z = abc*de;
//            sprintf(buf, "%d%d%d%d%d", abc, de, x, y, z);
//            int ok = 1;
//            for(int i = 0; i < strlen(buf); i++)
//                if(strchr(s, buf[i]) == NULL) ok = 0;
//                if(ok) {
//                    printf("<%d>\n", ++count);
//                    printf("%5d\nX%4d\n-----\n%5d\n%4d\n-----\n%5d\n\n", abc, de, x, y, z);
//                }
//        }
//    printf("The number of solutions = %d\n", count);
//        return 0;
//}

//#include<stdio.h>
//int main() {
//    int c, q = 1;
//    while((c = getchar()) != EOF) {
//        if(c == '"') {
//            printf("%s", q ? "``" : "''");
//            q = !q;
//        }
//        else printf("%c", c); }
//    return 0;
//}
//
//#include <stdio.h>
//#define LOCAL
//#define maxn 1024
//
//int main(){
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//    int n, a[maxn], b[maxn];
//    int kase = 0;
//    while(scanf("%d", &n)==1 && n){
//        printf("Game %d:\n", ++kase);
//        for(int i=0; i<n; i++){
//            scanf("%d", &a[i]);
//        }
//        while(1){
//            int A=0,B=0;
//            for(int i=0;i<n;i++){
//                scanf("%d", &b[i]);
//                if(a[i]==b[i]){
//                   A++;
//                }
//            }
//            // 输入0结束程序
//            if(b[0]==0) break;
//            // 从数字1-9对数组a,b进行遍历
//            for(int d=1;d<=9;d++){
//                // 统计出现次数c1 c2，取min(c1, c2) 即
//                int c1=0,c2=0;
//                for(int i=0;i<n;i++){
//                    if(d==a[i]) c1++;
//                    if(d==b[i]) c2++;
//                }
//                // 求min(c1,c2)
//                if(c1<c2) B+=c1; else B+=c2;
//            }
//            printf("(%d,%d)\n", A, B-A);
//        }
//    }
//
//    return 0;
//}
//
//#include <stdio.h>
//#define LOCAL
//
//int main(){
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//
//    int flag=0;     // 标志是否连续O
//    int score=0;    // 得分
//    int num = 0;    // 一轮得分
//    char alph;
//    while(scanf("%c", &alph)==1){
//        if(alph == 'X') flag=0; else flag=1;
//        if(flag){
//            score += ++num;
//        }else {
//            num = 0;
//        }
//    }
//    printf("%d", score);
//}
//
//#include <iostream>
//#include <string>
//#include <map>
//
//#define LOCAL
//int main() {
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//    std::map<char, double> m;
//    m['C'] = 12.01;
//    m['H'] = 1.008;
//    m['O'] = 16.00;
//    m['N'] = 14.01;
//
//    char temp;
//    std::string str;
//    int i = 0;
//    int n = 0;
//    std::cin>>n;
//    for (int k = 0; k < n; ++k) {
//        std::cin>>str;
//        double result = 0.0;
//        int count = 0;
//        i = str.length();
//        for (int j = 0; j < i; j++) {
//            temp = str[j];
//            count = 0;
//            while(str[j + 1] > '0' && str[j + 1] <= '9') {     // 输入为数字
//                count = count * 10 + (str[j + 1] - '0');
//                j++;
//            }
//            count = count ? count:1;
//            result += m[temp]*count;
//
//        }
//
//
//        printf("%.3f\n", result);
//    }
//
//
//    return 0;
//}

//#include<stdio.h>
//#include<string.h>
//#define MaxL 1000
//int KMP_init(char *A) {
//    int i, j;
//    int P[MaxL];
//    P[0] = -1, i = 1, j = -1;
//    while(A[i]) {
//        while(j >= 0 && A[j+1] != A[i])
//            j = P[j];
//        if(A[j+1] == A[i])
//            ++j;
//        P[i] = j, ++i;
//    }
//    return j;
//}
//int main() {
//    int T;
//    char s[MaxL];
//    scanf("%d", &T);
//    while(T--) {
//        scanf("%s", s);
//        int l = strlen(s), t = KMP_init(s);
//        if(l%(l-t-1))
//            printf("%d\n", l);
//        else
//            printf("%d\n", l-t-1);
//        if(T)	puts("");
//    }
//    return 0;
//}

//
//#include<iostream>
//#include<cstdio>
//#include<cstring>
//using namespace std;
//char m[8][8];
//char mr[10050];
//int main()
//{
//    char c;int cc=1;
//    while(1)
//    {
//        memset(mr,'\0',sizeof(mr));//清空数组
//        if (cc>1) getchar();//让没有用的‘\n’滚蛋
//        c=getchar();
//        if (c=='Z') return 0;//其实，这里有个小漏洞，但是，AC了就好
//        if (cc!=1) cout<<endl;//按题意输出
//        m[1][1]=c;
//        for (int i=1;i<=5;i++)
//        {
//            int j;
//            for (j=1;j<=5;j++)
//            {
//                if(i==1&&j==1) continue;//第一个已经读过了，就不要再读了
//                m[i][j]=getchar();
//            }
//            getchar();//除掉‘\n’
//        }
//        int x,y;
//        for (int i=1;i<=5;i++)
//            for (int j=1;j<=5;j++)
//                if (m[i][j]==' ')//寻找到原始图像中的空格的位置
//                {
//                    x=i;
//                    y=j;//记录一下
//                }
//        int i=0;
//        while (1)//处理操作符
//        {
//            char x;
//            cin>>x;
//            mr[i++]=x;
//            if (x=='0') break;
//        }
//        int l=strlen(mr);bool flag=true;
//        for (int i=0;i<l-1;i++)//进行操作
//        {
//            char ch=mr[i],t;
//            if (ch=='A')
//            {
//                if(x-1<1)
//                {
//                    flag=false;
//                    break;
//                }
//                t=m[x-1][y];
//                m[x-1][y]=' ';
//                m[x][y]=t;
//                x=x-1;
//            }
//            else if (ch=='B')
//            {
//                if(x+1>5)
//                {
//                    flag=false;
//                    break;
//                }
//                t=m[x+1][y];
//                m[x+1][y]=' ';
//                m[x][y]=t;
//                x=x+1;
//            }
//            else if (ch=='L')
//            {
//                if(y-1<1)
//                {
//                    flag=false;
//                    break;
//                }
//                t=m[x][y-1];
//                m[x][y-1]=' ';
//                m[x][y]=t;
//                y=y-1;
//            }
//            else if (ch=='R')
//            {
//                if(y+1>5)
//                {
//                    flag=false;
//                    break;
//                }
//                t=m[x][y+1];
//                m[x][y+1]=' ';
//                m[x][y]=t;
//                y=y+1;
//            }
//            else
//            {
//                flag=false;
//                break;
//            }
//        }
//        printf("Puzzle #%d:\n",cc++);
//        if(flag==false)
//            printf("This puzzle has no final configuration.\n");
//        else
//            for(int i=1;i<=5;i++)
//            {
//                for(int j=1;j<=5;j++)
//                {
//                    if(j==1)
//                        cout<<m[i][j];
//                    else
//                        cout<<" "<<m[i][j];
//                }
//                cout<<endl;
//            }
//    }
//    return 0;
//}

//#include<iostream>
//#include<cstdio>
//#include<string>
//#include<stack>
//using namespace std;
//stack<int> num;
//stack<char> sign;
//int main()
//{
//    freopen("data.in", "r", stdin);
//    int n;
//    cin>>n;
//    while(n--)
//    {
//        string input;
//        cin>>input;
//        for(int i = 0; i < input.length(); i++)
//        {
//            if(input[i] >= '1' && input[i] <= '9')
//                num.push(input[i] - '0');
//            else if(input[i] == 'x' || input[i] == '/')
//            {
//                int t = i + 1;
//                int temp = num.top();
//                num.pop();
//                if(input[i] == 'x')
//                    num.push(temp * (input[t] - '0'));
//                else if(input[i] == '/')
//                    num.push(temp / (input[t] - '0'));
//                i++;
//            }
//            else
//                sign.push(input[i]);
//        }
//        while(!sign.empty())
//        {
//            char sig = sign.top();
//            sign.pop();
//            int temp1 = num.top();
//            num.pop();
//            int temp2 = num.top();
//            num.pop();
//            if(sig == '+')
//                num.push(temp1 + temp2);
//            else if(sig == '-')
//                num.push(temp2 - temp1);
//        }
//        if(num.top() == 24)cout<<"Yes";
//        else
//            cout<<"No";
//        if(n)cout<<endl;
//        num.pop();
//    }
//}

// DNA String. UVa1368

//#include <iostream>
//#include <stdio.h>
//#define LOCAL
//
//int bits[128];   // 数组形式保存字典数据
//
//char DNA[50][1024]; //DNA数据
//
//// 该函数判断参数c在bit数组中的个数是否为最大值
//int get_bit(char c){
//    return bits[c]>=bits['A']&&bits[c]>=bits['T']&&bits[c]>=bits['G']&&bits[c]>=bits['C'];
//}
//
//int main(){
//
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//
//    // t-数据组 m-字符串个数 n-字符串长度
//    int t,m,n;
//    while(scanf("%d",&t)==1){
//         while(t--){
//             int ans = 0;    // 汉明码长度
//             scanf("%d%d", &m,&n);
//             for(int i=0;i<m;i++){
//                 scanf("%s", DNA[i]);
//             }
//             // 统计每一列的众数
//             for(int i=0;i<n;i++){
//                 bits['A']=bits['T']=bits['C']=bits['G']=0;
//                 for(int j=0;j<m;j++){
//                    bits[DNA[j][i]]++;
//                 }
//                 if(get_bit('A')){
//                     printf("%c", 'A');
//                     ans += m-bits['A'];
//                 }else if(get_bit('C')){
//                     printf("%c", 'C');
//                     ans += m-bits['C'];
//                 }else if(get_bit('T')){
//                     printf("%c", 'T');
//                     ans += m-bits['T'];
//                 }else{
//                     printf("%c", 'G');
//                     ans += m-bits['G'];
//                 }
//             }
//             printf("\n%d\n", ans);
//         }
//    }
//    return 0;
//}
// 循环小数（Repeating Decimals, ACM/ICPC World Finals 1990, UVa202）
//#include <iostream>
//#include <stdio.h>
//
//#define LOCAL
//using namespace std;
//
//const int MAX = 3005;
//const int INF = 0x3f3f3f3f;
//
//int main(){
//    #ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//    int m,n,cou,i,t;
//    int fs[MAX];    //保存每次运算的商
//    int cx[MAX];    // 保存每次运算的被除数 即余数
//    while(cin>>n>>m){
//        printf("%d/%d=%d.", n,m,n/m);
//        cou=0;
//        memset(fs, 0, sizeof(fs));
//        memset(cx, 0, sizeof(cx));
//        t=n;
//        while(1){
//            t=t%m*10;   // 取每次除法运算余数
//            fs[cou] = t/m;  // 保存除法运算商
//            for(i=0;i<m;i++){
//                if(cx[i]==t) break;
//            }
//            if(i<cou) break;
//            cx[cou++] = t;  //保存余数
//        }
//        // 输出
//        for(int j=0;j<i;j++) cout<<fs[j];
//        cout<<'(';
//        if(cou>50){
//            // 超过50的长度 替换为...
//            for (int j = i; j < i+50; ++j) {
//                cout<<fs[j];
//            }
//            cout<<"...";
//        } else{
//            for (int j = i; j < cou; ++j) {
//                cout<<fs[j];
//            }
//        }
//        cout << ")\n";
//        printf("   %d = number of digits in repeating cycle\n\n", cou-i);
//
//    }
//    return 0;
//}

//#include <iostream>
//using namespace std;
//
//int sum(int a[]) {
//
//    int ans = 0; for(int i = 0; i < sizeof(a); i++) ans += a[i]; return ans;
//
//}
//
//int main(){
//    int a[10] = {1,2,3,4};
//    int sumq = sum(a);
//    cout<<sumq;
//    return 0;
//}

//Ancient Cipher UVA - 1339
// 这道题搞清楚 只有映射规律统一的两个字符串是yes
// 从结果出发 符合条件的字符串满足 两个字符串中包含的字符个数 在排序后应该是一致的
//#include <iostream>
//#include <stdio.h>
//#include <cstring>
//#define LOCAL
//using namespace std;
//
//
//int cmp(const void *a, const void *b){
//    // const void * 可以通过 (int *) 强制转换类型
//    return *(int *)a - *(int *)b;
//}
//
//int main(){
//
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//
//    char str1[105], str2[105];
//    while(~scanf("%s%s", str1,str2)){
//        int len = strlen(str1);
//        int flag=1;
//        int cnt1[26]={0}, cnt2[26]={0};
//        for (int i = 0; i < len; ++i) {
//            cnt1[str1[i]-'A']++;
//            cnt2[str2[i]-'A']++;
//        }
//        qsort(cnt1, 26, sizeof(int), cmp);
//        qsort(cnt2, 26, sizeof(int), cmp);
//        for (int j = 0; j < 26; ++j) {
//            if(cnt1[j]!=cnt2[j]){
//                flag=0;
//                break;
//            }
//        }
//        if (flag){
//            cout<<"YES"<<endl;
//        }else{
//            cout<<"NO"<<endl;
//        }
//    }
//
//    return 0;
//}

//Hangman Judge UVA - 489

//#include <iostream>
//#include <stdio.h>
//#include <string.h>
//using namespace std;
//#define LOCAL
//
//char str1[105];
//char str2[105];
//
//int win=0,lose=0;
//int strokes = 7;
//int mine_left=0;
//
//void guess(char c){
//    char *q=str1;
//    int bad = 1;
//    while(*q != '\0'){
//        if(*q==c){
//            mine_left--;
//            *q=' ';
//            bad=0;
//        }
//        q++;
//    }
//    if(bad) strokes--;
//    if(!mine_left) win=1;
//    if(!strokes) lose=1;
//}
//
//int main(){
//
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//
//    int rnd=-1;
//    while(scanf("%d%s%s",&rnd,str1,str2)==3&&rnd!=-1){
//        strokes=7;
//        win=lose=0;
//        mine_left = strlen(str1);
//        char *p=str2;
//        while(*p != '\0'){
//            guess(*p);
//            p++;
//        }
//        cout<<"Round "<<rnd<<endl;
//        if(strokes>0&&!win){
//            cout<<"You chickened out."<<endl;
//        }else if(strokes>0&&win){
//            cout<<"You win."<<endl;
//        }else{
//            cout<<"You lose"<<endl;
//        }
//    }
//    return 0;
//}

//The Dole Queue UVA - 133
// 约瑟夫环的变种

//#include <iostream>
//#define LOCAL
//
//using namespace std;
//
//int a[20];
//int n,k,m;
//int mine_left;
//int go(int p, int dir, int d){
//    // p 位置 dir-方向 d-步长
//    // dir : -1 下标递减 顺时针 1 下标递增 逆时针
//    while(--d) {
//        // 将下标指向下一个新的非0下标
//        while (a[p]==0){
//            p = (p + dir + n -1 ) % n + 1;
//        }
//        // 下标后移到后一个非0下标
//        do {
//            p = (p + dir + n -1 ) % n + 1;
//        } while (a[p] == 0);
//    }
//
//    return p;
//}
//
//int main(){
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//
//    while(scanf("%d%d%d",&n,&k,&m)==3&&n!=0){
//        mine_left = n;
//        a[0]=0; //模仿自然数形成的圈子
//        for (int i = 1; i <= n; ++i) {
//            a[i] = i;
//        }
//        int p1=1;// A offcial 1->n 步长k
//        int p2=n; // B offcial n->1 步长m
//        while(mine_left){
//            p1 = go(p1, 1, k);
//            p2 = go(p2, -1, m);
//            printf("%3d",a[p1]);
//            mine_left--;
//            if (p1 == p2 ){
//                // A与B指向同一个下标
//
//            }else{
//                printf("%3d",a[p2]);
//                mine_left--;
//            }
//            // p1 p2 出局
//            a[p1]=a[p2]=0;
//            if(mine_left>0){
//                printf(",");
//            }
//        }
//
//
//
//
//    }
//    return 0;
//}

// C++ 输入输出流
//#include <iostream>
//#include <sstream>
//#include <string>
//
//using namespace std;
//
//int main(){
//    string line;
//    while(getline(cin, line)){  //getline 读取一行数据
//        int  sum=0, x;
//        stringstream ss(line);  // 创建一个字符串流ss
//        while(ss>>x) sum+=x;    // 向cin 一样读取ss 返回值到x
//        cout<<sum<<endl;
//    }
//    return 0;
//}
//
//#include <iostream>
//using namespace std;
//
//struct Point{
//    int x,y;
//    Point(int x=0, int y=0) : x(x), y(y) {}
//};
//
//template <typename T>
//
//T sum(T *begin, T *end){
//    T *p = begin;
//    T ans=0;
//    for (T *p=begin; p < end ; ++p) {
//        ans = ans + *p;
//    }
//    return ans;
//}
//
//// 重载运算符 +
//Point operator + (const Point &A, const Point &B){
//    return Point(A.x+B.x, A.y+B.y);
//}
//
//// 重载运算符 <<
//ostream & operator << (ostream &out, const Point &P){
//    out << "(" << P.x << "," << P.y << ")";
//    return out;
//}
//
//
//int main(){
//    Point A, B(1,2);
//    A.x = 3;
//    cout<<A+B<<endl;
//
//    double a[] = {1.5, 2.2, 3.3, 4.4};
//    cout << sum(a, a+4) << endl;
//    Point b[] = { Point(1,2), Point(3,4), Point(5,6), Point(7,8) };
//    cout << sum(b, b+4) << "\n";
//
//    return 0;
//}
//
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <set>
//#include <cstdio>
//
//using namespace std;
//
//// 声明一个集合
//set <string> dict;
//
//int main(){
//
//    string s,buf;
//    while(cin>>s){
//        for(int i=0;i<s.length();i++){
//            if(!isalpha(s[i])) s[i] = ' ';
//            else
//                s[i] = tolower(s[i]);
//        }
//        stringstream ss(s);
//        while(ss>>buf) dict.insert(buf);
//
//    }
//
//    for(set <string> :: iterator it = dict.begin(); it != dict.end(); it++){
//        cout<<*it<<endl;
//    }
//
//
//    return 0;
//}

//
//#include <iostream>
//#include <string>
//#include <vector>
//#include <map>
//#include <algorithm>
//using namespace std;
//
//// 将单词s标准化 即去除符号数字内容
//string repr(const string &str){
//    int len = str.length();
//    string ans=str;
//    for(int i=0;i<len;i++){
//        ans[i] = tolower(ans[i]);
//    }
//    // 字符串排好序
//    sort(ans.begin(), ans.end());
//    return ans;
//}
//
//vector<string> words;
//map<string,int> cnt;
//
//int main(){
//    // 保存字符串数据 到vector words中
//    // 记录数据到字典map cnt中
//    string s;
//    while(cin>>s&&s!="#"){
//        words.push_back(s);
//        string r=repr(s);
//        if(!cnt.count(r)) {cnt[r]=0;}
//        // 统计排好序的r出现次数
//        cnt[r]++;
//    }
//    // 保存题目结果
//    vector <string> ans;
//    // 遍历words 找到里面的单词 在repr标准化后 结果在map cnt中只出现一次
//    for (int i=0;i<words.size();i++){
//        if(cnt[repr(words[i])]==1) {
//            ans.push_back(words[i]);
//        }
//    }
//
//    // 对结果ans 进行排序并输出
//    sort(ans.begin(), ans.end());
//    for (int j = 0; j < ans.size(); ++j) {
//        cout<<ans[j]<<endl;
//    }
//
//    return 0;
//}

//#include <iostream>
//#include <string>
//#include <queue>
//
//#include <cstring>
//using namespace std;
//
//struct Node{
//    bool have_valued;   // 是否被赋值了
//    int v;
//    Node *left, *right;
//    Node(): have_valued(false), left(NULL), right(NULL)  {}
//};
//
//Node *root;
//
//Node * newnode(){
//    return new Node();
//}
//
//bool failed = false;
//
//void addnode(int v, char *s){
//    int n=strlen(s);
//    Node *u=root;   // 从root开始初始化
//    for (int i = 0; i < n; ++i) {
//        if (s[i] == 'L'){
//            if (u->left==NULL) u->left = newnode();
//            u=u->left;
//        }
//        else if (s[i] == 'R'){
//            if (u->right==NULL) u->right = newnode();
//            u=u->right;
//        }
//        // 判断是否输入有误 即创建一个已存在的value
//        if (u->have_valued) failed = true;
//        u->v = v;
//        u->have_valued = true;
//    }
//}
//
//// 层次遍历算法bfs
//bool bfs(vector <int> &ans){
//    queue <Node *> q;
//    ans.clear();
//    q.push(root);   // 从root开始层次遍历
//    while(!q.empty()){
//        Node *u = q.front();    // 取队列的首部元素
//        q.pop();    //队首元素出队
//        if (!u->have_valued)    // 发现元素u没有值说明输入的二叉树有错
//            return false;
//        ans.push_back(u->v);    // 取u的值v作为结果保存在ans数据中
//        if(u->left != NULL) q.push(u->left);
//        if(u->right != NULL) q.push(u->left);
//    }
//    return true;
//}
//
//// 清除树 释放内存空间
//void remove_tree(Node *u){
//    if (u==NULL) return;
//    remove_tree(u->left);
//    remove_tree(u->right);
//    delete(u);
//}
//
//
//int main(){
//    string dir = "NESW";
//    cout<< strchr(dir, 'W') - dir <<endl;
//    return 0;
//}
//
//#include <iostream>
//using namespace std;
//// 迷宫问题
//
//struct Node{
//    int r,c;
//    char dir;
//    Node(int r,int c, char dir): r(r),c(c),dir(dir){};
//};
//
//const char * dirs = "NESW";
//const char * turns = "FLR";
//
//int dir_id(char c){ return strchr(dirs, c)-dirs;}
//int turn_id(char c){ return strchr(turns, c)-turns;}
//
//const int dr[] = {-1,0,1,0};
//const int dc[] = {0,1,0,-1};
//
//Node walk(const Node &u, int turn){
//    // turn 路口转向 FLR
//    int dir = u.dir;
//    // 向左L 顺时针翻转3个单位 相当一逆时针转1个单位
//    if (turn==1) dir = (dir+3)%4;
//    // 向右R 顺时针翻转1个单位
//    if (turn==2) dir = (dir+1)%4;
//    return Node(u.r+dr[dir], u.c+dc[dir], dir);
//}

//// 递归生成字典序
//
//#include <iostream>
//#include <cstdio>
////#define LOCAL
//using namespace std;
//
//void print_permutation(int n, int *A, int cur){
//    if (cur==n){
//        for (int i = 0; i < n; ++i) {
//            cout<<A[i];
//        }
//        cout<<endl;
//    }
//    else {
//        for (int i = 1; i <= n; ++i) {
//            int ok = 1;
//            for (int j = 0; j < cur; ++j) {
//                if (A[j]==i) ok=0;
//            }
//            if (ok){
//                A[cur]=i;
//                print_permutation(n,A,cur+1);
//            }
//        }
//    }
//}
//
//
//void print_subset(int n, int *A, int cur){
//    // 打印当前序列
//    for (int i = 0; i < cur; ++i) {
//        printf("%d", A[i]);
//    }
//    printf("\n");
//    // 确定当前元素的最小值
//    int s=cur?A[cur-1]+1:1;
//    for (int j = s; j < n; ++j) {
//        A[cur] = j;
//        print_subset(n, A, cur+1);
//    }
//}
//
//void print_subset2(int n, int s){
//    // 输出子集s的所有元素
//    for (int i=0;i<n;i++){
//        // & 取两个集合的交集
//        if (s&(1<<i)) printf("%d ", i);
//    }
//    printf("\n");
//}
//int main(){
//#ifdef LOCAL
//    freopen("data.in", "r", stdin);
//    freopen("data.out", "w", stdout);
//#endif
//
//    int A[1000];
//    //print_permutation(4,A,0);
//    //print_subset(6,A,0);
//    int n=2;
//    for(int i = 0; i < (1<<n); i++)
//        print_subset2(n, i);
////    printf("%d", 1<<2);
//    return 0;
//}
//

