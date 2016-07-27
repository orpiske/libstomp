%global _enable_debug_package 0
%global debug_package %{nil}

Summary:            A client library for the STOMP messaging protocol
Name:               litestomp
Version:            0.0.1
Release:            3%{?dist}
License:            Apache-2.0
Group:              Development/Tools
Source:             litestomp-%{version}.tar.gz
URL:                https://github.com/orpiske/litestomp
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
%autosetup -n litestomp-%{version}

%build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=%{buildroot}/usr ..
make

%install
cd build
make install

%files
%doc AUTHORS README.md LICENSE COPYING
%{_libdir}/*


%changelog
* Wed Jul 06 2016 Otavio R. Piske <angusyoung@gmail.com> - 20160706
- Memory leak fixes
- Timeout support
- Improved error handling
- And many other code improvements

* Thu Jun 09 2016 Otavio R. Piske <angusyoung@gmail.com> - 20160609
- Initial release
