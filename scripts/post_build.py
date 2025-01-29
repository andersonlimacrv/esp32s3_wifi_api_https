import os
import shutil
from datetime import datetime

def parse_version_header(version_header_path):
    version_info = {}
    if os.path.exists(version_header_path):
        with open(version_header_path, "r") as f:
            for line in f:
                if line.startswith("#define"):
                    parts = line.split()
                    if len(parts) >= 3:
                        key = parts[1]
                        value = parts[2].strip('"')
                        version_info[key] = value
    return version_info

def format_date_to_filename(date_str):
    date_obj = datetime.strptime(date_str, "%Y-%m-%dT%H:%M:%S.000Z")
    formatted_date = date_obj.strftime("%Y%m%d%H%M")
    return formatted_date

def after_build(source, target, env):
    firmware_path = os.path.join(env.subst("$BUILD_DIR"), "firmware.bin")
    destination_dir = os.path.join(env.subst("$PROJECT_DIR"), "current_firmware")
    
    if not os.path.exists(destination_dir):
        os.makedirs(destination_dir)
    
    version_header_path = os.path.join(env.subst("$PROJECT_DIR"), "src", "version.h")
    version_info = parse_version_header(version_header_path)
    firmware_version = version_info.get("FIRMWARE_VERSION", "unknown")
    commit_date = version_info.get("COMMIT_DATE", "unknown")
    commit_date_formatted = format_date_to_filename(commit_date)
    
    new_firmware_name = f"{commit_date_formatted}_firmware_{firmware_version}.bin"
    new_firmware_path = os.path.join(destination_dir, new_firmware_name)
    
    shutil.copy(firmware_path, new_firmware_path)
    print(f"Firmware copied to {new_firmware_path}")

Import("env")
env.AddPostAction("buildprog", after_build)
