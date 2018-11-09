out_dir="build"
version=${TRAVIS_TAG:-"local"}
compilerflags="-std=c++17 -Wall -Wextra"

mkdir -p ${out_dir}/release ${out_dir}/debug
clang++ ${compilerflags} -target x86_64-linux src/*.cpp -o ${out_dir}/debug/OH2RulesGenerator-${version}-linux-x64
clang++ ${compilerflags} -target i386-linux src/*.cpp -o ${out_dir}/debug/OH2RulesGenerator-${version}-linux-x86
clang++ ${compilerflags} -Ofast -target x86_64-linux src/*.cpp -o ${out_dir}/release/OH2RulesGenerator-${version}-linux-x64
clang++ ${compilerflags} -Ofast -target i386-linux src/*.cpp -o ${out_dir}/release/OH2RulesGenerator-${version}-linux-x86