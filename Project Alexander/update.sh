#!/bin/bash

# Update the package list
sudo apt update

# Install the latest kernel version
sudo apt install -y linux-generic

# Reboot the system to apply the new kernel
sudo reboot
