Summary:            A client library for the STOMP messaging protocol
Name:               litestomp
Version:            0.0.1
Release:            9%{?dist}
License:            Apache-2.0
Source:             litestomp-%{version}.tar.gz
URL:                https://github.com/orpiske/litestomp
BuildRequires:      cmake
BuildRequires:      make
BuildRequires:      gcc
BuildRequires:      apr-devel
BuildRequires:      apr-util-devel


%description
This library implements a simple STOMP, a text oriented messaging
protocol that can be used to talk to message brokers such as Apace ActiveMQ.


%package devel
Summary:            STOMP client library development kit
Requires:           %{name}%{?_isa} = %{version}-%{release}


%description devel
Development packages for the STOMP client library

%package devel-doc
Summary:            STOMP client library development kit documentation
BuildArch:          noarch
Obsoletes:          gru-devel-docs

%description devel-doc
Development documentation for the STOMP client library development

%prep
%autosetup -n litestomp-%{version}

%build
mkdir build && cd build
%cmake -DBUILD_WITH_DOCUMENTATION=ON -DCMAKE_USER_C_FLAGS="-fPIC" ..
%make_build all documentation

%install
cd build
%make_install

%files
%doc AUTHORS README.md
%license LICENSE COPYING
%{_libdir}/*.so.*

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files devel
%{_includedir}/*
%{_libdir}/*.so

%post devel -p /sbin/ldconfig

%postun devel -p /sbin/ldconfig

%files devel-doc
%license LICENSE COPYING
%{_datadir}/*


%changelog
* Wed Aug 30 2017 Otavio R. Piske <angusyoung@gmail.com> - 0.0.1-9
- Adjusted to comply with Fedora packaging guidelines

* Wed Aug 30 2017 Otavio R. Piske <angusyoung@gmail.com> - 0.0.1-8
- Fixed mismatch between this package version and the SO version

* Tue Dec 20 2016 Otavio R. Piske <angusyoung@gmail.com> - 20161220
- Several error handling fixes
- Reduced memory usage and fixed memory leaks
- Added support for setting litestomp options

* Tue Sep 07 2016 Otavio R. Piske <angusyoung@gmail.com> - 20160907
- Fixed incorrect license information 

* Wed Jul 06 2016 Otavio R. Piske <angusyoung@gmail.com> - 20160706
- Memory leak fixes
- Timeout support
- Improved error handling
- And many other code improvements

* Thu Jun 09 2016 Otavio R. Piske <angusyoung@gmail.com> - 20160609
- Initial release
