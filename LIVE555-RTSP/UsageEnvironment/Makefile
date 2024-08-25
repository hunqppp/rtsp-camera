INCLUDES = -Iinclude -I../groupsock/include
PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
##### Change the following for your environment:
-include ../config.compiler.mk
OBJ_USAGEENVIRONMENT_DIR = ../$(BUILD_DIR)/UsageEnvironment
##### End of variables to change

NAME = libUsageEnvironment
USAGE_ENVIRONMENT_LIB = $(NAME).$(LIB_SUFFIX)
ALL = $(OBJ_USAGEENVIRONMENT_DIR)/$(USAGE_ENVIRONMENT_LIB)

OBJS = UsageEnvironment.$(OBJ) HashTable.$(OBJ) strDup.$(OBJ)
TARGET = $(addprefix $(OBJ_USAGEENVIRONMENT_DIR)/, $(OBJS))

all: create $(ALL)

create:
	$(Print) $(KGRN)CREATE $(OBJ_USAGEENVIRONMENT_DIR) $(KRST)
	@mkdir -p $(OBJ_USAGEENVIRONMENT_DIR)

$(ALL): $(TARGET)
	$(Print) $(KCYC) LINK $(KRST) $(LIBRARY_LINK) $@
	@$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) $^

$(OBJ_USAGEENVIRONMENT_DIR)/%.$(OBJ): %.cpp
	$(Print)~ CXX $<
	@$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $< -o $@

$(OBJ_USAGEENVIRONMENT_DIR)/%.$(OBJ): %.c
	$(Print)~ CXX $<
	@$(C_COMPILER) -c $(C_FLAGS) $< -o $@

UsageEnvironment.$(CPP):	include/UsageEnvironment.hh
include/UsageEnvironment.hh:	include/UsageEnvironment_version.hh include/Boolean.hh include/strDup.hh
HashTable.$(CPP):		include/HashTable.hh
include/HashTable.hh:		include/Boolean.hh
strDup.$(CPP):			include/strDup.hh

clean:
	$(Print) $(KRED)REMOVE $(OBJ_USAGEENVIRONMENT_DIR) $(KRST);
	@rm -rf $(OBJ_USAGEENVIRONMENT_DIR)

install: install1 $(INSTALL2)
install1: $(USAGE_ENVIRONMENT_LIB)
	  install -d $(DESTDIR)$(PREFIX)/include/UsageEnvironment $(DESTDIR)$(LIBDIR)
	  install -m 644 include/*.hh $(DESTDIR)$(PREFIX)/include/UsageEnvironment
	  install -m 644 $(USAGE_ENVIRONMENT_LIB) $(DESTDIR)$(LIBDIR)
install_shared_libraries: $(USAGE_ENVIRONMENT_LIB)
	  ln -fs $(NAME).$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/$(NAME).$(SHORT_LIB_SUFFIX)
	  ln -fs $(NAME).$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/$(NAME).so

##### Any additional, platform-specific rules come here:
