# Comets
Comets is an attempt at making a 3D game based on the 2D game 'Asteroids'.

## Architecture
- Asteroids are simple 3D models, floating in space. They spawn at some distance from the player and loop around when far enough behind the player, to create a compact but "open" world. (This actually makes the world into a three-torus)

## Plan:
- Make asteroids spawn in more random locations
- Allow change of flight direction to up and down
- Implement newtonian movement (turning doesn't change direction of movement, just of acceleration)
- Make asteroids more complex shapes
- Implement a proper sun (visual only, should be drawn out of accessible map)
- Fix shooting direction
- Delete bullets at the edge of the map
- Collision check for shooting
- Make asteroids split into smaller asteroids when hit; with some kind of mechanism where they eventually get destroyed (perhaps just a simple counter of splits left)
- Generate new asteroids with ramping up difficulty (faster, bigger)
- Implement score tracking and display

Optional, if I still care after everything's working:
- Add skybox
- Add local score saving
- Add shadows
- Add "dust" particles, the visibility of which is determined by shadows, giving a reverse sunbeam effect left by asteroids
