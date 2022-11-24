#include <iostream>
#include <string>
#include <sys/types.h>
#include <signal.h>
#include <vector>
#include <regex>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

std::istringstream input;

int TerminateProcessByPID(int PID) {
    bool isTerminated = kill(PID, SIGKILL);
    return 1 + isTerminated;//TODO return
}

int TerminateProcessByName(std::string processName) {

    int numTerminatedProcess = 0;
    pid_t PID;
    size_t i, j;
    char *s = new char[264];
    char buf[128];
    FILE *st;
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        free(s);
        return 0;
    }
    struct dirent *f;
    while ((f = readdir(dir)) != NULL) {
        //std::cout << f->d_name << '\n';
        if (f->d_name[0] == '.')
            continue;

        for (i = 0; std::isdigit(f->d_name[i]); ++i);
        if (i < strlen(f->d_name)) continue;
        strcpy(s, "/proc/");
        strcat(s, f->d_name);
        strcat(s, "/status");
        st = fopen(s, "r");
        if (st == NULL)
            continue;
        if (fgets(buf, 128, st) == NULL) {
            continue;
        }
        fclose(st);

        for (j = 5; std::isspace(buf[j]); j++);
        *strchr(buf, '\n') = 0;
        std::string bufStr(&(buf[j]));
        //std::cout << bufStr << '\n';
        auto result= std::mismatch(bufStr.begin(),bufStr.end(),processName.begin());
        //std::cout << buf << ' ' << strcmp(&(buf[j]), processName.c_str()) << '\n';
        if (result.first==bufStr.end()) {
            sscanf(&(s[6]), "%d", &PID);
            //std::cout << PID << '\n';
            numTerminatedProcess += TerminateProcessByPID(PID);
        }
    }

    return numTerminatedProcess;
}

int TerminateProcessByEnvV() {
    char *cstr = std::getenv("PROC_TO_KILL");
    if (cstr == NULL)
        return 0;
    std::string str(cstr);
    std::regex regex{R"([,]+)"}; // split on space and comma
    std::sregex_token_iterator it{str.begin(), str.end(), regex, -1};
    std::vector<std::string> names{it, {}};
    int numTerminatedProcess = 0;
    for (auto &str: names) {
        numTerminatedProcess += TerminateProcessByName(str);
    }
    return numTerminatedProcess;

}

int KillerTerminateProcess(std::string flag) {


    int numTerminatedProcess = 0;
    if (flag == "/PID") {
        int PID;
        input >> PID;
        numTerminatedProcess += TerminateProcessByPID(PID);
    } else if (flag == "/NAME") {
        std::string NAME;
        input >> NAME;
        numTerminatedProcess += TerminateProcessByName(NAME);
    } else if (flag == "/ENVV") {
        numTerminatedProcess += TerminateProcessByEnvV();
    } else {
        std::cout << flag << " is not recognized.\n";
        return -1;
    }
    std::cout << "Number of terminated processes : " << numTerminatedProcess << "\n";
    return numTerminatedProcess;
}

void KillerRecursion() {
    std::string flag = "";
    while (flag != "/EXIT" && input.rdbuf()->in_avail() != 0) {
        input >> flag;
        if (flag == "/EXIT") {
            return;
        } else if (flag == "/HELP") {
            std::cout << "Help\n"
                      << "/PID DWORD_PID          Set terminate process by PID flag.\n"
                      << "/NAME STRING_NAME       Set terminate process by NAME flag\n"
                      << "/ENVV                   Set terminate process by Environment Variable PROC_TO_KILL\n"
                      << "/EXIT                   Exit program\n";
        } else {
            KillerTerminateProcess(flag);
        }
    }
}

std::string inputToString(int argc, char **argv) {
    std::string str = "";
    for (size_t i = 1; i < argc; ++i) {
        str += argv[i];
        str += ' ';
    }
    return str;
}

int main(int argc, char **argv) {
    input = std::istringstream(inputToString(argc, argv));
    KillerRecursion();
    std::cout << "Press entre to continue...";
    std::cin.get();
}