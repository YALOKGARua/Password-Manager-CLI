cmake_minimum_required(VERSION 3.15)
project(PasswordManagerCPP VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build libraries as shared" OFF)
option(ENABLE_INSTALL "Enable install rules" ON)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(spdlog REQUIRED)
if(UNIX)
  find_package(Curses REQUIRED)
endif()

add_library(crypto src/Crypto.cpp)
target_include_directories(crypto PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(crypto PUBLIC OpenSSL::Crypto)

add_library(storage src/Storage.cpp)
target_link_libraries(storage PUBLIC crypto nlohmann_json::nlohmann_json)

add_library(pwdmgr_lib src/PasswordManager.cpp)
target_link_libraries(pwdmgr_lib PUBLIC storage)

add_library(logger src/Logger.cpp)
target_link_libraries(logger PUBLIC spdlog::spdlog)

if(UNIX)
  add_library(plugin src/PluginManager.cpp)
  target_link_libraries(plugin PUBLIC pwdmgr_lib)
  add_library(ui src/UI.cpp)
  target_link_libraries(ui PUBLIC pwdmgr_lib ${CURSES_LIBRARIES})
endif()

add_library(pgen src/PasswordGenerator.cpp)
target_link_libraries(pgen PUBLIC pwdmgr_lib)

add_library(sync src/SyncDropbox.cpp src/SyncGoogleDrive.cpp)
target_link_libraries(sync PUBLIC OpenSSL::Crypto nlohmann_json::nlohmann_json pwdmgr_lib)

add_executable(pwdmgr src/main.cpp)
target_link_libraries(pwdmgr PRIVATE pwdmgr_lib)

if(ENABLE_INSTALL)
  install(TARGETS crypto storage pwdmgr_lib logger plugin pgen sync pwdmgr
    EXPORT PasswordManagerTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
  )
  install(DIRECTORY include/ DESTINATION include)
  install(EXPORT PasswordManagerTargets
    FILE PasswordManagerConfig.cmake
    NAMESPACE PasswordManager::
    DESTINATION lib/cmake/PasswordManager
  )
endif()

include(CTest)
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(--coverage)
  add_link_options(--coverage)
endif()

include(CPack) 