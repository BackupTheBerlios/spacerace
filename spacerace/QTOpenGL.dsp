# Microsoft Developer Studio Project File - Name="QTOpenGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=QTOpenGL - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QTOpenGL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QTOpenGL.mak" CFG="QTOpenGL - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QTOpenGL - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "QTOpenGL - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GX /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "QT_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt230.lib opengl32.lib glu32.lib winmm.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "QT_DLL" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt230.lib opengl32.lib glu32.lib winmm.lib /nologo /subsystem:console /map /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "QTOpenGL - Win32 Release"
# Name "QTOpenGL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Asteroid.cpp
# End Source File
# Begin Source File

SOURCE=.\BoundingField.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Gate.cpp
# End Source File
# Begin Source File

SOURCE=.\IntroController.cpp
# End Source File
# Begin Source File

SOURCE=.\IntroPaintWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix4x4.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\minipaint.cpp
# End Source File
# Begin Source File

SOURCE=.\Missile.cpp
# End Source File
# Begin Source File

SOURCE=.\MySound.cpp
# End Source File
# Begin Source File

SOURCE=.\Nebula.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherShip.cpp
# End Source File
# Begin Source File

SOURCE=.\paint_widget_qt.cpp
# End Source File
# Begin Source File

SOURCE=.\RotMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Ship.cpp
# End Source File
# Begin Source File

SOURCE=.\SpaceObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SpaceObjectList.cpp
# End Source File
# Begin Source File

SOURCE=.\State.cpp
# End Source File
# Begin Source File

SOURCE=.\TCPClient.cpp
# End Source File
# Begin Source File

SOURCE=.\TCPClSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\TCPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Vektor3f.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Asteroid.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Asteroid.h
InputName=Asteroid

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Asteroid.h
InputName=Asteroid

"czj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BoundingField.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\BoundingField.h
InputName=BoundingField

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Controller.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Controller.h
InputName=Controller

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Controller.h
InputName=Controller

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\enum_names.h
# End Source File
# Begin Source File

SOURCE=.\Game.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Game.h
InputName=Game

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Gate.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Gate.h
InputName=Gate

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Gate.h
InputName=Gate

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IntroController.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\IntroController.h
InputName=IntroController

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IntroPaintWidget.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\IntroPaintWidget.h
InputName=IntroPaintWidget

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Matrix4x4.h
# End Source File
# Begin Source File

SOURCE=.\Menu.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Menu.h
InputName=Menu

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mesh.h
# End Source File
# Begin Source File

SOURCE=.\Missile.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Missile.h
InputName=Missile

"czl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MySound.h
# End Source File
# Begin Source File

SOURCE=.\Nebula.h
# End Source File
# Begin Source File

SOURCE=.\OtherShip.h
# End Source File
# Begin Source File

SOURCE=.\paint_widget_qt.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\paint_widget_qt.h
InputName=paint_widget_qt

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\paint_widget_qt.h
InputName=paint_widget_qt

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RotMatrix.h
# End Source File
# Begin Source File

SOURCE=.\Ship.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Ship.h
InputName=Ship

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Ship.h
InputName=Ship

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SpaceObject.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\SpaceObject.h
InputName=SpaceObject

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\SpaceObject.h
InputName=SpaceObject

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SpaceObjectList.h
# End Source File
# Begin Source File

SOURCE=.\State.h
# End Source File
# Begin Source File

SOURCE=.\TCPClient.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\TCPClient.h
InputName=TCPClient

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\TCPClient.h
InputName=TCPClient

"cvhxc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TCPClSocket.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\TCPClSocket.h
InputName=TCPClSocket

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\TCPClSocket.h
InputName=TCPClSocket

"dtrzt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TCPServer.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\TCPServer.h
InputName=TCPServer

"$(ProjDir)/$(InputName)_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\TCPServer.h
InputName=TCPServer

"zttu" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Vektor3f.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
