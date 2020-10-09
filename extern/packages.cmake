if(MESHOPT_BUILD_GLTFPACK)
  hunter_add_package(cgltf)
  find_package(cgltf CONFIG REQUIRED)

  hunter_add_package(basis_universal)
  find_package(basis_universal CONFIG REQUIRED)
endif()

if(MESHOPT_BUILD_DEMO)
  hunter_add_package(vurtun-lib)
  find_package(vurtun CONFIG REQUIRED)
endif()

if(MESHOPT_BUILD_DEMO OR MESHOPT_BUILD_GLTFPACK)
  hunter_add_package(fast_obj)
  find_package(fast_obj CONFIG REQUIRED)
endif()
