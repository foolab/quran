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

%prep
%setup -q

%build
pushd sailfish

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

cp %SOURCE1 %{buildroot}/%{_datadir}/icons/hicolor/86x86/apps/
cp %SOURCE2 %{buildroot}/%{_datadir}/applications/

desktop-file-install --delete-original                   \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/harbour-quran
%{_datadir}/applications/harbour-quran.desktop
%{_datadir}/icons/hicolor/86x86/apps/harbour-quran.png
%{_datadir}/harbour-quran/data/search.db
%{_datadir}/harbour-quran/fonts/amiri-quran.ttf
%{_datadir}/harbour-quran/fonts/amiri-regular.ttf
%{_datadir}/harbour-quran/fonts/fonts.conf
