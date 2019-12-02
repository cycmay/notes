#include "bits/stdc++.h"
#define LOCAL
#define MAX_N 1000005
using namespace std;

int main() {
#ifdef LOCAL
    freopen("data.in", "r", stdin);
    freopen("data.out", "w", stdout);
#endif
    unsigned int n;
    while(scanf("%d", &n)==1){
    	unsigned int union_a [MAX_N]={0};
    	unsigned int a,b;
    	for (int i = 1; i <= n; ++i)
    	{	
    		scanf("%d %d", &a,&b);
    		// 标记小H上货时间为1
    		while(a<=b){
    			union_a[a] += 1;
    			a++;
    		}
    	}
    	for (int i = 1; i <= n; ++i)
    	{
    		scanf("%d %d", &a,&b);
    		// 标记小M上货时间为2
    		while(a<=b){
    			union_a[a] += 2;
    			a++;
    		}
    	}
    	int ans;
    	// 统计上货标记为3的即为交集时间
    	for (int i = 0; i < MAX_N; ++i)
    	{
    		if(union_a[i]==3)
    			ans += 1;
    	}
    	cout<<ans<<endl;
    }
    return 0;
}