/********************************************************************************
 *																				*
 *  This file is part of Spacerace.												*
 *	Copyright (C) 2004 by Benjamin Maier, Mark Hoffmann, Alexander Berner		*
 *																				*
 *  Spacerace is free software; you can redistribute it and/or modify			*
 *  it under the terms of the GNU General Public License as published by		*
 *  the Free Software Foundation; either version 2 of the License, or			*
 *  (at your option) any later version.											*
 *																				*
 *  Spacerace is distributed in the hope that it will be useful,				*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of				*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				*
 *  GNU General Public License for more details.								*
 *																				*
 *  You should have received a copy of the GNU General Public License			*
 *  along with Spacerace; if not, write to the Free Software						*
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	*
 *																				*
 ********************************************************************************/




#include "Mesh.h"


Mesh::Mesh(const QString& name)
{

	myBoundRad=0.0f;

	std::vector<Vektor3f> node;
	std::vector<Face> tri;

	if ( name.isEmpty() )
	{
		printf("%s:  Mesh not found\n", (const char *)name);
	};

	node.clear();
	tri.clear();
	
	QFile data(name);
	QStringList line;
    QString s;
	bool goOn=true;

	Vektor3f myV;
	myV.x=0.0f;myV.y=0.0f;myV.z=0.0f;
	node.push_back(myV);

	int fileType;

	if (name.right(3).upper()=="SMF")
		fileType=0;
	if (name.right(3).upper()=="ASC")
		fileType=1;;

	if (data.open(IO_ReadOnly) )
	{
		printf( "Loading Collision Mesh: %s ... ",  (const char *)name );
		QTextStream t( &data );        // use a text stream

		switch(fileType)
		{
	
		case 0:

	        while ( goOn ) // until end of file...
			{			
			    s = t.readLine();       // line of text excluding '\n'
				line=QStringList::split(' ', s);
		
				if (line[0]=="v")
				{
					Vektor3f cV;
					cV.x=line[1].toFloat();
					cV.y=line[2].toFloat();
					cV.z=line[3].toFloat();

					if (cV.length()>myBoundRad)
					{
						myBoundRad=cV.length();
					};

					node.push_back(cV);
				};
              
				if (line[0]=="f")
				{
					Face cF;
					cF.w[0]=line[1].toUInt();
					cF.w[1]=line[2].toUInt();
					cF.w[2]=line[3].toUInt();
					tri.push_back(cF);
				};
	
				goOn=!t.eof();
				if (line[0]=="bind") 
				{
					goOn=false;
				};
			
		    }; //while

			break;

		case 1:
			while (!t.eof())
			{
				s=t.readLine();
				line=QStringList::split(' ', s);
				if (line.count()==8) 
				{
					if (line[0]=="Face") 
					{
						Face cF;
						cF.w[0]=line[2].mid(2).toUInt()+1;
						cF.w[1]=line[3].mid(2).toUInt()+1;
						cF.w[2]=line[4].mid(2).toUInt()+1;
						tri.push_back(cF);
					}

				};
				if (line.count()>=5) 
				{
					Vektor3f cV;
					cV.x=line[2].mid(2).toFloat()/100.0f;
					cV.y=line[3].mid(2).toFloat()/100.0f;
					cV.z=line[4].mid(2).toFloat()/100.0f ;
					node.push_back(cV);
				};
			}; //while
			break;

		}; // switch

        data.close();

	}

	else
	{
		printf("* !! Konnte Datei %s nicht oeffnen!\n", (const char *)name);
	};	// if


	myNode = new Vektor3f[node.size()];
	myTriSize=tri.size();
	myTri = new Face[myTriSize];

	for (int i=0; i<node.size(); i++)
	{
		myNode[i] = node[i];
	};

	for (i=0; i<myTriSize; i++)
	{
		myTri[i] = tri[i];
	};


	node.clear();
	tri.clear();

	myBoundRad=myBoundRad+0.1f;		// Sicherheitsabstand

	printf("Load Complete!\n");
}


Mesh::~Mesh()
{
	delete myNode;
	delete myTri;
}
