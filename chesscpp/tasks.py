from pickle import BUILD
from invoke import task 
import invoke
import os
from dataclasses import dataclass
from glob import glob
from dotenv import dotenv_values
from numpy import source


G_TEST_FETCH = """
include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
"""

G_COMPILE_OPTIONS = """
add_compile_options(
-g
-O2
)
enable_testing()
"""

G_TEST_INCLUDE = """
include(GoogleTest)
gtest_discover_tests(chesscpp)
"""

G_TEST_LINK_LIBRARY = "gtest_main"

CMAKELISTS = """EOF
cmake_minimum_required(VERSION 3.14)

project(chesscpp)
set(CMAKE_CXX_STANDARD {0})
{1}
{2}
add_executable(
    chesscpp 
{3}
)
target_link_libraries(
  chesscpp
  {4}
)
{5}
EOF
"""

CONFIG = dotenv_values(".test.env")
SOURCE = CONFIG["SOURCE"]
TEST_TARGET = CONFIG["TEST_TARGET"]
DEV_TARGET = CONFIG["DEV_TARGET"]
CXX = CONFIG["CXX"]

def configure_cmake(files=list[str], includeGTest=False):
    files_str = "\n".join([f'    {file}' for file in files])
    print(files_str)
    if includeGTest: return CMAKELISTS.format(CXX, G_TEST_FETCH, G_COMPILE_OPTIONS, files_str, G_TEST_LINK_LIBRARY, G_TEST_INCLUDE
    )
    return CMAKELISTS.format(CXX, '', '', files_str, '', '')


@dataclass
class Cmd: 
    c: "invoke.Context"
    bot_mode: "bool"
    
    def add(self, cmd):
        if not hasattr(self, 'cmd'):
            self.cmd = cmd
        else:
            self.cmd += f" && {cmd}"

    def __call__(self, verbose=False):
        if verbose:
            print(f"\033[0;33m[RUN]: {self.cmd}\033[0;37m")
        self.c.run(self.cmd, pty=not self.bot_mode)
        return Cmd(self.c, self.bot_mode)

@task 
def clean(c):
    if os.path.exists(f'{TEST_TARGET}/build'):
        c.run(f'rm -rf {TEST_TARGET}/build')

@task
def build(c, remake=False, bot_mode=False):
    cmd = Cmd(c, bot_mode)
    cmd.add(f"cd {TEST_TARGET}")

    if not os.path.exists(f'{TEST_TARGET}/build'):
        cmd.add("mkdir build")

    if not os.path.exists(f'{TEST_TARGET}/CMakeLists.txt') or remake:
        files = [f'../{file}' for file in glob(os.path.join(SOURCE, '*.cpp'))]
        files += [file.split('/')[-1] for file in glob(os.path.join(TEST_TARGET,'*.c*'))]
        cmd.add(f"cat > CMakeLists.txt << {configure_cmake(files, includeGTest=True)}")
    
    cmd = cmd()
    cmd.add(f"cd {TEST_TARGET}/build")

    if not os.path.exists(f'{TEST_TARGET}/build') or remake:
        cmd.add("cmake ../")
    
    cmd.add("cmake --build .")
    cmd(verbose=True)


@task
def test(c, debugger='', remake=False, bot_mode=False, output_on_failure=False, rerun_failed=False):
    cmd = Cmd(c, bot_mode)
    cmd.add(f"cd {TEST_TARGET}")

    if not os.path.exists(f'{TEST_TARGET}/build'):
        cmd.add("mkdir build")

    if not os.path.exists(f'{TEST_TARGET}/CMakeLists.txt') or remake:
        files = [f'../{file}' for file in glob(os.path.join(SOURCE, '*.cpp'))]
        files += [file.split('/')[-1] for file in glob(os.path.join(TEST_TARGET,'*.c*'))]
        cmd.add(f"cat > CMakeLists.txt << {configure_cmake(files, includeGTest=True)}")
    
    cmd = cmd()
    cmd.add(f"cd {TEST_TARGET}/build")

    if not os.path.exists(f'{TEST_TARGET}/build') or remake:
        cmd.add("cmake ../")
    
    cmd.add("cmake --build .")

    if debugger == '':
        ctest_cmd = "ctest"
        if output_on_failure:
            ctest_cmd += " --output-on-failure"
        if rerun_failed:
            ctest_cmd += " --rerun-failed"
        cmd.add(ctest_cmd)
    else:
        cmd.add(f"{debugger} chesscpp")
    cmd(verbose=True)
   

@task 
def cleandev(c):
    if os.path.exists(f'{DEV_TARGET}/build'):
        c.run(f'rm -rf {DEV_TARGET}/build')

@task
def dev(c, no_debugger=False, remake=False, bot_mode=False, output_on_failure=False, rerun_failed=False):
    cmd = Cmd(c, bot_mode)
    cmd.add(f"cd {DEV_TARGET}")

    if not os.path.exists(f'{DEV_TARGET}/build'):
        cmd.add("mkdir build")

    if not os.path.exists(f'{DEV_TARGET}/CMakeLists.txt') or remake:
        files = [f'../{file}' for file in glob(os.path.join(SOURCE, '*.cpp'))]
        files += [file.split('/')[-1] for file in glob(os.path.join(DEV_TARGET,'*.c*'))]
        cmd.add(f"cat > CMakeLists.txt << {configure_cmake(files)}")
    
    cmd = cmd()
    cmd.add(f"cd {DEV_TARGET}/build")

    if not os.path.exists(f'{DEV_TARGET}/build') or remake:
        cmd.add("cmake ../")
    
    cmd.add("cmake --build .")
    if (no_debugger):
        cmd.add(f"lldb chesscpp")
    else:
        cmd.add(f"./chesscpp")
    cmd(verbose=True)