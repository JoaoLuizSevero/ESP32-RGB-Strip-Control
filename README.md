# ESP32 RGB Strip Control

## About

This is a **RGB strip control** using a **ESP32** (NODEMCU module) for addressable LEDs. Can set the number of LEDs and the **RGB** color to each one. The **ESP32** create an **access point**, then can be accessed via **192.168.4.1**. 

### NODEMCU
![NODEMCU](https://github.com/JoaoLuizSevero/RGBControl_template/blob/main/RGBControl/assets/node.PNG)

### RGB Strip
![Strip](https://github.com/JoaoLuizSevero/RGBControl_template/blob/main/RGBControl/assets/strip.PNG)

## WebPage

The page was made using **HTML, CSS and JavaScript**, and communicates to the **ESP32** with **WebSocket** sending **JSON** payloads.

### Page
![Page](https://github.com/JoaoLuizSevero/RGBControl_template/blob/main/RGBControl/assets/page.PNG)

### Tests
![Tests](https://github.com/JoaoLuizSevero/RGBControl_template/blob/main/RGBControl/assets/pic.jpeg)

## Comments

The page limits the number of LEDs at **10** just because i was feeding the strip directly at the **NODEMCU**, but can perfectly be increased.  
