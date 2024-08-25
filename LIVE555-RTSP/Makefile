##### Change the following for your environment:
-include config.compiler.mk
##### End of variables to change

#### LIVE555 Libraries
LIVEMEDIA_DIR = liveMedia
GROUPSOCK_DIR = groupsock
USAGE_ENVIRONMENT_DIR = UsageEnvironment
BASIC_USAGE_ENVIRONMENT_DIR = BasicUsageEnvironment

#### Application templates
TESTPROGS_DIR = testProgs
MEDIA_SERVER_DIR = mediaServer
PROXY_SERVER_DIR = proxyServer
HLS_PROXY_DIR = hlsProxy
USR_DIR = mainProg

#### Object files directory

all:
	$(Print) $(KBLU)CREATE $(BUILD_DIR) $(KRST); mkdir -p $(BUILD_DIR)
	$(Print) $(KYEL)BUILDING... $(KRST)
	@cd $(LIVEMEDIA_DIR) ; $(MAKE)
	@cd $(GROUPSOCK_DIR) ; $(MAKE)
	@cd $(USAGE_ENVIRONMENT_DIR) ; $(MAKE)
	@cd $(BASIC_USAGE_ENVIRONMENT_DIR) ; $(MAKE)
	$(Print) $(KGRN) BUILD DONE $(KRST)

install:
	@cd $(LIVEMEDIA_DIR) ; $(MAKE) install
	@cd $(GROUPSOCK_DIR) ; $(MAKE) install
	@cd $(USAGE_ENVIRONMENT_DIR) ; $(MAKE) install
	@cd $(BASIC_USAGE_ENVIRONMENT_DIR) ; $(MAKE) install

clean:
	$(Print) $(KRED)REMOVE $(BUILD_DIR) $(KRST);
	@rm -rf $(BUILD_DIR)
