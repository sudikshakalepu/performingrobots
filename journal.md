# Project Journal – nRF24 Communication Setup

**Date:** 11/6/2025

## Hardware Used

* Arduino Uno (Transmitter)
* Arduino Mega (Receiver)
* nRF24L01 Wireless Modules (x2)
* 16x2 LCD Display
* Push Buttons (5 total)

## Objective

To establish wireless communication between two Arduinos using the nRF24L01 modules, with one functioning as a transmitter and the other as a receiver, following the instructor’s provided documentation.

## Progress Summary

### Initial Setup

Both the transmitter and receiver were configured according to the assignment instructions. The shared `.ino` file was modified by uncommenting the relevant sections marked with `CHANGEHERE` to switch between transmitter and receiver modes.

### Receiver Configuration

The receiver code was uploaded to the Arduino Mega. After adjusting CE and CSN pin definitions, the receiver successfully initialized the radio and began listening for incoming packets.

### Transmitter Configuration

The transmitter was uploaded to the Arduino Uno, with CE and CSN pins defined on A4 and A5, respectively. The LCD displayed “I am a transmitter” after successful initialization.

## Images

![LCD Display 1](/images/lcd1.jpg)

![LCD Display 2](/images/lcd2.jpg)


