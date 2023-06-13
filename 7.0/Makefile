C_FLAGS=-std=c++11 -Wall -Wextra
SRC_DIR=src
OBJS_DIR=lib
SRCS=$(wildcard ${SRC_DIR}/*.cpp ${SRC_DIR}/*/*.cpp)
HEADERS=$(wildcard ${SRC_DIR}/*.hpp ${SRC_DIR}/custom_exceptions/*.hpp)
OBJS=$(subst ${SRC_DIR}, ${OBJS_DIR},$(subst .cpp,.o,$(SRCS)))

A7: $(OBJS)
	g++ $(OBJS) -o futballFantasy.out

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	g++ $(C_FLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS)
