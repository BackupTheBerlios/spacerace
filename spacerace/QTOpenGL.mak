# Microsoft Developer Studio Generated NMAKE File, Based on QTOpenGL.dsp
!IF "$(CFG)" == ""
CFG=QTOpenGL - Win32 Release
!MESSAGE Keine Konfiguration angegeben. QTOpenGL - Win32 Release wird als Standard verwendet.
!ENDIF 

!IF "$(CFG)" != "QTOpenGL - Win32 Release" && "$(CFG)" != "QTOpenGL - Win32 Debug"
!MESSAGE UngÅltige Konfiguration "$(CFG)" angegeben.
!MESSAGE Sie kînnen beim AusfÅhren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "QTOpenGL.mak" CFG="QTOpenGL - Win32 Release"
!MESSAGE 
!MESSAGE FÅr die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "QTOpenGL - Win32 Release" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE "QTOpenGL - Win32 Debug" (basierend auf  "Win32 (x86) Console Application")
!MESSAGE 
!ERROR Eine ungÅltige Konfiguration wurde angegeben.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "./IntroPaintWidget_moc.cpp" "$(OUTDIR)\QTOpenGL.exe"


CLEAN :
	-@erase "$(INTDIR)\Asteroid.obj"
	-@erase "$(INTDIR)\BoundingField.obj"
	-@erase "$(INTDIR)\Controller.obj"
	-@erase "$(INTDIR)\Gate.obj"
	-@erase "$(INTDIR)\IntroController.obj"
	-@erase "$(INTDIR)\IntroPaintWidget.obj"
	-@erase "$(INTDIR)\Matrix4x4.obj"
	-@erase "$(INTDIR)\Menu.obj"
	-@erase "$(INTDIR)\Mesh.obj"
	-@erase "$(INTDIR)\minipaint.obj"
	-@erase "$(INTDIR)\Missile.obj"
	-@erase "$(INTDIR)\MySound.obj"
	-@erase "$(INTDIR)\Nebula.obj"
	-@erase "$(INTDIR)\OtherShip.obj"
	-@erase "$(INTDIR)\paint_widget_qt.obj"
	-@erase "$(INTDIR)\RotMatrix.obj"
	-@erase "$(INTDIR)\Ship.obj"
	-@erase "$(INTDIR)\SpaceObject.obj"
	-@erase "$(INTDIR)\SpaceObjectList.obj"
	-@erase "$(INTDIR)\State.obj"
	-@erase "$(INTDIR)\TCPClient.obj"
	-@erase "$(INTDIR)\TCPClSocket.obj"
	-@erase "$(INTDIR)\TCPServer.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Vektor3f.obj"
	-@erase "$(OUTDIR)\QTOpenGL.exe"
	-@erase "./IntroPaintWidget_moc.cpp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /ML /W3 /GX /Ob2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "QT_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\QTOpenGL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt230.lib opengl32.lib glu32.lib winmm.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\QTOpenGL.pdb" /machine:I386 /out:"$(OUTDIR)\QTOpenGL.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Asteroid.obj" \
	"$(INTDIR)\BoundingField.obj" \
	"$(INTDIR)\Controller.obj" \
	"$(INTDIR)\Gate.obj" \
	"$(INTDIR)\IntroController.obj" \
	"$(INTDIR)\Matrix4x4.obj" \
	"$(INTDIR)\Menu.obj" \
	"$(INTDIR)\Mesh.obj" \
	"$(INTDIR)\minipaint.obj" \
	"$(INTDIR)\Missile.obj" \
	"$(INTDIR)\MySound.obj" \
	"$(INTDIR)\Nebula.obj" \
	"$(INTDIR)\OtherShip.obj" \
	"$(INTDIR)\paint_widget_qt.obj" \
	"$(INTDIR)\RotMatrix.obj" \
	"$(INTDIR)\Ship.obj" \
	"$(INTDIR)\SpaceObject.obj" \
	"$(INTDIR)\SpaceObjectList.obj" \
	"$(INTDIR)\State.obj" \
	"$(INTDIR)\TCPClient.obj" \
	"$(INTDIR)\TCPClSocket.obj" \
	"$(INTDIR)\TCPServer.obj" \
	"$(INTDIR)\Vektor3f.obj" \
	"$(INTDIR)\IntroPaintWidget.obj"

