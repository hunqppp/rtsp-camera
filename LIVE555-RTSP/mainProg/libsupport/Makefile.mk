CXXFLAGS 	+= -I./libsupport

VPATH 		+= libsupport

OBJS 		+= $(OBJ_USRCODE_DIR)/jsmn.o
OBJS 		+= $(OBJ_USRCODE_DIR)/Utils.o
OBJS 		+= $(OBJ_USRCODE_DIR)/h26XParser.o