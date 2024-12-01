# Rendering Wave Effect of Light

We are not focusing on the physical fact that light is composed of different frequency component. Instead, we want to show another exciting thing about wave, which is interference.

## Theory for Implementation

The Poynting vector is a concept in electromagnetism that represents the directional energy flux (the energy transfer per unit area per unit time) or power flow of an electromagnetic field.

Comparing with current implemented light, we only need to change `vec3` to `vec4`, where the additional dimension is used for recording the phase information. When we accumulated the samples, we can just add up the result with the phase.

## Possible Points for This Project

1. Implement Poynting-based Light Transport (2pts)
2. Support Non-air Material (1pt)
3. Creation of Physical-based Scene (1pt)
4. Support Diffraction by Special Material Slit (2pts)