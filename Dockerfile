FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV XDG_RUNTIME_DIR=/tmp/runtime
RUN mkdir -p $XDG_RUNTIME_DIR && chmod 700 $XDG_RUNTIME_DIR

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    python3-pip \
    clang \
    cmake \
    ninja-build \
    git \
    curl \
    bash \
    libwayland-dev \
    libx11-dev \
    libxext-dev \
    libxrender-dev \
    libxrandr-dev \
    libxtst-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libxkbcommon-dev \
    pkg-config \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

ARG BUILD_MODE=desktop
ENV BUILD_MODE=${BUILD_MODE}

RUN python3 scripts/build.py $BUILD_MODE --no-run

FROM ubuntu:24.04

ENV XDG_RUNTIME_DIR=/tmp/runtime
RUN mkdir -p $XDG_RUNTIME_DIR && chmod 700 $XDG_RUNTIME_DIR

WORKDIR /app
COPY --from=builder /app/out ./out
COPY --from=builder /app/www ./www

CMD ["bash"]