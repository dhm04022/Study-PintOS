# Study-PintOS
운영체제 수업, Docker 환경을 활용한 PintOS 실습 과제

1. [project1. threads](./src/threads/DESIGNDOC)


## 1. project1. threads
`pontos/src/devices/timer.c`의 `timer_sleep()` 함수는 default로 **busy-wating** sleep mode로 설정이 되어있다.

따라서,  CPU 자원 활용을 위해 **blocked** 모드로 개발하는 것이 목표.

[-> DESIGNDOC](./src/threads/DESIGNDOC)

### result

요구사항 통과 

![](doc/img/p1-threads-log01.png)
