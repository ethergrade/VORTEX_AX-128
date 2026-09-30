#!/usr/bin/env python3
"""Build a filename-based icon inventory and local visual review index."""

from __future__ import annotations

import csv
import html
import re
from pathlib import Path

BASE = Path(__file__).resolve().parent
PACK = BASE / "pack"
KEYMAP = BASE.parents[1] / "docs" / "05_KEYMAP_V1_0.md"
CSV_OUT = BASE / "pack-correlation.csv"
HTML_OUT = BASE / "pack-index.html"
TOP_PER_KEY = 6

# Filename concepts only; this is a review shortlist, not an automatic final selection.
SPECIAL_ALIASES = {
    "A1": ["layer", "layers", "layer stack", "stack", "stacked layers"],
    "A2": ["layer", "layers", "layer stack", "stack", "stacked layers"],
    "A3": ["layer", "layers", "layer stack", "stack", "stacked layers"],
    "A4": ["layer", "layers", "layer stack", "stack", "stacked layers"],
    "A5": ["mute", "volume mute", "volume off", "volume slash", "sound off"],
    "A6": ["solo", "microphone", "mic", "single user", "headphones"],
    "A7": ["copy", "duplicate", "clone", "copy files"],
    "A8": ["paste", "clipboard paste", "clipboard"],
    "A9": ["drum", "drums", "drum kit", "percussion"],
    "A10": ["granular", "grain", "grains", "particle", "particles", "waveform", "dots"],
    "A11": ["synth", "synthesizer", "waveform", "oscillator", "keyboard", "music"],
    "A12": ["analog", "wave sine", "sine wave", "waveform", "circuit", "signal"],
    "A13": ["effect", "effects", "sparkles", "magic wand", "wand", "stars"],
    "A14": ["sequencer", "sequence", "timeline", "list timeline", "step sequence"],
    "A15": ["mixer", "mix", "audio mixer", "sliders", "mixing console"],
    "A16": ["system", "settings", "gear", "cog", "controls"],
    "D1": ["freeze", "snowflake", "frozen", "ice"],
    "D2": ["hold", "gate", "lock", "pause", "hold gate"],
    "D3": ["retrigger", "repeat", "replay", "rotate right"],
    "D4": ["reverse", "backward", "rewind", "rotate left"],
    "D5": ["capture", "snapshot", "camera", "record screen", "screenshot"],
    "D6": ["resample", "refresh", "repeat", "sample"],
    "D7": ["sync", "synchronization", "clock", "free", "metronome"],
    "D8": ["random", "randomize", "shuffle", "dice", "lottery"],
    "D9": ["position", "move left", "arrow left", "left"],
    "D10": ["position", "move right", "arrow right", "right"],
    "D11": ["length", "shorten", "minus", "decrease"],
    "D12": ["length", "extend", "plus", "increase"],
    "D13": ["density", "sparse", "minus", "decrease"],
    "D14": ["density", "dense", "plus", "increase"],
    "D15": ["pitch", "pitch down", "arrow down", "down"],
    "D16": ["pitch", "pitch up", "arrow up", "up"],
    "E1": ["envelope square", "square", "square wave"],
    "E2": ["trapezoid", "trapezoidal", "envelope trapezoid", "polygon", "geometric shape", "chart"],
    "E3": ["hann", "hanning", "window hann", "window", "curve", "chart"],
    "E4": ["gaussian", "bell curve", "normal distribution", "bell", "curve", "chart"],
    "E5": ["triangle", "triangular", "triangle wave"],
    "E6": ["spray", "sprinkle", "scatter"],
    "E7": ["spray", "sprinkle", "scatter"],
    "E8": ["chaos", "chaotic", "random"],
    "E9": ["chaos", "chaotic", "random"],
    "E10": ["space", "spatial", "room", "distance"],
    "E11": ["space", "spatial", "room", "distance"],
    "E12": ["reverse", "backward", "rewind", "rotate left"],
    "E13": ["source", "input", "sound source"],
    "E14": ["source", "input", "sound source"],
    "E15": ["bank", "library", "collection", "folder"],
    "E16": ["bank", "library", "collection", "folder"],
    "G1": ["chorus", "choir", "group singing", "multiple voices"],
    "G2": ["flanger", "flange", "waveform", "wave", "audio", "sound"],
    "G3": ["phaser", "phase", "phase shift", "waveform", "wave", "audio", "sound"],
    "G4": ["tremolo", "trembling", "vibration", "waveform", "wave", "audio", "sound"],
    "G5": ["vibrato", "vibration", "waveform", "wave", "audio", "sound"],
    "G6": ["delay", "delayed", "clock", "timer"],
    "G7": ["reverb", "reverberation", "echo", "cave", "waveform", "audio", "sound"],
    "G8": ["drive", "overdrive", "distortion", "fire"],
    "G9": ["bitcrusher", "bit crusher", "pixel", "digital"],
    "G10": ["wavefold", "wave folding", "waveform", "sine wave"],
    "G11": ["stutter", "stammer", "repeat"],
    "G12": ["tape stop", "cassette", "tape", "stop"],
    "G13": ["reverse", "backward", "rewind"],
    "G14": ["glitch", "bug", "error", "corruption"],
    "G15": ["freeze", "snowflake", "frozen", "ice"],
    "G16": ["bypass", "kill", "power off", "disable", "cancel"],
    "H1": ["play", "stop", "play stop", "play pause", "media player"],
    "H2": ["record", "recording", "vinyl", "microphone"],
    "H3": ["overdub", "over dub", "layer recording", "recording"],
    "H4": ["tap tempo", "tempo", "metronome", "tap"],
    "H5": ["tempo", "bpm", "slow", "minus", "decrease"],
    "H6": ["tempo", "bpm", "fast", "plus", "increase"],
    "H7": ["pattern", "sequence", "previous", "arrow left"],
    "H8": ["pattern", "sequence", "next", "arrow right"],
}


