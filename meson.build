project('opengl', 'cpp', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=none'])
# Fix for old meson where cpp_std=c++20 is not available
add_project_arguments('-std=c++20', language : 'cpp')

glfw = dependency('glfw3')
opengl = dependency('GL')
assimp = dependency('assimp')

subdir('third_party/glad')

executable (
  'opengl',
  'main.cpp', 'third_party/stb_image/stb_image.cpp', 'utils/model_loader.cpp',
  install : true,
  dependencies : [opengl, glfw, assimp],
  include_directories: [
    glad_inc, include_directories('../cp'),
    include_directories('world_scripts/'),
    include_directories('gl_objects/'),
    include_directories('logic/'),
    include_directories('third_party/'),
    include_directories('utils/'),
    include_directories('window/'),
  ],
  link_with: [glad_lib],
  cpp_pch: 'pch/pch.h'
)
