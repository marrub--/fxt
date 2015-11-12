#version 110

// Uniforms
uniform int Ticks;

// Constants
// - Math
const float pi = 3.1415926535897932384626433832795;

// - Resolution
const vec2 res = vec2(640.0, 480.0);
const vec2 wh  = vec2(256.0, 224.0);
const float w = wh.x;
const float h = wh.y;

// - Border
const float borderx = wh.x / 16.0;
const float bordery = wh.y / 8.0;

// Variables
// - Timer
float t = float(Ticks) / 32.0;

float tbx = sin(t / 8.0);
float tby = cos(t / 8.0);

float tcx = sin(t / 3.0);
float tcy = cos(t / 2.0);

float tsx = sin(t / 48.0);

// Functions
// - Main
void main()
{
	vec2 uv = (gl_FragCoord.xy / res) * wh;

	float x = uv.x;
	float y = uv.y;

	float bx;
	float by;
	float v;

	if(y < bordery)
	{
		gl_FragColor = vec4(0.0, 0.0, (1.0 - (y / bordery)) * 0.5, 1.0);
		return;
	}

	if(y >= h - bordery)
	{
		gl_FragColor = vec4(0.0, 0.0, (1.0 - ((h - y) / bordery)) * 0.5, 1.0);
		return;
	}

	if(x < borderx || x >= w - borderx)
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		return;
	}

	bx = x + 0.5 * tbx;
	by = y + 0.5 * tby;

	v = sin((-bx + t) / 32.0);
	v += cos((by + t) / 32.0);
	v += sin((-bx + by + t) / 32.0);
	v += sin((sqrt(pow(bx + tcx, 2.0) + pow(by + tcy, 2.0) + 1.0) + t) / 128.0);

	gl_FragColor.r = sin(v * pi) * 0.5 + 0.5;
	gl_FragColor.g = cos(v * pi) * 0.5 + 0.5;
	gl_FragColor.g *= 0.6;
	gl_FragColor.b = abs(tsx * 0.25);
	gl_FragColor.a = 0.0;
}

