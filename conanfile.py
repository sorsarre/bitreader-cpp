from conans import ConanFile, CMake, tools


class BitreadercppConan(ConanFile):
    name = "bitreader-cpp"
    version = "0.1"
    license = "BSD 3-Clause"
    author = "Andrey Kaydalov <sorsarre@gmail.com>"
    url = "https://github.com/sorsarre/bitreader-cpp"
    description = "Bitstream reader implementation in C++"
    topics = ("bitstream", "bitreader", "binary")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def source(self):
        self.run("git clone https://github.com/sorsarre/bitreader-cpp")
        tools.replace_in_file("bitreader-cpp/CMakeLists.txt", "project(bitreader)",
                              '''project(bitreader)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="bitreader-cpp")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.hpp", dst="include", src="bitreader-cpp/bitreader/include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["bitreadercpp"]
