# Microsoft Developer Studio Project File - Name="NonSupervisedSystemWithNodesExample" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=NonSupervisedSystemWithNodesExample - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NonSupervisedSystemWithNodesExample.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NonSupervisedSystemWithNodesExample.mak" CFG="NonSupervisedSystemWithNodesExample - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NonSupervisedSystemWithNodesExample - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "NonSupervisedSystemWithNodesExample - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "CLAM_Skeleton_project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NonSupervisedSystemWithNodesExample - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\ReleaseFiles\bin"
# PROP Intermediate_Dir "..\ReleaseFiles\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Generators" /I "..\..\..\..\src\Processing\AudioFileIO" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Tools\AudioIO" /I "..\..\..\..\src\Processing\AudioIO" /I "..\..\..\..\src\Processing\Controls" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\..\dxsdk\include" /I "..\..\..\..\src\Tools\AudioFileIO" /FI"preinclude.hxx" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_DOUBLE" /FD /c
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 dsound.lib dxerr8.lib user32.lib gdi32.lib portmidi.lib porttime.lib winmm.lib /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\portmidi\lib" /libpath:"..\..\..\..\..\dxsdk\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "NonSupervisedSystemWithNodesExample - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\DebugFiles\bin"
# PROP Intermediate_Dir "..\DebugFiles\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Data\Base" /I "..\..\..\..\src\Data\BasicProcessing" /I "..\..\..\..\src\Flow\Nodes" /I "..\..\..\..\src\Flow\Controls" /I "..\..\..\..\src\Processing\Base" /I "..\..\..\..\src\Flow\Ports" /I "..\..\..\..\src\Processing\Generators" /I "..\..\..\..\src\Processing\AudioFileIO" /I "..\..\..\..\src\Processing\ArithOps" /I "..\..\..\..\src\Tools\AudioIO" /I "..\..\..\..\src\Processing\AudioIO" /I "..\..\..\..\src\Processing\Controls" /I "..\..\..\..\src\Defines\Windows" /I "..\..\..\..\..\dxsdk\include" /I "..\..\..\..\src\Tools\AudioFileIO" /FI"preinclude.hxx" /D "_DEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_DOUBLE" /FD /GZ /Zm1000 /c
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dsound.lib dxerr8.lib user32.lib gdi32.lib portmidid.lib porttimed.lib winmm.lib /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\portmidi\lib" /libpath:"..\..\..\..\..\dxsdk\lib"
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "NonSupervisedSystemWithNodesExample - Win32 Release"
# Name "NonSupervisedSystemWithNodesExample - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "examples Sources"

# PROP Default_Filter ""
# Begin Group "FlowControlExamples Sources"

# PROP Default_Filter ""
# Begin Group "NonSupervisedSystemWithNodes Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\examples\FlowControlExamples\NonSupervisedSystemWithNodes\main.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\examples\FlowControlExamples\NonSupervisedSystemWithNodes\NonSupervisedSystemWithNodes.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Tools Sources"

# PROP Default_Filter ""
# Begin Group "AudioFileIO Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AIFFFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\extended.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundFileIO.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\WaveFileIO.cxx
# End Source File
# End Group
# Begin Group "AudioIO Sources"

# PROP Default_Filter ""
# Begin Group "Windows Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\Windows\DirectXAudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\Windows\DXFullDuplex.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\Windows\RtAAudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\Windows\RtAudio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\Windows\RtAudioUtils.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioDevice.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioDeviceList.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioManager.cxx
# End Source File
# End Group
# End Group
# Begin Group "Errors Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Flow Sources"

# PROP Default_Filter ""
# Begin Group "Nodes Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\AddStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\DelayStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\InplaceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\ReadStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\SourceStreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegion.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegionContainer.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\WriteStreamRegion.cxx
# End Source File
# End Group
# Begin Group "Controls Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\OutControl.cxx
# End Source File
# End Group
# Begin Group "Ports Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\Port.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# Begin Group "Standard Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Flags.cxx
# End Source File
# End Group
# Begin Group "Data Sources"

# PROP Default_Filter ""
# Begin Group "BasicProcessing Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.cxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Sources"

