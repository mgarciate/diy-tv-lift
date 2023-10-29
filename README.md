# diy-tv-lift
Using an Arduino UNO, activate a servo motor via a switch to raise/lower a TV. The motion stops when the limit switch is triggered, ensuring safe operation. The motor drive manages the servo's power and movement direction.

## Components
- 1x Arduino UNO
- 1x switch
- 1x limit switch
- 1x motor driver shield
- 1x servo motor

## PINs
- **6** -> Motor direction
- **7** -> Motor speed
- **10** -> Switch
- **11** -> Limit switch

## Circuit design

![Circuit design](screenshots/arduino.png?raw=true)

## GH Action
- https://github.com/arduino/compile-sketches

## References
- https://blog.arduino.cc/2021/04/09/test-your-arduino-projects-with-github-actions/
