import subprocess
from datetime import datetime, timezone

def get_git_version():
    try:
        return subprocess.check_output(["git", "rev-parse", "--short", "HEAD"]).strip().decode("utf-8")
    except subprocess.CalledProcessError as e:
        print(f"Error getting Git version: {e}")
        return "unknown"

def get_git_commit_date():
    try:
        commit_date = subprocess.check_output(["git", "log", "-1", "--format=%ci"]).strip().decode("utf-8")
        
        commit_date_obj = datetime.strptime(commit_date, "%Y-%m-%d %H:%M:%S %z")
        commit_date_utc = commit_date_obj.astimezone(timezone.utc)  
        
        return commit_date_utc.strftime("%Y-%m-%dT%H:%M:%S.000Z") 
    except subprocess.CalledProcessError as e:
        print(f"Error getting commit date: {e}")
        return "unknown"
    
def get_git_commit_message():
    try:
        return subprocess.check_output(["git", "log", "-1", "--format=%s"]).strip().decode("utf-8")
    except subprocess.CalledProcessError as e:
        print(f"Error getting commit message: {e}")
        return "unknown" 


git_version = get_git_version()
commit_date = get_git_commit_date()
commit_message = get_git_commit_message()

with open("src/version.h", "w") as f:
    f.write(f"#define FIRMWARE_VERSION \"{git_version}\"\n")
    f.write(f"#define COMMIT_DATE \"{commit_date}\"\n")
    f.write(f"#define COMMIT_MESSAGE \"{commit_message}\"\n")

print("File src/version.h successfully updated.")
