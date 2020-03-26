# Comets
Comets is an attempt at making a 3D game based on the 2D game 'Asteroids'.

## Architecture
- Asteroids are simple 3D models, floating in space. They spawn at some distance from the player and loop around when far enough behind the player, to create a compact but "open" world. (This actually makes the world into a three-torus)

## Plan:
- Generate new asteroids with ramping up difficulty (faster, bigger)
- Implement score tracking and display

Optional, if I still care after everything's working:
- Before doing vertex-specific collision check first try rough bounding-box collision to see if it's even necessary.
- Implement a proper sun (visual only, should be drawn out of accessible map)
- Implement newtonian movement (turning doesn't change direction of movement, just of acceleration)
- Add skybox
- Add local score saving
- Add shadows
- Add "dust" particles, the visibility of which is determined by shadows, giving a reverse sunbeam effect left by asteroids