"$(OUTDIR)\QTOpenGL.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : ".\zttu" ".\dtrzt" ".\czl" ".\czj" ".\cvhxc" "./SpaceObject_moc.cpp" "./Ship_moc.cpp" "./paint_widget_qt_moc.cpp" "./Gate_moc.cpp" "./Controller_moc.cpp" "$(OUTDIR)\QTOpenGL.exe"


CLEAN :
	-@erase "$(INTDIR)\Asteroid.obj"
	-@erase "$(INTDIR)\BoundingField.obj"
	-@erase "$(INTDIR)\Controller.obj"
	-@erase "$(INTDIR)\Gate.obj"
	-@erase "$(INTDIR)\IntroController.obj"
	-@erase "$(INTDIR)\IntroPaintWidget.obj"
	-@erase "$(INTDIR)\Matrix4x4.obj"
	-@erase "$(INTDIR)\Menu.obj"
	-@erase "$(INTDIR)\Mesh.obj"
	-@erase "$(INTDIR)\minipaint.obj"
	-@erase "$(INTDIR)\Missile.obj"
	-@erase "$(INTDIR)\MySound.obj"
	-@erase "$(INTDIR)\Nebula.obj"
	-@erase "$(INTDIR)\OtherShip.obj"
	-@erase "$(INTDIR)\paint_widget_qt.obj"
	-@erase "$(INTDIR)\RotMatrix.obj"
	-@erase "$(INTDIR)\Ship.obj"
	-@erase "$(INTDIR)\SpaceObject.obj"
	-@erase "$(INTDIR)\SpaceObjectList.obj"
	-@erase "$(INTDIR)\State.obj"
	-@erase "$(INTDIR)\TCPClient.obj"
	-@erase "$(INTDIR)\TCPClSocket.obj"
	-@erase "$(INTDIR)\TCPServer.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Vektor3f.obj"
	-@erase "$(OUTDIR)\QTOpenGL.exe"
	-@erase "$(OUTDIR)\QTOpenGL.ilk"
	-@erase "$(OUTDIR)\QTOpenGL.map"
	-@erase "$(OUTDIR)\QTOpenGL.pdb"
	-@erase "./Controller_moc.cpp"
	-@erase "./Gate_moc.cpp"
	-@erase "./paint_widget_qt_moc.cpp"
	-@erase "./Ship_moc.cpp"
	-@erase "./SpaceObject_moc.cpp"
	-@erase "cvhxc"
	-@erase "czj"
	-@erase "czl"
	-@erase "dtrzt"
	-@erase "zttu"

CPP_PROJ=/nologo /G5 /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "QT_DLL" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\QTOpenGL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib qt230.lib opengl32.lib glu32.lib winmm.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\QTOpenGL.pdb" /map:"$(INTDIR)\QTOpenGL.map" /debug /machine:I386 /out:"$(OUTDIR)\QTOpenGL.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Asteroid.obj" \
	"$(INTDIR)\BoundingField.obj" \
	"$(INTDIR)\Controller.obj" \
	"$(INTDIR)\Gate.obj" \
	"$(INTDIR)\IntroController.obj" \
	"$(INTDIR)\Matrix4x4.obj" \
	"$(INTDIR)\Menu.obj" \
	"$(INTDIR)\Mesh.obj" \
	"$(INTDIR)\minipaint.obj" \
	"$(INTDIR)\Missile.obj" \
	"$(INTDIR)\MySound.obj" \
	"$(INTDIR)\Nebula.obj" \
	"$(INTDIR)\OtherShip.obj" \
	"$(INTDIR)\paint_widget_qt.obj" \
	"$(INTDIR)\RotMatrix.obj" \
	"$(INTDIR)\Ship.obj" \
	"$(INTDIR)\SpaceObject.obj" \
	"$(INTDIR)\SpaceObjectList.obj" \
	"$(INTDIR)\State.obj" \
	"$(INTDIR)\TCPClient.obj" \
	"$(INTDIR)\TCPClSocket.obj" \
	"$(INTDIR)\TCPServer.obj" \
	"$(INTDIR)\Vektor3f.obj" \
	"$(INTDIR)\IntroPaintWidget.obj"

