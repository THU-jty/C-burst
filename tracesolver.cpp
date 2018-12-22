#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <set>
using namespace std;
typedef double db;
typedef long long ll;
const ll bz = 4096;
ll area = 128*1024*4;
ll epoch = 1000000;//ns

int main()
{
    FILE *c[4], *w[4];
    c[1] = fopen("out1.csv", "r");
    c[2] = fopen("out2.csv", "r");
    c[3] = fopen("out3.csv", "r");

    w[1] = fopen("t1.csv", "w");
    w[2] = fopen("t2.csv", "w");
    w[3] = fopen("t3.csv", "w");
    ll pre = 0, time, siz, offset;
    vector<ll>p[4];
    for( int i = 1; i <= 3; i ++ ){
        ll pre = 0;
        while( ~fscanf(c[i], "%lld%lld%lld", &time, &offset, &siz) ){
            p[i].push_back( time-pre );
            pre = time;
        }
        sort( p[i].begin(), p[i].end() );
        for( int j = 0; j < p[i].size(); j ++ ){
            fprintf( w[i], "%.6f,%lld\n", j*1.0/p[i].size(), p[i][j] );
        }
        printf("%d: siz %d 50%% %lld\n", i, p[i].size(), p[i][ p[i].size()*0.5+1 ]);
        printf("%d: siz %d 80%% %lld\n", i, p[i].size(), p[i][ p[i].size()*0.8+1 ]);
    }

    fclose(c[1]);
    fclose(c[2]);
    fclose(c[3]);

    fclose(w[1]);
    fclose(w[2]);
    fclose(w[3]);
}
