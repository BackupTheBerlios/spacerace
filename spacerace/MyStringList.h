#pragma once

#include <qstring.h>
#include <qtextstream.h>

class MyStringList
{


public:
	MyStringList(QString);
	~MyStringList(void);

	QString * sList;

	void split();
	unsigned int count;

	inline QString operator[](const unsigned int& i) const 
	{
		return sList[i];
	}
};
