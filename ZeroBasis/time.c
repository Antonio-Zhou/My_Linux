#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main()
{
        struct timeval tpstart,tpend;
        float timeuse;
        int i;
        gettimeofday(&tpstart,NULL);

        scanf("%d", &i);
              /*这里计算的是scanf函数的执行时间，就是从函数执行到命令行输入回车的时间。*/

        gettimeofday(&tpend,NULL);
        timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
        tpend.tv_usec-tpstart.tv_usec;
        timeuse/=1000000;
        printf("Used Time:%f\n",timeuse);
        return(0);
}
