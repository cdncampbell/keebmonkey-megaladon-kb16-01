# KeebMonkey Megalodon (DOIO KB16-01) layout

My macropad layout: 16 keys, three dials (two small, one big) and an OLED. It is a DOIO KB16-01 rev2 (STM32F103, stm32duino bootloader, USB `D010:1601`).

## Layers

Layers are colour-coded on the pad's LEDs and in the cheat sheet.

| # | Layer    | Colour | Used for                                |
|---|----------|--------|-----------------------------------------|
| 0 | Base     | blue   | Browser, window snapping, screenshots   |
| 1 | Cursor   | orange | AI, editing, panels                     |
| 2 | Terminal | green  | Shell-safe copy/paste, git macros M0-M9 |
| 3 | Obsidian | purple | Notes, editing, view                    |

Pressing the small right dial steps to the next layer (0 → 1 → 2 → 3 → 0). Pressing the big dial goes straight back to Base.

## What's here

| Path | What it is |
|------|------------|
| `via/cdncampbell-KB16_01.layout.json` | The layout as VIA saves it: keys, dials and macros. This is the source of truth. |
| `via/definitions/kb16VIA.json` | DOIO's VIA keyboard definition, for when VIA doesn't detect the board. |
| `qmk/keymaps/cdncampbell/` | The same layout as a QMK keymap, plus the per-layer LED colours. |
| `cheat-sheet/` | Printable layer reference (HTML source and PDF). |
| `firmware/doio-stock-firmware.bin` | Dump of the factory firmware, for going back to stock. |

## Changing the layout

1. Remap in [VIA](https://usevia.app) and use **Save + Load → Save** to export over `via/cdncampbell-KB16_01.layout.json`.
2. If the change should survive a reflash, update `qmk/keymaps/cdncampbell/keymap.c` to match.
3. If a key's meaning changed, update the cheat sheet and re-export the PDF.
4. Commit.

To put a saved layout back on the board, use **Save + Load → Load** in VIA.

## Building the QMK firmware

The keymap lives in a [qmk_firmware](https://github.com/qmk/qmk_firmware) checkout at `keyboards/doio/kb16/keymaps/cdncampbell/`. Copy it from here, then:

```sh
qmk compile -kb doio/kb16/rev2 -km cdncampbell
qmk flash   -kb doio/kb16/rev2 -km cdncampbell
```

VIA is enabled, so after flashing, whatever VIA has saved in the board's memory takes priority over the defaults in `keymap.c`.
