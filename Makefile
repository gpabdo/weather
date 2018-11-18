build:
	docker build -t deb-builder .
	docker run -ti --rm -e PACKAGE_NAME=environment -v "${PWD}":/package deb-builder

help:
	@echo "clean - remove all build artifacts."
	@echo "build - do the build, default command"

clean:
	rm -Rf *.deb
	docker rmi deb-builder
