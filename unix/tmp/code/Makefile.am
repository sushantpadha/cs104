bin_PROGRAMS = testPQ testUtil ControlServer byte_op testTCP
ControlServer_SOURCES = ControlServer.cpp
ControlServer_LDADD = libUtil.a
testUtil_SOURCES = testUtil.cpp
testUtil_LDADD = libUtil.a
testPQ_SOURCES = testPQ.cpp
testPQ_LDADD = libUtil.a
byte_op_SOURCES = byte_op.cpp
testTCP_SOURCES = testTCP.cpp
testTCP_LDADD = libUtil.a
lib_LIBRARIES = libUtil.a
libUtil_a_SOURCES = Util.cpp PriorityQueue.cpp Debug.cpp KeyValue.cpp \
	LogLine.cpp Control.cpp JString.cpp Config.cpp Tree.cpp Graph.cpp
libUtil_a_LIBADD = Util.o PriorityQueue.o Debug.o KeyValue.o \
	LogLine.o Control.o JString.o Config.o Tree.o Graph.o
SUBDIRS = newran02

xfer-nfs:
	rsync -avz $(bin_PROGRAMS) @NFS@/util/

