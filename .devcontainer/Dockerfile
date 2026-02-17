ARG UBUNTU_VERSION=24.04
ARG GCC_VERSION=14
ARG LLVM_VERSION=17

FROM docker.io/library/ubuntu:${UBUNTU_VERSION} AS base

ARG UBUNTU_VERSION
ARG GCC_VERSION
ARG LLVM_VERSION

RUN apt-get update && apt-get install -y locales && rm -rf /var/lib/apt/lists/* \
	&& localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

ENV LANG en_US.utf8
ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /tmp

RUN apt-get update && apt-get install -y --no-install-recommends \
    gnupg2 ca-certificates software-properties-common \
    cppcheck valgrind ccache cmake wget git nano jq \
    # Required by Visual Studio
    gdb make ninja-build rsync zip sudo \
    # Required by VCPKG
    openssh-server tar curl unzip pkg-config bash-completion aria2 mono-complete \
    # Required by LLVM
    lsb-release zlib1g-dev

# +-----------------------------+
# | CMake                       |
# +-----------------------------+
RUN version="$(curl -s https://api.github.com/repos/Kitware/CMake/releases/latest | jq -r '.tag_name' | cut -c 2-)"; echo "Latest Version: $version" && \
    url="https://github.com/Kitware/CMake/releases/download/v$version/cmake-$version-linux-x86_64.sh"; echo "Download URL: $url" && \
    wget -q $url && \
    mkdir /opt/cmake && \
    sh cmake-$version-linux-x86_64.sh --prefix=/opt/cmake --skip-license && \
    ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake && \
    rm -rf cmake-$version-linux-x86_64.sh

# +-----------------------------+
# | GCC                         |
# +-----------------------------+
RUN add-apt-repository universe && \
    apt-get update && apt-get install -y --no-install-recommends \
    software-properties-common \
    libmpfr-dev libgmp3-dev libmpc-dev \
    gcc-${GCC_VERSION} g++-${GCC_VERSION}

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} 100

RUN apt-get autoremove -y libmpfr-dev libgmp3-dev libmpc-dev

# +-----------------------------+
# | LLVM                        |
# +-----------------------------+
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh ${LLVM_VERSION} all && \
    rm ./llvm.sh

ENV PATH="/usr/lib/llvm-${LLVM_VERSION}/bin:/usr/lib/llvm-${LLVM_VERSION}/include:${PATH}"
ENV LD_LIBRARY_PATH="/usr/lib/llvm-${LLVM_VERSION}/lib:${LD_LIBRARY_PATH}"

RUN ln -s /usr/bin/clang-${LLVM_VERSION} /usr/bin/clang && \
    ln -s /usr/bin/clang++-${LLVM_VERSION} /usr/bin/clang++ && \
    ln -s /usr/bin/clang-format-${LLVM_VERSION} /usr/bin/clang-format

ENV CC=/usr/bin/clang \
    CXX=/usr/bin/clang++

RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100 && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100

# +-----------------------------+
# | CLEANUP                     |
# +-----------------------------+
RUN apt-get autoremove -y && \
    apt-get clean autoclean && \
    rm -rf /var/lib/apt/lists/* \
    rm -rf /tmp/*

ENV LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8

WORKDIR /code-samples
