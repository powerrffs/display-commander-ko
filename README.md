# Display Commander KO

> **비공식 개인 포크입니다.** [pmnoxx/display-commander](https://github.com/pmnoxx/display-commander) 1.0.0 final을 기반으로, 원본 업데이트가 멈춘 뒤 따로 관리하고 있습니다. 원작자는 Piotr Mikulski(pmnoxx)이며, 이 포크의 문제는 원작자에게 문의하지 마세요.
>
> Unofficial personal fork of [pmnoxx/display-commander](https://github.com/pmnoxx/display-commander) 1.0.0 final, maintained separately. Please don't report issues from this fork to the original author.

### 이 포크에서 바뀐 것
- 여러 게임 인스턴스를 띄웠을 때 서로 커서 가두기(ClipCursor)를 풀어버리던 문제 수정
- Dynamic MFG 사용 시 FG가 꺼져도 OSD에 2x 등으로 남던 문제 수정
- 메인 탭 한국어 UI (`src/addons/display_commander/ui/i18n`, ReShade 언어가 한국어여야 한글이 표시됨)
- 로그 정리, x64 릴리스만 빌드
- 자세한 내용은 [CHANGELOG.md](CHANGELOG.md)의 v1.1.0 항목 참고

### 빌드
GitHub Actions가 `main`에 푸시할 때마다 x64 릴리스를 빌드하고 이 저장소의 `Latest` 릴리스에 `zzz_display_commander.addon64`를 올립니다.
로컬 빌드: Visual Studio 2022+ (C++ 데스크톱 개발), Developer Command Prompt에서
`cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel` → `cmake --build build --parallel`

### 라이선스
원본이 마지막으로 공개한 라이선스(AGPL-3.0, [LICENSE](LICENSE))를 따릅니다.

---

*아래는 원본 README입니다.*

Display Commander is a ReShade addon that provides in-game control over display, windowing, performance, and audio. It adds a simple UI inside the ReShade overlay to adjust borderless/fullscreen behavior, window size and alignment, monitor targeting, FPS limiting (including background caps), and per-process audio volume/mute.

Note: Applying window operations from the main thread can crash some apps. This addon performs them on a background thread.

**✅ Version Requirement**: This addon requires **stable ReShade 6.6.2** or later.

## Features

### Display & window

- **Window mode**: Borderless windowed (aspect ratio or explicit width/height), borderless fullscreen, or no changes
- **Monitor targeting**: Choose which monitor to use; view current display info and refresh rate
- **Black curtain**: Optional full-screen black layers on the game monitor and/or other monitors (windowed games, multi-monitor)
- **Alignment**: Snap window to corners or center
- **Window controls**: Minimize, restore, maximize (applied on a background thread to avoid crashes)
- **Display hotkeys**: Move to previous/next monitor (Win+Left / Win+Right), move to primary/secondary (numpad +/-), minimize/restore (Win+Down / Win+Up)
- **Prevent minimize**: Option to avoid game window minimizing on alt-tab

### Performance & FPS

- **FPS limiter**: Custom limiter with foreground and optional background caps; multiple modes (e.g. OnPresentSync, Reflex, LatentSync, Safe mode)
- **Background FPS**: Optional separate FPS limit when game is in background (checkbox + slider)
- **Live indicators**: Flip state (composed/independent), VRR status, present mode (DXGI/Vulkan/OpenGL)

### Audio

- **Per-process volume**: Control game volume independently
- **Mute options**: Manual mute, mute in background, conditional background mute
- **Hotkeys**: Volume up/down, brightness up/down (configurable step)

### Latency

- **NVIDIA Reflex**: Low-latency mode integration for reduced input lag (native and configurable)
- **Inject Reflex**: Optional injection of Reflex (sleep + latency markers) for games without native Reflex
- **PCLStats (ETW)**: Optional PCLStats event reporting for Special K–style tools

### DLSS & upscaling

- **DLSS information**: Status, quality preset, scaling ratio, internal/output resolution on Main and Swapchain tabs
- **Frame Generation (FG)**: FG mode display in the OSD when DLSS-G is active
- **DLSS overrides**: Preset overrides, internal resolution scale, M–Z presets; optional hide DLSS-G from ReShade
- **VRR status**: DXGI and NVAPI VRR state (enabled/requested/possible) on Swapchain tab

### Input & hotkeys

- **Configurable hotkeys**: Full Hotkeys tab — window/display, volume, brightness, black curtain (other displays), independent window toggle, etc.
- **XInput**: Controller support and hooks for remapping / compatibility
- **Windows.Gaming.Input**: Option to suppress WGI so games use XInput (helps with continue rendering / fake fullscreen)
- **Input remapping**: Controller remapping and related presets

### Advanced & power user

- **Continue rendering in background (fake fullscreen)**: Game keeps rendering when alt-tabbed (no minimize/focus spoofing)
- **Proxy loading**: Rename the built addon (`zzz_display_commander.addon64` / `.addon32`) next to the game to one of these proxy DLL names: **dxgi.dll**, **d3d9.dll**, **d3d11.dll**, **d3d12.dll**, **ddraw.dll**, **hid.dll**, **version.dll**, **opengl32.dll**, **dbghelp.dll**, **vulkan-1.dll**, or **winmm.dll**.
- **Addon directory DLL loading**: From the same folder as the addon, **.dc64 / .dc32 / .dc / .asi** are loaded before ReShade; **.dc64r / .dc32r / .dcr** are loaded after ReShade (for addons that need the ReShade API). All of these DLLs are loaded in place; Windows keeps them locked while the game is running, so you may need to exit the game before overwriting them with newer builds.
- **NVIDIA Profile (Inspector)**: View and edit driver profile for the current game; apply as administrator
- **Clip cursor**: Option to clip cursor to game window

## Continuous Integration

GitHub Actions builds x64 and x86 on pushes and PRs and uploads the resulting `.addon64` and `.addon32` as artifacts. Tag pushes also create releases.

- **Latest**: Every successful push to `main` updates the [Latest](https://github.com/pmnoxx/display-commander/releases/latest) release.
- **Latest Debug Build**: Every successful push to `main` also updates the [Latest Debug Build](https://github.com/pmnoxx/display-commander/releases/tag/latest_debug) release (debug binaries with PDB symbols).

## License

This project is distributed under the terms in [LICENSE](LICENSE). Third-party components and their notices are listed in [THIRD_PARTY_NOTICES.txt](THIRD_PARTY_NOTICES.txt).

## Credits

- ReShade and its addon SDK
- Dear ImGui (via ReShade dependencies)
- NVIDIA NVAPI headers/libs (`external/nvapi`)
- Additional third-party code under `external/` — see [THIRD_PARTY_NOTICES.txt](THIRD_PARTY_NOTICES.txt)

See `CHANGELOG.md` for version history.

---

<p align="center">
  <a href="https://ko-fi.com/pmnox" target="_blank">
    <img src="https://ko-fi.com/img/githubbutton_sm.svg" alt="Support on Ko‑fi" />
  </a>
  <br/>
  <a href="https://ko-fi.com/pmnox">ko-fi.com/pmnox</a>

</p>
