#ifndef PROC_H
#define PROC_H



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <dirent.h>
#include <string>
#include <string.h>

using namespace std;

// 프로세스 데이타 저장
typedef struct _process_info
{
    char            username[32];  // 유저이름
    vector<string>  process;       // stat정보들이 들어간다
} process_info;

// 프로세스의 리스트를 유지하기 위한 자료구조
// 결국은 배열의 배열이다.
typedef vector<process_info> Plist;

class Proc
{
    private:
        int         Processnum;     // 현재 프로세스 갯수
        Plist       ProcList;       // 프로세스정보 리스트

    public:

        // 생성자및 소멸자
        Proc();
        ~Proc();

        void            MakeProcInfo();      // 프로세스 정보를 얻어온다.
        vector<string>  ProcParser(char *);  // stat파일을 파싱한다
        int             ProcNum();           // 현재프로세스 갯수를 넘겨준다.
        int             IsDigit(char *);     // 문자열이 숫자인지 확인한다.
        Plist           GetProcList();       // 프로세스정보 리스트를 되돌려준다.
};


#endif // PROC_H
