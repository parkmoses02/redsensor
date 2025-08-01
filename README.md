# RedSensor 프로젝트

## 📖 프로젝트 소개
tcs34725 센서를 활용하여 색상을 감지하는 프로젝트입니다.

## 🚀 팀원을 위한 GitHub 사용 가이드

### 1️⃣ 처음 시작하기 (한 번만 하면 됨)

#### Git 설치 및 설정
 Git 사용자 정보 설정 (각자 본인 정보로 변경)
```bash
git config --global user.name "본인이름"
git config --global user.email "본인이메일@gmail.com"
```

#### 프로젝트 클론하기
프로젝트를 내 컴퓨터로 복사
```bash
git clone https://github.com/parkmoses02/redsensor.git
cd redsensor
```

### 2️⃣ 작업 시작하기 (매번 작업할 때)

#### 최신 코드 받기
```bash
# 항상 작업 전에 최신 코드를 받아오세요!
git checkout develop
git pull origin develop
```

#### 팀원별 브랜치로 작업하기
각 팀원은 본인 브랜치에서 작업하세요:
- **권기혁**: `feature/권기혁`
- **손지민**: `feature/손지민` 
- **윤세희**: `feature/윤세희`
- **phs**: `feature/phs`

```bash
# 본인 브랜치로 이동 (예: 권기혁의 경우)
git checkout feature/권기혁

# 작업 후 변경사항 확인
git status

# 변경된 파일들 추가
git add .

# 커밋 (메시지는 한글로 명확하게)
git commit -m "센서 데이터 수집 기능 추가"

# GitHub에 올리기 (본인 브랜치명으로 변경)
git push origin feature/권기혁
```

### 3️⃣ 코드 합치기 (Pull Request)

1. GitHub 웹사이트에서 본인의 브랜치를 `develop` 브랜치로 Pull Request 생성
2. 팀원들의 리뷰 받기
3. 승인 후 `develop` 브랜치에 합치기

### 4️⃣ 자주 사용하는 명령어

```bash
# 현재 상태 확인
git status

# 브랜치 목록 보기
git branch

# 다른 브랜치로 이동
git checkout 브랜치명

# 최신 코드 받기
git pull origin develop

# 변경사항 저장
git add .
git commit -m "변경 내용 설명"
git push origin 브랜치명
```

### 🚨 주의사항

1. **항상 작업 전에 `git pull origin develop`으로 최신 코드를 받으세요**
2. **`develop` 브랜치에서 직접 작업하지 마세요** - 새 브랜치를 만들어서 작업
3. **커밋 메시지는 명확하게** - "뭐 좀 고침" ❌, "센서 읽기 함수 버그 수정" ✅
4. **자주 커밋하세요** - 작은 단위로 자주 저장하는 것이 좋습니다

### 🆘 문제가 생겼을 때

```bash
# 작업을 되돌리고 싶을 때 (아직 커밋 안한 경우)
git checkout -- .

# 마지막 커밋을 취소하고 싶을 때
git reset --soft HEAD~1

# 다른 브랜치의 최신 변경사항 가져오기
git pull origin develop
```

### 📁 프로젝트 구조
```
redsensor/
├── src/            # 소스 코드
├── include/        # 헤더 파일
├── docs/           # 문서
├── tests/          # 테스트 코드
├── .gitignore      # Git이 무시할 파일들
└── README.md       # 이 파일
```

## 🛠️ 개발 환경 설정

### 필요한 도구들
- Git
- 코드 에디터 (VS Code 추천)
- 임베디드 개발 도구체인

### VS Code 확장 프로그램 추천
- GitLens
- C/C++ (Microsoft)
- PlatformIO IDE (임베디드 개발용)

## 📞 도움이 필요할 때
- GitHub Issues에 질문 올리기
- 팀 채팅방에서 질문하기
- 이 README 파일을 자주 참고하세요!

---
**💡 Tip**: 이 README 파일에 없는 내용이나 더 자세한 설명이 필요하면 언제든 질문하세요!