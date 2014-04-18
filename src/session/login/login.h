#include <curl/curl.h>
#include <string>
using namespace std;

#ifndef TTRSSLogin_H
#define TTRSSLogin_H

#define NOERR  0

class TTRSSLogin {
    public:
        TTRSSLogin();
        ~TTRSSLogin();
        void setUsername(string username);
        void setPasswordCMD(string pwdCMD);
        void setURL(CURL *handle, string URL);
        string getSession();
        int login(CURL *handle);
    private:
        string getPassword();
        string _username;
        string _pwdCMD;
        string _URL;
        string _session;
};

#endif