def words(value: str) -> list[str]:
    value = value.lower().replace("&", " and ")
    return re.findall(r"[a-z0-9]+", value)


def slug_words(value: str) -> str:
    return " ".join(words(value))


def key_aliases(key: dict) -> list[str]:
    code = key["key"]
    label = key["label"]
    result = list(SPECIAL_ALIASES.get(code, []))
    if code.startswith("B"):
        result += ["step", "step sequencer", "sequence step", "grid", "rhythm step"]
    elif code.startswith("C") and code not in SPECIAL_ALIASES:
        label_alias = {
            "BD": ["bass drum", "kick", "kick drum", "drum", "drum kit", "percussion"], "BD2": ["bass drum", "kick 2", "kick drum", "drum", "percussion"],
            "SNARE": ["snare", "snare drum", "drum", "percussion"], "CLAP": ["clap", "hand clap", "drum", "percussion"], "RIM": ["rim", "rimshot", "drum", "percussion"],
            "LT": ["low tom", "tom low", "tom", "drum"], "MT": ["mid tom", "middle tom", "tom", "drum"], "HT": ["high tom", "tom high", "tom", "drum"],
            "CLOSED HH": ["closed hi hat", "closed hihat", "closed hh", "hi hat closed", "hi hat", "cymbal", "drum"],
            "OPEN HH": ["open hi hat", "open hihat", "open hh", "hi hat open", "hi hat", "cymbal", "drum"],
            "CRASH": ["crash cymbal", "cymbal crash", "cymbal", "drum"], "RIDE": ["ride cymbal", "cymbal ride", "cymbal", "drum"],
            "COWBELL": ["cowbell", "cow bell", "bell", "drum"], "PERC 1": ["percussion", "percussion instrument", "drum"],
            "PERC 2": ["percussion", "percussion instrument", "drum"], "USER DRUM": ["user drum", "custom drum", "drum"],
        }
        result += label_alias.get(label, [label])
    elif code.startswith("F"):
        result += ["music note", "musical note", "music", "piano key", "note"]
    elif code.startswith("I"):
        result += [label]
    elif label.startswith("SCENE"):
        result += ["scene", "scenery", "landscape", "slideshow", "stage"]
    if label not in {"BD", "BD2", "LT", "MT", "HT"} and not code.startswith("B") and not code.startswith("F"):
        result.append(label)
    # Preserve ordered aliases while avoiding duplicate normalized phrases.
    unique = []
    seen = set()
    for alias in result:
        normalized = slug_words(alias)
        if normalized and normalized not in seen:
            unique.append(normalized)
            seen.add(normalized)
    return unique


