#include ".\mystringlist.h"

MyStringList::MyStringList(QString in)
{
	count=0;
	QTextStream ts( &in, IO_ReadOnly );

	QString tmp;

	while (!ts.atEnd())	// alle Zeilen durchgehen
	{
		ts >> tmp;
		count++;
	}

	
	QTextStream inStr( &in, IO_ReadOnly );

}

MyStringList::~MyStringList(void)
{
}

void MyStringList::split(){

}
