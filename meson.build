project('CppSand', 'cpp',
  version : '0.1',
  default_options : ['warning_level=everything', 'cpp_std=c++14'])

sdl2_dep = dependency('sdl2',required:true, static:true)

args = []

if meson.is_cross_build()
args+='-static'
endif

executable('CppSand',
            include_directories : include_directories('.'),
            sources:['main.cpp'],
            install : true,
            dependencies: [sdl2_dep],
            link_args: args)
