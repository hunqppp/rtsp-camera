INCLUDES = -Iinclude -I../UsageEnvironment/include
PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
##### Change the following for your environment:
-include ../config.compiler.mk
OBJ_GROUPSOCk_DIR = ../$(BUILD_DIR)/groupsock
##### End of variables to change

NAME = libgroupsock
ALL = $(OBJ_GROUPSOCk_DIR)/$(NAME).$(LIB_SUFFIX)

GROUPSOCK_LIB_OBJS = GroupsockHelper.$(OBJ) GroupEId.$(OBJ) inet.$(OBJ) Groupsock.$(OBJ) NetInterface.$(OBJ) NetAddress.$(OBJ) IOHandlers.$(OBJ)
TARGET = $(addprefix $(OBJ_GROUPSOCk_DIR)/, $(GROUPSOCK_LIB_OBJS))

all: create $(ALL)

create:
	$(Print) $(KGRN)CREATE $(OBJ_GROUPSOCk_DIR) $(KRST)
	@mkdir -p $(OBJ_GROUPSOCk_DIR)

$(ALL): $(TARGET) $(PLATFORM_SPECIFIC_LIB_OBJS)
	$(Print) $(KCYC) LINK $(KRST) $(LIBRARY_LINK) $@
	@$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) $^

$(OBJ_GROUPSOCk_DIR)/%.$(OBJ): %.cpp
	$(Print)~ CXX $<
	@$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $< -o $@

$(OBJ_GROUPSOCk_DIR)/%.$(OBJ): %.c
	$(Print)~ CXX $<
	@$(C_COMPILER) -c $(C_FLAGS) $< -o $@

GroupsockHelper.$(CPP):	include/GroupsockHelper.hh
include/GroupsockHelper.hh:	include/NetAddress.hh
include/NetAddress.hh:	include/NetCommon.h
GroupEId.$(CPP):	include/GroupEId.hh
include/GroupEId.hh:	include/NetAddress.hh
inet.$(C):		include/NetCommon.h
Groupsock.$(CPP):	include/Groupsock.hh include/GroupsockHelper.hh
include/Groupsock.hh:	include/groupsock_version.hh include/NetInterface.hh include/GroupEId.hh
include/NetInterface.hh:	include/NetAddress.hh
NetInterface.$(CPP):	include/NetInterface.hh include/GroupsockHelper.hh
NetAddress.$(CPP):	include/NetAddress.hh include/GroupsockHelper.hh
IOHandlers.$(CPP):	include/IOHandlers.hh

clean:
	$(Print) $(KRED)REMOVE $(OBJ_GROUPSOCk_DIR) $(KRST);
	@rm -rf $(OBJ_GROUPSOCk_DIR)

install: install1 $(INSTALL2)
install1: libgroupsock.$(LIB_SUFFIX)
	  install -d $(DESTDIR)$(PREFIX)/include/groupsock $(DESTDIR)$(LIBDIR)
	  install -m 644 include/*.hh include/*.h $(DESTDIR)$(PREFIX)/include/groupsock
	  install -m 644 libgroupsock.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)
install_shared_libraries: libgroupsock.$(LIB_SUFFIX)
	  ln -fs libgroupsock.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/libgroupsock.$(SHORT_LIB_SUFFIX)
	  ln -fs libgroupsock.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/libgroupsock.so

##### Any additional, platform-specific rules come here:
