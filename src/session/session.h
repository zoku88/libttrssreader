#include "login.h"
#include "categoryList.h"
#include <stdint.h>

#ifndef TTRSSSession_H
#define TTRSSSession_H

class TTRSSSession {
    private:
        TTRSSLogin* login;
        CategoryList* catList;
        CURL *curlSession;
        string URL;
        string session;
        bool excerptMode;
        bool listMode;
        bool unreadMode;
        bool threadStop;
        uint64_t seconds;
    public:
        TTRSSSession(string username, string URL, string pwdCMD, uint64_t time);
        TTRSSSession(string username, string URL, string pwdCMD, uint64_t time, bool excertMode, 
                    bool listMode, bool unreadMode);

        ~TTRSSSession();

        void setExcerptMode(bool mode);
        void setListMode(bool mode);
        void setUnreadMode(bool mode);
        void setUpdateTime(uint64_t time);
        const CategoryList* getCategoryList();
        const Json::Value getHeadlines(string category);
        const Json::Value getHeadlines(string category, int num);
        void refresh();
};

#endif
