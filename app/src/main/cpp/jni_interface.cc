/*
 * Copyright 2017 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <jni.h>

#include "hello_ar_application.h"

extern "C" {

namespace {
// maintain a reference to the JVM so we can use it later.
static JavaVM *g_vm = nullptr;

inline jlong jptr(hello_ar::HelloArApplication *native_hello_ar_application) {
  return reinterpret_cast<intptr_t>(native_hello_ar_application);
}

inline hello_ar::HelloArApplication *native(jlong ptr) {
  return reinterpret_cast<hello_ar::HelloArApplication *>(ptr);
}

}  // namespace

jint JNI_OnLoad(JavaVM *vm, void *) {
  g_vm = vm;
  return JNI_VERSION_1_6;
}

JNIEXPORT jlong JNICALL
Java_com_gpxblog_arkitarcorebridge_JniInterface_createNativeApplication
(JNIEnv *env, jclass, jobject j_asset_manager, jobject context) {
  AAssetManager *asset_manager = AAssetManager_fromJava(env, j_asset_manager);
  return jptr(new hello_ar::HelloArApplication(asset_manager, env, context));
}

JNIEXPORT void JNICALL Java_com_gpxblog_arkitarcorebridge_JniInterface_destroyNativeApplication
(JNIEnv *, jclass, jlong native_application) {
  delete native(native_application);
}

JNIEXPORT void JNICALL Java_com_gpxblog_arkitarcorebridge_JniInterface_onPause
(JNIEnv *, jclass, jlong native_application) {
  native(native_application)->OnPause();
}

JNIEXPORT void JNICALL Java_com_gpxblog_arkitarcorebridge_JniInterface_onResume
(JNIEnv *, jclass, jlong native_application) {
  native(native_application)->OnResume();
}

JNIEXPORT void JNICALL Java_com_gpxblog_arkitarcorebridge_JniInterface_onGlSurfaceCreated
(JNIEnv *, jclass, jlong native_application) {
  native(native_application)->OnSurfaceCreated();
}

JNIEXPORT void JNICALL Java_com_gpxblog_arkitarcorebridge_JniInterface_onDisplayGeometryChanged
(JNIEnv *, jobject, jlong native_application, int display_rotation, int width,
 int height) {
  native(native_application)
      ->OnDisplayGeometryChanged(display_rotation, width, height);
}

JNIEXPORT void JNICALL Java_com_gpxblog_arkitarcorebridge_JniInterface_onGlSurfaceDrawFrame
(JNIEnv *, jclass, jlong native_application) {
  native(native_application)->OnDrawFrame();
}

JNIEXPORT void JNICALL Java_com_gpxblog_arkitarcorebridge_JniInterface_onTouched
(JNIEnv *, jclass, jlong native_application, jfloat x, jfloat y) {
  native(native_application)->OnTouched(x, y);
}

JNIEXPORT jboolean JNICALL Java_com_gpxblog_arkitarcorebridge_JniInterface_hasDetectedPlanes
(JNIEnv *, jclass, jlong native_application) {
  return static_cast<jboolean>(
      native(native_application)->HasDetectedPlanes() ? JNI_TRUE : JNI_FALSE);
}

JNIEnv *GetJniEnv() {
  JNIEnv *env;
  jint result = g_vm->AttachCurrentThread(&env, nullptr);
  return result == JNI_OK ? env : nullptr;
}

jclass FindClass(const char *classname) {
  JNIEnv *env = GetJniEnv();
  return env->FindClass(classname);
}

}  // extern "C"
