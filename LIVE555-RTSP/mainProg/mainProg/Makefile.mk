-include mainProg/vvtk/Makefile.mk

CXXFLAGS 	+= -I./mainProg

VPATH 		+= mainProg

OBJS 		+= $(OBJ_USRCODE_DIR)/main.o
OBJS 		+= $(OBJ_USRCODE_DIR)/avSource.o
OBJS 		+= $(OBJ_USRCODE_DIR)/announceURL.o
OBJS 		+= $(OBJ_USRCODE_DIR)/h26XSource.o
OBJS 		+= $(OBJ_USRCODE_DIR)/H26XFrame.o
OBJS 		+= $(OBJ_USRCODE_DIR)/H264VideoSourceServerMediaSubsession.o
OBJS 		+= $(OBJ_USRCODE_DIR)/LiveServerMediaSubsession.o
