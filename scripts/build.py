#!/usr/bin/env python3
import os
import sys
import subprocess
import platform
import shutil
import time

GREEN = "\033[92m"
YELLOW = "\033[93m"
RED = "\033[91m"
RESET = "\033[0m"

if platform.system() == "Windows":
    os.system("")

def log_info(msg):
    print(f"{GREEN}[INFO] {msg}{RESET}")

def log_warn(msg):
    print(f"{YELLOW}[WARN] {msg}{RESET}")

def log_error(msg):
    print(f"{RED}[ERROR] {msg}{RESET}")

def run(cmd, cwd=None, env=None, shell=None, check=True):
    if shell is None:
        shell = platform.system() == "Windows"
    log_warn(f">>> Running: {' '.join(cmd) if isinstance(cmd, list) else cmd}")
    start_time = time.time()
    result = subprocess.run(cmd, cwd=cwd, env=env, shell=shell,
                            capture_output=True, text=True)
    elapsed = time.time() - start_time
    if result.stdout:
        print(result.stdout)
    if result.stderr:
        print(result.stderr)
    if check and result.returncode != 0:
        log_error(f"Command failed: {cmd} ({elapsed:.2f}s)")
        raise subprocess.CalledProcessError(result.returncode, cmd)
    log_info(f"Command completed in {elapsed:.2f}s")
    return result

def get_emscripten_env():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    EMSDK = os.path.abspath(os.path.join(script_dir, "../thirdparty/emscripten-sdk"))
    if not os.path.isdir(EMSDK):
        log_error("Emscripten SDK not found in ../thirdparty/emscripten-sdk")
        sys.exit(1)
    os.environ["EMSDK"] = EMSDK
    env = os.environ.copy()
    system = platform.system()
    log_info(">>> Activating Emscripten SDK...")
    if system == "Windows":
        subprocess.check_call(f'cmd /c "{EMSDK}\\emsdk.bat activate latest"', shell=True)
        em_env_cmd = f'cmd /c "{EMSDK}\\emsdk_env.bat && set"'
        em_env = subprocess.check_output(em_env_cmd, shell=True)
        for line in em_env.splitlines():
            key, _, value = line.decode(errors="ignore").partition("=")
            env[key] = value
    else:
        subprocess.check_call(f"bash -c 'source {EMSDK}/emsdk_env.sh && {EMSDK}/emsdk activate latest'", shell=True)
        em_env_cmd = f"bash -c 'source {EMSDK}/emsdk_env.sh && env'"
        em_env = subprocess.check_output(em_env_cmd, shell=True, executable="/bin/bash")
        for line in em_env.splitlines():
            key, _, value = line.decode(errors='ignore').partition("=")
            env[key] = value
    log_info(">>> Emscripten environment loaded.")
    return env

def build_emscripten():
    start_time = time.time()
    try:
        env = get_emscripten_env()
        run(["emcmake", "cmake", "--preset", "emscripten"], env=env)
        run(["cmake", "--build", "--preset", "emscripten"], env=env)
        build_dir = "out/web"
        dest_dir = os.path.join("www", "editor")
        os.makedirs(dest_dir, exist_ok=True)
        for filename in os.listdir(build_dir):
            if filename.endswith((".html", ".js", ".wasm")):
                src_path = os.path.join(build_dir, filename)
                dst_path = os.path.join(dest_dir, "index.html" if filename.endswith(".html") else filename)
                if filename.endswith(".html"):
                    with open(src_path, "r") as f:
                        content = f.read()
                    content = content.replace("<canvas", "<canvas style='position:absolute;top:0;left:0;width:100%;height:100%;'")
                    if "<head>" in content:
                        content = content.replace("<head>", "<head><style>html,body{margin:0;padding:0;overflow:hidden;height:100%;}</style>")
                    with open(dst_path, "w") as f:
                        f.write(content)
                else:
                    shutil.copy2(src_path, dst_path)
        elapsed = time.time() - start_time
        log_info(f"Emscripten build completed in {elapsed:.2f}s")
        log_info(f"Open {os.path.join(dest_dir, 'index.html')} manually in a browser")
    except Exception as e:
        elapsed = time.time() - start_time
        log_error(f"Emscripten build failed after {elapsed:.2f}s")
        raise e

