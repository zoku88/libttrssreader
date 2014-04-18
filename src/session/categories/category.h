#include <curl.h>
#include <string.h>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "curl_ops.h"

using namespace std;

#ifndef CATEGORY_H
#define CATEGORY_H

class Category {
    private:
        int cat_id;
        int unread_number;
        string name;
        Json::Value headlines;
        Json::Value feeds;
        bool is_toplevel;
        int parent_id;
        string URL; 
    public:
        Category(int id, int unread, string cat_name, string session_id, 
                bool unread_only, bool toplevel, int parent, string apiURL);
        ~Category();
        void refreshHeadlines(string session_id, bool unread_only);
        void refreshFeeds(string session_id);
        void setUnread(int unread);
        int  getUnread();
        const Json::Value getHeadlines();
        const Json::Value getFeeds();
        bool isTopLevel();
        int getID();
        int getParentID();
        string getName();
};

#endif
