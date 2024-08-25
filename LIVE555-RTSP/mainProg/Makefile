PREFIX 	 = 	/usr/local
INCLUDES = 	-I../UsageEnvironment/include		\
			-I../groupsock/include				\
			-I../liveMedia/include				\
			-I../BasicUsageEnvironment/include

##### User configurations:
-include ../config.compiler.mk
OBJ_USRCODE_DIR = ../$(BUILD_DIR)/mainProg

-include mainProg/Makefile.mk
-include libsupport/Makefile.mk
##### End of variables to change

#### LIVE555 Libraries
USAGE_ENVIRONMENT_LIB 		= ../$(BUILD_DIR)/UsageEnvironment/libUsageEnvironment.$(LIB_SUFFIX)
BASIC_USAGE_ENVIRONMENT_LIB = ../$(BUILD_DIR)/BasicUsageEnvironment/libBasicUsageEnvironment.$(LIB_SUFFIX)
LIVEMEDIA_LIB 				= ../$(BUILD_DIR)/liveMedia/libliveMedia.$(LIB_SUFFIX)
GROUPSOCK_LIB 				= ../$(BUILD_DIR)/groupsock/libgroupsock.$(LIB_SUFFIX)

#### User libraries
LDLIBS		+=	-lpthread					\
				-lrt						\
				-lm

LDLIBS		+= -lvvtkhal

#### Libraries flag compilation
LOCAL_LIBS =	$(LIVEMEDIA_LIB)	\
				$(GROUPSOCK_LIB)	\
				$(BASIC_USAGE_ENVIRONMENT_LIB)	\
				$(USAGE_ENVIRONMENT_LIB)

LIBS =			$(LOCAL_LIBS)	\
				$(LDLIBS)		\
				$(LIBS_FOR_CONSOLE_APPLICATION)

TARGETNAME 	= rtspserver
TARGET 		= $(OBJ_USRCODE_DIR)/$(TARGETNAME)$(EXE)

all: create $(TARGET)
	$(Print) $(KPUR)█ █ █ █ DONE $(KRST)

.PHONY: create
create:
	$(Print) $(KGRN)CREATE $(OBJ_USRCODE_DIR) $(KRST)
	@mkdir -p $(OBJ_USRCODE_DIR)

$(OBJ_USRCODE_DIR)/%.$(OBJ): %.cpp
	$(Print)~ CXX $<
	@$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $< -o $@

$(OBJ_USRCODE_DIR)/%.$(OBJ): %.c
	$(Print)~ CXX $<
	@$(C_COMPILER) -c $(C_FLAGS) $< -o $@

$(TARGET): $(OBJS) $(LOCAL_LIBS)
	$(Print) $(KCYC) LINK $(KRST) $^ $(LOCAL_LIBS) $@
	@$(LINK)$@ $(CONSOLE_LINK_OPTS) $(OBJS) $(LIBS)

.PHONY: run
run:  $(TARGET)
	/usr/bin/sudo CONSOLE=1 $(TARGET)

.PHONY: copy
copy:  all
	sudo cp $(TARGET) $(HOME)/FTEL/nfs-point/bin

.PHONY: clean
clean:
	$(Print) $(KRED)REMOVE $(OBJ_USRCODE_DIR) $(KRST);
	@rm -rf $(OBJ_USRCODE_DIR)


##### Any additional, platform-specific rules come here:
