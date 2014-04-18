#include <string>
#include <vector>
#include <utility>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include "category.h"

#ifndef CATEGORYLIST_H
#define CATEGORYLIST_H

using namespace std;

struct cat_list {
	string name;
	Category* cat;

	cat_list(string name, Category* cat):
    	name(name),
    	cat(cat){}
};

class CategoryList {
    private:
        vector<cat_list> list; 
        string session;
        void initialize();
        //True == Articles have excerpts.  False == Articles show "full" content.  Default is false
        bool excerptMode;
        bool listMode;
        bool unreadMode;
        string URL;
    public:
        CategoryList(string in_sesson, string inURL, bool except_mode, bool list_mode, bool unread_mode);
        CategoryList(string in_ession, string inURL);
        ~CategoryList();
        void refresh();
        Json::Value getHeadlines(string cat_name, int num=0);
        Json::Value getHeadlines(int pos, int num=0);
        const vector<Category*>* getTopLevelCategories();
        const vector<Category*>* getChildCategories(string name);
        const Category* getCategory(string name);
        void setSession(string in_session);
        void setExcerptMode(bool excerpt);
        void setListMode(bool list);
        void setUnreadMode(bool unread);
};

#endif
