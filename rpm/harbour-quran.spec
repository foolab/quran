Name:       harbour-quran
Summary:    Holy Quran reader
Version:    0.0.0
Release:    1
Group:      Applications
License:    GPLv2
URL:        https://gitorious.org/quran/
Source0:    %{name}-%{version}.tar.bz2
Source1:    harbour-quran.png
Source2:    harbour-quran.desktop
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Sensors)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(qdeclarative5-boostable)
BuildRequires:  pkgconfig(libpulse)
BuildRequires:  pkgconfig(sqlite3)
BuildRequires:  pkgconfig(fontconfig)
BuildRequires:  pkgconfig(libresourceqt5)
BuildRequires:  desktop-file-utils
Requires:  sailfishsilica-qt5
Requires:  mapplauncherd-booster-silica-qt5

%description
 A holy Quran reader. Allows reading the holy book
 and allows adding certain verses to a list of
 favorites to easily navigate to them later.

%define __provides_exclude_from ^%{_datadir}/harbour-quran/lib/.*$
%define __requires_exclude ^libaudioresource-qt.so.2|libdbus-glib-1.so.2|libdbus-qeventloop-qt5.so.1|libresource.so.0|libresourceqt5.so.1$

%prep
%setup -q

%build
pushd data
%qmake5
make
popd

pushd sailfish

%qmake5
make gen
%qmake5
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}

pushd sailfish
%qmake5_install
popd

mkdir -p %{buildroot}/%{_datadir}/harbour-quran/data/
cp data/search.db %{buildroot}/%{_datadir}/harbour-quran/data/

mkdir -p %{buildroot}/%{_datadir}/harbour-quran/fonts/
cp -a data/amiri-quran.ttf %{buildroot}/%{_datadir}/harbour-quran/fonts/
cp -a data/amiri-regular.ttf %{buildroot}/%{_datadir}/harbour-quran/fonts/
cp -a data/fonts.conf %{buildroot}/%{_datadir}/harbour-quran/fonts/

mkdir -p %{buildroot}/%{_datadir}/icons/hicolor/86x86/apps/
mkdir -p %{buildroot}/%{_datadir}/applications/
mkdir -p %{buildroot}/%{_datadir}/harbour-quran/lib/

cp %SOURCE1 %{buildroot}/%{_datadir}/icons/hicolor/86x86/apps/
cp %SOURCE2 %{buildroot}/%{_datadir}/applications/

desktop-file-install --delete-original                   \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

cp /usr/lib/libdbus-glib-1.so.2.2.2 %{buildroot}/%{_datadir}/harbour-quran/lib/libdbus-glib-1.so.2
cp /usr/lib/libresourceqt5.so.1.0.0 %{buildroot}/%{_datadir}/harbour-quran/lib/libresourceqt5.so.1
cp /usr/lib/libresource.so.0.0.18 %{buildroot}/%{_datadir}/harbour-quran/lib/libresource.so.0
cp /usr/lib/libdbus-qeventloop-qt5.so.1.0.0 %{buildroot}/%{_datadir}/harbour-quran/lib/libdbus-qeventloop-qt5.so.1

%files
%defattr(-,root,root,-)
%{_bindir}/harbour-quran
%{_datadir}/applications/harbour-quran.desktop
%{_datadir}/icons/hicolor/86x86/apps/harbour-quran.png
%{_datadir}/harbour-quran/lib/*.so.*
%{_datadir}/harbour-quran/data/search.db
%{_datadir}/harbour-quran/fonts/amiri-quran.ttf
%{_datadir}/harbour-quran/fonts/amiri-regular.ttf
%{_datadir}/harbour-quran/fonts/fonts.conf
