//#define TEST_INPUT
//#define TEST_T
//#define TEST_ANT
//#define TEST_VALUE

#include<cstdio>
#include<cstring>
#include<thread>
#include<vector>
#include<cstdlib>
#include<algorithm>
#include<ctime>
#include<cmath>

using namespace std;

#define eps 0.000001     //浮点数精度
#define maxn 50         //最大支持50座城市


mutex lock;             //锁


int city = 48;          //城市数量
int num = 1000;       //蚂蚁数量
int iteration = 100;          //迭代次数
double alpha = 1;           //信息素比重
double beta = 2;            //开销比重
double evaporation = 0.997;   //蒸发速率
double Q = 100;             //信息素增加量


int G[maxn+2][maxn+2];      //开销，城市编号从1开始
double D[maxn+2][maxn+2];   //开销倒数
double T[maxn+2][maxn+2];   //信息素


int ans = 0x7fffffff;   //最优解
vector<int> res;        //最优路径


//print the result
void output()
{
    printf("the shortest circle: ");
    for(vector<int>::iterator p=res.begin();p!=res.end();p++)
    {
        printf("%d%s",*p,p==res.end()-1?"\n":" ");
    }
    printf("the shortest circle cost: %d\n",ans);
}

//output to the file
void output_file()
{
    FILE* f = fopen("output.txt","w");
    fprintf(f,"%d\n",ans);
    for(vector<int>::iterator p=res.begin();p!=res.end();p++)
    {
        fprintf(f,"%d%s",*p,p==res.end()-1?"\n":" ");
    }
    fclose(f);
}

//print the and T
void print_T()
{
    printf("\nT:\n");
    for(int i=1;i<city+1;i++)
    {
        for(int j=1;j<city+1;j++)
        {
            printf("%f%s",T[i][j],j==city?"\n":" ");
        }
    }
}

//初始化
void init()
{
    memset(G,0,sizeof(G));
    memset(D,0,sizeof(D));
    memset(T,0,sizeof(T));
    res.clear();
    FILE* f = NULL;
    f = fopen("att48_d.txt","r");


#ifdef TEST_INPUT
    fclose(f);
    f = fopen("input.txt","r");
    city = 9;
    num = 100;
    iteration = 100;
#endif

    for(int i=1;i<city+1;i++)
    {
        for(int j=1;j<city+1;j++)
        {
            fscanf(f,"%d",&G[i][j]);
        }
    }
    fclose(f);

    for(int i=1;i<city+1;i++)
    {
        for(int j=1;j<city+1;j++)
        {
            if(i==j) continue;
            D[i][j] = 1.0/G[i][j];       //初始D为开销的倒数
            T[i][j] = 1.0;              //初始信息素T为1
        }
    }
}


//[x,y]随机整数
int get_random_int(int x,int y)
{
    return (int)rand()%(y-x+1)+x;
}


//[x,y]随机小数
double get_random_float(int x,int y)
{
    return (double)rand()/(RAND_MAX)*(y-x) + x;
}


//蚂蚁
void ant(int id)
{
    vector<int> A;                                      //已经访问过
    vector<int> B;                                      //需要访问
    int s = get_random_int(1,city);
    A.push_back(s);                                     //随机选择起始城市
    for(int i=1;i<city+1;i++) if(i!=s) B.push_back(i);  //待访问城市列表

    while(!B.empty())
    {
        double sum = 0;
        double best_v = 0;
        vector<int>::iterator best_city;

        lock.lock();

        //获取所有去向的评估值之和
        for(vector<int>::iterator p=B.begin();p!=B.end();p++) sum+=(pow(T[s][*p],alpha)*pow(D[s][*p],beta));

        //评估每一个需要被访问的城市
        for(vector<int>::iterator p=B.begin();p!=B.end();p++) {
            //当前城市
            int c = *p;

            //当前城市评估值
            double v = pow(T[s][c], alpha) * pow(D[s][c], beta) / sum;

            if (v > best_v) {
                best_city = p;
                best_v = v;

            }

            //评估值越大的城市越有几率成为下一个访问城市
            double r = get_random_float(0, 1);

#ifdef TEST_VALUE
            printf("ant:%d    last:%d    now:%d   r:%f      v:%f\n", id, *(A.end() - 1), c, r, v);
#endif
            if (r <= v + eps) {                         //这个比较值需要修改
                A.push_back(c);
                B.erase(p);
                break;
            }
        }

        lock.unlock();
    }

    //蚂蚁走完了所有城市
    int sum = 0;

    //获取路径总花费
    for(vector<int>::iterator p=A.begin()+1;p!=A.end();p++)
    {
        sum+=G[*(p-1)][*p];

#ifdef TEST_ANT
        printf("ant:%d   sum:%d   s:%d t:%d\n",id,sum,now,*p);
#endif

    }
    sum+=G[*(A.end()-1)][*(A.begin())];

#ifdef TEST_ANT
    printf("ant:%d   sum:%d   s:%d t:%d\n",id,sum,*(A.end()-1),*(A.begin()));
#endif

    //更新信息素
    lock.lock();
    for(vector<int>::iterator p=A.begin()+1;p!=A.end();p++)
    {
        T[*(p-1)][*p]+=(Q/sum);
    }
    T[*(A.end()-1)][*(A.begin())]+=(Q/sum);

    //如果当前最佳路径为空，则用当前路径初始化最佳路径
    if(res.empty())
    {
        res = A;
        ans = sum;
    }
    else if(sum < ans)
    {
        res = A;
        ans = sum;
    }

#ifdef TEST_T
    printf("\nant:%d\n",id);
    print_T();
#endif

    lock.unlock();
}


int main()
{
    srand((unsigned)time(0));
    init();
    for(int k=0;k<iteration;k++)
    {
#ifdef TEST_T
        printf("iteration:%d\n",k);
#endif

        vector<thread> t;
        for(int i=0;i<num;i++) t.push_back(thread(ant,i));
        for(int i=0;i<num;i++) t[i].join();

        for(int i=1;i<city+1;i++)
        {
            for(int j=1;j<city+1;j++)
            {
                T[i][j]*=evaporation;
            }
        }
    }
    output();
    output_file();
    return 0;
}