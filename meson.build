project('opengl', 'cpp', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

glfw = dependency('glfw3')
opengl = dependency('GL')

subdir('glad')

executable (
  'opengl',
  'main.cpp', 'stb_image.cpp',
  install : false,
  dependencies : [opengl, glfw],
  include_directories: [glad_inc],
  link_with: [glad_lib]
)
