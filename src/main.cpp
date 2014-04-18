#include <iostream>
#include "session.h"
#include <string>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>


int main ()
{
	CURL *curlSession = curl_easy_init();
	if(!NULL)
	{
			TTRSSLogin *Login = new TTRSSLogin();

            string username, pwd, URL;
            cout << "Please enter username"<< endl;
            std::getline(cin, username);
            cout << "Please enter password command" << endl;
            std::getline(cin, pwd);
            cout << "Please enter URL" << endl;
            std::getline(cin, URL);

			Login->setUsername(username);
			Login->setPasswordCMD(pwd);
			Login->setURL(curlSession, URL + "api/");
			Login->login(curlSession);

			string session = Login->getSession();
			
			CategoryList *cat = new CategoryList(session, URL + "api/");
            const vector<Category*> *cat_list = cat->getTopLevelCategories();
            const vector<Category*>::const_iterator it = cat_list->begin();

            cat->setUnreadMode(true);
            cat->setExcerptMode(true);
            cat->refresh();

            cout << (*it)->getName() << endl;

			cout << "headlines: "<< cat->getHeadlines((*it)->getName(),100) << endl;
			curl_easy_cleanup(curlSession);
			return (0);
	}
	else{
			cout << "Something went wrong, couldn't init CURL"<<endl;
			return (1);
	}
}
