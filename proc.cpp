#include "proc.h"
#include <iostream>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
Proc::Proc()
{
}

Proc::~Proc()
{
}

/*
 * 프로세스 정보를 가져온다.
 * 그러기 위해서 /proc/[PID]/stat파일을 읽어들이고 이를
 * 필드별로 파싱한다. 파싱은 " "문자를 기준으로 한다.
 * 또한 프로세스를 생성한 유저 이름도 얻어온다.
 */
void Proc::MakeProcInfo()
{
    DIR *directory;
    struct dirent *entry = NULL;
    char proc_file[40];
    vector<string> procinfo;
    process_info lprocess_info;
    struct stat lstat;

    ProcList.clear();

    // proc 디렉토리를 열어서 파일(디렉토리포함)의 리스트를
    // 얻어온다.
    if ((directory = opendir("/proc")) == NULL)
    {
        perror("opendir error :");
        exit(0);
    }

    while((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") !=0 &&
            strcmp(entry->d_name, "..") != 0)
        {
            sprintf(proc_file,"/proc/%s/stat", entry->d_name);
            // stat 파일이 존재하는지 확인하고 확인하고
            if (access(proc_file, F_OK) != 0)
            {
                continue;
            }

            // 디렉토리 이름이 숫자인지 확인한다.
            // 디렉토리 이름이 숫자라면 이를 파싱한다.
            // 또한 stat파일의 소유 UID를 이용해서 유저이름도 얻어온다.
            if (IsDigit(entry->d_name))
            {
                struct passwd *upasswd;
                stat(proc_file,&lstat);
                lprocess_info.process  = ProcParser(proc_file);

                upasswd = getpwuid(lstat.st_uid);
                strncpy(lprocess_info.username, upasswd->pw_name, 32);
                if(atoi(lprocess_info.process[0].c_str()) == atoi(entry->d_name))
                {
                    ProcList.push_back(lprocess_info);
                }
            }
            else
            {
            }
        }
    }

}

/*
 * MakeProcInfo를 통해만들어진 프로세스정보 리스트를 되돌려준다.
 */
Plist Proc::GetProcList()
{
    return ProcList;
}

/*
 * 프로세스의 갯수를 되돌려준다.
 * 갯수는 프로세스자료구조(vector)의 원소의 크기가 된다.
 */
int Proc::ProcNum()
{
    return ProcList.size();
}

/*
 * stat 파일을 열어서 " "문자를 기준으로 파싱해서
 * 배열(vector)에 집어넣는다.
 */
vector<string> Proc::ProcParser(char *proc_file)
{
    FILE *fp;
    char buf[512] = {0x00,};
    char field[80];
    int index = 0;
    unsigned int i, j = 0;
    vector<string> vproc;

    fp = fopen(proc_file, "r");
    if (fp == NULL)
    {
        perror("error : ");
        exit(0);
    }

    memset(field, 0x00, 80);
    fgets(buf, 511, fp);
    for(i = 0; i < strlen(buf); i++)
    {
        if (buf[i] != ' ' && i != (strlen(buf) -1))
        {
            field[j++] = buf[i];
        }
        else
        {
            if (index == 1)
            {
                field[strlen(field)-1] = 0x00;
                vproc.push_back(field+1);
            }
            else
                vproc.push_back(field);
            memset(field, 0x00, 80);
            j = 0;
            index++;
        }
    }
    fclose(fp);
    return vproc;
}

/*
 * 파일이름이 숫자인지 확인한다.
 */
int Proc::IsDigit(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i])==0)
            return 0;
    }
    return 1;
}
