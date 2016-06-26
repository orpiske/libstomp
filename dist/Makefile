VERSION=0.0.1

rpm-prep:
	mkdir -p ${HOME}/rpmbuild/SOURCES/
	tar --transform="s/\./litestomp-${VERSION}/" -cf ${HOME}/rpmbuild/SOURCES/litestomp-${VERSION}.tar.gz --exclude=./build --exclude=.git --exclude=*.bz ./ --gzip

fedora-rpm: rpm-prep
	rpmbuild -ba dist/fc-rpm.spec

rhel-rpm: rpm-prep
	rpmbuild -ba dist/fc-rpm.spec
