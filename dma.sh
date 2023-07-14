#!/bin/bash

# gain admin priv
sudo su

# modify max number of queues
echo 8 > /sys/bus/pci/devices/0000\:01\:00.0/qdma/qmax

# create queues for writing to SBI control registers and stream
dma-ctl qdma01000 q add idx 0 mode mm dir h2c
dma-ctl qdma01000 q add idx 1 mode mm dir h2c

# start queues
dma-ctl qdma01000 q start idx 0 dir h2c
dma-ctl qdma01000 q start idx 1 dir h2c aperture_sz 4096


