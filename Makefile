CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread
GTEST_FLAGS = -lgtest -lgtest_main

TARGET = XianHuanZhi
TEST_TARGET = XianHuanZhi_Test
SOURCES = main.cpp XianHuanZhi.cpp ItemManager.cpp
TEST_SOURCES = XianHuanZhi_Test.cpp XianHuanZhi.cpp ItemManager.cpp test_main.cpp

# 编译主程序
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# 编译测试程序
$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES) $(GTEST_FLAGS)

# 运行测试
test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) users.dat items.dat *.backup

.PHONY: clean test