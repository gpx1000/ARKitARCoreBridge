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

#ifndef C_ARCORE_HELLOE_AR_UTIL_H_
#define C_ARCORE_HELLOE_AR_UTIL_H_

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#include <android/log.h>
#include <jni.h>
#include "arcore_c_api.h"
#else
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
class ArAnchor;
class ArSession;
class ArPose;
class AAssetManager;
#define GL_TEXTURE_EXTERNAL_OES GL_TEXTURE_2D
#define AR_SUCCESS 0
#endif
#include <errno.h>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "glm.h"
#include "Logger.h"

#ifndef CHECK
#define CHECK(condition)                                                   \
  if (!(condition)) {                                                      \
    ERROR("*** CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
    abort();                                                               \
  }
#endif  // CHECK

namespace hello_ar {

// Utilities for C hello AR project.
namespace util {

// Provides a scoped allocated instance of Anchor.
// Can be treated as an ArAnchor*.
class ScopedArPose {
 public:
    explicit ScopedArPose(const ArSession* session);
    ~ScopedArPose();
    ArPose* GetArPose();
  // Delete copy constructors.
  ScopedArPose(const ScopedArPose&) = delete;
  void operator=(const ScopedArPose&) = delete;

 private:
  ArPose* pose_;
};

// Check GL error, and abort if an error is encountered.
//
// @param operation, the name of the GL function call.
void CheckGlError(const char* operation);

// Create a shader program ID.
//
// @param vertex_source, the vertex shader source.
// @param fragment_source, the fragment shader source.
// @return
GLuint CreateProgram(const char* vertex_source, const char* fragment_source);

// Load png file from assets folder and then assign it to the OpenGL target.
// This method must be called from the renderer thread since it will result in
// OpenGL calls to assign the image to the texture target.
//
// @param target, openGL texture target to load the image into.
// @param path, path to the file, relative to the assets folder.
// @return true if png is loaded correctly, otherwise false.
bool LoadPngFromAssetManager(int target, const std::string& path);

// Load obj file from assets folder from the app.
//
// @param mgr, AAssetManager pointer.
// @param file_name, name of the obj file.
// @param out_vertices, output vertices.
// @param out_normals, output normals.
// @param out_uv, output texture UV coordinates.
// @param out_indices, output triangle indices.
// @return true if obj is loaded correctly, otherwise false.
bool LoadObjFile(AAssetManager* mgr, const std::string& file_name,
                 std::vector<GLfloat>* out_vertices,
                 std::vector<GLfloat>* out_normals,
                 std::vector<GLfloat>* out_uv,
                 std::vector<GLushort>* out_indices);

// Format and output the matrix to logcat file.
// Note that this function output matrix in row major.
void Log4x4Matrix(float raw_matrix[16]);

// Get transformation matrix from ArAnchor.
void GetTransformMatrixFromAnchor(ArSession* ar_session,
                                  const ArAnchor* ar_anchor,
                                  glm::mat4* out_model_mat);
}  // namespace util
}  // namespace hello_ar

#endif  // C_ARCORE_HELLOE_AR_UTIL_H_
