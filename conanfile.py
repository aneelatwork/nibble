from io import StringIO
import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class gtestRecipe(ConanFile):
    name = "nibble"
    version = "00.00.01"

    # Optional metadata
    license = "BSD 2-Clause"
    author = "RaKs"
    url = None
    description = "nibble data type"
    topics = ("Type", "Data", "Format")

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build_requirements(self):
        self.test_requires("gtest/1.17.0")

    def layout(self):
        cmake_layout(self)
        build_dir_io = StringIO()
        self.run("bd", stdout=build_dir_io)
        build_dir = build_dir_io.getvalue().strip()
        self.folders.build = build_dir
        self.folders.generators = os.path.join(build_dir, "generators")
        self.output.info(f"Self Build Folder: {self.folders.build}")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator="Ninja")
        tc.variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        tc.user_presets_path = os.path.join(
            self.build_folder, "CMakeUserPresets.json"
        )
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake.test()
            # test_folder = os.path.join("test")
            # self.run(os.path.join(test_folder, "nibble_test"))

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["nibble"]
        self.cpp_info.components["libgtest"].set_property(
            "cmake_target_name", "Nibble::nibble"
        )
