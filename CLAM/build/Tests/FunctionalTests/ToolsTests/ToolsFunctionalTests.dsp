# Microsoft Developer Studio Project File - Name="ToolsFunctionalTests" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ToolsFunctionalTests - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ToolsFunctionalTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ToolsFunctionalTests.mak" CFG="ToolsFunctionalTests - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ToolsFunctionalTests - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ToolsFunctionalTests - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ToolsFunctionalTests - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /FD /c /Zm1000 /D "NDEBUG" /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\..\xercesc\include" /I "..\..\..\..\..\cppunit\include" /I "..\..\..\..\..\libsndfile\include" /I "..\..\..\..\..\oggvorbis\include" /I "..\..\..\..\..\libmad\include" /I "..\..\..\..\..\id3lib\include" /I "..\..\..\..\test\UnitTests" /I "..\..\..\..\test\UnitTests\CommonHelpers" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\Tools\AudioFileIO" /I "..\..\..\..\externals"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\cppunit\lib" /libpath:"..\..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\..\libmad\lib" /libpath:"..\..\..\..\..\id3lib\lib" xerces-c_2.lib cppunit_vc6.lib libsndfile.lib ogg_static.lib vorbis_static.lib vorbisenc_static.lib vorbisfile_static.lib id3lib.lib libmad.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "ToolsFunctionalTests - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /D "_DEBUG" /FD /GZ /Zm1000 /c /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "HAVE_STANDARD_VECTOR_AT" /D "WIN32" /D "_MBCS" /D "_CONSOLE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /D "ID3LIB_LINKOPTION=1" /I "..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\..\xercesc\include" /I "..\..\..\..\..\cppunit\include" /I "..\..\..\..\..\libsndfile\include" /I "..\..\..\..\..\oggvorbis\include" /I "..\..\..\..\..\libmad\include" /I "..\..\..\..\..\id3lib\include" /I "..\..\..\..\test\UnitTests" /I "..\..\..\..\test\UnitTests\CommonHelpers" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML" /I "..\..\..\..\src\Tools\AudioFileIO" /I "..\..\..\..\externals"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 /libpath:"..\..\..\..\..\xercesc\lib" /libpath:"..\..\..\..\..\cppunit\lib" /libpath:"..\..\..\..\..\libsndfile\lib" /libpath:"..\..\..\..\..\oggvorbis\lib" /libpath:"..\..\..\..\..\libmad\lib" /libpath:"..\..\..\..\..\id3lib\lib" xerces-c_2d.lib cppunitd_vc6.lib libsndfile.lib ogg_static_d.lib vorbis_static_d.lib vorbisenc_static_d.lib vorbisfile_static_d.lib id3libd.lib libmadd.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "ToolsFunctionalTests - Win32 Release"
# Name "ToolsFunctionalTests - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "test Sources"

# Begin Group "UnitTests Sources"

# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\TestRunnerConsole.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\cppUnitHelperTest.cxx
# End Source File
# Begin Group "ToolsTests Sources"

# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\ToolsTests\AudioFileTest.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Text.cxx
# End Source File
# End Group
# Begin Group "Tools Sources"

# Begin Group "AudioFileIO Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFileFormats.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFileHeader.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioTextDescriptors.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFile.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\PCMCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\OggVorbisCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegCodec.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioCodecs_Stream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\PCMAudioStream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\OggVorbisAudioStream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegBitstream.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegAudioStream.cxx
# End Source File
# End Group
# End Group
# Begin Group "Storage Sources"

# Begin Group "XML Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\BasicXMLable.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLComponentAdapter.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStorage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWriter.cxx
# End Source File
# End Group
# End Group
# Begin Group "Base Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.cxx
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# Begin Group "cppunit Headers"

# Begin Group "include Headers"

# Begin Group "cppunit Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\Portability.h
# End Source File
# Begin Group "extensions Headers"

# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\TestFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\TestFactoryRegistry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\TestSuiteFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\AutoRegisterSuite.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\TestSuiteBuilder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\extensions\HelperMacros.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\Test.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestSuite.h
# End Source File
# Begin Group "ui Headers"

# Begin Group "text Headers"

# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\ui\text\TestRunner.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\SourceLine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\Exception.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\Asserter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestAssert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestFixture.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestCase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\cppunit\include\cppunit\TestCaller.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "test Headers"

# Begin Group "UnitTests Headers"

# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\cppUnitHelper.hxx
# End Source File
# Begin Group "CommonHelpers Headers"

# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\CommonHelpers\BaseLoggable.hxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# End Group
# Begin Group "Defines Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\DataTypes.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\StaticBool.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfo.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\mtgsstream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\OSDefines.hxx
# End Source File
# End Group
# Begin Group "Storage Headers"

# Begin Group "Base Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\Base\Storage.hxx
# End Source File
# End Group
# Begin Group "XML Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\BasicXMLable.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLComponentAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLIterableAdapter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLStorage.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesEncodings.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesInitializer.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWriter.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomDocumentHandler.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomWritingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XercesDomReadingContext.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
# End Source File
# End Group
# End Group
# Begin Group "Base Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\src\Base\Component.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicTypeMacros.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Base\DynamicType.hxx
# End Source File
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Standard\Filename.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Text.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\FastRounding.hxx
# End Source File
# End Group
# Begin Group "libsndfile Headers"

# Begin Group "include Headers No. 1"

# Begin Source File

SOURCE=..\..\..\..\..\libsndfile\include\sndfile.h
# End Source File
# End Group
# End Group
# Begin Group "Tools Headers"

# Begin Group "AudioFileIO Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFileFormats.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFileHeader.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioTextDescriptors.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioFile.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\PCMCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\OggVorbisCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegCodec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\AudioCodecs_Stream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\PCMAudioStream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\OggVorbisAudioStream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegBitstream.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Tools\AudioFileIO\MpegAudioStream.hxx
# End Source File
# End Group
# End Group
# Begin Group "xercesc Headers"

# Begin Group "include Headers No. 2"

# Begin Group "xercesc Headers No. 1"

# Begin Group "util Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\XMemory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\util\PlatformUtils.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\parsers\XercesDOMParser.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\framework\MemBufFormatTarget.hpp
# End Source File
# End Group
# Begin Group "sax Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\DocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\DTDHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\EntityResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\ErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\SAXParseException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\sax\HandlerBase.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationLS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMRangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMImplementationRegistry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMWriterFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentRange.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocumentTraversal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\xercesc\include\xercesc\dom\DOMNodeList.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "oggvorbis Headers"

# Begin Group "include Headers No. 3"

# Begin Group "ogg Headers"

# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\ogg\os_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\ogg\ogg.h
# End Source File
# End Group
# Begin Group "vorbis Headers"

# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\vorbis\codec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\vorbis\vorbisfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\oggvorbis\include\vorbis\vorbisenc.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "externals Headers"

# Begin Source File

SOURCE=..\..\..\..\externals\deque
# End Source File
# End Group
# Begin Group "libmad Headers"

# Begin Group "include Headers No. 4"

# Begin Source File

SOURCE=..\..\..\..\..\libmad\include\mad.h
# End Source File
# End Group
# End Group
# Begin Group "id3lib Headers"

# Begin Group "include Headers No. 5"

# Begin Group "id3 Headers"

# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\sized_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\globals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\id3lib_frame.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\field.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\id3lib_streams.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\id3lib_strings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\id3lib\include\id3\tag.h
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "ToolsFunctionalTests - Win32 Release"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "ToolsFunctionalTests.dsp"
	srcdeps.exe settings.cfg ToolsFunctionalTests.dsp

# End Custom Build

!ELSEIF  "$(CFG)" == "ToolsFunctionalTests - Win32 Debug"

# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)" "ToolsFunctionalTests.dsp"
	srcdeps.exe settings.cfg ToolsFunctionalTests.dsp

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
