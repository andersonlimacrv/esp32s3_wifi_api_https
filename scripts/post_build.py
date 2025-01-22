import os
import shutil
from datetime import datetime

def parse_version_header(version_header_path):
    """Lê as informações de versão do arquivo version.h."""
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

def after_build(source, target, env):
    firmware_path = os.path.join(env.subst("$BUILD_DIR"), "firmware.bin")
    destination_dir = os.path.join(env.subst("$PROJECT_DIR"), "current_firmware")
    
    if not os.path.exists(destination_dir):
        os.makedirs(destination_dir)
    
    # Caminho para o arquivo version.h
    version_header_path = os.path.join(env.subst("$PROJECT_DIR"), "src", "version.h")
    
    # Obter informações de versão do arquivo version.h
    version_info = parse_version_header(version_header_path)
    firmware_version = version_info.get("FIRMWARE_VERSION", "unknown")
    commit_date = version_info.get("COMMIT_DATE", "unknown")
    
    # Formatar COMMIT_DATE para AAAAMMDD
    commit_date_short = commit_date.split(" ")[0].replace("-", "") if commit_date != "unknown" else "unknown"
    
    # Obter hora atual no formato HHMM
    current_time = datetime.now().strftime("%H%M")
    
    # Nome do arquivo no formato desejado
    new_firmware_name = f"firmware_{firmware_version}__{commit_date_short}_{current_time}.bin"
    new_firmware_path = os.path.join(destination_dir, new_firmware_name)
    
    # Copiar o firmware com o novo nome
    shutil.copy(firmware_path, new_firmware_path)
    print(f"Firmware copiado para {new_firmware_path}")

# Adiciona a ação ao PlatformIO
Import("env")
env.AddPostAction("buildprog", after_build)
