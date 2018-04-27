Summary: PAK file editor for Quake engine games
Name: pak
Version: 0.1.1
Release: 1
License: GPLv2
Group: System
Source: pak-0.1.1.tar.gz
#Source0: http://dennisk.customer.netspace.net.au/pak/pak-0.1.1.tar.gz

URL: http://dennisk.customer.netspace.net.au/pak.html
Distribution: Fedora
Vendor: DK Soft
Packager: Dennis Katsonis <dennisk@netspace.net.au>

%description
A utility for manipulating .PAK files used by Quake and Quake 2 engine games.  Allows for creation of .PAK data files from directores, extraction, individual file/directory insertion and extraction, and file/directory deletion.


%prep
%setup

%build
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
make

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_docdir}/*
%{_bindir}/*
%{_mandir}/man1/*


%clean
rm -rf $RPM_BUILD_ROOT

