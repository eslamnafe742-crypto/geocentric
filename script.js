// sidereal periods in days, AU distance, color, eclicpt longitude at 1 Jan 2026 and moons
let planets = [
	[ 87.9691, 0.35, 'white', 242.6 ],
	[ 224.7, 0.72, 'pink', 277.4 ],
	[ 365, 1.00, 'green', 100.6, [ [ 27.32, 0.10, 'white', 70 ], ] ],
	[ 687.0, 1.45, 'red', 284.2 ],
	[ 4331, 5.0, 'blue', 109.3 ],
	//[ 10747, 9.5, 'blue', 1.9 ], // no saturn for now
];
// the ecliptic coordinate system defines 0 degrees as the position
// of Earth on the March equinox and positive rotation when viewed from
// above.
// https://ssd.jpl.nasa.gov/planets/approx_pos.html


let t = Math.random() * 1e6;
let last_t = 0;
let scale = 100;
let paused = 0;
let fixed_earth = 0;
let step = 1.0 / 24.0 / 10.0;

function setup()
{
	createCanvas(windowWidth, windowHeight);
	background(0);
	angleMode(DEGREES);
}

function draw()
{
	if (last_t == t)
	{
		if (paused)
			return;
		t += step;
	}
	last_t = t;

	background(0, 0, 0, 10);
	fill("black");
	rect(0, 0, 400, 100);
	fill("white")
	textSize(20);
	text(t.toFixed(3), 50, 50);

	let hour = 24 * (t - Math.floor(t));
	text(hour.toFixed(3), 200, 50);


	push();
	translate(windowWidth/4, windowHeight/2);
	rotate(180);

	// draw heliocentric first
	noFill();
	stroke(255,255,255);
	strokeWeight(0.5);
	for(let x = 0 ; x < 8 ; x++)
	{
		line(-8*scale, -x*scale, +8*scale, -x*scale);
		line(-8*scale, +x*scale, +8*scale, +x*scale);
		line(-x*scale, -8*scale, -x*scale, +8*scale);
		line(+x*scale, -8*scale, +x*scale, +8*scale);
	}
	noStroke();

	fill('yellow');
	circle(0,0,15);
	draw_planets(planets, 10);
	pop();

	// now draw geocentric
	push();
	let earth = planets[2]
	translate(windowWidth*3/4, windowHeight/2);

	let earth_lon = earth[3]

	if (fixed_earth)
	{
		// earth is stationary, but the sun rotates once per year
		rotate(-t / earth[0] * 360 - earth_lon);
		translate(0, -scale * earth[1]);
		rotate(t / earth[0] * 360 + earth_lon);
	} else {
		// sun rotates once per day, north is at local midnight
		rotate(+hour * 360 / 24 + 90);
		translate(-scale * earth[1], 0);
		rotate(+t / earth[0] * 360 + earth_lon);
	}

	fill('yellow');
	circle(0,0,15);
	draw_planets(planets, 10);
	pop();
	
}

function draw_planet(planet, radius)
{
	push();
	let a = -t / planet[0] * 360 - planet[3];
	let r = scale * planet[1];
	stroke(255, 255, 255, 40);
	line(0, 0, r * cos(a), r * sin(a));

	noStroke();
	rotate(a);
	translate(r, 0);
	fill(planet[2]);
	circle(0, 0, radius);


	let moons = planet[4];
	rotate(-a);
	if(moons)
		draw_planets(moons, radius/2);
	pop();
}

function draw_planets(planets, radius)
{
	for(let planet of planets)
	{
		draw_planet(planet, radius);
	}
}

function mousePressed()
{
	//background(0);
	paused = !paused;
}
