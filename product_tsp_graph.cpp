#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(0));
    int G[10][10];
    memset(G,0,sizeof(G));
    FILE* f = fopen("input.txt","w");

    G[0][5] = G[5][6] = G[6][7] = G[7][8] = G[8][3] = G[3][2] = G[2][1] = G[1][4] = G[4][0] = 2000;

    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            if(i==j) fprintf(f,"0%s",j==8?"\n":" ");
            else if(G[i][j]==0) fprintf(f,"%d%s",2000+(int)rand()%6000,j==8?"\n":" ");
            else fprintf(f,"%d%s",2000,j==8?"\n":" ");
        }
    }

    fclose(f);
    return 0;
}