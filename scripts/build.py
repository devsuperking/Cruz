#!/usr/bin/env python3
import os
import sys
import shutil
import subprocess
import platform

if len(sys.argv) < 2:
    print(f"Usage: {sys.argv[0]} <emscripten|desktop> [args...]")
    sys.exit(1)

MODE = sys.argv[1]
ARGS = sys.argv[2:]

def run(cmd, cwd=None, env=None, shell=False):
    subprocess.check_call(cmd, cwd=cwd, env=env, shell=shell)

def get_emscripten_env():
    EMSDK = os.environ.get("EMSDK")
    if not EMSDK:
        print("Error: EMSDK environment variable is not set")
        sys.exit(1)

    system = platform.system()
    env = os.environ.copy()

    if system == "Windows":
        em_env_cmd = f'cmd /c "{EMSDK}\\emsdk_env.bat && set"'
        em_env = subprocess.check_output(em_env_cmd, shell=True)
        for line in em_env.splitlines():
            key, _, value = line.decode(errors="ignore").partition("=")
            env[key] = value
    else:
        em_env_cmd = f"source {EMSDK}/emsdk_env.sh && env"
        em_env = subprocess.check_output(em_env_cmd, shell=True, executable="/bin/bash")
        for line in em_env.splitlines():
            key, _, value = line.decode(errors="ignore").partition("=")
            env[key] = value

    return env

if MODE == "emscripten":
    env = get_emscripten_env()
    run(["emcmake", "cmake", "--preset", "emscripten"], env=env, shell=(platform.system()=="Windows"))
    run(["cmake", "--build", "--preset", "emscripten"], env=env, shell=(platform.system()=="Windows"))

    BUILD_DIR = "out/web"
    DEST_DIR = os.path.join("www", "editor")
    os.makedirs(DEST_DIR, exist_ok=True)

    for filename in os.listdir(BUILD_DIR):
        if filename.endswith((".html", ".js", ".wasm")):
            src_path = os.path.join(BUILD_DIR, filename)
            dst_path = os.path.join(DEST_DIR, filename)
            if filename.endswith(".html"):
                dst_path = os.path.join(DEST_DIR, "index.html")
            with open(src_path, "r" if filename.endswith(".html") else "rb") as f:
                content = f.read()
            if filename.endswith(".html"):
                content = content.replace("<canvas", "<canvas style='position:absolute;top:0;left:0;width:100%;height:100%;'")
                if "<head>" in content:
                    content = content.replace("<head>", "<head><style>html,body{margin:0;padding:0;overflow:hidden;height:100%;}</style>")
                with open(dst_path, "w") as f:
                    f.write(content)
            else:
                shutil.copy2(src_path, dst_path)

    html_file = os.path.join(DEST_DIR, "index.html")
    if shutil.which("emrun"):
        run(["emrun", html_file], env=env, shell=(platform.system()=="Windows"))
    else:
        print(f"Open {html_file} manually in a browser")

elif MODE == "desktop":
    BUILD_DIR = "out/desktop"
    run(["cmake", "--preset", "desktop"], shell=(platform.system()=="Windows"))
    run(["cmake", "--build", "--preset", "desktop"], shell=(platform.system()=="Windows"))

    exe_file = os.path.join(BUILD_DIR, "bin", "CruzGui")
    if platform.system() == "Windows":
        exe_file += ".exe"

    if not os.path.exists(exe_file):
        print(f"Error: {exe_file} not found")
        sys.exit(1)

    run([exe_file] + ARGS)
else:
    print(f"Unknown mode: {MODE}")
    sys.exit(1)
