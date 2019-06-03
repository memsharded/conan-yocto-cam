from conans import ConanFile, CMake


class ImageFilter(ConanFile):
    name = "image-filter"
    version = "0.0.1"
    settings = "os", "compiler", "arch", "build_type"
    requires = "cppzmq/4.3.0@bincrafters/stable", "fmt/5.3.0@bincrafters/stable", "opencv/3.4.5@conan/stable"
    generators = "cmake"
    exports_sources = "CMakeLists.txt", "app*", "lib/*", "res/*"  
    default_options = {
        "opencv:openexr": False,
        "opencv:png": False,
        "opencv:tiff": False,
        "opencv:jasper": False
    }


    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*", src="bin", dst="bin")
        self.copy("*", src="res", dst="res")

    def deploy(self):
        self.copy("*", src="bin", dst="bin")
        self.copy("*", src="res", dst="res")

    def package_id(self):
        del self.info.settings.compiler
        del self.info.settings.build_type

