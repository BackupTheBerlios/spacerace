#pragma once

#include <qstring.h>
#include <qtextstream.h>

class MyStringList
{


public:
	MyStringList(QString);
	~MyStringList(void);

	QString * s;

	void split();
	unsigned int count;
};
