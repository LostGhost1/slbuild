build: slbuild
	echo "Building..."
slbuild: src/
	cp src/slbuild .
clean: src/
	rm slbuild
install: slbuild
	cp src2/slbuild /usr/local/bin/
