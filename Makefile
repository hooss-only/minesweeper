# 컴파일러 설정
CC = clang
CFLAGS = -Wall -g
DEPFLAGS = -MMD -MP
LDFLAGS = -lncurses

# 타겟 실행 파일
TARGET = build/output/minesweeper

# 소스 파일
SRCS = $(wildcard src/*.c)

# 오브젝트 파일과 의존성 파일의 경로를 build/ 폴더에 설정
OBJS = $(SRCS:src/%.c=build/%.o)
DEPS = $(OBJS:.o=.d)

# build 폴더와 output 폴더가 존재하지 않으면 생성
BUILD_DIR = build
OUTPUT_DIR = $(BUILD_DIR)/output

# 타겟: 의존성
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# 각 오브젝트 파일을 컴파일하는 규칙
build/%.o: src/%.c | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# build/output 폴더 생성 규칙
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# 의존성 파일을 포함
-include $(DEPS)

# clean 규칙 (빌드된 파일 삭제)
clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)
	rm -rf $(BUILD_DIR)

