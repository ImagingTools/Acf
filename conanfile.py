#
# For using sparse checkout from the local repo set
# git config --global uploadpack.allowFilter true
#

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.env import Environment
from conan.tools.files import collect_libs, update_conandata, move_folder_contents, copy
from conan.tools.scm import Git
from conan.errors import ConanInvalidConfiguration, ConanException

import os
from pathlib import Path
import json


class AcfConan(ConanFile):
    name = "acf"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "qt_package": ["system", "conan"],
        "qt_version": "ANY",
        "BUILD_TESTING": [True, False]
    }

    default_options = {
        "qt_package": "conan",
        "qt_version": None,
        "BUILD_TESTING": False
    }

    description = "Application Component Framework"
    url = "https://github.com/ImagingTools/Acf"
    license = "LicenseRef-ACF-Commercial", "LGPL-2.1-or-later"
    author = "Witold Gantzke & Kirill Lepskiy"
    topics = ("qt", "component-framework", "model-observer", "undo-redo", "serialization", "gui", "imaging")
    generators = "CMakeDeps"
    python_requires = "conantools/0.2.0@gmg/stable"

    @property
    def _gmgtools(self):
        return self.python_requires["conantools"].module

    def set_version(self):
        if self.version is not None:
            self.output.warning(f'Version overriden')
            return self.version
        git = Git(self)
        try:
            self.version = 'git.' + git.get_commit()[:8]
            if git.is_dirty():
                self.version += '-dirty'
            self.output.info(f'Detected version {self.version}')
        except ConanException:
            self.output.error('Unable to detect version')
            self.version = 'unknown'

    def export(self):
        git = Git(self)
        url, commit = git.get_url_and_commit()
        subfolder = self.recipe_path.relative_to(Path(git.get_repo_root())).as_posix()
        # we store the current url and commit in conandata.yml
        update_conandata(self, {"sources": {"commit": commit, "url": url, "subfolder": subfolder}})

    def validate(self):
        if self.settings.arch == "x86":
            raise ConanInvalidConfiguration("Not supported architecture")

    def build_requirements(self):
        if self.settings.os == "Linux":
            self.build_requires("patchelf/0.13")

    def requirements(self):
        if self.options.qt_package == "conan":
            self.requires("qt/[>=6.8]@gmg/system")

    def configure(self):
        if self.options.qt_package == "system":
            # Make system qt environment version part of the package id
            self.options.qt_version = self._gmgtools.qmake_query("QT_VERSION")
            self.output.info(f"Auto-detected Qt version: {self.options.qt_version}")
        else:
            del self.options.qt_version

    def layout(self):
        cmake_layout(self, generator='Ninja', build_folder='out')
        for layoutData in (self.layouts.source, self.layouts.package):
            layoutData.buildenv_info.define_path("ACFDIR", ".")
            layoutData.buildenv_info.define_path("ACFCONFIGDIR", "Config")

        self.layouts.build.buildenv_info.define_path("ACFDIR_BUILD", "Acf")

    def source(self):
        git = Git(self)
        sources = self.conan_data["sources"]
        url = sources["url"]
        commit = sources["commit"]
        subfolder = sources["subfolder"]
        git.run('init')
        git.run(f'remote add origin "{url}"')
        git.run(f'fetch --depth=1 --filter=tree:0 origin "{commit}"')
        git.run(f'sparse-checkout set "{subfolder}"')
        git.checkout(commit)

        # move_folder_contents works only one level down, iterate through levels
        for sub in os.path.split(subfolder):
            self.output.info(f'moving {sub} -> {self.source_folder}')
            move_folder_contents(self, sub, self.source_folder)

    def _get_qt_version(self):
        if self.options.qt_package == "system":
            return self.options.qt_version
        else:
            return str(self.dependencies["qt"].ref.version)

    def _write_env_to_presets(self, env):
        presetsPath = self.generators_path / 'CMakePresets.json'
        with open(presetsPath) as f:
            data = json.load(f)
        for conf in data['configurePresets']:
            if conf['name'].endswith(str(self.settings.build_type).lower()):
                if 'environment' not in conf:
                    conf['environment'] = {}
                for var, val in env.items():
                    conf['environment'][var] = val
        with open(presetsPath, 'w') as f:
            json.dump(data, f, indent=2)

    def _update_version(self):
        script_name = "GenerateVersion.bat" if self.settings.os == "Windows" else "GenerateVersion.sh"
        script_path = os.path.join(self.source_folder, "Build", "Git", script_name)
        backup_dir = os.path.join(self.build_folder, "xtrsvn-backups")

        self.run(f'"{script_path}" "{backup_dir}"', cwd=self.source_folder)

    def generate(self):
        qt_major = self._get_qt_version().split(".")[0]

        if self.options.qt_package == "conan":
            qtDir = str(self.dependencies["qt"].cpp_info.bindirs[0])
        else:
            qtDir = self._gmgtools.detect_qtdir(self)
        self.output.info(f"QTDIR: {qtDir}")

        tc = CMakeToolchain(self, generator='Ninja')
        tc.user_presets_path = 'Build/CMake'
        tc.variables["BUILD_TESTING"] = self.options.BUILD_TESTING
        tc.variables["QT_VERSION_MAJOR"] = qt_major
        tc.variables["CMAKE_CXX_STANDARD"] = 17
        tc.variables['BUILDDIR'] = self.build_path.as_posix()
        # fvisibility hidden
        tc.variables['CMAKE_POLICY_DEFAULT_CMP0063'] = 'NEW'
        tc.variables['CMAKE_VISIBILITY_INLINES_HIDDEN'] = True
        tc.variables['CMAKE_CXX_VISIBILITY_PRESET'] = 'hidden'

        tc.generate()

        env = Environment()
        env.define("ACFDIR_BUILD", os.path.join(self.build_folder, "Acf"))
        env.define("QTDIR", qtDir)
        env = env.vars(self, scope="build")
        env.save_script("acfenv")

        self._write_env_to_presets({
            var: val for var, val in env.items()
            if var.startswith('ACF')})

        self._update_version()

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder='Build/CMake')
        cmake.build()

    def package_id(self):
        # qt may break binary compatibility with any change in the version
        # because of patches or repackaging
        self.info.requires['qt'].full_version_mode()

    def _build_folder_suffix(self, binary=True):
        # folder naming in acf and imt
        if self.settings.build_type == 'Release':
            build_type = 'Release'
        else:
            build_type = 'Debug'

        qt_major = self._get_qt_version().split(".")[0]

        if self.settings.os == 'Windows':
            if self.settings.compiler == 'msvc':
                compiler = 'VC' + {
                    "192": "16",
                    "193": "17", 
                    "194": "17",
                }[str(self.settings.compiler.version)]
            elif self.settings.compiler == 'Visual Studio':
                compiler = 'VC' + str(self.settings.compiler.version)
            else:
                compiler = 'Clang'
        elif self.settings.os == 'Linux':
            if self.settings.compiler == 'gcc':
                compiler = 'GCCLinux'
            else:
                compiler = 'ClangLinux'
        else:
            compiler = 'ClangOSX'

        if self.settings.arch == 'x86_64':
            arch = 'x64'
        elif self.settings.arch == 'armv8':
            arch = 'arm64'
        else:
            arch = 'x86'

        if binary:
            return f'{build_type}_Qt{qt_major}_{compiler}_{arch}'
        else:
            return f'Qt{qt_major}_{compiler}_{arch}'

    def _include_folder_suffix(self):
        return self._build_folder_suffix(binary=False)

    def package(self):
        def cp(patterns, path):
            for p in patterns:
                copy(self, p, src=os.path.join(self.source_folder, path), dst=os.path.join(self.package_folder, path))

        # Acf
        cp(["*.*"], "Config")
        cp(["*.*"], "Partitura")
        cp(["*.h", "*.cpp"], "Include")
        cp(["*.h"], "Impl")
        cp(["*.h"], "AuxInclude")
        cp(["*.lib", "*.pdb", "*.a"], "Lib")
        copy(self, "*.pdb",
             src=os.path.join(self.source_folder, "AuxInclude"),
             dst=os.path.join(self.package_folder, "Lib", self._build_folder_suffix()),
             keep_path=False)
        cp(["*.exe", "*.dll", "*.pdb", "*.arp"], "Bin")
        if self.settings.os != 'Windows':
            cp(["*"], os.path.join("Bin", self._build_folder_suffix()))

        from shlex import join
        bin_subdir = self.package_path / "Bin" / self._build_folder_suffix()
        self.output.info(f"Processing: {bin_subdir}")
        assert os.path.isdir(bin_subdir)
        if self.settings.os == 'Linux':
            # installation is done not with cmake, remove rpath manually
            for file in bin_subdir.iterdir():
                self.output.info(f"Removing rpath from {file.name}")
                self.run(join(["patchelf", "--remove-rpath", str(file)]))

    def _collect_libs(self):
        prefix = ''
        if (self.package_path / self.folders.build).is_dir():
            self.output.info("Assuming editable mode")
            prefix = self.folders.build

        libs = []
        for libdir in self.cpp_info.libdirs:
            absoluteLibdir = self.package_path / prefix / libdir
            if not absoluteLibdir.is_dir():
                raise Exception(f"libdir does not exists {absoluteLibdir}")
            libs += collect_libs(self, os.path.join(prefix, libdir))
        return libs

    def package_info(self):
        # Acf include dirs: "Impl", "Include", "AuxInclude/XXX", "AuxInclude/XXX/GeneratedFiles"
        acf_auxinc_dir = os.path.join("Acf/AuxInclude", self._include_folder_suffix())
        acf_dirs = ["Impl", "Include"]
        acf_auxinc_dirs = [acf_auxinc_dir, os.path.join(acf_auxinc_dir, "GeneratedFiles")]

        self.cpp_info.bindirs = []
        self.cpp_info.includedirs = acf_dirs + acf_auxinc_dirs
        self.cpp_info.libdirs = [os.path.join("Acf/Lib", self._build_folder_suffix())]
        # prioritize base libs on which other libs depend
        libs = 'iqt ipackage idoc ifile iimg icmm i2d i3d iprm ilog ibase icomp iattr imath iser imod istd'.split()
        libs = 'iqtdoc iview iqt2d iqtprm iloggui ifilegui iqtgui iwidgets'.split() + libs
        self.cpp_info.libs = [lib for lib in self._collect_libs() if lib not in libs] + libs

        # To support editable mode we need explicitly assign same values to conan v2 fields,
        # probably limitation of conan 1.x when using v2 features
        # HACK: we call it in package_info() instead of layout() because self.dependencies["qt"].ref.version is needed to calculate the directory name
        self.cpp.source.includedirs = acf_dirs
        self.cpp.build.includedirs = acf_auxinc_dirs
        self.cpp.build.libdirs = [] + self.cpp_info.libdirs # force deepcopy
        self.cpp.build.bindirs = [] + self.cpp_info.bindirs

        qt_components = self.dependencies["qt"].cpp_info.components.keys()
        self.output.info(f"Qt components: {qt_components}")
        if len(qt_components) > 0:
            # some acf components require more qt libraries, but for conan generated cmake configs keep it minimal
            self.cpp_info.requires = ['qt::qtCore']
        else:
            # we shall not keep cpp_info.requires empty, otherwise XmlPatterns will be added by default (see def requirements)
            # and we want to avoid it as consumers of Acf may depend on XmlPatterns optionally
            self.cpp_info.requires = ['qt::qt']

        if self.settings.os == 'Windows':
            self.cpp_info.system_libs = ["mpr"]

        cmakeModules = ["Config/CMake/CustomBuildFunction.cmake", "Config/CMake/ProjectRoot.cmake"]
        # modern v2 approach
        self.cpp.package.set_property("cmake_build_modules", cmakeModules)
        self.cpp.source.set_property("cmake_build_modules", cmakeModules)
        # old v1 approach
        self.cpp_info.build_modules["cmake_find_package"] = cmakeModules
        self.cpp_info.build_modules["cmake_find_package_multi"] = cmakeModules
