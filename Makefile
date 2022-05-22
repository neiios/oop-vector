all:
	mkdir -pv build
	cmake -S . -B build -G Ninja
	cmake --build build --target all
run: all
	./build/src/vector_run
test: all
	./build/test/vector_tst
clean:
	rm -rf build
