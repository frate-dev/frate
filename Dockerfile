FROM ubuntu:latest


WORKDIR /frate
# Install deps

RUN apt-get update && apt-get install -y \
    wget \
    gnupg \
    software-properties-common

# add llvm repo
RUN bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"

RUN add-apt-repository ppa:ubuntu-toolchain-r/test


RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    clang-17 \
    ccache \
    lua5.4 \
    liblua5.4-dev \
    libgit2-dev 

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 13 --slave /usr/bin/g++ g++ /usr/bin/g++-13
RUN wget -O lua.tar.gz https://www.lua.org/ftp/lua-5.4.6.tar.gz && \
      tar -xzf lua.tar.gz && \
      cd lua-5.4.6 && \
      make linux test && \
      make install


# set clang as default compiler
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-17 100

RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-17 100

RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang 9001

RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 9001


# get frate from github
COPY ./frate .


# build frate

##RUN cmake -DCMAKE_BUILD_TYPE=Debug ./ && make -j20




