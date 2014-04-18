#include "categoryList.h"
#include <iostream>

CategoryList::CategoryList(string in_session, string inURL, bool excerpt_mode, bool list_mode, bool unread_mode) {
    session = in_session;
    URL = inURL;
    excerptMode = excerpt_mode;
    listMode = list_mode;
    unreadMode = unread_mode;
    initialize();
}

CategoryList::CategoryList(string in_session, string inURL) {
    session = in_session;
    URL = inURL;
    excerptMode = false;
    listMode = false;
    unreadMode = true;
    initialize();
}

CategoryList::~CategoryList() {
}

void CategoryList::initialize() {
    Json::Value cats;
    Json::StyledWriter writer;
    Json::Reader reader;
    string* data = new string();

    cats["sid"]             = session;
    cats["enabled_nested"]	= false;
    cats["op"]              = "getCategories";
    string Json = writer.write(cats);

    CURL *curlSession = curl_easy_init();
    curl_easy_setopt(curlSession, CURLOPT_POSTFIELDS, Json.c_str());
    curl_easy_setopt(curlSession, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curlSession, CURLOPT_WRITEFUNCTION, write_function3);
    curl_easy_setopt(curlSession, CURLOPT_WRITEDATA, data);
    curl_easy_perform(curlSession);

    bool parsed = reader.parse(data->c_str(), cats);
    if (!parsed)
    {
        cerr << "Failed to parse configraution\n"
             << reader.getFormatedErrorMessages();
        return;
    }
    const Json::Value status    = cats["status"];
    const Json::Value content   = cats["content"];
    if (status.asInt() == 1) {
        cerr << "Wasn't able to get data, probably needs to log in.\n"
             << "Recieved " << content["error"].asString().c_str()<<endl;
        return;
    }

    //Get all of the top level
    for (int index = 0; index < content.size(); index++) {
        int id;
        //Special case for the special categories at the end (special and uncategorized)
        if (index > (content.size() - 3)) {
            id  = (index  + 1) - content.size() ;
        }
        else {
        	id  = atoi(content[index]["id"].asString().c_str());
        }
        string name	= content[index]["title"].asString();
        int unread	= content[index]["unread"].asInt();
        
        Category* cate	= new Category(id, unread, name, session, unreadMode, true, -2, URL);
        cat_list new_item = cat_list(name, cate);
        list.insert(list.end(),new_item);
    }
    //Now getting the feeds for each category, just for the purpose of finding the subcategories. 
    //This will hopefully get the correct parent ID.
    vector<cat_list>::iterator it;
    //list should grow as the number of subcategories grows.
    for (it = list.begin(); it != list.end(); ++it) {
        Json::Value feeds = it->cat->getFeeds();
        for (int i = 0; i < feeds.size(); i++) {
            if(feeds[i]["is_cat"].asBool()) {
                int id      = feeds[i]["id"].asInt();
                string name	= feeds[i]["title"].asString();
                int unread	= feeds[i]["unread"].asInt();

                Category* new_cat = new Category(id, unread, name, session, unreadMode, false, i, URL);
                list.insert(list.end(), cat_list(name,new_cat));
            }
        }
    }
}

void CategoryList::refresh() {
    //Going to loop through the list to refresh the headlines 
    vector <cat_list>::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        it->cat->refreshHeadlines(session, unreadMode);
    }
}

Json::Value CategoryList::getHeadlines(string cat_name, int num) {
    vector <cat_list>::iterator it;
    it = list.begin();
    while (it != list.end()) {
        if (it->name == cat_name) {
            return it->cat->getHeadlines();
        }
        it++;
    }
    assert(it != list.end() );
    printf("Category name is not found\n");
}

Json::Value CategoryList::getHeadlines(int pos, int num) {
    assert(pos < list.size());
    Category *cat = list[pos].cat;
    return cat->getHeadlines();
}

const vector<Category*>* CategoryList::getTopLevelCategories() {
    vector <Category*>* catList = new vector<Category*>;
    vector <cat_list>::iterator it;

    for(it=list.begin();it!=list.end();++it) {
        if(it->cat->isTopLevel()) {
            catList->insert(catList->end(), it->cat);
        }
    }
    return catList;
}

const vector<Category*>* CategoryList::getChildCategories(string name) {
    vector <Category*>* catList;
    vector <cat_list>::iterator it;

    int id;

    for (it = list.begin();it!=list.end();++it) {
        if ((it->name == name) && (it->cat->isTopLevel())) {
            id = it->cat->getID();
            break;
        }
    }

    for (it = list.begin(); it!= list.end(); ++it) {
        if (it->cat->getParentID() == id) {
            catList->insert(catList->end(),it->cat);
        }
    }
    return catList;
}

const Category* CategoryList::getCategory(string name) {
    vector <cat_list>::iterator it;

    for (it=list.begin();it!=list.end();++it){
        if(it->name == name) {
            return it->cat;
        }
    }
}

void CategoryList::setSession(string in_session) {
    session = in_session;
}

void CategoryList::setExcerptMode(bool excerpt) {
    excerptMode = excerpt;
}

void CategoryList::setListMode(bool list) {
    listMode = list;
}

void CategoryList::setUnreadMode(bool unread) {
    unreadMode = unread;
}
