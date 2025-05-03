# STM32 Fluid Simulation – Engineering Thesis Project

This repository contains the code and documentation for a mobile embedded device prototype that displays a water simulation responsive to accelerometer input. The project was developed as part of my engineering thesis.

## Project Overview

The device simulates a fluid motion effect using a simplified **FLIP (Fluid-Implicit Particle)** method, inspired by systems like *SideFX Houdini* and *FLIP Fluids* for Blender.

The simulation responds in real time to orientation data provided by a 3-axis accelerometer. 

## Hardware

- **Microcontroller:** STM32F103C6T6 (overclocked to 128 MHz)
- **Display:** 8x8 LED matrix (788AS) with CD4514 and CD4515 decoders
- **Accelerometer:** MPU6050 (I2C interface)
- **Power Supply:** 9 V battery regulated to 3.3 V via LM2596 buck converter

### Display Architecture

The LED matrix is configured so that any pixel can be selected with a single write to the `GPIO->ODR` register. This setup allows for cyclic DMA to update the display asynchronously, freeing up CPU resources for the simulation.

## Features

- Real-time response to tilt using accelerometer data
- Simplified FLIP-based water simulation
- Asynchronous display updates via DMA

## Future Work

- Design and manufacture a custom PCB
- Miniaturize the entire system
- Upgrade the simulation
- Replace the battery with a rechargeable accumulator
- Minimize power usage

## Demo

A video demonstrating the prototype in action: 
https://github.com/user-attachments/assets/38a17604-264b-4c63-8f16-0da452b18bbc
