include( qwtconfig.pri )

SOURCES += main.cpp \
           MainWin.cpp \
		   MemAdjustWig.cpp \
		   mempiemarker.cpp \
		   memplot.cpp \
		   memstat.cpp \
		   stdafx.cpp

HEADERS += MainWin.hxx \
		   MemAdjustWig.hxx \
		   mempiemarker.h \
		   memplot.hxx \
		   memstat.hxx \
		   stdafx.h

# Forms and resources
FORMS += MemAdjustWig.ui