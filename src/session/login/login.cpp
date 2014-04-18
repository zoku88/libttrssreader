#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "login.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <string.h>
#include "curl_ops.h"

using namespace std;

TTRSSLogin::TTRSSLogin() {
}

TTRSSLogin::~TTRSSLogin() {
}

void TTRSSLogin::setUsername(string username) {
    _username = username;
}

void TTRSSLogin::setPasswordCMD(string pwdCMD) {
    _pwdCMD = pwdCMD;
}

void TTRSSLogin::setURL(CURL *handle, string URL){
    curl_easy_setopt(handle, CURLOPT_URL, URL.c_str());
}

int TTRSSLogin::login(CURL *handle) {
    Json::Value root;
    Json::StyledWriter writer;
    Json::Value rcvd;
    Json::Reader reader;

    root["op"]          = "login";
    root["user"]        = _username;
    root["password"]    = getPassword();
    
    string JSON = writer.write(root);
    char* rcvdData;

    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, JSON.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunction);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &rcvdData);
    curl_easy_perform(handle);

    bool parsed = reader.parse(rcvdData, rcvd);
    if (!parsed)
    {
        cerr << "Failed to parse configuration\n"
             << reader.getFormatedErrorMessages();
        exit(1);
    }
    const Json::Value status = rcvd["status"];
    const Json::Value content = rcvd["content"];
    if (status.asInt() == 1) {
        cerr << "Wasn't able to log in.\n"
             << "Recieved " << content["error"].asString().c_str()<<endl;
        exit(1);
    }
    _session = content["session_id"].asString();

    cout << "This is the session_id we got " << _session.c_str() << endl;

    return NOERR;
}

string TTRSSLogin::getPassword() {
    string programPath = "/usr/bin/bash";
    string stdout;
    char buffer[256];
    FILE *stream = popen(_pwdCMD.c_str(), "r");
    while (fgets(buffer, 256, stream) !=NULL) {
        stdout.append(buffer);
    }
    pclose(stream);

    stdout.erase(std::remove(stdout.begin(), stdout.end(), '\n'), stdout.end());
    return stdout;
}

string TTRSSLogin::getSession() {
    return _session;
}
