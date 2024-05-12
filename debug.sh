meson configure build -Dstrip=false
meson configure build -Ddebug=true
meson configure build -Dbuildtype=debug
cd build
meson compile --clean
meson compile
gdb CppSand