set(COMMON_SOURCES
        background_renderer.cpp
        hello_ar_application.cpp
        obj_renderer.cpp
        plane_renderer.cpp
        point_cloud_renderer.cpp
        util.cpp
)
set(EXPORTED_HEADERS
    background_renderer.h
    glm.h
    hello_ar_application.h
    obj_renderer.h
    plane_renderer.h
    point_cloud_renderer.h
    util.h
)

add_library(Common STATIC ${COMMON_SOURCES})

target_include_directories( Common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

if(ANDROID)
find_library( log-lib log )
find_library( EGL-lib EGL )
find_library( GLESv3-lib GLESv3 )
find_library( android-lib android )

target_include_directories(Common PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/../ARCore/Include
                            ${ANDROID_NDK}/sources/third_party/vulkan/src/libs/glm
)
target_link_libraries ( Common
                        ${CMAKE_CURRENT_SOURCE_DIR}/../ARCore/jni/${ANDROID_ABI}/libarcore_sdk.so
                        ${log-lib}
                        ${EGL-lib}
                        ${GLESv3-lib}
                        ${android-lib}
                    )
else()
# Build custom framework.
find_library(QUARTZCORE QuartzCore)
find_library(OPENGLES OpenGLES)
find_library(UIKit UIKit)
find_library(FOUNDATION Foundation)
find_library(COREGRAPHICS CoreGraphics)
find_library(ARKIT ARKit)
target_link_libraries( Common
                        ${QUARTZCORE}
                        ${OPENGLES}
                        ${UIKIT}
                        ${FOUNDATION}
                        ${COREGRAPHICS}
                        ${ARKIT}
                    )

set_target_properties(Common PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION A
    MACOSX_FRAMEWORK_IDENTIFIER com.gpxblog.ARCommonFramework
    MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/framework.plist.in
    MACOSX_FRAMEWORK_BUNDLE_VERSION 1.0.0
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING 1.0.0
    VERSION 1.0.0
    SOVERSION 1.0.0
    PUBLIC_HEADER ${EXPORTED_HEADERS}
    XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET}
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${CODE_SIGN_IDENTITY}
    XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY ${DEVICE_FAMILY}
    XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
)

add_custom_command(
    TARGET Common POST_BUILD COMMAND
    /bin/bash -c "${CMAKE_CURRENT_LIST_DIR}/install_name.sh \${BUILT_PRODUCTS_DIR}/\${PRODUCT_NAME}.framework/\${PRODUCT_NAME}"
)

add_custom_command(
    TARGET Common POST_BUILD COMMAND
    COMMAND install_name_tool -id \"@rpath/\${PRODUCT_NAME}.framework/\${PRODUCT_NAME}\"
        \${BUILT_PRODUCTS_DIR}/\${PRODUCT_NAME}.framework/\${PRODUCT_NAME}
)

endif()