FROM ubuntu:24.04

ARG user=ubuntu
ENV DEBIAN_FRONTEND=noninteractive \
    PATH=/home/${user}/.cargo/bin:$PATH \
    LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8 \
    RUSTUP_HOME=/home/${user}/.rustup \
    CARGO_HOME=/home/${user}/.cargo

ARG workspace_dir=/workspace
ENV REPO_ROOT=${workspace_dir} \
    EXTERNAL_DIR=${workspace_dir}/external \
    MODULES_DIR=${workspace_dir}/modules \
    LIB_DIR=${workspace_dir}/lib \
    LIB_CPP_DIR=${workspace_dir}/lib/cpp

RUN apt-get update && apt-get install -y \
    binutils \
    build-essential \
    bzip2 \
    bash-completion \
    gnupg \
    ca-certificates \
    software-properties-common \
    curl \
    valgrind \
    ccache \
    cmake \
    wget \
    git \
    g++ \
    g++-14 \
    gcc-14 \
    gcc-multilib \
    gdb \
    gnupg \
    iproute2 \
    jq \
    libssl-dev \
    libx11-dev \
    libc6-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxi-dev \
    libxext-dev \
    libxfixes-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    lsof \
    lsb-release \
    libgtk-3-dev \
    libwayland-bin \
    libwayland-dev \
    wayland-protocols \
    mesa-utils \
    libxkbcommon-dev \
    make \
    net-tools \
    ninja-build \
    rsync \
    perl \
    zip \
    sed \
    sudo \
    openssh-server \
    python3 \
    python3-pip \
    tar \
    vim \
    unzip \
    patch \
    pkg-config \
    mono-complete \
    x11-apps \
    zlib1g-dev \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100 \
    && update-alternatives --set gcc /usr/bin/gcc-14 \
    && update-alternatives --set g++ /usr/bin/g++-14

RUN apt-get clean -y && \
    apt-get autoremove --purge -y && \
    rm -rf /var/lib/apt/lists/*

ARG clang_version=22
RUN wget -q "https://apt.llvm.org/llvm.sh" && \
    chmod +x llvm.sh && \
    ./llvm.sh ${clang_version} && \
    apt-get install -y clang-format-${clang_version} clangd-${clang_version} clang-tidy-${clang_version} && \
    ln -sf /usr/bin/clang-format-${clang_version} /usr/local/bin/clang-format && \
    ln -sf /usr/bin/clangd-${clang_version} /usr/local/bin/clangd && \
    ln -sf /usr/bin/clang-tidy-${clang_version} /usr/local/bin/clang-tidy && \
    rm ./llvm.sh

RUN usermod -aG sudo,dialout ${user}
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

WORKDIR /workspace
RUN chown -R ${user}:${user} /workspace

USER ${user}

# Rust installation.
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | \
    sh -s -- \
    -y \
    --default-toolchain stable \
    --profile default \
    --no-modify-path

CMD ["/bin/bash"]
