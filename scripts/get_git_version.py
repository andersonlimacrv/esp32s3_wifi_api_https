import subprocess
from datetime import datetime

def get_git_version():
    try:
        git_version = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"]).strip().decode('utf-8')
        return git_version
    except subprocess.CalledProcessError:
        return "unknown"

def get_git_commit_date():
    try:
        # Pega a data do commit (último commit)
        commit_date = subprocess.check_output(["git", "log", "-1", "--format=%ci"]).strip().decode('utf-8')
        return commit_date
    except subprocess.CalledProcessError:
        return "unknown"

git_version = get_git_version()
commit_date = get_git_commit_date()

current_date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

with open("src/version.h", "w") as f:
    f.write(f"#define FIRMWARE_VERSION \"{git_version}\"\n")
    f.write(f"#define COMMIT_DATE \"{commit_date}\"\n")
    f.write(f"#define BUILD_DATE \"{current_date}\"\n")
