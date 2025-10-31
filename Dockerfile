FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    clang \
    cmake \
    ninja-build \
    git \
    curl \
    bash \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

ENV PATH="/usr/bin:${PATH}"

ARG BUILD_MODE=desktop
RUN python3 scripts/build.py $BUILD_MODE

FROM ubuntu:24.04

WORKDIR /app

COPY --from=builder /app/out /app/out
COPY --from=builder /app/www /app/www

CMD ["./out/desktop/bin/CruzGui"]