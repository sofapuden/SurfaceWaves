
deps:
	cd ./vendor/ && make glfw
	cd ./vendor/ && make glew
	git submodule deinit -f .
	git submodule update --init
