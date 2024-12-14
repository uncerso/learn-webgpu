release:
	@CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake . -B ./build -DCMAKE_BUILD_TYPE=Release -DWEBGPU_BACKEND=WGPU_STATIC
	@cmake --build ./build --config Release --parallel
	@./build/run

clean:
	@rm -rf ./build