def score_name(stem: str, aliases: list[str]) -> tuple[int, str]:
    name_words = words(stem)
    best = (0, "")
    for alias in aliases:
        alias_words = words(alias)
        if not alias_words:
            continue
        if name_words == alias_words:
            candidate = (120 + len(alias_words), alias)
        elif any(name_words[i:i + len(alias_words)] == alias_words for i in range(len(name_words) - len(alias_words) + 1)):
            candidate = (95 + len(alias_words), alias)
        elif len(alias_words) == 1 and alias_words[0] in name_words:
            candidate = (76, alias)
        elif len(alias_words) > 1 and set(alias_words).issubset(set(name_words)):
            candidate = (72 + len(alias_words), alias)
        else:
            # Only permit prefix matching for long, distinctive icon-name tokens.
            matched = [t for t in alias_words if len(t) >= 5 and any(n.startswith(t) for n in name_words)]
            candidate = (55 + len(matched), alias) if len(matched) == len(alias_words) and matched else (0, "")
        if candidate[0] > best[0]:
            best = candidate
    return best


def esc(value: str) -> str:
    return html.escape(value, quote=True)


def load_keymap() -> list[dict]:
    records = []
    for line in KEYMAP.read_text(encoding="utf-8").splitlines():
        cells = [cell.strip() for cell in line.strip().strip("|").split("|")]
        if len(cells) != 17 or not (cells[0].startswith("**") and cells[0].endswith("**")):
            continue
        row = cells[0][2:-2]
        if row not in "ABCDEFGH":
            continue
        for column, label in enumerate(cells[1:], start=1):
            records.append({"key": f"{row}{column}", "row": row, "column": column, "label": label})
    if len(records) != 128:
        raise SystemExit(f"Expected 128 labels in {KEYMAP}, found {len(records)}")
    return records


