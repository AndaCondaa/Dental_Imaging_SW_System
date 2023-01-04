<h2>1차 - 12/13(화)</h2>

<h4>1. 회의 주기 및 코드 리뷰 주기 선정</h4>

- 회의 주기는 우선 주 1회로 선정
- 코드 리뷰 주기는 주 1~2회, 필요에 따라 추가 혹은 삭제

<h4>2. 모듈별 명세 정리</h4>

- 환자관리 SW : 환자정보 관리, 네트워크 서버 구축, DB 구축
- 촬영 SW : CT SW 제어, RAW 데이터 전처리
- CT 시뮬레이터 : 장비구동 3D 구현 (VTK, OpenGL)
- 영상뷰어 SW : 이미지 데이터 뷰어 구현 및 이미지 프로세싱

<h4>3. 모듈 수요 조사</h4>

- 안다미로 : 촬영 SW, 영상뷰어 SW
- 김유선 : 영상뷰어 SW, 환자관리 SW
- 박병규 : CT 시뮬레이터
- 이정연 : 영상뷰어 SW, 촬영 SW

<h4>4. 다음 회의 일정</h4>

- 전체 프로젝트 구성에 대한 청사진 완성
- 모듈 선택에 대한 의견 정리 및 확정
- 날짜 : 12/17(토)  

  <br>

  <br>

  <br>

  <br>

<h2>2차 - 12/17(토)</h2>

<h4>1. 자료 조사 공유</h4>

- DB와 관련하여, DB-서버-클라이언트 관계를 명확히 할 필요가 있음
- 촬영SW에서 MITK라는 툴킷을 이용하면 좋을 것으로 예상
- One2 제품 관련 : https://www.youtube.com/watch?v=sUYMXA47qPI
- One3 제품 관련 : https://www.youtube.com/watch?v=x1visphKCKU&t=1s
- 참고용 프로젝트 : https://www.youtube.com/watch?v=etaVUq96QHs
- T2 설명 : https://www.youtube.com/watch?v=oZkie866IMc

<h4>2. 모듈 선정</h4>

- 환자관리 SW - 김유선
- 촬영 SW - 안다미로
- CT 시뮬레이터 - 박병규
- 영상뷰어 SW - 이정연

<h4>3. 다음 회의 일정</h4>

- 각자 맡은 SW와 관련하여, 기능 명세 정리 (필요한 기능과 해당 기능을 구현하기 위해 필요한 기술들에 대해서도 조사)
- 기능 명세 정리된 것을 통해 주차별 계획 작성
- 날짜 : 12/19(월) 18:00 

<br>

<br>

<br>

<br>

<h2>3차 - 12/19(월)</h2>

<h4>1. 프로젝트 계획서에 대한 세부항목 선정 및 분배</h4>

- 프로젝트 계획서의 목차(가안)를 다음과 같이 설정

  1. 개요

     1-1. 프로젝트 명

     1-2. 프로젝트 기간

     1-3. 프로젝트 목적

     1-4. 프로젝트 기대효과

  2. 프로젝트 범위

     2-1. 작업 명세서

  3. 프로젝트 추진체계

     3-1. 역할 분담표

  4. 시스템 구축 환경

     4-1. 하드웨어 구성도

     4-2. 시스템 아키텍쳐 모형 (기능 구조 다이어그램)

     4-3. UI 구성

  5. 프로젝트 작업 및 관리 계획

     5-1. 프로젝트 작업 계획

     5-2. 산출물 검수 계획

     5-3. SW 테스트 계획

     5-4. 프로젝트 보고 계획

- 프로젝트 계획서 작성 분배
  - 김유선 : 1, 4-1, 4-2
  - 박병규 : 4-3
  - 안다미로 : 5
  - 이정연 : 2, 3 (1 보조)

- 참고사항
  1. 개발배경 및 목적 : 개발하는 이유, 목적, 배경, 동기에 대한 소개
  2. 개발환경 및 개발언어 : PC, 개발언어에 대한 설명 , 사용 라이브러리에 대한 표기 
  3. 시스템 구성 및 아키텍처 : 구조도를 제시 하시고자 하는경우 이미지로 대체가능, Text 설명으로도가능 
  4. 주요기능 : 개발물의 기능별 상세설명, 화면예상에 따른 이미지 제공 가능
  5. 프로젝트 추진 일정 : 개발일정에 대한 소개   
  6. 기대효과 및 활용분야 : 프로젝트가 활용되어 질 수 있는 분야, 상황, 산업에 대한 소개 및 기대효과
  7. 기타 : 프로젝트의 독창성 혹은 특장점에 대한 소개
  기타 참고문헌정보 기재 : 계획서 작성시 참고한 문서가 있을경우 기재 
