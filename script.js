
// sidereal periods in days, AU distance, color and moons
let planets = [
	[ 87.9691, 0.35, 'white' ],
	[ 224.7, 0.72, 'brown' ],
	[ 365.25635, 1.00, 'green', [ [ 27.32, 0.10, 'white' ], ] ],
	[ 678.0, 1.45, 'red' ],
	[ 4331, 5.0, 'yellow' ],
	[ 10747, 9.5, 'blue' ],
];

let t = Math.random() * 1e6;
let scale = 50;
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
	background(0, 0, 0, 10);
	fill("black");
	rect(0, 0, 400, 100);
	fill("white")
	textSize(20);
	text(t.toFixed(3), 50, 50);

	let hour = t - Math.floor(t);
	text(hour.toFixed(3), 200, 50);

	if (!paused)
		t += step;

	push();
	translate(windowWidth/4, windowHeight/2);

	// draw heliocentric first
	fill('yellow');
	circle(0,0,15);
	draw_planets(planets, 10);
	pop();

	// now draw geocentric
	push();
	let earth = planets[2]
	translate(windowWidth*3/4, windowHeight/2);

	if (fixed_earth)
	{
		// earth is stationary, but the sun rotates once per year
		rotate(-t / earth[0] * 360);
		translate(0, -scale * earth[1]);
		rotate(t / earth[0] * 360);
	} else {
		// sun rotates once per day
		rotate(hour * 360);
		rotate(-t / earth[0] * 360);
		translate(0, -scale * earth[1]);
		rotate(t / earth[0] * 360);
	}

	fill('yellow');
	circle(0,0,15);
	draw_planets(planets, 10);
	pop();
	
}

function draw_planet(planet, radius)
{
	push();
	rotate(-t / planet[0] * 360);
	translate(0, scale * planet[1]);
	fill(planet[2]);
	circle(0, 0, radius);

	let moons = planet[3];
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
