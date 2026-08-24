# Ubiquitous Language — Snake Game C++

_Generated 2026-08-24. **Human-reviewed**: yes._

> **Warning**: an unreviewed, agent-authored glossary is worse than none: it becomes
> confident-sounding lore that later sessions treat as truth. Edit this file before
> committing it.

## Human Review Summary & Edits
1. **Removed invented enterprise jargon**: Stripped candidate terms like `SnakeEntity`, `BoardCanvasController`, and `ScoreService` which were AI hallucinations of class names rather than authentic domain vocabulary.
2. **Standardized Consumable**: Established `Food` as the canonical domain term for the consumable item; flagged and discarded ambiguous synonyms like `Fruit`, `Apple`, `Pellet`, and `Token`.
3. **Clarified Discrete Motion**: Defined `Tick` explicitly as the discrete game loop step with associated `delay` (in ms), resolving ambiguity between high speed level and low millisecond sleep interval.
4. **Separated Player from Snake**: Formally separated the `Player` (input source, profile name, score) from the `Snake` (physical segmented grid entity, direction, coordinates) to establish clear boundaries for multiplayer extensibility.

---

## Canonical Terms

| Term | Definition | Canonical in code | Aliases to avoid |
|------|------------|-------------------|------------------|
| **Snake** | Controllable segmented entity navigating the grid; moves continuously in a direction and grows upon eating food. | `Snake` (`Snake.h:8`) | Worm, Serpent, PlayerAvatar |
| **Segment** | An individual `(x, y)` coordinate block comprising the snake's body or head. | `Point` (`Common.h:12`, `Snake.h:11`) | BodyPart, Block, Node, Cell |
| **Food** | Collectible item positioned on the board grid that increases score and extends snake length when consumed. | `Food` (`Food.h:7`, `Food.cpp:14`) | Fruit, Apple, Pellet, Dot, Powerup |
| **Board** | The bounded 2D playfield of fixed dimensions (`WIDTH x HEIGHT`) where entities exist and interact. | `GameBoard` (`GameBoard.h:12`, `Common.h:6`) | Canvas, Arena, Grid, Screen |
| **Tick** | A single discrete step of the active game loop during which input is polled, position updates, and collision checks occur. | `delay` / game loop step (`GameBoard.cpp:24`) | Turn, Round, Cycle, Frame |
| **Collision** | An invalid intersection event between snake head and boundary wall, self body, or opponent snake body causing death. | `checkCollision` (`Snake.cpp:64`) | Crash, Hit, Death, Impact |
| **Score** | Cumulative integer value earned by consuming food during a match. | `score` (`GameBoard.h:19`) | Points, Tally, Exp |
| **High Score** | Persistent record of top player performance across sessions saved to disk storage. | `HighScoreManager` (`HighScoreManager.h:14`) | Leaderboard, Record, HallOfFame |
| **Direction** | The 4 cardinal movement vectors (`UP`, `DOWN`, `LEFT`, `RIGHT`) governing snake advancement per tick. | `Direction` (`Common.h:14`) | Heading, Orientation, Bearing |
| **Player** | The human participant controlling a snake via key bindings, associated with an active score and profile. | `Player` / `playerName` (`GameManager.h:12`) | User, Gamer, Driver |

---

## Flagged Ambiguities

1. **`Player` vs `Snake`**:
   - In baseline `origin/main`, the code conflates the player identity with the single snake instance.
   - *Resolution*: A `Player` is the agent providing input and owning score; a `Snake` is the grid entity. In multiplayer, multiple `Snake` entities coexist under separate player controls.
   - *Citations*: `GameManager.cpp:45`, `GameBoard.h:15`.

2. **`Speed` vs `Delay`**:
   - Higher game speed corresponds to a *lower* numerical millisecond sleep delay (`delay = 100 - (level - 1) * 10`).
   - *Resolution*: Canonicalize domain term to `Tick Delay` in code and `Speed Level` in UI.
   - *Citations*: `GameBoard.cpp:18`, `GameBoard.cpp:215`.

3. **`Point` overloaded across concepts**:
   - `Point` struct represents a board boundary point, a snake body coordinate, and a food spawn position.
   - *Resolution*: Keep `Point` as basic coordinate primitive, but qualify usages contextually as `Segment` (snake) or `Position` (food).
   - *Citations*: `Common.h:12`, `Food.h:11`, `Snake.h:11`.
