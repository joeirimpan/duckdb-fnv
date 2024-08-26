# start from an ubuntu image
FROM quay.io/pypa/manylinux2014_x86_64

# Install dependent packages
RUN yum update && \
    yum install -y \
    git \
    ninja-build \
    ccache \
    unixODBC \
    python \
    awscli

# Set build environment variables
ENV EXTENSION_CONFIGS './duckdb/.github/config/bundled_extensions.cmake'
ENV ENABLE_EXTENSION_AUTOLOADING 1
ENV ENABLE_EXTENSION_AUTOINSTALL 1
ENV GEN 'ninja'
ENV BUILD_BENCHMARK 1
ENV BUILD_ODBC 1
ENV DEBUG_STACKTRACE 1
ENV FORCE_WARN_UNUSED 1
ENV DUCKDB_RUN_PARALLEL_CSV_TESTS 1

# commands to checkout your repo
COPY . .
WORKDIR .
RUN git checkout main

# Build
RUN make
