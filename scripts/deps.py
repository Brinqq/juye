import subprocess
import os
from collections import namedtuple
from enum import Enum
import shutil

#TODO: Set relative to project root so this is portable across computers.

DEP_ROOT = "C:/main/.dev/projects/codename/ssf/external/"
DEP_BIN = DEP_ROOT + "bin/"
DEP_INCLUDE = DEP_ROOT + "include/"

root = os.path.normpath(DEP_ROOT)
bin = os.path.normpath(DEP_BIN)
include = os.path.normpath(DEP_INCLUDE)


class BuildSystem(Enum):
    Cmake = 1

git_dependency = namedtuple("git_dependency", ["name", "repo", "build_system"])

dependencies_git = [
    git_dependency
    (
        "bcl", 
       "https://github.com/Brinqq/bcl.git", 
       BuildSystem.Cmake
    )
]

def make_directories():
    os.mkdir(DEP_ROOT)
    os.mkdir(DEP_BIN)
    os.mkdir(DEP_INCLUDE)

def clean():
    arr = os.listdir(DEP_ROOT)
    if not arr.__len__() == 0:
        shutil.rmtree(DEP_ROOT)

def build_dependency(sys: BuildSystem, proj_root: str): 
    if sys == BuildSystem.Cmake:
        return

def git_clone(repo: str, output_path: str, commit: str | None):
    res = subprocess.run(["git", "clone", repo, output_path]).returncode
    if res:
        print("error")
        exit()


def build_full():
    for dep in dependencies_git:
        git_clone(dep.repo, DEP_ROOT + dep.name, None)
        build_dependency(BuildSystem.Cmake, DEP_ROOT + dep.name)

clean()

if not os.path.exists(DEP_ROOT):
    make_directories()
    build_full()   

