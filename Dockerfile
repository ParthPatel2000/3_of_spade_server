FROM debian:stretch-slim

# Install dependencies for MySQL repo (lsb-release and wget)
RUN apt-get update && apt-get install -y \
    wget \
    lsb-release 

#install the mysql repo
RUN wget https://dev.mysql.com/get/mysql-apt-config_0.8.10-1_all.deb
RUN dpkg -i mysql-apt-config_0.8.10-1_all.deb

# Install MySQL
RUN apt-get update && apt-get install -y \
    libmysqlclient-dev 

#install build tools
RUN sudo apt-get install -y \
    build-essential \
    cmake \
    openssh-server \
    libssl-dev \
