include("C:/Users/Ritul Kumari/Documents/cctech/sketcher1/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/ProjectQT-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE C:/Users/Ritul Kumari/Documents/cctech/sketcher1/build/ProjectQT.exe
    GENERATE_QT_CONF
)
