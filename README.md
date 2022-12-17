# Dental_Imaging_SW_System

<br>

<h2>Git Management Rules</h2>

1. Branch

   - Default Branch는 main으로 합니다.

   - 각자 자신의 Branch를 Local에서 생성하여 작업합니다.
   - branch명은 지정된 형식을 이용합니다.
     - 김유선 : kim
     - 박병규 : park
     - 안다미로 : an
     - 이정연 : lee
   - 필요에 따라서 추가적인 branch명이 필요한 경우, 지정된 형식에 '_(언더바)'를 추가한 형식으로 분기하여 사용합니다. (예: an_server, an_client)

2. Merge

   - 초반에는 각 SW가 독립적으로 작업될 것이기에 수시로 main 브런치에 대한 Merge가 진행될 필요는 없습니다.
   - 단, 서버의 경우는 전체 시스템에 영향을 주기 때문에, 서버 수정 건과 관련한 내용은 수시로 Pull Request를 통해 Merge 요청을 보내시길 바랍니다.
   - 'Required number of approvals before merging = 1'으로 설정했기 때문에, Pull Request를 승인하는 팀원은 반드시 다른 팀원에게 $ git pull origin main 요청을 하시기 바랍니다. (안전한 관리를 위해 되도록이면 팀장이 Pull Request의 승인을 진행하는 방향으로 하겠습니다.)
   - 충돌이 없도록, git pull 요청을 받은 팀원들은 가능한 즉시 git pull을 진행하시길 바랍니다.

3. Commit Message

   - 다음과 같은 이유로 인해 Commit Message에 대한 규칙을 정합니다.
     - 커밋사항에 대한 preview
     - 효과적인 진행상황 관리
     - 과거 코드 추적의 용이성
   - 'date:  type :  contents' 형식으로 커밋 메세지를 구성합니다. (예: 1218 : feat : Add login widget)
   - date는 해당 커밋을 올리는 날짜로, mmdd형태로 작성합니다. (예: 09월 01일 -> 0901)
   - type은 아래 정의된 내용 중 해당되는 내용으로 작성합니다. 
     - feat : 새로운 기능에 대한 커밋
     - 



<br><br>


<h2>Reference</h2>

- [서버,네트워크,데이터베이스 이해](https://www.youtube.com/watch?v=Pc6n6HgWU5c&list=LL&index=2)
- [앱과 데이터베이스를 직접 연결하지 않는 이유](https://www.youtube.com/watch?v=L7yW9OWTNP8&list=LL&index=1)
- [One2 SW 소개 영상](https://www.youtube.com/watch?v=sUYMXA47qPI)

- [One3 SW 소개 영상](https://www.youtube.com/watch?v=x1visphKCKU&t=1s)
- [참고용 프로젝트](https://www.youtube.com/watch?v=etaVUq96QHs)
- [T2 제품 소개 영상](https://www.youtube.com/watch?v=oZkie866IMc)

- [ITK를 이용한 삼차원 흉부 CT 영상분석 및 폐 결절 검출 시스템](https://www.slideshare.net/choiwookjin/wj-choi-itk-lung-image-analysis-cad)
