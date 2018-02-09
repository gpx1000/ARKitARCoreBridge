cmake_minimum_required( VERSION 3.9 )

include (BundleUtilities)

set(CMAKE_Swift_LANGUAGE_VERSION 4.0)
set(CMAKE_XCODE_ATTRIBUTE_SWIFT_VERSION "4.0")
enable_language(Swift)

set(CMAKE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER "SwiftBridgingHeader.h")
set(CMAKE_ATTRIBUTE_SWIFT_OBJC_INTERFACE_HEADER_NAME "SwiftInterfaceHeader.h")

set(DEVELOPMENT_PROJECT_NAME "ARKitApp")
set(DEVELOPMENT_TEAM_ID "XXXXX") # note that this can be found in team account settings on your http://developer.apple.com

project(${DEVELOPMENT_PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out/library)

set(APP_NAME ${DEVELOPMENT_PROJECT_NAME})
set(APP_BUNDLE_IDENTIFIER "com.gpxblog.arkit")
set(PRODUCT_NAME ${APP_NAME})
set(EXECUTABLE_NAME ${APP_NAME})
set(CODE_SIGN_IDENTITY "iPhone Developer")
set(DEPLOYMENT_TARGET 11.2)
set(DEVICE_FAMILY "1") # set to "1" for iPhone, "2" for iPad, "1,2" for both

set(TEST_NAME "tests")
set(TEST_BUNDLE_IDENTIFIER "com.gpxblog.tests")

set(LOGIC_ONLY_TESTS 0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_STANDARD_REQUIRED)

set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_MACOSX_BUNDLE YES)
set(MACOSX_BUNDLE_EXECUTABLE_NAME ${APP_NAME})
set(MACOSX_BUNDLE_INFO_STRING ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_GUI_IDENTIFIER ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_LONG_VERSION_STRING ${MyLongVerStr})
set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${MyShortVerStr})
set(MACOSX_BUNDLE_BUNDLE_VERSION ${MyShortVerStr})
set(MACOSX_BUNDLE_COPYRIGHT "Copyright")
set(MACOSX_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET})

set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED NO)
set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID} CACHE STRING "")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)

set(CMAKE_XCODE_GENERATE_SCHEME YES)

add_subdirectory(${CMAKE_SOURCE_DIR}/../Common ${CMAKE_CURRENT_BINARY_DIR}/Common)

set( APP_HEADER_FILES
        src/AppDelegate.h
        src/ViewController.h
    )
set( APP_SOURCE_FILES
        src/AppDelegate.m
        src/ViewController.m
        src/main.m
    )
set( RESOURCES
        "LaunchScreen.storyboard"
        "Assets.xcassets"
    )

add_executable( ${APP_NAME} MACOSX_BUNDLE
    ${APP_HEADER_FILES}
    ${APP_SOURCE_FILES}
    ${RESOURCES}
)

# Copy the framework into the app bundle
add_custom_command( TARGET ${APP_NAME} POST_BUILD COMMAND
            /bin/sh -c
            \"COMMAND_DONE=0 \;
            if [ "\${DEPLOYMENT_LOCATION}" == "YES" ] \; then
                FRAMEWORKS_DIR=\${BUILT_PRODUCTS_DIR}/${APP_NAME}.app/Frameworks \;
            else
                FRAMEWORKS_DIR=${PROJECT_BINARY_DIR}/\${CONFIGURATION}\${EFFECTIVE_PLATFORM_NAME}/${APP_NAME}.app/Frameworks \;
            fi \;
            if ${CMAKE_COMMAND} -E make_directory
                \\$${FRAMEWORKS_DIR}
                \&\>/dev/null \; then
                COMMAND_DONE=1 \;
            fi \;
            if [ \\$$COMMAND_DONE -eq 0 ] \; then
                echo Failed to create Frameworks directory in app bundle \;
                exit 1 \;
            fi \;
            if ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_BINARY_DIR}/Common/\${CONFIGURATION}\${EFFECTIVE_PLATFORM_NAME}/
                \\$${FRAMEWORKS_DIR}
                \&\>/dev/null \; then
                COMMAND_DONE=1 \;
            fi \;
            if [ \\$$COMMAND_DONE -eq 0 ] \; then
                echo Failed to copy the framework into the app bundle \;
                exit 1 \;
            fi \;
            \"
)

find_library(UIKIT UIKit)
find_library(FOUNDATION Foundation)
find_library(COREGRAPHICS CoreGraphics)
find_library(ARKIT ARKit)
find_library(GLKIT GLKit)
find_library(SCENEKIT SceneKit)

target_link_libraries( ${APP_NAME}
            ${UIKIT}
            ${FOUNDATION}
            ${COREGRAPHICS}
            ${ARKIT}
            ${GLKIT}
            ${SCENEKIT}
)

set_target_properties( ${APP_NAME}
    PROPERTIES
    RESOURCE "${RESOURCES}"
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/plist.in"
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
    XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/Prefix.pch"
    XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${CODE_SIGN_IDENTITY}"
    XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${DEVELOPMENT_TEAM_ID}"
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "${DEVICE_FAMILY}"
    XCODE_ATTRIBUTE_IPHONEOS_DEVELOPMENT_TARGET 11.2
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC NO
    XCODE_ATTRIBUTE_INSTALL_PATH "${LOCAL_APPS_DIR}"
    # the below setups the app to use the custom framework
    XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/Frameworks"
    XCODE_ATTRIBUTE_FRAMEWORK_SEARCH_PATHS "${CMAKE_CURRENT_BINARY_DIR}/Common/\${CONFIGURATION}\${EFFECTIVE_PLATFORM_NAME}"
    XCODE_ATTRIBUTE_OTHER_LDFLAGS "${XCODE_ATTRIBUTE_OTHER_LDFLAGS} -framework Common"
)
add_dependencies( ${APP_NAME} Common )