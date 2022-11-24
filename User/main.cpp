#include <vector>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
int main() {

    putenv("PROC_TO_KILL=telegram-desktop,rythmbox");
    pid_t PID=fork();
    if(PID==NULL){
        execl("../../Killer/cmake-build-debug/Killer",
              "/HELP /NAME telegram-desktop /PID 21868 /ENVV /EXIT","/HELP /NAME telegram-desktop /PID 21868 /ENVV /EXIT");
    }
//    STARTUPINFO startupinfo = {sizeof startupinfo};
//    PROCESS_INFORMATION processInformation = {0};
//    CreateProcess(
//            "..\\..\\Killer\\cmake-build-debug\\Killer.exe",
//            "/HELP /NAME Telegram.exe /PID 21868 /ENVV /EXIT", NULL, NULL, FALSE, NULL, NULL, NULL, &startupinfo,
//            &processInformation
//    );//TODO only from cmd, not from Cline
    char str[200];
    //GetCurrentDirectory(sizeof str, str);
    putenv("PROC_TO_KILL");//TODO is it right way to remove environment variable?

}