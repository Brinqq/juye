import platform
import subprocess
from pathlib import Path

external_folder = Path(__file__).resolve().parent.parent / "external"

glm_folder = external_folder / "glm"
yamlcpp_folder = external_folder / "yaml-cpp"
stb_folder = external_folder / "stb"
assimp_folder = external_folder / "assimp"
glfw_folder = external_folder / "glfw"


#TODO: maybe use something like gitpython for better dependency managment<git tags/branches>.

def git_clone(repo: str, path: Path):
    if not path.exists():
        subprocess.run(
            ["git", "clone", "-q", repo, path])

git_clone("https://github.com/g-truc/glm.git", glm_folder)
git_clone("https://github.com/jbeder/yaml-cpp.git", yamlcpp_folder)
git_clone("https://github.com/nothings/stb.git", stb_folder)
git_clone("https://github.com/assimp/assimp.git", assimp_folder)

if not platform.system() == "Windows":
    git_clone("https://github.com/glfw/glfw.git", glfw_folder)
