# Changelog

## 0.3.4 (2026-07-27)
- Line numbers displayed in a left gutter with automatic width
- Status bar shows current line and total visible lines (`Ln N / M`)
- Tab icons (system file icon)
- Filter stability fix: removed unreliable async thread, scanning is now synchronous

## 0.3.3 (2026-07-27)
- Window title bar shows the full file path of the active tab (UNC paths supported)

## 0.3.2 (2026-07-27) — reverted
- Async filter thread introduced then reverted due to stability issues

## 0.3.1 (2026-07-27)
- Added Filter menu (between View and Help)
  - Filter by keyword — show only lines containing a word or expression
  - Filter by pattern — show only lines matching a highlight rule
  - Clear filter — return to normal view
- Filter updates automatically in follow-tail mode as the file grows
- File rotation / truncation triggers a full rescan from scratch
- Status bar shows "Filter active: N matches" when a filter is on
- Search while filtering operates only on visible (filtered) lines

## 0.3.0 (2026-07-27)
- Fixed highlighting: no longer reset on scroll or tail update
- Highlighting embedded directly in the text layout loop — applied on every repaint
- Highlight rules update the view immediately without restart

## 0.2.9 (2026-07-26)
- Edit and Delete buttons in Highlighting dialog now activate on row selection
- Added case-sensitive option per highlight rule (case-insensitive by default)
- Enabled/disabled checkbox is now inline in the Pattern column
- Duplicate rules (same pattern + same case sensitivity) are rejected
- Live preview in the rule dialog reflects the current Preferences palette
- List preview also uses the current Preferences palette

## 0.2.8 (2026-07-26)
- Added keyword/phrase highlighting (Edit → Highlighting...)
- Per rule: pattern, foreground/background color, regex option, enable/disable toggle
- Rules saved to `~/.yata/highlights.yaml`
- Highlighting works in real time during tail follow
- Rule list with visual preview of each rule

## 0.2.7 (2026-07-26)
- Fixed include paths across all modules (author TODO from 0.2.0)
- Made `yFileCursor(QTextCursor)` private; added clean `fileCursorForLayoutLine()` (author TODO from 0.2.0)

## 0.2.6 (2026-07-26)
- Fixed potential crash on search with empty history
- Fixed potential UB in memory-mapped file I/O (null check on `QFile::map`)
- Fixed scrollbar inconsistency in follow-tail mode on large files
- Removed stray debug include from production code
- Replaced `QScopedPointer<bool>` with two plain fields for debug-menu flag

## 0.2.5 (2026-07-26)
- Preferences file now includes a version field
- Incompatible preferences version shows a clear warning; defaults are used

## 0.2.4 (2026-07-26)
- Corrupted or unreadable preferences file now shows an error dialog instead of silently ignoring it

## 0.2.3 (2026-07-26)
- Session restore: scroll position and follow-tail mode are saved and restored on restart
- File integrity check before restoring: FNV-32 hash of first 256 bytes + size comparison
- Any anomaly (file replaced, truncated, rotated) causes the file to open as new

## 0.2.2 (2026-07-26)
- Double-click on a word selects it

## 0.2.1 (2026-07-26)
- Updated copyright to 2010-2026 James Smith & metr0n0m
- Updated project URL to https://github.com/metr0n0m/Yata

---

## 0.2.0 (2012-02-24) — James Smith (original)
- Added GUI and storage for text color customization
- Rudimentary text selection and copying
- Fixed layout with non-default fonts
- Search fix: allow changing case of words already in search history
- Various text display / layout bug fixes

## 0.1.0 — James Smith (original)
- Initial release
