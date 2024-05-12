project('CppSand', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])


sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('sdl2_image')

executable('CppSand',
           'main.cpp',
           install : true,
           dependencies: [sdl2_dep,sdl2_image_dep])
