# Lab 2_3 Report — Detect Code Smell and Design-First Refactoring

## 1. Tool and Install Route

- **Tool**: Gemini Agentic Assistant / Antigravity IDE
- **Install Route**: Installed skills directly into `.agents/skills/` directory (`ubiquitous-language`, `refactoring/detect-code-smells`, and `refactoring/review-accuracy-calibration`). The agent loaded and read the skill instruction files on demand.

---

## 2. Ubiquitous Language: Human Edits

During human review of the generated glossary, we made four key editorial decisions:
1. **Removed Hallucinated Class Jargon**: Deleted artificial terms like `BoardCanvasController` and `SnakeEntity` to keep the glossary focused strictly on domain terminology.
2. **Canonicalized `Food`**: Eliminated ambiguous synonyms (`Fruit`, `Apple`, `Pellet`, `Dot`) in favor of canonical `Food`.
3. **Clarified `Tick` and `Speed`**: Defined `Tick` as the discrete game loop step with associated millisecond delay, resolving inverted meaning between speed level and sleep duration.
4. **Decoupled `Player` from `Snake`**: Formally separated `Player` (input source, profile, score) from `Snake` (grid coordinate entity) to prepare the domain model for multiplayer extension.

---

## 3. Code Smell Audit and Delta Table

### Delta Table

| Category | Count | Smells Cited |
|----------|:-----:|--------------|
| **Introduced by Lab 1** | 1 | `Duplicated Code / Parallel Fields` (`GameBoard.h:16-21`, `GameBoard.cpp:128-142`) |
| **Left Untouched from main** | 7 | `Divergent Change` (`GameBoard.cpp:1-245`), `Feature Envy` (`GameBoard.cpp:130-145`), `Data Clump` (`Common.h:12-13`), `Hardcoded Configuration` (`Common.h:6-10`), `Long Method` (`GameManager.cpp:45-120`), `Speculative Generality` (`HighScoreManager.cpp:42-65`), `Primitive Obsession` (`GameBoard.h:20-21`) |
| **Removed by Lab 1** | 0 | None (Lab 1 feature implementation did not eliminate any existing architectural smells) |

---

## 4. Rejected Smell Candidates

| # | Smell Reported | File : Line | Reason for Rejection |
|---|----------------|-------------|----------------------|
| 1 | **Speculative Generality** | `HighScoreManager.cpp:42-65` | The dynamic vector sizing in `HighScoreManager` is standard defensive C++ file I/O capacity handling rather than unneeded premature abstraction. |
| 2 | **Primitive Obsession** | `GameBoard.h:19`, `GameBoard.cpp:14` | Storing score as a primitive integer is completely appropriate for a lightweight arcade game and wrapping it in a value object creates unnecessary indirection. |
| 3 | **Large Class** | `GameBoard.h:12-44` | Although `GameBoard` has multiple responsibilities, in a 200-LOC console arcade game, splitting it into micro-services adds boilerplate complexity without tangible maintainability benefits. |

---

## 5. Commit Table

| # | SHA | Message | Files changed | Lines added | Lines removed |
|---|-----|---------|---------------|-------------|---------------|
| 1 | `904a693` | lab2_3/UBIQUITOUS_LANGUAGE.md: add human-reviewed domain glossary | 1 | +49 | -0 |
| 2 | `6532c1d` | lab2_3/audits/main.md: audit code smells on main baseline | 1 | +27 | -0 |
| 3 | `873eee2` | refactor: consolidate snake count into snakes[NUM_PLAYERS] — single source of truth | 6 | +151 | -103 |
| 4 | `4a2a7ab` | feat: implement two-player mode using decoupled player configuration | 3 | +70 | -61 |

---

## 6. Two-Run Measurement Table

| Metric | Run 1 (Lab 1 — Ad-hoc Feature) | Run 2 (Lab 2 — Design-First Commit 4 alone) |
|--------|:------------------------------:|:-------------------------------------------:|
| **Feature Files Changed** | 8 | 3 |
| **Feature Lines Added** | 218 | 70 |
| **Feature Lines Removed** | 57 | 61 |
| **Feature Total LOC Diff** | 275 | 131 |
| **Structural Refactor Diff (Commit 3)** | 0 (No separate refactor) | 254 (6 files changed, +151 / -103) |
| **Smells Introduced** | 1 (`Duplicated Code`) | 0 |

---

## 7. Analysis: Q1 and Q2

### Q1: Which smell did commit 3 actually fix?
Commit 3 addressed **Shotgun Surgery** (`Food.h:13`, `GameBoard.cpp:32`) and prevented **Duplicated Code / Parallel Fields**. In baseline `main`, adding a second snake required modifying `Food::spawn`, snake storage, drawing, and collision in multiple separate places. Commit 3 established `NUM_PLAYERS` and the `snakes[NUM_PLAYERS]` array as the single source of truth and generalized `Food::spawn` to iterate over all active snake segments. Consequently, extending the game to two players in Commit 4 required changing only the configuration table and input bindings in a single module without touching `Food` or `Snake` internals.

### Q2: Compare commit 4 to your Lab-1 diff. What changed in the cost, and what did not?
In Lab 1, implementing multiplayer required changing 8 files (275 LOC diff) because feature additions were coupled with ad-hoc structural modifications, resulting in duplicated snake pointers (`snake1`, `snake2`) and duplicate scoring fields. In Run 2, because Commit 3 absorbed the structural reorganization, Commit 4 touched only 3 files (131 LOC diff). The marginal feature cost was cut by more than half (131 vs 275 LOC), and zero architectural smells were introduced. What remained unchanged was the inherent domain requirement for two-player keyboard event dispatching and collision rule evaluation.

---

## 8. Analysis: Q3 and Q4

### Q3: Did the assistant suggest restructuring before adding the feature in Lab 1?
In Lab 1, the LLM assistant did not suggest any prior structural refactoring; it immediately jumped into adding parallel variables (`snake1`, `snake2`, `score1`, `score2`) within `GameBoard`. The prompt directly instructed the model to "convert it into a 2-player multiplayer game", so the agent optimized for the shortest path to feature completion. For the assistant to propose design refactoring upfront, the prompt would need an explicit requirement such as: *"First evaluate architectural extensibility and refactor existing structures for multi-entity support before implementing player controls."*

### Q4: How do you know commit 3 did not change behaviour?
We verified Commit 3 by compiling cleanly with `g++ -std=c++17` and running single-player manual gameplay checks (verifying snake movement, food spawning, boundary collisions, and score incrementing). However, this represents an empirical confidence level rather than formal mathematical proof. Because the repository lacks automated unit and regression test suites, manual verification cannot guarantee complete absence of edge-case regressions (e.g. rare coordinate spawn collisions). True verification would require dedicated regression test fixtures asserting board invariant states.
