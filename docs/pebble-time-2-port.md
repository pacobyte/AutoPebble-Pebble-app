# Pebble Time 2 / Emery Port

This document is the durable handoff for the Pebble Time 2 port of AutoPebble. It summarizes the engineering decisions, current state, test strategy, and remaining work so future contributors (including a new ChatGPT conversation) do not need to reconstruct the project from dozens of commits and CI runs.

## Source lineage

- Original project: João Dias' `AutoPebble-Pebble-app` source.
- This repository was forked from the later community-maintained `clach04/AutoPebble-Pebble-app` fork, branch `Mediocre-Put2826`, because it already contained useful post-original fixes and Pebble platform improvements.
- PT2 work is developed on `pebble-time-2`.
- Tracking PR: #1, **Pebble Time 2 / Emery support**.
- The ActionBar visual experiment is isolated on `actionbar-prototype` / draft PR #2.

No explicit software license was found during the initial source audit. Resolve licensing before publishing or redistributing a stable release beyond private testing.

## Compatibility contract

The port is intentionally conservative about AutoPebble's external behavior.

Do not change unless a future design explicitly requires it:

- App UUID: `1b2d45a5-c00a-4f89-b567-4cf4d2d78b2f`
- Android AutoPebble companion compatibility
- AppMessage command/key protocol
- Tasker-facing behavior and command semantics

Caller-supplied fonts, row sizes, titles, and other explicit AutoPebble settings remain authoritative. PT2-specific defaults should improve the out-of-box layout without overriding explicit values.

## Target platform

- Pebble platform: `emery`
- Device: Pebble Time 2
- Native display: 200×228
- Legacy targets retained: `aplite`, `basalt`, `chalk`

`package.json` now includes `emery`, and the current RePebble SDK successfully compiles all declared targets.

## Screen inventory

The active AutoPebble content UI consists of three screen types:

1. **List**
2. **Quick Screen**
3. **Text Screen**

There is also startup/tutorial UI. `SETTINGS` is a command type that changes persisted behavior rather than displaying another content screen. An old `INDIVIDUAL` screen type constant remains, but this fork has no active implementation/dispatch for it. `detailView.c` is commented-out legacy code and `listView.c` is not another active screen.

## Important fixes made during the port

### Compiler/correctness

The modern Emery compiler exposed a real inherited out-of-bounds bug:

- List capacity is 30.
- `AutoPebbleList.cellSizes` was only 20 elements.
- Initialization wrote indices 20–29 past the end of the array.

The array now contains 30 entries.

A separate inherited Text Screen helper bug was also fixed: the Text Screen replacement path previously allocated the wrong screen object type.

### List

PT2 defaults now use:

- Gothic 18 for normal rows instead of Gothic 14
- 32 px default rows instead of 25 px

Explicit caller-provided fonts and cell sizes still win. Legacy-width watches retain legacy defaults.

The default menu index now initializes both `section = 0` and `row = 0`.

### Quick Screen

The PT2 renderer now adapts default typography to content:

- short labels can use Gothic 24 Bold
- longer labels step down to Gothic 18 Bold
- labels can wrap inside their button region
- each of the three action labels is vertically centered in its own third of the available area
- no-title layouts remain centered
- explicit caller-supplied fonts still win

The production Quick Screen remains text-based; it does not currently use `ActionBarLayer`.

### Text Screen

The legacy fixed 144 px title width was removed.

The PT2 layout now uses:

- full native width
- a fixed title
- body-only scrolling
- small horizontal body padding on the wider display
- pagination-aware text layout and page scrolling on PT2, preventing page boundaries from cutting through glyphs

Legacy rectangular platforms keep their previous scrolling behavior where practical.

## Emulator and CI strategy

The repository contains a GitHub Actions build workflow plus a CI-only Emery demo harness.

The normal production PBW is built first. The CI harness then creates a disposable emulator-only build that synthesizes representative AutoPebble screens. Synthetic demo behavior is not included in the production PBW.

The emulator suite currently exercises:

- long Quick Screen title/labels
- no-title Quick Screen
- long List labels/header
- List selection movement using simulated hardware-button input
- long Text Screen content
- Text Screen scrolling using simulated hardware-button input
- explicit legacy font/row-size overrides

A phone-less emulator initially made long tests nondeterministic because production AutoPebble responds to Bluetooth disconnects by closing its windows. The CI-only demo therefore avoids production phone/Bluetooth startup and uses a harmless keeper window to keep the app active. Production Bluetooth behavior was not changed.

Transient screenshots from every CI run should remain GitHub Actions artifacts rather than being committed to Git.

## Curated visual references

Only durable reference images are kept in `docs/screenshots/`.

### Initial native Emery launch

![Initial native Emery launch](screenshots/emery-baseline.png)

### Current PT2 List

![Current PT2 List](screenshots/pt2-list.png)

### Current PT2 Quick Screen

![Current PT2 Quick Screen](screenshots/pt2-quick.png)

### Current PT2 Text Screen

![Current PT2 Text Screen](screenshots/pt2-text.png)

## ActionBar experiment

The Quick Screen source historically uses text layers only. It does not contain button-adjacent icons or an `ActionBarLayer`.

A PT2-only design experiment was requested after the core port was complete. The experiment is intentionally isolated from the production port on `actionbar-prototype` / PR #2.

A semantic ActionBar (for example, light bulb / garage / climate icons) is not currently appropriate because AutoPebble sends arbitrary text labels/actions, not semantic icon identities. Adding semantic icons would require a protocol extension or unreliable label inference.

The safe prototype therefore uses neutral **button-position indicators** for Up / Select / Down while preserving the existing command protocol.

![ActionBar prototype](screenshots/actionbar-prototype.png)

Current assessment: the ActionBar version gives stronger visual alignment between each command and the physical button, but the narrower content region needs title treatment refinement. Do not merge PR #2 blindly; compare it against the locked full-width Quick Screen first.

## Current project status

The core emulator port is structurally complete and hardened:

- native Emery build: complete
- three active content screens adapted: complete
- emulator rendering hardening: complete
- emulator List/Text input/scroll verification: complete
- first PT2 visual polish pass: complete
- ActionBar experiment: in progress, isolated

The compatibility contract has been preserved in source, but real Android companion ↔ physical PT2 communication still requires one late-stage hardware integration test.

## Remaining work

1. Finish the ActionBar A/B decision and either adopt a refined version or close the prototype without merging.
2. Run a final emulator regression pass after the visual decision.
3. Secure a known-good rollback PBW for the currently working watch installation.
4. Build the candidate PT2 PBW.
5. Perform one broad physical Pebble Time 2 test:
   - install/launch
   - Android AutoPebble communication
   - representative Tasker → watch command
   - watch → Tasker action
   - List / Quick / Text behavior
6. Fix only hardware-specific issues discovered by that test.
7. Package the stable release and update this document with final release/rollback information.

Physical-watch testing has intentionally been deferred until late in the process to avoid disrupting a currently functional AutoPebble installation and to minimize repeated manual testing.

## Documentation policy

Keep this file concise and durable. Update it when an important architectural decision, compatibility constraint, test strategy, or release-state fact changes.

Do not commit every emulator screenshot, CI log, PBW artifact, token, URL containing credentials, or device-specific private automation detail. Use CI artifacts for transient build/test evidence and keep only a small set of curated reference screenshots in Git.
