# Git 커맨드 정리

## 참고 자료

* [opentutorials : 지옥에서 온 git](https://opentutorials.org/course/2708)
* [github 블로그 : git 간편 안내서](https://rogerdudler.github.io/git-guide/index.ko.html)

## command 정리

* `git status` : 현재 깃 스테이징 파일 확인
* `git log` : 깃 커밋 로그 확인
* `git add <file_name>` : 추적 안된 파일 스테이징
* `git add .` : 현재 폴더 기준 추적안된 모든 파일 스테이징
* `git commit` : 스테이징 파일 로컬 깃에 커밋(등록)
* `git push <repository_name> <branch_name>` : 원격 저장소(github 등..)에 로컬 커밋상황 푸쉬

    * 나중에 교수님의 원격 저장소를 열어주신다고 함.
    * `<repository_name>`은 `git remote -v`로 확인 가능
    * `git clone <url>`로 프로젝트 생성시 자동적으로 `origin` repository 등록

