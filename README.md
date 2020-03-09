# Comets

Comets is an attempt at making a 3D game based on the 2D game 'Asteroids'.

## Architecture
- Asteroids are simple 3D models, floating in space. They spawn at some distance from the player and loop around when far enough behind the player, to create a compact but "open" world. (This actually makes the world into a three-torus)

## Plan:
- Generate and save asteroids as sets of triangles in model space; map them to world space using a placeholder identity matrix, and then to camera space, idem.
- Add locations to asteroids and implement the correct matrices for mapping to world space.
- Implement the camera using a fixed location and view angle, by implementing the correct matrix
- Implement camera movement
- Implement shooting, visually
- Implement collision check for shooting
- Implement asteroids splitting into smaller asteroids; with some kind of mechanism where they eventually get destroyed (perhaps just a simple counter of splits left)
- Implement some kind of model, perhaps with a very simple animation (just a rotation) that corresponds to movement
- Add asteroid generation and destruction at the ends of the map.
- Add 'moving' asteroids.
- Optimize the engine by using `GL_TRIANGLE_STRIP` instead of `GL_TRIANGLES` where possible.
- Asteroids should be stored as sets of triangles which are to be drawn using `GL_TRIANGLES`. We can expand this to also allow `GL_TRIANGLE_STRIP` later.

