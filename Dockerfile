FROM ubuntu:22.04 AS dev

# cpp dev
RUN apt-get update -y
RUN apt-get install gcc -y
RUN apt-get install g++ -y
RUN apt-get install build-essential -y
RUN apt-get install ninja-build -y

# install cmake
RUN \ 
    sh -c "$(curl -fsSL https://apt.kitware.com/kitware-archive.sh)" && \
    apt update && \
    apt install -y cmake

# get systemc
ARG SYSTEMC_VERSION=2.3.3

ADD https://www.accellera.org/images/downloads/standards/systemc/systemc-${SYSTEMC_VERSION}.tar.gz ./

RUN \
    # mkdir -p systemc_build && \
    tar -xzf systemc-${SYSTEMC_VERSION}.tar.gz && \
    cmake \
            -G Ninja \
            -S systemc-${SYSTEMC_VERSION} \ 
            -B systemc_build \
            -D CMAKE_CXX_STANDARD=17 \
            -D CMAKE_BUILD_TYPE=RelWithDebInfo \
            -D CMAKE_INSTALL_PREFIX=/usr/local/lib/cmake/SystemC && \
    cmake --build systemc_build -j $(nproc) && \
    cmake --install systemc_build

# add dev user
FROM dev as dev-container

ARG USERNAME=dev
ARG USER_UID=1007
ARG USER_GID=$USER_UID
RUN \ 
    groupadd --gid $USER_GID $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -m $USERNAME && \
    apt-get update && \
    apt-get install -y sudo && \
    echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME

# ENV LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}

# entrypoint
ENTRYPOINT ["/bin/bash"]