"$(OUTDIR)\QTOpenGL.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("QTOpenGL.dep")
!INCLUDE "QTOpenGL.dep"
!ELSE 
!MESSAGE Warning: cannot find "QTOpenGL.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "QTOpenGL - Win32 Release" || "$(CFG)" == "QTOpenGL - Win32 Debug"
SOURCE=.\Asteroid.cpp

"$(INTDIR)\Asteroid.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h" ".\Asteroid_moc.cpp" ".\Asteroid.h"


SOURCE=.\BoundingField.cpp

"$(INTDIR)\BoundingField.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h" ".\BoundingField.h" ".\BoundingField_moc.cpp"


SOURCE=.\Controller.cpp

"$(INTDIR)\Controller.obj" : $(SOURCE) "$(INTDIR)" ".\TCPClSocket.h" ".\paint_widget_qt.h" ".\Controller.h" ".\TCPServer.h" ".\BoundingField.h" ".\TCPClient.h" ".\Asteroid.h" ".\Controller_moc.cpp" ".\SpaceObject.h" ".\Gate.h" ".\Ship.h"


SOURCE=.\Gate.cpp

"$(INTDIR)\Gate.obj" : $(SOURCE) "$(INTDIR)" ".\Gate_moc.cpp" ".\Gate.h" ".\SpaceObject.h"


SOURCE=.\IntroController.cpp

"$(INTDIR)\IntroController.obj" : $(SOURCE) "$(INTDIR)" ".\TCPClSocket.h" ".\IntroController.h" ".\paint_widget_qt.h" ".\IntroController_moc.cpp" ".\TCPServer.h" ".\BoundingField.h" ".\TCPClient.h" ".\Asteroid.h" ".\SpaceObject.h" ".\Gate.h" ".\Ship.h"


SOURCE=.\IntroPaintWidget.cpp

"$(INTDIR)\IntroPaintWidget.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Matrix4x4.cpp

"$(INTDIR)\Matrix4x4.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Menu.cpp

"$(INTDIR)\Menu.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Mesh.cpp

"$(INTDIR)\Mesh.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\minipaint.cpp

"$(INTDIR)\minipaint.obj" : $(SOURCE) "$(INTDIR)" ".\TCPClSocket.h" ".\IntroController.h" ".\paint_widget_qt.h" ".\Controller.h" ".\TCPServer.h" ".\BoundingField.h" ".\TCPClient.h" ".\Asteroid.h" ".\SpaceObject.h" ".\Gate.h" ".\Ship.h"


SOURCE=.\Missile.cpp

"$(INTDIR)\Missile.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h"


SOURCE=.\MySound.cpp

"$(INTDIR)\MySound.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Nebula.cpp

"$(INTDIR)\Nebula.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h"


SOURCE=.\OtherShip.cpp

"$(INTDIR)\OtherShip.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h"


SOURCE=.\paint_widget_qt.cpp

"$(INTDIR)\paint_widget_qt.obj" : $(SOURCE) "$(INTDIR)" ".\BoundingField.h" ".\Gate.h" ".\SpaceObject.h" ".\paint_widget_qt_moc.cpp" ".\Ship.h" ".\paint_widget_qt.h"


