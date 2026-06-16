# Geocentric Orrery

Most orrerys put the sun at the center, but that's boring and doesn't
reflect what we see in the sky.  So this one has a stationary earth
in the center and everything rotates around it.  

The sun rotates once per day, dragging all of the other planets with it.
The moon rotates a little less than once per day, which creates its
phases.

# Moon gear

Since the moon gears are being rotated with the sun carrier, they
act more like planetary or epicyclic gears and the math for the
effective gear ratio is a little weird.

The carrier makes 1 rotation per day (or 15 degrees per hour).
The moon takes 29.530 days to complete its rotation relative to the
sun, which means that it is running 12.191 degrees slower per day.
This is a gear ratio of 0.03379 in the direction against the rotation
of the carrier, which can be achieved with a two stage reduction
that also preserves the rotation direction.

18:77 and 83:12 are nice since they do not share common factors
and result in a very close match for the drive ratio.


# Venus gear

