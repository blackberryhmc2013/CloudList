APP_NAME = CloudListTemplate

CONFIG += qt warn_on cascades10
LIBS += -lQtXml -lbbsystem -lbbnetwork -lbbdata -lbbdevice -lQtLocationSubset -lcurl -L../libs/cloudbase-libs -lCBHelper

include(config.pri)
