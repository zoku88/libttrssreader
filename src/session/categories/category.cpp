#include "category.h"
#include <iostream>

Category::Category(int id, int unread, string cat_name, string session_id, bool unread_only, bool toplevel, int parent, string apiURL) {
    cat_id = id;
    unread_number = unread;
    name = cat_name;
    URL = apiURL;
    is_toplevel = toplevel;
    if(!toplevel)
        parent_id = 0;
    else
        parent_id = parent;
    refreshHeadlines(session_id, unread_only);
    refreshFeeds(session_id);
}

Category::~Category() {
}

void Category::refreshHeadlines(string session_id, bool unread_only) {
    string *data = new string();
    Json::Value temp;
    Json::Value rcvd;
    Json::StyledWriter writer;
    Json::Reader reader;

    temp["sid"]                 = session_id;
    temp["op"]                  = "getHeadlines";
    temp["feed_id"]             = cat_id;
    temp["is_cat"]              = true;
    if(unread_only) {
        temp["view_mode"]       = "unread";
    }
    else {
        temp["view_mode"]       = "all_articles";
    }
    temp["show_excerpt"]        = false;
    temp["show_content"]        = true;
    string Json = writer.write(temp);

    CURL* session = curl_easy_init();
    curl_easy_setopt(session, CURLOPT_POSTFIELDS, Json.c_str());
    curl_easy_setopt(session, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_function3);
    curl_easy_setopt(session, CURLOPT_WRITEDATA, data);
    curl_easy_perform(session);

    bool parsed = reader.parse(data->c_str(), rcvd);
    if (!parsed)
    {
        cerr << "Failed to parse configraution\n"
             << reader.getFormatedErrorMessages();
        return;
    }
    const Json::Value status    = rcvd["status"];
    const Json::Value content   = rcvd["content"];

    if (status.asInt() == 1) {
        cerr << "Wasn't able to get data, probably needs to log in.\n"
             << "Recieved " << content["error"].asString().c_str()<<endl;
        return;
    }
    headlines = content;
    curl_easy_cleanup(session);
}

void Category::refreshFeeds(string session_id) {
    string* data = new string();
    Json::Value temp;
    Json::Value rcvd;
    Json::StyledWriter writer;
    Json::Reader reader;

    temp["sid"]             = session_id;
    temp["op"]              = "getFeeds";
    temp["cat_id"]          = cat_id;
    temp["unread_only"]     = false;
    string Json = writer.write(temp);

    CURL* session = curl_easy_init();
    curl_easy_setopt(session, CURLOPT_POSTFIELDS, Json.c_str());
    curl_easy_setopt(session, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, write_function3);
    curl_easy_setopt(session, CURLOPT_WRITEDATA, data);
    curl_easy_perform(session);

    bool parsed = reader.parse(*data, rcvd);
    if (!parsed)
    {
        cout << "Failed to parse configraution\n"
             << reader.getFormatedErrorMessages();
        return;
    }
    const Json::Value status    = rcvd["status"];
    const Json::Value content   = rcvd["content"];

    if (status.asInt() == 1) {
        cerr << "Wasn't able to get data, probably needs to log in.\n"
             << "Recieved " << content["error"].asString().c_str()<<endl;
        return;
    }
    feeds = content;
    curl_easy_cleanup(session);

}

int Category::getUnread() {
    return unread_number;
}

const Json::Value Category::getHeadlines() {
    return headlines;
}

const Json::Value Category::getFeeds() {
    return feeds;
}

void Category::setUnread(int unread) {
    unread_number = unread;
}

bool Category::isTopLevel() {
    return is_toplevel;
}

int Category::getID() {
    return cat_id;
}

int Category::getParentID() {
    return parent_id;
}

string Category::getName() {
    return name;
}
