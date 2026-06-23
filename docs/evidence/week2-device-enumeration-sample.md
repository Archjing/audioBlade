# Phase 0 Week 2 Device Enumeration Sample

## Status

Result: `pass`

Reason:

The Windows host build enumerated real audio devices through JUCE and produced visible backend, device-name, direction, channel-count, and default-status fields. The diagnostic output was copied from the terminal screen because the intermediate `.txt` capture path introduced mojibake for Chinese device names.

## Evidence

Date: 2026-06-23T13:06:48.322+08:00

Host OS: Windows 11

Environment label: Windows host development checkout at `D:\ZJ\Dev\audioBlade`

Audio backend or API: JUCE audio-device enumeration across DirectSound, Windows Audio, Windows Audio (Exclusive Mode), and Windows Audio (Low Latency Mode)

Command:

```powershell
.\build\apps\desktop\audioBladeDesktop_artefacts\Debug\audioBlade.exe --diagnose-devices
```

Default input: field emitted; no device was marked `default_input=true` in this capture.

Default output: field emitted; no device was marked `default_output=true` in this capture.

Visible devices:

```text
date=2026-06-23T13:06:48.322+08:00
os=Windows 11
device_count=43
backend=DirectSound
- name=主声音捕获驱动程序 direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name={0.0.1.00000000}.{b52d3c3c-8aae direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name={0.0.1.00000000}.{10d3cd5e-0e17 direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name=内部 AUX 插座 (Steam Streaming Speakers) direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name={0.0.1.00000000}.{3b0e9706-3a6c direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name=Microphone (2K USB Camera-Audio) direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name=麦克风 (Steam Streaming Microphone) direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name={0.0.1.00000000}.{c1efd625-f4cb direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name={0.0.1.00000000}.{f68ffe97-b388 direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name=主声音驱动程序 direction=output input_channels=0 output_channels=2 default_input=false default_output=false
backend=Windows Audio
- name=[unnamed-device] direction=unknown input_channels=0 output_channels=0 default_input=false default_output=false
- name= (2) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=内部 AUX 插座 (Steam Streaming Speakers) direction=input input_channels=1 output_channels=0 default_input=false default_output=false
- name= (3) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=Microphone (2K USB Camera-Audio) direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name=麦克风 (Steam Streaming Microphone) direction=input input_channels=1 output_channels=0 default_input=false default_output=false
- name= (4) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name= (5) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=扬声器 (Steam Streaming Speakers) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
- name=数字音频(S/PDIF) (High Definition Audio Device) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
- name=扬声器 (Steam Streaming Microphone) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
backend=Windows Audio (Exclusive Mode)
- name=[unnamed-device] direction=unknown input_channels=0 output_channels=0 default_input=false default_output=false
- name= (2) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=内部 AUX 插座 (Steam Streaming Speakers) direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name= (3) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=Microphone (2K USB Camera-Audio) direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name=麦克风 (Steam Streaming Microphone) direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name= (4) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name= (5) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=扬声器 (Steam Streaming Speakers) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
- name=数字音频(S/PDIF) (High Definition Audio Device) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
- name=扬声器 (Steam Streaming Microphone) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
backend=Windows Audio (Low Latency Mode)
- name=[unnamed-device] direction=unknown input_channels=0 output_channels=0 default_input=false default_output=false
- name= (2) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=内部 AUX 插座 (Steam Streaming Speakers) direction=input input_channels=1 output_channels=0 default_input=false default_output=false
- name= (3) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=Microphone (2K USB Camera-Audio) direction=input input_channels=2 output_channels=0 default_input=false default_output=false
- name=麦克风 (Steam Streaming Microphone) direction=input input_channels=1 output_channels=0 default_input=false default_output=false
- name= (4) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name= (5) direction=duplex input_channels=2 output_channels=2 default_input=false default_output=false
- name=扬声器 (Steam Streaming Speakers) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
- name=数字音频(S/PDIF) (High Definition Audio Device) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
- name=扬声器 (Steam Streaming Microphone) direction=output input_channels=0 output_channels=2 default_input=false default_output=false
```

## Notes

- The `.txt` capture path is not used as the authoritative evidence because PowerShell native-command output can misdecode UTF-8 stdout before writing the file.
- This evidence confirms device enumeration visibility only. It does not claim real routing, virtual-driver integration, AirPlay, or background routing behavior.
- Default device resolution remains a follow-up diagnostic improvement because this capture emitted the default-status fields but did not mark a concrete default input or output.
