#
#
#

message(WARING: IN WINDOWS ENVIRONMENT:  \
- MariaDB or MySQL must be 32BITS distribution.)
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
# - VCPLIB2 and qDocBase2: Both Must be compiled into 32bits.)

>>>>>>> f3834ea18c73921b0cb662a98523ccf016904561
>>>>>>> dab3220d751d9d8e6a4c0956d8e51c857fc24141

TEMPLATE = subdirs
#CONFIG += ordered
CONFIG += release

SUBDIRS += \
<<<<<<< HEAD
    src/qdocbase2.pro

qdocbas2.subdir = src/
=======
   src/qdocbase2.pro
<<<<<<< HEAD
	
qdocbas2.subdir = src/
=======
#   vcplib2/vcplib2.pro
	
#vcplib2.subdir = vcplib2/
qdocbas2.subdir = src/

#qdocbase2.depends = vcplib2
>>>>>>> f3834ea18c73921b0cb662a98523ccf016904561
>>>>>>> dab3220d751d9d8e6a4c0956d8e51c857fc24141
