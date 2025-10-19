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

BUILD_DIR = "out/build"

if os.path.exists(BUILD_DIR):
    shutil.rmtree(BUILD_DIR)
os.makedirs(BUILD_DIR, exist_ok=True)

def run(cmd, cwd=None, env=None):
    subprocess.check_call(cmd, cwd=cwd, env=env)

if MODE == "emscripten":
    EMSDK = os.environ.get("EMSDK")
    if not EMSDK:
        print("Error: EMSDK environment variable is not set")
        sys.exit(1)
    em_env = subprocess.check_output(f"source {EMSDK}/emsdk_env.sh && env", shell=True, executable="/bin/bash")
    env = dict(line.decode().split("=", 1) for line in em_env.splitlines() if b"=" in line)
    cmake_cmd = [
        "cmake", "-S", ".", "-B", BUILD_DIR,
        "-DCMAKE_BUILD_TYPE=Release",
        f"-DCMAKE_TOOLCHAIN_FILE={EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake",
        "-DBUILD_TARGET=emscripten"
    ]
    run(cmake_cmd, cwd=os.getcwd(), env=env)
    run(["cmake", "--build", BUILD_DIR, "--config", "Release"], cwd=os.getcwd(), env=env)

    html_file = os.path.join(BUILD_DIR, "CruzGui.html")
    if not os.path.exists(html_file):
        print(f"Error: {html_file} not found")
        sys.exit(1)

    with open(html_file, "r") as f:
        html = f.read()

    html = html.replace("<canvas", "<canvas style='position:absolute;top:0;left:0;width:100%;height:100%;'")
    if "<head>" in html:
        html = html.replace("<head>", "<head><style>html,body{margin:0;padding:0;overflow:hidden;height:100%;}</style>")

    with open(html_file, "w") as f:
        f.write(html)

    if shutil.which("emrun"):
        run(["emrun", html_file], env=env)
    else:
        print(f"Open {html_file} manually in a browser")

else:
    cmake_cmd = [
        "cmake", "-S", ".", "-B", BUILD_DIR,
        "-DCMAKE_BUILD_TYPE=Release",
        "-DBUILD_TARGET=desktop"
    ]
    run(cmake_cmd, cwd=os.getcwd())
    run(["cmake", "--build", BUILD_DIR, "--config", "Release"], cwd=os.getcwd())

    exe_file = os.path.join(BUILD_DIR, "bin", "CruzGui")
    if not os.path.exists(exe_file):
        print(f"Error: {exe_file} not found")
        sys.exit(1)

    run([exe_file] + ARGS)
