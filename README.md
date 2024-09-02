# The Game Design Exquisite Corpse

This is a game design [exquisite corpse](https://en.wikipedia.org/wiki/Exquisite_corpse). You should download this code, add a game mechanic or some other small change or improvement, and submit it. This project uses C and the Raylib library.

To participate, check the [request thread](https://github.com/geajack/gdxc/issues/1). If no one has currently claimed the right to play the next "turn", the last comment in that thread will be from me, stating that the next turn is open. In that case, make a comment claiming it. You don't have to wait for approval from anyone to start coding, just submit a pull request.

## Rules

1. As stated above, make sure to declare your intent to submit the next "turn" in the [thread](https://github.com/geajack/gdxc/issues/1).
2. Code must be in pure C and use only the [Raylib](https://www.raylib.com/) library. It should compile with a single clang or gcc command. It should compile and run on Linux, Mac and Windows, but you are not expected to test on all of those yourself.
3. The game should always be playable with a mouse and keyboard only, to keep it accessible to people that don't have other hardware.
3. Graphics and sound submissions are accepted, but game mechanics and code is preferred. If you do add graphics and sound, make sure to commit any necessary files to this repo.
4. Purely technical submissions like refactors and optimization are discouraged and will probably only be allowed if they fix a significant problem, like the game not compiling on some platforms or something. You can do a little refactoring if it's in aid of an actual mechanic you're adding.
3. Submit your PR pretty quick, say within 48 hours.
4. Your submission should be an incremental change. Examples: adding a walljump to a platformer, fixing a bug, adding a boss.
5. Bugs or other problems are okay if they're minor and you either didn't notice them or just sincerely could not figure out how to fix them. Just don't submit something obviously completely broken. Slightly janky collision or something is okay.
6. Don't do anything that obviously tramples all over a previous additions. For example, if a recent submission added a powerup, your own submission shouldn't include removing that powerup entirely.
7. You are encouraged to take the game in unexpected directions! Just because the game so far looks like it belongs to one genre, that doesn't mean you can't add a mechanic usually associated with a different genre.
8. You can't play (submit code) two turns in a row.
9. Don't touch this README or the LICENSE.