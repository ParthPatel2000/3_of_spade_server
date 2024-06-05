FROM debian:stable-slim

# Install dependencies for MySQL repo (lsb-release and wget)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    lsb-release \
    gnupg \
    debconf-utils\
    libboost-all-dev \
    libssl-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Preconfigure the MySQL APT repository package to run non-interactively
RUN echo "mysql-apt-config mysql-apt-config/select-server select mysql-8.0" | debconf-set-selections && \
    echo "mysql-apt-config mysql-apt-config/select-product select Apply" | debconf-set-selections

# Install the MySQL repo
# Update the package list to include MySQL packages
# Install MariaDB client development libraries for MySQL compatibility
# Clean up APT when done to reduce image size
RUN wget https://repo.mysql.com//mysql-apt-config_0.8.30-1_all.deb && \
    DEBIAN_FRONTEND=noninteractive dpkg -i mysql-apt-config_0.8.30-1_all.deb\
    && apt-get update && apt-get install -y libmysqlclient-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*\
    && rm mysql-apt-config_0.8.30-1_all.deb


