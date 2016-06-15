%global _enable_debug_package 0
%global debug_package %{nil}

Summary:            A client library for the STOMP messaging protocol
Name:               libstomp
Version:            0.0.1
Release:            1%{?dist}
License:            Apache-2.0
Summary:            C library used to talk the Stomp 
Group:              Development/Tools
Source:             libstomp-%{version}.tar.gz
URL:                https://github.com/orpiske/libstomp
BuildRequires:      cmake
BuildRequires:      make
BuildRequires:      gcc
BuildRequires:      apr-devel
BuildRequires:      apr-util-devel
Requires:           apr
Requires:           apr-util


%description
This library implements a simple STOMP, a text oriented messaging
protocol that can be used to talk to message brokers such as Apace ActiveMQ.

%prep
%autosetup -n libstomp-%{version}

%build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr ..
make

%install
cd build
make install

%files
%doc AUTHORS README LICENSE COPYING
%{_libdir}/*
%{_includedir}/*


%changelog
* Thu Jun 09 2016 Otavio R. Piske <angusyoung@gmail.com> - 20160609
- Initial release