SOURCE=.\RotMatrix.cpp

"$(INTDIR)\RotMatrix.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Ship.cpp

"$(INTDIR)\Ship.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h" ".\Ship.h" ".\Ship_moc.cpp"


SOURCE=.\SpaceObject.cpp

"$(INTDIR)\SpaceObject.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h" ".\SpaceObject_moc.cpp"


SOURCE=.\SpaceObjectList.cpp

"$(INTDIR)\SpaceObjectList.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h" ".\BoundingField.h"


SOURCE=.\State.cpp

"$(INTDIR)\State.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TCPClient.cpp

"$(INTDIR)\TCPClient.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h" ".\Ship.h" ".\TCPClient_moc.cpp" ".\TCPClient.h"


SOURCE=.\TCPClSocket.cpp

"$(INTDIR)\TCPClSocket.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h" ".\TCPClSocket.h" ".\Ship.h" ".\TCPClSocket_moc.cpp"


SOURCE=.\TCPServer.cpp

"$(INTDIR)\TCPServer.obj" : $(SOURCE) "$(INTDIR)" ".\SpaceObject.h" ".\TCPClSocket.h" ".\TCPServer.h" ".\Ship.h" ".\TCPServer_moc.cpp"


SOURCE=.\Vektor3f.cpp

"$(INTDIR)\Vektor3f.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Asteroid.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\Asteroid.h
InputName=Asteroid

".\Asteroid_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\Asteroid.h
InputName=Asteroid

"$(INTDIR)\czj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\BoundingField.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\BoundingField.h
InputName=BoundingField

".\BoundingField_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

!ENDIF 

SOURCE=.\Controller.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\Controller.h
InputName=Controller

".\Controller_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\Controller.h
InputName=Controller

"$(INTDIR)\Controller_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\Gate.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\Gate.h
InputName=Gate

".\Gate_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\Gate.h
InputName=Gate

"$(INTDIR)\Gate_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\IntroController.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\IntroController.h
InputName=IntroController

".\IntroController_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

!ENDIF 

SOURCE=.\IntroPaintWidget.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\IntroPaintWidget.h
InputName=IntroPaintWidget

".\IntroPaintWidget_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

!ENDIF 

SOURCE=.\Missile.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\Missile.h
InputName=Missile

"$(INTDIR)\czl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\paint_widget_qt.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\paint_widget_qt.h
InputName=paint_widget_qt

".\paint_widget_qt_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\paint_widget_qt.h
InputName=paint_widget_qt

"$(INTDIR)\paint_widget_qt_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\Ship.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\Ship.h
InputName=Ship

".\Ship_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\Ship.h
InputName=Ship

"$(INTDIR)\Ship_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\SpaceObject.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\SpaceObject.h
InputName=SpaceObject

".\SpaceObject_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\SpaceObject.h
InputName=SpaceObject

"$(INTDIR)\SpaceObject_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\TCPClient.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\TCPClient.h
InputName=TCPClient

".\TCPClient_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\TCPClient.h
InputName=TCPClient

"$(INTDIR)\cvhxc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\TCPClSocket.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\TCPClSocket.h
InputName=TCPClSocket

".\TCPClSocket_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\TCPClSocket.h
InputName=TCPClSocket

"$(INTDIR)\dtrzt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 

SOURCE=.\TCPServer.h

!IF  "$(CFG)" == "QTOpenGL - Win32 Release"

ProjDir=.
InputPath=.\TCPServer.h
InputName=TCPServer

".\TCPServer_moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ELSEIF  "$(CFG)" == "QTOpenGL - Win32 Debug"

ProjDir=.
InputPath=.\TCPServer.h
InputName=TCPServer

"$(INTDIR)\zttu" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	moc $(ProjDir)/$(InputName).h -o$(ProjDir)/$(InputName)_moc.cpp
<< 
	

!ENDIF 


!ENDIF 

