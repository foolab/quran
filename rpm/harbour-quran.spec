Name:       harbour-quran
Summary:    Holy Quran reader
Version:    0.0.0
Release:    1
Group:      Applications
License:    GPLv2
URL:        https://gitorious.org/quran/
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Sensors)
BuildRequires:  pkgconfig(audioresource-qt)
BuildRequires:  pkgconfig(qdeclarative5-boostable)
BuildRequires:  pkgconfig(libpulse)
BuildRequires:  pkgconfig(sqlite3)
BuildRequires:  desktop-file-utils
Requires:  sailfishsilica-qt5
Requires:  mapplauncherd-booster-silica-qt5

%description
 A holy Quran reader. Allows reading the holy book
 and allows adding certain verses to a list of
 favorites to easily navigate to them later.

%prep
%setup -q

%build
pushd data
%qmake5
make
popd

pushd sailfish
../build-libav.sh --prefix=%{_datadir}/harbour-quran/ --enable-shared --disable-static
%qmake5

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}

pushd libav
%make_install
popd

%qmake5_install
cp harbour-quran.png %{_datadir}/icons/hicolor/86x86/apps/

rm -rf %{buildroot}/%{_datadir}/harbour-quran/lib/*.so
rm -rf %{buildroot}/%{_datadir}/harbour-quran/lib/pkgconfig/
rm -rf %{buildroot}/%{_datadir}/harbour-quran/include/

desktop-file-install --delete-original                   \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/harbour-quran
%{_datadir}/applications/harbour-quran.desktop
%{_datadir}/icons/hicolor/86x86/apps/harbour-quran.png
