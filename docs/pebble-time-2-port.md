# Pebble Time 2 / Emery Port

This document is the durable handoff for the Pebble Time 2 port of AutoPebble. It summarizes the engineering decisions, current state, test strategy, and remaining work so future contributors do not need to reconstruct the project from dozens of commits and CI runs.

## Source lineage

- Original project: João Dias' `AutoPebble-Pebble-app` source.
- This repository was forked from the later community-maintained `clach04/AutoPebble-Pebble-app` fork, branch `Mediocre-Put2826`, because it already contained useful post-original fixes and Pebble platform improvements.
- PT2 production work is on `pebble-time-2`.
- Tracking PR: #1, **Pebble Time 2 / Emery support**.
- The completed ActionBar development history remains on `actionbar-prototype` / draft PR #2 for reference; the curated final implementation was promoted to `pebble-time-2` in commit `9f5bd3b6d63d3dc6d2643513582cda7050bc1ac8`.

No explicit software license was found during the initial source audit. Public release/redistribution has not yet been decided; resolve licensing before publishing a stable build beyond private testing.

## Compatibility contract

The port is intentionally conservative about AutoPebble's external behavior.

Do not change unless a future design explicitly requires it:

- App UUID: `1b2d45a5-c00a-4f89-b567-4cf4d2d78b2f`
- Android AutoPebble companion compatibility
- AppMessage command/key protocol
- Tasker-facing behavior and command semantics

Caller-supplied fonts, row sizes, titles, and other explicit AutoPebble settings remain authoritative. PT2-specific defaults improve the out-of-box layout without overriding explicit values.

## Target platforms

- Primary new platform: `emery` / Pebble Time 2
- Native PT2 display: 200×228
- Legacy targets retained: `aplite`, `basalt`, `chalk`

`package.json` includes all four targets, and the current RePebble SDK compiles them successfully.

## Screen inventory

The active AutoPebble content UI consists of three screen types:

1. **List**
2. **Quick Screen**
3. **Text Screen**

There is also startup/tutorial UI. `SETTINGS` is a command type that changes persisted behavior rather than displaying another content screen. An old `INDIVIDUAL` screen type constant remains, but this fork has no active implementation/dispatch for it. `detailView.c` is commented-out legacy code and `listView.c` is not another active screen.

## Important fixes made during the port

### Compiler/correctness

The modern compiler exposed a real inherited out-of-bounds bug:

- List capacity is 30.
- `AutoPebbleList.cellSizes` was only 20 elements.
- Initialization wrote indices 20–29 past the end of the array.

The array now contains 30 entries.

A separate inherited Text Screen helper bug was fixed: the Text Screen replacement path previously allocated the wrong screen object type.

### List

PT2 defaults use:

- Gothic 18 for normal rows instead of Gothic 14
- 32 px default rows instead of 25 px

Explicit caller-provided fonts and cell sizes still win. Legacy-width watches retain legacy defaults. The default menu index now initializes both `section = 0` and `row = 0`.

### Quick Screen

The production PT2 Quick Screen now uses Pebble's native `ActionBarLayer` on wide rectangular watches.

Final PT2 behavior:

- full-height native right-edge ActionBar; its frame is not manually resized or shortened
- native ActionBar button press animation/highlighting
- Up / Select / Down app resources aligned to the three physical buttons
- action-label centers aligned to the native Emery ActionBar centers at approximately y = 54 / 114 / 174
- preferred action-label font: Gothic 28 Bold
- automatic fallback to Gothic 24 Bold only when 28 Bold will not fit the 58 px action region
- explicit caller-supplied `textFont` remains authoritative
- optional title/context uses Gothic 18 Bold by default and does not recreate the old colored title banner
- no-title layouts are supported
- labels wrap and clip inside the available content region

The ActionBar click provider preserves the existing AutoPebble single-click, long-click, and multi-click subscriptions. No AppMessage or Tasker protocol changes were made.

The native ActionBar path is intentionally limited to wide **rectangular** watches. Chalk is also 180 px wide, so width alone is not used as the discriminator. Chalk retains the existing adaptive round Quick Screen layout and does not instantiate the ActionBar.

The current Up/Select/Down bitmaps are app resources rather than firmware-provided icons. They are neutral positional/Pebble-style indicators; arbitrary AutoPebble action text cannot safely be mapped to semantic icons without additional information. A backward-compatible optional per-button icon parameter may be considered later, but only after the core port is fully proven on hardware. Do not infer icon semantics from action labels and do not extend the protocol merely for cosmetics during stabilization.

### Text Screen

The legacy fixed 144 px title width was removed.

The PT2 layout uses:

- full native width
- a fixed title
- body-only scrolling
- small horizontal body padding on the wider display
- pagination-aware text layout and page scrolling on PT2, preventing page boundaries from cutting through glyphs

Legacy rectangular platforms keep their previous scrolling behavior where practical.

## Emulator and CI strategy

The repository contains a GitHub Actions build workflow plus a CI-only emulator harness.

The normal production PBW is built first. The CI harness then creates a disposable emulator-only build that drives the real production screen renderer. Synthetic demo behavior is not included in the production PBW.

Earlier hardening covered the three content screens, including List input/selection, Text scrolling/paging, long labels, titles, no-title Quick layouts, and explicit legacy overrides.

