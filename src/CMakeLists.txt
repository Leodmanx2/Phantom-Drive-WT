add_executable(pd-renderer-test
  Actor.cpp
  Application.cpp
  Geometry.cpp
  Light.cpp
  PMDL.cpp
  PSCN.cpp
  RenderComponent.cpp
  Renderer.cpp
  ResourceCache.cpp
  Scene.cpp
  ShaderProgram.cpp
  SpatialComponent.cpp
)

install(TARGETS pd-renderer-test DESTINATION bin)
target_link_libraries(pd-renderer-test PUBLIC glfw3 glbinding glbinding-aux globjects)

target_compile_definitions(pd-renderer-test PUBLIC gsl_CONFIG_CONTRACT_VIOLATION_THROWS)
if(NOT ENABLE_TESTS)
	target_compile_definitions(pd-renderer-test PUBLIC DOCTEST_CONFIG_DISABLE)
endif()
