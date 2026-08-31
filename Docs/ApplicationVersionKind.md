# Application Version Kind

ACF applications can be built as one of the following version kinds:

| Kind | Displayed as | Selection |
|------|--------------|-----------|
| Developer | `Developer Version` | Default (no selection needed) |
| Beta | `Beta Version` | Build pipeline |
| Release Candidate | `Release Candidate` | Build pipeline |
| Release | `Release` | Build pipeline |

The version kind is selected via the build pipeline at compile time.
Building on a developer PC (without any explicit selection) always results in the **Developer Version**.

## Selecting the version kind in the build pipeline

### CMake

Pass the `ACF_VERSION_KIND` cache variable when configuring:

```bash
cmake -DACF_VERSION_KIND=Release <source-dir>
```

Possible values: `Developer` (default), `Beta`, `ReleaseCandidate`, `Release`.

### QMake

Pass the `ACF_VERSION_KIND` variable when running qmake:

```bash
qmake "ACF_VERSION_KIND=Release"
```

Possible values: `Developer` (default), `Beta`, `ReleaseCandidate`, `Release`.

Both build systems map the selection to one of the compile-time definitions
`ACF_VERSION_KIND_BETA`, `ACF_VERSION_KIND_RELEASE_CANDIDATE` or `ACF_VERSION_KIND_RELEASE`.
If none of these definitions is set (e.g. a local build on a developer PC), the application
reports the Developer Version.

## Programmatic access

The interface `ibase::IApplicationInfo` was extended:

- `IApplicationInfo::VersionKind` — enumeration of the possible version kinds
  (`VK_DEVELOPER`, `VK_BETA`, `VK_RELEASE_CANDIDATE`, `VK_RELEASE`).
- `IApplicationInfo::GetVersionKind()` — returns the version kind of the running application.
  The default implementation returns `VK_DEVELOPER`.
- `IApplicationInfo::AA_VERSION_KIND` — application attribute ID returning the
  (optionally translated) display text of the version kind via `GetApplicationAttribute()`.

`ibase::CApplicationInfoComp` implements `GetVersionKind()` based on the compile-time
definitions listed above.

## Visualization in the UI

The version kind is shown automatically next to the main application version:

- `iqtgui::CAboutWidgetGuiComp` — in the "About..." dialog.
- `iqtgui::CSplashScreenGuiComp` — on the application splash screen.

If no main version is available, the version kind text is displayed alone.
