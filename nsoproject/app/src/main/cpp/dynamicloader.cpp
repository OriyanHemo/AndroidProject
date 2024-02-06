#include <jni.h>
#include <iostream>
#include <dlfcn.h>
#include <android/log.h>
#include <string>
#include <LIEF/LIEF.hpp>
static void* libHelloHandle = nullptr;
static void* libGadgetHandle = nullptr;

extern "C" {
/*
*This function load library into the process
*
*Input: JNIEnv *env, jobject thiz, jstring libPath
*Output: void
*/
JNIEXPORT void JNICALL Java_com_example_nso_1project_MainActivity_loadLib(JNIEnv *env, jobject thiz, jstring libPath) {
    const char *path = env->GetStringUTFChars(libPath, nullptr);
    const char *libHelloPath("/data/user/0/com.example.nso_project/app_lib/libhello.so");//this path lead to the libhello.so library that is in the device
    libHelloHandle = dlopen(libHelloPath, RTLD_NOW);//using dlopen function as needed to open libhello.so library
    if (libHelloHandle == nullptr) {//checks if dlopen function did not do correctly
        const char* error = dlerror();
        __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "Error loading library libhello.so :%s", error);
    } 
    else {
        __android_log_print(ANDROID_LOG_INFO, "TRACKERS", "Success loading library libhello.so.");
    }
    env->ReleaseStringUTFChars(libPath, path);
}
/*
*This function unload library from the process
*
*Input: JNIEnv *env, jobject thiz, jstring libPath
*Output: void
*/
JNIEXPORT void JNICALL Java_com_example_nso_1project_MainActivity_unloadLib(JNIEnv *env, jobject thiz, jstring libPath) {
    const char *path = env->GetStringUTFChars(libPath, nullptr);
    if (libHelloHandle != nullptr) {//checks if dlopen function was executed and loaded the libhello.so library
        dlclose(libHelloHandle);//using dlclose function as needed to close/unload libhello.so library
        libHelloHandle = nullptr;
        __android_log_print(ANDROID_LOG_INFO, "TRACKERS", "Success unloading library libhello.so.");
    }
    else {
        __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "Error unloading library libhello.so(the library wasn't loaded ).");
    }
    env->ReleaseStringUTFChars(libPath, path);
}
/*
*This function load library to the process and patch it also
*
*Input: JNIEnv *env, jobject thiz, jstring libPath
*Output: void
*/
JNIEXPORT void JNICALL Java_com_example_nso_1project_Utils_patchLib(JNIEnv *env, jobject thiz, jstring libPath) {
    const char *path = env->GetStringUTFChars(libPath, nullptr);

    std::string libHelloPath("/data/user/0/com.example.nso_project/app_lib/libhello.so");//this path lead to the libhello.so library that is in the device

    std::unique_ptr<LIEF::ELF::Binary> binary = std::unique_ptr<LIEF::ELF::Binary>{LIEF::ELF::Parser::parse(libHelloPath)};
    if (binary == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "Failed to read binary libhello.so library");
        env->ReleaseStringUTFChars(libPath, path);
        return;
    }
    else { //as a checking if the libhello.so library has loaded and binary readed
      binary->add_library("libgadget.so");//adding the NEEDED dependency into its(libhello.so library that is in the device as needed) Dynamic section. 
      binary->write(libHelloPath);


      const char *libGadgetPath("/data/user/0/com.example.nso_project/app_lib/libgadget.so");//this path lead to the libgadget.so library that is in the device
      libGadgetHandle = dlopen(libGadgetPath, RTLD_NOW);

      if (libGadgetHandle == nullptr) {
          const char* error = dlerror();
          __android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "Error loading library libgadget.so: %s", error);
      } else {
          __android_log_print(ANDROID_LOG_INFO, "TRACKERS", "Success loading library libgadget.so.");
      }
    }
    env->ReleaseStringUTFChars(libPath, path);


}

} // extern "C"
