#include ".\mystringlist.h"

MyStringList::MyStringList(QString in)
{
	cnt=0;
	QTextStream ts( &in, IO_ReadOnly );

	QString tmp;

	while (!ts.atEnd())
	{
		ts >> tmp;
		cnt++;
	}


	QTextStream inStr( &in, IO_ReadOnly );
	sList=new QString[cnt];

	cnt=0;
	while (!inStr.atEnd())	
	{
		ts >> sList[cnt];
		cnt++;
	}

}

MyStringList::~MyStringList(void)
{
	delete[] sList;
}

