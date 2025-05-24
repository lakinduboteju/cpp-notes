FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      build-essential \
      cmake \
      gdb \
      git gnupg openssh-client \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace