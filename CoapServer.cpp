#pragma once

#include "CoapServer.h"
#include "stdlib.h"

CoapNoun::CoapNoun(char* name, CoapHandler get, CoapHandler post, CoapHandler put, CoapHandler del)
{
	Get = get;
	Post = post;
    Put = put;
	Delete = del;
	Name = name;
}

CoapServer::CoapServer(CoapNoun nouns[], unsigned int count)
{
	m_count = count;
	m_nouns = nouns;
}

int CoapServer::FindNoun(char* name)
{
	for (int i = 0; i < m_count; i++)
	{
		if (strcmp(m_nouns[i], name) == 0)
			return i;
	}

	return -1;
}

int CoapServer::CallNoun(char* name, CoapPDU& request, CoapPDU& response)
{
	int index = FindNoun(name);

	// HTTP Not Found
	if (index < 0)
		return CoapPDU::COAP_NOT_FOUND;

	//Invalid method code
	int code = request.getCode();
	if (code < 1 || code > 4)
		return CoapPDU::COAP_BAD_REQUEST;

	// Not found (again)
	CoapNoun* noun = &m_nouns[index];
	if (noun == NULL)
		return CoapPDU::COAP_NOT_FOUND;

	CoapHandler handlerFunc = ((CoapHandler*)noun)[(int)(code - 1)];

	// Http Method Not Allowed
	if (handlerFunc == NULL)
		return CoapPDU::COAP_METHOD_NOT_ALLOWED;

	return handlerFunc(*this, request, response);
}

void CoapServer::HandleRequest(CoapPDU& request, CoapPDU& response)
{
	/* CoapPDU *pdu = new CoapPDU();
	pdu->setType(CoapPDU::COAP_CONFIRMABLE);
	pdu->setCode(CoapPDU::COAP_GET);
	pdu->setToken((uint8_t*)F("\3\2\1\0"),4);
	pdu->setMessageID(0x0005);
	pdu->setURI((char*)F("test"),4);
	pdu->addOption(11,5,(uint8_t*)F("hello"));
	pdu->addOption(11,5,(uint8_t*)F("there"));
	pdu->addOption(11,6,(uint8_t*)F("server"));
	delete pdu;

	char buffer[100];
	CoapPDU* recvPDU = new CoapPDU((uint8_t*)buffer, 100, 100);
	if(recvPDU->validate()) {
		int recvURILen;
		recvPDU->getURI(buffer,100,&recvURILen);
	}
	delete recvPDU; */
}