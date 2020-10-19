build: slbuild
	echo "Building..."
slbuild: src2/
	cp src2/slbuild .
clean: src2/
	rm slbuild
install: slbuild
	cp src2/slbuild /usr/local/bin/
