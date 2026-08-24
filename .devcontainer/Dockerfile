FROM ubuntu:24.04

ARG USERNAME=user
ARG UID=1000
ARG GID=1000
ENV DEBIAN_FRONTEND=noninteractive \
    PATH=/home/${USERNAME}/.cargo/bin:$PATH \
    LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8 \
    RUSTUP_HOME=/home/${USERNAME}/.rustup \
    CARGO_HOME=/home/${USERNAME}/.cargo

ARG WORKSPACE=/workspace
ENV REPO_ROOT=${WORKSPACE} \
    EXTERNAL_DIR=${WORKSPACE}/external \
    MODULES_DIR=${WORKSPACE}/modules \
    LIB_DIR=${WORKSPACE}/lib \
    LIB_CPP_DIR=${WORKSPACE}/lib/cpp

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

ARG CLANG_VERSION=22
RUN wget -q "https://apt.llvm.org/llvm.sh" && \
    chmod +x llvm.sh && \
    ./llvm.sh ${CLANG_VERSION} && \
    apt-get install -y clang-format-${CLANG_VERSION} clangd-${CLANG_VERSION} clang-tidy-${CLANG_VERSION} && \
    ln -sf /usr/bin/clang-format-${CLANG_VERSION} /usr/local/bin/clang-format && \
    ln -sf /usr/bin/clangd-${CLANG_VERSION} /usr/local/bin/clangd && \
    ln -sf /usr/bin/clang-tidy-${CLANG_VERSION} /usr/local/bin/clang-tidy && \
    rm ./llvm.sh

# Create user account.
RUN <<EOF
	# Remove 'ubuntu' user to free UID 1000.
	userdel -r ubuntu || true

	# Create user group and user account.
	groupadd -g ${GID} -o ${USERNAME}
	useradd -u ${UID} -m -g ${USERNAME} -G plugdev ${USERNAME}

	# Add user account to sudoers.
	echo ${USERNAME} ' ALL = NOPASSWD: ALL' > /etc/sudoers.d/${USERNAME}
	chmod 0440 /etc/sudoers.d/${USERNAME}
EOF

# Bash completion script.
ADD bash_completion /home/${USERNAME}/.bash_completion
RUN mkdir -p /home/${USERNAME}/.bash_completion.d

WORKDIR ${WORKSPACE}
RUN chown -R ${USERNAME}:${USERNAME} ${WORKSPACE} /home/${USERNAME}

# Silence annoying warnings when running GUI applications inside the container.
RUN mkdir -p /tmp/runtime-root && \
    chown ${USERNAME}:${USERNAME} /tmp/runtime-root && \
    chmod 700 /tmp/runtime-root

COPY --chmod=0755 scripts/ /usr/local/bin/

USER ${USERNAME}

# Rust installation.
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | \
    sh -s -- \
    -y \
    --default-toolchain stable \
    --profile default \
    --no-modify-path

CMD ["/bin/bash"]
