# Geocentric Orrery

Most orrerys put the sun at the center, but that's boring and doesn't
reflect what we see in the sky.  So this one has a stationary earth
in the center and everything rotates around it.  

The sun rotates once per day, dragging all of the other planets with it.
The moon rotates a little less than once per day, which creates its
phases.

The Moon, Mercury and Venus appear to rotate counter clockwise
around the sun at the same time of each day.  They run slightly slower
than the carrier.

Mars, Jupiter and Saturn appear to rotate clockwise around the sun
when viewed at the same time each day.  They run slightly faster than
the carrier.

# Epicyclic gears

With normal gears the ratio is easy to compute: N2/N1.
The way to think about it is that an N tooth gear rotating at w
speed will have a tangental velocity proportional to N * w.
Since the two gears are meshed without slipping the two tangental
speeds must be identical, but with opposite signs since the
second gear is spinning opposite the first, so for a given
input speed w1, N1 * w1 = -N2 * w2 and the output speed w2 = -N1/N2 * w1.

However, epicyclic or planetary gears are weird.  In addition
to the two gears, called the sun and the planet, there is also the
speed of the carrier the planet gears are mounted to to take into
consideration, as well as sometimes a ring gear.

In general one of the sun, planet, carrier or ring will
be held stationary and some of the four will be driven as inputs.
The problem is to compute the speeds of the outputs.

In general there are three equations that relate the sun, carrier,
planet and ring.  The stationary element has a zero velocity.

```
Ns Ws + Np Wp = (Ns + Np) Wc	# Wr = 0
Nr Wr - Np Wp = (Nr - Np) Wc	# Ws = 0
Ns Ws + Nr Wr = (Ns + Nr) Wc	# Wp = 0
```

Note that if the carrier is fixed, Wc = 0, and the first equation
becomes the normal gear equation:

```
Ns Ws + Np Wp = (Ns + Np) * 0
Ns Ws = -Np Wp
```

# Moon gear

Since the moon is being rotated with the sun carrier, they
are epicyclic gears.

The moon takes 29.530 days to complete its rotation relative to the sun
(synodic period), which means that it moves 347.809 degrees per day,
12.191 degrees slower than the sun per day.  This 347/360 is hard to
achieve with a single stage, so a two-stage step gear box is used.

The first stage has a stationary 19 tooth sun gear (Ns=19, Ws=0)
and 100 tooth planet (Np=100). The carrier is driven at 1 turn per day (Wc=1),
so the resulting planetary speed is:

```
Ns Ws + Np Wp = (Ns + Np) Wc
Wp = (Ns + Np) / Np * Wc
   = (19 + 100) / 100 * 1
   = 1.19
```

The second stage has a small planetary gear (Np=18) fused and coaxial
with the first stage planetary gear, turning a larger
sun gear (called the "moon wheel", Np=101).  Since they are all on a
common carrier, Wc is still 1.  Solving for Ws this time:

```
Ns Ws + Np Wp = (Ns + Np) Wc
Ws = ((Ns + Np) Wc - Np Wp) / Ns
   = (Ns Wc + Np Wc - Np Wp) / Ns
   = (Ns Wc) / Ns + (Wc - Wp) Np / Ns
   = Wc + (Wc - Wp) Np / Ns
   = 1 + (1 - 1.19) 18 / 101
   = 0.9662
   = 347.83 / 360
```

This is quite close to the desired value given the integer number of
teeth and the range of sizes allowed.


# Mercury and Venus gears

There are another two stages for this reduction.  The first is similar
to the moon wheel; an 17 tooth planteary driving a 102 tooth sun:



```
Ns Ws + Np Wp = (Ns + Np) Wc
Ws = ((Ns + Np) Wc - Np Wp) / Ns
   = 1 + (1 - 1.19) 17 / 102
   = 0.9684
```

Then there is a second stage reduction using a fused sun gear
and another planetary.  But this time Ws != 0, it is the value from
the first stage output.  Ns = 27, Np = 92

```
Ns Ws + Np Wp = (Ns + Np) Wc
Wp = ((Ns + Np) * Wc - Ns Ws) / Np
   = (Ns Wc + Np Wc - Ns Ws) / Np
   = Ns / Np (Wc - Ws) + Wc
   = 27 / 92 (1 - 0.9684) + 1
   = 1.0092
```

Fused atop this planetary gear are two smaller gears,
called the 'Inner Planet cake', which drive other spur gears
for the Planets.  It is not a ring gear, since it is also
rigid on the carrier with the planet gear, so normal gear
math applies.


Mercury's sidereal period is 87.9 days, but again we need to use the
synodic period of 115.88 days so that it retains its relative position
to the Earth-Sun, not the fixed stars.  Because the sidereal is faster
than the Earth's orbit the synodic is going slower than the carrier, which
means no reverser gear and that every day it is 360 / 115.88 = 3.1066 degrees
slower than the sun.  Thus the desired gear ratio is 356.8934:360 or 0.9913.

```
N2 w2 = -N1 w1
   w2 = -w1 N1/N2
      = -1.0092 * 65 / 70 + 1
      = -1.0085 + 1
      = +0.0085
```


Venus' sidereal is 224.7 days, but a synodic of 583.9 days.  This is a very
small step down from the carrier speed of just 1.5997 degrees per day,
or a 0.00171 ratio.  Using the Mercury slow down gear


# Mars, Jupiter and Saturn gears

Mars appears to rotate the other at the slowest of all at 779.9
synodic days.  The sidereal year is longer than Earth's, so it appears
to be gaining 0.024 degrees per day.

Jupiter (4331 days) and Saturn (10747 days) sidereal years are so
slow that their synodic year is very close to one Earth year,
398.9 and 378.1 days. This means that Jupiter gains 0.0303 degrees
per day and Saturn 0.0763.

Since these three are gaining on the carrier, they need
a reversing gear from the venus/mercury gear train.  Another way to
achieve it would be with a sector gear, especially for Saturn, since it
won't complete a rotation for a very long time.
