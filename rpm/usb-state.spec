Name:           usb-state
Version:        1.0.0
Release:        1%{?dist}
Summary:        Usb state notifier

License:        GPLv3
URL:            https://example.com
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  pkgconfig(libudev)

Requires(post): systemd
Requires(preun): systemd
Requires(postun): systemd

%description
Simple daemon that monitors power supply events and sents appropriate usb state.

%prep
%autosetup

%build
%cmake
%cmake_build

%install
%cmake_install

# Binary
install -Dm755 usb_state_service \
    %{buildroot}/usr/bin/usb_state_service

# Systemd service
install -Dm644 usb-state.service \
    %{buildroot}%{_unitdir}/usb-state.service

%post
%systemd_post usb-state.service

%preun
%systemd_preun usb-state.service

%postun
%systemd_postun_with_restart usb-state.service

%files
/usr/bin/usb_state_service
%{_unitdir}/usb-state.service
