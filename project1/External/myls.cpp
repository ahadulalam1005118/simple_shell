#include <stdio.h>
#include <iostream>
#include <sys/stat.h>
#include <zconf.h>
#include<vector>
#include<iostream>
#include <algorithm>
#include <sys/stat.h>
#include <sstream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <pwd.h>
#include <grp.h>
using namespace std;

vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}
void createList(vector<string> &ls_dir) {
    DIR *current_dir = NULL;
    struct dirent *Dir;
    current_dir = opendir(get_current_dir_name());

    while((Dir = readdir(current_dir)) != NULL) {
        char *base = Dir->d_name;
        ls_dir.push_back(base);
    }
    sort(ls_dir.begin(),ls_dir.end());
    closedir(current_dir);
}
void createListargv(vector<string> &ls_dir, std::string argv) {
    DIR *current_dir = NULL;
    struct dirent *Dir;
    current_dir = opendir(argv.c_str());

    while((Dir = readdir(current_dir)) != NULL) {
        char *base = Dir->d_name;
        ls_dir.push_back(base);
    }
    sort(ls_dir.begin(),ls_dir.end());
    closedir(current_dir);
}

void drwx(mode_t i, char *buf) {
    *buf++ = (i & S_IFDIR) ? 'd' : '-';
    *buf++ = (i & S_IRUSR) ? 'r' : '-';
    *buf++ = (i & S_IWUSR) ? 'w' : '-';
    *buf++ = (i & S_IXUSR) ? 'x' : '-';
    *buf++ = (i & S_IRGRP) ? 'r' : '-';
    *buf++ = (i & S_IWGRP) ? 'w' : '-';
    *buf++ = (i & S_IXGRP) ? 'x' : '-';
    *buf++ = (i & S_IROTH) ? 'r' : '-';
    *buf++ = (i & S_IWOTH) ? 'w' : '-';
    *buf++ = (i & S_IXOTH) ? 'x' : '-';
    *buf = '\0';
}

vector<vector<string> > createTable(vector<string> &lsdir) {
    vector<vector<string> > table_dir;
    struct stat objFile;
    vector<string> header;
    char uname[20],gname[20],size[100];
    header.push_back("Mode     ");
    header.push_back("User ");
    header.push_back("Group ");
    header.push_back("Size ");
    header.push_back("Modify time ");
    header.push_back("Name ");

    table_dir.push_back(header);
    struct group *grp;
    struct passwd *pwd;

    for(unsigned int i=0; i<lsdir.size();i++) {
        vector<string> line;
        stat(lsdir[i].c_str(), &objFile);

        struct tm * timeinfo;
        timeinfo = localtime(&objFile.st_ctime);
        char time[25];
        strftime(time,sizeof(time),"%Y-%m-%d %H:%M",timeinfo);
        char mode[BUFSIZ];
        drwx(objFile.st_mode,mode);
        //uid = objFile.st_uid;
        //gid = objFile.st_gid;
        grp = getgrgid(objFile.st_gid);
        pwd = getpwuid(objFile.st_uid);

        sprintf(uname, "%s", pwd->pw_name);
        sprintf(gname, "%s", grp->gr_name);
        sprintf(size, "%d", (int)objFile.st_size);

        line.push_back(mode);
        line.push_back(uname);
        line.push_back(gname);
        line.push_back(size);
        line.push_back(time);
        line.push_back(lsdir[i].c_str());

        table_dir.push_back(line);

    }
    return table_dir;

}

void print(vector<vector<string> > &dir_struct) {
    for(unsigned int i=0; i< dir_struct.size(); i++) {
        for(unsigned int j=0; j<dir_struct[0].size(); j++) {
            cout << dir_struct[i][j].c_str()<< " ";
        }
        cout << endl;
    }

}
int main(int argc, char *argv[]) {
    vector<string> lsdir;
    vector<vector<string> > dir_s;
        if(argc == 1) createList(lsdir);
        else createListargv(lsdir,argv[1]);
        dir_s = createTable(lsdir);
        print(dir_s);
}
