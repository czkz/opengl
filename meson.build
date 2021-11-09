project('opengl', 'cpp', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

glfw = dependency('glfw3')
opengl = dependency('GL')

subdir('third_party/glad')

executable (
  'opengl',
  'main.cpp', 'third_party/stb_image/stb_image.cpp',
  install : true,
  dependencies : [opengl, glfw],
  include_directories: [
    glad_inc, include_directories('../cp'),
    include_directories('window/'),
  ],
  link_with: [glad_lib],
)
