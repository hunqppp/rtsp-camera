PREFIX 	= /usr/local
INCLUDES = 	-I../UsageEnvironment/include		\
			-I../groupsock/include				\
			-I../liveMedia/include				\
			-I../BasicUsageEnvironment/include

##### User configurations:
-include ../config.compiler.mk
OBJ_HLSPROXY_DIR = ../$(BUILD_DIR)/HLSProxy
##### End of variables to change

#### LIVE555 Libraries
USAGE_ENVIRONMENT_LIB 		= ../$(BUILD_DIR)/UsageEnvironment/libUsageEnvironment.$(LIB_SUFFIX)
BASIC_USAGE_ENVIRONMENT_LIB = ../$(BUILD_DIR)/BasicUsageEnvironment/libBasicUsageEnvironment.$(LIB_SUFFIX)
LIVEMEDIA_LIB 				= ../$(BUILD_DIR)/liveMedia/libliveMedia.$(LIB_SUFFIX)
GROUPSOCK_LIB 				= ../$(BUILD_DIR)/groupsock/libgroupsock.$(LIB_SUFFIX)

#### User libraries
LDLIBS		+=	-lpthread					\
				-lrt						\
				-lm							\
				-lmosquitto -lmosquittopp	\
				-lcurl						\
				-lSDL2main -lSDL2			\
				-lavcodec

#### Libraries flag compilation
LOCAL_LIBS =	$(LIVEMEDIA_LIB)	\
				$(GROUPSOCK_LIB)	\
				$(BASIC_USAGE_ENVIRONMENT_LIB)	\
				$(USAGE_ENVIRONMENT_LIB)

LIBS =			$(LOCAL_LIBS)	\
				$(LDLIBS)		\
				$(LIBS_FOR_CONSOLE_APPLICATION)

TARGETNAME 	= live555HLSProxy
OBJS 		= $(OBJ_HLSPROXY_DIR)/live555HLSProxy.$(OBJ)
TARGET 		= $(OBJ_HLSPROXY_DIR)/$(TARGETNAME)$(EXE)

all: create $(TARGET)
	$(Print) $(KPUR)█ █ █ █ DONE $(KRST)

.PHONY: create
create:
	$(Print) $(KGRN)CREATE $(OBJ_HLSPROXY_DIR) $(KRST)
	@mkdir -p $(OBJ_HLSPROXY_DIR)

$(OBJ_HLSPROXY_DIR)/%.$(OBJ): %.cpp
	$(Print)~ CXX $<
	@$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $< -o $@

$(OBJ_HLSPROXY_DIR)/%.$(OBJ): %.c
	$(Print)~ CXX $<
	@$(C_COMPILER) -c $(C_FLAGS) $< -o $@

$(TARGET): $(OBJS) $(LOCAL_LIBS)
	$(Print) $(KCYC) LINK $(KRST) $^ $(LOCAL_LIBS) $@
	@$(LINK)$@ $(CONSOLE_LINK_OPTS) $(OBJS) $(LIBS)

.PHONY: run
run:  $(TARGET)
	/usr/bin/sudo CONSOLE=1 $(TARGET)

.PHONY: clean
clean:
	$(Print) $(KRED)REMOVE $(OBJ_HLSPROXY_DIR) $(KRST);
	@rm -rf $(OBJ_HLSPROXY_DIR)


##### Any additional, platform-specific rules come here:
