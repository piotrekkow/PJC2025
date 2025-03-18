FROM mcr.microsoft.com/devcontainers/cpp:ubuntu

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libasound2-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev

# Build and install raylib
WORKDIR /tmp
RUN git clone https://github.com/raysan5/raylib.git
WORKDIR /tmp/raylib
RUN mkdir build && cd build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DSHARED=ON -DBUILD_EXAMPLES=OFF .. && \
    make -j$(nproc) && \
    make install

# Clean up
RUN rm -rf /tmp/raylib