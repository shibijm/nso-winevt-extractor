# nso-winevt-extractor

`nso-winevt-extractor` is a command-line utility to extract Windows event logs pertaining to network share objects.

[![Latest Release](https://img.shields.io/github/v/release/shibijm/nso-winevt-extractor?label=Latest%20Release)](https://github.com/shibijm/nso-winevt-extractor/releases/latest)
[![Build Status](https://img.shields.io/github/actions/workflow/status/shibijm/nso-winevt-extractor/release.yml?label=Build&logo=github)](https://github.com/shibijm/nso-winevt-extractor/actions/workflows/release.yml)

## Download

Downloadable builds are available on the [releases page](https://github.com/shibijm/nso-winevt-extractor/releases).

## Usage

Auditing has to be turned on under the ["Audit object access" group policy](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/basic-audit-object-access).

Run the program with administrator privileges.

```text
$ sudo nso-winevt-extractor.exe
2025-05-15 15:34:40	192.168.0.2	\\*\Shared\desktop.ini	Read	0x00120089
2025-05-15 15:34:45	192.168.0.2	\\*\Shared\VID_20250420_233209.mp4	Read	0x00120089
2025-05-15 15:44:00	192.168.0.2	\\*\Shared\New Text Document.txt	Read Write Append	0x0016019F
2025-05-15 15:44:00	192.168.0.2	\\*\Shared\New Text Document.txt	Write	0x00000002
2025-05-15 15:44:05	192.168.0.2	\\*\Shared\New Text Document.txt	Read	0x00120089
2025-05-15 15:44:06	192.168.0.2	\\*\Shared\New Text Document.txt	Read Write Append	0x0012019F
2025-05-15 15:44:09	192.168.0.2	\\*\Shared\New Text Document.txt	Delete	0x00110080
2025-05-15 15:44:09	192.168.0.2	\\*\Shared\\	Read	0x00120089
2025-05-15 15:44:09	192.168.0.2	\\*\Shared\New Text Document.txt	Delete	0x00010080
```

Output Format: `Time\tIpAddress\tPath\tActions\tAccessMask`

Source Event: [Event 5145 - A network share object was checked to see whether client can be granted desired access](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-5145)
