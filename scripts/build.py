#!/usr/bin/env python3
import os
import sys
import shutil
import subprocess

if len(sys.argv) < 2:
    print(f"Usage: {sys.argv[0]} <emscripten|desktop> [args...]")
    sys.exit(1)

MODE = sys.argv[1]
ARGS = sys.argv[2:]

def run(cmd, cwd=None, env=None):
    subprocess.check_call(cmd, cwd=cwd, env=env)

if MODE == "emscripten":
    EMSDK = os.environ.get("EMSDK")
    if not EMSDK:
        print("Error: EMSDK environment variable is not set")
        sys.exit(1)
    em_env = subprocess.check_output(
        f"source {EMSDK}/emsdk_env.sh && env",
        shell=True, executable="/bin/bash"
    )
    env = dict(line.decode().split("=", 1) for line in em_env.splitlines() if b"=" in line)

    run(["emcmake", "cmake", "--preset", "emscripten"], env=env)
    run(["cmake", "--build", "--preset", "emscripten"], env=env)

    BUILD_DIR = "out/web"
    DEST_DIR = os.path.join("www", "editor")
    if not os.path.exists(DEST_DIR):
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
                content = content.replace(
                    "<canvas", "<canvas style='position:absolute;top:0;left:0;width:100%;height:100%;'"
                )
                if "<head>" in content:
                    content = content.replace(
                        "<head>",
                        "<head><style>html,body{margin:0;padding:0;overflow:hidden;height:100%;}</style>"
                    )
                with open(dst_path, "w") as f:
                    f.write(content)
            else:
                shutil.copy2(src_path, dst_path)

    html_file = os.path.join(DEST_DIR, "index.html")
    if shutil.which("emrun"):
        run(["emrun", html_file], env=env)
    else:
        print(f"Open {html_file} manually in a browser")

else:
    BUILD_DIR = "out/desktop"
    run(["cmake", "--preset", "desktop"])
    run(["cmake", "--build", "--preset", "desktop"])

    exe_file = os.path.join(BUILD_DIR, "bin", "CruzGui")
    if not os.path.exists(exe_file):
        print(f"Error: {exe_file} not found")
        sys.exit(1)
    run([exe_file] + ARGS)