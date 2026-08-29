# PT2 Font Lab

Standalone Pebble Time 2 / Emery experiment for Issue #8.

It does **not** change production AutoPebble and uses its own UUID.

## Candidates

1. Pebble Raster Gothic (control)
2. Atkinson Hyperlegible
3. Source Sans 3
4. Fira Sans
5. IBM Plex Sans

Each candidate is rendered at 18 px and 24 px using identical sample strings. Use the physical Up/Down buttons to cycle through candidates.

The workflow downloads the upstream TTF files from immutable Git blob SHAs before building, so font binaries do not need to be stored in this repository.

## Font source blobs

- Atkinson Hyperlegible Regular — google/fonts blob `b3cc4200fcf09d600eac74002ab4ea65b404a536`
- Source Sans 3 Regular — adobe-fonts/source-sans blob `c5dd0b88cc8ff4edd7ebdcefee0290df15529657`
- Fira Sans Regular — mozilla/Fira blob `d9fdc0e922030c6836a9ad5f808fd7e1c5970873`
- IBM Plex Sans Regular — IBM/plex blob `bd6817d5202895da5ac4fad88de3da71e652881a`

Licensing/provenance should be reviewed before any of these fonts are incorporated into a distributed production AutoPebble build.
