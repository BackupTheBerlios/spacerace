#include ".\mystringlist.h"

MyStringList::MyStringList(QString in)
{
	count=0;
	QTextStream ts( &in, IO_ReadOnly );

	QString tmp;

	while (!ts.atEnd())
	{
		ts >> tmp;
		count++;
	}


	QTextStream inStr( &in, IO_ReadOnly );
	sList=new QString[count];

	count=0;
	while (!inStr.atEnd())	
	{
		ts >> sList[count];
		count++;
	}

}

MyStringList::~MyStringList(void)
{
}

void MyStringList::split(){

}
