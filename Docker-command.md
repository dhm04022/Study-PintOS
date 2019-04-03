# Docker Command 정리

* `docker pull <ImageName>` : 도커 이미지 다운
* `docker run -it --name <name> <ImageID>` : 이미지를 사용하여 컨테이너 생성 (실행도 동시에 됨)
* `docker start <Container ID or Name>` : 컨테이너 실행
* `dcoker attach <Container ID or Name>` : 컨테이너 배쉬 접속 (tty 같이)
* 내부에서 `exit` : 종료
* `docker images` : 현재 풀 받은 이미지 리스트
* `docker ps -a` : 모든 컨테이너 리스트 (죽은 것 포함)
* `docker exec -it <Container ID or Name> /bin/bash` : 여러 셸(배쉬) 만들기
