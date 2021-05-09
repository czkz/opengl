glad_lib = static_library(
  'glad',
  'glad.c',
  include_directories: glad_inc,
  dependencies: [dependency('gl'), meson.get_compiler('c').find_library('dl')]
)

