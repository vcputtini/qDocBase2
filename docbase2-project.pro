#
#
#

message(WARING: IN WINDOWS ENVIRONMENT:  \
- MariaDB or MySQL must be 32BITS distribution.)

TEMPLATE = subdirs
#CONFIG += ordered
CONFIG += release

SUBDIRS += \
   src/qdocbase2.pro
	
qdocbas2.subdir = src/
