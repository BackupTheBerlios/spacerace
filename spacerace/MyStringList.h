#pragma once

#include <qstring.h>
#include <qtextstream.h>

class MyStringList
{
	unsigned int cnt;

public:
	MyStringList(QString);
	~MyStringList(void);

	QString * sList;


	unsigned int count() {return cnt;}

	inline QString operator[](const unsigned int& i) const 
	{
		return sList[i];
	}
};
