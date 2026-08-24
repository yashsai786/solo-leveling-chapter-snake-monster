# Code Smell Audit: Baseline `main`

_Audit performed against baseline commit `origin/main` prior to design refactoring._
_Rules applied: Report only, exact `file:line` citations, calibrated confidence ratings (C4–C1)._

---

## Findings Catalog

| # | Smell Name | Severity | File : Line | Confidence | Description | Proposed Refactoring |
|---|------------|----------|-------------|:----------:|-------------|----------------------|
| 1 | **Shotgun Surgery** | High | `Food.h:13`, `Food.cpp:14`, `GameBoard.cpp:32` | **C4** | Food spawning accepts a single `const vector<Point>& snakeBody`. Extending the game to multiple snakes requires coordinated edits across 4 files. | Extract container parameter / pass aggregated snake segments to decouple food placement from player count. |
| 2 | **Divergent Change / Large Class** | High | `GameBoard.cpp:1-238`, `GameBoard.h:12-44` | **C4** | `GameBoard` couples console UI rendering (`gotoxy`, `drawBorder`), game loop timing (`delay`), scoring, and collision evaluation into a single class. | Extract separate ConsoleRenderer / UI class and isolate pure game state logic. |
| 3 | **Feature Envy** | Medium | `GameBoard.cpp:115-120` | **C3** | `GameBoard::update()` inspects `snake.getBody().front()` coordinates to compare against `food.getPosition()`, driving growth externally instead of encapsulating consumption logic. | Move intersection/eating check into `Snake` or `Food` entity domain methods. |
| 4 | **Data Clump** | Medium | `Common.h:12-13`, `Snake.h:11`, `Snake.cpp:15` | **C3** | Raw coordinate pairs `int x, int y` are repeatedly unpacked and passed together without vector helper operations. | Enrich `Point` struct with equality and distance operators. |
| 5 | **Hardcoded Configuration** | Low | `Common.h:6-10`, `GameBoard.cpp:18`, `GameBoard.cpp:215` | **C2** | Board dimensions (`WIDTH = 40`, `HEIGHT = 20`) and speed step formulas (`100 - (level - 1) * 10`) are hardcoded globally. | Encapsulate dimensions and difficulty curve in a configurable `GameSettings` struct. |
| 6 | **Long Method** | Low | `GameManager.cpp:45-120` | **C2** | `GameManager::displayMenu()` contains monolithic console rendering and sequential switch branches spanning >75 lines. | Extract helper functions for individual menu sub-views. |
| 7 | **Speculative Generality** | Low | `HighScoreManager.cpp:42-65` | **C1** | High score vector dynamically resizes for arbitrary unbounded record counts when UI only displays top 5. | *False Positive / C1*: Vector resizing is standard defensive C++ file I/O rather than unnecessary over-engineering. |
| 8 | **Primitive Obsession** | Low | `GameBoard.h:19`, `GameBoard.cpp:14` | **C1** | Score is stored as raw primitive `int` rather than a domain `Score` value object. | *Over-generalization / C1*: A primitive integer is fully sufficient for an arcade score counter. |

---

## Audit Summary
- **Total findings reported**: 8
- **High confidence findings (C4–C3)**: 4 (Shotgun Surgery, Divergent Change, Feature Envy, Data Clump)
- **Low confidence / candidate rejections (C2–C1)**: 4 (Hardcoded Config, Long Method, Speculative Generality, Primitive Obsession)
- **Primary architectural bottleneck**: Shotgun Surgery around single-snake coupling in `Food` and `GameBoard`.
