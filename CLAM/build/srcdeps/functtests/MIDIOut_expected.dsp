# Microsoft Developer Studio Project File - Name="MIDIOut" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MIDIOut - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MIDIOut.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MIDIOut.mak" CFG="MIDIOut - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MIDIOut - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MIDIOut - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MIDIOut - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\portmidi\include" /I "..\..\..\src\Base" /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\MIDIIO" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\Tools\MIDIIO"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\portmidi\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" pthreadVC.lib portmidi.lib porttime.lib winmm.lib libsndfile.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib id3lib.lib libmad.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRTD /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "MIDIOut - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "HAVE_STANDARD_UNICODE" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\portmidi\include" /I "..\..\..\src\Base" /I "..\..\..\src\Defines" /I "..\..\..\src\Errors" /I "..\..\..\src\Flow\Controls" /I "..\..\..\src\Flow\Ports" /I "..\..\..\src\Processing\Base" /I "..\..\..\src\Processing\MIDIIO" /I "..\..\..\src\Standard" /I "..\..\..\src\Storage\Base" /I "..\..\..\src\Storage\XML" /I "..\..\..\src\Tools\MIDIIO"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\pthreads\lib" /libpath:"..\..\..\..\portmidi\lib" /libpath:"..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\libmad\lib" /libpath:"..\..\..\..\id3lib\lib" pthreadVC.lib portmidid.lib porttimed.lib winmm.lib libsndfiled.lib ogg_static_d.lib vorbis_static_d.lib vorbisenc_static_d.lib vorbisfile_static_d.lib id3libd.lib libmadd.lib /nologo /subsystem:console /NODEFAULTLIB:MSVCRT /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "MIDIOut - Win32 Release"
# Name "MIDIOut - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "examples Sources"

# Begin Group "MIDIOut Sources"

# Begin Source File

SOURCE=..\..\..\examples\MIDIOut\main.cxx
# End Source File
# End Group
# End Group
# Begin Group "Tools Sources"

# Begin Group "MIDIIO Sources"

# Begin Group "PortMIDI Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\PortMIDI\PortMIDIDevice.cxx
# End Source File
# End Group
# Begin Group "File Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\FileMIDIDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDISong.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDITempo.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIReader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDISongPlayer.cxx
# End Source File
# End Group
# Begin Group "TextFile Sources"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\TextFile\TextFileMIDIDevice.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIDeviceList.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIManager.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIEnums.cxx
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

SOURCE=..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\src\Defines\Assert.cxx
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
# End Group
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Processing Sources"

# Begin Group "Base Sources No. 1"

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
# Begin Group "MIDIIO Sources No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIInControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIOut.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIOutControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIClocker.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

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

SOURCE=..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\EDataFormat.hxx
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

SOURCE=..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Defines\OSDefines.hxx
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
# End Group
# End Group
# Begin Group "Tools Headers"

# Begin Group "MIDIIO Headers"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIDevice.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIDeviceList.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIManager.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\MIDIEnums.hxx
# End Source File
# Begin Group "File Headers"

# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIDataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIMessage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIEvent.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDITrack.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDISong.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDITempo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDIReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Tools\MIDIIO\File\MIDISongPlayer.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers"

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
# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Base\DynamicType.hxx
# End Source File
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Standard\FastRounding.hxx
# End Source File
# End Group
# Begin Group "Processing Headers"

# Begin Group "Base Headers No. 2"

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
# Begin Group "MIDIIO Headers No. 1"

# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIInControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIOut.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIOutControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Processing\MIDIIO\MIDIClocker.hxx
# End Source File
# End Group
# End Group
# Begin Group "portmidi Headers"

# Begin Group "include Headers"

# Begin Source File

SOURCE=..\..\..\..\portmidi\include\portmidi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\portmidi\include\porttime.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Qt .ui Files"

# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "MIDIOut - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ELSEIF  "$(CFG)" == "MIDIOut - Win32 Debug"

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
