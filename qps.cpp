// Local 헤더파일
#include "qps.h"

// 표준 C++ 헤더파일들
#include <iostream>
#include <vector>
#include <string>

// 표준 C 헤더파일
#include <stdio.h>

using namespace std;

/*
 * 프로세스정보 리스트를 얻어와서 보기좋게 출력한다.
 */
int viewProc()
{
    Proc *mproc;
    unsigned i;
    int pcpu;
    int seconds = 0;

    Plist ProcList;
    mproc = new Proc;
    mproc->MakeProcInfo();

    // 프로세스정보 리스트를 얻어온다.
    ProcList = mproc->GetProcList();

    int total_cpu = 0;

    // OS의 uptime을 얻어온다.
    // 얻어온 정보는 프로세스의 CPU 사용율을 구하기 위한 기초정보로
    // 사용된다.
    seconds = uptime();
    printf("%-10s %7s %7s %2s %16s %4s %9s\n", "USER", "PID", "PPID",
                                              "ST", "NAME", "CPU", "VMEM");
    printf("==============================================================\n");
    for (i = 0; i < mproc->ProcNum(); i++)
    {
        // CPU사용율을 얻어온다.
        pcpu = getcputime(atol(ProcList[i].process[13].c_str()),
            atol(ProcList[i].process[14].c_str()),
            atol(ProcList[i].process[21].c_str()), seconds);

        // 보기좋게 출력한다.
        printf("%-10s %7s %7s %2s %16s %2d.%d %9s\n", ProcList[i].username,
                                    ProcList[i].process[0].c_str(),
                                    ProcList[i].process[3].c_str(),
                                    ProcList[i].process[2].c_str(),
                                    ProcList[i].process[1].c_str(), pcpu/10, pcpu%10,
                                    ProcList[i].process[22].c_str());
    }
    printf("\n");
    printf("컴퓨터 사용분 : :%d\n",seconds/60);
    return 1;
}

/*
 * 프로세스의 CPU사용율을 얻기 위해서 사용한다.
 * utime     : 유저모드에서 프로세스가 스케쥴링되면서 사용한 jiffies 수이다.
 *             프로세스가 스케쥴링될때마다 증가되는 수치이다.
 * stime     : 커널모드에서 프로세스가 스케쥴링되면서 사용한 jiffies 수이다.
 * starttime : 운영체제가 시작되고나서 몇 jiffies가 지난후
 *             프로세스가 시작되었는지
 */
int getcputime(ulong utime, ulong stime, ulong starttime, int seconds)
{
    unsigned long long total_time;
    int pcpu=0;

    // 유저 jiffies 와 커널 jiffies를 더한다.
    total_time = utime + stime;

    // 100은 HZ값이다. HZ이 작아질수록 context switching가 빨라진다.
    // 이값은 /usr/include/asm/param.h에 선언되어 있다.
    // 100. 대신에 (unsigned long long)HZ 정도로 코드를
    // 작성하는게 좀더 안전할것이다. 여기에서는 직관적으로 설명하기
    // 위해서 하드코딩했다.
    seconds = seconds - (int)(starttime/100.);

    if(seconds)
    {
        pcpu = (int)(total_time * 1000ULL/100.)/seconds;
    }

    return pcpu;
}

/*
 * 운영체제가 부팅하고 나서 얼마의 시간이 지났는지
 */
int uptime()
{
    FILE *fp;
    char buf[36];
    double stime;
    double idletime;

    if ((fp = fopen("/proc/uptime", "r")) == NULL)
    {
        perror("fopen error : ");
        exit(0);
    }
    fgets(buf, 36, fp);
    sscanf(buf, "%lf %lf", &stime, &idletime);
    fclose(fp);

    return (int)stime;
}
