meson setup --cross-file x86_64-w64-mingw32.txt build-mingw --wipe
meson compile -C build-mingw --clean
meson configure build-mingw  -Dstrip=true
meson configure build-mingw  -Ddebug=false
meson configure build-mingw  -Dbuildtype=release
meson configure build-mingw  --prefer-static
meson configure build --default-library=static
meson configure build-mingw -Dwarning_level=0
meson compile -C build-mingw