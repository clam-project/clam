# Microsoft Developer Studio Project File - Name="BPFTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=BPFTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BPFTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BPFTest.mak" CFG="BPFTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BPFTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "BPFTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "empty"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BPFTest - Win32 Release"

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
# ADD CPP -O3 -fomit-frame-pointer -pipe /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "CLAM_HAVE_PTHREADS" /D "HAVE_STANDARD_UNICODE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /I "..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\test\UnitTests\CommonHelpers" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML"
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 xerces-c.lib pthread.lib cppunit.lib sndfile.lib vorbis.lib ogg.lib vorbisfile.lib vorbisenc.lib mad.lib m.lib id3.lib z.lib
# SUBTRACT LINK32 /nologo /verbose /pdb:none /debug

!ELSEIF  "$(CFG)" == "BPFTest - Win32 Debug"

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
# ADD CPP -g -fno-inline -pipe -D_DEBUG /D "HAVE_STANDARD_SSTREAM" /D "HAVE_STANDARD_SSTREAM_STR" /D "CLAM_HAVE_PTHREADS" /D "HAVE_STANDARD_UNICODE" /D "CLAM_FLOAT" /D "CLAM_USE_XML" /I "..\..\..\..\src\Defines" /FI"preinclude.hxx" /I "..\..\..\..\test\UnitTests\CommonHelpers" /I "..\..\..\..\src\Base" /I "..\..\..\..\src\Defines" /I "..\..\..\..\src\Errors" /I "..\..\..\..\src\Standard" /I "..\..\..\..\src\Storage\Base" /I "..\..\..\..\src\Storage\XML"
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32
# ADD LINK32 xerces-c.lib pthread.lib cppunit.lib sndfile.lib vorbis.lib ogg.lib vorbisfile.lib vorbisenc.lib mad.lib m.lib id3.lib z.lib -rdynamic
# SUBTRACT LINK32 /nologo /verbose /pdb:none

!ENDIF 

# Begin Target

# Name "BPFTest - Win32 Release"
# Name "BPFTest - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Source Files"

# Begin Group "test Sources"

# Begin Group "NonPortedTests Sources"

# Begin Source File

SOURCE=..\..\..\..\test\NonPortedTests\BPFTest.cxx
# End Source File
# End Group
# Begin Group "UnitTests Sources"

# Begin Group "CommonHelpers Sources"

# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\CommonHelpers\XMLTestHelper.cxx
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Errors Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.cxx
# End Source File
# End Group
# Begin Group "Defines Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Defines\Assert.cxx
# End Source File
# End Group
# Begin Group "Standard Sources"

# Begin Source File

SOURCE=..\..\..\..\src\Standard\Point.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\GlobalEnums.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPF.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Text.cxx
# End Source File
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

# Begin Group "Defines Headers"

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

SOURCE=..\..\..\..\src\Defines\Assert.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\CLAM_Math.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\ByteOrder.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\EDataFormat.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\TypeInfoStd.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Defines\OSDefines.hxx
# End Source File
# End Group
# Begin Group "Errors Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Errors\Err.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrOutOfMemory.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Errors\ErrDynamicType.hxx
# End Source File
# End Group
# Begin Group "Standard Headers"

# Begin Source File

SOURCE=..\..\..\..\src\Standard\FastRounding.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Point.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\PointTmpl.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Array.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\SearchArray.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Enum.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\GlobalEnums.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmplDec.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPF.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\Text.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmplDef.hxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Standard\BPFTmpl.hxx
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

SOURCE=..\..\..\..\src\Storage\XML\XMLArrayAdapter.hxx
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
# Begin Group " Headers"

# Begin Group "usr Headers"

# Begin Group "include Headers"

# Begin Group "sys Headers"

# Begin Source File

SOURCE=\usr\include\sys\cdefs.h
# End Source File
# End Group
# Begin Group "gnu Headers"

# Begin Source File

SOURCE=\usr\include\gnu\stubs.h
# End Source File
# End Group
# Begin Source File

SOURCE=\usr\include\features.h
# End Source File
# Begin Group "bits Headers"

# Begin Source File

SOURCE=\usr\include\bits\wordsize.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\typesizes.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\types.h
# End Source File
# Begin Source File

SOURCE=\usr\include\bits\endian.h
# End Source File
# End Group
# Begin Source File

SOURCE=\usr\include\endian.h
# End Source File
# Begin Source File

SOURCE=\usr\include\ctype.h
# End Source File
# Begin Source File

SOURCE=\usr\include\time.h
# End Source File
# Begin Group "cppunit Headers"

# Begin Source File

SOURCE=\usr\include\cppunit\config-auto.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\Portability.h
# End Source File
# Begin Group "extensions Headers"

# Begin Source File

SOURCE=\usr\include\cppunit\extensions\TestFactory.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\extensions\TestSuiteFactory.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\extensions\TestFactoryRegistry.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\extensions\AutoRegisterSuite.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\extensions\TestSuiteBuilder.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\extensions\HelperMacros.h
# End Source File
# End Group
# Begin Source File

SOURCE=\usr\include\cppunit\Test.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\TestSuite.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\SourceLine.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\Exception.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\Asserter.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\TestAssert.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\TestFixture.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\TestCase.h
# End Source File
# Begin Source File

SOURCE=\usr\include\cppunit\TestCaller.h
# End Source File
# End Group
# Begin Group "xercesc Headers"

# Begin Group "util Headers"

# Begin Source File

SOURCE=\usr\include\xercesc\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\util\XMemory.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\util\PlatformUtils.hpp
# End Source File
# End Group
# Begin Group "parsers Headers"

# Begin Source File

SOURCE=\usr\include\xercesc\parsers\XercesDOMParser.hpp
# End Source File
# End Group
# Begin Group "framework Headers"

# Begin Source File

SOURCE=\usr\include\xercesc\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\framework\XMLFormatter.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\framework\MemBufFormatTarget.hpp
# End Source File
# End Group
# Begin Group "sax Headers"

# Begin Source File

SOURCE=\usr\include\xercesc\sax\DocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\sax\DTDHandler.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\sax\EntityResolver.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\sax\ErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\sax\SAXParseException.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\sax\HandlerBase.hpp
# End Source File
# End Group
# Begin Group "dom Headers"

# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMImplementationLS.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMException.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMRangeException.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMImplementationRegistry.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMNode.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMWriterFilter.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMWriter.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMDocumentRange.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMDocumentTraversal.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMDocument.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMElement.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMText.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=\usr\include\xercesc\dom\DOMNodeList.hpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# End Group
# Begin Group "test Headers"

# Begin Group "UnitTests Headers"

# Begin Group "CommonHelpers Headers"

# Begin Source File

SOURCE=..\..\..\..\test\UnitTests\CommonHelpers\XMLTestHelper.hxx
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "Qt .ui Files"

# End Group
# Begin Source File

SOURCE=settings.cfg

!IF  "$(CFG)" == "BPFTest - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=settings.cfg

"buildstamp" : $(SOURCE) "$(INTDIR)"
	srcdeps.exe settings.cfg

# End Custom Build

!ELSEIF  "$(CFG)" == "BPFTest - Win32 Debug"

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
