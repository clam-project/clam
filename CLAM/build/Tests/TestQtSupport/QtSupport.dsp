# Microsoft Developer Studio Project File - Name="QtSupport" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=QtSupport - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QtSupport.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QtSupport.mak" CFG="QtSupport - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QtSupport - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "QtSupport - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QtSupport - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../ReleaseFiles/bin"
# PROP Intermediate_Dir "../ReleaseFiles/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\externals\CbLib" /I "..\..\..\..\xercesc\include" /I "..\..\..\..\pthreads\include" /I "C:\Qt\3.0.0\include" /I "..\..\..\examples\NetworkEditor\GUI\Base" /I "..\..\..\examples\NetworkEditor\GUI\Interface" /I "..\..\..\examples\NetworkEditor\GUI\Qt_Presentations" /I "..\..\..\examples\NetworkEditor\Visualization" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Defines" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Networks" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Visualization\Base" /I ".\uic"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" /libpath:"C:\Qt\3.0.0\lib" RFFTW2st.lib FFTW2st.lib xerces-c_2.lib pthreadVC.lib libsndfile.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib id3lib.lib libmad.lib C:\Qt\3.0.0/lib/qt-mt300.lib.lib C:\Qt\3.0.0/lib/qtmain.lib.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRTD /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "QtSupport - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../DebugFiles/bin"
# PROP Intermediate_Dir "../DebugFiles/obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\externals\CbLib" /I "..\..\..\..\xercesc\include" /I "..\..\..\..\pthreads\include" /I "C:\Qt\3.0.0\include" /I "..\..\..\examples\NetworkEditor\GUI\Base" /I "..\..\..\examples\NetworkEditor\GUI\Interface" /I "..\..\..\examples\NetworkEditor\GUI\Qt_Presentations" /I "..\..\..\examples\NetworkEditor\Visualization" /I "..\..\..\src\Base" /I "..\..\..\src\Data\Base" /I "..\..\..\src\Data\BasicProcessing" /I "..\..\..\src\Defines" /I "..\..\..\src\Defines\Windows" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Networks" /I "..\..\..\src\Flow\Nodes" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\System\Threads" /I "..\..\..\src\Visualization\Base" /I ".\uic"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\fftw\lib" /libpath:"..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" /libpath:"C:\Qt\3.0.0\lib" FFTW2std.lib RFFTW2std.lib xerces-c_2d.lib pthreadVC.lib libsndfiled.lib ogg_static_d.lib vorbis_static_d.lib vorbisenc_static_d.lib vorbisfile_static_d.lib id3libd.lib libmadd.lib C:\Qt\3.0.0/lib/qt-mt300.lib.lib C:\Qt\3.0.0/lib/qtmain.lib.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRT /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "QtSupport - Win32 Release"
# Name "QtSupport - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group ". Sources"

# Begin Source File

SOURCE=.\main.cxx
# End Source File
# Begin Group "moc Sources"

# Begin Source File

SOURCE=.\moc\MainWindow_moc.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\Qt_ProcessingPresentation_moc.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\ProcessingTree_moc.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\Qt_ProcessingConfigPresentation_moc.cxx
# End Source File
# Begin Source File

SOURCE=.\moc\ControlPanel_moc.cxx
# End Source File
# End Group
# Begin Group "uic Sources"

# Begin Source File

SOURCE=.\uic\ControlPanel.cxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Sources"

# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Presentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelController.cxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "examples Sources"

# Begin Group "NetworkEditor Sources"

# Begin Group "GUI Sources"

# Begin Group "Base Sources No. 1"

# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Base\NetworkPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Base\ProcessingPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Base\ConnectionPresentation.cxx
# End Source File
# End Group
# Begin Group "Qt_Presentations Sources"

# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_NetworkPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ProcessingPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ConnectionPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_PortConnectionPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ControlConnectionPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_InPortPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_OutPortPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_InControlPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_OutControlPresentation.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ProcessingConfigPresentation.cxx
# End Source File
# End Group
# Begin Group "Interface Sources"

# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Interface\MainWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Interface\ProcessingTree.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Interface\ProcessingItem.cxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Sources No. 1"

# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\Visualization\ProcessingController.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\Visualization\NetworkController.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Storage Sources"

# Begin Group "XML Sources"

# Begin Source File

SOURCE=..\..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLComponentAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomWriter.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources No. 2"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Flow Sources"

# Begin Group "Controls Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# End Group
# Begin Group "Ports Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.cxx
# End Source File
# End Group
# Begin Group "Nodes Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\ReadStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\SourceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\WriteStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\Node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\DelayStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AddStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegionContainer.cxx
# End Source File
# End Group
# Begin Group "Networks Sources"

# Begin Source File

SOURCE=..\..\..\src\Flow\Networks\Network.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Networks\FlowControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Networks\ProcessingDefinitionAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Networks\ConnectionDefinitionAdapter.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources"

# Begin Group "Base Sources No. 3"

# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# End Group
# End Group
# Begin Group "System Sources"

# Begin Group "Threads Sources"

# Begin Source File

SOURCE=..\..\..\src\System\Threads\Condition.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Thread.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.cxx
# End Source File
# End Group
# End Group
# Begin Group "Data Sources"

# Begin Group "BasicProcessing Sources"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\src\Standard\Text.cxx
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "C: Headers"

# Begin Group "Qt Headers"

# Begin Group "3.0.0 Headers"

# Begin Group "include Headers"

# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qglobal.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qobjectdefs.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qshared.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qgarray.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qmemarray.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qcstring.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qstring.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qnamespace.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qwindowdefs.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpoint.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qsize.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qrect.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qregion.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qiodevice.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qdatastream.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpair.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qtextstream.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qtl.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qmap.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qmime.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qevent.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qobject.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpaintdevice.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qcolor.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qbrush.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpalette.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qfont.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qfontmetrics.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qfontinfo.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qsizepolicy.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qwidget.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qframe.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qdockwindow.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qtoolbar.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qptrcollection.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qglist.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qptrlist.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qmainwindow.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpointarray.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpen.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qwmatrix.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpainter.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpixmap.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qstrlist.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qvaluelist.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qstringlist.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qimage.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qdragobject.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qrangecontrol.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qscrollbar.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qscrollview.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qlistview.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qstatusbar.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qkeysequence.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qbutton.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qlayout.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qiconset.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qvariant.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qsignal.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qmenudata.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpopupmenu.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qmenubar.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qfile.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qdatetime.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qfileinfo.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qdir.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qdialog.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qurl.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qurlinfo.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qgdict.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qdict.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qnetworkprotocol.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qurloperator.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qfiledialog.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qdesktopwidget.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qasciidict.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qtranslator.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\quuid.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qapplication.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qhbox.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qvbox.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qtooltip.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qlineedit.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qcursor.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qbitmap.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qheader.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qlabel.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qgroupbox.h
# End Source File
# Begin Source File

SOURCE=C:\Qt\3.0.0\include\qpushbutton.h
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "Visualization Headers"

# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Connection.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slot.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signal.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ConnectionHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv1ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv0ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Presentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv2ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\ModelController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Slotv3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv3ImplSerious.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Visualization\Base\Signalv3.hxx
# End Source File
# End Group
# End Group
# Begin Group "externals Headers"

# Begin Group "CbLib Headers"

# Begin Source File

SOURCE=..\..\..\externals\CbLib\FunctorBase.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor0R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor1R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor2R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor3R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\Functor4R.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBLMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\externals\CbLib\CBL.hxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrSystem.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\mtgsstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Group "Windows Headers"

# Begin Source File

SOURCE=..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\OSDefines.hxx
# End Source File
# End Group
# Begin Group "examples Headers"

# Begin Group "NetworkEditor Headers"

# Begin Group "GUI Headers"

# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Base\NetworkPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Base\ProcessingConfigPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Base\ProcessingPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Base\ConnectionPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Base\ConnectionPointPresentation.hxx
# End Source File
# End Group
# Begin Group "Qt_Presentations Headers"

# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_NetworkPresentation.hxx
# End Source File
#Begin Source File

SOURCE="..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ProcessingPresentation.hxx"

!IF "$(CFG)" == "QtSupport - Win32 Release "

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing Qt_ProcessingPresentation.hxx...

".\moc\Qt_ProcessingPresentation_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\3.0.0\bin\moc.exe ..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ProcessingPresentation.hxx -o .\moc\Qt_ProcessingPresentation_moc.cxx 

# End Custom Build 

!ELSEIF "$(CFG)" == "QtSupport - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing Qt_ProcessingPresentation.hxx...

".\moc\Qt_ProcessingPresentation_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\3.0.0\bin\moc.exe ..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ProcessingPresentation.hxx -o .\moc\Qt_ProcessingPresentation_moc.cxx 

# End Custom Build 

!ENDIF

#End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ConnectionPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_PortConnectionPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ControlConnectionPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_InPortPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_OutPortPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_InControlPresentation.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_OutControlPresentation.hxx
# End Source File
#Begin Source File

SOURCE="..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ProcessingConfigPresentation.hxx"

!IF "$(CFG)" == "QtSupport - Win32 Release "

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing Qt_ProcessingConfigPresentation.hxx...

".\moc\Qt_ProcessingConfigPresentation_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\3.0.0\bin\moc.exe ..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ProcessingConfigPresentation.hxx -o .\moc\Qt_ProcessingConfigPresentation_moc.cxx 

# End Custom Build 

!ELSEIF "$(CFG)" == "QtSupport - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing Qt_ProcessingConfigPresentation.hxx...

".\moc\Qt_ProcessingConfigPresentation_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\3.0.0\bin\moc.exe ..\..\..\examples\NetworkEditor\GUI\Qt_Presentations\Qt_ProcessingConfigPresentation.hxx -o .\moc\Qt_ProcessingConfigPresentation_moc.cxx 

# End Custom Build 

!ENDIF

#End Source File
# End Group
# Begin Group "Interface Headers"

#Begin Source File

SOURCE="..\..\..\examples\NetworkEditor\GUI\Interface\MainWindow.hxx"

!IF "$(CFG)" == "QtSupport - Win32 Release "

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing MainWindow.hxx...

".\moc\MainWindow_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\3.0.0\bin\moc.exe ..\..\..\examples\NetworkEditor\GUI\Interface\MainWindow.hxx -o .\moc\MainWindow_moc.cxx 

# End Custom Build 

!ELSEIF "$(CFG)" == "QtSupport - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing MainWindow.hxx...

".\moc\MainWindow_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\3.0.0\bin\moc.exe ..\..\..\examples\NetworkEditor\GUI\Interface\MainWindow.hxx -o .\moc\MainWindow_moc.cxx 

# End Custom Build 

!ENDIF

#End Source File
#Begin Source File

SOURCE="..\..\..\examples\NetworkEditor\GUI\Interface\ProcessingTree.hxx"

!IF "$(CFG)" == "QtSupport - Win32 Release "

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing ProcessingTree.hxx...

".\moc\ProcessingTree_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\3.0.0\bin\moc.exe ..\..\..\examples\NetworkEditor\GUI\Interface\ProcessingTree.hxx -o .\moc\ProcessingTree_moc.cxx 

# End Custom Build 

!ELSEIF "$(CFG)" == "QtSupport - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - MOCing ProcessingTree.hxx...

".\moc\ProcessingTree_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\3.0.0\bin\moc.exe ..\..\..\examples\NetworkEditor\GUI\Interface\ProcessingTree.hxx -o .\moc\ProcessingTree_moc.cxx 

# End Custom Build 

!ENDIF

#End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\GUI\Interface\ProcessingItem.hxx
# End Source File
# End Group
# End Group
# Begin Group "Visualization Headers No. 1"

# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\Visualization\ProcessingController.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\examples\NetworkEditor\Visualization\NetworkController.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers No. 2"

# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# Begin Group "XML Headers"

# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\BasicXMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLComponentAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLIterableAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesEncodings.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesInitializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomWriter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomDocumentHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomWritingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Storage\XML\XercesDomReadingContext.hxx
# End Source File
# End Group
# End Group
# Begin Group "Base Headers No. 3"

# Begin Source File

SOURCE=..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\Factory.hxx
# End Source File
# End Group
# Begin Group "Flow Headers"

# Begin Group "Controls Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\ControlLinker.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\InControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Controls\OutControl.hxx
# End Source File
# End Group
# Begin Group "Ports Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# End Group
# Begin Group "Nodes Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\ReadStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\SourceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\WriteStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\Node.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\DelayStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AddStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamRegionContainer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\CircularStreamImpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AudioStreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\StreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\AudioNodeTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Nodes\NodeTmpl.hxx
# End Source File
# End Group
# Begin Group "Networks Headers"

# Begin Source File

SOURCE=..\..\..\src\Flow\Networks\Network.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Networks\FlowControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Networks\ProcessingDefinitionAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Flow\Networks\ConnectionDefinitionAdapter.hxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\FastRounding.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\PhantomBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\Text.hxx
# End Source File
# End Group
# Begin Group "Processing Headers"

# Begin Group "Base Headers No. 4"

# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# End Group
# Begin Group "pthreads Headers"

# Begin Group "include Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\pthreads\include\pthread.h
# End Source File
# End Group
# End Group
# Begin Group "System Headers"

# Begin Group "Threads Headers"

# Begin Source File

SOURCE=..\..\..\src\System\Threads\xtime.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Lock.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Condition.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Thread.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\System\Threads\Mutex.hxx
# End Source File
# End Group
# End Group
# Begin Group "Data Headers"

# Begin Group "Base Headers No. 5"

# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# End Group
# Begin Group "BasicProcessing Headers"

# Begin Source File

SOURCE=..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# Begin Group "include Headers No. 2"

# Begin Group "xercesc Headers No. 1"

# Begin Group "util Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\XMemory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\parsers\XercesDOMParser.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\framework\MemBufFormatTarget.hpp
# End Source File
# End Group
# Begin Group "sax Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\DocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\DTDHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\EntityResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\ErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\SAXParseException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\sax\HandlerBase.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationLS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMRangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationRegistry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMWriterFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentRange.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentTraversal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\xercesc\include\xercesc\dom\DOMNodeList.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group ". Headers"

# Begin Group "uic Headers"

# Begin Source File

SOURCE=.\uic\ControlPanel.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Qt .ui Files"

# Begin Group ". Headers"

#Begin Source File

SOURCE=".\ControlPanel.ui"

!IF "$(CFG)" == "QtSupport - Win32 Release "

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ControlPanel.ui...

BuildCmds= \
	C:\Qt\3.0.0\bin\uic.exe .\ControlPanel.ui -o .\uic\ControlPanel.h \
	C:\Qt\3.0.0\bin\uic.exe .\ControlPanel.ui -i ControlPanel.h -o .\uic\ControlPanel.cxx \
	C:\Qt\3.0.0\bin\moc.exe .\uic\ControlPanel.h -o .\moc\ControlPanel_moc.cxx \


".\uic\ControlPanel.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
".\uic\ControlPanel.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
".\moc\ControlPanel_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build 

!ELSEIF "$(CFG)" == "QtSupport - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - UICing ControlPanel.ui...

BuildCmds= \
	C:\Qt\3.0.0\bin\uic.exe .\ControlPanel.ui -o .\uic\ControlPanel.h \
	C:\Qt\3.0.0\bin\uic.exe .\ControlPanel.ui -i ControlPanel.h -o .\uic\ControlPanel.cxx \
	C:\Qt\3.0.0\bin\moc.exe .\uic\ControlPanel.h -o .\moc\ControlPanel_moc.cxx \


".\uic\ControlPanel.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
".\uic\ControlPanel.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
".\moc\ControlPanel_moc.cxx" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build 

!ENDIF

#End Source File
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "QtSupport - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ELSEIF  "$(CFG)" == "QtSupport - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
