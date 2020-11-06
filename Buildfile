build: slbuild
	echo "Building..."
slbuild: src/
	cp src/slbuild .
clean: src/
	rm slbuild
install: slbuild
	cp src/slbuild /usr/local/bin/
