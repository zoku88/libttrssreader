#include <iostream>
#include "session.h"
#include <string>
#include <stdint.h>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <unistd.h>

TTRSSSession::TTRSSSession(string username, string URL, string pwdCMD, uint64_t time) {
    TTRSSSession(username, URL, pwdCMD, time, false, false, false);
}

TTRSSSession::TTRSSSession(string username, string URL, string pwdCMD, uint64_t time, 
                           bool excerptMode, bool listMode, bool unreadMode) {

    curlSession = curl_easy_init();

    login = new TTRSSLogin();
    login->setUsername(username);
    login->setPasswordCMD(pwdCMD);
    login->setURL(curlSession, URL);
    login->login(curlSession);

    session = login->getSession();

    this->excerptMode = excerptMode;
    this->listMode    = listMode;
    this->unreadMode  = unreadMode;

    catList = new CategoryList(session, URL);
    catList->setExcerptMode(excerptMode);
    catList->setListMode(listMode);
    catList->setUnreadMode(unreadMode);

    login->login(curlSession);

    threadStop  = false;
}

TTRSSSession::~TTRSSSession() {
    delete login;
    delete catList;
    curl_easy_cleanup(curlSession);
}

void TTRSSSession::setExcerptMode(bool mode) {
    this->excerptMode = mode;
}

void TTRSSSession::setListMode(bool mode) {
    this->listMode = mode;
}

void TTRSSSession::setUnreadMode(bool mode) {
    this->unreadMode = mode;
}

void TTRSSSession::setUpdateTime(uint64_t time) {
    this->seconds = time;
}

const CategoryList* TTRSSSession::getCategoryList() {
    return catList;
}

const Json::Value TTRSSSession::getHeadlines(string category) {
    return getHeadlines(category, 0);
}

const Json::Value TTRSSSession::getHeadlines(string category, int num) {
    return getHeadlines(category, num);
}

void TTRSSSession::refresh() {
    while(!threadStop) {
        catList->refresh();
        sleep(seconds);
    }
}
