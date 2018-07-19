#include <cstdio>
#include <cstdlib>
#include <cstring>

int main()
{
    int ans = 0;
    int v[100];
    int g[100][100];
    memset(v,0,sizeof(v));
    memset(g,0,sizeof(g));

    int n = 48;


    FILE* f = fopen("output.txt","r");
    fscanf(f,"%d",&ans);

    for(int i=0;i<n;i++)
    {
        fscanf(f,"%d",&v[i]);
    }
    fclose(f);

    f = fopen("att48_d.txt","r");

    for(int i=1;i<n+1;i++)
    {
        for(int j=1;j<n+1;j++)
        {
            fscanf(f,"%d",&g[i][j]);
        }
    }

    fclose(f);

    int sum = 0;
    for(int i=1;i<n+1;i++)
    {
        sum+=g[v[i-1]][v[i]];
    }
    sum+=g[v[n-1]][v[0]];

    printf("%d %d\n",ans,sum);
    return 0;
}