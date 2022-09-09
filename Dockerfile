FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow

RUN apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y install clang-12 python3 python3-pip cmake && \
    mkdir ai_service && \
    useradd -m -s /bin/bash builder && \
    chown -R builder:builder /ai_service && \
    chmod -R u+rwx /ai_service

USER builder

ENV PATH=/home/builder/.local/bin:$PATH
ENV CC=/usr/bin/clang-12
ENV CXX=/usr/bin/clang++-12

RUN pip install conan

COPY conanfile.txt ./ai_service/conanfile.txt


RUN conan profile new default --detect && \
    conan profile update settings.compiler=clang default && \
    conan profile update settings.compiler.version=12 default && \
    conan profile update settings.build_type=Release default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default

WORKDIR /ai_service

RUN mkdir conan-libs \
    && cd conan-libs \
    && conan install --build=missing ..

COPY CMakeLists.txt /ai_service/
COPY src/ /ai_service/src/

RUN mkdir /ai_service/build/

WORKDIR /ai_service/build/

RUN cmake /ai_service/ && \
    cmake --build .

WORKDIR /ai_service

EXPOSE 3000

CMD ["/ai_service/build/bin/ai_service"]