def build_desktop_windows(config="Debug", compiler="msvc"):
    start_time = time.time()
    try:
        base_build_dir = os.path.join("out", "desktop")
        if compiler == "msvc":
            build_dir = os.path.join(base_build_dir, "msvc")
        else:
            build_dir = os.path.join(base_build_dir, "clang")
        os.makedirs(build_dir, exist_ok=True)
        exe_file = None
        if compiler == "msvc":
            run([
                "cmake", "-G", "Visual Studio 17 2022", "-A", "x64",
                "-S", ".", "-B", build_dir
            ])
            run(["cmake", "--build", build_dir, "--config", config])
            
            exe_file = os.path.join(build_dir, "bin", config, "CruzGui.exe")
        elif compiler == "clang":
            run([
                "cmake", "-G", "Ninja",
                f"-DCMAKE_C_COMPILER=clang",
                f"-DCMAKE_CXX_COMPILER=clang++",
                f"-DCMAKE_BUILD_TYPE={config}",
                "-S", ".", "-B", build_dir
            ])
            build_cmd = ["cmake", "--build", build_dir]
            if config.lower() == "release":
                build_cmd += ["--", "-j", str(os.cpu_count())]
            run(build_cmd)
            exe_file = os.path.join(build_dir, "bin", "CruzGui.exe")
        else:
            log_error(f"Unknown compiler: {compiler}")
            sys.exit(1)
        if not os.path.exists(exe_file):
            log_error(f"{exe_file} not found. Check build configuration and compiler output.")
            sys.exit(1)
        elapsed = time.time() - start_time
        log_info(f"Desktop build completed in {elapsed:.2f}s: {exe_file}")
        return exe_file
    except Exception as e:
        elapsed = time.time() - start_time
        log_error(f"Desktop build failed after {elapsed:.2f}s")
        raise e

def build_desktop_linux(config="Debug"):
    start_time = time.time()
    try:
        build_dir = os.path.join("out", "desktop")
        os.makedirs(build_dir, exist_ok=True)
        run(["cmake", "-S", ".", "-B", build_dir, f"-DCMAKE_BUILD_TYPE={config}"])
        run(["cmake", "--build", build_dir])
        exe_file = os.path.join(build_dir, "CruzGui")
        if not os.path.exists(exe_file):
            log_error(f"{exe_file} not found")
            sys.exit(1)
        elapsed = time.time() - start_time
        log_info(f"Desktop build completed in {elapsed:.2f}s: {exe_file}")
        return exe_file
    except Exception as e:
        elapsed = time.time() - start_time
        log_error(f"Desktop build failed after {elapsed:.2f}s")
        raise e

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <emscripten|desktop> [--config CONFIG] [--compiler COMPILER] [--run|-r] [args...]")
        sys.exit(1)
    mode = sys.argv[1]
    args = sys.argv[2:]
    config = "Debug"
    compiler = "msvc"
    run_after_build = False
    app_args = []
    while args:
        arg = args.pop(0)
        if arg == "--run" or arg == "-r":
            run_after_build = True
        elif arg == "--config":
            if args:
                config = args.pop(0)
        elif arg == "--compiler":
            if args:
                compiler = args.pop(0)
        else:
            app_args.append(arg)
    system = platform.system()
    if mode == "emscripten":
        build_emscripten()
    elif mode == "desktop":
        if system == "Windows":
            exe = build_desktop_windows(config, compiler)
        else:
            exe = build_desktop_linux(config)
        if run_after_build:
            run([exe] + app_args)
    else:
        log_error(f"Unknown mode: {mode}")
        sys.exit(1)