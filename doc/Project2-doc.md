# project2. user program

프로젝트 진행용 정리 문서입니다.

## Goal

User Program을 PintOS에서 실행될 수 있도록 해주는 것.

* 이때까지는 수정된 코드를 커널과 같이 바로 컴파일했다.
* 커널의 Top에 유저 프로그램을 올려보자!
* 하나 이상의 프로세스가 수행되어야 한다.

## To do list

### 1. **Argument Passing and Setup Stack**

> * 프로그램 파일 이름을 Parsing 하고, 스택을 세팅해야한다.
> * `src/userprog/process.c` 를 주로보자.

### 2. **System Calls**

> * 요구한 각 시스템 콜 함수들을 구현해야 한다.
> * `src/userprog/syscall.c` 에 구현해야 할 함수들이 있다.

## 1. Argument Passing and Setup Stack

