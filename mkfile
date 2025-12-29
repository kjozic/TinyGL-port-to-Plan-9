 </$objtype/mkfile

LIB=libString.a$O

OFILES= s_alloc.$O\

    s_append.$O\

    s_array.$O\

    s_copy.$O\

    s_getline.$O\

    s_grow.$O\

    s_nappend.$O\

    s_parse.$O\

    s_read.$O\

    s_read_line.$O\

    s_tolower.$O\

</sys/src/cmd/mklib

nuke:V:

    mk clean

    rm -f libString.a[$OS]




include config.mk

all:
	( for f in $(DIRS); do ( cd $$f ; make all ) || exit 1 ; done )

clean:
	rm -f *~ lib/libTinyGL.a include/GL/*~ TAGS
	( for f in $(DIRS); do ( cd $$f ; make clean ; ) done )

install:
	( for f in $(DIRS); do ( cd $$f ; make install ; ) done )


tar:
	( cd .. ; tar zcvf /tmp/TinyGL-0.4.1.tar.gz TinyGL --exclude .svn --exclude TAGS )
