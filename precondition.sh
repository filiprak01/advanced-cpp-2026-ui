#!/usr/bin/env bash
set -euo pipefail

if ! command -v apt-get >/dev/null 2>&1; then
    echo "Ten skrypt jest przygotowany dla Ubuntu/Debian/WSL z apt-get."
    exit 1
fi

echo "==> Instalowanie zaleznosci UI PWChat dla Linux/WSL..."
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    qt6-base-dev \
    qt6-websockets-dev \
    libgl1-mesa-dev \
    libxkbcommon-x11-0 \
    libxcb-cursor0 \
    nlohmann-json3-dev \
    libssl-dev \
    libgtest-dev \
    doxygen \
    graphviz

echo
echo "==> Gotowe. Mozesz budowac UI:"
echo "    cmake --preset client-debug"
echo "    cmake --build --preset client-debug"
