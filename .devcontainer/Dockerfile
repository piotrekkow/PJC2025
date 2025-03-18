FROM mcr.microsoft.com/devcontainers/cpp:ubuntu

# Install basic dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    libasound2-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev \
    wget \
    software-properties-common

# Install newer GCC
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y gcc-11 g++-11 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110 --slave /usr/bin/g++ g++ /usr/bin/g++-11

# Install newer version of CMake
WORKDIR /tmp
RUN wget https://github.com/Kitware/CMake/releases/download/v3.26.4/cmake-3.26.4-linux-x86_64.sh \
    && chmod +x cmake-3.26.4-linux-x86_64.sh \
    && ./cmake-3.26.4-linux-x86_64.sh --skip-license --prefix=/usr/local \
    && rm cmake-3.26.4-linux-x86_64.sh

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