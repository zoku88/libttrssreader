//This is a file for misc curl opts
//
#include <cstdlib>
#include <cassert>
#include <string.h>
#include <iostream>

#ifndef CURL_OP
#define CURL_OP
using namespace std;

struct thread_status {
		void *data;
		size_t bufferlen;
		size_t writepos;
		int initialized;
};

size_t static writefunction(void *ptr, size_t size, size_t nmemb, void *userdata) {
		char **temp = (char **) userdata;
		*temp = strndup((const char*)ptr, (size_t)size*nmemb);
		return nmemb*size;
}



size_t static write_function3(void *buffer, size_t size, size_t nmemb, void *userp) //no ordering of responses against requests maintained yet.
{
	  string chunk((char*)buffer,nmemb*size/sizeof(char));
	  ((string *) userp)->append(chunk);
	  return nmemb*size;
}
#endif
