#pragma once

#include <cantcoap.h>

//Forward declarations
struct CoapServer;
struct CoapNoun;

typedef int(*CoapHandler)(CoapServer&, CoapPDU&, CoapPDU&);

typedef struct CoapNoun
{
	// It's critical that the first 4 members of this struct are CoapHandler field AND are in this specific order!
	// server grabs the correct handler by indexing into this struct as if it is a CoapHandler array!
public:
	CoapHandler Get;
	CoapHandler Post;
	CoapHandler Put;
	CoapHandler Delete;

	char* Name;

public:
	CoapNoun(char* name, CoapHandler get, CoapHandler post, CoapHandler put, CoapHandler del);
};

typedef struct CoapServer
{
public:
	CoapServer(CoapNoun nouns[], unsigned int count);

private:
	unsigned int m_count;
	CoapNoun* m_nouns;

	int CallNoun(char* name, CoapPDU& request, CoapPDU& response);
	int FindNoun(char* name);

public:
	void HandleRequest(CoapPDU& request, CoapPDU& response);
};