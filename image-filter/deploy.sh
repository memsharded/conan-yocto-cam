conan create . conan/stable --profile ./armv8
rm -rf build
mkdir -p build
cd build
conan install image-filter/0.0.1@conan/stable --profile ../armv8
scp bin/* root@192.168.10.100:/bin
cd ..
