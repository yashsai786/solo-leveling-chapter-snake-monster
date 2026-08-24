# Code Smell Audit: Lab-1 PR Head (`feat/multiplayer`)

_Audit performed against Lab-1 PR head branch (`feat/multiplayer`) prior to design-first refactoring._
_Rules applied: Report only, exact `file:line` citations, calibrated confidence ratings (C4–C1)._

---

## Findings Catalog

| # | Smell Name | Severity | File : Line | Confidence | Description | Proposed Refactoring |
|---|------------|----------|-------------|:----------:|-------------|----------------------|
| 1 | **Duplicated Code / Parallel Fields** *(Introduced in Lab 1)* | High | `GameBoard.h:16-21`, `GameBoard.cpp:128-142` | **C4** | Snake and score state duplicated into parallel scalar variables (`snake1`, `snake2`, `score1`, `score2`) with duplicated movement, growth, and drawing logic. | Consolidate players into a container `snakes[NUM_PLAYERS]` and `scores[NUM_PLAYERS]`. |
| 2 | **Divergent Change / Large Class** *(Untouched from main)* | High | `GameBoard.cpp:1-245`, `GameBoard.h:12-46` | **C4** | `GameBoard` continues to manage rendering, game loop timing, scoring, and input handling simultaneously. | Extract separate renderer / input handler components. |
| 3 | **Feature Envy** *(Untouched from main)* | Medium | `GameBoard.cpp:130-145` | **C3** | `GameBoard::update()` directly manipulates and checks snake growth and coordinates rather than encapsulating consumption in domain entities. | Move food consumption query into `Snake` domain class. |
| 4 | **Data Clump** *(Untouched from main)* | Medium | `Common.h:12-13`, `Snake.h:11`, `Snake.cpp:15` | **C3** | Raw coordinate pairs `int x, int y` are passed and manipulated across modules without point helper methods. | Add operator overloads to `Point` struct. |
| 5 | **Hardcoded Configuration** *(Untouched from main)* | Low | `Common.h:6-10`, `GameBoard.cpp:18` | **C2** | Grid dimensions and level speed steps remain hardcoded global constants. | Encapsulate game parameters in a configurable settings struct. |
| 6 | **Long Method** *(Untouched from main)* | Low | `GameManager.cpp:45-120` | **C2** | Monolithic menu display function with repetitive print statements. | Extract helper functions for individual menu views. |
| 7 | **Speculative Generality** *(Untouched from main)* | Low | `HighScoreManager.cpp:42-65` | **C1** | Unbounded vector growth for top 5 leaderboard display. | *False Positive / C1*: Standard dynamic buffer pattern. |
| 8 | **Primitive Obsession** *(Untouched from main)* | Low | `GameBoard.h:20-21` | **C1** | Separate integer scores used instead of domain player objects. | *Over-generalization / C1*: Sufficient for simple arcade scoring. |

---

## Audit Summary
- **Total findings reported**: 8
- **Introduced by Lab 1 feature**: 1 (Duplicated Code across `snake1`/`snake2` and `score1`/`score2`)
- **Left untouched from main**: 7
- **Removed by Lab 1 feature**: 0