def main() -> None:
    if not PACK.is_dir():
        raise SystemExit(f"Missing SVG pack: {PACK}")
    key_records = load_keymap()
    icons = []
    for path in sorted(PACK.glob("*.svg")):
        stem = path.stem.removeprefix("fi-rr-")
        icons.append({"file": path.name, "name": stem, "size": path.stat().st_size})

    suggestions_by_key: dict[str, list[dict]] = {}
    icon_key_matches: dict[str, list[tuple[int, str, str]]] = {icon["file"]: [] for icon in icons}
    for key in key_records:
        scored = []
        aliases = key_aliases(key)
        for icon in icons:
            score, basis = score_name(icon["name"], aliases)
            if score:
                record = {**icon, "score": score, "basis": basis, "key": key["key"], "label": key["label"]}
                scored.append(record)
                if score >= 76:
                    icon_key_matches[icon["file"]].append((score, key["key"], key["label"]))
        suggestions_by_key[key["key"]] = sorted(scored, key=lambda r: (-r["score"], r["name"]))[:TOP_PER_KEY]

    with CSV_OUT.open("w", newline="", encoding="utf-8-sig") as handle:
        writer = csv.writer(handle, lineterminator="\n")
        writer.writerow(["svg_file", "icon_name", "bytes", "candidate_keys_by_filename", "best_candidate", "best_key_label"])
        for icon in icons:
            matches = sorted(icon_key_matches[icon["file"]], key=lambda r: (-r[0], r[1]))
            writer.writerow([
                icon["file"], icon["name"], icon["size"],
                "; ".join(f"{code} {label} ({score})" for score, code, label in matches),
                matches[0][1] if matches else "",
                matches[0][2] if matches else "",
            ])

    sections = []
    for key in key_records:
        candidates = suggestions_by_key[key["key"]]
        cards = []
        for candidate in candidates:
            cards.append(
                '<article class="candidate">'
                f'<a class="preview" href="pack/{esc(candidate["file"])}" target="_blank" rel="noreferrer">'
                f'<img loading="lazy" src="pack/{esc(candidate["file"])}" alt="{esc(candidate["name"])}"></a>'
                f'<div class="candidate-name"><a href="pack/{esc(candidate["file"])}" target="_blank" rel="noreferrer">{esc(candidate["file"])}</a></div>'
                f'<small>Match filename: {candidate["score"]} · {esc(candidate["basis"])}</small>'
                '</article>'
            )
        if not cards:
            cards.append('<p class="empty">Nessun nome file corrisponde alle parole chiave: cercare manualmente nel CSV o nel pack.</p>')
        sections.append(
            f'<section class="key" data-search="{esc(key["key"] + " " + key["label"])}">'
            f'<header><span class="code">{esc(key["key"])}</span><h3>{esc(key["label"])}</h3></header>'
            f'<div class="candidates">{"".join(cards)}</div></section>'
        )

    html_doc = f'''<!doctype html>
<html lang="it"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width">
<title>Indice di correlazione SVG · Keymap V1.0</title>
<style>
:root{{--bg:#0c0f17;--panel:#161b27;--line:#2c3547;--ink:#f0f3fa;--muted:#a2acc0;--accent:#79f2ca}}
*{{box-sizing:border-box}}body{{margin:0;background:var(--bg);color:var(--ink);font:15px/1.45 system-ui,sans-serif}}
main{{max-width:1500px;margin:auto;padding:24px}}h1{{margin:0 0 8px;font-size:28px}}p{{color:var(--muted)}}
.toolbar{{position:sticky;top:0;background:#0c0f17f2;padding:14px 0;z-index:2;border-bottom:1px solid var(--line)}}
input{{width:min(100%,540px);padding:12px 14px;border:1px solid var(--line);border-radius:9px;background:var(--panel);color:var(--ink);font:inherit}}
.summary{{font-size:13px}}.row{{margin:32px 0}}.row h2{{color:var(--accent);border-bottom:1px solid var(--line);padding-bottom:8px}}
.key{{border:1px solid var(--line);background:var(--panel);border-radius:12px;padding:14px;margin:12px 0}}
.key header{{display:flex;align-items:baseline;gap:12px}}.key h3{{margin:0 0 12px;font-size:18px}}.code{{font-weight:800;color:var(--accent);min-width:38px}}
.candidates{{display:grid;grid-template-columns:repeat(auto-fill,minmax(150px,1fr));gap:12px}}
.candidate{{min-width:0;border:1px solid var(--line);border-radius:9px;padding:8px;background:#10141e}}
.preview{{display:flex;height:116px;align-items:center;justify-content:center;background:white;border-radius:6px}}
.preview img{{width:100px;height:100px;object-fit:contain}}a{{color:#b6f5e2;text-decoration:none;overflow-wrap:anywhere}}a:hover{{text-decoration:underline}}
.candidate-name{{margin:8px 0 2px;font-size:12px}}small{{color:var(--muted);font-size:11px}}.empty{{grid-column:1/-1}}
.key[hidden],.row[hidden]{{display:none}}.note{{padding:12px 14px;border-left:3px solid var(--accent);background:var(--panel);border-radius:4px}}
</style></head><body><main>
<h1>Indice grafica ↔ keymap</h1>
<p>Shortlist visiva costruita usando solo i nomi dei file. Le corrispondenze sono suggerimenti da verificare, non selezioni definitive.</p>
<p class="note">Pack locale: {len(icons):,} SVG fi-rr. I candidati qui sotto aprono il file originale in assets/key-art/pack. L’inventario completo è nel CSV accanto a questo indice.</p>
<div class="toolbar"><input id="search" type="search" placeholder="Filtra per tasto, nome tasto o filename…"><span class="summary" id="count"></span></div>
{''.join(f'<div class="row"><h2>Riga {row}</h2>{"".join(section for section, record in zip(sections, key_records) if record["row"] == row)}</div>' for row in "ABCDEFGH")}
</main><script>
const input=document.querySelector('#search'), keys=[...document.querySelectorAll('.key')], count=document.querySelector('#count');
function filter(){{const q=input.value.toLowerCase().trim();let n=0;for(const k of keys){{const hit=!q||k.dataset.search.toLowerCase().includes(q)||k.textContent.toLowerCase().includes(q);k.hidden=!hit;if(hit)n++}}for(const row of document.querySelectorAll('.row'))row.hidden=!row.querySelector('.key:not([hidden])');count.textContent=` ${{n}} tasti visibili`}}
input.addEventListener('input',filter);filter();
</script></body></html>'''
    HTML_OUT.write_text(html_doc, encoding="utf-8")
    print(f"SVG inventariati: {len(icons)}")
    print(f"Tabella: {CSV_OUT}")
    print(f"Indice visuale: {HTML_OUT}")
    no_match = [key["key"] for key in key_records if not suggestions_by_key[key["key"]]]
    print(f"Tasti senza candidati filename: {len(no_match)} ({', '.join(no_match) or 'nessuno'})")


if __name__ == "__main__":
    main()
