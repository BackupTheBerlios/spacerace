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
 *  along with Spacerace; if not, write to the Free Software					*
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	*
 *																				*
 ********************************************************************************/




#include "IntroController.h"


IntroController::IntroController(const QString &host,const QString &portString ,QWidget * parent, const char * name, WFlags f)
:QWidget(parent,name,f)
{

	alreadyDone=false;
	tickCount = 0;


	sndMusic = new MySound("sounds/music.mp3",1);
	playingmusic=true;


	QGLFormat form;
	form.setStencil(true);
	form.setDirectRendering(true);
	QGLFormat::setDefaultFormat(form);


	Viewer = new IntroPaintWidget(this, "viewer", 0);
	connect (this,SIGNAL(sigResize(QResizeEvent*)),Viewer,SLOT(sltResize(QResizeEvent*)));


	timer = new QTimer( this );
    QObject::connect( timer, SIGNAL(timeout()), SLOT(tick()) );

	connect (this,SIGNAL(sigPaint()),Viewer,SLOT(updateGL()));
	connect (this,SIGNAL(sigUpdateViewer(SpaceObjectList*)),Viewer,SLOT(sltInput(SpaceObjectList*)));

	
//	Mesh* mesh_Other = new Mesh("Models/racercollilow.smf");


	viewList = new SpaceObjectList();

	State tmpStat;


	Schiff1 = new Ship();
	Viewer->Schiff=Schiff1;
	viewList->addItem(Schiff1);

	tmpStat.objType = o_Ship;
	tmpStat.ori = Vektor3f(1.0f, 0.0f, 0.0f);
	tmpStat.up = Vektor3f(0.0f, 1.0f, 0.0f);
	tmpStat.pos = Vektor3f(0.0f, 0.0f, 0.0f);
	Schiff1->setState(tmpStat);
	Schiff1->setSpeedSet(1000);
	Schiff1->setSpeedReal(1000);

	Schiff2 = new SpaceObject();
	viewList->addItem(Schiff2);
	tmpStat.objType = o_OtherShip;
	tmpStat.ori = Vektor3f(1.0f, 0.0f, 0.0f);
	tmpStat.up = Vektor3f(0.0f, 1.0f, 0.07f);
	tmpStat.up.normalize();
	tmpStat.pos =	Vektor3f(-1000.0f, 0.0f, 0.0f);
	Schiff2->setState(tmpStat);

	Schiff3 = new SpaceObject();
	viewList->addItem(Schiff3);
	tmpStat.objType = o_OtherShip;
	tmpStat.ori = Vektor3f(1.0f, 0.0f, 0.0f);
	tmpStat.up = Vektor3f(0.0f, 1.0f, 0.3f);
	tmpStat.up.normalize();
	tmpStat.pos =	Vektor3f(-1000.0f, 0.0f, 0.0f);
	Schiff3->setState(tmpStat);

	Schiff4 = new SpaceObject();
	viewList->addItem(Schiff4);
	tmpStat.objType = o_OtherShip;
	tmpStat.ori = Vektor3f(1.0f, 0.0f, 0.0f);
	tmpStat.up = Vektor3f(0.0f, 1.0f, 0.15f);
	tmpStat.up.normalize();
	tmpStat.pos =	Vektor3f(-1000.0f, 0.0f, 0.0f);
	Schiff4->setState(tmpStat);
	
	Flare = new SpaceObject();
	viewList->addItem(Flare);
	tmpStat.objType = 15;
	tmpStat.ori = Vektor3f(-1.0f, 0.0f, 0.0f);
	tmpStat.up = Vektor3f(0.0f, 1.0f, 0.0f);
	tmpStat.pos =	Vektor3f(-1000.0f, 0.0f, 0.0f);
	Flare->setState(tmpStat);


	OurNames = new SpaceObject();
	viewList->addItem(OurNames);
	tmpStat.objType = o_Alex;
	tmpStat.ori = Vektor3f(1.0f, 0.0f, 0.0f);
	tmpStat.up = Vektor3f(0.0f, 1.0f, 0.0f);
	tmpStat.pos =	Vektor3f(-1000.0f, 0.0f, 0.0f);
	OurNames->setState(tmpStat);


	setFocus();
	
    timer->start( TICKRATE, FALSE );
//    timer->start(20, FALSE );

	sndMusic->play();
}



