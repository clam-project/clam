dnl Some aditional checks for the CLAM library
dnl
dnl
dnl Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
dnl                         UNIVERSITAT POMPEU FABRA
dnl
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


AC_DEFUN(CLAM_CHECK_CXX,
[
	AC_MSG_CHECKING([if default compiler g++ is acceptable])
	CXXVERSION=`g++ -v 2>&1 | grep version | cut -d' ' -f3`
	CXXVERSION_MAJOR=`echo $CXXVERSION | cut -d'.' -f1`
	CXXVERSION_MINOR=`echo $CXXVERSION | cut -d'.' -f2`
	if test $CXXVERSION_MAJOR = 3 ; then
		CXX=g++
		AC_MSG_RESULT(yes ($CXX=$CXXVERSION))
	elif test $CXXVERSION_MAJOR = 2 ; then
		if test $CXXVERSION_MINOR = 96 ; then
			AC_MSG_RESULT(no ($CXXVERSION))
			AC_MSG_CHECKING([if prefered g++ 3 is available])
			if test x`which g++3` != x ; then
				CXX=g++3
				CXXVERSION=`$CXX -v 2>&1 | grep version | cut -d' ' -f3`
				AC_MSG_RESULT(yes ($CXX=$CXXVERSION))
			elif test x`which g++-3.0` != x ; then
				CXX=g++-3.0
				CXXVERSION=`$CXX -v 2>&1 | grep version | cut -d' ' -f3`
				AC_MSG_RESULT(yes ($CXX=$CXXVERSION))
			fi			
		else
			AC_MSG_RESULT(yes ($CXX=$CXXVERSION))
		fi
	else
		AC_MSG_ERROR([cannot handle g++ version ($CXXVERSION)]);
	fi
])

