LOCAL_PATH:= $(call my-dir)

ZKSW_PREBUILT_LIB_PATH := $(ZKSW_REAL_PREBUILT)/lib/
ZKSW_PREBUILT_INCLUDE_PATH := $(ZKSW_REAL_PREBUILT)/include/

EASYUI_COMMON_INCLUDE := $(LOCAL_PATH)/include/


include $(CLEAR_VARS)
LOCAL_MODULE	:= easyui
LOCAL_SRC_FILES	:= libeasyui.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)


#配置自己的源文件目录和源文件后缀名
MY_FILES_PATH  :=  $(LOCAL_PATH)  
  
MY_FILES_SUFFIX := %.cpp %.c  
  
# 递归遍历目录下的所有的文件 
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))  
  
# 获取相应的源文件
MY_ALL_FILES := $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*.*) )   
MY_ALL_FILES := $(MY_ALL_FILES:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)  
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(MY_ALL_FILES))   
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)  
  
# 去除字串的重复单词
define uniq =  
  $(eval seen :=)  
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))  
  ${seen}  
endef  
 
#递归遍历目录下的所有的文件
MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )  
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))  
  
# 获取相应的源文件
LOCAL_SRC_FILES  := $(MY_SRC_LIST)  
#end

#LOCAL_SRC_FILES := Main.cpp\
#				activity/MainActivity.cpp



LOCAL_CPPFLAGS := -fexceptions -fpermissive
LOCAL_LDFLAGS := -L$(ZKSW_PREBUILT_LIB_PATH)/system -lstlport

LOCAL_C_INCLUDES := $(EASYUI_COMMON_INCLUDE) \
					$(ZKSW_PREBUILT_INCLUDE_PATH)/bionic \
					$(ZKSW_PREBUILT_INCLUDE_PATH)/stlport \
					$(ZKSW_PREBUILT_INCLUDE_PATH)/gui \

LOCAL_LDLIBS := -llog
LOCAL_CFLAGS := -DUSE_ANDROID_LOG

LOCAL_SHARED_LIBRARIES := easyui
LOCAL_MODULE := zkgui

include $(BUILD_SHARED_LIBRARY)