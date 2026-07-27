# Yata — Yet Another Tail Application

A GUI log viewer for Windows — tail, search, keyword highlighting and line filtering.

**This is a fork** of the original [Yata by James Smith](https://github.com/josmith42/yata),
which was released as open source (GPL) in 2010–2012 and has not been updated since.
The original author and their work are fully credited — see [LICENSE](license.txt).

---

## What's new in this fork (0.3.x)

- **Keyword highlighting** — assign colors to words or phrases; they light up in the log stream in real time (Edit → Highlighting)
- **Line filter** — show only lines matching a keyword or a highlight pattern (Filter menu)
- **Line numbers** — absolute line numbers in a left gutter
- **Session restore** — scroll position and follow-tail mode are restored on restart; file integrity is verified before restoring (rotation/truncate handled gracefully)
- **Status bar** — current line / total lines; active filter indicator
- **Full path in title bar** — each tab shows the complete file path including UNC paths (`\\server\share\...`)
- **Double-click word selection**, **tab icons**, and a number of bug fixes and code cleanups inherited from the original TODO list

See [CHANGELOG.md](CHANGELOG.md) for the full version history.

---

## Building

### Requirements

- Qt 4.8.7 with MinGW 4.8.2 (tested; see [original readme](readme.txt) for Qt 5 notes)
- yaml-cpp 0.2.6 — source at https://github.com/jbeder/yaml-cpp (tag `release-0.2.6`)
- CMake (to build yaml-cpp)

### Windows (MinGW)

```
qmake YAMLCPP=<path-to-yaml-cpp> -spec win32-g++
mingw32-make release
```

By default qmake looks for yaml-cpp one directory above the source tree.
If you installed it elsewhere, pass the path explicitly:

```
qmake YAMLCPP=C:/yaml-cpp -spec win32-g++
```

### Linux / macOS

```
qmake YAMLCPP=<path-to-yaml-cpp>
make
sudo make install
```

---

## Configuration files

Yata stores its settings in `~/.yata/`:

| File | Contents |
|---|---|
| `preferences.yaml` | Font, text colors |
| `session.yaml` | Open files, scroll positions, search history |
| `highlights.yaml` | Keyword highlighting rules |

---

## Original project

- Author: James Smith (josmith42)
- Original repository: https://github.com/josmith42/yata
- License: GNU General Public License — see [license.txt](license.txt)

This fork does not claim ownership of the original code.
All original copyright notices are preserved in every source file.
