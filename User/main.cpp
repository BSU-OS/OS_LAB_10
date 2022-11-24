#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
int main() {

    putenv("PROC_TO_KILL=telegram-desktop,rythmbox");
    pid_t PID=fork();
    if(PID==NULL){
        execl("../../Killer/cmake-build-debug/Killer",
              "/HELP /NAME telegram-desktop /PID 21868 /ENVV /EXIT","/HELP /NAME telegram-desktop /PID 21868 /ENVV /EXIT");
    }
    char str[200];
    putenv("PROC_TO_KILL");//TODO is it right way to remove environment variable?

}