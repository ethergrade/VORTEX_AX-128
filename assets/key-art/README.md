# VORTEX AX-128 key-art review

## Flaticon pack correlation

- `pack-correlation.csv` inventories every SVG in the local `pack/` folder and records filename-based candidate keys.
- `pack-index.html` is a local visual review page with shortlisted icons grouped by key. It opens the source SVGs from `pack/` in a new tab.
- `build_pack_index.py` regenerates the inventory and review page from `keymap.json` and the local SVG pack.
- `keymap.json` contains the project Keymap V1.0 used as the correlation target.

The filename correlation is heuristic. Scores rank textual matches only; inspect the SVG visually and confirm each match before using it on a physical key. Some controls have no obvious filename match in this library.

The original Flaticon files in `pack/` and the generated visual review page are local-only. The GitHub repository is public, and Flaticon does not permit distributing its licensed assets separately from an end product. The raw pack is excluded by `.gitignore`; the CSV inventory contains filenames and suggestions, not SVG artwork. See `FLATICON-LICENSE.txt` and [Flaticon licensing](https://www.flaticon.com/merchandising-license).

## Earlier generated cards

`svg/` and `index.html` are the earlier font-based graphics. Their external font dependency does not render reliably inside SVG image previews. Use `pack-index.html` to review the original standalone SVG candidates instead.