# PROP Default_Filter ""
# Begin Group "Base Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\Processing.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingComposite.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutControls.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutPorts.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\TopLevelProcessing.cxx
# End Source File
# End Group
# Begin Group "Generators Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Generators\Oscillator.cxx
# End Source File
# End Group
# Begin Group "AudioFileIO Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileOut.cxx
# End Source File
# End Group
# Begin Group "ArithOps Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMultiplier.cxx
# End Source File
# End Group
# Begin Group "AudioIO Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioIn.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioOut.cxx
# End Source File
# End Group
# Begin Group "Controls Sources No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Controls\AutoPanner.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Group "Errors Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrProcessingObj.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrSoundFileIO.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# PROP Default_Filter ""
# Begin Group "Windows Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Defines\Windows\CLAM_windows.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\mtgsstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\OSDefines.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# End Group
# Begin Group "Flow Headers"

# PROP Default_Filter ""
# Begin Group "Nodes Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\AddStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\AudioNodeTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\AudioStreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\CircularStreamImpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\DelayStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\InplaceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\Node.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\NodeTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\ReadStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\SourceStreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamBuffer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegion.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\StreamRegionContainer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Nodes\WriteStreamRegion.hxx
# End Source File
# End Group
# Begin Group "Controls Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\ControlLinker.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\InControlArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Controls\OutControl.hxx
# End Source File
# End Group
# Begin Group "Ports Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\AudioInPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\AudioOutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPortArrayTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\InPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPort.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\OutPortTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Flow\Ports\Port.hxx
# End Source File
# End Group
# End Group
# Begin Group "Base Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# End Group
# Begin Group "Storage Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# End Group
# Begin Group "Standard Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\CommonOps.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Flags.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PhantomBuffer.hxx
# End Source File
# End Group
# Begin Group "Data Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\Base\ProcessingData.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Data\Base\ProcessingDataConfig.hxx
# End Source File
# End Group
# Begin Group "BasicProcessing Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Data\BasicProcessing\Audio.hxx
# End Source File
# End Group
# End Group
# Begin Group "Processing Headers"

# PROP Default_Filter ""
# Begin Group "Base Headers No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\Processing.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\ProcessingComposite.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedInPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutControls.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\PublishedOutPorts.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Base\TopLevelProcessing.hxx
# End Source File
# End Group
# Begin Group "Generators Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Generators\Oscillator.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioFileIO\AudioFileOut.hxx
# End Source File
# End Group
# Begin Group "ArithOps Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMixer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\AudioMultiplier.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\ArithOps\BinaryAudioOp.hxx
# End Source File
# End Group
# Begin Group "AudioIO Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioIn.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Processing\AudioIO\AudioOut.hxx
# End Source File
# End Group
# Begin Group "Controls Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Processing\Controls\AutoPanner.hxx
# End Source File
# End Group
# End Group
# Begin Group "Tools Headers"

# PROP Default_Filter ""
# Begin Group "AudioIO Headers No. 1"

# PROP Default_Filter ""
# Begin Group "Windows Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\Windows\DXFullDuplex.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\Windows\RtAudio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\Windows\RtAudioUtils.hxx
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioDevice.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioDeviceList.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioIO\AudioManager.hxx
# End Source File
# End Group
# Begin Group "AudioFileIO Headers No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AIFFFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\extended.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundFileIO.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\SoundHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\WaveFileIO.hxx
# End Source File
# End Group
# End Group
# Begin Group "examples Headers"

# PROP Default_Filter ""
# Begin Group "FlowControlExamples Headers"

# PROP Default_Filter ""
# Begin Group "NonSupervisedSystemWithNodes Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\examples\FlowControlExamples\NonSupervisedSystemWithNodes\NonSupervisedSystemWithNodes.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "dxsdk Headers"

# PROP Default_Filter ""
# Begin Group "include Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\dxsdk\include\basetsd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\dxsdk\include\dinput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\dxsdk\include\dsound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\dxsdk\include\dxerr8.h
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "NonSupervisedSystemWithNodesExample - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	srcdeps.exe settings.cfg NonSupervisedSystemWithNodesExample.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "NonSupervisedSystemWithNodesExample - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	srcdeps.exe settings.cfg NonSupervisedSystemWithNodesExample.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
