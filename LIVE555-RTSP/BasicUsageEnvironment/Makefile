INCLUDES = -Iinclude -I../UsageEnvironment/include -I../groupsock/include
PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
##### Change the following for your environment:
-include ../config.compiler.mk
OBJ_BASICUSAGEENVIRONMENT_DIR = ../$(BUILD_DIR)/BasicUsageEnvironment
##### End of variables to change

NAME = libBasicUsageEnvironment
LIB_FILE = $(NAME).$(LIB_SUFFIX)
ALL = $(OBJ_BASICUSAGEENVIRONMENT_DIR)/$(LIB_FILE)

OBJS = BasicUsageEnvironment0.$(OBJ) BasicUsageEnvironment.$(OBJ) \
	BasicTaskScheduler0.$(OBJ) BasicTaskScheduler.$(OBJ) \
	DelayQueue.$(OBJ) BasicHashTable.$(OBJ)

TARGET = $(addprefix $(OBJ_BASICUSAGEENVIRONMENT_DIR)/, $(OBJS))

all: create $(ALL)

create:
	$(Print) $(KGRN)CREATE $(OBJ_BASICUSAGEENVIRONMENT_DIR) $(KRST)
	@mkdir -p $(OBJ_BASICUSAGEENVIRONMENT_DIR)

$(ALL): $(TARGET)
	$(Print) $(KCYC) LINK $(KRST) $(LIBRARY_LINK) $@
	@$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) $^

$(OBJ_BASICUSAGEENVIRONMENT_DIR)/%.$(OBJ): %.cpp
	$(Print)~ CXX $<
	@$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $< -o $@

$(OBJ_BASICUSAGEENVIRONMENT_DIR)/%.$(OBJ): %.c
	$(Print)~ CXX $<
	@$(C_COMPILER) -c $(C_FLAGS) $< -o $@

BasicUsageEnvironment0.$(CPP):	include/BasicUsageEnvironment0.hh
include/BasicUsageEnvironment0.hh:	include/BasicUsageEnvironment_version.hh include/DelayQueue.hh
BasicUsageEnvironment.$(CPP):	include/BasicUsageEnvironment.hh
include/BasicUsageEnvironment.hh:	include/BasicUsageEnvironment0.hh
BasicTaskScheduler0.$(CPP):	include/BasicUsageEnvironment0.hh include/HandlerSet.hh
BasicTaskScheduler.$(CPP):	include/BasicUsageEnvironment.hh include/HandlerSet.hh
DelayQueue.$(CPP):		include/DelayQueue.hh
BasicHashTable.$(CPP):		include/BasicHashTable.hh

clean:
	$(Print) $(KRED)REMOVE $(OBJ_BASICUSAGEENVIRONMENT_DIR) $(KRST);
	@rm -rf $(OBJ_BASICUSAGEENVIRONMENT_DIR)

install: install1 $(INSTALL2)
install1: libBasicUsageEnvironment.$(LIB_SUFFIX)
	  install -d $(DESTDIR)$(PREFIX)/include/BasicUsageEnvironment $(DESTDIR)$(LIBDIR)
	  install -m 644 include/*.hh $(DESTDIR)$(PREFIX)/include/BasicUsageEnvironment
	  install -m 644 libBasicUsageEnvironment.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)
install_shared_libraries: libBasicUsageEnvironment.$(LIB_SUFFIX)
	  ln -fs $(NAME).$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/$(NAME).$(SHORT_LIB_SUFFIX)
	  ln -fs $(NAME).$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/$(NAME).so

##### Any additional, platform-specific rules come here: