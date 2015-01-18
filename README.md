liblxc-node
===

### Currently under development.

<!-- Badges will go here -->

Node bindings to liblxc

# Installation

        npm install liblxc

## Requirements

A recent version of `lxc-dev` is required to compile and `lxc` is required at runtime.

On Ubuntu:

        apt-get install lxc lxc-dev

On Debian:

        sudo apt-get install automake
        curl -L https://github.com/lxc/lxc/archive/lxc-1.0.7.tar.gz | tar -xzv
        cd lxc-lxc-1.0.7
        ./autogen.sh
        ./configure
        make
        sudo make install