The final ActionBar gate additionally verifies on Emery:

- the production Quick renderer actually creates the native ActionBar
- title and no-title rendering
- single-click dispatch through the ActionBar provider
- long-click dispatch through the ActionBar provider
- native held-button visual feedback
- stacking a second Quick Screen, selecting from it, popping it, and restoring the first screen/provider correctly

A separate Chalk emulator regression verifies that the production Quick renderer reports the native ActionBar inactive and captures a round Quick Screen reference screenshot.

GitHub Actions run #72 (`32742112334`) on production commit `9f5bd3b6d63d3dc6d2643513582cda7050bc1ac8` passed the production build, Emery ActionBar hardening diagnostics, and Chalk isolation regression.

After the first physical PT2 install exposed vertical glyph clipping in the Quick Screen title and action labels, hardware-fix branch `pt2-hardware-fixes` added deliberate vertical render headroom instead of sizing layers tightly to emulator-reported glyph bounds. The exact observed `Media Volume / +5% / Nothing Playing · --% / -5%` case is now reproduced in CI. The same hardware-aware title headroom was applied to the PT2 Text Screen after source review showed similarly tight Gothic 18 Bold title geometry.

GitHub Actions run #80 (`33033381156`) on commit `1d3d13fcee71f5e1d4d207384ffd47fa7b7a9648` passed the production build, existing Emery ActionBar gates, Chalk isolation regression, and new Emery screenshots for Quick, Text, and List. These emulator images are clean, but because the original clipping was hardware-only they reduce risk rather than proving the physical fix.

Synthetic xdotool double-click injection was investigated separately. The same double-click failure occurred with AutoPebble's inherited direct Window click provider and with the ActionBar provider, while single and long clicks passed. The ActionBar was therefore not the cause. Existing multi-click subscriptions remain unchanged; **multi-click is a mandatory physical PT2 test item** rather than an emulator gate.

A phone-less emulator initially made long tests nondeterministic because production AutoPebble responds to Bluetooth disconnects by closing its windows. The CI-only demo therefore avoids production phone/Bluetooth startup and uses a harmless keeper window to keep the app active. Production Bluetooth behavior was not changed.

Transient screenshots and logs from routine CI runs should remain GitHub Actions artifacts rather than being committed to Git.

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

## Current project status

The emulator-side PT2 port is structurally complete and hardened, and the first limited physical PT2 smoke test has been performed.

Completed physical checks:

- candidate PBW installed successfully over the existing app with the preserved UUID
- app launched successfully
- an existing Tasker → AutoPebble action reached the watch successfully
- native Quick Screen ActionBar appeared on the real PT2

That first hardware test also found a real rendering issue that the emulator had not shown: the Quick Screen title and middle action label were vertically clipped. The current `pt2-hardware-fixes` branch adds PT2 render headroom for Quick Screen labels/title and Text Screen title geometry. Run #80 validates the changed production build plus Quick/Text/List emulator references without regressions.

A known-good rollback PBW has been identified and preserved: the community-modded **Classic Pebble menu color** AutoPebble v1.11 build with SHA-256 `4b102bd3d0924b8c045888f9141a65bd16faddf9377d921f2931377fb180e0cd`.

The next physical test must be a **batched hardware validation**, not a one-change iteration. Physical testing is intentionally treated as expensive because it requires manual watch photography and phone/watch interaction.

## Remaining work

1. Preserve the run #80 PBW as the next physical hardware candidate. It keeps UUID `1b2d45a5-c00a-4f89-b567-4cf4d2d78b2f`, version 1.11.0, and Aplite/Basalt/Chalk/Emery targets.
2. Perform one consolidated physical Pebble Time 2 session covering:
   - verify the previous Quick Screen clipping is gone on the real display
   - representative titled and no-title Quick Screens, including a longer label
   - native ActionBar visual feedback
   - single-click and long-click actions
   - **multi-click**, which remains mandatory because emulator injection is unreliable
   - representative List navigation/selection
   - Text Screen visual/scroll sanity if a low-friction real Tasker path is available; do not create a burdensome one-off test solely for this
   - Tasker → watch and watch → Tasker communication
   - confirm existing Sleep as Android handoff remains unaffected and AutoPebble does not cover the Sleep app after tracking starts
   - where practical, piggyback already-pending real AutoPebble Control Hub checks such as AV Receiver Volume and Pause / Resume rather than creating another device-test session
3. Fix only hardware-specific issues discovered by that batch, then repeat only checks plausibly affected by those fixes.
4. Promote the tested hardware fixes from `pt2-hardware-fixes` to `pebble-time-2` only after that physical validation.
5. Decide later whether this work should remain private, be shared informally, or become a public release. If public distribution is chosen, resolve source/icon licensing and package/release documentation before publishing.

Physical-watch testing should remain sparse and batched. Do not request repeated install/photo cycles for cosmetic or emulator-provable changes.

## Documentation policy

Keep this file concise and durable. Update it when an important architectural decision, compatibility constraint, test strategy, or release-state fact changes.

Do not commit every emulator screenshot, CI log, PBW artifact, token, URL containing credentials, or device-specific private automation detail. Use CI artifacts for transient build/test evidence and keep only a small set of curated reference screenshots in Git.