IntroController::~IntroController()
{
	sndMusic->stop();
	delete  Schiff1;
	delete	viewList;
	delete sndMusic;
}



void IntroController::tick(void)
{


	tickCount++;

	Schiff1->reCalc(viewList);
	Schiff2->reCalc();
	Schiff3->reCalc();
	Schiff4->reCalc();
	OurNames->reCalc();
	Flare->reCalc();

	Vektor3f s1Pos = Schiff1->getPos();

	if ( ( (Schiff2->getPos()-s1Pos).sqrLength()<380)  ||  ((Schiff3->getPos()-s1Pos).sqrLength()<400) || ((Schiff4->getPos()-s1Pos).sqrLength()<400) )	// Abstand < 20 dann Schütteln
	{
		Viewer->afterburner = true;
	}
	else
	{
		Viewer->afterburner = false;
	};


	switch (tickCount)
	{
		case 100:	{
					Flare->setPos(Vektor3f(250.0f,-1.2f,-20.0f));
					Flare->setMovement(Vektor3f(0.0f,0.0f,1.0f),1.0f);
					resizeEvent(new QResizeEvent(QSize(width(),height()),QSize(width(),height())));
					};
					break;

		case 110:	{
					OurNames->setPos(Vektor3f(230.0f,-1.0f,-4.0f));
					OurNames->setMovement(Vektor3f(-1.0,0.0f,0.0f),0.5f);
					};
					break;

		case 120:	{
					Flare->setPos(Vektor3f(0.0f,0.0f,0.0f));
					Flare->setMovement(Vektor3f(0.0f,0.0f,0.0f),0.0f);
					};
					break;

		case 250:	{
					Flare->setPos(Vektor3f(400.0f,15.7f,-10.5f));
					Flare->setMovement(Vektor3f(0.0f,0.0f,1.0f),1.0f);
					};
					break;

		case 260:	{
					State st = OurNames->getState();
					st.objType=o_Benni;
					OurNames->setState(st);
					OurNames->setPos(Vektor3f(380.0f,3.2f,0.0f));
					OurNames->setMovement(Vektor3f(-1.0,0.0f,0.0f),0.5f);
					};
					break;

		case 270:	{
					Flare->setPos(Vektor3f(0.0f,0.0f,0.0f));
					Flare->setMovement(Vektor3f(0.0f,0.0f,0.0f),0.0f);
					};
					break;

		case 400:	{
					Flare->setPos(Vektor3f(550.0f,-1.25f,16.0f));
					Flare->setMovement(Vektor3f(0.0f,0.0f,1.0f),1.0f);
					};
					break;

		case 410:	{
					State st = OurNames->getState();
					st.objType=o_Mark;
					OurNames->setState(st);
					OurNames->setPos(Vektor3f(530.0f,-1.0f,4.0f));
					OurNames->setMovement(Vektor3f(-1.0,0.0f,0.0f),0.45f);
					};
					break;

		case 420:	{
					Flare->setPos(Vektor3f(0.0f,0.0f,0.0f));
					Flare->setMovement(Vektor3f(0.0f,0.0f,0.0f),0.0f);
					};
					break;

		case 570:	{
					Schiff1->sltKey(Qt::Key_9);
					};
					break;

		case 610:	{
					State st = OurNames->getState();
					st.objType=o_Present;
					OurNames->setState(st);
					OurNames->setPos(Vektor3f(730.0f,0.0f,0.0f));
					OurNames->setMovement(Vektor3f(0.0,0.0f,0.0f),0.0f);
					};
					break;
	
		case 710:	{
					Schiff1->sltKeyRelease(Qt::Key_9);
					};
					break;

		case 790:	{
					Schiff2->setPos(Vektor3f(750.0f,-1.0f,3.0f));
					Schiff2->setMovement(Vektor3f(1.0f,0.0f,0.0f),1.5f);
					Schiff3->setPos(Vektor3f(670.0f,-1.5f,-4.7f));
					Schiff3->setMovement(Vektor3f(1.0f,0.0f,0.0f),1.7f);
					Schiff4->setPos(Vektor3f(590.0f,2.7f,0.0f));
					Schiff4->setMovement(Vektor3f(1.0f,0.0f,0.0f),1.9f);
					};
					break;

		case 1270:	{
					Schiff1->sltKey(Qt::Key_7);
					};
					break;

		case 1275:	{
					State st = OurNames->getState();
					st.objType=o_Title;
					OurNames->setState(st);
					OurNames->setPos(Vektor3f(1770.0f,0.0f,0.0f));
					OurNames->setMovement(Vektor3f(-1.0,0.0f,0.0f),1.605f);
					};
					break;



		case 1350:	{
					State st = Schiff2->getState();
					st.ori=Vektor3f(-1.0f,0.0f,0.0f);
					Schiff2->setState(st);
					Schiff2->setMovement(Vektor3f(-1.0f,0.0f,0.0f),1.5f);

					st = Schiff3->getState();
					st.ori=Vektor3f(-1.0f,0.0f,0.0f);
					Schiff3->setState(st);
					Schiff3->setMovement(Vektor3f(-1.0f,0.0f,0.0f),1.7f);

					st = Schiff4->getState();
					st.ori=Vektor3f(-1.0f,0.0f,0.0f);
					Schiff4->setState(st);
					Schiff4->setMovement(Vektor3f(-1.0f,0.0f,0.0f),1.9f);
					};
					break;

		case 1410:	{
					Schiff1->sltKeyRelease(Qt::Key_7);
					};
					break;

		case 1450:	{
					Schiff1->setSpeedSet(0);
					OurNames->setMovement(Vektor3f(0.0,0.0f,0.0f),0.0f);
					};
					break;

		case 1550:	{
					Schiff1->setSpeedSet(1000);
					OurNames->setMovement(Vektor3f(1.0,0.0f,0.0f),0.7f);
					};
					break;

		case 2000:	{
					timer->stop();
					emit quit();
					return;
					};
					break;

		default:	{};
					break;

	};	// switch tickCount
	

	emit sigUpdateViewer(viewList);
	emit sigPaint();

}



void IntroController::resizeEvent(QResizeEvent* event)
{
	emit sigResize(event);
};


void IntroController::sltResize(QResizeEvent* event)
{
	resize(event->size());
}


void IntroController::keyPressEvent( QKeyEvent *e )
{

		if (e->key() == Key_Escape)
		{
			timer->stop();
			emit quit();
		};
		
	
	// P = Pause
	if (e->key() == Key_P)
	{
		timer->stop();
		QDialog* mb = new QDialog(this,"pausedialog",TRUE,WStyle_Customize | WStyle_DialogBorder | WStyle_Dialog);
		mb->setCaption("[Qt]: Pause");
		QPushButton* pb =  new QPushButton("Pause.  Zum weiterspielen [Esc] oder hier drücken!",mb,"pb");
		pb->setFixedSize(400,30);
		pb->setFont(QFont( "arial",10));
		QLayout* layout = new QVBoxLayout(mb);
		layout->add(pb);
		layout->activate();
		layout->freeze();
		connect(pb, SIGNAL(released()), mb, SLOT(close()));
		mb->exec();
		delete mb;
//	    timer->start( TICKRATE-10, FALSE );
	    timer->start( 20, FALSE );
	};
	

}


#include"IntroController_moc.cpp"