- 참고자료
  - [프로젝트 계획서 작성 가이드](http://www.swbank.kr/html/pdf/sample/project_plan_guide.pdf)
  - [소프트웨어 개발 프로세스](https://pangtrue.tistory.com/category/Software%20Engineering)

- 1차 제출기한
  - 12/21(수) 21:00
  - 1차 취합이므로, 시간이 부족할 경우 전체적인 흐름만 작성

<h4>2. 다음 회의 일정</h4>

- 프로젝트 계획서 보충에 대한 회의 예정

- 날짜: 미정
- 강사님 1차 피드백 이후 회의 진행

<br>

<br>

<br>

<br>

<h2>4차 - 12/22(목)</h2>

\- 최우선 과제: 시스템 구현과 모듈 간 연계

 

**●** **작업명세서**

1. 환자 SW: 멘토님 PPT의 개발 예시 ui를 따른다고 했을 때, 촬영 버튼을 눌렀을 때 촬영 SW를 띄울 것인지, 촬영 시작이 되도록 만들 것인지 정해야 함

2. 촬영 SW & CT Simulation SW 
   - 영상 재구성 부분이 5번에서 팀 해야하는 것이 맞는지 논의 필요. Image Processing 팀에서 해야할 부분이라 Volume이 너무 큼
   - 촬영 종료 버튼은 3번 CT Simulation SW에 있어야 할 듯함
   - 촬영 시작 버튼을 클릭했을 때 3번 CT Simulation SW에서 촬영이 시작된다는 알림이 뜨도록 하면 좋을 것 같음

 3. 뷰어 SW: 

    - 2D 픽셀 단위의 영상처리가 들어갈 것

      - Qt에서 QGraphicsEffect Class를 사용하면 Blur, Colorize, DropShadow, Opacity 효과를 손쉽게 줄 수 있으니 참고

      - OpenGL의 쉐이더를 사용해 효과를 추가할 수도 있을 것

    - 이미지만 띄울 것이 아니라 환자 정보도 떠야 함(환자 성명, 성별, 생년월일, 환자 특이사항 정도만 떠도 괜찮지 않을까 싶음) -> 1번 환자SW와의 유기적 연결 필요
    - LineEdit를 사용해 의사가 진료 중 메모기능을 사용할 수 있도록. 이 메모 데이터는 1번 환자SW에도 저장되어야 함
    - 3D 구현은 가능하다면 좋긴 하겠지만 다른 팀이 맡게 될 프로젝트에 해당되는 부분이고, 본 프로젝트의 목적은 ‘치과 영상 SW 시스템 구축’이므로 PPT의 영상 시스템 구조를 확인한 후 시스템 구축에 집중하는 방향으로
    - 1번 환자SW에서 환자가 접수되면 뷰어 쪽에서 환자명과 함께 촬영 준비의 필요성을 알리는 알람이 뜨도록 하면 좋지 않을까 싶음

 

**●** **계획서**

1. 시스템 구조의 큰 틀은 PPT에 설계해주신 부분 그대로 가져와도 될 것

2. 메인/보조라고 계획서에 직접적으로 명시하기 보다는 살짝 언급만 해주는 편이 나을 것 같음
   - 간트차트나 최종 발표 피피티에서 살짝 언급만 하고 넘어가면 되지 않을까 싶음

3. 책임중심설계를 따라 계획서 작성할 것 
   - 환자 DB: 사무 업무를 담당하는 직원이나 간호사들이 사용할 데이터의 저장장소
   -  영상 DB: 의사가 사용할 데이터의 저장장소

4. 서버
   - 1대를 두고, 하나의 서버 안에 데이터를 보내는 채널, 받는 채널을 둘 것(스레드, 포트를 따로 설정)

5. 젯슨나노는 촬영SW에 사용

6. 소스코드 형식과 형상관리 방안, 사용할 협업툴도 계획서에 모두 정리

 

**●** **DICOM** **포맷**

1. DICOM 포맷에 모든 부분을 맞추어야 할 필요는 없을 듯함(아마도 CT Simulation쪽의 raw data들). 다만 재구성된 촬영이미지는 반드시 DICOM 포맷을 준수할 수 있도록 할 것
2. 다만 Frame 데이터를 법으로 보존해야 하는 것인지 확실히 알아야 함! (재구성된 하나의 data만을 저장할 것인지 raw data들 전부를 저장할 것인지 관련 법규에 대해 알아본 후 결정할 것) <br>DICOM 포맷에 맞는 라이브러리를 처음부터 구현하려고 하기보다는 DICOM C++ 라이브러리를 사용할 것 – 프로젝트명에 DICOM이라는 것을 명시할 필요까지는 없고, DICOM보다는 System 구축과 연계에 중점 두기

 

**아이디어 추가** **예시**

: 처방할 약 의사가 선택하면 성분, 복용량, 용량 등의 정보가 뜰 수 있도록 만들기 -> 사용자의 편의성을 고려한 UI/UX를 만들 것

 

 

**●** **멘토님께 여쭤볼 부분 – 애매한 부분이 있으면 여쭈어 볼 것!**

1. 영상 재구성이 이 프로젝트에서 중요한 부분인지

2. DICOM을 적용하려고 하는데 어떤 의견이신지

 



<h4>**일정**</h4>

1. 12/28~29까지는 계획서 마무리해서 멘토님께 전송
   
2. 계획서에서 4번 시스템 구축 환경 빠르게 작성. 특히 DB 테이블 같은 경우에는 빠르게 마무리되어야 어떤 데이터를 사용하게 될 지, 화면 구성은 어떻게 나올지 결정됨. ERD도 함께 작성하기

3. 마지막 주는 임원면접 준비에 집중

<br>

<br>

<br>

<br>

<h2>5차(막간 회의) - 12/23(금)</h2>

1. 환자의 간략 정보는 4개의 모듈에 모두 들어가야함

2. 뷰어 – One2같은 프로그램상에서 뷰어가 어떤식의 ui를 가지고 있는지

3. 환자관리 – 차트 ui 부분 확인

4. 기능 추가하고 싶은 부분 얘기하기

5. 촬영 sign에 대한 부분도 생각하기(관리->촬영)