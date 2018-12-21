#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdio>
using namespace std;
typedef double db;
typedef long long ll;
const ll bz = 4096;
ll area = 128*1024*4;
ll epoch = 1000000;//ns

class block
{
public:
    ll time;
    ll id;
    block( ll t, ll i ): time(t), id(i){}
};

class LRU
{
public:
    int m, n, cnt;// m ʵ�ʴ�С n Ŀǰ�Ĵ�С
    ll *tag;
    ll *stp;
    LRU( int m, int n ):m(m), n(n){// 32768 16384
        tag = new ll[m];
        stp = new ll[m];
        for( int i = 0; i < m; i ++ ){
            tag[i] = -1;
            stp[i] = 0;
        }
        cnt = 0;
    }
    bool inc(){
        if( n == m ) return 1;
        n += 512;
        return 0;
    }
    int find( block a ){
        for( int i = 0; i < n; i ++ ){
            if( a.id == tag[i] ){
                stp[i] = a.time;
                return 0;
            }
        }
        return 1;
    }
    int ins( block a, block &b ){
        if( cnt == n ){
            ll mnid, mn = (1LL<<60);
            for( int i = 0; i < n; i ++ ){
                if( mn >= stp[i] ){
                    mn = stp[i];
                    mnid = i;
                }
            }
            b.id = tag[mnid];
            b.time = stp[mnid];
            tag[mnid] = a.id;
            stp[mnid] = a.time;
            return 2;
        }
        else{
            for( int i = 0; i < n; i ++ ){
                if( tag[i] == -1 ){
                    tag[i] = a.id;
                    stp[i] = a.time;
                    cnt ++;
                    return 1;
                }
            }
        }
    }
};

class BG
{
public:
    vector<block>q;
    ll ep;
};

class EAR
{
public:
    vector<BG>p[35];
    ll tot, up;
    EAR(){
        tot = 0;
        up = area/2;
    }
    bool find( block a ){
        for( int i = 0; i < 30; i ++ ){
            for( int j = 0; j < p[i].size(); j ++ ){
                for( int k = 0; k < p[i][j].q.size(); k ++ ){
                    if( p[i][j].q[k].id == a.id ){
                        return 0;
                    }
                }
            }
        }
        return 1;
    }
    void ins_bg( BG bg ){
        int i = 29;
        while( tot+bg.q.size() > up ){
            if( p[i].size() != 0 ){
                tot -= p[i][0].q.size();
                p[i].erase( p[i].begin() );
                i --;
                if( i < 0 ) i += 30;
            }
        }
        ll tmp = log( bg.q.size()+0.5 );
        p[tmp].push_back( bg );
    }
    void dec(){
        int i = 29;
        up -= 2*1024/4;
        while( tot > up ){
            if( p[i].size() != 0 ){
                tot -= p[i][0].q.size();
                p[i].erase( p[i].begin() );
                i --;
                if( i < 0 ) i += 30;
            }
        }
    }
    void del( block a ){
        int ii, jj;
        tot --;
        ii = jj = -1;
        for( int i = 0; i < 30; i ++ ){
            for( int j = 0; j < p[i].size(); j ++ ){
                for( int k = 0; k < p[i][j].q.size(); k ++ ){
                    if( p[i][j].q[k].id == a.id ){
                        p[i][j].q.erase( p[i][j].q.begin()+k );
                        ii = i; jj = j;
                        break;
                    }
                }
                if( ii != -1 || jj != -1 ) break;
            }
            if( ii != -1 || jj != -1 ) break;
        }
        if( ii != -1 || jj != -1 ){
            if( p[ii][jj].q.size() < pow( 2.0, ii ) ){
                BG tmp = p[ii][jj];
                p[ii].erase( p[ii].begin()+jj );
                if( tmp.q.size() != 0 ){
                    int i;
                    for( i = p[ii-1].size()-1; i >= 0; i -- ){
                        if( p[ii-1][i].ep < tmp.ep ){
                            p[ii-1].insert( p[ii-1].begin()+i+1, tmp );
                            break;
                        }
                    }
                    if( i == -1 ){
                        p[ii-1].insert( p[ii-1].begin()+i+1, tmp );
                    }
                }
            }
        }
    }
    void ins( block a ){
        int ii, jj;
        tot ++;
        ii = jj = -1;
        for( int i = 0; i < 30; i ++ ){
            for( int j = 0; j < p[i].size(); j ++ ){
                for( int k = 0; k < p[i][j].q.size(); k ++ ){
                    if( p[i][j].ep == a.time/epoch ){
                        p[i][j].q.push_back( a );
                        ii = i; jj = j;
                        break;
                    }
                }
                if( ii != -1 || jj != -1 ) break;
            }
            if( ii != -1 || jj != -1 ) break;
        }
        if( ii != -1 || jj != -1 ){
            if( p[ii][jj].q.size() >= pow( 2.0, ii+1 ) ){
                BG tmp = p[ii][jj];
                p[ii].erase( p[ii].begin()+jj );
                int i;
                for( i = p[ii+1].size()-1; i >= 0; i -- ){
                    if( p[ii+1][i].ep < tmp.ep ){
                        p[ii+1].insert( p[ii+1].begin()+i+1, tmp );
                        break;
                    }
                }
                if( i == -1 ){
                    p[ii+1].insert( p[ii+1].begin()+i+1, tmp );
                }
            }
        }
    }
};

