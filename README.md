# AutoPebble-Pebble-app

This is an archive of the the AutoPebble Pebbble app's code that joaomgcd is no longer mantaining. Original https://apps.rebble.io/en_US/application/52d1e5957c0c7321a8000008?section=watchapps&dev_settings=true

Additional Features/Improvements by Mediocre-Put2826:

  * Pebble round support
  * color support whilst preserving black and white support
  * Russian text support

PBW downloads https://github.com/clach04/AutoPebble-Pebble-app/releases/tag/tweaks

Requires companion app for Android as well as Tasker

  * https://joaoapps.com/autopebble/
      * https://play.google.com/store/apps/details?id=com.joaomgcd.autopebble
  * https://play.google.com/store/apps/details?id=net.dinglisch.android.taskerm

For more details see:

  * https://www.reddit.com/r/pebble/comments/sxtrwd/autopebble_version_with_fixes_for_pebble_time/
  * https://apps.rebble.io/en_US/application/52d1e5957c0c7321a8000008?query=autopebble&section=watchapps
  * https://joaoapps.com/autopebble/

## Pebble Time 2 / Emery port

Active PT2 work is on the `pebble-time-2` branch. The port adds native `emery` / 200×228 support while preserving the existing AutoPebble UUID, AppMessage protocol, Android companion behavior, and Tasker-facing semantics. The PT2 Quick Screen now uses a native Pebble `ActionBarLayer`; Emery interaction/lifecycle hardening and a Chalk non-ActionBar regression both pass in CI.

See **[docs/pebble-time-2-port.md](docs/pebble-time-2-port.md)** for current status, design decisions, emulator/CI testing, curated screenshots, and remaining physical-watch/release work.

## Resources

  * https://www.reddit.com/r/pebble/comments/2uv35t/app_autopebble_im_blond_so_stupid_please_help_me/
  * https://github.com/clach04/AutoPebble-Pebble-app/wiki
 
