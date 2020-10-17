name := geon.out
username := bjcolbe
hostname := access.computing.clemson.edu

libs := SDL2_image m
configs := `sdl2-config --libs --cflags`

dir := $(shell basename "$(CURDIR)")
src := $(wildcard src/*.cpp)
objs := ${src:.cpp=.o}

CPPFLAGS += -std=c++1z -Wall
CPPFLAGS += $(foreach dir, include, -I$(dir))
LDFLAGS += $(foreach lib, $(libs), -l$(lib))
LDFLAGS += $(foreach config, $(configs), $(config))

all: create-folders $(name)
test: clear all run

clear:
	@clear
clean:
	@- $(RM) $(name)
	@- $(RM) $(objs)
create-folders:
	@mkdir -p include
	@mkdir -p src
run:
	@echo =======
	@echo Running
	@echo =======
	@./$(name)

$(name): $(objs)
	$(LINK.cc) $(objs) $(LDFLAGS) -o $(name)

share:
	scp -r ../$(dir) $(username)@$(hostname):$(dir)