int main()
{
    int i, j, k;
    char file[10] = "in.txt";
    freopen(file, "r", stdin);
    ll time, offset, siz;
    ll base = -1, pre;
    LRU gh( area, area );
    LRU my( area, area/2 );
    EAR ear;
    LRU bff( area, area );
    LRU bff2( area, area );
    EAR e;
    BG bg;
    ll tot = 0, hit1 = 0, hit2 = 0, hit3 = 0;
    FILE *c1 = fopen("out1.csv", "w");
    FILE *c2 = fopen("out2.csv", "w");
    FILE *c3 = fopen("out3.csv", "w");
    while( ~scanf("%lld%lld%lld", &time, &offset, &siz) ){
        if( base == -1 ){
            base = time;
            pre = 0;
        }
        time -= base;
        ++ tot;

        int ft = 0;
        for( ll x = offset/bz; x < (offset+siz+bz-1)/bz; x ++ ){
            block nw( time, x );
            block w(0, 0);
            int ret = gh.find(nw);
            if( ret == 0 ) continue;
            else{
                int ret = gh.ins( nw, w );
                //if( ret == 2 ) printf("xxx\n");
                ft = 1;
            }
        }
        if( ft ){
            fprintf(c1, "%lld %lld %lld\n", time, offset, siz);
        }
        else hit1 ++;

        if( time-pre >= epoch ){
            bg.ep = pre/epoch;
            for( i = 0; i < bff.n; i ++ ){
                if( bff.tag[i] != -1 ){
                    bg.q.push_back( block( bff.stp[i], bff.tag[i] ) );
                }
            }
            ear.ins_bg( bg );
            pre = time;
            printf("%lld epoch insert\n", tot);
        }

        int fl = 0;
        for( ll x = offset/bz; x < (offset+siz+bz-1)/bz; x ++ ){
            block nw( time, x );
            block w(0, 0);
            // not in LRU
            int ret = my.find( nw );
            if( ret == 0 ){
                continue;
            }
            // not in  ear
            ret = ear.find(nw);
            if( ret == 0 ){
                ear.del( nw );
                int ret1 = my.ins( nw, w );
                if( ret1 == 2 )
                    ear.ins( w );
                continue;
            }
            // in buffer
            fl = 1;
            ret = bff.find( nw );
            if( ret == 0 ) continue;
            ret = bff.ins( nw, w );
            if( ret == 2 ){
                printf("no space for in buffer\n");
                exit(1);
            }
        }
        if( fl ){
            fprintf(c3, "%lld %lld %lld\n", time, offset, siz);
        }
        else hit3 ++;

        if( 1.0*hit1/tot - 1.0*hit3/tot >= 0.1 && tot%1000 == 0 ){
            if( !my.inc() ){
                ear.dec();
            }
            printf("judge %lld 1 %.2f 3 %.2f\n", tot, 100.0*hit1/tot, 100.0*hit3/tot);
        }
        //printf("%lld ok\n", tot);
    }
    printf("LRU %.2f\n", 100.0*hit1/tot);
    printf("EAR+LRU %.2f\n", 100.0*hit3/tot);
    fclose(c1);
    fclose(c2);
    fclose(c3);
}