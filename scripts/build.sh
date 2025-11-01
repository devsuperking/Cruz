#!/usr/bin/env bash
set -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 <emscripten|desktop> [--config CONFIG] [--run|-r] [args...]"
    exit 1
fi

MODE="$1"
shift

CONFIG="Debug"
RUN_AFTER_BUILD=false
APP_ARGS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --run|-r)
            RUN_AFTER_BUILD=true
            shift
            ;;
        --config)
            shift
            CONFIG="$1"
            shift
            ;;
        *)
            APP_ARGS+=("$1")
            shift
            ;;
    esac
done

run_cmd() {
    echo ">>> Running: $*"
    "$@"
}

build_emscripten() {
    SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    EMSDK="$SCRIPT_DIR/../thirdparty/emscripten-sdk"

    if [ ! -d "$EMSDK" ]; then
        echo "Error: Emscripten SDK not found in $EMSDK"
        exit 1
    fi

    echo ">>> Activating Emscripten SDK..."
    source "$EMSDK/emsdk_env.sh"
    emsdk activate latest

    run_cmd emcmake cmake --preset emscripten
    run_cmd cmake --build --preset emscripten

    BUILD_DIR="out/web"
    DEST_DIR="www/editor"
    mkdir -p "$DEST_DIR"

    for file in "$BUILD_DIR"/*; do
        filename=$(basename "$file")
        if [[ "$filename" == *.html ]]; then
            dst="$DEST_DIR/index.html"
            content=$(<"$file")
            content=${content//<canvas/<canvas style='position:absolute;top:0;left:0;width:100%;height:100%;'}
            content=${content/<head>/<head><style>html,body{margin:0;padding:0;overflow:hidden;height:100%;}<\/style>}
            echo "$content" > "$dst"
        else
            cp "$file" "$DEST_DIR/"
        fi
    done

    echo "Open $DEST_DIR/index.html manually in a browser"
}

build_desktop() {
    BUILD_DIR="out/desktop"
    mkdir -p "$BUILD_DIR"

    run_cmd cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$CONFIG"
    run_cmd cmake --build "$BUILD_DIR"

    EXE_FILE="$BUILD_DIR/CruzGui"
    if [ ! -f "$EXE_FILE" ]; then
        echo "Error: $EXE_FILE not found"
        exit 1
    fi

    if [ "$RUN_AFTER_BUILD" = true ]; then
        run_cmd "$EXE_FILE" "${APP_ARGS[@]}"
    else
        echo "Desktop build completed. Run with --run to launch the application."
    fi
}

case "$MODE" in
    emscripten)
        build_emscripten
        ;;
    desktop)
        build_desktop
        ;;
    *)
        echo "Unknown mode: $MODE"
        exit 1
        ;;
esac