AC_DEFUN(CLAM_CHECK_LIBCXX_FEATURES,
[
dnl TODO: We should check if c++ and libc++ are available first...
	
	dnl Try the vector::at method.
	AC_MSG_CHECKING([for std::vector::at() method in libstdc++])
    AC_TRY_RUN([
#include<vector>
int main() {
	std::vector<int> v(1);
	v[0]=0;
	return v.at(0);
}
    ],vec_at_fails=no,vec_at_fails=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

	if test "$vec_at_fails" = no ; then
    	AC_MSG_RESULT(yes)
		DEFINE_HAVE_STANDARD_VECTOR_AT=HAVE_STANDARD_VECTOR_AT
	else
    	AC_MSG_RESULT(no)
	fi

    dnl We now try to make the standard sstream header work
	dnl Really old versions of g++ do not have it.
    AC_MSG_CHECKING([for standard sstream header in libstdc++])
    AC_TRY_RUN([
#include<sstream>
int main() {
	std::stringstream ss;
	ss << "hi!" << std::ends;
	return 0;
}
    ],sstream_fails=no,sstream_fails=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

	if test "$sstream_fails" = no ; then

    	AC_MSG_RESULT(yes)
				DEFINE_HAVE_STANDARD_SSTREAM=HAVE_STANDARD_SSTREAM
		DEFINE_HAVE_STANDARD_SSTREAM_STR=HAVE_STANDARD_SSTREAM_STR
		echo '#define STANDARD_SSTREAM' >> confdefs.h
	else

	    AC_MSG_RESULT(no)
		dnl We don't have the standard, so we check if older headers are provided
	    AC_MSG_CHECKING([for obsolete strstream header in libstdc++])
	    AC_TRY_RUN([
#include<strstream>
int main() {
	std::strstream ss;
	ss << "hi!" << std::ends;
}
	    ],strstream_fails=no,strstream_fails=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

		if test "$strstream_fails" = no ; then

	    	AC_MSG_RESULT(yes)
				DEFINE_HAVE_STRSTREAM_SSTREAM=HAVE_STRSTREAM_SSTREAM
		dnl Bad luck. No known string stream header file found.
		else

			AC_MSG_ERROR([No standard c++ library String Streams header file found! ])

		fi
	fi 


	dnl It seems we have an lod string stream implementation. Lets check 
	dnl if it has any peculiarity...
	AC_MSG_CHECKING([if stringstream::str() returns std::string in libstdc++])
    AC_TRY_RUN([
#ifdef STANDARD_SSTREAM
#include <sstream>
#else
#include <strstream>
#endif
#include<iostream>
int main() {
#ifdef STANDARD_SSTREAM
	std::stringstream ss("hi");
#else
    std::strstream ss;
	ss << "hi";
#endif
	if ( ss.str().c_str()[0] == 'h')
      return 0;
    return 1;
}
    ],str_string_fails=no,str_string_fails=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

	if test "$str_string_fails" = no ; then
		AC_MSG_RESULT(yes)
		DEFINE_HAVE_STANDARD_SSTREAM_STR=HAVE_STANDARD_SSTREAM_STR
	else
		AC_MSG_RESULT(no)
	fi

])

AC_DEFUN(CLAM_LIB_FLTK,
[
AC_MSG_CHECKING([for fltk headers; looking relative to CLAM])
fltk_local=no
if test -d ../../fltk/include/FL/; then
   AC_MSG_RESULT(yes)
   found_fltk=yes
   FLTK_INCLUDES="-I../../fltk/include"
   FLTK_LIB_PATH="-L../../fltk/lib"
   fltk_local=yes
else
 AC_MSG_RESULT(no)
 AC_MSG_CHECKING([for fltk headers; looking in standard locations...])
 found_fltk=no
 for base in "/usr/include" \
             "/usr/local/include" \
             "/opt/include" \
             "/usr/"; do
   if test -d $base/FL; then
			AC_MSG_RESULT(yes)
      found_fltk=yes
      break;
   fi
 done
 FLTK_LIB_PATH=
fi		
if test $found_fltk = yes; then
   AC_MSG_CHECKING([for fltk library (and other fltk required)...])
   OLD_FLAGS=$CXXFLAGS

	 link_ok=no

	 if test $link_ok = no; then
	    FLTK_LIBS="-lfltk"
    	CXXFLAGS="$CXXFLAGS $FLTK_INCLUDES $FLTK_LIBS $FLTK_LIB_PATH"
			AC_TRY_LINK([
	 #include<FL/Fl_Window.H>
	 #include<FL/Fl.H>
			],[
	 Fl_Window w(100,100);
	 Fl::run();
	 return 0;
			],[
	  		 link_ok=yes
			],[])
	 fi

	 if test $link_ok = no; then
	    FLTK_LIBS="-lfltk -lGL"
    	CXXFLAGS="$CXXFLAGS $FLTK_INCLUDES $FLTK_LIBS $FLTK_LIB_PATH"
			AC_TRY_LINK([
	 #include<FL/Fl_Window.H>
	 #include<FL/Fl.H>
			],[
	 Fl_Window w(100,100);
	 Fl::run();
	 return 0;
			],[
	  		 link_ok=yes
			],[])
	 fi

	 if test $link_ok = no; then
	    FLTK_LIBS="-lfltk -lGL -lpthread"
    	CXXFLAGS="$CXXFLAGS $FLTK_INCLUDES $FLTK_LIBS $FLTK_LIB_PATH"
			AC_TRY_LINK([
	 #include<FL/Fl_Window.H>
	 #include<FL/Fl.H>
			],[
	 Fl_Window w(100,100);
	 Fl::run();
	 return 0;
			],[
	  		 link_ok=yes
			],[])
	 fi
	 
	 if test $link_ok = no; then
			AC_MSG_ERROR([
	  		 The test program did not compile or link. Check your config.log for details.
			])
	 else
			AC_MSG_RESULT(yes: [$FLTK_LIBS])
	 fi
	 
   AC_TRY_RUN([
#include<FL/Fl_Window.H>
#include<FL/Fl.H>
int main()
{
	Fl_Window w(100,100);
	Fl::run();
	return 0;
}
   ],[
      AC_MSG_RESULT(yes)
			DEFINE_HAVE_FLTK=HAVE_FLTK
      if test $fltk_local = yes; then
       FLTK_INCLUDES="-I\$(TOP)/../fltk/include"
       FLTK_LIB_PATH="-L\$(TOP)/../fltk/lib"
      fi
   ],[
The test program did compile and to link, but failed to run. This probably 
means that the run-time linker is not able to find libfltk.so. You might want
to set your LD_LIBRARY_PATH variable, or edit /etc/ld/ld.conf to point to
the right location.
	 ],[echo $ac_n "cross compiling; assumed OK... $ac_c"])
	 
   CXXFLAGS=$OLD_FLAGS
else
   AC_MSG_ERROR([
       No fltk headers found!
   ])
fi;
]
)

AC_DEFUN(CLAM_LIB_XERCESC,
[
	AC_MSG_CHECKING(XERCESCROOT environment variable defined?)
	if test "x$XERCESCROOT" != x ; then
		# We first try to find the include directory.
		AC_MSG_RESULT(yes)
		AC_MSG_CHECKING([for xercesc headers, using XERCESCROOT environment variable...])
		XERCESC_LIB_PATH=-L${XERCESCROOT}/lib
		found_dom=no
		if test -d $XERCESCROOT/include/dom ; then
			found_dom=yes
			XERCESC_INCLUDES=$XERCESCROOT/include
		elif test -d $XERCESCROOT/include/xercesc/dom; then
			found_dom=yes
			XERCESC_INCLUDES="$XERCESCROOT/include $XERCESCROOT/include/xercesc"
		else
			AC_MSG_ERROR([
				No xerces header directories found! 

				If you don't want to compile with XML support, run
				configure with the --disable-xml option.
			])
		fi
	else # XERCESCROOT 
		AC_MSG_RESULT(no)
		AC_MSG_CHECKING([for xercesc headers; looking relative to CLAM])
		xerces_local=no
		if test -d ../../xercesc/include/xercesc/dom; then
			AC_MSG_RESULT(yes)
			found_dom=yes
			export XERCESC_INCLUDES="`cd ../../xercesc/include/xercesc; pwd` `cd ../../xercesc/include; pwd`"
			export XERCESC_LIB_PATH="-L`cd ../../xercesc/lib; pwd`"
			xerces_local=yes
		elif test -d ../../xercesc/include/dom; then
			AC_MSG_RESULT(yes)
			found_dom=yes
			export XERCESC_INCLUDES=`cd ../../xercesc/include; pwd`
			export XERCESC_LIB_PATH="-L`../../xercesc/lib; pwd`"
			xerces_local=yes
		else
			AC_MSG_RESULT(no)
			AC_MSG_CHECKING([for xercesc headers; looking in standard locations...])
			found_dom=no
			for base in "/usr/include" \
			"/usr/local/include" \
			"/opt/include" \
			"/usr/"; do
				if test -d $base/dom; then
					AC_MSG_RESULT(yes)
					found_dom=yes
					break;
				fi
				if test -d $base/xercesc/dom; then
					AC_MSG_RESULT(yes)
					found_dom=yes
					export XERCESC_INCLUDES=$base/xercesc
					break;
				fi
			done
			XERCESC_LIB_PATH=
		fi		
	fi
	if test $found_dom = yes; then
		AC_MSG_CHECKING([for xercesc library...])
		OLD_FLAGS=$CXXFLAGS
		XERCESC_LIBS=-lxerces-c
		CXXFLAGS="$CXXFLAGS $XERCESC_LIBS $XERCESC_LIB_PATH"
		export CPLUS_INCLUDE_PATH=${XERCESC_INCLUDES// /\:}
		AC_TRY_RUN([
			#include<dom/DOM_Document.hpp>
			int main()
			{
				DOM_Document::createDocument();
				return 0;
			}
		],[
			AC_MSG_RESULT(yes)
			DEFINE_HAVE_XERCESC=HAVE_XERCESC
		],[
			AC_TRY_LINK([
					#include<dom/DOM_Document.hpp>
				],[
					DOM_Document::createDocument();
					return 0;
				],[
				AC_MSG_ERROR(
					[The test program did compile, but failed to link. This probably means that
					the run-time linker is not able to find libxercesc.so. You might want
					to set your LD_LIBRARY_PATH variable, or edit /etc/ld/ld.conf to point to
					the right location.]
					)
				],[
				AC_MSG_ERROR([
					The test program did not compile or link. Check your config.log for details.
				])
			])
		],[echo $ac_n "cross compiling; assumed OK... $ac_c"
		])
		CXXFLAGS=$OLD_FLAGS
	else
		AC_MSG_ERROR([
			No xerces headers found!

			If you don't want to compile with XML support, run
			configure with the --disable-xml option.
		])
	fi;
])




AC_DEFUN(CLAM_LIB_FFTW,
[
	if test "$usedouble" = no; then
		AC_MSG_CHECKING([for single precision fftw library])
    		OLD_LIBS=$LIBS
		AC_SEARCH_LIBS(fftw_sizeof_fftw_real,sfftw,
			[AC_CHECK_HEADER(srfftw.h,
					[ DEFINE_HAVE_FFTW=HAVE_FFTW
						FFTW_LIBS="srfftw sfftw"
					]
				)
			],
			[AC_MSG_CHECKING([for double precision fftw library])
			AC_SEARCH_LIBS(fftw_sizeof_fftw_real,fftw,
				[AC_CHECK_HEADER(rfftw.h,
						[ DEFINE_HAVE_FFTW=HAVE_FFTW
							FFTW_LIBS="rfftw fftw"
						]
					)
				],AC_MSG_ERROR([fftw not found])
				,-lfftw -lm)
			],-lsfftw -lm)
	else
		AC_MSG_CHECKING([for double precision fftw library])
	
    AC_SEARCH_LIBS(fftw_sizeof_fftw_real,fftw,
    	[AC_CHECK_HEADER(rfftw.h,
					[ DEFINE_HAVE_FFTW=HAVE_FFTW
						FFTW_LIBS="rfftw fftw"
					]
				)
			],
	AC_MSG_ERROR(
[fftw library (double precision) not found. Maybe you want to configure with 
--disable-double?]) 		
	,-lfftw -lm
    )
	fi
	LIBS=$OLD_LIBS
])

