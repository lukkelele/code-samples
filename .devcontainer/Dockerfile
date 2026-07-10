FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV DISPLAY=:0
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

ARG user=ubuntu
ENV RUSTUP_HOME=/home/${user}/.rustup
ENV CARGO_HOME=/home/${user}/.cargo
ENV PATH=/home/${user}/.cargo/bin:$PATH

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
    lsb-release \
    libgtk-3-dev \
    libwayland-bin \
    libwayland-dev \
    wayland-protocols \
    mesa-utils \
    libxkbcommon-dev \
    make \
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

RUN wget -q "https://apt.llvm.org/llvm.sh" && \
    chmod +x llvm.sh && \
    ./llvm.sh 19 && \
    apt-get install -y clang-format-19 clangd-19 clang-tidy-19 && \
    ln -sf /usr/bin/clang-format-19 /usr/local/bin/clang-format && \
    ln -sf /usr/bin/clangd-19 /usr/local/bin/clangd && \
    ln -sf /usr/bin/clang-tidy-19 /usr/local/bin/clang-tidy && \
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
