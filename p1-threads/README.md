# project 1. threads

`pontos/src/devices/timer.c`의 `timer_sleep()` 함수는 default로 **busy-wating** sleep mode로 설정이 되어있다.

따라서,  CPU 자원 활용을 위해 **blocked** 모드로 개발하는 것이 목표.