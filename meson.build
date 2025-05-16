# para iniciar arquivo
# remove o dir build se tiver
#
#   rm -rf build
#
# Inicia o build novamente
#
#   meson setup build 
#
# Compilar
#
#   ninja -C build
#
# OU
#
#   meson compile -C build

project(
	'T1 labredes', 
	'cpp', 
 	default_options : ['cpp_std=c++20']
)

incdir = include_directories('src/include')

src = [
    'src/main.cpp',
    'src/model/Utils.cpp',
    'src/Protocol.cpp',
    'src/model/Client.cpp',
    'src/model/Packet.cpp',
    'src/model/TokenManager.cpp',
    'src/view/Console.cpp',
    'src/controller/LogStreamBuf.cpp',
]

deps = [

]

executable('tf', src, dependencies: deps, include_directories: incdir)