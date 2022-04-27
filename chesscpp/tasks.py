from invoke import task 
import invoke
import os
from dataclasses import dataclass
from glob import glob
from dotenv import dotenv_values

CMAKELISTS = """EOF
cmake_minimum_required(VERSION 3.14)

project(chesscpp)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD {0})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_compile_options(
    -g
)

enable_testing()

add_executable(
    chesscpp 
    main.cpp
{1}
)

target_link_libraries(
  chesscpp
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(chesscpp)
EOF
"""

CONFIG = dotenv_values(".test.env")
SOURCE = CONFIG["SOURCE"]
TARGET = CONFIG["TARGET"]
CXX = CONFIG["CXX"]

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
    if os.path.exists(f'{TARGET}/build'):
        c.run(f'rm -rf {TARGET}/build')

@task
def test(c, debug=False, remake=False, bot_mode=False, output_on_failure=False, rerun_failed=False):
    cmd = Cmd(c, bot_mode)
    cmd.add(f"cd {TARGET}")

    if not os.path.exists(f'{TARGET}/build'):
        cmd.add("mkdir build")

    if not os.path.exists(f'{TARGET}/CMakeLists.txt') or remake:
        files = "\n".join([f'    ../{file}' for file in glob(SOURCE)])
        cmd.add(f"cat > CMakeLists.txt << {CMAKELISTS.format(CXX, files)}")
    
    cmd = cmd()
    cmd.add(f"cd {TARGET}/build")

    if not os.path.exists(f'{TARGET}/build') or remake:
        cmd.add("cmake ../")
    
    cmd.add("cmake --build .")

    if not debug:
        ctest_cmd = "ctest"
        if output_on_failure:
            ctest_cmd += " --output-on-failure"
        if rerun_failed:
            ctest_cmd += " --rerun-failed"
        cmd.add(ctest_cmd)
    else:
        cmd.add("lldb chesscpp")

    cmd(verbose=True)
    