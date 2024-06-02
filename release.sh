mkdir subprojects
meson wrap install sdl2
meson setup build --wipe
meson configure build -Dstrip=true
meson configure build -Ddebug=false
meson configure build -Dbuildtype=release
meson configure build --prefer-static
meson configure build --default-library=static
cd build
meson compile --clean
meson compile
./CppSand