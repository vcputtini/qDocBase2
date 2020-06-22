#
#
#

message(WARING: IN WINDOWS ENVIRONMENT:  \
- MariaDB or MySQL must be 32BITS distribution.)
# - VCPLIB2 and qDocBase2: Both Must be compiled into 32bits.)


TEMPLATE = subdirs
#CONFIG += ordered
CONFIG += release

SUBDIRS += \
   src/qdocbase2.pro
#   vcplib2/vcplib2.pro
	
#vcplib2.subdir = vcplib2/
qdocbas2.subdir = src/

#qdocbase2.depends = vcplib2
