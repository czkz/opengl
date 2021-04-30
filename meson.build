project('opengl', 'cpp', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=none'],
)
# Fix for old meson where cpp_std=c++20 is not available
add_project_arguments('-std=c++20', language : 'cpp')

glfw = dependency('glfw3')
opengl = dependency('GL')

subdir('glad')

executable (
  'opengl',
  'main.cpp', 'stb_image/stb_image.cpp',
  install : false,
  dependencies : [opengl, glfw],
  include_directories: [glad_inc, include_directories('../cp')],
  link_with: [glad_lib]
)
